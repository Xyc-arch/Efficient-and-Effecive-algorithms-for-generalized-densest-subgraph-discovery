#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <cstring>
#include <sstream>
#include <bits/stdc++.h>
#include <cmath>
#include <math.h>
#include "experiment3.h"
using namespace std;



void readDenoGraph(string name, string split, bool fromZero, int& vertexNum, int V, int startLine, int lineNum, long double & totalEW, long double & totalVW, int indicateGraph[], float strength[], float vertexS[], vector <pair <int, float>> adj[]){
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
        if (n1 != n2){
            adj[n1].push_back(make_pair(n2+V, 1));
            adj[n2+V].push_back(make_pair(n1, 1));
            strength[n1] += 1;
            strength[n2+V] += 1;
            totalEW += 1;
        }
    allVertex.insert(pair<int, int>(n1, 1));
    allVertex.insert(pair<int, int>(n2+V, 1));
    if (currentLine%10000000 == 0) cout << "Line: " << currentLine << endl;
    }

    for (auto it = allVertex.begin(); it != allVertex.end(); ++it){
        vertexNum += 1;
        indicateGraph[it -> first] = 1;
        if ((it -> first) <= V){
            totalVW += 1;
            vertexS[it->first] = 1;
        } else {
            totalVW += 0.25;
            vertexS[it->first] = 0.25;
        }
    }

    cout << "Graph reading finish" << endl;
}

float denoBerkley2(int V, int vertexNum, long double totalVW, long double totalEW, int indicateGraph[], float strength[], float vertexS[], vector <pair <int, float>> adj[]){
    clock_t start, finish;
    double duration;

    float maxS=0;
    int heapSize = vertexNum;
    int* heapPos = new int[2*V+1];
    pair<float, float> temp;
    float densityMax = totalEW/totalVW;
    for (int i=0; i<=2*V; i++){
        if (indicateGraph[i]>0 && strength[i] > maxS){
            maxS = strength[i];
        }
    }
    maxS = 4*maxS;
    int* rawIndicateGraph= new int[2*V+1]{0};
    for (int i=0; i<=2*V; i++){
        rawIndicateGraph[i] = indicateGraph[i];
    }

    start = clock();
    int cp = 1;

    pair <float, int>* heap = new pair <float, int>[heapSize+1];
    for (int i=1; i<=2*V; i++) {
        if (indicateGraph[i] > 0){
            if (i <= V) heap[cp] = make_pair(strength[i], i);
            else heap[cp] = make_pair(4*strength[i], i);
            heapPos[i] = cp;
            cp += 1;
        }
    }

    for (int i=heapSize/2; i>=1; i--){
        minHeapify(i, heap, heapSize, heapPos);
    }


    while (heapSize > 1){
        temp = heapExtractMinDeno(heap, heapSize, V, heapPos, adj, indicateGraph);
        totalVW -= temp.first;
        totalEW -= temp.second;
        if (densityMax < totalEW/totalVW) {
            densityMax = totalEW/totalVW;
        }
    }


    float thres;
    thres = densityMax;



    delete [] heap;
    delete [] heapPos;
    heapPos = new int[2*V+1]{0};
    heapSize = vertexNum;
    heap = new pair <float, int>[heapSize+1];
    for (int i=0; i<=2*V; i++){
        indicateGraph[i] = rawIndicateGraph[i];
    }
    cp = 1;
    for (int i=1; i<=2*V; i++) {
        if (indicateGraph[i] > 0){
            if (i <= V) heap[cp] = make_pair(strength[i], i);
            else heap[cp] = make_pair(4*strength[i], i);
            heapPos[i] = cp;
            cp += 1;
        }
    }

    for (int i=heapSize/2; i>=1; i--){
        minHeapify(i, heap, heapSize, heapPos);
    }

    while (heap[1].first < thres){
        heapExtractMinDeno(heap, heapSize, V, heapPos, adj, indicateGraph);
    }

    cout << "Heap size " << heapSize << endl;
    cout << "threshold " << thres << endl;
    cout << "max strength " << maxS << endl;


    float d;
    for (int i=0; i<=2*V; i++){
        strength[i] = 0;
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

    d = BerkleyDeno(thres, maxS, 0.001, V, indicateGraph, strength, vertexS, adj);
    finish = clock();
    cout <<"optimal density: "<< d << endl;
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "Time of accelerated berkley: " << duration << endl;
    return 0;
}




float denoBerkley3(int V, int indicateGraph[], float strength[], float vertexS[], vector <pair <int, float>> adj[]){
    clock_t start, finish;
    double duration;
    float resultDen;
    float maxS=0;
    for (int i=1; i<=2*V; i++){
        if (strength[i] > maxS){
            maxS = strength[i];
        }
    }
    maxS = 4*maxS;

    start = clock();
    resultDen = BerkleyDeno(0, maxS, 0.001, V, indicateGraph, strength, vertexS, adj);
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << duration << endl;
    cout << "berkley density: " << resultDen << endl;

    return resultDen;
}


int denoExp(){
//    int V = 8297;
//    string file = "D:\\research\\2020-6-12\\data\\Wiki-Vote.txt";
//    int startLine = 5;
//    int lineNum = 103693;
//    bool fromZero = false;

//    int V = 6301;
//    string file = "D:\\research\\2020-6-12\\data\\p2p-Gnutella08.txt";
//    int startLine = 5;
//    int lineNum = 20781;
//    bool fromZero = true;

//    int V = 281903;
//    string file = "D:\\research\\2020-6-12\\data\\web-Stanford.txt";
//    int startLine = 5;
//    int lineNum = 2312501;
//    bool fromZero = false;


    int V = 325729;
    string file = "D:\\research\\2020-6-12\\data\\web-NotreDame.txt";
    int startLine = 5;
    int lineNum = 1497138;
    bool fromZero = true;
//    int V = getV(file, "	", startLine, lineNum);
//    cout << V << endl;

   // program execution
    int vertexNum = 0;
    long double totalEW = 0;
    long double totalVW = 0;
    int* indicateGraph = new int[2*V+1]{0};
    float* strength = new float[2*V+1]{0};
    float* vertexS = new float[2*V+1]{0};
    vector <pair <int, float>>* adj = new vector <pair <int, float>> [2*V+1];
    float resultDen;


    readDenoGraph(file, "	", fromZero, vertexNum, V, startLine, lineNum, totalEW, totalVW, indicateGraph, strength, vertexS, adj);
//    cout << totalEW/totalVW << endl;


    resultDen = denoBerkley3(V, indicateGraph, strength, vertexS, adj);
//    resultDen = denoBerkley2(V, vertexNum, totalVW, totalEW, indicateGraph, strength, vertexS, adj);


    return 0;
}




//int main(){
//    denoExp();
//    return 0;
//}



