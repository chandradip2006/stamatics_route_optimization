#include<bits/stdc++.h>
using namespace std;
int bfs(vector<int>adj[] , vector<int>&connected , int n){
    int count=0;
    for(int i=1;i<=n;i++){
        if(!connected[i]){
            count++;
            queue<int>q;
            q.push(i);
            connected[i]=count;
            while(!q.empty()){
                int node=q.front();
                q.pop();
                for(auto it:adj[node]){
                    if(!connected[it]){
                        connected[it]=count;
                        q.push(it);
                    }
                }
            }
        }
    }
    return count;
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

    vector<int>connected(n+1 , 0);
    int count = bfs(adj , connected , n);
    cout<<count-1<<endl;
    if(count>1){
        vector<vector<int>>components(count+1);
        for(int i=1;i<=n;i++){
            components[connected[i]].push_back(i);
        }
        for(int i=1;i<count;i++){
            cout<<components[i][0]<<" "<<components[i+1][0]<<endl;
        }
    }
    
}