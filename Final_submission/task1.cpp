#include <bits/stdc++.h>
using namespace std;

// Prim's algorithm for finding the minimum spanning tree of a graph represented as an adjacency list
vector<vector<int>> minimum_spanning_tree_prim(vector<pair<int , int>>* adj_list , int n){
    vector<vector<int>> mst(n); // create a vector to store the minimum spanning tree
    vector<bool> visited(n , false); // create a vector to keep track of visited cities
    priority_queue<pair<int , pair<int , int>> , vector<pair<int , pair<int , int>>> , greater<pair<int , pair<int , int>>>> pq; // create a priority queue to store the edges of the graph

    pq.push({0 , {0 , -1}}); // push the first city into the priority queue with distance 0 and no parent city

    while(!pq.empty()){
        auto [dis , edge] = pq.top(); // get the edge with the smallest distance from the priority queue
        pq.pop();
        int node = edge.first; // get the current city
        int parent = edge.second; // get the parent city

        if(visited[node]) continue; // if the current city has already been visited, skip it
        visited[node] = true; // mark the current city as visited

        if(parent != -1){ // if there is a parent city, add the edge to the minimum spanning tree
            mst[parent].push_back(node);
            mst[node].push_back(parent);
        }

        for(auto [next_node , next_dis] : adj_list[node]){ // iterate through all the neighboring cities of the current city
            if(!visited[next_node]){ // if the neighboring city has not been visited, push it into the priority queue with its distance and current city as its parent
                pq.push({next_dis , {next_node , node}});
            }
        }
    }

    return mst; // return the minimum spanning tree
}


// Kruskal's algorithm for finding the minimum spanning tree of a graph represented as an adjacency list
vector<vector<int>> minimum_spanning_tree_kruskal(vector<pair<int , int>>* adj_list , int n){
    vector<vector<int>> mst(n); // create a vector to store the minimum spanning tree
    vector<pair<int , pair<int , int>>> edges; // create a vector to store the edges of the graph

    for(int i=0;i<n;i++){ // iterate through all the cities
        for(auto [next_node , next_dis] : adj_list[i]){ // iterate through all the neighboring cities of the current city
            if(i < next_node){ // to avoid duplicate edges, only consider edges where the current city is less than the neighboring city
                edges.push_back({next_dis , {i , next_node}}); // add the edge to the edges vector with its distance and the two cities it connects
            }
        }
    }

    sort(edges.begin() , edges.end()); // sort the edges in ascending order based on their distances

    vector<int> parent(n); // create a vector to keep track of the parent of each city
    iota(parent.begin() , parent.end() , 0); // initialize the parent of each city to itself

    function<int(int)> find = [&](int x){ // define a function to find the parent of a city using path compression
        if(parent[x] != x){
            parent[x] = find(parent[x]);
        }
        return parent[x];
    };

    for(auto [dis , edge] : edges){ // iterate through all the edges in ascending order of their distances
        int u = edge.first; // get the first city of the edge
        int v = edge.second; // get the second city of the edge

        int pu = find(u); // find the parent of the first city
        int pv = find(v); // find the parent of the second city

        if(pu != pv){ // if the two cities have different parents, they are not connected, so add the edge to the minimum spanning tree and union their parents
            mst[u].push_back(v);
            mst[v].push_back(u);
            parent[pu] = pv;
        }
    }

    return mst; // return the minimum spanning tree
}



// original cost of the netwotk
int original_cost(vector<pair<int , int>>* adj_list , int n){
    int cost = 0; // initialize the cost to 0
    for(int i=0;i<n;i++){ // iterate through all the cities
        for(auto [next_node , next_dis] : adj_list[i]){ // iterate through all the neighboring cities of the current city
            cost += next_dis; // add the distance of the edge to the cost
        }
    }

    return cost/2; // return the total cost divided by 2, since each edge is counted twice in an undirected graph
}



// minimum cost of the minimum spanning tree
int minimum_cost(vector<vector<int>> mst , vector<pair<int , int>>* adj_list){
    int cost = 0; // initialize the cost to 0
    for(int i=0;i<mst.size();i++){ // iterate through all the cities in the minimum spanning tree
        for(auto it:mst[i]){ // iterate through all the neighboring cities of the current city in the minimum spanning tree
            for(auto [next_node , next_dis] : adj_list[i]){ // iterate through all the neighboring cities of the current city in the original graph
                if(next_node == it){ // if the neighboring city in the original graph is also a neighboring city in the minimum spanning tree, add its distance to the cost
                    cost += next_dis;
                    break;
                }
            }
        }
    }

    return cost/2; // return the total cost divided by 2, since each edge is counted twice in an undirected graph
}