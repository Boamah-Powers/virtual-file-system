vfs: vfs.o main.o
	g++ vfs.o  main.o -o vfs
vfs.o: vfs.hpp vfs.cpp node.hpp queue.hpp vector.hpp
	g++ -c vfs.cpp
main.o: main.cpp
	g++ -c main.cpp
clean: 
	rm *.o vfs