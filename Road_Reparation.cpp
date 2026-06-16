#include<bits/stdc++.h>
using namespace std;
int find(vector<int>&root, int x) {
    if (root[x] == x) return x;        // x is its own root -- found it
    // path compression: point x straight at the root for next time
    return root[x] = find(root , root[x]);
}
int main(){
    int n, m;
    cin>>n>>m;
    vector<vector<int>>v;
    while(m--){
        int a ,b , c;
        cin>>a>>b>>c;
        v.push_back({c , a , b});
    }

    sort(v.begin() , v.end());
    vector<int>root(n+1);
    for(int i=0;i<=n;i++){
        root[i]=i;
    }
    vector<int>size;
    for(int i=0;i<=n;i++){
        size.push_back(1);
    }

    long long int ans=0;
    for(int i=0;i<v.size();i++){
        long long int c=v[i][0];
        int a=v[i][1];
        int b=v[i][2];
        int roota=find(root , a);
        int rootb=find(root , b);
        if(roota!=rootb){
            ans+=c;
            if(size[roota]>size[rootb]){
                root[rootb]=roota;
                size[roota]+=size[rootb];
            }
            else{
                root[roota]=rootb;
                size[rootb]+=size[roota];
            }
        }
    }
    for(auto &it:size){
        if(it==n){
            cout<<ans<<endl;
            return 0;
        }
    }
    cout<<"IMPOSSIBLE"<<endl;
    return 0;

}