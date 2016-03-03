//
//  main.cpp
//  Shortest Path
//
//  Created by Ian Murphy on 3/1/16.
//  Copyright © 2016 Ian Murphy. All rights reserved.
//

#include <iostream>
using std::cout;
using std::endl;
#include <fstream>
//#include "Graph.h"
#include "ShortestPathPackage.h"
#include <string>
#include <algorithm>
using std::string;

void Print(string data)
{
    cout<<data<<endl;
}

void ReadFile(string filename, Graph<string, string> & graph)
{
    std::ifstream in(filename);
    if(in.is_open())
    {
        char buffer[513];
        char * start = nullptr;
        char * end = nullptr;
        char * path = nullptr;
        char * mileage = nullptr;
        while(!in.eof())
        {
            in.getline(buffer, sizeof(buffer));
            start = strtok(buffer, ",");
            end = strtok(NULL, ",");
            path = strtok(NULL, ",");
            mileage = strtok(NULL, ",");
            graph.AddVertex(start);
            graph.AddVertex(end);
            graph.AddEdge(start, end, path, atof(mileage));
        }
    }
}



int main(int argc, const char * argv[])
{
    Graph<string, string> mygraph;
    
    ReadFile("input.txt", mygraph);
    bool exit = false;
    string from;
    string to;
    while(!exit)
    {
        cout<<"Calculate shortest path from: ";
        std::getline(std::cin, from);
        cout<<"To: ";
        std::getline(std::cin, to);
        ShortestPathPackage<string, string> sp(mygraph, from);
        sp.PathTo(to);
    }
    
    
    
    
    
    
    return 0;
}
