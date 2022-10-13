#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <cstring>
#include <sstream>
#include <bits/stdc++.h>
#include <math.h>
using namespace std;


typedef pair<int, int> pi;


void addEdge(vector<pair<int, float>> adj[], float strength[], int s, int d, float w) {
    adj[s].push_back(make_pair(d, w));
    adj[d].push_back(make_pair(s, w));
    strength[s] = strength[s] + w;
    strength[d] = strength[d] + w;
}


void printAdjacent(int id, vector <pair <int, float>> adj[]){
    cout << "Node " << id << " has adjacent node: "<< endl;
    vector <pair <int, float>> tempVec;
    tempVec = adj[id];
    for (int i=0; i < int(tempVec.size()); i++){
        cout << "Adjacent node " << tempVec[i].first << " with weight " << tempVec[i].second << endl;
    }
}

void printGraph(vector<pair<int,float> > adj[], int V)
{
    int v;
    float w;
    for (int u = 1; u <= V; u++)
    {
        cout << "Node " << u << " makes an edge with:" << endl;
        for (auto it = adj[u].begin(); it!=adj[u].end(); it++)
        {
            v = it->first;
            w = it->second;
            cout << "Node " << v << " with edge weight =" << w << endl;
        }
        cout << endl;
    }
}



void printStrength(float strength[], int V){
    for (int idx = 1; idx <= V; idx++){
        cout << "The strenth of Node " << idx << " is " << strength[idx] << endl;
    }
}

void minHeapify(int pos, pair <float, int> heap[], int heapSize, int heapPos[]){
    int leftPos = 2*pos;
    int rightPos = leftPos + 1;
    int least;
    if (leftPos <= heapSize && heap[leftPos].first < heap[pos].first){
        least = leftPos;
    } else {
        least = pos;
    }
    if (rightPos <= heapSize && heap[rightPos].first < heap[least].first){
        least = rightPos;
    }
    if (least != pos){
        int currentID = heap[pos].second;
        int leastID = heap[least].second;

        pair <float, int> temp = heap[least];
        heap[least] = heap[pos];
        heap[pos] = temp;

        int tempPos = heapPos[leastID];
        heapPos[leastID] = heapPos[currentID];
        heapPos[currentID] = tempPos;
        minHeapify(least, heap, heapSize, heapPos);
    }
}


void minHeapify(int pos, pair <float, int> heap[], int heapSize){
    int leftPos = 2*pos;
    int rightPos = 2*pos + 1;
    int least;
    if (leftPos <= heapSize && heap[leftPos].first < heap[pos].first){
        least = leftPos;
    } else {
        least = pos;
    }
    if (rightPos <= heapSize && heap[rightPos].first < heap[least].first){
        least = rightPos;
    }
    if (least != pos){
        pair <float, int> temp = heap[least];
        heap[least] = heap[pos];
        heap[pos] = temp;
        minHeapify(least, heap, heapSize);
    }
}



void heapDecreaseKey(pair <float, int> heap[], int heapPos[], int id, float value){
    int pos = heapPos[id];
    float newValue = heap[pos].first - value;
    pair <float, int> newPair = make_pair(newValue, id);
    heap[pos] = newPair;
    while (pos > 1 && newValue < heap[pos/2].first) {
        int currentID = heap[pos].second;
        int parentID = heap[pos/2].second;

        pair <float, int> temp = heap[pos/2];
        heap[pos/2] = heap[pos];
        heap[pos] = temp;

        int tempPos = heapPos[parentID];
        heapPos[parentID] = heapPos[currentID];
        heapPos[currentID] = tempPos;

        pos = pos/2;
    }

}

