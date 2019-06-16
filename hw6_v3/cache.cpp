#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <bitset> 
#include <cstdlib>
#include <ctime>

using namespace std;

int main(int argc, char *argv[]) {
  
  srand(time(NULL));
  
  string traceIn = argv[1];
  string traceOut = argv[2];
  
  int cacheSize, blockSize;
  int associative, policy;
  
  ifstream fileIn(traceIn);
  ofstream fileOut(traceOut);
  
  fileIn >> cacheSize >> blockSize;
  fileIn >> associative >> policy;
  
  int blockCount, tmp;
  tmp = blockCount = cacheSize * 1024 / blockSize;
  int indexLength = 0;
  while (tmp >>= 1) ++indexLength;
  int offsetLength = 0;
  while (blockSize >>= 1) ++offsetLength;
  
  int tagLength = 32 - offsetLength - indexLength;
  int wayCount;
  
  switch (associative) {
    case 0:  // Direct-mapped
      wayCount = 1;
      break;
    case 1:  // Four-way set
      wayCount = 4;
      blockCount /= 4;
      indexLength -= 2;
      tagLength += 2;
      break;
    case 2:  // Fully
      wayCount = blockCount;
      blockCount = 1;
      tagLength += indexLength;
      indexLength = 0;
      break;
  }
  
  //cout << tagLength << " " << indexLength << " " << offsetLength << endl;
  
  unsigned int tag[blockCount][wayCount] = { {0} };
  int valid[blockCount][wayCount] = { {0} };
  
  string strIn;
  while (fileIn >> strIn) {
    
    unsigned int addr;
    stringstream ss;
    ss << std::hex << strIn;
    ss >> addr;
    
    unsigned int addrTag = addr >> offsetLength >> indexLength;
    unsigned int addrIndex = ((unsigned int)(addr << tagLength)) >> tagLength >> offsetLength;
    
    //cout << addrTag << " " << addrIndex << endl;
    
    int FIFOidx[blockCount] = {-1};
    int LRUidx[blockCount][wayCount] = { {-1} };
    int LRUmax[blockCount] = {-1};
    for (int i = 0; i < wayCount; i++) {
      
      if (valid[addrIndex][i] && tag[addrIndex][i] == addrTag) {  // hit
        fileOut << -1 << endl;
        LRUidx[addrIndex][i] = ++LRUmax[addrIndex];
        break;
      } else if (!valid[addrIndex][i]) {  // invalid && miss
        fileOut << -1 << endl;
        valid[addrIndex][i] = 1;
        tag[addrIndex][i] = addrTag;
        FIFOidx[addrIndex] = ++FIFOidx[addrIndex] % wayCount;
        LRUidx[addrIndex][i] = ++LRUmax[addrIndex];
        break;
      } else if (i == wayCount - 1) {  // miss && replace
        int way = 0;
        if (wayCount == 1) {  // direct-mapped replace
          way = 0;
        } else if (policy == 1) {  // FIFO
          way = FIFOidx[addrIndex] = ++FIFOidx[addrIndex] % wayCount;
        } else if (policy == 2) {  // LRU
          for (int j = 0; j < wayCount; j++) {
            if (LRUidx[addrIndex][j] < LRUidx[addrIndex][way])
              way = j;
          }
          LRUidx[addrIndex][way] = ++LRUmax[addrIndex];
        } else if (policy == 3) {  // Random
          way = rand() % wayCount;
        }
        fileOut << tag[addrIndex][way] << endl;
        tag[addrIndex][way] = addrTag;
      }
    }
    
    
  }
  
  fileIn.close();
  fileOut.close();
  
  return 0;
}
