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
#include "experiment2.h"
using namespace std;



void test(string name, string split){
    int currentLine = 0;
    ifstream infile;
    string line;
    char c[20];
    char* num_str;
    stringstream s;
    int n1, n2;
    float w;

    infile.open(name, ios::in);

    while (getline(infile, line)){
        if (currentLine%10000000 == 0) cout << "Line: " << currentLine << endl;
//        if (currentLine > 500) break;
//        cout << line << endl;

        currentLine += 1;
        strcpy(c, line.c_str());

        num_str = strtok(c, split.c_str());
        s << num_str;
        s >> n1;
        s.clear();
        s.str("");
//        cout << n1 << " ";

        num_str = strtok(NULL, split.c_str());
        s << num_str;
        s >> n2;
        s.clear();
        s.str("");
//        cout << n2 << " ";


        num_str = strtok(NULL, split.c_str());
        s << num_str;
        s >> w;
        s.clear();
        s.str("");
//        cout << w << endl;


    }
    cout << currentLine<< endl;

}



void readWeightGraph(string name, string split, bool fromZero, int& vertexNum, int & edgeNum, int startLine, int lineNum, long double & totalW, int indicateGraph[], float strength[], float vertexS[], vector <pair <int, float>> adj[]){
    ifstream infile;
    string line;
    int n1;
    int n2;
    float w;
    int currentLine=0;
    char c[20];
    char* num_str;
    stringstream s;
    map <int, int> allVertex;

    float maxW=0;
    float minW = FLT_MAX;



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

        num_str = strtok(NULL, split.c_str());
        s << num_str;
        s >> w;
        s.clear();
        s.str("");

        if (fromZero){
            n1 += 1;
            n2 += 1;
        }

        if (n1 < n2){
            adj[n1].push_back(make_pair(n2, w));
            adj[n2].push_back(make_pair(n1, w));
            strength[n1] += w;
            strength[n2] += w;
            totalW += w;
            edgeNum += 1;
        }
        if (n1 == n2){
            strength[n1] += w;
            vertexS[n1] += w;
            totalW += w;
        }
        allVertex.insert(pair<int, int>(n1, 1));
        allVertex.insert(pair<int, int>(n2, 1));
        if (currentLine%10000000 == 0) cout << "Line: " << currentLine << endl;

        if (w > maxW) maxW = w;
        if (w < minW) minW = w;
    }

    for (auto it = allVertex.begin(); it != allVertex.end(); ++it){
        vertexNum += 1;
        indicateGraph[it -> first] = 1;
    }

    cout << "Graph reading finish" << endl;

    cout << "max weight: " << maxW << endl;
    cout << "min weight: " << minW << endl;
    cout << "average weight: " << totalW/edgeNum << endl;
}






