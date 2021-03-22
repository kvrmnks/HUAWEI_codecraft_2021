//
// Created by kvrmnks on 2021/3/19.
//

#ifndef PROJECT_SEGMENTTREE_H
#define PROJECT_SEGMENTTREE_H
#define NODEA 0
#define NODEB 1
#include <iostream>
using namespace std;
// 节点的数据结构
struct Node{
    int mm[2], cu[2];
    Node *ch[2];
    int mmtag[2], cutag[2];
};

// 用来返回查询结果
struct DataPackage{
    int mm[2], cu[2];
    friend ostream& operator<<(ostream& o, DataPackage &b){
        o << b.mm[0] << " " << b.mm[1] << " " << b.cu[0] << " " << b.cu[1] << endl;
        return o;
    }
};

//线段树
class SegmentTree{
private:
    Node *root;
    void pushup(Node *x);
    void pushdown(Node *x);
    Node *newNode();
    void _clear(Node *x);
    DataPackage _query(Node *x, int L, int R, int l, int r);
    void _modify(Node *x, int L,int R,int l,int r, int delta[4]);
public:
    SegmentTree();
    // L, R代表定义域, L>=1, l,r表示查询区间，delta表示区间加的数值
    // delta mm[0] mm[1] cu[0] cu[1]
    void modify(int L,int R, int l,int r, int delta[4]);
    DataPackage query(int L,int R,int l,int r);
    void clear();
    ~SegmentTree();
};

#endif //PROJECT_SEGMENTTREE_H

/*
    使用教程
    定义
        SegmentTree tree;
    修改
        int delta[4] = {0, 1, 23 ,4};
        tree.modify(L, R, l, r, delta);
    查询
        DataPackage dp = tree.query(L, R, l, r);
 */