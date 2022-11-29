#ifndef KD_TREE_H
#define KD_TREE_H
#include <iostream>
#include <math.h>
#include <vector>
#include "node.h"
#define vd std::vector<double>

template<typename T, int dim=2>
class KD_Tree
{
private:
    Node<T,dim>* root;

    // punto, valor, dimension actual (x, y, ...), nodo, padre del nodo
    void insert(vd& coord, T value, int cd, Node<T,dim>*& N, Node<T,dim>* P);       // O(logn)
    // busca posibles puntos mas cercanos en otras ramas (regiones)
    void prune(vd& coord, Node<T,dim>* N, Node<T,dim>* &best, double& best_dist);  
    // compara si la distancia al punto de P es mas corta       
    void distance(vd& coord, Node<T,dim>* N, double& best_dist, Node<T,dim>*& best);

    void inOrder(Node<T,dim>* N);
    void postOrder(Node<T,dim>* N);
    void preOrder(Node<T,dim>* N);
public:
    KD_Tree():root(nullptr){}
    ~KD_Tree();
    // punto, valor
    void insert(vd& coord, T value);       
    // busca vecino mas cercano 
    void NNSearch(vd& coord);              

    void inOrder();
    void postOrder();
    void preOrder();
};

template<typename T, int dim>
KD_Tree<T,dim>::~KD_Tree()
{
    root->automatate(root);
    std::cout<<"\ntermino\n";
}

template<typename T, int dim>
void KD_Tree<T,dim>::insert(vd& coord, T value)
{
    // std::cout<<"insertando "<<coord[dim-1]<<'\n';
    insert(coord, value, 0, root, nullptr);
}

template<typename T, int dim>
void KD_Tree<T,dim>::insert(vd& coord, T value, int cd, Node<T,dim>*& N, Node<T,dim>* P)
{
    if(!N)
    {
        N = new Node<T,dim>(value, coord);
        N->cd = cd;
        N->P = P;
        return;
    }
    if(coord[cd] < N->coord[cd])
        insert(coord, value, (cd+1)%dim, N->L, N);
    else
        insert(coord, value, (cd+1)%dim, N->R, N);
}

template<typename T, int dim>
void KD_Tree<T,dim>::inOrder()
{
    inOrder(root);
}

template<typename T, int dim>
void KD_Tree<T,dim>::inOrder(Node<T,dim>* N)
{
    if(!N) return;
    inOrder(N->L);
    std::cout<<N->value<<" (";
    for(int i=0 ; i<dim ; ++i)
    {
        std::cout<<N->coord[i];
        if(i!=dim-1)
            std::cout<<" ";
    }
    std::cout<<")"<<" padre "<<(N->P? N->P->value:"root")<<"\n";
    inOrder(N->R);
}

template<typename T, int dim>
void KD_Tree<T,dim>::postOrder()
{
    postOrder(root);
}

template<typename T, int dim>
void KD_Tree<T,dim>::postOrder(Node<T,dim>* N)
{
    if(!N) return;
    postOrder(N->L);
    postOrder(N->R);
    std::cout<<N->value<<" (";
    for(int i=0 ; i<dim ; ++i)
    {
        std::cout<<N->coord[i];
        if(i!=dim-1)
            std::cout<<" ";
    }
    std::cout<<")"<<" padre "<<(N->P? N->P->value:"root")<<"\n";
}

template<typename T, int dim>
void KD_Tree<T,dim>::preOrder()
{
    preOrder(root);
}

template<typename T, int dim>
void KD_Tree<T,dim>::preOrder(Node<T,dim>* N)
{
    if(!N) return;
    std::cout<<N->value<<" (";
    for(int i=0 ; i<dim ; ++i)
    {
        std::cout<<N->coord[i];
        if(i!=dim-1)
            std::cout<<" ";
    }
    std::cout<<")"<<" padre "<<(N->P? N->P->value:"root")<<"\n";
    preOrder(N->L);
    preOrder(N->R);
}

template<typename T, int dim>
void KD_Tree<T,dim>::distance(vd& coord, Node<T,dim>* N, double& best_dist, Node<T,dim>*& best)
{
    double dist = 0;
    for(int i=0 ; i<dim ; ++i)
        dist += pow(N->coord[i] - coord[i], 2); // acumula segun la dimension
    dist = sqrt(dist);
    if(dist < best_dist)
    {
        best_dist = dist;
        best = N;
    }
}

template<typename T,int dim>
void KD_Tree<T,dim>::NNSearch(vd& coord)
{
    Node<T,dim>* tmp = root;
    Node<T,dim>* p = 0;
    bool child = 0;     // L->0, R->1
    // baja a una hoja
    while(tmp)
    {
        p = tmp;
        if(coord[tmp->cd] < tmp->coord[tmp->cd])
        {
            tmp = tmp->L;
            child = 0;
        }
        else
        {
            tmp = tmp->R;
            child = 1;
        }
    }
    Node<T,dim>* best = p;
    double best_dist = INT_MAX;
    // primera mejor distancia
    distance(coord, p, best_dist, best);
    // sube a los padres hasta llegar a la raiz
    p = p->P;
    while(p)
    {
        // nueva posible mejor distancia
        distance(coord, p, best_dist, best);

        // poda ramas si hay interseccion con otra region, 
        // debe buscar posibles puntos mas cercanos
        if(!child && abs(coord[p->cd] - p->coord[p->cd]) < best_dist)
            prune(coord, p->R, best, best_dist);
        else if(child && abs(coord[p->cd] - p->coord[p->cd]) < best_dist)
            prune(coord, p->L, best, best_dist);

        // de que rama viene
        if(p->P && p->P->L == p)
            child = 0;
        else if(p->P && p->P->R == p)
            child = 1;
        // sube al padre
        p = p->P;
    }
    std::cout<<"distancia "<<best_dist<<" a "<<best->value<<" (";
    for(int i=0 ; i<dim ; ++i)
    {
        std::cout<<best->coord[i];
        if(i!=dim-1)
            std::cout<<" ";
    }
    std::cout<<")\n";
}

template<typename T,int dim>
void KD_Tree<T,dim>::prune(vd& coord, Node<T,dim>* N, Node<T,dim>*& best, double& best_dist)
{
    // mientras no sea hoja
    if(!N) return;
    // std::cout<<"podando "<<N->value<<'\n';
    // nueva posible mejor distancia
    distance(coord, N, best_dist, best);
    // busca hasta las hojas
    prune(coord, N->L, best, best_dist);
    prune(coord, N->R, best, best_dist);
}

#endif