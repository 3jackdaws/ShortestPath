
//  ShortestPathPackage.h
//  Shortest Path
//
//  Created by Ian Murphy on 3/1/1
//  Copyright © 2016 Ian Murphy. All rights reserved.
//

#ifndef ShortestPathPackage_h
#define ShortestPathPackage_h
#include "Graph.h"
#include <vector>
#include <iomanip>

void Print1(string data)
{
    cout<<data<<endl;
}

template <typename V, typename E>
class ShortestPathPackage
{
    friend class Graph<V,E>;
public:
    ShortestPathPackage(Graph<V,E> & graph, V start);
    //ShortestPathPackage(const ShortestPathPackage & cp);
    //ShortestPathPackage & operator = (const ShortestPathPackage & rhs);
    //~ShortestPathPackage();
    
    //V GetStartPoint();
    //void Begin();
    //void MoveNext();
    //const Vertex<V, E> & GetVertex() const;
    //bool IsDone();
    void PathTo(V data);
private:
    typename std::vector<Vertex<V, E>>::iterator _internal;
    bool _valid;
    std::vector<Vertex<V, E>> _ordered;
    std::vector<double> _distance;
    std::vector<int> _predecessor;
    std::vector<bool> _processed;
    int _start_pos;
    
    int FindSmallest();
    void ComputeAdjacent(int index);
    int GetIndexOf(const Vertex<V, E> & vertex) const;
    bool Done();
    
    Edge<V, E> & GetLinkingEdge(Vertex<V, E> & from, Vertex<V, E> & to);
    int VertexDataToIndex(V data);
};


template <typename V, typename E>
ShortestPathPackage<V,E>::ShortestPathPackage(Graph<V, E> & graph, V start): _valid(false)
{

    for(Vertex<V, E> vertex: graph.GetVertexList())
    {
        _ordered.push_back(vertex);
    }
    Vertex<V, E> swap;
    //_start_pos = VertexDataToIndex(start);
    //typename std::vector<Vertex<V, E>>::iterator index;
    bool found = false;
    int index;
    for(index = 0; !_valid && index<_ordered.size(); index++)
    {
        if(_ordered[index].GetData() == start)
        {
            _valid = true;
            _start_pos = index;
        }
    }
    if(_valid)
    {
        _distance.assign(_ordered.size(), -1);
        _predecessor.assign(_ordered.size(), -1);
        _processed.assign(_ordered.size(), false);
        
        _distance[_start_pos] = 0;
        ComputeAdjacent(_start_pos);
        
        while(!Done())
        {
            int small = FindSmallest();
            ComputeAdjacent(small);
        }
        
    }
    else
    {
        cout<<start<<" is not a valid start point."<<endl;
    }
}

template <typename V, typename E>
int ShortestPathPackage<V,E>::FindSmallest()
{
    unsigned int smallest = -1;
    int index = 0;
    for (int i = 0; i<_distance.size(); i++)
    {
        if(_distance[i] > 0 && !_processed[i] && _distance[i] < smallest)
        {
            index = i;
            smallest = _distance[i];
        }
    }
    return index;
}




template <typename V, typename E>
void ShortestPathPackage<V,E>::ComputeAdjacent(int index)
{
#ifdef __DEBUG__
    cout<<"From "<<_ordered[index].GetData() <<endl;
#endif
    for(Edge<V, E> & edge : _ordered[index].GetEdgeList())
    {
        if(!_processed[VertexDataToIndex(edge.GetLink().GetData())])
        {
#ifdef __DEBUG__
            cout<<"\t"<<"Processing"<<edge.GetLink().GetData()<<endl;
#endif
            int edge_link_index = GetIndexOf(edge.GetLink());
            int distance = edge.GetWeight();
            if(_distance[index] > 0)
                distance += _distance[index];
            if(distance < _distance[edge_link_index] || _distance[edge_link_index] < 0)
            {
                _distance[edge_link_index] = distance;
                _predecessor[edge_link_index] = index;
            }
        }
#ifdef __DEBUG__
        else
        {
            cout<<"\t"<<edge.GetLink().GetData()<<" already Processed"<<endl;
        }
#endif
    }
    _processed[index] = true;
}

template <typename V, typename E>
int ShortestPathPackage<V,E>::GetIndexOf(const Vertex<V, E> & vertex) const
{
    int index = 0;
    bool found = false;
    for (int i = 0; !found && i<_ordered.size(); i++)
    {
        if(_ordered[i].GetData() == vertex.GetData())
        {
            found = true;
            index = i;
        }
    }
    return index;
}


template <typename V, typename E>
bool ShortestPathPackage<V,E>::Done()
{
    int num_lt_zero = 0;
    for (int i = 0; i<_ordered.size(); i++)
    {
        if(!_processed[i])
        {
            return false;
        }
    }
    return true;
}

template <typename V, typename E>
void ShortestPathPackage<V,E>::PathTo(V data)
{
    if(_valid)
    {
        int index = VertexDataToIndex(data);
        if(index >= 0)
        {
            //int distance = 0;
            std::list<Vertex<V, E>> stack;
            stack.push_back(_ordered[index]);
            while(index != _start_pos)
            {
                index = _predecessor[index];
                stack.push_back(_ordered[index]);
            }
            while (stack.back().GetData() != data)
            {
                Vertex<V, E> current = stack.back();
                stack.pop_back();
                Edge<V, E> route;
                int distance = _distance[VertexDataToIndex(stack.back().GetData())];
                double time = distance/55.00;
                if(GetLinkingEdge(current, stack.back()).GetData() == "I-5")
                    time = distance/65.00;
                int inttime = time*3600;
                int hours = inttime/3600;
                int minutes = (inttime-(hours*3600))/60;
                int seconds = (inttime-(minutes*60)-(hours*3600));
                cout<<"From " << current.GetData() << " along " << GetLinkingEdge(current, stack.back()).GetData() << " to " << stack.back().GetData() <<" Distance: " << distance <<" Miles, Travel Time: " << std::setw(2)<<std::setfill('0')<< hours<<":"<<minutes<<":"<<seconds <<endl;
            }
            
        }
        else
        {
            cout<<data<< " could not be found."<<endl;
        }
    }
    else
    {
        
    }
}

template <typename V, typename E>
int ShortestPathPackage<V,E>::VertexDataToIndex(V data)
{
    int index = -1;
    bool found = false;
    for (int i = 0; !found && i<_ordered.size(); i++)
    {
        if(_ordered[i].GetData() == data)
        {
            found = true;
            index = i;
        }
    }
    return index;
}

template <typename V, typename E>
Edge<V,E> & ShortestPathPackage<V,E>::GetLinkingEdge(Vertex<V, E> &from, Vertex<V, E> &to)
{
    from.Begin();
    while (from.GetEdge().GetLink().GetData() != to.GetData())
    {
        from.MoveNext();
    }
    return from.GetEdge();
}










#endif /* ShortestPathPackage_h */