double dalksMag(int T){
//    string file = "D:\\research\\2020-6-12\\data\\open-flight.txt";
//    int V = 7976;
//    int startLine = 1;
//    int lineNum = 30501;


//    string file = "D:\\research\\2020-6-12\\data\\OF_one-mode_weightedmsg_sum.txt";
//    int V = 899;
//    int startLine = 1;
//    int lineNum = 142760;

    string file = "D:\\research\\2020-6-12\\data\\test.txt";
    int V = 16726;
    int startLine = 1;
    int lineNum = 95188;





    int oV = V;
    clock_t start1, finish1, start2, finish2;
    double duration1, duration2;

    int vertexNum = 0;
    int edgeNum = 0;
    long double ototalW = 0;
    int* oindicateGraph = new int[V+1]{0};
    float* ostrength = new float[V+1]{0};
    float* vertexS = new float[V+1]{0};
    vector <pair <int, float>>* adj = new vector <pair <int, float>> [V+1];
    readWeightGraph(file, " ", false, vertexNum, edgeNum, startLine, lineNum, ototalW, oindicateGraph, ostrength, vertexS, adj);
    long double totalW = ototalW;
    int* indicateGraph = new int[V+1]{0};
    float* strength = new float[V+1]{0};
    int* optGraph = new int[V+1];
    int* heapPos = new int[V+1];
    int* heap1Pos = new int[V+1];
    pair <float, int> * heap = new pair <float, int> [V+1];
    pair <float, int> * maxheap = new pair <float, int>[V+1];
    // setting parameter
    int j = 8;
    int k = 10;
    // setting parameters
    int topNum = j * k;
    pair <float, int> * heap1 = new pair <float, int> [topNum+1];
    int * heap2Pos = new int[V+1];
    int * heap2Strength = new int[V+1];
    float optDen;


    start1 = clock();
    for (int t=1; t<=T; t++){
        for (int i=0; i<=V; i++){
            indicateGraph[i] = oindicateGraph[i];
            strength[i] = ostrength[i];
        }
        V = oV;
        totalW = ototalW;

        int heapSize = V;
        float density = totalW/V;
        int indicateGraph[V+1];
        for (int i=0; i <= V; i++){
            indicateGraph[i] = 1;
        }

        optDen = density;
        float optW = totalW;
        int optSize = heapSize;
        for (int i=0; i<=V; i++){
            optGraph[i] = indicateGraph[i];
        }


        float thres;
        float outW;
        for (int i=1; i<=V; i++) {
            heap[i] = make_pair(strength[i], i);
            heapPos[i] = i;
        }


//        int maxheapSize = V;
//        for (int i=1; i <= V; i++){
//            indicateGraph[i] = 0;
//        }
//        for (int i=1; i<=V; i++) {
//            maxheap[i] = make_pair(strength[i], i);
//        }
//        for (int i=V/2; i>=1; i--){
//            maxHeapify(i, maxheap, maxheapSize);
//        }
//        pair <float, int> outPair;
//        for (int i = 1; i <= topNum; i++){
//            outPair = heapExtractMax(maxheap, maxheapSize);
//            indicateGraph[outPair.second] = 1;
//        }


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


        int topStrength[V+1];
        for (int i = 0; i <= V; i++){
            topStrength[i] = 0;
        }

        int cpPos = 1;
        totalW = 0;
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


        for (int i=topNum/2; i>=1; i--){
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
            if (strength[i] > optDen) indicateGraph[i] = 1;
            if (indicateGraph[i] > 0) heap2Size += 1;
        }

        pair <float, int> * heap2 = new pair <float, int>[heap2Size+1];
        for (int i=0; i<=V; i++){
            heap2Strength[i] = 0;
        }
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
    }
    finish1 = clock();
    cout << optDen << endl;




    start2 = clock();
    for (int t=1; t<=T; t++){
        totalW = ototalW;
        V = oV;
        int heapSize = V;
        float density = totalW/V;
        for (int i=0; i <= V; i++){
            indicateGraph[i] = oindicateGraph[i];
            strength[i] = ostrength[i];
        }

        optDen = density;
        float optW = totalW;
        int optSize = heapSize;
        for (int i=0; i<=V; i++){
            optGraph[i] = indicateGraph[i];
        }


        float thres;
        float outW;
        float tempS;

        for (int i=1; i<=V; i++) {
            heap[i] = make_pair(strength[i], i);
            heapPos[i] = i;
        }

        for (int i=V; i>=1; i--){
            minHeapify(i, heap, heapSize, heapPos);
        }

        for (int t=1; t<=T; t++){
            pair <float, int> heap[V+1];
            for (int i=1; i<=V; i++) {
                heap[i] = make_pair(strength[i], i);
                heapPos[i] = i;
            }

            for (int i=V; i>=1; i--){
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
                density = totalW/heapSize;
                if (density > optDen){
                    optDen = density;
                    optW = totalW;
                    optSize = heapSize;
                    for (int i=0; i<=V; i++){
                        optGraph[i] = indicateGraph[i];
                    }
                }
            }
        }
    }
    finish2 = clock();
    cout << optDen << endl;


    duration1 = (long double)(finish1 - start1) / CLOCKS_PER_SEC;
    duration2 = (long double)(finish2 - start2) / CLOCKS_PER_SEC;


    cout << "naive dalks: " << duration2 << endl;
    cout << "accelerated dalks: " << duration1 << endl;

    return duration2/duration1;
}






int weightexp(){
    // lbimseti
//    string file = "D:\\research\\2020-6-12\\data\\lbimseti\\edges.csv";
//    int V = 220970;
//    int startLine = 2;
//    int lineNum = 17359347;

    // test
//    string file = "D:\\research\\2020-6-12\\data\\test.txt";
//    int V = 16726;
//    int startLine = 1;
//    int lineNum = 95188;

    // open-flight
    string file = "D:\\research\\2020-6-12\\data\\open-flight.txt";
    int V = 7976;
    int startLine = 1;
    int lineNum = 30501;


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

//    getcsvV(file, ",", startLine, lineNum);
//    readWeightGraph(file, ",", true, vertexNum, edgeNum, startLine, lineNum, totalW, indicateGraph, strength, vertexS, adj);
    readWeightGraph(file, " ", false, vertexNum, edgeNum, startLine, lineNum, totalW, indicateGraph, strength, vertexS, adj);

//    DALKSexp(V, vertexNum, 1000, 8, totalW, indicateGraph, strength, vertexS, adj);
//    berkleyexp2(V, vertexNum, totalW, indicateGraph, strength, vertexS, adj);
//    berkleyexp3(V, indicateGraph, strength, vertexS, adj);
//    soda22exp2(V, vertexNum, totalW, indicateGraph, strength, vertexS, adj);
//    soda22exp3(V, totalW, indicateGraph, strength, vertexS, adj);
//    soda22exp4(V, totalW, indicateGraph, strength, vertexS, adj);
//    greedyPPexp(V, vertexNum, totalW, indicateGraph, strength, vertexS, adj);

    return 0;

}

