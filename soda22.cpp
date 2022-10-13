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

float soda22Flow(float l, float u, float eps, int V, int edgeNum, int indicateGraph[], float vertexS[], vector <pair <int, float>> adj[]){
    float lambda=(u+l)*0.5;
    float W;
    Graph g(V+edgeNum+2);
    map <pair<int, int>, int> code;
    vector<pair<int, float>> adjacent;
    pair<int, float> tempPair;
    while (u-l>eps){
        W = 0;
        g.renew();
        code.erase(code.begin(), code.end());

        int pos = V+1;
        lambda = (u+l)*0.5;
        for (int i=1; i<=V; i++){
            if (indicateGraph[i] > 0){
                adjacent = adj[i];
                if (vertexS[i] != 0){
                    g.addEdge(0, i, vertexS[i]);
                }
                g.addEdge(i, V+edgeNum+1, lambda);
                for (int j=0; j<int(adjacent.size()); j++){
                    tempPair = adjacent[j];
                    if (indicateGraph[tempPair.first] > 0 && i<tempPair.first){
                        code[make_pair(i, tempPair.first)] = pos;
                        g.addEdge(pos, i, FLT_MAX);
                        g.addEdge(pos, tempPair.first, FLT_MAX);
                        g.addEdge(0, pos, tempPair.second);
                        W += tempPair.second;
                        pos += 1;
                    }
                }
            }
        }
        float total = g.DinicMaxflow(0, V+edgeNum+1);
        if (total < W){
            l = lambda;
        } else {
            u = lambda;
        }
    }
//    vector <int> dsg;
//    dsg = g.Cut(0);
//    int idx;
//    for (int i=0; i<int(dsg.size()); i++){
//        idx = dsg[i];
//        if (idx <= V){
//            cout << idx << endl;
//        }
//    }

    return lambda;
}


float soda22FlowApp(float l, float u, float eps, int V, int edgeNum, int indicateGraph[], float vertexS[], vector <pair <int, float>> adj[]){
    float lambda=(u+l)*0.5;
    float eps0 = (lambda - l)/(2*lambda);
    long double W;
    Graph g(V+edgeNum+2);
    map <pair<int, int>, int> code;
    vector<pair<int, float>> adjacent;
    pair<int, float> tempPair;
    while (eps0>eps/(3-2*eps)){
        W = 0;
        g.renew();
        code.erase(code.begin(), code.end());

        int pos = V+1;
        lambda = (u+l)*0.5;
        eps0 = (lambda-l)/(2*lambda);
        for (int i=1; i<=V; i++){
            if (indicateGraph[i] > 0){
                adjacent = adj[i];
                if (vertexS[i] != 0){
                    g.addEdge(0, i, vertexS[i]);
                }
                g.addEdge(i, V+edgeNum+1, lambda);
                for (int j=0; j<int(adjacent.size()); j++){
                    tempPair = adjacent[j];
                    if (indicateGraph[tempPair.first] > 0 && i<tempPair.first){
                        code[make_pair(i, tempPair.first)] = pos;
                        g.addEdge(pos, i, FLT_MAX);
                        g.addEdge(pos, tempPair.first, FLT_MAX);
                        g.addEdge(0, pos, tempPair.second);
                        W += tempPair.second;
                        pos += 1;
                    }
                }
            }
        }
        long double total = g.soda22MaxflowApp(0, V+edgeNum+1, eps0, W);
        if (total < W){
            l = (lambda+l)/2;
        } else {
            u = lambda;
        }
        cout << "The current gap: " << u-l << endl;
    }
    return lambda;
}


float soda22FlowAcc(float l, float u, float eps, int V, int edgeNum, int & heapSize, int heapPos[], int indicateGraph[], float vertexS[], float strength[], pair <float, int> heap[], vector <pair <int, float>> adj[]){
    float lambda=(u+l)*0.5;
    float eps0 = (lambda - l)/(2*lambda);
    long double W;
    Graph g(V+edgeNum+2);
    map <pair<int, int>, int> code;
    vector<pair<int, float>> adjacent;
    pair<int, float> tempPair;
    while (eps0>eps/(3-2*eps)){
        W = 0;
        g.renew();
        code.erase(code.begin(), code.end());

        int pos = V+1;
        lambda = (u+l)*0.5;
        eps0 = (lambda-l)/(2*lambda);
        for (int i=1; i<=V; i++){
            if (indicateGraph[i] > 0){
                adjacent = adj[i];
                if (vertexS[i] != 0){
                    g.addEdge(0, i, vertexS[i]);
                }
                g.addEdge(i, V+edgeNum+1, lambda);
                for (int j=0; j<int(adjacent.size()); j++){
                    tempPair = adjacent[j];
                    if (indicateGraph[tempPair.first] > 0 && i<tempPair.first){
                        code[make_pair(i, tempPair.first)] = pos;
                        g.addEdge(pos, i, FLT_MAX);
                        g.addEdge(pos, tempPair.first, FLT_MAX);
                        g.addEdge(0, pos, tempPair.second);
                        W += tempPair.second;
                        pos += 1;
                    }
                }
            }
        }
        long double total = g.soda22MaxflowApp(0, V+edgeNum+1, eps0, W);
        if (total < W){
            l = (lambda+l)/2;
            while (heap[1].first < l){
                heapExtractMin(heap, heapSize, heapPos, adj, indicateGraph, strength);
            }
        } else {
            u = lambda;
        }
//        cout << "The current gap: " << u-l <<endl;
    }
    return lambda;
}





