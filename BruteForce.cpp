#include <bits/stdc++.h>
#define pii pair<int,int>
using namespace std;

vector<pii> points;
pii pr = {100,100};
set<pair <double, pii> > order;


double dist(const pii &a, const pii &b){
    return (sqrt( pow(b.first - a.first, 2.00) + pow( b.second  - a.second, 2.00)));
}

int main(){
    srand(time(NULL));
    for(int i = 0; i < 100; ++i)
    {
        int x_ = 1 + rand() % 121;
        int y_ = 1 + rand() % 121;
        points.emplace_back(x_, y_);
    }   
    
    for( pii par : points ){
        double d = dist(par , pr);
        order.insert(make_pair(d , par));
    }
    
    for(auto it = order.begin() ; it != order.end(); it++){
        cout << "Dist: " << it-> first << "\t: Par x = " << it->second.first << "\ty = " << it->second.second << endl;
    } 
    return 0;
}