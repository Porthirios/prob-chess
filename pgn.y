%{
  #include <string>
  #include "chess.hpp"
  int yylex();
  int yyerror();

  std::list<game*> lstgames;
%}

%union {
  FIGURE fig;
  int num;
  std::string* str;
  std::map<std::string,std::string>* hdr;
  game* part;
  shortmove smove;
  field fld;
  std:pair<std::string*,std::string*> parm;
}

%token<num> NUM
%token<str> NAME
%token<str> VALUE
%token<fig> FIGURE
%token<num> VERT
%token<num> HORIS
%type<part> party
%type<hdr> header
%type<num> result
%type<parm> parameter

%%

pgn
: party { lstgames.push_back($1); }
| pgn party { lstgames.push_back($2); }
;

party
: header moves result { $$=new game($1,$2,$3); }
;

header
: parameter {
  $$=new std::map<std::string,std::string>;
  (*$$)[*$1.first]=*$1.second;
  delete $1.first;
  delete $1.second;
}
| header parameter {
  (*$$)[*$1.first]=*$1.second;
  delete $1.first;
  delete $1.second;
}
;

parameter
: '[' NAME VALUE ']' { $$.first=$2; $$.second=$3; }
;

moves
: NUM '.' move
| moves NUM '.' move
;

move
: stripmove check comment
;

stripmove
: FIGURE field
| field
| FIGURE 'x' field
| VERT 'x' field
| FIGURE VERT field
| FIGURE HORIS field
| FIGURE VERT 'x' field
| FIGURE HORIS 'x' field
| field FIGURE
| VERT 'x' field FIGURE
;

field
: VERT HORIS
;

check
:
| '+'
;

comment
:
| '{' parameter '}'
;

result
: "1-0"
| "0-1"
| "1/2-1/2"
;

%%
