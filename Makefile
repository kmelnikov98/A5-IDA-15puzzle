Main: Main.cpp PuzzleSearch.cpp Node.cpp PatternDatabase.cpp
	g++ -o Main Main.cpp PuzzleSearch.cpp PatternDatabase.cpp Node.cpp -g -Wall -Wextra -O3 -std=c++11

clean: 
	rm -rf *.o Main