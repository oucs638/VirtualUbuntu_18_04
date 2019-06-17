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
  int entryCount;  // number of entries in a set.
  int setCount;  // number of set in the cache.
  
  switch (associative) {
    case 0:  // Direct-mapped
      setCount = blockCount;
      entryCount = 1;
      break;
    case 1:  // Four-way set
      setCount = blockCount / 4;
      entryCount = 4;
      indexLength -= 2;
      tagLength += 2;
      break;
    case 2:  // Fully
      setCount = 1;
      entryCount = blockCount;
      tagLength += indexLength;
      indexLength = 0;
      break;
  }
  
  //cout << tagLength << " " << indexLength << " " << offsetLength << endl;
  
  unsigned int tag[setCount][entryCount];  // tag of an enrty in the set.
  int valid[setCount][entryCount];  // whether an enrty in the set has data or not.
  int FIFOidx[setCount];
  int LRUidx[setCount][entryCount];
  int LRUmax[setCount];
  
  for (int i = 0; i < setCount; i++) {
    int LRUidx[setCount][entryCount];
    int LRUmax[setCount];
    for (int j = 0; j < entryCount; j++) {
      tag[i][j] = 0;
      valid[i][j] = 0;
      LRUidx[i][j] = -1;
    }
  }
  
  int test = 0;
  string strIn;
  while (fileIn >> strIn) {
    test++;
    unsigned int addr;
    stringstream ss;
    ss << std::hex << strIn;
    ss >> addr;
    
    unsigned int addrTag = addr >> offsetLength >> indexLength;
    unsigned int addrIndex = ((unsigned int)(addr << tagLength)) >> tagLength >> offsetLength;
    
    //cout << addrTag << " " << addrIndex << endl;
    
    if (test == 2571) {
      test = test;
    } else {
      test = test;
    }
    
    for (int i = 0; i < entryCount; i++) {
      
      if (valid[addrIndex][i] && tag[addrIndex][i] == addrTag) {  // hit
        fileOut << -1 << endl;
        LRUidx[addrIndex][i] = ++LRUmax[addrIndex];
        break;
      } else if (!valid[addrIndex][i]) {  // invalid && miss
        fileOut << -1 << endl;
        valid[addrIndex][i] = 1;
        tag[addrIndex][i] = addrTag;
        FIFOidx[addrIndex] = ++FIFOidx[addrIndex] % entryCount;
        LRUidx[addrIndex][i] = ++LRUmax[addrIndex];
        break;
      } else if (i == entryCount - 1) {  // miss && replace
        int way = 0;
        if (entryCount == 1) {  // direct-mapped replace
          way = 0;
        } else if (policy == 0) {  // FIFO
          way = FIFOidx[addrIndex] = ++FIFOidx[addrIndex] % entryCount;
        } else if (policy == 1) {  // LRU
          for (int j = 0; j < entryCount; j++) {
            if (LRUidx[addrIndex][j] < LRUidx[addrIndex][way])
              way = j;
          }
          LRUidx[addrIndex][way] = ++LRUmax[addrIndex];
        } else if (policy == 2) {  // Random
          way = rand() % entryCount;
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
