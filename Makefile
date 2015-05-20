test: chess.o test.o
	g++ chess.o test.o -o test

test-pgn: test-pgn.o chess.o pgn.tab.o
	g++ -o test-pgn test-pgn.o chess.o pgn.tab.o

test.o: test.cpp chess.hpp
	g++ -c test.cpp

test-pgn.o: test-pgn.cpp chess.hpp
	g++ -c test-pgn.cpp

chess.o: chess.cpp chess.hpp
	g++ -c chess.cpp

pgn.tab.o: pgn.tab.c
	g++ -c pgn.tab.c

pgn.tab.c: pgn.y
	bison -d pgn.y

stat-pgn: stat-pgn.cpp
	g++ stat-pgn.cpp -o stat-pgn
