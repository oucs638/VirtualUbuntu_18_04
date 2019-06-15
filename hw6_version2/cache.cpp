#include <math.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int main(int argc, char *argv[]) {
  fstream filerd, filewt;
  filerd.open(argv[1], ios::in);
  filewt.open(argv[2], ios::out | ios::trunc);

  int cacheSize, blockSize, associativity, replacement;
  int blocks, sets, tagbits, indexbits, tmptag, tmpindex;
  vector<vector<int> > tag;
  string tmpstr;

  filerd >> cacheSize;
  filerd >> blockSize;
  filerd >> associativity;
  filerd >> replacement;

  blocks = (cacheSize * 1024) / blockSize;
  sets = 1;
  if (associativity == 1) {
    blocks /= 4;
    sets = 4;
  }
  tag.resize(blocks);
  if (associativity == 2) {
    blocks = 1;
    sets = (cacheSize * 1024) / blockSize;
  }
  indexbits = (int)log2((double)blocks);
  tagbits = 32 - indexbits - (int)log2((double)blockSize);

  while (filerd >> hex >> tmpstr) {
    stringstream tmp;
    unsigned tmpnum1, tmpnum2;
    tmp << hex << tmpstr;
    tmp >> tmpnum1;
    tmpnum2 = tmpnum1 << tagbits;
    tmptag = tmpnum1 >> (32 - tagbits);
    tmpindex = tmpnum2 >> (32 - indexbits);
    if (associativity == 2) tmpindex = 0;
    if (tag[tmpindex].size() == 0) tag[tmpindex].push_back(tmptag);
    vector<int>::iterator it =
        find(tag[tmpindex].begin(), tag[tmpindex].end(), tmptag);
    if (it != tag[tmpindex].end()) {
      filewt << -1 << endl;
      if (replacement == 1) {
        tag[tmpindex].erase(it);
        tag[tmpindex].push_back(tmptag);
      } else if (replacement == 2) {
        reverse(it, tag[tmpindex].begin());
      }
    } else {
      if (tag[tmpindex].size() == sets) {
        filewt << tag[tmpindex][0] << endl;
        tag[tmpindex].erase(tag[tmpindex].begin());
        tag[tmpindex].push_back(tmptag);
      } else {
        filewt << -1 << endl;
        tag[tmpindex].push_back(tmptag);
      }
    }
  }

  filerd.close();
  filewt.close();
  return 0;
}