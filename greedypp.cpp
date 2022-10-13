#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <cstring>
#include <sstream>
#include <bits/stdc++.h>
#include <math.h>
#include "heap.h"
#include "dinic.h"
using namespace std;


int heapExtractMinl(pair <float, int> heap[], int &heapSize, int heapPos[], vector <pair <int, float>> adj[], int indicateGraph[], long double l[]){
    float minStrength = heap[1].first;
    int minID = heap[1].second;
    l[minID] = minStrength;
    heap[1] = heap[heapSize];
    heapSize = heapSize - 1;
    heapPos[minID] = -1;
    indicateGraph[minID] = 0;
    heapPos[heap[1].second] = 1;
    minHeapify(1, heap, heapSize, heapPos);
    vector <pair <int, float>> adjacent = adj[minID];
    float outW = 0;
    for (int i=0; i<int(adjacent.size()); i++){
        pair <int, float> adjPair = adjacent[i];
        if (indicateGraph[adjPair.first] > 0) {
            heapDecreaseKey(heap, heapPos, adjPair.first, adjPair.second);
            outW += adjPair.second;
        }
    }
    return outW;
}


float greedyPlusPlus(){
    clock_t start, finish;
    double duration;
    ifstream infile;
    infile.open("D:\\research\\2020-6-12\\data\\test.txt", ios::in);
    int count=0;
    int V = 16726;
    vector <pair <int, float>> * adj = new vector <pair <int, float>> [V+1];
    float * strength = new float[V+1];
    for (int i=0; i<=V; i++){
        strength[i] = 0;
    }
    float vertexS[V+1];
    for (int i=1; i<=V; i++) {
        vertexS[i] = 0;
    }
    float totalW=0;


    string line;
    char c[20];
    char* num_str;
    stringstream s;
    int n1;
    int n2;
    float w;

    while (getline(infile, line)){
        if (count > 142759) break;

        strcpy(c, line.c_str());

        num_str = strtok(c, " ");
        s << num_str;
        s >> n1;
        s.clear();
        s.str("");


        num_str = strtok(NULL, " ");
        s << num_str;
        s >> n2;
        s.clear();
        s.str("");


        num_str = strtok(NULL, " ");
        s << num_str;
        s >> w;
        s.clear();
        s.str("");


        if (n1 < n2){
            adj[n1].push_back(make_pair(n2, w));
            adj[n2].push_back(make_pair(n1, w));
            strength[n1] += w;
            strength[n2] += w;
            totalW += w;
        } else if (n1 == n2){
            strength[n1] += w;
            vertexS[n1] += w;
            totalW += w;
        }
    }

    float maxS=0;
    for (int i=0; i<=V; i++){
        if (strength[i] > maxS){
            maxS = strength[i];
        }
    }


    int * indicateGraph = new int[V+1];
    for (int i=1; i <= V; i++){
        indicateGraph[i] = 1;
    }

    int vertexNum = 0;
    for (int i=1; i<=V; i++){
        if (indicateGraph[i] > 0){
            vertexNum += 1;
        }
    }

    start = clock();
    int T = 100;
    int cp=1;
    int heapSize=vertexNum;
    int optSize = vertexNum;
    float originTotalW = totalW;
    float optDen = totalW/heapSize;
    long double * l = new long double [V+1]{0};
    int* heapPos = new int[V+1]{0};
    int* originIndicateG = new int[V+1]{0};
    for (int i=1; i<=V; i++){
        originIndicateG[i] = indicateGraph[i];
    }
    pair <float, int>* heap = new pair <float, int>[heapSize+1];

    for (int t=1; t<=T; t++){
        totalW = originTotalW;
        heapSize = vertexNum;
        optDen = totalW/heapSize;
        cp = 1;
        for (int i=1; i<=V; i++){
            indicateGraph[i] = originIndicateG[i];
        }

        for (int i=1; i<=V; i++) {
            if (indicateGraph[i] > 0){
                heap[cp] = make_pair(strength[i]+l[i], i);
                heapPos[i] = cp;
                cp += 1;
            }
        }

        for (int i=heapSize/2; i>=1; i--){
            minHeapify(i, heap, heapSize, heapPos);
        }


        float tempS;
        while (heapSize > 1){
            tempS = heapExtractMinl(heap, heapSize, heapPos, adj, indicateGraph, l);
            totalW -= tempS;
            if (optDen < totalW/heapSize) {
                optDen = totalW/heapSize;
                optSize = heapSize;
            }
        }
    }

    finish = clock();

    cout << optDen << endl;
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "Time cost of greedy++ is " << duration << endl;

    return 0;
}




float naivePeeling(){
    clock_t start, finish;
    double duration;
    ifstream infile;
    infile.open("D:\\research\\2020-6-12\\data\\test.txt", ios::in);
    int count=0;
    int V = 16726;
    vector <pair <int, float>> * adj = new vector <pair <int, float>> [V+1];
    float * strength = new float[V+1];
    for (int i=0; i<=V; i++){
        strength[i] = 0;
    }
    float vertexS[V+1];
    for (int i=1; i<=V; i++) {
        vertexS[i] = 0;
    }
    float totalW=0;


    string line;
    char c[20];
    char* num_str;
    stringstream s;
    int n1;
    int n2;
    float w;

    while (getline(infile, line)){
        if (count > 142759) break;

        strcpy(c, line.c_str());

        num_str = strtok(c, " ");
        s << num_str;
        s >> n1;
        s.clear();
        s.str("");


        num_str = strtok(NULL, " ");
        s << num_str;
        s >> n2;
        s.clear();
        s.str("");


        num_str = strtok(NULL, " ");
        s << num_str;
        s >> w;
        s.clear();
        s.str("");


        if (n1 < n2){
            adj[n1].push_back(make_pair(n2, w));
            adj[n2].push_back(make_pair(n1, w));
            strength[n1] += w;
            strength[n2] += w;
            totalW += w;
        } else if (n1 == n2){
            strength[n1] += w;
            vertexS[n1] += w;
            totalW += w;
        }
    }

    float maxS=0;
    for (int i=0; i<=V; i++){
        if (strength[i] > maxS){
            maxS = strength[i];
        }
    }


    int * indicateGraph = new int[V+1];
    for (int i=1; i <= V; i++){
        indicateGraph[i] = 1;
    }

    start = clock();
    int heapSize = V;
    int* heapPos = new int[V+1];
    float dmax=0;
    float tempS;
    float densityMax = totalW/V;


    int cp = 1;

    pair <float, int>* heap = new pair <float, int>[heapSize+1];
    for (int i=1; i<=V; i++) {
        if (indicateGraph[i] > 0){
            heap[cp] = make_pair(strength[i], i);
            heapPos[i] = cp;
            cp += 1;
        }
    }

    for (int i=heapSize/2; i>=1; i--){
        minHeapify(i, heap, heapSize, heapPos);
    }

    int tempSize=heapSize;

    while (heapSize > 1){
        tempS = heapExtractMin(heap, heapSize, heapPos, adj, indicateGraph);
        totalW -= tempS;
        if (tempS > dmax) dmax = tempS;
        if (densityMax < totalW/heapSize) {
            densityMax = totalW/heapSize;
            tempSize = heapSize;
        }
    }
    finish = clock();

    cout << densityMax << endl;

    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "Time cost of naive peeling is " << duration << endl;


    return 0;
}

//int main(){
////    naivePeeling();
//    greedyPlusPlus();
//    return 0;
//}


