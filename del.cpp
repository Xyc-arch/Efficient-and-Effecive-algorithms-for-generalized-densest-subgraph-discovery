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


struct dynEdge {
    int u;
    int v;
    int w;
};


void readDelGraph(string name, string split, bool fromZero, int& vertexNum, int& edgeNum, int startLine, int lineNum, long double & totalW, int indicateGraph[], float strength[], float vertexS[], vector <pair <int, float>> adj[], int indicateLine[], vector<dynEdge>& randDelSet, bool isW){
    ifstream infile;
    string line;
    int n1;
    int n2;
    int currentLine=0;
    char c[20];
    char* num_str;
    stringstream s;
    map <int, int> allVertex;

    int u, v;
    int w=1;
    dynEdge tempEdge;

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

        w = 1;
        if (isW) {
            num_str = strtok(NULL, split.c_str());
            s << num_str;
            s >> w;
            s.clear();
            s.str("");
        }

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
            edgeNum += w;
        }
        if (n1 == n2){
            strength[n1] += w;
            vertexS[n1] += w;
            totalW += w;
        }
        allVertex.insert(pair<int, int>(n1, 1));
        allVertex.insert(pair<int, int>(n2, 1));

        if (indicateLine[currentLine-1]){
            if (n1 != n2){
                u = n1;
                v = n2;
                tempEdge = {.u = u, .v = v, .w = w};
                randDelSet.push_back(tempEdge);
            }
        }


        if (currentLine%10000000 == 0) cout << "Line: " << currentLine << endl;

    }

    for (auto it = allVertex.begin(); it != allVertex.end(); ++it){
        vertexNum += 1;
        indicateGraph[it -> first] = 1;
    }

    cout << "Graph reading finish" << endl;
}





void randDelLine(int lineNum, int targetLineNum, int indicateLine[]){
    int l;
    bool qual = false;
    for (int i=0; i<targetLineNum; i++){
        qual = false;
        while (!qual){
            l = rand() % lineNum;
            if (indicateLine[l] == 0){
                qual = true;
                indicateLine[l] = 1;
            }
        }
    }
}


float cCoreUpdateDel(int V, int vertexNum, float& hatRhoPlus, long double totalW, int indicateGraph[], int* tildeS, int* C, float strength[], float vertexS[], vector <pair <int, float>> adj[]){
    cCoreUpdateIns(V, vertexNum, hatRhoPlus, totalW, indicateGraph, tildeS, C, strength, vertexS, adj);
    return 0;
}

void rmEdge(int u, int v, vector <pair <int, float>> adj[]){
    vector <pair <int, float>> uadj = adj[u];
    vector <pair <int, float>> vadj = adj[v];
    int uSize, vSize;
    int uSize2, vSize2;

    uSize = int(adj[u].size());
    vSize = int(adj[v].size());

    for (auto it = uadj.begin(); it != uadj.end(); it++){
        if (it->first == v) {
            uadj.erase(it);
            it--;
            // cout << "u find" << endl;
        }
    }

    adj[u] = uadj;

    for (auto it = vadj.begin(); it != vadj.end(); it++){
        if (it->first == u) {
            vadj.erase(it);
            it--;
            // cout << "v find" << endl;
        }
    }

    adj[v] = vadj;

    uSize2 = int(adj[u].size());
    vSize2 = int(adj[v].size());

//    cout << "uSize: " << uSize << endl;
//    cout << "uSize2: " << uSize2 << endl;
//    cout << "vSize: " << vSize << endl;
//    cout << "vSize2: " << vSize2 << endl;
}

float cCoreDel(int V, int vertexNum, long double totalW, int indicateGraph[], int* tildeS, int* C, float strength[], float vertexS[], vector <pair <int, float>> adj[], int u, int v, float& hatRhoPlus, int& hit){
    if (((tildeS[u] == 0) && (C[u] == 0)) or ((tildeS[v] == 0) && (C[v] == 0))) {
        hit++;
        return 0;
    } else {
        if (((tildeS[u] == 0) && (C[u] == 1)) && ((tildeS[v] == 0) && (C[v] == 1))) {
            hit++;
            updateC(V, C, hatRhoPlus, adj);
        } else {
            cCoreUpdateIns(V, vertexNum, hatRhoPlus, totalW, indicateGraph, tildeS, C, strength, vertexS, adj);
        }
    }
    return 0;
}


