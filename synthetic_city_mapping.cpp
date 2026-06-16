// This code generates a synthetic city mapping(undirected graph) with three different versions of adjacency list generation, finds clusters in the graph, and calculates the shortest path from each city to all other cities using Dijkstra's algorithm.

#include<bits/stdc++.h>
using namespace std;

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






// function to find clusters in the graph using BFS and return the cluster id for each city


vector<int>city_clusters(int n , vector<pair<int , int>>* adj_list){
    vector<int> visited(n , 0);
    vector<int> cluster_id(n , 0);
    int cluster_count = 1;
    queue<int> q;
    for(int i=0;i<n;i++){
        if(!visited[i]){
            
            q.push(i);
            visited[i] = 1;
            cluster_id[i] = cluster_count;

            while(!q.empty()){
                int node = q.front();
                q.pop();

                for(auto it:adj_list[node]){
                    int neighbor = it.first;
                    if(!visited[neighbor]){
                        visited[neighbor] = 1;
                        cluster_id[neighbor] = cluster_count;
                        q.push(neighbor);
                    }
                }
            }
            cluster_count++;
        }
    }

    return cluster_id;
}






// function to find the largest cluster in the graph and return the nodes in that cluster


vector<int> largest_cluster(vector<int> cluster_id){
    unordered_map<int , int> cluster_size;
    for(auto it:cluster_id){
        cluster_size[it]++;
    }
    int max_size = 0;
    int largest_cluster_id = 0;
    for(auto it:cluster_size){
        if(it.second > max_size){
            max_size = it.second;
            largest_cluster_id = it.first;
        }
    }
    vector<int> largest_cluster_nodes;
    for(int i=0;i<cluster_id.size();i++){
        if(cluster_id[i] == largest_cluster_id){
            largest_cluster_nodes.push_back(i);
        }
    }

    return largest_cluster_nodes;
}






// function to find the number of clusters in the graph and return the size of each cluster

vector<int> no_of_clusters(vector<int> cluster_id){
    unordered_map<int , int> cluster_size;
    for(auto it:cluster_id){
        cluster_size[it]++;
    }
    vector<int> cluster_sizes;
    for(auto it:cluster_size){
        cluster_sizes.push_back(it.second);
    }

    return cluster_sizes;
}








// function to find the shortest path from a source city to all other cities using Dijkstra's algorithm

vector<int>shortest_path(int n , vector<pair<int , int>>* adj_list , int src){
    vector<int> dist(n , INT_MAX);
    dist[src] = 0;
    priority_queue<pair<int , int> , vector<pair<int , int>> , greater<pair<int , int>>> pq;
    pq.push({0 , src});
    
    while(!pq.empty()){
        int node = pq.top().second;
        int node_dist = pq.top().first;
        pq.pop();
    
        if(node_dist > dist[node]){
            continue;
        }
    
        for(auto it:adj_list[node]){
            int neighbor = it.first;
            int weight = it.second;
    
            if(dist[node] + weight < dist[neighbor]){
                dist[neighbor] = dist[node] + weight;
                pq.push({dist[neighbor] , neighbor});
            }
        }
    }
    
    return dist;
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

    // print the cluster id for each city

    vector<int> clusters = city_clusters(n , adj3);  // find the clusters in the graph using the adjacency list of version 3
    for(int i=0;i<n;i++){
        cout<<clusters[i]<<" , ";
    }

    cout<<endl<<endl;

    // find the largest cluster and print the nodes in that cluster

    vector<int> largest_cluster_nodes = largest_cluster(clusters); // find the largest cluster using the cluster id for each city
    cout<<"Largest cluster size: "<<largest_cluster_nodes.size()<<endl;
    cout<<"Nodes in largest cluster: ";
    for(auto it:largest_cluster_nodes){
        cout<<it<<" , ";
    }
    cout<<endl<<endl;

    // find the number of clusters and print the size of each cluster

    vector<int> cluster_sizes = no_of_clusters(clusters); // find the size of each cluster using the cluster id for each city
    cout<<"Cluster sizes: ";
    for(auto it:cluster_sizes){
        cout<<it<<" , ";
    }

    int min_size = *min_element(cluster_sizes.begin() , cluster_sizes.end());
    int max_size = *max_element(cluster_sizes.begin() , cluster_sizes.end());
    cout<<endl<<"Minimum cluster size: "<<min_size<<endl;
    cout<<"Maximum cluster size: "<<max_size<<endl;

    cout<<endl<<endl;

    // find the shortest path from each city to all other cities and print the distances

    for(int i=0;i<n;i++){
        vector<int> shortest_distances = shortest_path(n , adj3 , i); // find the shortest path from city i to all other cities using the adjacency list of version 3
        cout<<"Shortest distances from city "<<i<<": ";
        for(auto it:shortest_distances){
            cout<<it<<" , ";
        }
        cout<<endl;
    }

    cout<<endl<<endl;

    
    
    return 0;
}