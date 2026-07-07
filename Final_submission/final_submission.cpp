#include<bits/stdc++.h>
using namespace std;
const int INF = 1e9; 


// version 1: For each pair of cities, randomly decide whether to create a road between them and assign a random distance within the specified range.(with low cluster density)
vector<pair<int , int>>* adjacency_list_v1(int n , int min_dis , int max_dis){
    vector<pair<int , int>>* adj_list = new vector<pair<int , int>>[n];
    for(int i=0;i<n;i++){
        for(int j=i;j<n;j++){
            int random_size = rand() % (n - j + 1); // randomly decide the number of roads between city i and city j
            random_size = min(random_size , 1);
            for(int k=0;k<random_size;k++){
                int node = rand() % (n - j) + j; // randomly select a city from the remaining cities to create a road with city i
                int dis = rand() % (max_dis - min_dis + 1) + min_dis; // randomly assign a distance within the specified range for the road between city i and city j
                adj_list[j].push_back({node , dis});
                adj_list[node].push_back({j , dis});
            }
        }
    }

    return adj_list;

}


// version 2: Randomly generate a certain number of roads between pairs of cities, ensuring that the distances are within the specified range.(with medium cluster density)

vector<pair<int , int>>* adjacency_list_v2(int n , int min_dis , int max_dis){
    vector<pair<int , int>>* adj_list = new vector<pair<int , int>>[n];
    int edge_count = rand() % (n*(n-1)/2 + 1); // randomly decide the number of roads to be generated between pairs of cities
    for(int i=0;i<edge_count;i++){
        int u = rand() % n; // randomly select a city u from the list of cities
        int v = rand() % n; // randomly select a city v from the list of cities
        while(v == u){     // ensure that the same city is not selected for both u and v
            v = rand() % n;
        }
        int dis = rand() % (max_dis - min_dis + 1) + min_dis; // randomly assign a distance within the specified range for the road between city u and city v
        adj_list[u].push_back({v , dis});
        adj_list[v].push_back({u , dis});
    }

    return adj_list;

}


// version 3: For each city, randomly generate a certain number of roads to other cities, ensuring that the distances are within the specified range.(with high or reasonable cluster density)

vector<pair<int , int>>* adjacency_list_v3(int n , int min_dis , int max_dis){
    vector<pair<int , int>>* adj_list = new vector<pair<int , int>>[n];
    for(int i=0;i<n;i++){
        int edge_count = rand() % 3; // each city can have 0, 1, or 2 roads to other cities, but here one can increase the edge count to 3 or 4 to increase the cluster density

        for(int j=0;j<edge_count;j++){
            int node = rand() % n; // randomly select a city from the list of cities to create a road with city i
            while(node == i){     // ensure that the same city is not selected for both i and node
                node = rand() % n; 
            }
            int dis = rand() % (max_dis - min_dis + 1) + min_dis; // randomly assign a distance within the specified range for the road between city i and city node
            adj_list[i].push_back({node , dis});
            adj_list[node].push_back({i , dis});
        }
    }

    return adj_list;

}

// Task1: 

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


// Task 2:
vector<vector<int>> top_10_most_connected_cities(vector<pair<int , int>>* adj_list , int n){
    vector<pair<int , int>> degree(n); // create a vector to store the degree of each city
    for(int i=0;i<n;i++){
        degree[i] = {adj_list[i].size() , i}; // store the degree and the index of each city
    }
    sort(degree.rbegin() , degree.rend()); // sort the cities in descending order based on their degrees
    vector<vector<int>> top_10; // create a vector to store the top 10 most connected cities
    for(int i=0;i<min(10 , n);i++){
        top_10.push_back({degree[i].second , degree[i].first}); // add the index and degree of the top 10 most connected cities to the vector
    }
    return top_10; // return the top 10 most connected cities
}



// Task 3:
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




// Task 4:
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



// Task 5:
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




