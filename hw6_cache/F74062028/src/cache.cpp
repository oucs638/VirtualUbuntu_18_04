#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <math.h>

using namespace std;
// typedef bitset<32> bit32;

bitset<32> hextobinary(string num);
int bit32tagtoint(bitset<32> num, int bitnum, int start);

int main(int argc, char *argv[])
{

    fstream rdfile, wtfile;
    rdfile.open(argv[1], ios::in);
    wtfile.open(argv[2], ios::out | ios::trunc);
    if (!rdfile)
    {
        cout << "ERROR: CANNOT FIND THE FILE" << endl;
        return 1;
    }

    int cacheSize, blockSize, associativity, replacement;
    int blockNum, offsetNum, indexNum, tagNum, tmpint;
    vector<bitset<32> > addrs;
    string addr;
    bitset<32> tmp;

    rdfile >> cacheSize;
    rdfile >> blockSize;
    rdfile >> associativity;
    rdfile >> replacement;

    if (associativity == 0)
    {
        blockNum = (cacheSize * 1024) / blockSize;
        // cout << blockNum << endl;
        offsetNum = (int)log2((double)blockSize);
        indexNum = (int)log2((double)blockNum);
        tagNum = 32 - offsetNum - indexNum;
        int tags[blockNum][1];
        bitset<32> data[blockNum][1];
        bool chck[blockNum][1];
        for (int i = 0; i < blockNum; i++)
            chck[i][0] = false;
        if (replacement == 0)
        {
            while (rdfile >> hex >> addr)
            {
                tmp = hextobinary(addr);
                tmpint = bit32tagtoint(tmp, indexNum, 31 - tagNum);
                // cout << tmpint << endl;
                if (!chck[tmpint][0])
                {
                    tags[tmpint][0] = bit32tagtoint(tmp, tagNum, 31);
                    data[tmpint][0] = tmp;
                    chck[tmpint][0] = true;
                    wtfile << -1 << endl;
                }
                else
                {
                    if (tags[tmpint][0] == bit32tagtoint(tmp, tagNum, 31))
                        wtfile << -1 << endl;
                    else
                    {
                        wtfile << tags[tmpint][0] << endl;
                        tags[tmpint][0] = bit32tagtoint(tmp, tagNum, 31);
                        data[tmpint][0] = tmp;
                    }
                }
            }
        }
        else if (replacement == 1)
        {
            while (rdfile >> hex >> addr)
            {
                tmp = hextobinary(addr);
                tmpint = bit32tagtoint(tmp, indexNum, 31 - tagNum);
                cout << tmpint << endl;
                if (!chck[tmpint][0])
                {
                    tags[tmpint][0] = bit32tagtoint(tmp, tagNum, 31);
                    data[tmpint][0] = tmp;
                    chck[tmpint][0] = true;
                    wtfile << -1 << endl;
                }
                else
                {
                    if (tags[tmpint][0] == bit32tagtoint(tmp, tagNum, 31))
                        wtfile << -1 << endl;
                    else
                    {
                        wtfile << tags[tmpint][0] << endl;
                        tags[tmpint][0] = bit32tagtoint(tmp, tagNum, 31);
                        data[tmpint][0] = tmp;
                    }
                }
            }
        }
        else if (replacement == 2)
        {
        }
    }
    else if (associativity == 1)
    {
        blockNum = ((cacheSize * 1024) / blockSize) / 4;
        offsetNum = (int)log2((double)blockSize);
        indexNum = (int)log2((double)blockNum);
        tagNum = 32 - offsetNum - indexNum;
        int tag[blockNum][4];
        bitset<32> data[blockNum][4];
        bool check[blockNum][4];
        for (int i = 0; i < blockNum; i++)
            for (int j = 0; j < 4; j++)
                check[i][j] = false;
        if (replacement == 0)
        {
        }
        else if (replacement == 1)
        {
        }
        else if (replacement == 2)
        {
        }
    }
    else if (associativity == 2)
    {
        blockNum = 1;
        offsetNum = (int)log2((double)blockSize);
        indexNum = (int)log2((double)blockNum);
        tagNum = 32 - offsetNum - indexNum;
        int longBlockNum = (cacheSize * 1024) / blockSize;
        int tags[blockNum][longBlockNum];
        bitset<32> data[blockNum][longBlockNum];
        bool chck[blockNum][longBlockNum];
        // cout << blockNum << endl;
        // cout << longBlockNum << endl;
        for (int i = 0; i < longBlockNum; i++)
            chck[0][i] = false;
        int tmpindex = -1;
        if (replacement == 0)
        {
            tmpindex = -1;
            while (rdfile >> hex >> addr)
            {
                tmp = hextobinary(addr);
                for (int i = 0; i < longBlockNum; i++)
                {
                    if (!chck[0][i])
                        break;
                    else
                        tmpindex = i;
                }
                if (tmpindex == -1)
                {
                    tags[0][0] = bit32tagtoint(tmp, tagNum, 31);
                    data[0][0] = tmp;
                    chck[0][0] = true;
                    wtfile << -1 << endl;
                }
                else if (tmpindex == 127)
                {
                    tmpindex = -1;
                    int tmptag = bit32tagtoint(tmp, tagNum, 31);
                    for (int i = 0; i < longBlockNum; i++)
                        if (tags[0][i] == tmptag)
                        {
                            tmpindex = i;
                            break;
                        }
                    if (tmpindex == -1)
                    {
                        wtfile << tags[0][0] << endl;
                        for (int i = 0; i < (longBlockNum - 1); i++)
                        {
                            tags[0][i] = tags[0][i + 1];
                            data[0][i] = data[0][i + 1];
                        }
                        tags[0][longBlockNum - 1] = tmptag;
                        data[0][longBlockNum - 1] = tmp;
                    }
                    else
                    {
                        wtfile << -1 << endl;
                    }
                }
                else
                {
                    int tmptmpIndex = -1;
                    int tmptag = bit32tagtoint(tmp, tagNum, 31);
                    for (int i = 0; i <= tmpindex; i++)
                        if (tags[0][i] == tmptag)
                        {
                            tmptmpIndex = i;
                            break;
                        }
                    if (tmptmpIndex == -1)
                    {
                        tags[0][tmpindex + 1] = bit32tagtoint(tmp, tagNum, 31);
                        data[0][tmpindex + 1] = tmp;
                        chck[0][tmpindex + 1] = true;
                        wtfile << -1 << endl;
                    }
                    else
                    {
                        wtfile << -1 << endl;
                    }
                }
                // tmpint = bit32tagtoint(tmp, indexNum, 31 - tagNum);
            }
        }
        else if (replacement == 1)
        {
            while (rdfile >> hex >> addr)
            {
                tmp = hextobinary(addr);
                for (int i = 0; i < longBlockNum; i++)
                {
                    if (!chck[0][i])
                        break;
                    else
                        tmpindex = i;
                }
                if (tmpindex == -1)
                {
                    tags[0][0] = bit32tagtoint(tmp, tagNum, 31);
                    data[0][0] = tmp;
                    chck[0][0] = true;
                    wtfile << -1 << endl;
                }
                else if (tmpindex == 127)
                {
                    tmpindex = -1;
                    int tmptag = bit32tagtoint(tmp, tagNum, 31);
                    for (int i = 0; i < longBlockNum; i++)
                        if (tags[0][i] == tmptag)
                        {
                            tmpindex = i;
                            break;
                        }
                    if (tmpindex == -1)
                    {
                        wtfile << tags[0][0] << endl;
                        for (int i = 0; i < (longBlockNum - 1); i++)
                        {
                            tags[0][i] = tags[0][i + 1];
                            data[0][i] = data[0][i + 1];
                        }
                        tags[0][longBlockNum - 1] = tmptag;
                        data[0][longBlockNum - 1] = tmp;
                    }
                    else
                    {
                        wtfile << -1 << endl;
                        bitset<32> tmpdata = data[0][tmpindex];
                        for (int i = tmpindex; i < longBlockNum; i++)
                        {
                            tags[0][i] = tags[0][i + 1];
                            data[0][i] = data[0][i + 1];
                        }
                        tags[0][longBlockNum - 1] = tmptag;
                        data[0][longBlockNum - 1] = tmpdata;
                    }
                }
                else
                {
                    int tmptmpIndex = -1;
                    int tmptag = bit32tagtoint(tmp, tagNum, 31);
                    for (int i = 0; i <= tmpindex; i++)
                        if (tags[0][i] == tmptag)
                        {
                            tmptmpIndex = i;
                            break;
                        }
                    if (tmptmpIndex == -1)
                    {
                        tags[0][tmpindex + 1] = bit32tagtoint(tmp, tagNum, 31);
                        data[0][tmpindex + 1] = tmp;
                        chck[0][tmpindex + 1] = true;
                        wtfile << -1 << endl;
                    }
                    else
                    {
                        wtfile << -1 << endl;
                        bitset<32> tmpdata = data[0][tmptmpIndex];
                        for (int i = tmptmpIndex; i < tmpindex; i++)
                        {
                            tags[0][i] = tags[0][i + 1];
                            data[0][i] = data[0][i + 1];
                        }
                        tags[0][tmpindex] = tmptag;
                        data[0][tmpindex] = tmpdata;
                    }
                }
                // tmpint = bit32tagtoint(tmp, indexNum, 31 - tagNum);
            }
        }
        else if (replacement == 2)
        {
        }
    }

    rdfile.close();
    wtfile.close();
}

bitset<32> hextobinary(string num)
{
    stringstream tmp;
    tmp << hex << num;
    unsigned n;
    tmp >> n;
    bitset<32> b(n);
    return b;
}

int bit32tagtoint(bitset<32> num, int bitnum, int start)
{
    int result = 0;
    for (int i = 0; i < bitnum; i++)
        result += num[start - i] * (int)pow((double)2, (double)(bitnum - 1 - i));
    return result;
}
