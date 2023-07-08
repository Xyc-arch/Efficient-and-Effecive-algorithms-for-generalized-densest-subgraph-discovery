#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <cstring>
#include <sstream>
#include <bits/stdc++.h>
#include <math.h>
#include <cstdlib>
#include "experiment2.h"
#include "dinic.h"

float cCoreUpdateIns(int V, int vertexNum, float& hatRhoPlus, long double totalW, int indicateGraph[], int* tildeS, int* C, float strength[], float vertexS[], vector <pair <int, float>> adj[]);

struct dynEdge {
    int u;
    int v;
    int w;
};


void copyGraph(int* graphSrc, int* graphDst, int V){
    for (int i=0; i<V+1; i++){
        graphDst[i] = graphSrc[i];
    }
}

int graphSize(int* graph, int V){
    int count = 0;
    for (int i=0; i<V+1; i++){
        if (graph[i] == 1) count ++;
    }
    return count;
}

void updateC(int V, int* C, float hatRho, vector <pair <int, float>> adj[]){
    int* heapPos = new int[V+1]{0};
    int heapSize = graphSize(C, V);
    pair <float, int>* heap = new pair <float, int>[heapSize+1];
    int* strengthC = new int[V+1]{0};
    int sum;
    vector<pair<int, float>> tp;
    long double totalW = 0;

    // construct C
    for (int i=1; i<=V; i++){
        if (!C[i]) continue;
        sum = 0;
        tp = adj[i];
        for (int j=0; j<int(tp.size()); j++){
            if (C[tp[j].first] > 0){
                sum += tp[j].second;
                if (i < tp[j].first) totalW += tp[j].second;
                strengthC[i] += sum;
            }
        }
    }

    // peeling
    int cp = 1;
    for (int i=1; i<=V; i++) {
        if (C[i] > 0){
            heap[cp] = make_pair(strengthC[i], i);
            heapPos[i] = cp;
            cp += 1;
        }
    }

    for (int i=heapSize/2; i>=1; i--){
        minHeapify(i, heap, heapSize, heapPos);
    }

    while (heap[1].first < hatRho){
        heapExtractMin(heap, heapSize, heapPos, adj, C);
    }

    // cout << "hatRho: " << hatRho << endl;

    delete [] heapPos;
    delete [] heap;
    delete [] strengthC;
}



