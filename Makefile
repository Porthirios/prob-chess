test: test.cpp chess.cpp chess.hpp
	g++ -o test test.cpp chess.cpp

pgn.tab.c: pgn.y
	bison -d pgn.y