int main(){
    int n=500;  // number of cities
    int min_dis = 5;  // minimum distance between cities
    int max_dis = 50; // maximum distance between cities

    srand(time(0)); // seed the random number generator with the current time

    vector<pair<int , int>>* adj1; // adjacency list for version 1
    vector<pair<int , int>>* adj2; // adjacency list for version 2
    vector<pair<int , int>>*adj3;  // adjacency list for version 3
    adj1 = adjacency_list_v1(n , min_dis , max_dis); // generate the adjacency list for version 1
    adj2 = adjacency_list_v2(n , min_dis , max_dis); // generate the adjacency list for version 2
    adj3 = adjacency_list_v3(n , min_dis , max_dis); // generate the adjacency list for version 3
    

    // print the adjacency list for version 3

    for(int i=0;i<n;i++){
        cout<<"city_id_"<<i<<" -> "<<"[";
        for(auto it:adj2[i]){
            cout<<"("<<it.first<<" , "<<it.second<<") ";
            cout<<" , ";
        }
        cout<<"]";
        cout<<endl;
    }

    cout<<endl<<endl;

    cout<<"Task 1:"<<endl;
    cout<<"Original cost of the network: "<<original_cost(adj2 , n)<<endl; // print the original cost of the network
    vector<vector<int>> mst1 = minimum_spanning_tree_prim(adj2 , n);
    cout<<"Minimum cost : "<<minimum_cost(mst1 , adj2)<<endl; // print the minimum cost of the minimum spanning tree using Prim's algorithm

    cout<<"Cost saved in % : "<<((original_cost(adj2 , n) - minimum_cost(mst1 , adj2))*100)/original_cost(adj2 , n)<<"%"<<endl; // print the cost saved in percentage
    cout<<"selected roads:"<<endl;
    cout<<"[";
    for(auto &it:mst1){
        cout<<"(";
        cout<<it[0]<<" , "<<it[1];
        cout<<"), ";
    }
    cout<<"]"<<endl;

    cout<<endl<<endl;
    cout<<"Task 2:"<<endl;
    vector<vector<int>> top_10 = top_10_most_connected_cities(adj2 , n);
    cout<<"Top 10 most connected cities:"<<endl;
    
    for(auto &it:top_10){
        cout<<"City "<<it[0]<<" ->  Degree "<<it[1]<<endl;
    }
    cout<<endl<<endl;

    





    cout<<"Task 3:"<<endl;
    int source = 0; // starting city
    int destination = 10; // destination city
    set<int> bannedNodes = {5, 7}; // cities to be banned

    set<pair<int, int>> bannedEdges = {{2, 3}, {4, 6}}; // roads to be banned

    auto [pathLength, path] = shortestPathWithBans(n, adj2, source, destination, bannedNodes, bannedEdges);
    if (pathLength == -1) {
        cout << "No path exists from city " << source << " to city " << destination << " after banning specified cities and roads." << endl;
    } else {
        cout << "Shortest path length from city " << source << " to city " << destination << ": " << pathLength << endl;
        cout << "Path: ";
        for (int city : path) {
            cout << city;
            if (city != destination) cout << " -> ";
        }
        cout << endl;
    }

    cout<<endl<<endl;



    

    cout<<"Task 4:"<<endl;
    vector<vector<int>> updates = {
        {1, 2, 10}, // Update road between city 1 and city 2 to weight 10
        {3, 4, 5},  // Update road between city 3 and city 4 to weight 5
        {0, 5, 15}  // Update road between city 0 and city 5 to weight 15
    };

    // cout<<"Normal route distance between city "<<source<<" and city "<<destination<<" : "<<dijkstra(n, adj2, source, destination)<<endl;
    auto [distanceBefore, distanceAfter] = updateBatchRoadsAndGetDistances(n, adj2, source, destination, updates);
    cout << "Normal Distance: " << distanceBefore << endl;
    cout << "Traffic Distance: " << distanceAfter << endl;
    cout<<"Delay in %: "<<((distanceAfter - distanceBefore)*100)/distanceBefore<<"%"<<endl;
    cout<<endl<<endl;





    cout<<"Task 5:"<<endl;
    auto [criticalNode, maxComponents] = findMostCriticalNode(n, mst1);
    cout << "Most critical city: " << criticalNode << " -> Total components after removal: " << maxComponents << endl;
    cout<< "Most critical road: ";
    auto [criticalEdge, maxComponentsEdge] = findMostCriticalEdge(n, mst1);
    cout << "(" << criticalEdge.first << ", " << criticalEdge.second << ") -> Total components after removal: " << maxComponentsEdge << endl;
    



}