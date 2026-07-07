#include<bits/stdc++.h>
using namespace std;
# define INF 1e9

// Dynamically update the weight of roads between cities and find the shortest distance between two cities before and after the update
int dijkstra(int V, vector<pair<int, int>>* adj, int src, int dest) {
    if (src == dest) return 0;

    vector<int> dist(V, INF);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;
        if (u == dest) return dist[u]; 

        for (auto& edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
    return (dist[dest] == INF) ? -1 : dist[dest];
}

// Main Function
// updates vector format: { {u1, v1, w1}, {u2, v2, w2}, ... }
// Returns: { distance_before_updates, distance_after_all_updates }
pair<int, int> updateBatchRoadsAndGetDistances(
    int V, 
    vector<pair<int, int>>* adj,         // Main adjacency list passed by reference
    int source, 
    int destination, 
    const vector<vector<int>>& updates) {       // Updates in vector<vector<int>> matrix form

    // 1. Calculate the shortest distance BEFORE any updates are applied
    int distanceBefore = dijkstra(V, adj, source, destination);

    // 2. Iterate through the updates matrix and dynamically apply changes
    for (const auto& road : updates) {
        if (road.size() < 3) continue; // Safety check for malformed rows
        
        int u = road[0];
        int v = road[1];
        int newWeight = road[2];

        // Update u -> v direction
        bool updatedUtoV = false;
        for (auto& edge : adj[u]) {
            if (edge.first == v) {
                edge.second = newWeight;
                updatedUtoV = true;
                break;
            }
        }
        if (!updatedUtoV) {
            adj[u].push_back({v, newWeight});
        }

        // Update v -> u direction (undirected graph safety)
        bool updatedVtoU = false;
        for (auto& edge : adj[v]) {
            if (edge.first == u) {
                edge.second = newWeight;
                updatedVtoU = true;
                break;
            }
        }
        if (!updatedVtoU) {
            adj[v].push_back({u, newWeight});
        }
    }

    // 3. Calculate the shortest distance AFTER all updates are in place
    int distanceAfter = dijkstra(V, adj, source, destination);

    return {distanceBefore, distanceAfter};
}