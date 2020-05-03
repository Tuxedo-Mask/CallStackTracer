CC=g++
CXXFLAGS=-c -g -Wall -std=c++17 -fno-pie -no-pie  -DBOOST_STACKTRACE_USE_ADDR2LINE
LDFLAGS=-ldl -rdynamic

run.exe: main.o DynamicAllocationTroubleshooter.o
	$(CC) -g -no-pie -fno-pie main.o DynamicAllocationTroubleshooter.o -o run.exe $(LDFLAGS)
main.o: main.cpp
	$(CC) $(CXXFLAGS) main.cpp
DynamicAllocationTroubleshooter.o: DynamicAllocationTroubleshooter/DynamicAllocationTroubleshooter.cpp DynamicAllocationTroubleshooter/DynamicAllocationTroubleshooter.h
	cd DynamicAllocationTroubleshooter; $(CC) $(CXXFLAGS) DynamicAllocationTroubleshooter.cpp; mv DynamicAllocationTroubleshooter.o ..; cd ..;
clean:
	rm *.o *.exe