#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <cstring>
#include <sstream>
#include <bits/stdc++.h>
#include <math.h>
#include "experiment2.h"
#include "dinic.h"


int ccorexp(int V, int vertexNum, int edgeNum, long double totalW, int indicateGraph[], float strength[], float vertexS[], vector <pair <int, float>> adj[]){
    int remainNum = vertexNum;
    float maxS;
    float guess;
    vector <int> dsg;
    vector <int> tempDS;
    Graph g(V+2);
    vector <pair <int, float>> adjacent;
    float m;
    float eps = 0.001;
    float l, u;
    float densityMax;
    float tempS;
    float thres;
    int heapSize;
    int cp;
    int levelNum = 0;
    clock_t start, finish;
    double duration;
    long double localTotalW;

    start = clock();

    while (remainNum > 0 && levelNum <= 20){
        heapSize = remainNum;
        int* heapPos = new int[V+1];
        int* localIndGraph= new int[V+1]{0};
        float* localStrength = new float[V+1]{0};
        for (int i=0; i<= V; i++){
            localIndGraph[i] = indicateGraph[i];
            localStrength[i] = strength[i];
        }
        pair <float, int>* heap = new pair <float, int>[heapSize+1];
        cp = 1;
        localTotalW = totalW;
        densityMax = localTotalW/heapSize;


        for (int i=1; i<=V; i++) {
            if (localIndGraph[i] > 0){
                heap[cp] = make_pair(localStrength[i], i);
                heapPos[i] = cp;
                cp += 1;
            }
        }

        for (int i=heapSize/2; i>=1; i--){
            minHeapify(i, heap, heapSize, heapPos);
        }

        while (heapSize > 1){
            tempS = heapExtractMin(heap, heapSize, heapPos, adj, localIndGraph);
            localTotalW -= tempS;
            if (densityMax < localTotalW/heapSize) {
                densityMax = localTotalW/heapSize;
            }
        }

        thres = densityMax;

        delete [] heap;
        delete [] heapPos;

        heapPos = new int[V+1]{0};
        heapSize = remainNum;
        heap = new pair <float, int>[heapSize+1];
        for (int i=0; i<=V; i++){
            localIndGraph[i] = indicateGraph[i];
        }
        cp = 1;

        for (int i=1; i<=V; i++) {
            if (localIndGraph[i] > 0){
                heap[cp] = make_pair(localStrength[i], i);
                heapPos[i] = cp;
                cp += 1;
            }
        }

        for (int i=heapSize/2; i>=1; i--){
            minHeapify(i, heap, heapSize, heapPos);
        }

        while (heap[1].first < thres){
            heapExtractMin(heap, heapSize, heapPos, adj, localIndGraph);
        }

        for (int i=0; i<=V; i++){
            localStrength[i] = vertexS[i];
        }

        float sum;
        vector<pair<int, float>> tp1;
        for (int i=1; i<=heapSize; i++){
            sum = 0;
            tp1 = adj[heap[i].second];
            for (int j=0; j<int(tp1.size()); j++){
                if (localIndGraph[tp1[j].first] > 0){
                    sum += tp1[j].second;
                }
            }
            localStrength[heap[i].second] += sum;
        }

        maxS = 0;
        for (int i=0; i<=V; i++){
            if (localIndGraph[i]>0 && localStrength[i] > maxS){
                maxS = localStrength[i];
            }
        }

        u = maxS;
        l = 0;


        while((u-l)>eps){
            g.renew();
            guess = (u+l)*0.5;

            m = 0;
            for (int i=0; i<=V; i++){
                if (localIndGraph[i] == 0) continue;
                if (localStrength[i] + vertexS[i]-2*guess > m) m = localStrength[i] + vertexS[i]-2*guess;
            }
//            m = edgeNum;

            for (int i=1; i<=V; i++){
                if (localIndGraph[i] == 0) continue;
                g.addEdge(0, i, m);
                g.addEdge(i, V+1, m+2*guess-localStrength[i]-vertexS[i]);
                adjacent = adj[i];
                for (int j=0; j<int(adjacent.size()); j++){
                    if (localIndGraph[adjacent[j].first] == 0) continue;
                    if (adjacent[j].first > i){
                        g.addEdge(i, adjacent[j].first, adjacent[j].second);
                        g.addEdge(adjacent[j].first, i, adjacent[j].second);
                    }
                }
            }


            g.DinicMaxflow(0, V+1);
            dsg = g.Cut(0);
            if (dsg.size() <= 1) {
                u = guess;
            } else {
                l = guess;
                tempDS = dsg;
            }
        }

        int* dsIndGraph= new int[V+1]{0};
        int vRm;
        for (int i=1; i<(int)(tempDS.size()); i++){
            if (tempDS[i] == 0) continue;
            vRm = tempDS[i];
            dsIndGraph[vRm] = 1;
        }
        for (int i=1; i<(int)(tempDS.size()); i++){
            if (tempDS[i] == 0) continue;
            vRm = tempDS[i];
            adjacent = adj[vRm];
            for (int j=0; j<(int)(adjacent.size()); j++){
                pair <int, float> adjPair = adjacent[j];
                if (indicateGraph[adjPair.first] > 0){
                    vertexS[adjPair.first] += adjPair.second;
                    if (dsIndGraph[adjPair.first] > 0) totalW -= adjPair.second;
                }
            }
            totalW -= vertexS[vRm];
            indicateGraph[vRm] = 0;
        }
        remainNum = remainNum - (int)(tempDS.size()) + 1;
        delete [] localIndGraph;
        delete [] localStrength;
        delete [] dsIndGraph;
        cout << "density: " << l << " #remain " << remainNum << endl;
        levelNum += 1;
//        break;
    }

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "ccore decomp time cost: " << duration << endl;

    return 0;
}

