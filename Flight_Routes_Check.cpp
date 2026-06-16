#include<bits/stdc++.h>
using namespace std;
void bfs(vector<int>adj[] , vector<int>&check){
    queue<int>q;
    vector<int>vis(check.size() , 0);
    int count=1;
    for(int i=1;i<check.size();i++){
        if(!vis[i]){
            q.push(i);
            vis[i]=1;
            check[i]=count;
            while(!q.empty()){
                int node=q.front();
                q.pop();
                for(int j=0;j<adj[node].size();j++){
                    int child=adj[node][j];
                    if(!vis[child]){
                        vis[child]=1;
                        check[child]=count;
                        q.push(child);
                    }
                }
            }
            count++;
        }
    }
}
int main(){
    int n , m;
    cin>>n>>m;
    vector<int>check(n+1 , 0);
    vector<int>adj[n+1];
    while(m--){
        int a , b;
        cin>>a>>b;
        
        adj[a].push_back(b);
    }
    bfs(adj , check);
    int d1=0;
    int d2=0;
    for(int i=1;i<=n;i++){
        if(check[i]==1){
            d1=i;
        }
        else d2=i;
    }
    if(d1&&d2){
        cout<<"NO"<<endl;
        cout<<d1<<" "<<d2<<endl;
    }
    else cout<<"YES"<<endl;
}