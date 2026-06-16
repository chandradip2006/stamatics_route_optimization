#include<bits/stdc++.h>
using namespace std;
int bfs(vector<int>adj[] , vector<int>&bipartite , int n){
    for(int i=1;i<=n;i++){
        if(bipartite[i]==-1){
            queue<int>q;
            q.push(i);
            bipartite[i]=0;
            while(!q.empty()){
                int node=q.front();
                q.pop();
                for(auto it:adj[node]){
                    if(bipartite[it]==-1){
                        bipartite[it]=1-bipartite[node];
                        q.push(it);
                    }
                    else if(bipartite[it]==bipartite[node]){
                        return 0; // Not possible to bipartition
                    }
                }
            }
        }
    }
    return 1; // Possible to bipartition
}
int main(){
    int n , m;
    cin>>n>>m;

    vector<int>adj[n+1];
    for(int i=0;i<m;i++){
        int u , v;
        cin>>u>>v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<int>bipartite(n+1 , -1);
    int isPossible= bfs(adj , bipartite , n);
    if(isPossible){
        
        for(int i=1;i<=n;i++){
            if(bipartite[i]==0){
                cout<<1<<" ";
            }
            else{
                cout<<2<<" ";
            }
        }
        cout<<endl;
    }
    else{
        cout<<"IMPOSSIBLE"<<endl;
    }
}