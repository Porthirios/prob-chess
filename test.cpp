#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "chess.hpp"

main(int argc, char** argv) {
  int depth=1;
  for(int i=1; i<argc; i++)
    if(!strcmp(argv[i],"-d")) depth=atoi(argv[++i]);
  tree p;
  static char name[]=" NBRQK";
  for(;;) {
    std::cout << "   a b c d e f g h\n";
    for(int i=0; i<8; i++) {
      std::cout << 8-i << ' ';
      for(int j=0; j<8; j++) {
        Figure f=p.get(field(j,7-i));
        std::cout << "\033[";
        if(f&White) std::cout << '2';
        else if(f&Black) std::cout << '1';
        else std::cout << '8';
        std::cout << ';' << ((i+j)&1?40:39) << "m ";
        switch(f&7) {
          case Empty:
            std::cout << '.'; break;
          case Pown:
            std::cout << 'p'; break;
          default:
            std::cout << name[(f&7)-1];
        }
        std::cout  << "\033[0m";
      }
      std::cout << ' ' << 8-i << '\n';
    }
    std::cout << "   a b c d e f g h\n";
    if(!p.verify()) return 1;
    try {
      p.make(depth);
    } catch(move m) {
      std::cout << "\nIllegal move" << name[m.f-1] << char('a'+m.from.v) << int(m.from.h+1) << '-' << char('a'+m.to.v) << int(m.to.h+1) << '\n';
      return 1;
    }
    for(int i=0; i<p.size(); i++)
      std::cout << i+1 << '.' << name[p[i].m.f-1] << char('a'+p[i].m.from.v) << int(p[i].m.from.h+1) << '-' << char('a'+p[i].m.to.v) << int(p[i].m.to.h+1) << '{' << p[i].l << '-' << p[i].w << '}' << '\t';
    if(!p.size()) break;
    int n;
    std::cout << "\n: "; std::cout.flush();
    std::cin >> n;
    if(std::cin.eof()) break;
    p.choose(n-1);
  }
  return 0;
}
