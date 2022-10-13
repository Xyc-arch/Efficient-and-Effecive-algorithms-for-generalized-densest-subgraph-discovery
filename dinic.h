#ifndef DINIC_H
#define DINIC_H

#endif // DINIC_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <cstring>
#include <sstream>
#include <bits/stdc++.h>


// A structure to represent a edge between
// two vertex
struct Edge
{
    int v ;  // Vertex v (or "to" vertex)
             // of a directed edge u-v. "From"
             // vertex u can be obtained using
             // index in adjacent array.

    float flow ; // flow of data in edge

    float C;    // capacity

    int rev ; // To store index of reverse
              // edge in adjacency list so that
              // we can quickly find it.
};

// Residual Graph
class Graph
{
    int V; // number of vertex
    int *level ; // stores level of a node
    std::vector< Edge > *adj;
public :
    Graph(int V);


    // add edge to the graph
    void addEdge(int u, int v, float C);

    bool BFS(int s, int t);
    std::vector<int> Cut(int s);
    std::vector<int> soda22Cut(int s);
    void renew();
    long double sendFlow(int s, float flow, int t, int ptr[]);
    long double DinicMaxflow(int s, int t);
    long double soda22MaxflowApp(int s, int t, float eps, long double W);
};