int heapExtractMin(pair <float, int> heap[], int &heapSize, int heapPos[], vector <pair <int, float>> adj[], int indicateGraph[]){
    float minStength = heap[1].first;
    int minID = heap[1].second;
    heap[1] = heap[heapSize];
    heapSize = heapSize - 1;
    heapPos[minID] = -1;
    indicateGraph[minID] = 0;
    heapPos[heap[1].second] = 1;
    minHeapify(1, heap, heapSize, heapPos);
    vector <pair <int, float>> adjacent = adj[minID];
    for (int i=0; i<int(adjacent.size()); i++){
        pair <int, float> adjPair = adjacent[i];
        if (indicateGraph[adjPair.first] > 0) heapDecreaseKey(heap, heapPos, adjPair.first, adjPair.second);
    }
    return minStength;
}

void heapExtractMin(pair <float, int> heap[], int &heapSize, int heapPos[], vector <pair <int, float>> adj[], int indicateGraph[], float strength[]){
    int minID = heap[1].second;
    heap[1] = heap[heapSize];
    heapSize = heapSize - 1;
    heapPos[minID] = -1;
    indicateGraph[minID] = 0;
    heapPos[heap[1].second] = 1;
    minHeapify(1, heap, heapSize, heapPos);
    vector <pair <int, float>> adjacent = adj[minID];
    for (int i=0; i<int(adjacent.size()); i++){
        pair <int, float> adjPair = adjacent[i];
        if (indicateGraph[adjPair.first] > 0) {
            heapDecreaseKey(heap, heapPos, adjPair.first, adjPair.second);
            strength[adjPair.first] -= adjPair.second;
        }
    }
}

pair<float, float> heapExtractMinDeno(pair <float, int> heap[], int &heapSize, int V,  int heapPos[], vector <pair <int, float>> adj[], int indicateGraph[]){
    pair <int, int> out;
    float minStength = heap[1].first;
    float outVW;
    int minID = heap[1].second;
    if (minID > V) outVW = 0.25;
    else outVW = 1;
    heap[1] = heap[heapSize];
    heapSize = heapSize - 1;
    heapPos[minID] = -1;
    indicateGraph[minID] = 0;
    heapPos[heap[1].second] = 1;
    minHeapify(1, heap, heapSize, heapPos);
    vector <pair <int, float>> adjacent = adj[minID];
    for (int i=0; i<int(adjacent.size()); i++){
        pair <int, float> adjPair = adjacent[i];
        if (indicateGraph[adjPair.first] > 0) {
            if (adjPair.first <= V)heapDecreaseKey(heap, heapPos, adjPair.first, adjPair.second);
            else heapDecreaseKey(heap, heapPos, adjPair.first, 4*adjPair.second);
        }
    }
    out = make_pair(outVW, minStength);
    return out;
}

void minHeapInsert(pair <float, int> heap[], int &heapSize, float strength, int id){
    heapSize = heapSize + 1;
    heap[heapSize] = make_pair(strength, id);
    int pos = heapSize;
    while (pos > 1 && strength < heap[pos/2].first) {

        pair <float, int> temp = heap[pos/2];
        heap[pos/2] = heap[pos];
        heap[pos] = temp;

        pos = pos/2;
    }
}

void maxHeapify(int pos, pair <float, int> heap[], int heapSize){
    int leftPos = 2*pos;
    int rightPos = 2*pos + 1;
    int largest;
    if (leftPos <= heapSize && heap[leftPos].first > heap[pos].first){
        largest = leftPos;
    } else {
        largest = pos;
    }
    if (rightPos <= heapSize && heap[rightPos].first > heap[largest].first){
        largest = rightPos;
    }
    if (largest != pos){
        pair <float, int> temp = heap[largest];
        heap[largest] = heap[pos];
        heap[pos] = temp;
        maxHeapify(largest, heap, heapSize);
    }
}

pair <int, int> heapExtractMax(pair <float, int> heap[], int &heapSize){
    heap[1] = heap[heapSize];
    heapSize = heapSize - 1;
    maxHeapify(1, heap, heapSize);
    return make_pair(heap[1].first, heap[1].second);
}


