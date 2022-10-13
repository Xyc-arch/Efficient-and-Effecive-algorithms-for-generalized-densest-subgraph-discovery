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
#include "dinic.h"



int getV(string name, string split,int startLine, int lineNum){
    int V = 0;
    int n1, n2;
    int currentLine = 0;
    ifstream infile;
    string line;
    char c[20];
    char* num_str;
    stringstream s;

    infile.open(name, ios::in);

    while (getline(infile, line)){
        currentLine += 1;
        if (currentLine > lineNum){
            break;
        }
        if (currentLine < startLine){
            continue;
        }

        strcpy(c, line.c_str());

        num_str = strtok(c, split.c_str());
        s << num_str;
        s >> n1;
        s.clear();
        s.str("");

        if (n1 > V) V = n1;


        num_str = strtok(NULL, split.c_str());
        s << num_str;
        s >> n2;
        s.clear();
        s.str("");

        if (n2 > V) V = n2;
        if (currentLine%10000000 == 0) cout << "Line: " << currentLine << endl;

    }
    return V;
}


void readtxtGraph(string name, string split, bool fromZero, int& vertexNum, int startLine, int lineNum, long double & totalW, int indicateGraph[], float strength[], float vertexS[], vector <pair <int, float>> adj[]){
    ifstream infile;
    string line;
    int n1;
    int n2;
    int currentLine=0;
    char c[20];
    char* num_str;
    stringstream s;
    map <int, int> allVertex;

    infile.open(name, ios::in);

    while (getline(infile, line)){
        currentLine += 1;
        if (currentLine > lineNum){
            break;
        }
        if (currentLine < startLine){
            continue;
        }

        strcpy(c, line.c_str());
        num_str = strtok(c, split.c_str());
        s << num_str;
        s >> n1;
        s.clear();
        s.str("");

        num_str = strtok(NULL, split.c_str());
        s << num_str;
        s >> n2;
        s.clear();
        s.str("");

        if (fromZero){
            n1 += 1;
            n2 += 1;
        }
        if (n1 < n2){
            adj[n1].push_back(make_pair(n2, 1));
            adj[n2].push_back(make_pair(n1, 1));
            strength[n1] += 1;
            strength[n2] += 1;
            totalW += 1;
        }
        if (n1 == n2){
            strength[n1] += 1;
            vertexS[n1] += 1;
            totalW += 1;
        }
    allVertex.insert(pair<int, int>(n1, 1));
    allVertex.insert(pair<int, int>(n2, 1));
    if (currentLine%10000000 == 0) cout << "Line: " << currentLine << endl;

    }

    for (auto it = allVertex.begin(); it != allVertex.end(); ++it){
        vertexNum += 1;
        indicateGraph[it -> first] = 1;
    }

    cout << "Graph reading finish" << endl;
}



