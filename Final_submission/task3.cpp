# include<bits/stdc++.h>
using namespace std;

// Shortest distance between two cities after removal one or multiple cities or roads
// Structure to represent an edge weight
const int INF = 1e9; 

// Returns: { path_length, vector_representing_the_path }
pair<int, vector<int>> shortestPathWithBans(int V, 
    vector<pair<int, int>>* adj, // {neighbor, weight}
    int source, 
    int destination,
    const set<int>& bannedNodes, 
    const set<pair<int, int>>& bannedEdges) {

    // If source or destination themselves are deleted, no path exists
    if (bannedNodes.count(source) || bannedNodes.count(destination)) {
        return {-1, {}};
    }

    vector<int> dist(V, INF);
    vector<int> parent(V, -1);
    // Min-heap: {distance, node}
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;
        if (u == destination) break; // Found shortest path to destination

        for (auto& edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;

            // 1. Skip if the neighbor node is removed
            if (bannedNodes.count(v)) continue;

            // 2. Skip if this specific road/edge is removed
            // Checking both directions (u->v and v->u) ensures undirected safety
            if (bannedEdges.count({u, v}) || bannedEdges.count({v, u})) continue;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    // If destination is unreachable
    if (dist[destination] == INF) {
        return {-1, {}};
    }

    // Reconstruct the shortest path
    vector<int> path;
    for (int curr = destination; curr != -1; curr = parent[curr]) {
        path.push_back(curr);
    }
    reverse(path.begin(), path.end());

    return {dist[destination], path};
}