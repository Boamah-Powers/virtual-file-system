# virtual-file-system

This project focuses on replicating the Unix-based directory system using tree data structure implementation. 

To run the program, execute the make command from the terminal and open the 'vfs' executable file

# Supported commands
1. help			- Prints the available menu of commands
2. pwd			- Prints the path of the current node
3. ls           - Prints the children of the current node
4. mkdir		- Creates a folder under the current folder
5. touch		- Creates a file under the current node with specified filename and size
6. cd  			- Changes current node per arguments passed
7. rm			- Removes the specified folder or file
8. find			- Returns the path of the file or the folder if it exists
9. mv			- Moves a file located under the current node, to the specified folder path
10. size		- Returns the total size of the folder or file
11. showbin		- Shows the oldest node of the bin
12. emptybin	- Empties the bin
13. exist		- The program exits

# Feature(s) to be implemented
14. recover		- Reinstates the oldest node back from the bin to its original position

# Additional(s) to be implemented
1. Ability to read and write current file system to a file
