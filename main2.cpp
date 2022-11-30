#include <iostream>
#include <math.h>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include "kdtree.h"
// #define vd vector<double>
using namespace std;

set<pair<double, pair<int, vd>>> NNBF;
vector<pair<int, vd>> points;       // tiempo insercion O(1)

// distancia a todos los puntos
void NNFuerzaBruta(vd& coord, int dim)      // O()
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
    string dir = "20000.csv";
    KD_Tree<int, dim> KD;

    cout<<"USANDO ARCHIVO "<<dir<<'\n';
    ifstream f(dir);
    int i=1;
    auto start_insertion_BF = chrono::high_resolution_clock::now();
    while(!f.eof())
    {
        string linea;
        f>>linea;
        stringstream str(linea);
        vd coord;
        string axis;
        while(getline(str,axis,','))
            coord.push_back(stod(axis));
        points.push_back({i++, coord});
    }
    f.close();
    auto end_insertion_BF = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> insertion_time_BF = end_insertion_BF - start_insertion_BF;
    
    f.open(dir);
    i=1;
    auto start_KD = chrono::high_resolution_clock::now();
    while(!f.eof())
    {
        string linea;
        f>>linea;
        stringstream str(linea);
        vd coord;
        string axis;
        while(getline(str,axis,','))
            coord.push_back(stod(axis));
        KD.insert(coord, i++);
    }
    f.close();
    auto end_KD = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> insertion_time_KD = end_KD - start_KD;

    cout<<"TIEMPO DE INSERCION (FUERZA BRUTA) "<<insertion_time_BF.count()<<'\n';
    cout<<"TIEMPO DE INSERCION (KD TREE) "<<insertion_time_KD.count()<<'\n';

    double x,y,z;
    x=2425; y=14; z=477;
    vd searchPoint = {x, y, z};
    cout<<"\nBUSCANDO PUNTO ("<<x<<","<<y<<","<<z<<")\n";

    int k=6;
    auto start_time_NNBF = chrono::high_resolution_clock::now();
    NNFuerzaBruta(searchPoint, dim);
    auto end_time_NNBF = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> time_NNBF = end_time_NNBF-start_time_NNBF;

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
    auto start_time_NNKD = chrono::high_resolution_clock::now();
    KD.NNSearch(searchPoint);
    auto end_time_NNKD = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> time_NNKD = end_time_NNKD-start_time_NNKD;

    cout<<"\nTIEMPO DE BUSQUEDA DEL VECINO MAS CERCANO (FUERZA BRUTA) "<<time_NNBF.count()<<'\n';
    cout<<"TIEMPO DE BUSQUEDA DEL VECINO MAS CERCANO (KD TREE) "<<time_NNKD.count()<<'\n';
}