float soda22FlowOrigin(float l, float u, float eps, int V, int edgeNum, int indicateGraph[], float vertexS[], vector <pair <int, float>> adj[]){
    float lambda;
    float eps0 = 0.5;
    long double W;
    Graph g(V+edgeNum+2);
    map <pair<int, int>, int> code;
    vector<pair<int, float>> adjacent;
    pair<int, float> tempPair;
    while (eps0>eps/2){
        W = 0;
        g.renew();
        code.erase(code.begin(), code.end());

        int pos = V+1;
        lambda = (u+l)*0.5;
        for (int i=1; i<=V; i++){
            if (indicateGraph[i] > 0){
                adjacent = adj[i];
                if (vertexS[i] != 0){
                    g.addEdge(0, i, vertexS[i]);
                }
                g.addEdge(i, V+edgeNum+1, lambda);
                for (int j=0; j<int(adjacent.size()); j++){
                    tempPair = adjacent[j];
                    if (indicateGraph[tempPair.first] > 0 && i<tempPair.first){
                        code[make_pair(i, tempPair.first)] = pos;
                        g.addEdge(pos, i, FLT_MAX);
                        g.addEdge(pos, tempPair.first, FLT_MAX);
                        g.addEdge(0, pos, tempPair.second);
                        W += tempPair.second;
                        pos += 1;
                    }
                }
            }
        }
        long double total = g.soda22MaxflowApp(0, V+edgeNum+1, eps0, W);
        if (total < W){
            if ((1-eps0)*lambda <= l){
                eps0 = eps0/2;
            } else {
                l = (1-eps0)*lambda;
            }
        } else {
            u = lambda;
            if ((u-l)/u < eps0){
                eps0 = eps0/2;
            }
        }
//        cout << "The current gap: " << u-l << endl;
    }
    return lambda;
}



float soda22FlowOriginAcc(float l, float u, float eps, int V, int edgeNum, int & heapSize, int heapPos[], int indicateGraph[], float vertexS[], float strength[], pair <float, int> heap[], vector <pair <int, float>> adj[]){
    float lambda;
    float eps0 = 0.5;
    long double W;
    Graph g(V+edgeNum+2);
    map <pair<int, int>, int> code;
    vector<pair<int, float>> adjacent;
    pair<int, float> tempPair;
//    int count = 0;
    while (eps0>eps/2){
        W = 0;
        g.renew();
        code.erase(code.begin(), code.end());

        int pos = V+1;
        lambda = (u+l)*0.5;
        for (int i=1; i<=V; i++){
            if (indicateGraph[i] > 0){
                adjacent = adj[i];
                if (vertexS[i] != 0){
                    g.addEdge(0, i, vertexS[i]);
                }
                g.addEdge(i, V+edgeNum+1, lambda);
                for (int j=0; j<int(adjacent.size()); j++){
                    tempPair = adjacent[j];
                    if (indicateGraph[tempPair.first] > 0 && i<tempPair.first){
                        code[make_pair(i, tempPair.first)] = pos;
                        g.addEdge(pos, i, FLT_MAX);
                        g.addEdge(pos, tempPair.first, FLT_MAX);
                        g.addEdge(0, pos, tempPair.second);
                        W += tempPair.second;
                        pos += 1;
                    }
                }
            }
        }
        long double total = g.soda22MaxflowApp(0, V+edgeNum+1, eps0, W);
        if (total < W){
            if ((1-eps0)*lambda <= l){
                eps0 = eps0/2;
            } else {
                l = (1-eps0)*lambda;
                while (heap[1].first < l){
                    heapExtractMin(heap, heapSize, heapPos, adj, indicateGraph, strength);
                }
            }
        } else {
            u = lambda;
            if ((u-l)/u < eps0){
                eps0 = eps0/2;
            }
        }
//        cout << "The current gap: " << u-l << endl;
    }
    return lambda;
}