pair <float, float> compare(string file, int V, int lineNum){
    clock_t start1, finish1, start2, finish2;
    double duration1, duration2;
    ifstream infile;
    infile.open(file, ios::in);
    int originV = V;
    int count=0;
    vector <pair <int, float>> adj[V+1];
    float strength[V+1];
    for (int i=0; i<=V; i++){
        strength[i] = 0;
    }
    float vertexS[V+1];
    for (int i=1; i<=V; i++) vertexS[i] = 0;
    float totalW=0;


    string line;
    char c[20];
    char* num_str;
    stringstream s;
    int n1;
    int n2;
    float w;
//    Node *temp = NULL;

    while (getline(infile, line)){
        if (count > lineNum) break;

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

    start1 = clock();

    int heapSize = V;
    int heapPos[V+1];
    float density = totalW/V;
    int k = 10;
    int indicateGraph[V+1];
    for (int i=0; i <= V; i++){
        indicateGraph[i] = 1;
    }

    float optDen = density;
    float optW = totalW;
    int optSize = heapSize;
    int optGraph[V+1];
    for (int i=0; i<=V; i++){
        optGraph[i] = indicateGraph[i];
    }


    float thres;
    float outW;
    float tempS;
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

    finish1 = clock();
    duration1 = (long double)(finish1 - start1) / CLOCKS_PER_SEC;
//    cout << optDen << endl;
//    printf("%f seconds for simple peeling\n", duration1);

//    cout << "naive peeling density" << optDen << endl;
//    cout << optSize << endl;
//    cout << thres << endl;



    start2 = clock();
    V = originV;
    int j = 16;
    int topNum = j * k;


//    int maxheapSize = V;
//    for (int i=1; i <= V; i++){
//        indicateGraph[i] = 0;
//    }
//    pair <float, int> maxheap[V+1];
//    for (int i=1; i<=V; i++) {
//        maxheap[i] = make_pair(strength[i], i);
//    }
//    for (int i=V/2; i>=1; i--){
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


    int topStrength[V+1];
    for (int i = 0; i <= V; i++){
        topStrength[i] = 0;
    }

    int heap1Pos[V+1];
    int cpPos = 1;
    totalW = 0;
    pair <float, int> heap1[topNum+1];
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

//    cout << totalW << endl;
//    int a = 0;
//    int idx = 0;
//    for (int i=0; i<=V; i++){
//        if (topStrength[i] > a) {
//            a = topStrength[i];
//            idx = i;
//        }
//    }
//    cout << a << endl;
//    cout << idx << endl;


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

    //    cout << optW << endl;
//        cout << optDen << endl;
    //    cout << optSize << endl;
    //    cout << thres << endl;

    int heap2Size = 0;
    for (int i=1; i<=V; i++){
        if (strength[i] > optDen) indicateGraph[i] = 1;
        if (indicateGraph[i] > 0) heap2Size += 1;
    }

    int heap2Pos[V+1];
    int heap2Strength[V+1];
    pair <float, int> heap2[heap2Size+1];
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

        //new here
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
    duration2 = (long double)(finish2 - start2) / CLOCKS_PER_SEC;
//    cout << "accelerated dalks density: " << optDen << endl;
//    printf("%f seconds for our algorithm \n", duration2);

//    cout << optDen << endl;
//    cout << optSize << endl;
//    cout << thres << endl;



    return make_pair(duration1, duration2);
}







int DALKS(){
    pair <float, float> out;
    float time1=0;
    float time2=0;
    float ratio;
    for (int i=1; i<=10; i++){
        out = compare("D:\\research\\2020-6-12\\data\\test.txt", 16726, 95188);
//        out = compare("D:\\research\\2020-6-12\\data\\OF_one-mode_weightedmsg_sum.txt", 899, 142760);
        time1 += out.first;
        time2 += out.second;
    }
    ratio = time1/time2;
//    cout << time1 << endl;
//    cout << time2 << endl;
    cout << ratio << endl;
//    float totalTime = 0;
//    float average;
//    int iter = 100;
//    for (int i = 1; i <= iter; i++){
//        totalTime += compare();
//    }
//    average = totalTime/iter;
//    cout << "The average time of " << iter << " iterations is " << average << endl;
    return 0;
}

//int main(){
//    DALKS();
//    return 0;
//}
