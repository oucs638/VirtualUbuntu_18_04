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
  vector<int> vec;
} int4;
typedef struct {
  vector<bool> vec;
} bool4;

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
    chk.resize(blkNm);
    for (int i = 0; i < blkNm; i++) chk[i] = false;
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
      }  // ascty==0 check if tgrt empty
    }    // ascty==0 rdng wrd
  } else if (ascty == 1) {
    blkNm = ((cchSz * 1024) / blkSz) / 4;
    oftNm = (int)log2((double)blkSz);
    idxNm = (int)log2((double)blkNm);
    tagNm = 32 - oftNm - idxNm;
    int tpidx = -1;
    vector<int4> tag;
    vector<bool4> chk;
    tag.resize(blkNm);
    chk.resize(blkNm);
    for (int i = 0; i < blkNm; i++) {
      tag[i].vec.resize(4);
      chk[i].vec.resize(4);
      for (int j = 0; j < 4; j++) chk[i].vec[j] = false;
    }
    if (rlcmt == 0) {
      while (frd >> hex >> tmpwd) {
        tmpbt = hexToBinary(tmpwd);
        tmptg = bstTagToInt(tmpbt, tagNm, 31);
        tmpit = bstTagToInt(tmpbt, idxNm, 31 - tagNm);
        tpidx = -1;
        for (int i = 0; i < 4; i++) {
          if (!chk[tmpit].vec[i])
            break;
          else {
            tpidx = i;
          }
        }
        if (tpidx == -1) {
          tag[tmpit].vec[0] = tmptg;
          chk[tmpit].vec[0] = true;
          fwt << -1 << endl;
        } else if (tpidx == 3) {
          tpidx = -1;
          for (int i = 0; i < 4; i++)
            if (tag[tmpit].vec[i] == tmptg) {
              tpidx = i;
              break;
            }
          if (tpidx != -1)
            fwt << -1 << endl;
          else {
            fwt << tag[tmpit].vec[0] << endl;
            tag[tmpit].vec.erase(tag[tmpit].vec.begin());
            tag[tmpit].vec.push_back(tmptg);
          }
        } else {
          int tptpidx = -1;
          for(int i = 0; i <= tpidx; i++)
            if(tag[tmpit].vec[i]==tmptg){
              tptpidx=i;
              break;
            }
          if(tptpidx!=-1)
            fwt << -1 << endl;
          else {
            fwt << -1 << endl;
            tag[tmpit].vec[tpidx + 1] = tmptg;
            chk[tmpit].vec[tpidx + 1] = true;
          }
        }
      }
    } else if (rlcmt == 1) {
      while (frd >> hex >> tmpwd) {
        tmpbt = hexToBinary(tmpwd);
        tmptg = bstTagToInt(tmpbt, tagNm, 31);
        tmpit = bstTagToInt(tmpbt, idxNm, 31 - tagNm);
        tpidx = -1;
        for (int i = 0; i < 4; i++) {
          if (!chk[tmpit].vec[i])
            break;
          else
            tpidx = i;
        }
        if (tpidx == -1) {
          tag[tmpit].vec[0] = tmptg;
          chk[tmpit].vec[0] = true;
          fwt << -1 << endl;
        } else if (tpidx == 3) {
          tpidx = -1;
          for (int i = 0; i < 4; i++)
            if (tag[tmpit].vec[i] == tmptg) {
              tpidx = i;
              break;
            }
          if(tpidx!=-1){
            fwt << -1 << endl;
            tag[tmpit].vec.erase(tag[tmpit].vec.begin() + (tpidx - 1));
            tag[tmpit].vec.push_back(tmptg);
          } else {
            fwt << tag[tmpit].vec[0] << endl;
            tag[tmpit].vec.erase(tag[tmpit].vec.begin());
            tag[tmpit].vec.push_back(tmptg);
          }
        } else {
          int tptpidx = -1;
          for (int i = 0; i <= tpidx; i++)
            if (tag[tmpit].vec[i] == tmptg) {
              tptpidx = i;
              break;
            }
          if (tptpidx != -1) {
            fwt << -1 << endl;
            for (int i = tptpidx; i < tpidx; i++)
              tag[tmpit].vec[i] = tag[tmpit].vec[i + 1];
            tag[tmpit].vec[tpidx] = tmptg;
          } else {
            fwt << -1 << endl;
            tag[tmpit].vec[tpidx + 1] = tmptg;
            chk[tmpit].vec[tpidx + 1] = true;
          }
        }
      }
    } else if (rlcmt == 2) {
      while (frd >> hex >> tmpwd) {
        tmpbt = hexToBinary(tmpwd);
        tmptg = bstTagToInt(tmpbt, tagNm, 31);
        tmpit = bstTagToInt(tmpbt, idxNm, 31 - tagNm);
        tpidx = -1;
        for (int i = 0; i < 4; i++) {
          if (!chk[tmpit].vec[i])
            break;
          else
            tpidx = i;
        }
        if (tpidx == -1) {
          tag[tmpit].vec[0] = tmptg;
          chk[tmpit].vec[0] = true;
          fwt << -1 << endl;
        } else if (tpidx == 3) {
          tpidx = -1;
          for (int i = 0; i < 4; i++)
            if (tag[tmpit].vec[i] == tmptg) {
              tpidx = i;
              break;
            }
          if (tpidx != -1) {
            fwt << -1 << endl;
            if (tpidx != 3) {
              int tmp = tag[tmpit].vec[tpidx];
              tag[tmpit].vec[tpidx] = tag[tmpit].vec[tpidx + 1];
              tag[tmpit].vec[tpidx + 1] = tmp;
            }
          } else {
            fwt << tag[tmpit].vec[0] << endl;
            tag[tmpit].vec.erase(tag[tmpit].vec.begin());
            tag[tmpit].vec.push_back(tmptg);
          }
        } else {
          int tptpidx = -1;
          for (int i = 0; i <= tpidx; i++)
            if (tag[tmpit].vec[i] == tmptg) {
              tptpidx = i;
              break;
            }
          if (tptpidx != -1) {
            fwt << -1 << endl;
            if (tptpidx != tpidx) {
              int tmp = tag[tmpit].vec[tptpidx];
              tag[tmpit].vec[tptpidx] = tag[tmpit].vec[tptpidx + 1];
              tag[tmpit].vec[tptpidx + 1] = tmp;
            }
          } else {
            fwt << -1 << endl;
            tag[tmpit].vec[tpidx + 1] = tmptg;
            chk[tmpit].vec[tpidx + 1] = true;
          }
        }
      }
    }  // end of rlcmt
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
    chk.resize(lbkNm);
    for (int i = 0; i < lbkNm; i++) chk[i] = false;
    if (rlcmt == 0) {
      while (frd >> hex >> tmpwd) {
        tpidx = -1;
        tmpbt = hexToBinary(tmpwd);
        tmptg = bstTagToInt(tmpbt, tagNm, 31);
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
        } else if (tpidx == (lbkNm - 1)) {
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
          for (int i = 0; i <= tpidx; i++)
            if (tag[i] == tmptg) {
              tptpidx = i;
              break;
            }
          if (tptpidx != -1)
            fwt << -1 << endl;
          else {
            fwt << -1 << endl;
            tag[tpidx + 1] = tmptg;
            chk[tpidx + 1] = true;
          }
        }
      }
    } else if (rlcmt == 1) {
      while (frd >> hex >> tmpwd) {
        tpidx = -1;
        tmpbt = hexToBinary(tmpwd);
        tmptg = bstTagToInt(tmpbt, tagNm, 31);
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
        } else if (tpidx == (lbkNm - 1)) {
          tpidx = -1;
          for (int i = 0; i < lbkNm; i++)
              if (tag[i] == tmptg) {
                tpidx = i;
                break;
              }
          if (tpidx != -1){
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
          for (int i = 0; i <= tpidx; i++)
              if (tag[i] == tmptg) {
                tptpidx = i;
                break;
              }
          if (tptpidx != -1) {
            fwt << -1 << endl;
            for (int i = tptpidx; i < tpidx; i++)
              tag[i] = tag[i + 1];
            tag[tpidx] = tmptg;
          } else {
            fwt << -1 << endl;
            tag[tpidx + 1] = tmptg;
            chk[tpidx + 1] = true;
          }
        }
      }
    } else if (rlcmt == 2) {
      while (frd >> hex >> tmpwd) {
        tpidx = -1;
        tmpbt = hexToBinary(tmpwd);
        tmptg = bstTagToInt(tmpbt, tagNm, 31);
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
        } else if (tpidx == (lbkNm - 1)) {
          tpidx = -1;
          for (int i = 0; i < lbkNm; i++)
            if (tag[i] == tmptg) {
              tpidx = i;
              break;
            }
          if (tpidx != -1) {
            fwt << -1 << endl;
            if (tpidx != (lbkNm - 1)) {
              int tmp = tag[tpidx];
              tag[tpidx] = tag[tpidx + 1];
              tag[tpidx + 1] = tmp;
            }
          } else {
            fwt << tag[0] << endl;
            tag.erase(tag.begin());
            tag.push_back(tmptg);
          }
        } else {
          int tptpidx = -1;
          for (int i = 0; i <= tpidx; i++)
            if (tag[i] == tmptg) {
              tptpidx = i;
              break;
            }
          if (tptpidx != -1) {
            fwt << -1 << endl;
            if (tptpidx != tpidx) {
              int tmp = tag[tptpidx];
              tag[tptpidx] = tag[tptpidx + 1];
              tag[tptpidx + 1] = tmp;
            }
          } else {
            fwt << -1 << endl;
            tag[tpidx + 1] = tmptg;
            chk[tpidx + 1] = true;
          }
        }
      }
    }  // end of rlcmt
  }    // end of ascty

  frd.close();
  fwt.close();
  return 0;
}  // end of main

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