//
// Created by kvrmnks on 2021/3/19.
//

#ifndef PROJECT_SEGMENTTREE_H
#define PROJECT_SEGMENTTREE_H
#define NODEA 0
#define NODEB 1

// 节点的数据结构
struct Node{
    int mm[2], cu[2];
    Node *ch[2];
    int mmtag[2], cutag[2];
};

// 用来返回查询结果
struct DataPackage{
    int mm[2], cu[2];
};

//线段树
class SegmentTree{
private:
    Node *root;
    void pushup(Node *x);
    void pushdown(Node *x);
    Node *newNode();
    DataPackage _query(Node *x, int L, int R, int l, int r);
    void _modify(Node *x, int L,int R,int l,int r, int delta[4]);
public:
    SegmentTree();
    // L, R代表定义域, L>=1, l,r表示查询区间，delta表示区间加的数值
    // delta mm[0] mm[1] cu[0] cu[1]
    void modify(int L,int R, int l,int r, int delta[4]);
    DataPackage query(int L,int R,int l,int r);

};

#endif //PROJECT_SEGMENTTREE_H