int soda22(){
    clock_t start1, finish1, start2, finish2;
    double duration1, duration2;
    ifstream infile;
    infile.open("D:\\research\\2020-6-12\\data\\OF_one-mode_weightedmsg_sum.txt", ios::in);
    int count=0;
    int V = 16726;
    vector <pair <int, float>> adj[V+1];
    float strength[V+1];
    for (int i=0; i<=V; i++){
        strength[i] = 0;
    }
    float vertexS[V+1];
    for (int i=1; i<=V; i++) {
        vertexS[i] = 0;
    }
    float totalW=0;
    int edgeNum = 0;


    string line;
    char c[20];
    char* num_str;
    stringstream s;
    int n1;
    int n2;
    float w;
    float resultDen;

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
            edgeNum += 1;
        } else if (n1 == n2){
            strength[n1] += w;
            vertexS[n1] += w;
            totalW += w;
        }
    }


    int heapSize = V;
    int heapPos[V+1];
    float dmax=0;
    float tempS;
    float densityMax = totalW/V;
    int indicateGraph[V+1];
    for (int i=0; i <= V; i++){
        indicateGraph[i] = 1;
    }

    float maxS=0;
    for (int i=0; i<=V; i++){
        if (strength[i] > maxS){
            maxS = strength[i];
        }
    }

    start1 = clock();
    resultDen = soda22FlowApp(0, maxS, 0.001, V, edgeNum, indicateGraph, vertexS, adj);
    finish1 = clock();
    duration1 = (double)(finish1 - start1) / CLOCKS_PER_SEC;
    cout << resultDen << endl;


    start2 = clock();
    resultDen = soda22FlowOrigin(0, maxS, 0.001, V, edgeNum, indicateGraph, vertexS, adj);
    finish2 = clock();
    duration2 = (double)(finish2 - start2) / CLOCKS_PER_SEC;
    cout << resultDen << endl;

    float ratio = duration2/duration1;
    cout << "Ratio is " << ratio << endl;

//******************************************second part
    start2 = clock();
    pair <float, int> heap[V+1];
    for (int i=1; i<=V; i++) {
        heap[i] = make_pair(strength[i], i);
        heapPos[i] = i;
    }

    for (int i=V/2; i>=1; i--){
        minHeapify(i, heap, heapSize, heapPos);
    }


    while (heap[1].first < densityMax or heap[1].first < 0.5*dmax){
        tempS = heapExtractMin(heap, heapSize, heapPos, adj, indicateGraph);
        totalW -= tempS;
        if (tempS > dmax) dmax = tempS;
        if (densityMax < totalW/heapSize) densityMax = totalW/heapSize;
    }


    int thres;
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

//******************************************test
//    vector<pair<int, float>> adjacent;
//    pair<int, float> tempPair;
//    int pos = V+1;
//    for (int i=1; i<=V; i++){
//        if (indicateGraph[i] > 0){
//            adjacent = adj[i];
//            if (vertexS[i] != 0){
//                g.addEdge(0, i, vertexS[i]);
//            }
//            g.addEdge(i, V+edgeNum+1, lambda);
//            for (int j=0; j<int(adjacent.size()); j++){
//                tempPair = adjacent[j];
//                if (indicateGraph[tempPair.first] > 0 && i<tempPair.first){
//                    code[make_pair(i, tempPair.first)] = pos;
//                    g.addEdge(pos, i, FLT_MAX);
//                    g.addEdge(pos, tempPair.first, FLT_MAX);
//                    g.addEdge(0, pos, tempPair.second);
//                    pos += 1;
//                }
//            }
//        }
//    }

//    float total = g.DinicMaxflow(0, V+edgeNum+1);
//    cout << total << endl;
//    vector <int> dsg;
//    dsg = g.Cut(0);
//    int idx;
//    for (int i=0; i<int(dsg.size()); i++){
//        idx = dsg[i];
//        if (idx <= V){
//            cout << idx << endl;
//        }
//    }
//******************************************test


//    resultDen = soda22FlowApp(thres, maxS, 0.1, V, edgeNum, indicateGraph, vertexS, adj);
//    resultDen = soda22FlowAcc(thres, maxS, 0.001, V, edgeNum, heapSize, heapPos, indicateGraph, vertexS, strength, heap, adj);
//    finish2 = clock();
//    duration2 = (double)(finish2 - start2) / CLOCKS_PER_SEC;
//    cout << resultDen << endl;

//    return duration1/duration2;
    return 0;
}

//int main(){
//    float ratio;
//    ratio = soda22();
////    cout << "Ratio is " << ratio <<endl;
//    return 0;
//}


