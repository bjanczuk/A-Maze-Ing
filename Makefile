CMP = g++ -std=c++11 -Wall
CLASS1 = cell
CLASS2 = maze
MAIN = main
EXEC = runMaze

$(EXEC): $(CLASS1).o $(CLASS2).o $(MAIN).o
	$(CMP) $(CLASS1).o $(CLASS2).o	$(MAIN).o -o $(EXEC)

$(CLASS1).o: $(CLASS1).cpp $(CLASS1).h
	$(CMP) -c $(CLASS1).cpp -o $(CLASS1).o

$(CLASS2).o: $(CLASS2).cpp $(CLASS2).h
	$(CMP) -c $(CLASS2).cpp -o $(CLASS2).o

$(MAIN).o: $(MAIN).cpp $(CLASS1).o $(CLASS2).o
	$(CMP) -c $(MAIN).cpp -o $(MAIN).o

clean:
	rm *.o
	rm $(EXEC)
