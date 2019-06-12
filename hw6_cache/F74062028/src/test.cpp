#include <math.h>
#include <bitset>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

typedef bitset<32> bst;
typedef struct {
  bst wd;
  int sc;
} pck;

bst hexToBinary(string str);
int bstTagToInt(bst tag, int rng, int bgn);

int main(int argc, char* argv[]) {
  fstream frd, fwt;
  frd.open(argv[1], ios::in);
  fwt.open(argv[2], ios::out | ios::trunc);

  int cchSz, blkSz, ascty, rlcmt;
  vector<pck> pckgp;
  string tmpwd;
  bst tmpbt;

  frd >> cchSz;
  frd >> blkSz;
  frd >> ascty;
  frd >> rlcmt;

  while (frd >> hex >> tmpwd) {
    tmpbt = hexToBinary(tmpwd);
    if (pckgp.empty()) {
      pck tmp;
      tmp.wd = tmpbt;
      tmp.sc = 1;
      pckgp.push_back(tmp);
    } else {
      bool flag = false;
      for (int i = 0; i < pckgp.size(); i++) {
        if (pckgp[i].wd == tmpbt) {
          pckgp[i].sc += 1;
          flag = true;
          break;
        }
      }
      if (!flag) {
        pck tmp;
        tmp.wd = tmpbt;
        tmp.sc = 1;
        pckgp.push_back(tmp);
      }
    }
  }

  for (int i = 0; i < pckgp.size(); i++) {
    cout << pckgp[i].wd << " " << pckgp[i].sc << endl;
  }
  frd.close();
  fwt.close();
  return 0;
}

bst hexToBinary(string str) {
  stringstream tmp;
  unsigned num;
  tmp << hex << str;
  tmp >> num;
  bst rst(num);
  return rst;
}

int bstTagToInt(bst tag, int rng, int bgn) {
  int rst = 0;
  for (int i = 0; i < rng; i++)
    rst += tag[bgn - i] * (int)pow((double)2, (double)(rng - 1 - i));
  return rst;
}