float cCoreIns(int V, int vertexNum, long double totalW, int indicateGraph[], int* tildeS, int* C, float strength[], float vertexS[], vector <pair <int, float>> adj[], int u, int v, float& hatRhoPlus, int& hit){

    int* rawIndicateGraph= new int[V+1]{0};
    copyGraph(indicateGraph, rawIndicateGraph, V);
    int* rawTildeS= new int[V+1]{0};
    copyGraph(tildeS, rawTildeS, V);
    int* strengthTS = new int[V+1]{0};
    int* strengthC = new int[V+1]{0};
    long double totalWTS = 0;
    long double rawTotalWTS = 0;

    float hatRho = hatRhoPlus;
    float hatRhoRaw = 0;
    int tildeSNum = 0;
    int cpTS = 1;
    int heapSizeTS;
    int* heapPosTS = new int[V+1];
    float sumTS;
    vector<pair<int, float>> tpTS;
    float tempSTS;

    int mySize;

    if (tildeS[u] && tildeS[v]) {

        // construct original tildeS
        for (int i=1; i<=V; i++){
            if (!tildeS[i]) continue;
            tildeSNum ++;
            sumTS = 0;
            tpTS = adj[i];
            for (int j=0; j<int(tpTS.size()); j++){
                if (tildeS[tpTS[j].first] > 0){
                    sumTS += tpTS[j].second;
                    if (i < tpTS[j].first) totalWTS += tpTS[j].second;
                    strengthTS[i] = sumTS;
                }
            }
        }

        hatRho = totalWTS/tildeSNum;
        heapSizeTS = tildeSNum;
        rawTotalWTS = totalWTS;

        // new hatRho
        cpTS = 1;
        pair <float, int>* heapTS = new pair <float, int>[heapSizeTS+1];
        for (int i=1; i<=V; i++) {
            if (tildeS[i] > 0){
                heapTS[cpTS] = make_pair(strengthTS[i], i);
                heapPosTS[i] = cpTS;
                cpTS += 1;
            }
        }

        for (int i=heapSizeTS/2; i>=1; i--){
            minHeapify(i, heapTS, heapSizeTS, heapPosTS);
        }

        while (heapSizeTS > 1){
            tempSTS = heapExtractMin(heapTS, heapSizeTS, heapPosTS, adj, tildeS);
            totalWTS -= tempSTS;
            if (hatRho < totalWTS/heapSizeTS) {
                hatRho = totalWTS/heapSizeTS;
            }
        }

//        int mySize = graphSize(tildeS, V);
//        cout << "# tildeS: " << mySize << endl;
//        cout << "new hatRho: " << hatRho << endl;


        // obtain new tildeS

        hatRhoRaw = hatRho;
        totalWTS = rawTotalWTS;
        hatRho = totalWTS/tildeSNum;
        copyGraph(rawTildeS, tildeS, V);
        heapSizeTS = tildeSNum;



        cpTS = 1;
        for (int i=1; i<=V; i++) {
            if (tildeS[i] > 0){
                heapTS[cpTS] = make_pair(strengthTS[i], i);
                heapPosTS[i] = cpTS;
                cpTS += 1;
            }
        }

        for (int i=heapSizeTS/2; i>=1; i--){
            minHeapify(i, heapTS, heapSizeTS, heapPosTS);
        }

        while (heapSizeTS > 1 && abs(hatRho-hatRhoRaw)>1e-5){
            tempSTS = heapExtractMin(heapTS, heapSizeTS, heapPosTS, adj, tildeS);
            totalWTS -= tempSTS;
            if (hatRho < totalWTS/heapSizeTS) {
                hatRho = totalWTS/heapSizeTS;
            }
        }


        // cout << "hatRho: " << hatRho << endl;

        hatRhoPlus = hatRho;
    }


    if (C[u] && C[v]) {
        hit ++;
        updateC(V, C, hatRhoPlus, adj);
    }
    else {
        if (strength[u] > hatRhoPlus && strength[v] > hatRhoPlus){
            // cout << "hatRhoPlus: " << hatRhoPlus << endl;
            float wu = 0;
            float wv = 0;
            vector<pair<int, float>> tpU, tpV;
            tpU = adj[u];
            for (int j=0; j<int(tpU.size()); j++){
                if (strength[tpU[j].first] > hatRhoPlus){
                    wu += tpU[j].second;
                }
            }
            tpV = adj[v];
            for (int j=0; j<int(tpV.size()); j++){
                if (strength[tpV[j].first] > hatRhoPlus){
                    wv += tpV[j].second;
                }
            }

            if (wu > hatRhoPlus && wv > hatRhoPlus){
                cCoreUpdateIns(V, vertexNum, hatRhoPlus, totalW, indicateGraph, tildeS, C, strength, vertexS, adj);
            } else {
                updateC(V, C, hatRhoPlus, adj);
                hit ++;
            }

        } else {
            updateC(V, C, hatRhoPlus, adj);
            hit ++;
        }
    }


    mySize = graphSize(C, V);
    // cout << "C size: " << mySize << endl;

    delete [] rawIndicateGraph;
    delete [] strengthTS;
    delete [] strengthC;
    delete [] heapPosTS;
    delete [] rawTildeS;
    return 0;
}


