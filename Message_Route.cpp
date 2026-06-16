#include<bits/stdc++.h>
using namespace std;

int bfs(vector<int>adj[] , vector<int>&parent , int n){
    queue<int>q;
    q.push(1);
    parent[1]=0; // 0 now safely acts as the ultimate stopping condition
    
    while(!q.empty()){
        int node=q.front();
        q.pop();
        for(auto it:adj[node]){
            // FIX 1: Check if unvisited using -1 instead of 0
            if(parent[it]==-1){ 
                parent[it]=node;
                q.push(it);
            }
        }
    }
    
    // FIX 2: Check if target node n was ever reached
    if(parent[n]==-1){ 
        return -1;
    }
    
    int count=0;
    int node = n;
    while(node!=0){
        count++;
        node = parent[node];
    }
    return count;
}

int main(){
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n , m;
    cin>>n>>m;

    vector<int>adj[n+1];
    for(int i=0;i<m;i++){
        int u , v;
        cin>>u>>v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // FIX 3: Initialize parent array with -1 instead of 0
    vector<int>parent(n+1 , -1); 
    
    int shortest_path = bfs(adj , parent , n);
    if(shortest_path==-1){
        cout<<"IMPOSSIBLE"<<endl;
    }
    else{
        cout<<shortest_path<<endl;
        vector<int>path;
        int node = n;
        while(node!=0){
            path.push_back(node);
            node = parent[node];
        }
        reverse(path.begin() , path.end());
        for(auto it:path){
            cout<<it<<" ";
        }
        cout<<endl;
    }
    return 0;
}