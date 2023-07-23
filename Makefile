CXX = g++
FLAGS = -std=c++11 
CFLAGS = -c
DEBUGFLAGS = -g 
all: pprun
debug: pprun_debug

# LINKFLAGS = -pedantic -Wall -fomit-frame-pointer -funroll-all-loops -O3
LINKFLAGS = 

pprun: main.o ppsolver.o ppmodule.o parser.o
	$(CXX) $(FLAGS) $(LINKFLAGS) $^ -o $@
		
main.o: main.cpp 
	$(CXX) $(FLAGS) $(LINKFLAGS) $(CFLAGS) -DCOMPILETIME="\"`date`\"" $^ -o $@

ppsolver.o: ppsolver.cpp ppsolver.h
	$(CXX) $(FLAGS) $(LINKFLAGS) $(CFLAGS) ppsolver.cpp -o $@
			
ppmodule.o: ppmodule.cpp ppmodule.h
	$(CXX) $(FLAGS) $(LINKFLAGS) $(CFLAGS) ppmodule.cpp -o $@

parser.o: parser.cpp parser.h
	$(CXX) $(FLAGS) $(LINKFLAGS) $(CFLAGS) parser.cpp -o $@


pprun_debug: main_debug.o ppsolver_debug.o ppmodule_debug.o
	$(CXX) $(FLAGS) $(DEBUGFLAGS) $^ -o $@
		
main_debug.o: main.cpp 
	$(CXX) $(FLAGS) $(DEBUGFLAGS) $(CFLAGS) -DCOMPILETIME="\"`date`\"" $^ -o $@

ppsolver_debug.o: ppsolver.cpp ppsolver.h
	$(CXX) $(FLAGS) $(DEBUGFLAGS) $(CFLAGS) ppsolver.cpp -o $@
			
ppmodule_debug.o: ppmodule.cpp ppmodule.h
	$(CXX) $(FLAGS) $(DEBUGFLAGS) $(CFLAGS) ppmodule.cpp -o $@

parser_debug.o: parser.cpp parser.h
	$(CXX) $(FLAGS) $(DEBUGFLAGS) $(CFLAGS) parser.cpp -o $@


clean:
	rm -rf *.o *.gch pprun pprun_debug
