#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <unordered_set>
#include <queue>

#include "types.h"


template <typename Key>
class Graph
{
private:
    /*
    template <typename U>
    class Edge
    {

    };*/
public:
    void addEdge(const Key&);
    void addArc(const Key&, const Key&);
    std::vector<Key> getNeighbors(const Key&);

    uint getDegree(const Key&);
    std::vector<Key> getKeys();

public:

private:
    uint nodeCount;
    std::map<uint, Key> nodes;
    std::map<Key, uint> nodeIds;
    std::map<Key, std::unordered_set<uint>> arcs;

};


template<typename Key>
void Graph<Key>::addEdge(const Key& key)
{
    if (arcs.find(key) != arcs.end())
    {
        //std::cout << "already in graph" << "\n";
    }
    else
    {
        nodeIds.insert(std::pair<Key, uint>(key, nodeCount));
        nodes.insert(std::pair<uint, Key>(nodeCount, key));
        std::unordered_set<uint> neighbors;
        arcs.insert(std::pair<Key, std::unordered_set<uint>>(key, neighbors));
        //std::cout << "added edge" << key << ", id: "<<nodeCount<<"\n";
        nodeCount++;
    }
}

template<typename Key>
void Graph<Key>::addArc(const Key& key1, const Key& key2)
{
    addEdge(key1);
    addEdge(key2);
    uint id1 = nodeIds.at(key1);
    uint id2 = nodeIds.at(key2);
    //std::cout << "before arcs.at(key1).size(): "<<arcs.at(key1).size() << "\n";
    arcs.at(key1).insert(nodeIds.at(key2));
    //std::cout << "after arcs.at(key1).size(): " << arcs.at(key1).size() << "\n";
    arcs.at(key2).insert(nodeIds.at(key1));
    //std::cout << "added arc: " << key1 << "\t" << key2 << "\n";
}

template<typename Key>
std::vector<Key> Graph<Key>::getNeighbors(const Key& key)
{
    //std::cout << "begin getNeighbors()\n";
    std::vector<Key> neighbors;
    if (arcs.find(key) != arcs.end())
    {
        for (auto id : arcs.at(key))
        {
            neighbors.push_back(nodes.at(id));
            //std::cout << nodes.at(id) << " is neighbor of " << key << "\n";
        }
    }
    else
    {
       // std::cout << key << " not in graph\n";
    }
    //std::cout << "end getNeighbors()\n";
    return neighbors;
}

template<typename Key>
uint Graph<Key>::getDegree(const Key& key)
{
    return arcs.at(key).size();
}


template<typename Key>
std::vector<Key> Graph<Key>::getKeys()
{
    std::vector<Key> v;

    for (auto p : nodes)
    {
        v.push_back(p.second);
    }

    return v;
}



// non-member functions
/*
template<typename Key>
std::vector<std::vector<Key>> getConnexParts(Graph<Key> g);


template<typename Key>
std::vector<std::vector<Key>> getConnexParts(Graph<Key> g)
{
    std::vector <std::vector<Key>> connexParts;
    uint nbConnexes = 0;

    auto keys = g.getKeys();
    std::vector<char> hasBeenExplored(keys.size());
    std::vector<int> connexId(keys.size(), -1);
    std::queue<Key> toBeExplored;


    uint explorationIterator = 0;
    while( explorationIterator < keys.size())
    {
        while (toBeExplored.size() == 0 && explorationIterator < keys.size())
        {
            if (!hasBeenExplored[explorationIterator])
            {
                toBeExplored.push(explorationIterator);
            }
            explorationIterator++;
        }

        if (toBeExplored.size() > 0)
        {
            auto currentlyExploring = toBeExplored.pop();

        }



    }




    return connexParts;
}

*/

#endif
