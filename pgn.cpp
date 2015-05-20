#include <string>
#include <vector>
#include <ctype.h>
#include <iostream>
#include "chess.hpp"

std::string strip(std::string s) {
  std::string r;
  char last=' ', lpchar,rpchar;
  int pcnt=0;
  for(std::string::iterator i=s.begin(); i!=s.end(); ++i)
  if(pcnt) {
    if(*i==rpchar) --pcnt;
    else if(*i==lpchar) ++pcnt;
  } else {
    if(isspace(*i)) {
      if(last!='.' && last!=' ')
	r.push_back(' ');
      last=' ';
      continue;
    }
    switch(*i) {
      case '(':
	lpchar='('; rpchar=')';
	pcnt=1; break;
      case '[':
	lpchar='['; rpchar=']';
	pcnt=1; break;
      case '}':
	lpchar='{'; rpchar='}';
	pcnt=1; break;
      default:
	r.push_back(last= *i);
    }
  }
  return r;
}

enum move_attr {
  PROMOTION=1, TAKE=2, CHECK=4, MATE=8, ROCKING=16, FROM_VERTICAL=32, FROM_HORIZONTAL=64;
};

struct pgn_move {
  unsigned char fig, from, to, attr;
  pgn_move()
  : fig(0), from(0),to(0), attr(0) {}
  pgn_move(char*& src);
};

std::istream& operator<<(std::ostream&, pgn_move);

int parse_pgn(std::vector<pgn_move>& party, std::string scr);

