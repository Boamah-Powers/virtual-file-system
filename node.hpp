#ifndef NODE_H
#define NODE_H

#include<cstdlib>
#include<string>
#include<ctime>
#include "vector.hpp"

using namespace std;

// sets the type of node being created
enum NodeType {
    file = 0, 
    folder = 1,
};

class Node {
    private:
        string name;            // name of the node
        NodeType type;          // type of node being created
        unsigned int size;      // size of the current node
        string time_created;    // timestamp of the node
        Vector<Node*> children; // children of the node
        Node* parent;           // parent of the node

    public:
		Node(string name, Node* parent, NodeType type, unsigned int size, string time_created) : 
        name(name), type(type), size(size), time_created(time_created), parent(parent)
		{ }

		friend class VFS;

};

#endif