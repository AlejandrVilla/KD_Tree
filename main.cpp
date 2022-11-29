#include <iostream>
#include <math.h>
#include <set>
#include <vector>
#include "kdtree.h"
// #define vd vector<double>
using namespace std;

set<pair<double, pair<string, vd>>> NNBF;
vector<pair<string, vd>> points;

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
    const int dim=2;
    KD_Tree<string,dim> KD;
    vd point(dim);
    point[0]=30;
    point[1]=40;
    points.push_back({"A",point});
    point[0]=5;
    point[1]=25;
    points.push_back({"B",point});
    point[0]=10;
    point[1]=12;
    points.push_back({"C",point});
    point[0]=70;
    point[1]=70;
    points.push_back({"D",point});
    point[0]=50;
    point[1]=30;
    points.push_back({"E",point});
    point[0]=35;
    point[1]=45;
    points.push_back({"F",point});
    point[0]=25;
    point[1]=65;
    points.push_back({"G",point});
    point[0]=29;
    point[1]=98;
    points.push_back({"H",point});
    for(int i=0 ; i<points.size() ; ++i)
        KD.insert(points[i].second, points[i].first);

    cout<<"RECORRIDOS\n\n";
    cout<<"INORDER\n";
    KD.inOrder();
    cout<<"PREORDER\n";
    KD.preOrder();
    cout<<"POSTORDER\n";
    KD.postOrder();

    vd searchPoint = {31,99};
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
