#include "vfs.hpp"

// constructor of the VFS class
VFS::VFS() {
    // creates root node
    root = new Node("/", nullptr, folder, 0, getTime());

    // sets current Node
    curr_Node = root;

    // sets previous Node
    prev_Node = root;

}

// prints the available menu of commands
void VFS::help() {
    cout<<"List of available Commands:"<<endl
		<<"help                     : Prints the available menu of commands"<<endl
		<<"pwd                      : Prints the path of the current node"<<endl
		<<"ls [sort]                : Prints the children of the current node"<<endl
		<<"mkdir <foldername>       : Creates a folder under the current folder"<<endl
		<<"touch <filename> <size>  : Creates a file under the current node with specified filename and size"<<endl
		<<"cd [ foldername | .. | - | /my/path/name]   "<<endl
        <<"    [foldername]         : Changes current node to the specified folder"<<endl
        <<"    [..]                 : Changes current node to its parent folder"<<endl
        <<"    [-]                  : Changes current node to the previous working folder"<<endl
        <<"    [/my/path/name]      : Changes current node to the specified path if it exists"<<endl
		<<"rm <foldername>|<filename>   : Removes the specified folder or file"<<endl
        <<"find <foldername>|<filename> : Returns the path of the file or the folder if it exists"<<endl
        <<"mv <filename> <foldername>   : Moves a file located under the current node, to the specified folder path"<<endl
        <<"size <foldername>|<filename> : Returns the total size of the folder or file"<<endl
		<<"showbin                  : Shows the oldest node of the bin"<<endl
		<<"emptybin                 : Empties the bin"<<endl
		<<"exit                     : The program exits"<<endl;
}

// prints the path of the current node
string VFS::pwd() {
    if (curr_Node == root) {
        return root->name;
    }
    else {
        return getPath(curr_Node);
    }
}

// prints the children of the current node
void VFS::ls() {
    // loops through the children of the current node and prints them
    for (int i = 0; i < curr_Node->children.size(); i++) {
        // checks the type of the child
        if (curr_Node->children[i]->type == folder) {
            cout << setw(4) << "dir" << " ";
        }
        else {
            cout << setw(4) << "file" << " ";
        }

        cout << setw(10) << curr_Node->children[i]->name 
            << " " << setw(10) << curr_Node->children[i]->size
            << " " << setw(15) << curr_Node->children[i]->time_created;
        
    }
}

// creates a folder under the current folder
void VFS::mkdir(string folder_name) {
    // creates folder if folder name is valid
    if (isValid(folder_name, folder)) {
        // checks if folder name is unique
        if (isUnique(folder_name, curr_Node)) {
            // creates a folder node
            Node* newFolder = new Node(folder_name, curr_Node, folder, 10, getTime());

            // adds to the children of current node
            curr_Node->children.push_back(newFolder);

            // updates size of the current folder
            updateSize(newFolder);
        }
        else {
            throw runtime_error("Folder name is not unique");
        }
    }
    else {
        throw runtime_error("Folder name is not valid");
    }
    
}

// creates a file under the current folder with specified filename and size
void VFS::touch(string file_name, unsigned int size) {
    // checks if the filename is valid
    if (isValid(file_name, file)) {
        // checks if the file name is unique
        if (isUnique(file_name, curr_Node)) {
            // creates a file node
            Node* newFile = new Node(file_name, curr_Node, file, size, getTime());

            // adds to the children of current node
            curr_Node->children.push_back(newFile);

            // updates size of the current folder
            updateSize(newFile);
        }
        else {
            throw runtime_error("File name is not unique");
        }
    }
    else {
        throw runtime_error("File name is not valid");
    }
}

