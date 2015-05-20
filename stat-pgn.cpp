#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <string>
#include <iostream>

const char* fldname[]={
  "Annotator", "BlackElo", "BlackTeam", "Black", "Board", "Date", "ECO", "EventCountry", "EventDate", "EventRounds", "EventType", "Event", "PlyCount", "Result", "Round", "Site", "SourceDate", "WhiteElo", "WhiteTeam", "White"
};

static int reccnt=0;
const int qsize=128;

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
        pcnt=1; continue;
      case '[':
        lpchar='['; rpchar=']';
        pcnt=1; continue;
      case '{':
        lpchar='{'; rpchar='}';
        pcnt=1; continue;
      default:
        r.push_back(last= *i);
        break;
    }
  }
  return r;
}

void outrec(std::string (&rec)[21]) {
  if(!(reccnt%qsize)) {
    std::cout << "insert ignore into games (";
    for(int i=0; i<20; i++)
      std::cout << (i==5?"Game":"") << fldname[i] << ',';
    std::cout << "content) values ";
  } else std::cout << ',';
  rec[20]=strip(rec[20]);
  for(int i=0; i<21; i++) {
    std::cout << (i?',':'(') << '"' << rec[i] << '"';
    rec[i]="";
  }
  std::cout << ')';
  if(!(++reccnt%qsize))
    std::cout << ";\n";
}

char* replquotes(char* s) {
  for(char* p=s; *p; p++)
    if(*p=='"') *p='\'';
  return s;
}

main(int argc, const char** argv) {
  std::string rec[21];
  std::cout << "set names utf8;\n";
  for(int i=1; i<argc; i++) {
    FILE* file=fopen(argv[i],"r");
    if(!file) {
      std::cerr << "Не могу открыть файл " << argv[i] << '\n';
      continue;
    }
    static size_t size=0;
    static char* line=NULL;
    bool isparm=true;
    for(int l; (l=getline(&line,&size,file))>0;) {
      if(line[l-1]=='\n') line[--l]=' ';
      if(line[l-1]=='\r') line[--l]=' ';
      if(*line=='[') {
        int n;
        for(n=0; n<20; n++)
          if(!strncasecmp(line+1,fldname[n],strlen(fldname[n]))) break;
        if(n==20) goto content;
        char* p=strrchr(line,']');
        if(!p) goto content;
        *p--=0;
        int b=1+strlen(fldname[n]);
        if(!isspace(line[b])) {
          line[strlen(line)]=']';
          goto content;
        }
        line[b++]=0;
        while(isspace(line[b])) b++;
        if(line[b]=='"')
          *strrchr(line+b++,'"')=0;
        if(!isparm) {
          outrec(rec);
          isparm=true;
        }
        rec[n]=replquotes(line+b);
        continue;
      }
      content:
      isparm=false;
      rec[20]+=replquotes(line);
    }
    if(!isparm) outrec(rec);
    fclose(file);
  }
  if(reccnt%qsize) std::cout << ";\n";
  return 0;
}
