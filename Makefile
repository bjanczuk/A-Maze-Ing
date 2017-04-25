CMP = g++ -std=c++11 -Wall
CLASS1 = cell
CLASS2 = maze
#CLASS3 = draw
LINKER_FLAGS = -lSDL2
MAIN = main
EXEC = runMaze

all:	$(EXEC)

$(EXEC): $(CLASS1).o $(CLASS2).o $(MAIN).o
	$(CMP) $(LINKER_FLAGS) $(CLASS1).o $(CLASS2).o $(MAIN).o -o $(EXEC)

$(CLASS1).o: $(CLASS1).cpp $(CLASS1).h
	$(CMP) -c $(CLASS1).cpp -o $(CLASS1).o

$(CLASS2).o: $(CLASS2).cpp $(CLASS2).h
	$(CMP) -c $(LINKER_FLAGS) $(CLASS2).cpp -o $(CLASS2).o

#$(CLASS3).o: $(CLASS3).cpp $(CLASS3).h
#	$(CMP) -c $(LINKER_FLAGS) $(CLASS3).cpp -o $(CLASS3).o

$(MAIN).o: $(MAIN).cpp $(CLASS1).o $(CLASS2).o
	$(CMP) -c $(LINKER_FLAGS) $(MAIN).cpp -o $(MAIN).o
	
kill:	
	killall -9 runMaze

clean:
	rm *.o
	rm $(EXEC)