// changes the current folder to the specified directory
void VFS::cd(string path) {
    // checks if the path specified is a file or folder
    size_t found = path.find('/');
    // if the path does not contain '/', then it is either a file or folder
    if(found == string::npos) {
        // ------- checks for specific cd parameter -------------
        // checks for ".." argument
        if (path ==  "..") {
            // checks if the current node is root and throws an error
            if (curr_Node == root) {
                throw runtime_error("Root has no parent folder");
            }

            prev_Node = curr_Node;
            curr_Node = curr_Node->parent;
        }

        // checks for "-" argument
        else if (path == "-") {
            Node* temp = prev_Node;
            prev_Node = curr_Node;
            curr_Node = temp;
        }

        // checks for no argument
        else if (path == "") {
            prev_Node = curr_Node;
            curr_Node = root;
        }
        // checks for folder name argument
        else {
            // gets node of the specified name
            Node* nextCurr = getChild(curr_Node, path);

            // checks if the specified node exists in the folder
            if (nextCurr == nullptr) {
                throw runtime_error("Specified folder does not exist");
            }

            // checks if the specified node is not a file
            if (nextCurr->type == file) {
                throw runtime_error("Cannot change directory to a file");
            }

            // updates current node
            prev_Node = curr_Node;
            curr_Node = nextCurr;
        } 
    }
    else{
        // gets node at the given path
        Node* tracking_ptr = getNode(path);

        // check if a valid node was returned
        if (tracking_ptr == nullptr) {
            throw runtime_error("Invalid path");
        }

        // checks if path points to a file
        if (tracking_ptr->type == file) {
            throw runtime_error("Cannot change directory to a file");
        }

        prev_Node = curr_Node;
        curr_Node = tracking_ptr;

    }
}

// removes the specified folder or file
void VFS::rm(string file_name) {
    // variable that holds node to be removed
    Node* removeNode = getChild(curr_Node, file_name);

    // checks if the specified name exists
    if (removeNode == nullptr) {
        throw runtime_error("File or folder does not exist");
    }

    // move to bin if found
    bin.enqueue(removeNode);

    // keep track of path to item removed
    bin_paths.enqueue(getPath(removeNode));

    // remove node from children of current node
    for (int i = 0; i < curr_Node->children.size(); i++) {
        if (curr_Node->children[i] == removeNode) {
            curr_Node->children.erase(i);
            break;
        }
    }

    // update size of folder
    updateSize(curr_Node);
}

// returns the total size of the folder or file
void VFS::size(string path) {
    // checks if the path specified is a file or folder
    size_t found = path.find('/');
    // if the path does not contain '/', then it is either a file or folder
    if(found == string::npos) {
        // gets node of the specified name
        Node* tracking_ptr = getChild(curr_Node, path);

        // checks if the specified node exists in the folder
        if (tracking_ptr == nullptr) {
            throw runtime_error("Specified file or folder does not exist");
        }

        // prints size 
        cout << tracking_ptr->size << endl;
    }
    else {
        // checks if the path is the root
        if (path == root->name) {
            cout << root->size << endl;
        }
        else {
            // gets node at the given path
            Node* tracking_ptr = getNode(path);

            // check if a valid node was returned
            if (tracking_ptr == nullptr) {
                throw runtime_error("Invalid path");
            }

            // prints size 
            cout << tracking_ptr->size << endl;
        }
    }
}

// shows the oldest node of the bin
void VFS::showbin() {
    // checks if bin is empty
    if (bin.isEmpty()) {
        throw runtime_error("Bin is empty");
    }

    // checks the type of the child
    if (bin.front_element()->type == folder) {
        cout << setw(4) << "dir" << " ";
    }
    else {
        cout << setw(4) << "file" << " ";
    }

    cout << setw(10) << bin.front_element()->name 
        << " " << setw(5) << bin.front_element()->size
        << " " << setw(15) << bin_paths.front_element()
        << " " << setw(15) << bin.front_element()->time_created;
}

// empties the bin
void VFS::emptybin() {
    while (!bin.isEmpty()) {
        bin.dequeue();
        bin_paths.dequeue();
    }
}

// returns the path of the file or folder if it exits
void VFS::find(string name) {
    // variable for holding matching nodes
    Queue<Node*> matching_nodes;

    // populates matching_nodes with all matching nodes
    getMatchingNode(root, name, matching_nodes);

    // enqueues path of matching nodes to path queue
    while (!matching_nodes.isEmpty()) {
        cout << getPath(matching_nodes.dequeue()) << endl;
    }
}

// moves a file located under the current node to the specified path
void VFS::mv(string file, string folder) {

    // gets node of file and folder
    Node* file_node = (file.find('/') != string::npos) ? getNode(file) : getChild(curr_Node, file);
    Node* folder_node = (folder.find('/') != string::npos) ? getNode(folder) : getChild(curr_Node, folder);

    // checks if file and folder exist
    if ((file_node == nullptr) || (folder_node == nullptr)) {
        throw runtime_error("File or folder not located at specified path");
    }

    // checks if folder is actually of type folder
    if (folder_node->type != NodeType::folder) {
        throw runtime_error("Cannot change the directory to a file");
    }

    // adds file at folder
    folder_node->children.push_back(file_node);

    // remove file_node from children of its parent node
    for (int i = 0; i < file_node->parent->children.size(); i++) {
        if (file_node->parent->children[i] == file_node) {
            file_node->parent->children.erase(i);
            break;
        }
    }

    // updates size of the folder
    if (file_node->parent == folder_node->parent) {
        updateSize(folder_node);
    } 
    else {
        updateSize(folder_node);
        updateSize(file_node->parent);
    }

    // update parent of file_node
    file_node->parent = folder_node;
}

