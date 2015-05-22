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

test: test.cpp chess.cpp chess.hpp
	g++ -o test test.cpp chess.cpp

pgn.tab.c: pgn.y
	bison -d pgn.y

import-tables: parm-sql games.sql.part1 twic.sql.part1 cache.sql openings.sql
	echo "Импорт баз данных партий в mysql"
	cat games.sql.part* twic.sql.part*  cache.sql openings.sql| mysql -h`head parm-sql -n 1` -u`head parm-sql -n 2 | tail -n 1` -p`head parm-sql -n 3 | head -n 1` -A `head parm-sql -n 4 | tail -n 1`
	echo "Создание сводной таблицы"
	echo "drop table pgn if exists; create table pgn (content text, whiteelo int(4), blackelo int(4), result varchar(7), primary key game(content(64),whiteelo,blackelo,result)) select content,whiteelo,blackelo,result from twic where content<>'' and result in ('1-0','0-1','1/2-1/2') union distinct select content,whiteelo,blackelo,result from games where content<>'' and result in ('1-0','0-1','1/2-1/2')" | mysql -h`head parm-sql -n 1` -u`head parm-sql -n 2 | tail -n 1` -p`head parm-sql -n 3 | head -n 1` -A `head parm-sql -n 4 | tail -n 1`

export-tables: games.sql.part1 twic.sql.part1 cache.sql openings.sql

games.sql.part1: split parm-sql
	mysqldump -h`head parm-sql -n 1` -u`head parm-sql -n 2 | tail -n 1` -p`head parm-sql -n 3 | head -n 1` `head parm-sql -n 4 | tail -n 1` games | ./split -o games.sql

twic.sql.part1:
	mysqldump -h`head parm-sql -n 1` -u`head parm-sql -n 2 | tail -n 1` -p`head parm-sql -n 3 | head -n 1` `head parm-sql -n 4 | tail -n 1` twic | ./split -o twic.sql

cache.sql:
	mysqldump -h`head parm-sql -n 1` -u`head parm-sql -n 2 | tail -n 1` -p`head parm-sql -n 3 | head -n 1` `head parm-sql -n 4 | tail -n 1` cache > cache.sql

openings.sql:
	mysqldump -h`head parm-sql -n 1` -u`head parm-sql -n 2 | tail -n 1` -p`head parm-sql -n 3 | head -n 1` `head parm-sql -n 4 | tail -n 1` openings > openings.sql

split: split.o
	g++ -o split split.o

split.o: split.cpp
	g++ split.cpp

parm-sql:
	dialog --form "Параметры сервера mysql" 15 30 10 "Хост:" 2 1 localhost 2 8 16 15 "User:" 4 1 "chessplayer" 4 8 16 15 "пароль" 6 1 " " 6 8 16 15 "dbname:" 8 1 "chess" 8 8 16 15 2> parm-sql
	clear

install: parm-sql import-tables openings.php seekgame.php
	mkdir /var/www/chess
	cp openings.php seekgame.php /var/www/chess
	echo "<? \$sqlserver='`head parm-sql -n 1`'; \$sqluser='`head parm-sql -n 2|tail -n 1`'; \$sqlpassword='`head parm-sql -n 3|tail -n 1`'; \$dbname='`head parm-sql -n 4|tail -n 1`'; ?>" > /var/www/chess/sqlconnect.php
	chmod a+r /var/www/chess /var/www/chess/*
