#include <iostream>
#include <math.h>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>
#include "kdtree.h"
// #define vd vector<double>
using namespace std;

set<pair<double, pair<int, vd>>> NNBF;
vector<pair<int, vd>> points;

// distancia a todos los puntos
void NNFuerzaBruta(vd& coord, int dim)
{
    for(int i=0 ; i<points.size() ; ++i)
    {
        double dist=0;
        for(int j=0 ; j<dim ; ++j)
            dist+=pow(points[i].second[j] - coord[j], 2);   // acumula segun dimension
        dist=sqrt(dist);
        NNBF.insert({dist, points[i]});
    }
}

int main()
{
    const int dim = 3;
    KD_Tree<int, dim> KD;
    ifstream f("10000.csv");
    int i=1;
    while(!f.eof())
    {
        string linea;
        f>>linea;
        // if(i==11)break;
        // cout<<linea<<"\n";
        stringstream str(linea);
        vd coord;
        string axis;
        while(getline(str,axis,','))
            coord.push_back(stod(axis));
        points.push_back({i, coord});
        KD.insert(coord, i++);
    }
    f.close();

    vd searchPoint = {2425, 14, 477};
    int k=6;
    NNFuerzaBruta(searchPoint, dim);
    cout<<"\nK vecinos mas cercanos con k= "<<k<<'\n';
    for(auto it=NNBF.begin() ; it!=NNBF.end() && k-- ; it++)
    {
        cout<<it->first<<" a "<<it->second.first<<" con (";
        for(int i=0 ; i<dim ; ++i)
        {
            cout<<it->second.second[i];
            if(i!=dim-1)
                cout<<" ";
        }
        cout<<")\n";
    }
    cout<<"\nVECINO MAS CERCANO (FUERZA BRUTA)\n";
    cout<<"distancia "<<NNBF.begin()->first<<" a "<<NNBF.begin()->second.first<<" (";
    for(int i=0 ; i<dim ; ++i)
    {
        cout<<NNBF.begin()->second.second[i];
        if(i!=dim-1)
            cout<<" ";
    }
    cout<<")\n";

    cout<<"\nVECINO MAS CERCANO (KD_TREE)\n";
    KD.NNSearch(searchPoint);
}