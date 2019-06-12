#include <math.h>
#include <bitset>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
typedef bitset<32> bst;

bst hexToBinary(string str);
int bstTagToInt(bst tag, int rng, int bgn);

int main(int argc, char* argv[]) {
  fstream frd, fwt;
  frd.open(argv[1], ios::in);
  fwt.open(argv[2], ios::out | ios::trunc);

  if (!frd) {
    cout << "ERROR: CANNOT FIND THE FILE" << endl;
    return 1;
  }

  int cchSz, blkSz, ascty, rlcmt;
  int blkNm, oftNm, idxNm, tagNm, tmpit, tmptg;
  string tmpwd;
  bst tmpbt;

  frd >> cchSz;
  frd >> blkSz;
  frd >> ascty;
  frd >> rlcmt;

  if (ascty == 0) {
    blkNm = (cchSz * 1024) / blkSz;
    oftNm = (int)log2((double)blkSz);
    idxNm = (int)log2((double)blkNm);
    tagNm = 32 - oftNm - idxNm;
    vector<int> tag;
    vector<bool> chk;
    tag.resize(blkNm);
    for (int i = 0; i < blkNm; i++) chk.push_back(false);
    while (frd >> hex >> tmpwd) {
      tmpbt = hexToBinary(tmpwd);
      tmptg = bstTagToInt(tmpbt, tagNm, 31);
      tmpit = bstTagToInt(tmpbt, idxNm, 31 - tagNm);
      if (!chk[tmpit]) {
        tag[tmpit] = tmptg;
        chk[tmpit] = true;
        fwt << -1 << endl;
      } else {
        if (tag[tmpit] == tmptg)
          fwt << -1 << endl;
        else {
          fwt << tag[tmpit] << endl;
          tag[tmpit] = tmptg;
        }
      }
    }
    // if (rlcmt == 0) {

    // } else if (rlcmt == 1) {

    // } else if (rlcmt == 2) {

    // }
  } else if (ascty == 1) {
    if (rlcmt == 0) {
    } else if (rlcmt == 1) {
    } else if (rlcmt == 2) {
    }
  } else if (ascty == 2) {
    blkNm = 1;
    oftNm = (int)log2((double)blkSz);
    idxNm = (int)log2((double)blkNm);
    tagNm = 32 - oftNm - idxNm;
    int lbkNm = (cchSz * 1024) / blkSz;
    int tpidx = -1;
    vector<int> tag;
    vector<bool> chk;
    tag.resize(lbkNm);
    for (int i = 0; i < lbkNm; i++) chk.push_back(false);
    if (rlcmt == 0) {
      tpidx = -1;
      while (frd >> hex >> tmpwd) {
        tmpbt = hexToBinary(tmpwd);
        tmptg = bstTagToInt(tmpbt, tagNm, 31);
        // tmpit = bstTagToInt(tmpbt, idxNm, 31 - tagNm);
        for (int i = 0; i < lbkNm; i++) {
          if (!chk[i])
            break;
          else
            tpidx = i;
        }
        if (tpidx == -1) {
          tag[0] = tmptg;
          chk[0] = true;
          fwt << -1 << endl;
        } else if (tpidx == 127) {
          tpidx = -1;
          for (int i = 0; i < lbkNm; i++)
            if (tag[i] == tmptg) {
              tpidx = i;
              break;
            }
          if (tpidx != -1)
            fwt << -1 << endl;
          else {
            fwt << tag[0] << endl;
            tag.erase(tag.begin());
            tag.push_back(tmptg);
          }
        } else {
          int tptpidx = -1;
          tmptg = bstTagToInt(tmpbt, tagNm, 31);
          for (int i = 0; i < tpidx; i++)
            if (tag[i] == tmptg) {
              tptpidx = i;
              break;
            }
          if (tpidx != -1)
            fwt << -1 << endl;
          else {
            fwt << -1 << endl;
            tag[tptpidx + 1] = tmptg;
            chk[tptpidx + 1] = true;
          }
        }
      }
    } else if (rlcmt == 1) {
      tpidx = -1;
      while (frd >> hex >> tmpwd) {
        tmpbt = hexToBinary(tmpwd);
        tmptg = bstTagToInt(tmpbt, tagNm, 31);
        // tmpit = bstTagToInt(tmpbt, idxNm, 31 - tagNm);
        for (int i = 0; i < lbkNm; i++) {
          if (!chk[i])
            break;
          else
            tpidx = i;
        }
        if (tpidx == -1) {
          tag[0] = tmptg;
          chk[0] = true;
          fwt << -1 << endl;
        } else if (tpidx == 127) {
          tpidx = -1;
          for (int i = 0; i < lbkNm; i++)
            if (tag[i] == tmptg) {
              tpidx = i;
              break;
            }
          if (tpidx != -1) {
            fwt << -1 << endl;
            tag.erase(tag.begin() + (tpidx - 1));
            tag.push_back(tmptg);
          } else {
            fwt << tag[0] << endl;
            tag.erase(tag.begin());
            tag.push_back(tmptg);
          }
        } else {
          int tptpidx = -1;
          tmptg = bstTagToInt(tmpbt, tagNm, 31);
          for (int i = 0; i < tpidx; i++)
            if (tag[i] == tmptg) {
              tptpidx = i;
              break;
            }
          if (tptpidx != -1) {
            fwt << -1 << endl;
            tag.erase(tag.begin() + (tptpidx - 1));
            tag.push_back(tmptg);
          } else {
            fwt << -1 << endl;
            tag[tptpidx + 1] = tmptg;
            chk[tptpidx + 1] = true;
          }
        }
      }
    } else if (rlcmt == 2) {
    }
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
