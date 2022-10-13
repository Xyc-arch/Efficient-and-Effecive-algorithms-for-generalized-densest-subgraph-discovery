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



float BerkleyFlow(float l, float u, float eps, int V, int indicateGraph[], float strength[], float vertexS[], vector <pair <int, float>> adj[]){
    float guess;
    vector <int> dsg;
    Graph g(V+2);
    vector <pair <int, float>> adjacent;
    float m=0;

    while((u-l)>eps){
        g.renew();
        guess = (u+l)*0.5;
        for (int i=0; i<=V; i++){
            if (indicateGraph[i] == 0) continue;
            if (strength[i] + vertexS[i]-2*guess > m) m = strength[i] + vertexS[i]-2*guess;
        }


        for (int i=1; i<=V; i++){
            if (indicateGraph[i] == 0) continue;
            g.addEdge(0, i, m);
            g.addEdge(i, V+1, m+2*guess-strength[i]-vertexS[i]);
            adjacent = adj[i];
            for (int j=0; j<int(adjacent.size()); j++){
                if (indicateGraph[adjacent[j].first] == 0) continue;
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
        }
        cout << "The current gap: " << u-l << endl;
    }



    dsg = g.Cut(0);
    return guess;

}

float BerkleyDeno(float l, float u, float eps, int V, int indicateGraph[], float strength[], float vertexS[], vector <pair <int, float>> adj[]){
    float guess;
    vector <int> dsg;
    Graph g(2*V+2);
    vector <pair <int, float>> adjacent;
    float m=0;

    while((u-l)>eps){
        g.renew();
        guess = (u+l)*0.5;
        for (int i=1; i<=2*V; i++){
            if (indicateGraph[i] == 0) continue;
            if (strength[i]-2*guess*vertexS[i] > m) m = strength[i]-2*guess*vertexS[i];
        }


        for (int i=1; i<=2*V; i++){
            if (indicateGraph[i] == 0) continue;
            g.addEdge(0, i, m);
            g.addEdge(i, 2*V+1, m+2*guess*vertexS[i]-strength[i]);
            adjacent = adj[i];
            for (int j=0; j<int(adjacent.size()); j++){
                if (indicateGraph[adjacent[j].first] == 0) continue;
                if (adjacent[j].first > i){
                    g.addEdge(i, adjacent[j].first, adjacent[j].second);
                    g.addEdge(adjacent[j].first, i, adjacent[j].second);
                }
            }
        }


        g.DinicMaxflow(0, 2*V+1);
        dsg = g.Cut(0);
        if (dsg.size() <= 1) {
            u = guess;
        } else {
            l = guess;
        }
        cout << "The current gap: " << u-l << endl;
    }



    dsg = g.Cut(0);
    return guess;

}

float BerkleyFlowAcc(float l, float u, float eps, int V, int & heapSize, int heapPos[], int indicateGraph[], float strength[], float vertexS[], pair <float, int> heap[], vector <pair <int, float>> adj[]){
    float guess;
    vector <int> dsg;
    Graph g(V+2);
    vector <pair <int, float>> adjacent;
    float m=0;

    while((u-l)>eps){
        g.renew();
        guess = (u+l)*0.5;
        for (int i=0; i<=V; i++){
            if (indicateGraph[i] == 0) continue;
            if (strength[i] + vertexS[i]-2*guess > m) m = strength[i] + vertexS[i]-2*guess;
        }


        for (int i=1; i<=V; i++){
            if (indicateGraph[i] == 0) continue;
            g.addEdge(0, i, m);
            g.addEdge(i, V+1, m+2*guess-strength[i]-vertexS[i]);
            adjacent = adj[i];
            for (int j=0; j<int(adjacent.size()); j++){
                if (indicateGraph[adjacent[j].first] == 0) continue;
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
            while (heap[1].first < l){
                heapExtractMin(heap, heapSize, heapPos, adj, indicateGraph, strength);
            }
        }
//        cout << "The current gap: " << u-l << endl;
    }



    dsg = g.Cut(0);
    return guess;

}




float berkleyCompare(){
    clock_t start1, finish1, start2, finish2;
    double duration1, duration2;
    ifstream infile;
    infile.open("D:\\research\\2020-6-12\\data\\test.txt", ios::in);
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

    int heapSize = V;
    int heapPos[V+1];
    float dmax=0;
    float tempS;
    float densityMax = totalW/V;
    int indicateGraph[V+1];
    for (int i=0; i <= V; i++){
        indicateGraph[i] = 1;
    }


    float d;

    start1 = clock();
    d = BerkleyFlow(0, maxS, 0.1, V, indicateGraph, strength, vertexS, adj);
    finish1 = clock();
    duration1 = (double)(finish1 - start1) / CLOCKS_PER_SEC;
    cout << d << endl;
//****************************second part

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


//    d = BerkleyFlow(thres, maxS, 0.1, V, indicateGraph, strength, vertexS, adj);
    d = BerkleyFlowAcc(thres, maxS, 0.1, V, heapSize, heapPos, indicateGraph, strength, vertexS, heap, adj);
    finish2 = clock();
    duration2 = (double)(finish2 - start2) / CLOCKS_PER_SEC;
    cout << d << endl;
    cout << "berkley accelerated time " << duration2 << endl;

    // ***************************************************test
//        vector <pair<int, float>> adjacent;
//        float guess;
//        Graph g(V+2);
//        float m = 0;
//        vector <int> dsg;
//        guess = 43;
//        for (int i=0; i<=V; i++){
//            if (indicateGraph[i] == 0) continue;
//            if (strength[i] + vertexS[i]-2*guess > m) m = strength[i] + vertexS[i]-2*guess;
//        }


//        for (int i=1; i<=V; i++){
//            if (indicateGraph[i] == 0) continue;
//            g.addEdge(0, i, m);
//            g.addEdge(i, V+1, m+2*guess-strength[i]-vertexS[i]);
//            adjacent = adj[i];
//            for (int j=0; j<int(adjacent.size()); j++){
//                if (indicateGraph[adjacent[j].first] == 0) continue;
//                if (adjacent[j].first > i){
//                    g.addEdge(i, adjacent[j].first, adjacent[j].second);
//                    g.addEdge(adjacent[j].first, i, adjacent[j].second);
//                }
//            }
//        }

//        g.DinicMaxflow(0, V+1);
//        dsg = g.Cut(0);
//        for (int i=0; i<int(dsg.size()); i++) cout << dsg[i] << endl;
    // ***************************************************test







//    return 0;

    return duration1/duration2;
}

//int main(){
//    float ratio;
//    ratio = berkleyCompare();
////    cout << "Ratio is " << ratio << endl;
//    return 0;
//}









