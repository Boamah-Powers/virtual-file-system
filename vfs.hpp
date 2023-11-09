
#ifndef VFS_H
#define VFS_H

#include<iostream>
#include<iomanip>
#include<cstdlib>
#include<string>
#include<ctime>
#include<sstream>
#include<fstream>
#include "node.hpp"
#include "queue.hpp"

using namespace std;

class VFS
{
	private:
		Node *root;				//root of the VFS
		Node *curr_Node;			//current Node
		Node *prev_Node;			//previous Node
		Queue<Node*> bin;			//bin containing the deleted Nodes
		Queue<string> bin_paths;	//paths of the items in bin
	
	public:	 	
		//Required methods
		VFS();	
        // ~VFS();   todo
		void help();						
		string pwd();
		void ls();						
		void mkdir(string folder_name);
		void touch(string file_name, unsigned int size);
		void cd(string path);
		void rm(string file_name);
        void find(string name);
        void mv(string file, string folder);
		void size(string path);
		void showbin();
		void emptybin();
		void exit();

        // ---------------- Helper methods -------------------------
        string getTime();                           // returns system time as a string
        string getPath(Node* ptr);                  // returns the path of the current node
        bool isValid(string name, NodeType type);   // checks if file or folder name is valid
        bool isUnique(string name, Node* curr_dir); // checks if file or folder name is unique
		bool find_helper(Node *ptr, string name);	//recursive method to check if a given child is present under specific Node or not
        Node* getChild(Node *ptr, string childname);	//returns a specific child of given Node
        void updateSize(Node *ptr);                  // re-calculates the size of a Node and its parents
		void getMatchingNode(Node *ptr, string name, Queue<Node*>& matching_nodes); // populates a queue with matching nodes
        Node* getNode(string path);			//Helper method to get a pointer to Node at given path
        /*
		//Optional methods
		void find(string name);
		void mv(string file, string folder);
		void recover();

		//Optional helper methods
		void load(ifstream &fin);				//Helper method to load the vfs.dat
		Node* getNode(string path);			//Helper method to get a pointer to Node at given path
		Node* getChild(Node *ptr, string childname);	//returns a specific child of given Node
		bool find_helper(Node *ptr, string name);		//recursive method to check if a given child is present under specific Node or not
		void updateSize(Node *ptr);					//re-calculatre the size of an Node and its parents
		void write(ofstream &fout, Node *ptr);			//traverse and write recrusively the vfs data
		string getTime();								//return system time as an string
		*/
};
//===========================================================
#endif