int delExp(){
    clock_t start1, finish1, start2, finish2;
    double duration1, duration2;
    int setSize = 1000;

    // unweighted

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
//    string file = "D:\\research\\2022-6-12\\data\\com-youtube.ungraph.txt";
//    int startLine = 5;
//    int V = 1157827;
//    int lineNum = 2987628;
//    bool fromZero = false;


    // weighted graphs

    // test
    string file = "D:\\research\\2022-6-12\\data\\test.txt";
    int V = 16726;
    int startLine = 1;
    int lineNum = 95188;
    bool fromZero = false;


    // lbimseti
//    string file = "D:\\research\\2022-6-12\\data\\lbimseti\\edges.csv";
//    int V = 220970;
//    int startLine = 2;
//    int lineNum = 17359347;
//    bool fromZero = true;



//    bool isW = false;
    bool isW = true;

    int edgeNum = 0;
    int vertexNum = 0;
    long double totalW = 0;
    int* indicateGraph = new int[V+1]{0};
    float* strength = new float[V+1]{0};
    float* vertexS = new float[V+1]{0};
    vector <pair <int, float>>* adj = new vector <pair <int, float>> [V+1];

    int* tildeS = new int[V+1]{0};
    int* C = new int[V+1]{0};


    int u, v, w;
    dynEdge tempEdge;
    int* indicateLine = new int[lineNum + 1]{0};
    vector<dynEdge> randDelSet;

    randDelLine(lineNum, 1000, indicateLine);

//    int count = 0;
//    for (int i = 0; i <= lineNum; i++) {
//        if (indicateLine[i] == 1) count++;
//    }
//    cout << "# delete: " << count << endl;


//    readDelGraph(file, "	", fromZero, vertexNum, edgeNum, startLine, lineNum, totalW, indicateGraph, strength, vertexS, adj, indicateLine, randDelSet, isW);

//    readDelGraph(file, ",", fromZero, vertexNum, edgeNum, startLine, lineNum, totalW, indicateGraph, strength, vertexS, adj, indicateLine, randDelSet, isW);

    readDelGraph(file, " ", fromZero, vertexNum, edgeNum, startLine, lineNum, totalW, indicateGraph, strength, vertexS, adj, indicateLine, randDelSet, isW);


//    for (int i=0; i<int(randDelSet.size()); i++){
//        tempEdge = randDelSet[i];
//        cout << "u: " << tempEdge.u << " v: " << tempEdge.v << " w: " << tempEdge.w << endl;
//    }

    copyGraph(indicateGraph, tildeS, V);
    copyGraph(indicateGraph, C, V);


    // cCoreUpdateDel

    float hatRho = 0;
    int mySize = 0;
    int hit = 0;
    start1 = clock();

    for (int i=0; i<1000; i++){
        u = randDelSet[i].u;
        v = randDelSet[i].v;
        w = randDelSet[i].w;

        totalW -= w;
        strength[u] -= w;
        strength[v] -= w;
        rmEdge(u, v, adj);


        // cCoreUpdateDel(V, vertexNum, hatRho, totalW, indicateGraph, tildeS, C, strength, vertexS, adj);

        if (i == 0) cCoreUpdateDel(V, vertexNum, hatRho, totalW, indicateGraph, tildeS, C, strength, vertexS, adj);
        cCoreDel(V, vertexNum, totalW, indicateGraph, tildeS, C, strength, vertexS, adj, u, v, hatRho, hit);


        if (i%100 == 0){
            mySize = graphSize(C, V);
            cout << "iter: " << i << endl;
            cout << "C size: " << mySize << endl;
        }
    }

    finish1 = clock();
    duration1 = (double)(finish1 - start1) / CLOCKS_PER_SEC;
    cout << "Hit: " << hit << endl;
    cout << "Time cost: " << duration1 << endl;



    return 0;
}

//int main(){
//    delExp();
//    return 0;
//}


