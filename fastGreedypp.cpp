#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <cstring>
#include <sstream>
#include <bits/stdc++.h>
#include <math.h>
#include "experiment.h"
#include "heap.h"
#include "dinic.h"
using namespace std;




int fasterGreedypp(int V, int vertexNum, long double totalW, int indicateGraph[], float strength[], float vertexS[], vector <pair <int, float>> adj[]){
    clock_t start, finish;
    double duration;
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

    float tempS;
    float dmax = 0;
    float maxS=0;
    float densityMax = totalW/vertexNum;


    start = clock();

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


    while (heapSize > 1){
        tempS = heapExtractMin(heap, heapSize, heapPos, adj, indicateGraph);
        totalW -= tempS;
        if (tempS > dmax) dmax = tempS;
        if (densityMax < totalW/heapSize) {
            densityMax = totalW/heapSize;
        }
    }


    float thres;
    if (densityMax < 0.5*dmax){
        thres = 0.5*dmax;
    } else {
        thres = densityMax;
    }



    delete [] heap;
    delete [] heapPos;
    heapPos = new int[V+1]{0};
    heapSize = vertexNum;
    heap = new pair <float, int>[heapSize+1];
    for (int i=0; i<=V; i++){
        indicateGraph[i] = originIndicateG[i];
    }
    cp = 1;
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

    while (heap[1].first < thres){
        heapExtractMin(heap, heapSize, heapPos, adj, indicateGraph);
    }

//    cout << "Heap size " << heapSize << endl;
//    cout << "threshold " << thres << endl;
//    cout << "max strength " << maxS << endl;

    for (int i=0; i<=V; i++){
        strength[i] = vertexS[i];
    }

    float sum;
    originTotalW = 0;
    vector<pair<int, float>> tp1;
    for (int i=1; i<=heapSize; i++){
        sum = 0;
        tp1 = adj[heap[i].second];
        for (int j=0; j<int(tp1.size()); j++){
            if (indicateGraph[tp1[j].first] > 0){
                sum += tp1[j].second;
                if (heap[i].second < tp1[j].first) {
                    originTotalW += tp1[j].second; // loc
                }
            }
        }
        strength[heap[i].second] += sum;
    }


    vertexNum = 0;
    for (int i=1; i<=V; i++){
        originIndicateG[i] = indicateGraph[i];
        if (indicateGraph[i] > 0) vertexNum ++;
    } // loc
    cout << "remain #" << vertexNum << endl;
    cout << "remain weight " << originTotalW << endl;


    // iteration begins
    for (int t=1; t<=T; t++){
        cout << "iter " << t << endl;
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
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "Density: " << optDen << endl;
    cout << "Time of greedy++: " << duration << endl;


    return 0;
}





int fastUnweighted(){
    // com-YouTube parameter set
//    string file = "D:\\research\\2020-6-12\\data\\com-youtube.ungraph.txt";
//    int startLine = 5;
//    int V = 1157827;
//    int lineNum = 2987628;
//    bool fromZero = false;

    // livejournal
//    string file = "D:\\research\\2020-6-12\\data\\com-lj.ungraph.txt";
//    int startLine = 5;
//    int lineNum = 34681193;
//    int V=4036538;
//    bool fromZero = true;

    // com-dblp parameter set
//    string file = "D:\\research\\2020-6-12\\data\\com-dblp.ungraph.txt";
//    int startLine = 5;
//    int lineNum = 1049866;
//    int V = 425957;
//    bool fromZero = true;

    // com-Amazon
    string file = "D:\\research\\2020-6-12\\data\\com-amazon.ungraph.txt";
    int startLine = 5;
    int lineNum = 925876;
    int V=548551;
    bool fromZero = false;


    int edgeNum;
    int vertexNum = 0;
    long double totalW = 0;
    int* indicateGraph = new int[V+1]{0};
    float* strength = new float[V+1]{0};
    float* vertexS = new float[V+1]{0};
    vector <pair <int, float>>* adj = new vector <pair <int, float>> [V+1];


    readtxtGraph(file, "	", fromZero, vertexNum, edgeNum, startLine, lineNum, totalW, indicateGraph, strength, vertexS, adj);


    fasterGreedypp(V, vertexNum, totalW, indicateGraph, strength, vertexS, adj);

    return 0;
}


int fastWeighted(){
    // lbimseti
//    string file = "D:\\research\\2020-6-12\\data\\lbimseti\\edges.csv";
//    int V = 220970;
//    int startLine = 2;
//    int lineNum = 17359347;

    // test
    string file = "D:\\research\\2020-6-12\\data\\test.txt";
    int V = 16726;
    int startLine = 1;
    int lineNum = 95188;

    // open-flight
//    string file = "D:\\research\\2020-6-12\\data\\open-flight.txt";
//    int V = 7976;
//    int startLine = 1;
//    int lineNum = 30501;


    // facebook-like social network
//    string file = "D:\\research\\2020-6-12\\data\\OF_one-mode_weightedmsg_sum.txt";
//    int V = 899;
//    int startLine = 1;
//    int lineNum = 142760;

    // program execution
    int vertexNum = 0;
    int edgeNum = 0;
    long double totalW = 0;
    int* indicateGraph = new int[V+1]{0};
    float* strength = new float[V+1]{0};
    float* vertexS = new float[V+1]{0};
    vector <pair <int, float>>* adj = new vector <pair <int, float>> [V+1];

//    readWeightGraph(file, ",", true, vertexNum, edgeNum, startLine, lineNum, totalW, indicateGraph, strength, vertexS, adj);
    readWeightGraph(file, " ", false, vertexNum, edgeNum, startLine, lineNum, totalW, indicateGraph, strength, vertexS, adj);

    fasterGreedypp(V, vertexNum, totalW, indicateGraph, strength, vertexS, adj);
    return 0;
}


int fastWeighted2(){
    // live journal parameter set
//    string file = "D:\\research\\2020-6-12\\data\\com-lj.ungraph.txt";
//    int startLine = 5;
//    int lineNum = 34681193;
//    int V=4036538;
//    bool fromZero = true;


    // com-YouTube parameter set
    string file = "D:\\research\\2020-6-12\\data\\com-youtube.ungraph.txt";
    int startLine = 5;
    int V = 1157827;
    int lineNum = 2987628;
    bool fromZero = false;

    float* fraudar = new float[V+1]{0};
    fraudarGraph(file, "	", fromZero, V, startLine, lineNum, fraudar);


    int vertexNum = 0;
    int edgeNum = 0;
    long double totalW = 0;
    int* indicateGraph = new int[V+1]{0};
    float* strength = new float[V+1]{0};
    float* vertexS = new float[V+1]{0};
    vector <pair <int, float>>* adj = new vector <pair <int, float>> [V+1];

    readFraudarGraph(file, "	", fromZero, startLine, lineNum, vertexNum, edgeNum, totalW, indicateGraph, fraudar, strength, vertexS, adj);

    fasterGreedypp(V, vertexNum, totalW, indicateGraph, strength, vertexS, adj);
    return 0;
}




int main(){
    fastWeighted();
    return 0;
}





