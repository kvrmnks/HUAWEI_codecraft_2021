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
    int search_by_restriction(int L, int R, DataPackage dp);
    std::pair<int,int> search_by_restriction_single(int L,int R,DataPackage dp);
};

BlockList::BlockList() {
    // a ba a ba
    for(auto & i : data){
        i = vector<DataPackage>(SIZE_PER_BLOCK);
        for(auto &j : i){
            j = DataPackage();
        }
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
        exit(-1);
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

bool zgc_greater(DataPackage &a, DataPackage &b){
    return
           a.cu[0] >= b.cu[0]
        && a.cu[1] >= b.cu[1]
        && a.mm[0] >= b.mm[0]
        && a.mm[1] >= b.mm[1];
}

int BlockList::search_by_restriction(int L, int R, DataPackage dp) {
    int loc1 = L / SIZE_PER_BLOCK;
    int loc2 = R / SIZE_PER_BLOCK;
    int real_l = L - loc1 * SIZE_PER_BLOCK;
    int real_r = R - loc2 * SIZE_PER_BLOCK;
    if(loc1 == loc2){
        for(int i=real_l;i<=real_r;i++){
            if(zgc_greater(data[loc1][i], dp)){
                return i + loc1 * SIZE_PER_BLOCK;
            }
        }
    }else{
        for(int i=real_l;i<SIZE_PER_BLOCK;i++){
            if(zgc_greater(data[loc1][i], dp)){
                return i + loc1 * SIZE_PER_BLOCK;
            }
        }
        for(int i=0;i<=real_r;i++){
            if(zgc_greater(data[loc2][i], dp)){
                return i + loc2 * SIZE_PER_BLOCK;
            }
        }
        for(int i=loc1+1;i<=loc2-1;i++){
            if(zgc_greater(maxV[i], dp)){
                for(int j=0;j<SIZE_PER_BLOCK;j++){
                    if(zgc_greater(data[i][j], dp)){
                        return j + i * SIZE_PER_BLOCK;
                    }
                }
            }
        }

    }
    return -1;
}

std::pair<int, int> BlockList::search_by_restriction_single(int L, int R, DataPackage dp) {
    int loc1 = L / SIZE_PER_BLOCK;
    int loc2 = R / SIZE_PER_BLOCK;
    int real_l = L - loc1 * SIZE_PER_BLOCK;
    int real_r = R - loc2 * SIZE_PER_BLOCK;
    if(loc1 == loc2){
        for(int i=real_l;i<=real_r;i++){
            if(data[loc1][i].cu[0] >= dp.cu[0] && data[loc1][i].mm[0] >= dp.mm[0]){
                return std::make_pair(0, i + loc1 * SIZE_PER_BLOCK);
            }
            if(data[loc1][i].cu[1] >= dp.cu[1] && data[loc1][i].mm[1] >= dp.mm[1]){
                return std::make_pair(1, i + loc1 * SIZE_PER_BLOCK);
            }
        }
    }else{
        for(int i=real_l;i<SIZE_PER_BLOCK;i++){
            if(data[loc1][i].cu[0] >= dp.cu[0] && data[loc1][i].mm[0] >= dp.mm[0]){
                return std::make_pair(0, i + loc1 * SIZE_PER_BLOCK);
            }
            if(data[loc1][i].cu[1] >= dp.cu[1] && data[loc1][i].mm[1] >= dp.mm[1]){
                return std::make_pair(1, i + loc1 * SIZE_PER_BLOCK);
            }
        }
        for(int i=0;i<=real_r;i++){
            if(data[loc2][i].cu[0] >= dp.cu[0] && data[loc2][i].mm[0] >= dp.mm[0]){
                return std::make_pair(0, i + loc2 * SIZE_PER_BLOCK);
            }
            if(data[loc2][i].cu[1] >= dp.cu[1] && data[loc2][i].mm[1] >= dp.mm[1]){
                return std::make_pair(1, i + loc2 * SIZE_PER_BLOCK);
            }
        }
        for(int i=loc1+1;i<=loc2-1;i++){
            if((maxV[i].cu[0]>=dp.cu[0]&&maxV[i].mm[0]>=maxV[i].mm[0]) ||
                    (maxV[i].cu[1]>=dp.cu[1] && maxV[i].mm[1]>=dp.mm[1])){
                for(int j=0;j<SIZE_PER_BLOCK;j++){
                    if(data[i][j].cu[0] >= dp.cu[0] && data[i][j].mm[0] >= dp.mm[0]){
                        return std::make_pair(0, j + i * SIZE_PER_BLOCK);
                    }
                    if(data[i][j].cu[1] >= dp.cu[1] && data[i][j].mm[1] >= dp.mm[1]){
                        return std::make_pair(1, j + i * SIZE_PER_BLOCK);
                    }
                }
            }
        }

    }
    return std::pair<int, int>(-1, -1);
}

#endif //PROJECT_BLOCKLIST_H
