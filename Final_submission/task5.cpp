#include<bits/stdc++.h>
using namespace std;

void dfsCritical(int u, int parent, const vector<vector<int>>& adj, int& time, vector<int>& disc, vector<int>& low, vector<int>& componentDelta, vector<bool>& visited) {
    
    visited[u] = true;
    disc[u] = low[u] = ++time;
    int children = 0;
    int independent_separated_children = 0;

    for (int v : adj[u]) {
        if (v == parent) continue;

        if (visited[v]) {
            low[u] = min(low[u], disc[v]);
        } else {
            children++;
            dfsCritical(v, u, adj, time, disc, low, componentDelta, visited);

            low[u] = min(low[u], low[v]);

            // If u is not the root and the subtree at v cannot reach any ancestor of u
            if (parent != -1 && low[v] >= disc[u]) {
                independent_separated_children++;
            }
        }
    }

    // Determine the net change in components if node 'u' is deleted
    if (parent == -1) {
        componentDelta[u] = children - 1; 
    } else {
        componentDelta[u] = independent_separated_children;
    }
}

// Main Function
// Returns: {critical_node_index, max_components_after_removal}
pair<int, int> findMostCriticalNode(int V, const vector<vector<int>>& adj) {
    vector<int> disc(V, 0);
    vector<int> low(V, 0);
    vector<bool> visited(V, false);
    
    // Default delta is -1 (removing an isolated node reduces total components by 1)
    vector<int> componentDelta(V, -1); 

    int time = 0;
    int initialComponents = 0;

    // 1. Find initial connected components and populate discovery/low arrays
    for (int i = 0; i < V; i++) {
        if (!visited[i]) {
            initialComponents++;
            dfsCritical(i, -1, adj, time, disc, low, componentDelta, visited);
        }
    }

    int criticalNode = 0;
    int maxComponents = -1;

    // 2. Evaluate each node to find the one that yields the maximum split
    for (int i = 0; i < V; i++) {
        int componentsAfterRemoval = initialComponents + componentDelta[i];
        
        if (componentsAfterRemoval > maxComponents) {
            maxComponents = componentsAfterRemoval;
            criticalNode = i;
        }
    }

    return {criticalNode, maxComponents};
}





// Helper DFS function to find a bridge
void dfsEdge(int u, int parent, const vector<vector<int>>& adj, int& time,
             vector<int>& disc, vector<int>& low, vector<bool>& visited, 
             pair<int, int>& criticalEdge, bool& bridgeFound) {
    
    visited[u] = true;
    disc[u] = low[u] = ++time;

    for (int v : adj[u]) {
        if (v == parent) continue;

        if (visited[v]) {
            low[u] = min(low[u], disc[v]);
        } else {
            dfsEdge(v, u, adj, time, disc, low, visited, criticalEdge, bridgeFound);

            low[u] = min(low[u], low[v]);

            // Condition for a bridge: condition low[v] > disc[u] means v has no back-edge 
            // to any ancestor of u. Removing (u, v) will split the component.
            if (low[v] > disc[u]) {
                // If we only need *any* critical edge that maximizes components, 
                // the first bridge we find works perfectly.
                criticalEdge = {u, v};
                bridgeFound = true;
            }
        }
    }
}

// Main Function
// Returns: { {u, v}, total_components_after_removal }
pair<pair<int, int>, int> findMostCriticalEdge(int V, const vector<vector<int>>& adj) {
    vector<int> disc(V, 0);
    vector<int> low(V, 0);
    vector<bool> visited(V, false);

    int time = 0;
    int initialComponents = 0;
    
    pair<int, int> criticalEdge = {-1, -1}; // Defaults to {-1, -1} if no edge splits the graph
    bool bridgeFound = false;

    // 1. Calculate initial components and search for bridges
    for (int i = 0; i < V; i++) {
        if (!visited[i]) {
            initialComponents++;
            // We pass bridgeFound to stop updating once we secure at least one bridge
            dfsEdge(i, -1, adj, time, disc, low, visited, criticalEdge, bridgeFound);
        }
    }

    // 2. Determine final component count
    // If a bridge was found, total components become initialComponents + 1.
    // If no bridge exists, removing an edge doesn't split the graph (components stay the same).
    int maxComponents = bridgeFound ? (initialComponents + 1) : initialComponents;
    
    // If there are no bridges but the graph has edges, we can technically pick any arbitrary edge.
    if (!bridgeFound) {
        for (int u = 0; u < V; ++u) {
            if (!adj[u].empty()) {
                criticalEdge = {u, adj[u][0]};
                break;
            }
        }
    }

    return {criticalEdge, maxComponents};
}