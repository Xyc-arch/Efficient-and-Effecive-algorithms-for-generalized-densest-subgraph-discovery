#ifndef HEAP_H
#define HEAP_H

#endif // HEAP_H

#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <cstring>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;

void printAdjacent(int id, vector <pair <int, int>> adj[]);

void printGraph(vector<pair<int,float> > adj[], int V);

void printStrength(float strength[], int V);

void minHeapify(int pos, pair <float, int> heap[], int heapSize, int heapPos[]);

void heapDecreaseKey(pair <float, int> heap[], int heapPos[], int id, float value);

int heapExtractMin(pair <float, int> heap[], int &heapSize, int heapPos[], vector <pair <int, float>> adj[], int indicateGraph[]);

void heapExtractMin(pair <float, int> heap[], int &heapSize, int heapPos[], vector <pair <int, float>> adj[], int indicateGraph[], float strength[]);

void maxHeapify(int pos, pair <float, int> heap[], int heapSize);

pair <int, int> heapExtractMax(pair <float, int> heap[], int &heapSize);
