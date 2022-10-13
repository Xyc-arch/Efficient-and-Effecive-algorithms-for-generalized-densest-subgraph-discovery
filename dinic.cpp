#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <cstring>
#include <sstream>
#include <cmath>
#include <bits/stdc++.h>
#include "dinic.h"

using namespace std;



Graph::Graph(int V)
    {
        adj = new std::vector<Edge>[V];
        this->V = V;
        level = new int[V];
    }

void Graph::addEdge(int u, int v, float C)
    {
        std::vector<Edge> rev1 = adj[v];
        // Forward edge : 0 flow and C capacity
        Edge a{v, 0, C, int(adj[v].size())};

        // Back edge : 0 flow and 0 capacity
        Edge b{u, 0, 0, int(adj[u].size())};

        adj[u].push_back(a);
        adj[v].push_back(b); // reverse edge
    }

void Graph::renew(){
    delete[] adj;
    delete[] level;
    adj = new std::vector<Edge>[V];
    level = new int[V];
}


vector<int> Graph::Cut(int s){
    int V = this->V;
    int* visit = new int[V];
    for (int i=0; i<V; i++){
        visit[i] = 0;
    }
    vector<int> outIdx;
    list <int> q;
    q.push_back(s);
    outIdx.push_back(s);
    visit[s] = 1;
    vector<Edge>::iterator i ;
    while (!q.empty())
    {
        int u = q.front();
        q.pop_front();
        for (i = adj[u].begin(); i != adj[u].end(); i++)
        {
            Edge &e = *i;
            if (visit[e.v] > 0) continue;
            visit[e.v] = 1;
            if (e.flow < e.C)
            {
                outIdx.push_back(e.v);
                q.push_back(e.v);
            }
        }
    }
    return outIdx;
}


// Finds if more flow can be sent from s to t.
// Also assigns levels to nodes.
bool Graph::BFS(int s, int t)
{
    for (int i = 0 ; i < V ; i++)
        level[i] = -1;

    level[s] = 0;  // Level of source vertex

    // Create a queue, enqueue source vertex
    // and mark source vertex as visited here
    // level[] array works as visited array also.
    list< int > q;
    q.push_back(s);

    vector<Edge>::iterator i ;
    while (!q.empty())
    {
        int u = q.front();
        q.pop_front();
        for (i = adj[u].begin(); i != adj[u].end(); i++)
        {
            Edge &e = *i;
            if (level[e.v] < 0  && e.flow < e.C)
            {
                // Level of current vertex is,
                // level of parent + 1
                level[e.v] = level[u] + 1;

                q.push_back(e.v);
            }
        }
    }

    // IF we can not reach to the sink we
    // return false else true
    return level[t] < 0 ? false : true ;
}

// A DFS based function to send flow after BFS has
// figured out that there is a possible flow and
// constructed levels. This function called multiple
// times for a single call of BFS.
// flow : Current flow send by parent function call
// start[] : To keep track of next edge to be explored.
//           start[i] stores  count of edges explored
//           from i.
//  u : Current vertex
//  t : Sink
long double Graph::sendFlow(int u, float flow, int t, int start[])
{
    // Sink reached
    if (u == t)
        return (long double)(flow);

    // Traverse all adjacent edges one -by - one.
    for (  ; start[u] < int(adj[u].size()); start[u]++)
    {
        // Pick next edge from adjacency list of u
        Edge &e = adj[u][start[u]];

        if (level[e.v] == level[u]+1 && e.flow < e.C)
        {
            // find minimum flow from u to t
            float curr_flow = min(flow, e.C - e.flow);

            long double temp_flow = sendFlow(e.v, curr_flow, t, start);

            // flow is greater than zero
            if (temp_flow > 0)
            {
                // add flow  to current edge
                e.flow += temp_flow;

                // subtract flow from reverse edge
                // of current edge
                adj[e.v][e.rev].flow -= temp_flow;
                return temp_flow;
            }
        }
    }

    return 0;
}

// Returns maximum flow in graph
long double Graph::DinicMaxflow(int s, int t)
{
    // Corner case
    if (s == t)
        return -1;

    long double total = 0;  // Initialize result

    // Augment the flow while there is path
    // from source to sink
    while (BFS(s, t) == true)
    {
        // store how many edges are visited
        // from V { 0 to V }
        int *start = new int[V+1] {0};

        // while flow is not zero in graph from S to D
        while (long double flow = sendFlow(s, FLT_MAX, t, start))

            // Add path flow to overall flow
            total += flow;
    }

    // return maximum flow
    return total;
}

long double Graph::soda22MaxflowApp(int s, int t, float eps, long double W){
    // Corner case
    if (s == t)
        return -1;

    long double total = 0;  // Initialize result
    float h = ceil(2*log(W)/eps) + 2;
    int iter = 0;

    // Augment the flow while there is path
    // from source to sink
    while (BFS(s, t) == true && iter < h)
    {
        // store how many edges are visited
        // from V { 0 to V }
        int *start = new int[V+1] {0};

        // while flow is not zero in graph from S to D
        while (long double flow = sendFlow(s, FLT_MAX, t, start))

            // Add path flow to overall flow
            total += flow;
    }

    // return maximum flow
    return total;
}



int dinicExp()
{
    Graph g(6);
    g.addEdge(0, 1, 16 );
    g.addEdge(0, 2, 13 );
    g.addEdge(1, 2, 10 );
    g.addEdge(1, 3, 12 );
    g.addEdge(2, 1, 4 );
    g.addEdge(2, 4, 14);
    g.addEdge(3, 2, 9 );
    g.addEdge(3, 5, 20 );
    g.addEdge(4, 3, 7 );
    g.addEdge(4, 5, 4);

    // next exmp
    /*g.addEdge(0, 1, 3 );
      g.addEdge(0, 2, 7 ) ;
      g.addEdge(1, 3, 9);
      g.addEdge(1, 4, 9 );
      g.addEdge(2, 1, 9 );
      g.addEdge(2, 4, 9);
      g.addEdge(2, 5, 4);
      g.addEdge(3, 5, 3);
      g.addEdge(4, 5, 7 );
      g.addEdge(0, 4, 10);

     // next exp
     g.addEdge(0, 1, 10);
     g.addEdge(0, 2, 10);
     g.addEdge(1, 3, 4 );
     g.addEdge(1, 4, 8 );
     g.addEdge(1, 2, 2 );
     g.addEdge(2, 4, 9 );
     g.addEdge(3, 5, 10 );
     g.addEdge(4, 3, 6 );
     g.addEdge(4, 5, 10 ); */

    cout << "Maximum flow " << g.DinicMaxflow(0, 5);
    return 0;
}








