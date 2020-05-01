CC=g++
CFLAGS=-c -Wall -std=c++17
LDFLAGS=-ldl -rdynamic

run.exe: main.o DynamicAllocationTroubleshooter.o
	$(CC) main.o DynamicAllocationTroubleshooter.o -o run.exe $(LDFLAGS)
main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp
DynamicAllocationTroubleshooter.o: DynamicAllocationTroubleshooter/DynamicAllocationTroubleshooter.cpp DynamicAllocationTroubleshooter/DynamicAllocationTroubleshooter.h
	cd DynamicAllocationTroubleshooter; g++ -std=c++17 -c DynamicAllocationTroubleshooter.cpp; mv DynamicAllocationTroubleshooter.o ..; cd ..;
clean:
	rm *.o *.exe