pair<double, double> DALKSexp(int V, int vertexNum, int k, int j, long double totalW, int indicateGraph[], float strength[], float vertexS[], vector <pair <int, float>> adj[]){
    clock_t start1, finish1, start2, finish2;
    double duration1, duration2;

    int* indicateGraphCopy = new int[V+1];
    for (int i=1; i<=V; i++){
        indicateGraphCopy[i] = indicateGraph[i];
    }


    start1 = clock();
    int heapSize = vertexNum;
    int* heapPos = new int[V+1];
    float density;
    density = totalW/vertexNum;


    float optDen = density;
    float optW = totalW;
    int optSize = vertexNum;
    int* optGraph = new int[V+1];
    for (int i=1; i<= V; i++){
        optGraph[i] = indicateGraph[i];
    }

    float thres;
    float outW;
    float tempS;

    pair <float, int> * heap = new pair <float, int>[vertexNum+1];
    int tp = 1;


    for (int i=1; i<=V; i++) {
        if (indicateGraph[i] > 0){
            heap[tp] = make_pair(strength[i], i);
            heapPos[i] = tp;
            tp += 1;
        }
    }

    for (int i=heapSize/2; i>=1; i--){
        minHeapify(i, heap, heapSize, heapPos);
    }

    while (heapSize > k){
        thres = heap[1].first;
        outW = 0;
        while (heap[1].first <= thres && heapSize > k){
            tempS = heapExtractMin(heap, heapSize, heapPos, adj, indicateGraph);
            outW += tempS;
        }
        totalW -= outW;
        density = totalW/float(heapSize);
        if (density > optDen){
            optDen = density;
            optW = totalW;
            optSize = heapSize;
            for (int i=0; i<=V; i++){
                optGraph[i] = indicateGraph[i];
            }
        }
    }
    finish1 = clock();
    duration1 = (double)(finish1 - start1) / CLOCKS_PER_SEC;
    cout << optDen << endl;



//********************************************second
    int topNum = j * k;
    for (int i=1; i <= V; i++){
        indicateGraph[i] = 0;
    }

    start2 = clock();
//    int maxheapSize = vertexNum;
//    tp = 1;
//    pair <float, int>* maxheap = new pair <float, int>[maxheapSize+1];
//    for (int i=1; i<=V; i++) {
//        if (indicateGraphCopy[i]>0){
//            maxheap[tp] = make_pair(strength[i], i);
//            tp += 1;
//        }
//    }
//    for (int i=maxheapSize/2; i>=1; i--){
//        maxHeapify(i, maxheap, maxheapSize);
//    }
//    pair <float, int> outPair;
//    for (int i = 1; i <= topNum; i++){
//        outPair = heapExtractMax(maxheap, maxheapSize);
//        indicateGraph[outPair.second] = 1;
//    }

    //new here
    int topMinHeapSize = topNum;
    for (int i=1; i <= V; i++){
        indicateGraph[i] = 0;
    }
    pair <float, int>* topMinHeap = new pair <float, int>[topMinHeapSize+1];
    for (int i=1; i<=topMinHeapSize; i++){
        topMinHeap[i] = make_pair(strength[i], i);
    }
    for (int i=topMinHeapSize/2; i>=1; i--){
        minHeapify(i, topMinHeap, topMinHeapSize);
    }

    pair<float, int> tempPair;
    for (int i=topMinHeapSize+1; i<=V; i++){
        if (strength[i] > topMinHeap[1].first){
            tempPair = make_pair(strength[i], i);
            topMinHeap[1] = tempPair;
            minHeapify(1, topMinHeap, topMinHeapSize);
        }
    }

    for (int i=1; i<=topMinHeapSize; i++){
        indicateGraph[topMinHeap[i].second] = 1;
    }
    //new end

    int * topStrength = new int[V+1]{0};

    int* heap1Pos = new int[V+1]{0};
    int cpPos = 1;
    totalW = 0;
    pair <float, int>* heap1 = new pair <float, int>[topNum+1];
    vector <pair <int, float>> adjacent;
    int heap1Size = topNum;
    for (int i=1; i<=V; i++){
        if (indicateGraph[i] == 0) continue;
        adjacent = adj[i];
        for (int j=0; j< int(adjacent.size()); j++){
            if (indicateGraph[adjacent[j].first] > 0 && i < adjacent[j].first){
                totalW += adjacent[j].second;
                topStrength[i] += adjacent[j].second;
                topStrength[adjacent[j].first] += adjacent[j].second;
            }
        }
        totalW += vertexS[i];
        topStrength[i] += vertexS[i];
        heap1[cpPos] = make_pair(topStrength[i], i);
        heap1Pos[i] = cpPos;
        cpPos += 1;
    }


    for (int i=heap1Size/2; i>=1; i--){
        minHeapify(i, heap1, heap1Size, heap1Pos);
    }


    density = totalW/topNum;
    optDen = density;
    optW = totalW;
    optSize = heap1Size;
    for (int i=0; i<=V; i++){
        optGraph[i] = indicateGraph[i];
    }



    while (heap1Size > k){
        thres = heap1[1].first;
        outW = 0;
        while (heap1[1].first <= thres && heap1Size > k){
            outW += heapExtractMin(heap1, heap1Size, heap1Pos, adj, indicateGraph);
        }
        totalW -= outW;
        density = totalW/heap1Size;
        if (density > optDen){
            optDen = density;
            optW = totalW;
            optSize = heap1Size;
            for (int i=0; i<=V; i++){
                optGraph[i] = indicateGraph[i];
            }
        }
    }


    int heap2Size = 0;
    for (int i=1; i<=V; i++){
        if (strength[i] > optDen || strength[i] > thres) indicateGraph[i] = 1;
        if (indicateGraph[i] > 0) heap2Size += 1;
    }

    int* heap2Pos = new int[V+1]{0};
    int* heap2Strength = new int[V+1]{0};
    pair <float, int>* heap2 = new pair <float, int>[heap2Size+1];
    cpPos = 1;
    totalW = 0;
    if (heap2Size > topNum){
        for (int i=1; i<=V; i++){
            if (indicateGraph[i] == 0) continue;
            adjacent = adj[i];
            for (int j=0; j< int(adjacent.size()); j++){
                if (indicateGraph[adjacent[j].first] > 0 && i < adjacent[j].first){
                    totalW += adjacent[j].second;
                    heap2Strength[i] += adjacent[j].second;
                    heap2Strength[adjacent[j].first] += adjacent[j].second;
                }
            }
            totalW += vertexS[i];
            heap2Strength[i] += vertexS[i];
            heap2[cpPos] = make_pair(heap2Strength[i], i);
            heap2Pos[i] = cpPos;
            cpPos += 1;
        }

        //new
        for (int i=heap2Size/2; i>=1; i--){
            minHeapify(i, heap2, heap2Size, heap2Pos);
        }

        density = totalW/heap2Size;
        optDen = density;
        optW = totalW;
        optSize = heap2Size;
        for (int i=0; i<=V; i++){
            optGraph[i] = indicateGraph[i];
        }

        while (heap2Size > k){
            thres = heap2[1].first;
            outW = 0;
            while (heap2[1].first <= thres && heap2Size > k){
                outW += heapExtractMin(heap2, heap2Size, heap2Pos, adj, indicateGraph);
            }
            totalW -= outW;
            density = totalW/heap2Size;
            if (density > optDen){
                optDen = density;
                optW = totalW;
                optSize = heap2Size;
                for (int i=0; i<=V; i++){
                    optGraph[i] = indicateGraph[i];
                }
            }
        }
    }

    finish2 = clock();
    duration2 = (double)(finish2 - start2) / CLOCKS_PER_SEC;
    cout << optDen << endl;
    cout << "duration 1: " << duration1 << endl;
    cout << "duration 2: " << duration2 << endl;


    return make_pair(duration1, duration2);

//    return 0;
}


