//
// Created by kvrmnks on 2021/3/19.
//
#ifndef __SEG__
#define __SEG__
#include "SegmentTree.h"
#include <algorithm>
#include <iostream>
using namespace std;
#define REP(i,j,k) for(int i=j;i<=k;i++)

SegmentTree::SegmentTree() {
    this->root = this->newNode();
}

Node* SegmentTree::newNode() {
    Node *ret = new Node();
    ret->ch[0] = ret->ch[1] = nullptr;
    ret->cu[0] = ret->cu[1] = 0;
    ret->mm[0] = ret->mm[1] = 0;
    ret->mmtag[0] = ret->mmtag[1] = 0;
    ret->cutag[0] = ret->cutag[1] = 0;
    return ret;
}

void SegmentTree::pushup(Node *x) {
    Node *l = x->ch[0];
    Node *r = x->ch[1];
    x->mm[0] = x->mm[1] = x->cu[0] = x->cu[1] = 0;
    if(l != nullptr){
        for(int i=0;i<2;i++){
            x->mm[i] = max(x->mm[i], l->mm[i]);
            x->cu[i] = max(x->cu[i], l->cu[i]);
        }
    }
    if(r != nullptr){
        for(int i=0;i<2;i++){
            x->mm[i] = max(x->mm[i], r->mm[i]);
            x->cu[i] = max(x->cu[i], r->cu[i]);
        }
    }
}

void SegmentTree::pushdown(Node *x) {
    Node *&l = x->ch[0];
    Node *&r = x->ch[1];
    if(l == nullptr){
        l = this->newNode();
        REP(i,0,1){
            l->mm[i] = x->mm[i] - x->mmtag[i];
            l->cu[i] = x->cu[i] - x->cutag[i];
        }
    }
    if(r == nullptr){
        r = this->newNode();
        REP(i,0,1){
            r->mm[i] = x->mm[i] - x->mmtag[i];
            r->cu[i] = x->cu[i] - x->cutag[i];
        }
    }
    REP(i,0,1){
        l->cutag[i] += x->cutag[i];
        r->cutag[i] += x->cutag[i];

        l->cu[i] += x->cutag[i];
        r->cu[i] += x->cutag[i];

        l->mmtag[i] += x->mmtag[i];
        r->mmtag[i] += x->mmtag[i];

        l->mm[i] += x->mmtag[i];
        r->mm[i] += x->mmtag[i];
    }
    REP(i,0,1){
        x->mmtag[i] = x->cutag[i] = 0;
    }
}

DataPackage SegmentTree::_query(Node *x, int L, int R, int l, int r) {
    pushdown(x);
    if(l==L && r==R){
        return DataPackage{x->mm[0], x->mm[1], x->cu[0], x->cu[1]};
    }
    int mid = (L+R)>>1;
    if(r<=mid){
        return _query(x->ch[0], L, mid, l, r);
    }else if(l > mid){
        return _query(x->ch[1], mid+1, R, l, r);
    }else{
        DataPackage pa = _query(x->ch[0], L, mid, l, mid);
        DataPackage pb = _query(x->ch[1], mid+1, R, mid+1, r);
        REP(i,0,1){
            pa.mm[i] = max(pa.mm[i], pb.mm[i]);
            pa.cu[i] = max(pa.cu[i], pb.cu[i]);
        }
        return pa;
    }
}

DataPackage SegmentTree::query(int L, int R, int l, int r) {
    return _query(root, L, R, l, r);
}

void SegmentTree::_modify(Node *x, int L, int R, int l, int r, int *delta) {
    pushdown(x);
    if(l == L && r == R){
        REP(i,0,1){
            x->mm[i] += delta[i];
            x->mmtag[i] += delta[i];
            x->cu[i] += delta[i + 2];
            x->cutag[i] += delta[i + 2];
        }
        pushdown(x);
        return;
    }
    int mid = (L + R) >> 1;
    if(r <= mid){
        _modify(x->ch[0], L, mid, l, r, delta);
    }else if(l > mid){
        _modify(x->ch[1], mid+1, R, l, r, delta);
    }else{
        _modify(x->ch[0], L, mid, l, mid, delta);
        _modify(x->ch[1], mid+1, R, mid+1, r, delta);
    }
    pushup(x);
}

void SegmentTree::modify(int L, int R, int l, int r, int *delta) {
    _modify(root, L, R, l, r, delta);
}

void SegmentTree::clear() {
    this->_clear(this->root);
}

void SegmentTree::_clear(Node *x) {
    if(x == nullptr){return;}
    Node *l = x->ch[0];
    Node *r = x->ch[1];
    if(l != nullptr){
        _clear(l);
        delete l;
    }
    if(r != nullptr){
        _clear(r);
        delete(r);
    }
}

SegmentTree::~SegmentTree() {
    this->clear();
}


#undef REP
#endif