float cCoreUpdateIns(int V, int vertexNum, float& hatRhoPlus, long double totalW, int indicateGraph[], int* tildeS, int* C, float strength[], float vertexS[], vector <pair <int, float>> adj[]){

    int heapSize = vertexNum;
    int* heapPos = new int[V+1];
    float tempS;
    float densityMax = totalW/vertexNum;
    int mySize = 0;
    long double tempSSum = 0;
    long double totalWopt = 0;

    // raw data
    int* rawIndicateGraph= new int[V+1]{0};
    copyGraph(indicateGraph, rawIndicateGraph, V);
    long double rawTotalW = totalW;

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


    while (heapSize > 1){
        tempS = heapExtractMin(heap, heapSize, heapPos, adj, indicateGraph);
        totalW -= tempS;
        tempSSum += tempS;
        if (densityMax < totalW/heapSize) {
            densityMax = totalW/heapSize;
            totalWopt = totalW;
        }
    }


    float thres;
    thres = densityMax;
    hatRhoPlus = densityMax;

    // obtain tildeS

    float densityMaxRaw = densityMax;
    totalW = rawTotalW;
    densityMax = totalW/vertexNum;
    copyGraph(rawIndicateGraph, indicateGraph, V);
    heapSize = vertexNum;

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

    while (heapSize > 1 && abs(densityMax-densityMaxRaw)>1e-5){
        tempS = heapExtractMin(heap, heapSize, heapPos, adj, indicateGraph);
        totalW -= tempS;
        if (densityMax < totalW/heapSize) {
            densityMax = totalW/heapSize;
        }
    }

    copyGraph(indicateGraph, tildeS, V);

    // peel hat rho (thres) c-core C

    delete [] heap;
    delete [] heapPos;
    heapPos = new int[V+1]{0};
    heapSize = vertexNum;
    heap = new pair <float, int>[heapSize+1];

    for (int i=0; i<=V; i++){
        indicateGraph[i] = rawIndicateGraph[i];
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

    copyGraph(indicateGraph, C, V);
    copyGraph(rawIndicateGraph, indicateGraph, V);
    totalW = rawTotalW;


    // cout << "Now tildeS size: " << graphSize(tildeS, V) << endl;

    delete [] heapPos;
    delete [] heap;
    delete [] rawIndicateGraph;

    return 0;
}


void randDynEdge(int V, int setSize, int wlb, int wub, dynEdge dynEdgeSet[], int indicateGraph[]){
    int u, v;
    int w;
    if (wlb == wub) {
        w = wlb;
    } else {
        w = (rand() % (wub - wlb + 1)) + wlb;
    }
    for (int i=0; i<setSize; i++){
        u = rand()%V;
        while (!indicateGraph[u]){
            u = rand()%V;
        }
        v = rand()%V;
        while (!indicateGraph[v]){
            v = rand()%V;
        }
        dynEdge tempEdge = {.u = u, .v = v, .w = w};
        dynEdgeSet[i] = tempEdge;
    }
}

int insertExp(){
    clock_t start1, finish1, start2, finish2;
    double duration1, duration2;
    int setSize = 1000;

    // com-dblp parameter set
//    string file = "D:\\research\\2022-6-12\\data\\com-dblp.ungraph.txt";
//    int startLine = 5;
//    int lineNum = 1049866;
//    int V = 425957;
//    bool fromZero = true;

    // live journal parameter set
//    string file = "D:\\research\\2022-6-12\\data\\com-lj.ungraph.txt";
//    int startLine = 5;
//    int lineNum = 34681193;
//    int V=4036538;
//    bool fromZero = true;


    // com-YouTube parameter set
    string file = "D:\\research\\2022-6-12\\data\\com-youtube.ungraph.txt";
    int startLine = 5;
    int V = 1157827;
    int lineNum = 2987628;
    bool fromZero = false;

    int edgeNum = 0;
    int vertexNum = 0;
    long double totalW = 0;
    int* indicateGraph = new int[V+1]{0};
    float* strength = new float[V+1]{0};
    float* vertexS = new float[V+1]{0};
    vector <pair <int, float>>* adj = new vector <pair <int, float>> [V+1];

    int* tildeS = new int[V+1]{0};
    int* C = new int[V+1]{0};


    readtxtGraph(file, "	", fromZero, vertexNum, edgeNum, startLine, lineNum, totalW, indicateGraph, strength, vertexS, adj);

    copyGraph(indicateGraph, tildeS, V);
    copyGraph(indicateGraph, C, V);

    dynEdge dynEdgeSet[setSize];
    randDynEdge(V, setSize, 1, 1, dynEdgeSet, indicateGraph);
//    cout << dynEdgeSet[setSize-1].u << endl;
//    cout << dynEdgeSet[setSize-1].v << endl;

    int u, v, w;
    int iter = 0;
    float hatRho = 0;
    int mySize = 0;
    int hit = 0;

    // cCoreUpdateIns

//    start1 = clock();

//    for (int i=0; i<100; i++){
//        if (iter%100 == 0 && iter != 0) {
//            cout << "At iter: " << iter << endl;
//        }
////        mySize = graphSize(tildeS, V);
////        cout << "tilde S: " << mySize << endl;
//        iter ++;
//        u = dynEdgeSet[i].u;
//        v = dynEdgeSet[i].v;
//        w = dynEdgeSet[i].w;
//        adj[u].push_back(make_pair(v, w));
//        adj[v].push_back(make_pair(u, w));
//        totalW += w;
//        strength[u] += w;
//        strength[v] += w;
//        cCoreUpdateIns(V, vertexNum, hatRho, totalW, indicateGraph, tildeS, C, strength, vertexS, adj);
//        // berkleyexp2(V, vertexNum, totalW, indicateGraph, strength, vertexS, adj);
//    }

//    finish1 = clock();
//    duration1 = (double)(finish1 - start1) / CLOCKS_PER_SEC;
//    cout << "Time cost of cCoreUpdateIns: " << duration1 << endl;


    // cCoreIns

    start2 = clock();

    float hatRhoPlus = 0;

    for (int i=0; i<1000; i++){
        if (iter%100 == 0 && iter != 0) {
            cout << "At iter: " << iter << endl;
        }
        iter ++;


        u = dynEdgeSet[i].u;
        v = dynEdgeSet[i].v;
        w = dynEdgeSet[i].w;
        adj[u].push_back(make_pair(v, w));
        adj[v].push_back(make_pair(u, w));
        totalW += w;
        strength[u] += w;
        strength[v] += w;
        if (i == 0) {
            cCoreUpdateIns(V, vertexNum, hatRhoPlus, totalW, indicateGraph, tildeS, C, strength, vertexS, adj);
        } else {
            cCoreIns(V, vertexNum, totalW, indicateGraph, tildeS, C, strength, vertexS, adj, u, v, hatRhoPlus, hit);
        }

    }

    cout << "Hit: " << hit << endl;

    finish2 = clock();
    duration2 = (double)(finish2 - start2) / CLOCKS_PER_SEC;
    cout << "Time cost of cCoreIns: " << duration2 << endl;


    // cCoreUpdateIns(V, vertexNum, totalW, indicateGraph, tildeS, C, strength, vertexS, adj);
    // cCoreIns(V, vertexNum, totalW, indicateGraph, tildeS, C, strength, vertexS, adj, u, v);

    return 0;
}


int insertExpW(){

    clock_t start1, finish1, start2, finish2;
    double duration1, duration2;
    int setSize = 1000;

    // test
    string file = "D:\\research\\2022-6-12\\data\\test.txt";
    int V = 16726;
    int startLine = 1;
    int lineNum = 95188;

    // open-flight
//    string file = "D:\\research\\2022-6-12\\data\\open-flight.txt";
//    int V = 7976;
//    int startLine = 1;
//    int lineNum = 30501;


    // lbimseti
//    string file = "D:\\research\\2022-6-12\\data\\lbimseti\\edges.csv";
//    int V = 220970;
//    int startLine = 2;
//    int lineNum = 17359347;

    int edgeNum = 0;
    int vertexNum = 0;
    long double totalW = 0;
    int* indicateGraph = new int[V+1]{0};
    float* strength = new float[V+1]{0};
    float* vertexS = new float[V+1]{0};
    vector <pair <int, float>>* adj = new vector <pair <int, float>> [V+1];

    int* tildeS = new int[V+1]{0};
    int* C = new int[V+1]{0};


    // readWeightGraph(file, ",", true, vertexNum, edgeNum, startLine, lineNum, totalW, indicateGraph, strength, vertexS, adj);
    readWeightGraph(file, " ", false, vertexNum, edgeNum, startLine, lineNum, totalW, indicateGraph, strength, vertexS, adj);

    copyGraph(indicateGraph, tildeS, V);
    copyGraph(indicateGraph, C, V);

    dynEdge dynEdgeSet[setSize];
    randDynEdge(V, setSize, 1, 10, dynEdgeSet, indicateGraph);


    int u, v, w;
    int iter = 0;
    float hatRho = 0;
    int mySize = 0;
    int hit = 0;


    // cCoreUpdateIns

//    start1 = clock();

//    for (int i=0; i<100; i++){
//        iter ++;
//        u = dynEdgeSet[i].u;
//        v = dynEdgeSet[i].v;
//        w = dynEdgeSet[i].w;
//        adj[u].push_back(make_pair(v, w));
//        adj[v].push_back(make_pair(u, w));
//        totalW += w;
//        strength[u] += w;
//        strength[v] += w;
//        cCoreUpdateIns(V, vertexNum, hatRho, totalW, indicateGraph, tildeS, C, strength, vertexS, adj);
//        // berkleyexp2(V, vertexNum, totalW, indicateGraph, strength, vertexS, adj);
//    }

//    finish1 = clock();
//    duration1 = (double)(finish1 - start1) / CLOCKS_PER_SEC;
//    cout << "Time cost of cCoreUpdateIns: " << duration1 << endl;


    // cCoreIns

    start2 = clock();

    float hatRhoPlus = 0;
    bool init = true;

    for (int i=0; i<1000; i++){

        if (iter%100 == 0 && iter != 0) {
            cout << "At iter: " << iter << endl;
            cout << "C has size: " << graphSize(C, V) << endl;
//            cout << "hatRho: " << hatRhoPlus << endl;
        }

        iter ++;
        if (iter > 1) init = false;

//        if (iter == 2) {
//            cout << "C has size: " << graphSize(C, V) << endl;
//            cout << "tildeS has size: " << graphSize(tildeS, V) << endl;
//        }

        u = dynEdgeSet[i].u;
        v = dynEdgeSet[i].v;
        w = dynEdgeSet[i].w;
        adj[u].push_back(make_pair(v, w));
        adj[v].push_back(make_pair(u, w));
        totalW += w;
        strength[u] += w;
        strength[v] += w;
        if (i == 0) cCoreUpdateIns(V, vertexNum, hatRhoPlus, totalW, indicateGraph, tildeS, C, strength, vertexS, adj);
        cCoreIns(V, vertexNum, totalW, indicateGraph, tildeS, C, strength, vertexS, adj, u, v, hatRhoPlus, hit);
    }

    cout << "Hit: " << hit << endl;

    finish2 = clock();
    duration2 = (double)(finish2 - start2) / CLOCKS_PER_SEC;
    cout << "Time cost of cCoreIns: " << duration2 << endl;




    return 0;
}



int compareInsertC(bool dataW){

    int setSize = 1000;
    clock_t start1, finish1, start2, finish2;
    double duration1, duration2;

    // unweighted graphs

    // com-dblp parameter set
//    string file = "D:\\research\\2022-6-12\\data\\com-dblp.ungraph.txt";
//    int startLine = 5;
//    int lineNum = 1049866;
//    int V = 425957;
//    bool fromZero = true;

    // live journal parameter set
    string file = "D:\\research\\2022-6-12\\data\\com-lj.ungraph.txt";
    int startLine = 5;
    int lineNum = 34681193;
    int V=4036538;
    bool fromZero = true;


    // com-YouTube parameter set
//    string file = "D:\\research\\2022-6-12\\data\\com-youtube.ungraph.txt";
//    int startLine = 5;
//    int V = 1157827;
//    int lineNum = 2987628;
//    bool fromZero = false;




    // weighted graphs

    // test
//    string file = "D:\\research\\2022-6-12\\data\\test.txt";
//    int V = 16726;
//    int startLine = 1;
//    int lineNum = 95188;

    // open-flight
//    string file = "D:\\research\\2022-6-12\\data\\open-flight.txt";
//    int V = 7976;
//    int startLine = 1;
//    int lineNum = 30501;


    // lbimseti
//    string file = "D:\\research\\2022-6-12\\data\\lbimseti\\edges.csv";
//    int V = 220970;
//    int startLine = 2;
//    int lineNum = 17359347;


    int edgeNum = 0;
    int vertexNum = 0;
    long double totalW = 0;
    int* indicateGraph = new int[V+1]{0};
    int* rawIndicateGraph = new int[V+1]{0};
    float* strength = new float[V+1]{0};
    float* vertexS = new float[V+1]{0};
    vector <pair <int, float>>* adj = new vector <pair <int, float>> [V+1];

    int* sizeC1 = new int[V+1]{0};
    int* sizeC2 = new int[V+1]{0};

    int* tildeS = new int[V+1]{0};
    int* C = new int[V+1]{0};


    if (!dataW){
        readtxtGraph(file, "	", fromZero, vertexNum, edgeNum, startLine, lineNum, totalW, indicateGraph, strength, vertexS, adj);
    } else {
        // readWeightGraph(file, ",", true, vertexNum, edgeNum, startLine, lineNum, totalW, indicateGraph, strength, vertexS, adj);
        readWeightGraph(file, " ", false, vertexNum, edgeNum, startLine, lineNum, totalW, indicateGraph, strength, vertexS, adj);
    }

    copyGraph(indicateGraph, tildeS, V);
    copyGraph(indicateGraph, C, V);
    copyGraph(indicateGraph, rawIndicateGraph, V);


    dynEdge dynEdgeSet[setSize];
    if (! dataW) {
        randDynEdge(V, setSize, 1, 1, dynEdgeSet, indicateGraph);
    } else {
        randDynEdge(V, setSize, 1, 10, dynEdgeSet, indicateGraph);
    }

    int u, v, w;
    float hatRho = 0;
    int mySize = 0;

    cout << "cCoreUpdateIns begins" << endl;
    start1 = clock();
    for (int i=0; i<1000; i++){
        if (i%100 == 0 && i != 0) {
            cout << "At iter: " << i << endl;
        }
        u = dynEdgeSet[i].u;
        v = dynEdgeSet[i].v;
        w = dynEdgeSet[i].w;
        adj[u].push_back(make_pair(v, w));
        adj[v].push_back(make_pair(u, w));
        totalW += w;
        strength[u] += w;
        strength[v] += w;
        cCoreUpdateIns(V, vertexNum, hatRho, totalW, indicateGraph, tildeS, C, strength, vertexS, adj);
        mySize = graphSize(C, V);
        sizeC1[i] = mySize;
    }
    finish1 = clock();
    duration1 = (double)(finish1 - start1) / CLOCKS_PER_SEC;
    cout << "Time cost of cCoreUpdateIns: " << duration1 << endl;


    copyGraph(rawIndicateGraph, indicateGraph, V);
    copyGraph(indicateGraph, tildeS, V);
    copyGraph(indicateGraph, C, V);
    float hatRhoPlus = 0;
    int hit = 0;

    cout << "cCoreIns begins" << endl;
    start2 = clock();
    for (int i=0; i<1000; i++){
        if (i%100 == 0 && i != 0) {
            cout << "At iter: " << i << endl;
        }

        u = dynEdgeSet[i].u;
        v = dynEdgeSet[i].v;
        w = dynEdgeSet[i].w;
        adj[u].push_back(make_pair(v, w));
        adj[v].push_back(make_pair(u, w));
        totalW += w;
        strength[u] += w;
        strength[v] += w;
        if (i == 0) {
            cCoreUpdateIns(V, vertexNum, hatRhoPlus, totalW, indicateGraph, tildeS, C, strength, vertexS, adj);
        } else {
            cCoreIns(V, vertexNum, totalW, indicateGraph, tildeS, C, strength, vertexS, adj, u, v, hatRhoPlus, hit);
        }
        mySize = graphSize(C, V);
        sizeC2[i] = mySize;
    }
    finish2 = clock();
    duration2 = (double)(finish2 - start2) / CLOCKS_PER_SEC;
    cout << "Time cost of cCoreIns: " << duration2 << endl;
    cout << "Hit: " << hit << endl;

    float speedup = 0;
    speedup = duration1/duration2;
    cout << "Speedup: " << speedup << endl;

    int diff = 0;
    int maxSizeDiff = 0;
    float maxRatioDiff = 0;
    for (int i=0; i<=V; i++){
        if (sizeC1[i] != sizeC2[i]) {
            diff++;
            if (sizeC2[i] - sizeC1[i] > maxSizeDiff) maxSizeDiff = sizeC2[i] - sizeC1[i];
            if ((sizeC2[i] - sizeC1[i])/sizeC1[i] > maxRatioDiff) maxRatioDiff = (sizeC2[i] - sizeC1[i])/sizeC1[i];
        }
    }

    cout << "diff: " << diff << endl;
    cout << "maxSizeDiff: " << maxSizeDiff << endl;
    cout << "maxRatioDiff: " << maxRatioDiff << endl;


    return 0;
}


//int main(){
//   // insertExp();
//   // insertExpW();
//   bool dataW = false;
//   // bool dataW = true;

//   compareInsertC(dataW);
//   return 0;
//}