float berkleyexp1(int V, int vertexNum, long double totalW, int indicateGraph[], float strength[], float vertexS[], vector <pair <int, float>> adj[]){
    clock_t start, finish, start0, finish0;
    double duration, duration0;

    float maxS=0;
    int heapSize = vertexNum;
    int* heapPos = new int[V+1];
    float dmax=0;
    float tempS;
    float densityMax = totalW/vertexNum;
    for (int i=0; i<=V; i++){
        if (indicateGraph[i]>0 && strength[i] > maxS){
            maxS = strength[i];
        }
    }

    float d;
//    start0 = clock();
//    d = BerkleyFlow(0, maxS, 0.001, V, indicateGraph, strength, vertexS, adj);
//    finish0 = clock();
//    duration0 = (double)(finish0 - start0) / CLOCKS_PER_SEC;
//    cout << duration0 << endl;

    start = clock();
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

    while (heap[1].first < densityMax or heap[1].first < 0.5*dmax){
        tempS = heapExtractMin(heap, heapSize, heapPos, adj, indicateGraph);
        totalW -= tempS;
        if (tempS > dmax) dmax = tempS;
        if (densityMax < totalW/heapSize) densityMax = totalW/heapSize;
    }


    float thres;
    if (densityMax < 0.5*dmax){
        thres = 0.5*dmax;
    } else {
        thres = densityMax;
    }

    for (int i=0; i<=V; i++){
        strength[i] = vertexS[i];
    }

    float sum;
    vector<pair<int, float>> tp1;
    for (int i=1; i<=heapSize; i++){
        sum = 0;
        tp1 = adj[heap[i].second];
        for (int j=0; j<int(tp1.size()); j++){
            if (indicateGraph[tp1[j].first] > 0){
                sum += tp1[j].second;
            }
        }
        strength[heap[i].second] += sum;
    }

    cout << "Heap size " << heapSize << endl;



    d = BerkleyFlowAcc(thres, maxS, 0.001, V, heapSize, heapPos, indicateGraph, strength, vertexS, heap, adj);
    finish = clock();
    cout <<"optimal density: "<< d << endl;
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "Time of accelerated berkley: " << duration << endl;
    return 0;
}



