CMP = g++ -std=c++11 -Wall
CLASS1 = cell
CLASS2 = maze
LINKER_FLAGS = -lSDL2
MAIN = main
EXEC = runMaze
WINDOW = window

all:	$(EXEC) $(WINDOW)

$(EXEC): $(CLASS1).o $(CLASS2).o $(MAIN).o
	$(CMP) $(CLASS1).o $(CLASS2).o	$(MAIN).o -o $(EXEC)

$(CLASS1).o: $(CLASS1).cpp $(CLASS1).h
	$(CMP) -c $(CLASS1).cpp -o $(CLASS1).o

$(CLASS2).o: $(CLASS2).cpp $(CLASS2).h
	$(CMP) -c $(CLASS2).cpp -o $(CLASS2).o

$(MAIN).o: $(MAIN).cpp $(CLASS1).o $(CLASS2).o
	$(CMP) -c $(MAIN).cpp -o $(MAIN).o
	
$(WINDOW): $(WINDOW).cpp
	   $(CMP) $(LINKER_FLAGS) $(WINDOW).cpp -o $(WINDOW)

clean:
	rm *.o
	rm $(EXEC)
	rm $(WINDOW)
