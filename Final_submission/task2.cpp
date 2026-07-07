#include<bits/stdc++.h>
using namespace std;

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