void fraudarGraph(string name, string split, bool fromZero, int V, int startLine, int lineNum, float fraudar[]){
    ifstream infile;
    string line;
    int n1;
    int n2;
    int currentLine=0;
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
            fraudar[n1] += 1;
            fraudar[n2] += 1;
        }
    }

    for (int i=1; i<=V; i++){
        fraudar[i] = round(10/log(fraudar[i]+5));
    }
}

void readFraudarGraph(string name, string split, bool fromZero, int startLine, int lineNum, int& vertexNum, int & edgeNum, long double & totalW, int indicateGraph[], float fraudar[], float strength[], float vertexS[], vector <pair <int, float>> adj[]){
    ifstream infile;
    string line;
    int n1;
    int n2;
    float w;
    int currentLine=0;
    char c[20];
    char* num_str;
    stringstream s;
    map <int, int> allVertex;

    float maxW=0;
    float minW = FLT_MAX;

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

        w = fraudar[n1] + fraudar[n2];
//        w = 2;

        if (n1 < n2){
            adj[n1].push_back(make_pair(n2, w));
            adj[n2].push_back(make_pair(n1, w));
            strength[n1] += w;
            strength[n2] += w;
            totalW += w;
            edgeNum += 1;
        }
        if (n1 == n2){
            strength[n1] += w;
            vertexS[n1] += w;
            totalW += w;
        }

        allVertex.insert(pair<int, int>(n1, 1));
        allVertex.insert(pair<int, int>(n2, 1));
        if (currentLine%10000000 == 0) cout << "Line: " << currentLine << endl;

        if (w > maxW) maxW = w;
        if (w < minW) minW = w;
    }

    for (auto it = allVertex.begin(); it != allVertex.end(); ++it){
        vertexNum += 1;
        indicateGraph[it -> first] = 1;
    }

    cout << "Graph reading finish" << endl;

    cout << "max weight: " << maxW << endl;
    cout << "min weight: " << minW << endl;
    cout << "average weight: " << totalW/edgeNum << endl;

}


int weightexp2(){
//    string file = "D:\\research\\2020-6-12\\data\\com-dblp.ungraph.txt";
//    int startLine = 5;
//    int lineNum = 1049866;
//    int V = 425957;
//    bool fromZero = true;

    // live journal parameter set
    string file = "D:\\research\\2020-6-12\\data\\com-lj.ungraph.txt";
    int startLine = 5;
    int lineNum = 34681193;
    int V=4036538;
    bool fromZero = true;

//    string file = "D:\\research\\2020-6-12\\data\\com-orkut.ungraph.txt";
//    int startLine = 5;
//    int V = 3072626;
//    int lineNum = 117185087;
//    bool fromZero = false;

    // com-YouTube parameter set
//    string file = "D:\\research\\2020-6-12\\data\\com-youtube.ungraph.txt";
//    int startLine = 5;
//    int V = 1157827;
//    int lineNum = 2987628;
//    bool fromZero = false;



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

    //DALKSexp(V, vertexNum, 5000, 8, totalW, indicateGraph, strength, vertexS, adj);
    //berkleyexp2(V, vertexNum, totalW, indicateGraph, strength, vertexS, adj);
    //berkleyexp3(V, indicateGraph, strength, vertexS, adj);
    //soda22exp2(V, vertexNum, totalW, indicateGraph, strength, vertexS, adj);
    //soda22exp3(V, totalW, indicateGraph, strength, vertexS, adj);
    //greedyPPexp(V, vertexNum, totalW, indicateGraph, strength, vertexS, adj);


    return 0;
}


//int main(){
////    weightexp();
////    double ratio = dalksMag(100);
////    cout << ratio << endl;
//    weightexp2();
//    return 0;
//}