float berkleyexp2(int V, int vertexNum, long double totalW, int indicateGraph[], float strength[], float vertexS[], vector <pair <int, float>> adj[]){
    clock_t start, finish, start0, finish0;
    double duration, duration0;

    float maxS=0;
    int heapSize = vertexNum;
    int* heapPos = new int[V+1];
    float dmax=0;
    float tempS;
    float densityMax = totalW/vertexNum;
    for (int i=0; i<=V; i++){
        if (indicateGraph[i]>0 && strength[i] > maxS){
            maxS = strength[i];
        }
    }
    int* rawIndicateGraph= new int[V+1]{0};
    for (int i=0; i<=V; i++){
        rawIndicateGraph[i] = indicateGraph[i];
    }

    float d;

//    start0 = clock();
//    d = BerkleyFlow(0, maxS, 0.001, V, indicateGraph, strength, vertexS, adj);
//    finish0 = clock();
//    duration0 = (double)(finish0 - start0) / CLOCKS_PER_SEC;
//    cout << duration0 << endl;

    start = clock();
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

    cout << "Heap size " << heapSize << endl;
    cout << "threshold " << thres << endl;
    cout << "max strength " << maxS << endl;

    for (int i=0; i<=V; i++){
        strength[i] = vertexS[i];
    }

    float sum;
    vector<pair<int, float>> tp1;
    for (int i=1; i<=heapSize; i++){
        sum = 0;
        tp1 = adj[heap[i].second];
        for (int j=0; j<int(tp1.size()); j++){
            if (indicateGraph[tp1[j].first] > 0){
                sum += tp1[j].second;
            }
        }
        strength[heap[i].second] += sum;
    }

    d = BerkleyFlowAcc(thres, min(maxS, 2*thres), 0.001, V, heapSize, heapPos, indicateGraph, strength, vertexS, heap, adj);
    finish = clock();
    cout <<"optimal density: "<< d << endl;
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "Time of accelerated berkley: " << duration << endl;
    return 0;
}


float berkleyexp3(int V, int indicateGraph[], float strength[], float vertexS[], vector <pair <int, float>> adj[]){
    clock_t start, finish;
    double duration;
    float resultDen;
    float maxS=0;
    for (int i=0; i<=V; i++){
        if (strength[i] > maxS){
            maxS = strength[i];
        }
    }

    start = clock();
    resultDen = BerkleyFlow(0, maxS, 0.001, V, indicateGraph, strength, vertexS, adj);
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << duration << endl;
    cout << "berkley density: " << resultDen << endl;

    return 0;
}



