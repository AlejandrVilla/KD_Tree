#ifndef NODE_H
#define NODE_H
#include <vector>
#define vd std::vector<double>

template<typename T, int dim=2>
class Node
{
public: 
    vd coord; // 0->x, 1->y, 2->z, ...
    T value;
    Node<T,dim>* L;
    Node<T,dim>* R;
    Node<T,dim>* P;
    int cd;

    Node();
    Node(T _value, vd& _coord);
    void automatate(Node<T,dim>* N);
};

template<typename T, int dim>
Node<T,dim>::Node()
{
    for(int i=0 ; i<dim ; ++i)
        coord.push_back(0);
    cd = -1;
    L = R = P = nullptr;
}

template<typename T, int dim>
Node<T,dim>::Node(T _value, vd& _coord)
{
    value = _value;
    for(int i=0 ; i<dim ; ++i)
        coord.push_back(_coord[i]);
    cd = -1;
    L = R = P = nullptr;
}

template<typename T, int dim>
void Node<T,dim>::automatate(Node<T,dim>* N)
{
    if(!N) return;
    N->automatate(N->L);
    N->automatate(N->R);
    delete N;
}

#endif