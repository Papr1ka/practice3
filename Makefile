main.o : main.cpp
	g++ -c main.cpp
binaryutils.o: binaryutils.cpp
	g++ -c binaryutils.cpp
hashtable.o: hashtable.cpp
	g++ -c hashtable.cpp
hashbinary.o: hashbinary.cpp
	g++ -c hashbinary.cpp
run: hashbinary.o hashtable.o binaryutils.o main.o
	g++ -o main main.cpp hashtable.cpp binaryutils.cpp hashbinary.cpp
	chmod +x ./main
	./main