int decompexp(int V, int vertexNum, int edgeNum, long double totalW, int indicateGraph[], float strength[], float vertexS[], vector <pair <int, float>> adj[]){
    int remainNum = vertexNum;
    int levelNum = 0;
    float maxS;
    for (int i=0; i<=V; i++){
        if (indicateGraph[i]>0 && strength[i] > maxS){
            maxS = strength[i];
        }
    }
    float guess;
    vector <int> dsg;
    vector <int> tempDS;
    Graph g(V+2);
    vector <pair <int, float>> adjacent;
    float m;
    float eps = 0.001;
    float l, u;
    float maxFlow;
    clock_t start, finish;
    double duration;

    start = clock();

    while (remainNum > 0 && levelNum <= 20){
        u = maxS;
        l = 0;


        while((u-l)>eps){
            g.renew();
            guess = (u+l)*0.5;

            m = 0;
            for (int i=0; i<=V; i++){
                if (indicateGraph[i] == 0) continue;
                if (strength[i] + vertexS[i]-2*guess > m) m = strength[i] + vertexS[i]-2*guess;
            }
//            m = edgeNum;

            for (int i=1; i<=V; i++){
                if (indicateGraph[i] == 0) continue;
                g.addEdge(0, i, m);
                g.addEdge(i, V+1, m+2*guess-strength[i]-vertexS[i]);
//                g.addEdge(0, i, m);
//                g.addEdge(i, V+1, m+2*guess-strength[i]);
                adjacent = adj[i];
                for (int j=0; j<int(adjacent.size()); j++){
                    if (indicateGraph[adjacent[j].first] == 0) continue;
                    if (adjacent[j].first > i){
                        g.addEdge(i, adjacent[j].first, adjacent[j].second);
                        g.addEdge(adjacent[j].first, i, adjacent[j].second);
                    }
                }
            }


            maxFlow = g.DinicMaxflow(0, V+1);
            dsg = g.Cut(0);
            if (dsg.size() <= 1) {
                u = guess;
            } else {
                l = guess;
                tempDS = dsg;
                // g.printFinalGraph();
                // cout << "#DS: " << tempDS.size() << " density " << l << " m " << m << endl;
            }

        }


        int vRm;
        for (int i=1; i<(int)(tempDS.size()); i++){
            if (tempDS[i] == 0) continue;
            vRm = tempDS[i];
            adjacent = adj[vRm];
//            cout << vRm << endl;
            for (int j=0; j<(int)(adjacent.size()); j++){
                pair <int, float> adjPair = adjacent[j];
                if (indicateGraph[adjPair.first] > 0){
                    vertexS[adjPair.first] += adjPair.second;
                }
            }
            indicateGraph[vRm] = 0;
        }
        remainNum = remainNum - (int)(tempDS.size()) + 1;
        cout << "density: " << l << " #remain " << remainNum << endl;
        levelNum += 1;
//        cout << "DS cut has:" << endl;
//        for (int i=0; i<(int)(tempDS.size()); i++){
//            cout << tempDS[i] << endl;
//        }
//        cout << "max flow: " << maxFlow << endl;
//        cout << "pseudo max flow: " << g.getCutFlow(tempDS) << endl;
//        break;
    }

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "decomp time cost: " << duration << endl;

    return 0;
}






int myexp(){
    // toy3
//    string file = "D:\\research\\2020-6-12\\data\\toy3.txt";
//    int V = 3;
//    int startLine = 1;
//    int lineNum = 2;
//    bool fromZero = false;

//    // toy2
//    string file = "D:\\research\\2020-6-12\\data\\toy2.txt";
//    int V = 4;
//    int startLine = 1;
//    int lineNum = 5;
//    bool fromZero = false;

//    // toy
//    string file = "D:\\research\\2020-6-12\\data\\toy.txt";
//    int V = 5;
//    int startLine = 1;
//    int lineNum = 6;
//    bool fromZero = false;

//    // test
//    string file = "D:\\research\\2020-6-12\\data\\test.txt";
//    int V = 16726;
//    int startLine = 1;
//    int lineNum = 95188;
//    bool fromZero = false;

    // live journal parameter set
    string file = "D:\\research\\2020-6-12\\data\\com-lj.ungraph.txt";
    int startLine = 5;
    int lineNum = 34681193;
    int V=4036538;
    bool fromZero = true;


    // parameter preparation
    int vertexNum = 0;
    int edgeNum = 0;
    long double totalW = 0;
    int* indicateGraph = new int[V+1]{0};
    float* strength = new float[V+1]{0};
    float* vertexS = new float[V+1]{0};
    vector <pair <int, float>>* adj = new vector <pair <int, float>> [V+1];

//    readWeightGraph(file, " ", fromZero, vertexNum, edgeNum, startLine, lineNum, totalW, indicateGraph, strength, vertexS, adj);



//    readtxtGraph(file, " ", fromZero, vertexNum, edgeNum, startLine, lineNum, totalW, indicateGraph, strength, vertexS, adj);
    readtxtGraph(file, "	", fromZero, vertexNum, edgeNum, startLine, lineNum, totalW, indicateGraph, strength, vertexS, adj);

    cout << "total number of vertices " << vertexNum << endl;
    cout << "total number of edges " << edgeNum << endl;
//    decompexp(V, vertexNum, edgeNum, totalW, indicateGraph, strength, vertexS, adj);
    ccorexp(V, vertexNum, edgeNum, totalW, indicateGraph, strength, vertexS, adj);

    return 0;
}


//int main(){
//    myexp();
//    return 0;
//}
