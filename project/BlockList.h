//
// Created by kvrmnks on 2021/3/23.
//

#ifndef PROJECT_BLOCKLIST_H
#define PROJECT_BLOCKLIST_H
#include <iostream>
#include <vector>
#include "SegmentTree.h"
using namespace std;
const int SIZE_PER_BLOCK = 320;
class BlockList{
private:
    vector<DataPackage> data[SIZE_PER_BLOCK];
    DataPackage delta[SIZE_PER_BLOCK];
    DataPackage maxV[SIZE_PER_BLOCK];
public:
    BlockList();
    void modify(int L, int R, DataPackage ds);
    DataPackage query(int L, int R);

};

BlockList::BlockList() {
    // a ba a ba
    for(auto & i : data){
        i = vector<DataPackage>(SIZE_PER_BLOCK);
    }
}

void BlockList::modify(int L, int R, DataPackage ds) {
    int loc1 = L / SIZE_PER_BLOCK;
    int loc2 = R / SIZE_PER_BLOCK;
    int real_l = L - loc1 * SIZE_PER_BLOCK;
    int real_r = R - loc2 * SIZE_PER_BLOCK;
    if(loc1 == loc2){
        for(int i=real_l; i<=real_r;i++){
            data[loc1][i] =  data[loc1][i] + ds;
            maxV[loc1] = bigger(maxV[loc1], data[loc1][i] + delta[loc1]);
        }
    }else{
        for(int i=loc1+1;i<=loc2-1;i++){
            delta[i] = delta[i] + ds;
            maxV[i] = maxV[i] + ds;
        }
        for(int i=real_l;i<SIZE_PER_BLOCK;i++){
            data[loc1][i] = data[loc1][i] + ds;
            maxV[loc1] = bigger(maxV[loc1], data[loc1][i] + delta[loc1]);
        }
        for(int i=0;i<=real_r;i++){
            data[loc2][i] = data[loc2][i] + ds;
            maxV[loc2] = bigger(maxV[loc2], data[loc2][i] + delta[loc2]);
        }
    }
}

DataPackage BlockList::query(int L, int R) {
    DataPackage t;
    int loc1 = L / SIZE_PER_BLOCK;
    int loc2 = R / SIZE_PER_BLOCK;
    int real_l = L - loc1 * SIZE_PER_BLOCK;
    int real_r = R - loc2 * SIZE_PER_BLOCK;
    if(loc1 == loc2){
        for(int i=real_l;i<=real_r;i++){
            t = bigger(t, delta[loc1] + data[loc1][i]);
        }
    }else{
        for(int i=real_l;i<SIZE_PER_BLOCK;i++){
            t = bigger(t, delta[loc1] + data[loc1][i]);
        }
        for(int i=0;i<=real_r;i++){
            t = bigger(t, delta[loc2] + data[loc2][i]);
        }
        for(int i=loc1+1;i<=loc2-1;i++){
            t = bigger(t, maxV[i]);
        }
    }
    return t;
}

#endif //PROJECT_BLOCKLIST_H