float soda22exp1(int V, int vertexNum, long double totalW, int indicateGraph[], float strength[], float vertexS[], vector <pair <int, float>> adj[]){
    clock_t start, finish;
    double duration;

    int heapSize = vertexNum;
    int* heapPos = new int[V+1];
    float dmax=0;
    float tempS;
    float densityMax = totalW/vertexNum;
    float resultDen;

    float maxS=0;
    for (int i=0; i<=V; i++){
        if (strength[i] > maxS){
            maxS = strength[i];
        }
    }

//    resultDen = soda22FlowApp(0, maxS, 0.1, V, int(totalW), indicateGraph, vertexS, adj);

    start = clock();
    pair <float, int>* heap = new pair <float, int>[V+1];
    int cp = 1;
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


    while (heap[1].first < densityMax or heap[1].first < 0.5*dmax){
        tempS = heapExtractMin(heap, heapSize, heapPos, adj, indicateGraph);
        totalW -= tempS;
        if (tempS > dmax) dmax = tempS;
        if (densityMax < totalW/heapSize) densityMax = totalW/heapSize;
    }


    float thres;
    if (densityMax < 0.5*dmax){
        thres = 0.5*dmax;
    } else {
        thres = densityMax;
    }

    for (int i=0; i<=V; i++){
        strength[i] = vertexS[i];
    }

    float sum;
    int edgeNum = 0;
    vector<pair<int, float>> tp1;
    for (int i=1; i<=heapSize; i++){
        sum = 0;
        tp1 = adj[heap[i].second];
        for (int j=0; j<int(tp1.size()); j++){
            if (indicateGraph[tp1[j].first] > 0){
                sum += tp1[j].second;
                edgeNum += 1;
            }
        }
        strength[heap[i].second] += sum;
    }

//    resultDen = soda22FlowAcc(thres, maxS, 0.001, V, edgeNum, heapSize, heapPos, indicateGraph, vertexS, strength, heap, adj);
    resultDen = soda22FlowOriginAcc(thres, maxS, 0.001, V, edgeNum, heapSize, heapPos, indicateGraph, vertexS, strength, heap, adj);
    cout << resultDen << endl;

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "Time of accelerated soda22: " << duration << endl;

    return 0;
}



float soda22exp2(int V, int vertexNum, long double totalW, int indicateGraph[], float strength[], float vertexS[], vector <pair <int, float>> adj[]){

    clock_t start, finish, start0, finish0;
    double duration, duration0;

    int heapSize = vertexNum;
    int* heapPos = new int[V+1];
    float dmax=0;
    float tempS;
    float densityMax = totalW/vertexNum;
    float resultDen;

    float maxS=0;
    for (int i=0; i<=V; i++){
        if (strength[i] > maxS){
            maxS = strength[i];
        }
    }
    int* rawIndicateGraph= new int[V+1]{0};
    for (int i=0; i<=V; i++){
        rawIndicateGraph[i] = indicateGraph[i];
    }

//    cout << "Edge number: " << int(totalW) << endl;
//    start0 = clock();
//    resultDen = soda22FlowApp(0, maxS, 0.001, V, int(totalW), indicateGraph, vertexS, adj);
//    finish0 = clock();
//    duration0 = (double)(finish0 - start0) / CLOCKS_PER_SEC;
//    cout << duration0 << endl;


    start = clock();
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


    float thres = densityMax;
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

    cout << "Heap size " << heapSize << endl;

    for (int i=0; i<=V; i++){
        strength[i] = vertexS[i];
    }

    float sum;
    int edgeNum = 0;
    vector<pair<int, float>> tp1;
    for (int i=1; i<=heapSize; i++){
        sum = 0;
        tp1 = adj[heap[i].second];
        for (int j=0; j<int(tp1.size()); j++){
            if (indicateGraph[tp1[j].first] > 0){
                sum += tp1[j].second;
                edgeNum += 1;
            }
        }
        strength[heap[i].second] += sum;
    }

    resultDen = soda22FlowAcc(thres, min(maxS, 2*thres), 0.001, V, edgeNum, heapSize, heapPos, indicateGraph, vertexS, strength, heap, adj);
    //resultDen = soda22FlowOriginAcc(thres, min(maxS, 2*thres), 0.001, V, edgeNum, heapSize, heapPos, indicateGraph, vertexS, strength, heap, adj);
    cout << resultDen << endl;

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "Time of accelerated soda22: " << duration << endl;

    return 0;
}



float soda22exp3(int V, long double totalW, int indicateGraph[], float strength[], float vertexS[], vector <pair <int, float>> adj[]){
    clock_t start, finish;
    double duration;
    float resultDen;
    float maxS=0;
    for (int i=0; i<=V; i++){
        if (strength[i] > maxS){
            maxS = strength[i];
        }
    }

    start = clock();
    resultDen = soda22FlowApp(0, maxS, 0.001, V, int(totalW), indicateGraph, vertexS, adj);
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << duration << endl;
    cout << "soda22 modified density: " << resultDen << endl;

    return 0;
}