// exits the program
void VFS::exit() {

}


// ---------------- HELPER METHODS -------------------------

// returns system time as a string
string VFS::getTime() {
    // creates a variable of hold current time  of type time_t
    time_t timer;

    // gets current time
    time(&timer);

    // converts time_t value to string
    return ctime(&timer);
}

// returns the path of the current node
string VFS::getPath(Node *ptr) {
    // checks if the current node is the root
    if (ptr->name ==  root->name) {
        return "";
    }
    else {
        return getPath(ptr->parent) + '/' + ptr->name;
    }
}

// checks if file or folder name is valid
bool VFS::isValid(string name, NodeType type) {
    // folder check
    if (type == folder) {
        // checks for every character in folder name
        for (int i = 0; i < name.size(); i++) {
            if ((int(name[i]) >= int('A') &&  int(name[i]) <= int('Z')) || 
                (int(name[i]) >= int('a') && int(name[i]) <= int('z')) || 
                (int(name[i]) >= int('0') && int(name[i]) <= int('9'))) {}
            else {
                return false;
            }
        }
        return true;
    }
    else {
        // checks for every character in file name
        for (int i = 0; i < name.size(); i++) {
            if ((int(name[i]) >= int('A') &&  int(name[i]) <= int('Z')) || 
                (int(name[i]) >= int('a') && int(name[i]) <= int('z')) || 
                (int(name[i]) >= int('0') && int(name[i]) <= int('9')) ||
                (int(name[i]) == int('.'))) {}
            else {
                return false;
            }
        }
        return true;
    }
}

// checks if file or folder name is unique
bool VFS::isUnique(string name, Node* curr_dir) {
    // checks the file or folder name is unique
    for (int i = 0; i < curr_dir->children.size(); i++) {
        if (curr_dir->children[i]->name == name) {
            return false;
        }
    }

    return true;
}

// returns a specific child of given Node
Node* VFS::getChild(Node *ptr, string childname) {
    for (int i = 0; i < ptr->children.size(); i++) {
        if (ptr->children[i]->name == childname) {
            return ptr->children[i];
        }
    }
    return nullptr;
}	

// populates a queue with matching nodes
void VFS::getMatchingNode(Node *ptr, string name, Queue<Node*>& matching_nodes) {
    for (int i = 0; i < ptr->children.size(); i++) {
        if (ptr->children[i]->name == name) {
            matching_nodes.enqueue(ptr->children[i]);
        }
        getMatchingNode(ptr->children[i], name, matching_nodes);
    }
}

//Helper method to get a pointer to Node at given path
Node* VFS::getNode(string path) {
    // converts path to stringstream
    stringstream sstr(path);

    // variable for keeping track subdirectories
    string current_child;

    // reads first current folder from the path
    getline(sstr, current_child, '/');

    // variable for keeping track of subdirectories
    Node* tracking_ptr = path[0] == '/' ? root : curr_Node;

    // loops through path to get to specific node
    while (!sstr.eof()) {
        getline(sstr, current_child, '/');
        tracking_ptr = getChild(tracking_ptr, current_child);
    }

    return tracking_ptr;
}

// re-calculates the size of an Node and its parents
void VFS::updateSize(Node *ptr) {
    // checks if ptr is a folder and updates its size
    if (ptr->type == folder) {
        // set the size to the default size
        ptr->size = 10;

        // adds the size of children
        for (int i = 0; i < ptr->children.size(); i++) {
            ptr->size += ptr->children[i]->size;
        }
    }

    while (ptr->parent != nullptr) {
        // set the size of the parent to the default size if not root
        if (ptr->parent != root) {
            ptr->parent->size = 10;
        }
        else {
            ptr->parent->size = 0;
        }

        // adds the size of children
        for (int i = 0; i < ptr->parent->children.size(); i++) {
            ptr->parent->size += ptr->parent->children[i]->size;
        }

        // updates current ptr
        ptr = ptr->parent;
    }
}				