float soda22exp4(int V, long double totalW, int indicateGraph[], float strength[], float vertexS[], vector <pair <int, float>> adj[]){
    clock_t start, finish;
    double duration;
    float resultDen;
    float maxS=0;
    for (int i=0; i<=V; i++){
        if (strength[i] > maxS){
            maxS = strength[i];
        }
    }

    start = clock();
    resultDen = soda22FlowOrigin(0, maxS, 0.001, V, int(totalW), indicateGraph, vertexS, adj);
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << duration << endl;
    cout << "soda22 origin density: " << resultDen << endl;

    return 0;
}













float greedyPPexp(int V, int vertexNum, long double totalW, int indicateGraph[], float strength[], float vertexS[], vector <pair <int, float>> adj[]){
    clock_t start, finish;
    double duration;
    int T = 3;
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

    start = clock();
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



int unWeightexp(){


    // live journal parameter set
//    string file = "D:\\research\\2020-6-12\\data\\com-lj.ungraph.txt";
//    int startLine = 5;
//    int lineNum = 34681193;
//    int V=4036538;
//    bool fromZero = true;

//    int vertexNum = 0;
//    long double totalW = 0;
//    int* indicateGraph = new int[V+1]{0};
//    float* strength = new float[4036538]{0};
//    float* vertexS = new float[4036538]{0};
//    vector <pair <int, float>>* adj = new vector <pair <int, float>> [V+1];
//    readtxtGraph(file, "	", vertexNum, startLine, lineNum, totalW, indicateGraph, strength, vertexS, adj);
//    DALKSexp(V, vertexNum, 500, 8, totalW, indicateGraph, strength, vertexS, adj);
//    berkleyexp(V, vertexNum, totalW, indicateGraph, strength, vertexS, adj);
//    soda22exp(V, vertexNum, totalW, indicateGraph, strength, vertexS, adj);

     // friendster parameter set
//    V = getV("D:\\research\\2020-6-12\\data\\com-friendster.ungraph.txt", "	", startLine, 1806067139);
//    string file = "D:\\research\\2020-6-12\\data\\com-friendster.ungraph.txt";
//    int startLine = 5;
//    int lineNum = 1806067139;
//    int V=124836179;
//    bool fromZero = false;


    // orkut parameter set
//    string file = "D:\\research\\2020-6-12\\data\\com-orkut.ungraph.txt";
//    int startLine = 5;
//    int V = 3072626;
//    int lineNum = 117185087;
//    bool fromZero = false;


    // com-YouTube parameter set
    string file = "D:\\research\\2020-6-12\\data\\com-youtube.ungraph.txt";
    int startLine = 5;
    int V = 1157827;
    int lineNum = 2987628;
    bool fromZero = false;


    // com-dblp parameter set
//    string file = "D:\\research\\2020-6-12\\data\\com-dblp.ungraph.txt";
//    int startLine = 5;
//    int lineNum = 1049866;
//    int V = 425957;
//    bool fromZero = true;


    // com-Amazon
//    string file = "D:\\research\\2020-6-12\\data\\com-amazon.ungraph.txt";
//    int startLine = 5;
//    int lineNum = 925876;
//    int V=548551;
//    bool fromZero = false;


    // program execution
    int vertexNum = 0;
    long double totalW = 0;
    int* indicateGraph = new int[V+1]{0};
    float* strength = new float[V+1]{0};
    float* vertexS = new float[V+1]{0};
    vector <pair <int, float>>* adj = new vector <pair <int, float>> [V+1];


    readtxtGraph(file, "	", fromZero, vertexNum, startLine, lineNum, totalW, indicateGraph, strength, vertexS, adj);

    DALKSexp(V, vertexNum, 11349, 8, totalW, indicateGraph, strength, vertexS, adj);
//    berkleyexp2(V, vertexNum, totalW, indicateGraph, strength, vertexS, adj);
//    soda22exp2(V, vertexNum, totalW, indicateGraph, strength, vertexS, adj);
//    soda22exp3(V, totalW, indicateGraph, strength, vertexS, adj);
//    soda22exp4(V, totalW, indicateGraph, strength, vertexS, adj);
//    greedyPPexp(V, vertexNum, totalW, indicateGraph, strength, vertexS, adj);


    //program execution for weighted graph



    return 0;
}


int main(){
    unWeightexp();
    return 0;
}





