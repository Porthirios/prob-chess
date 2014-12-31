#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <string>
#include <iostream>
#include "chess.hpp"

static std::pair<unsigned,field> first_pos[2][16]=
{
  {
    std::pair<unsigned,field>(King,field(4,0)),
    std::pair<unsigned,field>(Queen,field(3,0)),
    std::pair<unsigned,field>(Rock,field(0,0)),  std::pair<unsigned,field>(Rock,field(7,0)),
    std::pair<unsigned,field>(Bishop,field(2,0)),
    std::pair<unsigned,field>(Bishop,field(5,0)),
    std::pair<unsigned,field>(Knight,field(1,0)),
    std::pair<unsigned,field>(Knight,field(6,0)),
    std::pair<unsigned,field>(Pown,field(0,1)),
    std::pair<unsigned,field>(Pown,field(1,1)),
    std::pair<unsigned,field>(Pown,field(2,1)),
    std::pair<unsigned,field>(Pown,field(3,1)),
    std::pair<unsigned,field>(Pown,field(4,1)),
    std::pair<unsigned,field>(Pown,field(5,1)),
    std::pair<unsigned,field>(Pown,field(6,1)),
    std::pair<unsigned,field>(Pown,field(7,1)),
  },
  {
    std::pair<unsigned,field>(King,field(4,7)),
    std::pair<unsigned,field>(Queen,field(3,7)),
    std::pair<unsigned,field>(Rock,field(0,7)),
    std::pair<unsigned,field>(Rock,field(7,7)),
    std::pair<unsigned,field>(Bishop,field(2,7)),
    std::pair<unsigned,field>(Bishop,field(5,7)),
    std::pair<unsigned,field>(Knight,field(1,7)),
    std::pair<unsigned,field>(Knight,field(6,7)),
    std::pair<unsigned,field>(Pown,field(0,6)),
    std::pair<unsigned,field>(Pown,field(1,6)),
    std::pair<unsigned,field>(Pown,field(2,6)),
    std::pair<unsigned,field>(Pown,field(3,6)),
    std::pair<unsigned,field>(Pown,field(4,6)),
    std::pair<unsigned,field>(Pown,field(5,6)),
    std::pair<unsigned,field>(Pown,field(6,6)),
    std::pair<unsigned,field>(Pown,field(7,6)),
  }
};

position::position()
: enable0_0(15), last_pmove(-2), pl(White) {
  bzero(wh,sizeof(wh));
  bzero(bl,sizeof(bl));
  bzero(desk,sizeof(desk));
  for(int i=0; i<16; i++) {
    set(Figure(first_pos[0][i].first|White), first_pos[0][i].second);
    set(Figure(first_pos[1][i].first|Black), first_pos[1][i].second);
  }
}

bool position::go(move m) {
  Figure f=get(m.from), g=get(m.to);
  switch(pl) {
    case White:// ход белых
      if(!(f&White) || (g&White)) return false;
      for(int i=0; i<16; i++)
	if(wh[i].first==(f^White) && wh[i].second==m.from) {
          // взятие
	  if(g) for(int j=1; j<16; j++)
	    if(bl[j].first!=Empty && bl[j].second==m.to) {
	      bl[j].first=Empty;
              break;
            }
	  wh[i].second=m.to;
	  desk[m.from.v][m.from.h]=Empty;
	  desk[m.to.v][m.to.h]=f;
	  switch(f) {
	    case Pown|White:
              // взятие на проходе
	      if(last_pmove==m.to.v && m.to.h==5) {
		desk[m.to.v][4]=Empty;
		bl[8+m.to.v].first=Empty;
	      } else
	      if(m.from.h==1 && m.to.h==3)
		last_pmove=m.from.v;
	      else {
		last_pmove=-2;
                // превращение
                if(m.to.h==7) {
                  desk[m.to.v][7]=m.f|White;
                  wh[i].first=m.f;
                }
	      }
	      break;
	    case King|White:
              if(m.from.v==4)
                // короткая рокировка
                if(m.to.v==6) {
                  desk[5][0]=Rock|White;
                  desk[7][0]=Empty;
                  wh[3].second.v=5;
                } else
                // длинная рокировка
                if(m.to.v==2) {
                  desk[3][0]=Rock|White;
                  desk[0][0]=Empty;
                  wh[2].second.v=3;
                }
              enable0_0 &= ~3;
              last_pmove=-2;
              break;
            case Rock|White:
              if(i==2) enable0_0 &= ~2;
              else if(i==3) enable0_0 &=~1;
	    default:
              last_pmove=-2;
              break;
	  }
	  break;
	}
      pl=Black;
      break;
    case Black: // ход чёрных
      if(!(f&Black) || (g&Black)) return false;
      for(int i=0; i<16; i++)
        if(bl[i].first==(f^Black) && bl[i].second==m.from) {
          if(g) for(int j=1; j<16; j++)
            if(wh[j].first!=Empty && wh[j].second==m.to) {
              wh[j].first=Empty;
              break;
            }
          bl[i].second=m.to;
          desk[m.from.v][m.from.h]=Empty;
          desk[m.to.v][m.to.h]=f;
          switch(f) {
            case Pown|Black:
              if(last_pmove==m.to.v && m.to.h==2) {
                desk[m.to.v][4]=Empty;
                wh[8+m.to.v].first=Empty;
              } else
              if(m.from.h==6 && m.to.h==4)
                last_pmove=m.from.v;
              else {
                last_pmove=-2;
                if(m.to.h==0) {
                  desk[m.to.v][0]=m.f|Black;
                  bl[i].first=m.f;
                }
              }
              break;
            case King|Black:
              if(m.from.v==4)
                if(m.to.v==6) {
                  desk[5][7]=Rock|Black;
                  desk[7][7]=Empty;
                  bl[3].second.v=5;
                } else
                if(m.to.v==2) {
                  desk[3][7]=Rock|Black;
                  desk[0][7]=Empty;
                  bl[2].second.v=3;
                }
              enable0_0 &= ~12;
              last_pmove=-2;
              break;
            case Rock|Black:
              if(i==2) enable0_0 &= ~8;
              else if(i==3) enable0_0 &=~4;
            default:
              last_pmove=-2;
              break;
          }
        }
      pl=White;
      break;
    default:
      return false;
  }
  return true;
}

int position::lstmoves(std::vector<std::pair<move, position> >& lst) const {
  switch(pl) {
    case White:
      for(int i=0; i<16; i++) {
        field p=wh[i].second;
        switch(wh[i].first) {
          case King:
            if(p.v) {
              if(!(desk[p.v-1][p.h]&White))
                addmove(King,p,field(p.v-1,p.h),lst);
              if(p.h && !(desk[p.v-1][p.h-1]&White))
                addmove(King,p,field(p.v-1,p.h-1),lst);
              if(p.h<7 && !(desk[p.v-1][p.h+1]&White))
                addmove(King,p,field(p.v-1,p.h+1),lst);
            }
            if(p.h && !(desk[p.v][p.h-1]&White))
              addmove(King,p,field(p.v,p.h-1),lst);
            if(p.h<7 && !(desk[p.v][p.h+1]&White))
              addmove(King,p,field(p.v,p.h+1),lst);
            if(p.v<7) {
              if(!(desk[p.v+1][p.h]&White))
                addmove(King,p,field(p.v+1,p.h),lst);
              if(p.h && !(desk[p.v+1][p.h-1]&White))
                addmove(King,p,field(p.v+1,p.h-1),lst);
              if(p.h<7 && !(desk[p.v+1][p.h+1]&White))
                addmove(King,p,field(p.v+1,p.h+1),lst);
            }
            break;
          case Queen:
            for(field g(p); g.v-- && g.h--; ) {
              if(!(desk[g.v][g.h]&White))
                addmove(Queen,p,g,lst);
              if(desk[g.v][g.h]) break;
            }
            for(field g(p); g.v-- && g.h++<7; ) {
              if(!(desk[g.v][g.h]&White))
                addmove(Queen,p,g,lst);
              if(desk[g.v][g.h]) break;
            }
            for(field g(p); g.v++<7 && g.h--; ) {
              if(!(desk[g.v][g.h]&White))
                addmove(Queen,p,g,lst);
              if(desk[g.v][g.h]) break;
            }
            for(field g(p); g.v++<7 && g.h++<7; ) {
              if(!(desk[g.v][g.h]&White))
                addmove(Queen,p,g,lst);
              if(desk[g.v][g.h]) break;
            }
          case Rock:
            for(field g(p); g.v--; ) {
              if(!(desk[g.v][g.h]&White))
                addmove(Figure(wh[i].first),p,g,lst);
              if(desk[g.v][g.h]) break;
            }
            for(field g(p); g.h--; ) {
              if(!(desk[g.v][g.h]&White))
                addmove(Figure(wh[i].first),p,g,lst);
              if(desk[g.v][g.h]) break;
            }
            for(field g(p); g.v++<7; ) {
              if(!(desk[g.v][g.h]&White))
                addmove(Figure(wh[i].first),p,g,lst);
              if(desk[g.v][g.h]) break;
            }
            for(field g(p); g.h++<7; ) {
              if(!(desk[g.v][g.h]&White))
                addmove(Figure(wh[i].first),p,g,lst);
              if(desk[g.v][g.h]) break;
            }
            break;
          case Bishop:
            for(field g(p); g.v-- && g.h--; ) {
              if(!(desk[g.v][g.h]&White))
                addmove(Bishop,p,g,lst);
              if(desk[g.v][g.h]) break;
            }
            for(field g(p); g.v-- && g.h++<7; ) {
              if(!(desk[g.v][g.h]&White))
                addmove(Bishop,p,g,lst);
              if(desk[g.v][g.h]) break;
            }
            for(field g(p); g.v++<7 && g.h--; ) {
              if(!(desk[g.v][g.h]&White))
                addmove(Bishop,p,g,lst);
              if(desk[g.v][g.h]) break;
            }
            for(field g(p); g.v++<7 && g.h++<7; ) {
              if(!(desk[g.v][g.h]&White))
                addmove(Bishop,p,g,lst);
              if(desk[g.v][g.h]) break;
            }
            break;
          case Knight:
            if(p.v>1 && p.h && !(desk[p.v-2][p.h-1]&White))
              addmove(Knight,p,field(p.v-2,p.h-1),lst);
            if(p.v && p.h>1 && !(desk[p.v-1][p.h-2]&White))
              addmove(Knight,p,field(p.v-1,p.h-2),lst);
            if(p.v>1 && p.h<7 && !(desk[p.v-2][p.h+1]&White))
              addmove(Knight,p,field(p.v-2,p.h+1),lst);
            if(p.v && p.h<6 && !(desk[p.v-1][p.h+2]&White))
              addmove(Knight,p,field(p.v-1,p.h+2),lst);
            if(p.v<6 && p.h && !(desk[p.v+2][p.h-1]&White))
              addmove(Knight,p,field(p.v+2,p.h-1),lst);
            if(p.v<6 && p.h<7 && !(desk[p.v+2][p.h+1]&White))
              addmove(Knight,p,field(p.v+2,p.h+1),lst);
            if(p.v<7 && p.h>1 && !(desk[p.v+1][p.h-2]&White))
              addmove(Knight,p,field(p.v+1,p.h-2),lst);
            if(p.v<7 && p.h<6 && !(desk[p.v+1][p.h+2]&White))
              addmove(Knight,p,field(p.v+1,p.h+2),lst);
            break;
          case Pown:
            if(desk[p.v][p.h+1]==Empty)
              if(p.h<6) {
                addmove(Pown,p,field(p.v,p.h+1),lst);
                if(p.h==1 && desk[p.v][3]==Empty)
                  addmove(Pown,p,field(p.v,3),lst);
              }
              else {
                addmove(Queen,p,field(p.v,7),lst);
                addmove(Rock,p,field(p.v,7),lst);
                addmove(Bishop,p,field(p.v,7),lst);
                addmove(Knight,p,field(p.v,7),lst);
              }
            if(p.v && desk[p.v-1][p.h+1]&Black)
              if(p.h<6)
                addmove(Pown,p,field(p.v-1,p.h+1),lst);
              else {
                addmove(Queen,p,field(p.v-1,7),lst);
                addmove(Rock,p,field(p.v-1,7),lst);
                addmove(Bishop,p,field(p.v-1,7),lst);
                addmove(Knight,p,field(p.v-1,7),lst);
              }
            if(p.v<7 && desk[p.v+1][p.h+1]&Black)
              if(p.h<6)
                addmove(Pown,p,field(p.v+1,p.h+1),lst);
              else {
                addmove(Queen,p,field(p.v+1,7),lst);
                addmove(Rock,p,field(p.v+1,7),lst);
                addmove(Bishop,p,field(p.v+1,7),lst);
                addmove(Knight,p,field(p.v+1,7),lst);
              }
            if(p.h==4 && (last_pmove==p.v-1 || last_pmove==p.v+1))
              addmove(Pown,p,field(last_pmove,5),lst);
            break;
        }
      }
      if(enable0_0&3 && !check(wh[0].second,Black)) {
        if(enable0_0&1 && desk[5][0]==Empty && desk[6][0]==Empty && !check(field(5,0),Black))
          addmove(King,wh[0].second,field(6,0),lst);
        if(enable0_0&2 && desk[3][0]==Empty && desk[2][0]==Empty && desk[1][0]==Empty && !check(field(3,0),Black))
          addmove(King,wh[0].second,field(2,0),lst);
      }
      break;
    case Black:
      for(int i=0; i<16; i++) {
        field p=bl[i].second;
        switch(bl[i].first) {
          case King:
            if(p.v) {
              if(!(desk[p.v-1][p.h]&Black))
                addmove(King,p,field(p.v-1,p.h),lst);
              if(p.h && !(desk[p.v-1][p.h-1]&Black))
                addmove(King,p,field(p.v-1,p.h-1),lst);
              if(p.h<7 && !(desk[p.v-1][p.h+1]&Black))
                addmove(King,p,field(p.v-1,p.h+1),lst);
            }
            if(p.h && !(desk[p.v][p.h-1]&Black))
              addmove(King,p,field(p.v,p.h-1),lst);
            if(p.h<7 && !(desk[p.v][p.h+1]&Black))
              addmove(King,p,field(p.v,p.h+1),lst);
            if(p.v<7) {
              if(!(desk[p.v+1][p.h]&Black))
                addmove(King,p,field(p.v+1,p.h),lst);
              if(p.h && !(desk[p.v+1][p.h-1]&Black))
                addmove(King,p,field(p.v+1,p.h-1),lst);
              if(p.h<7 && !(desk[p.v+1][p.h+1]&Black))
                addmove(King,p,field(p.v+1,p.h+1),lst);
            }
            break;
          case Queen:
            for(field g(p); g.v-- && g.h--; ) {
              if(!(desk[g.v][g.h]&Black))
                addmove(Queen,p,g,lst);
              if(desk[g.v][g.h]) break;
            }
            for(field g(p); g.v-- && g.h++<7; ) {
              if(!(desk[g.v][g.h]&Black))
                addmove(Queen,p,g,lst);
              if(desk[g.v][g.h]) break;
            }
            for(field g(p); g.v++<7 && g.h--; ) {
              if(!(desk[g.v][g.h]&Black))
                addmove(Queen,p,g,lst);
              if(desk[g.v][g.h]) break;
            }
            for(field g(p); g.v++<7 && g.h++<7; ) {
              if(!(desk[g.v][g.h]&Black))
                addmove(Queen,p,g,lst);
              if(desk[g.v][g.h]) break;
            }
          case Rock:
            for(field g(p); g.v--; ) {
              if(!(desk[g.v][g.h]&Black))
                addmove(Figure(bl[i].first),p,g,lst);
              if(desk[g.v][g.h]) break;
            }
            for(field g(p); g.h--; ) {
              if(!(desk[g.v][g.h]&Black))
                addmove(Figure(bl[i].first),p,g,lst);
              if(desk[g.v][g.h]) break;
            }
            for(field g(p); g.v++<7; ) {
              if(!(desk[g.v][g.h]&Black))
                addmove(Figure(bl[i].first),p,g,lst);
              if(desk[g.v][g.h]) break;
            }
            for(field g(p); g.h++<7; ) {
              if(!(desk[g.v][g.h]&Black))
                addmove(Figure(bl[i].first),p,g,lst);
              if(desk[g.v][g.h]) break;
            }
            break;
          case Bishop:
            for(field g(p); g.v-- && g.h--; ) {
              if(!(desk[g.v][g.h]&Black))
                addmove(Bishop,p,g,lst);
              if(desk[g.v][g.h]) break;
            }
            for(field g(p); g.v-- && g.h++<7; ) {
              if(!(desk[g.v][g.h]&Black))
                addmove(Bishop,p,g,lst);
              if(desk[g.v][g.h]) break;
            }
            for(field g(p); g.v++<7 && g.h--; ) {
              if(!(desk[g.v][g.h]&Black))
                addmove(Bishop,p,g,lst);
              if(desk[g.v][g.h]) break;
            }
            for(field g(p); g.v++<7 && g.h++<7; ) {
              if(!(desk[g.v][g.h]&Black))
                addmove(Bishop,p,g,lst);
              if(desk[g.v][g.h]) break;
            }
            break;
          case Knight:
            if(p.v>1 && p.h && !(desk[p.v-2][p.h-1]&Black))
              addmove(Knight,p,field(p.v-2,p.h-1),lst);
            if(p.v && p.h>1 && !(desk[p.v-1][p.h-2]&Black))
              addmove(Knight,p,field(p.v-1,p.h-2),lst);
            if(p.v>1 && p.h<7 && !(desk[p.v-2][p.h+1]&Black))
              addmove(Knight,p,field(p.v-2,p.h+1),lst);
            if(p.v && p.h<6 && !(desk[p.v-1][p.h+2]&Black))
              addmove(Knight,p,field(p.v-1,p.h+2),lst);
            if(p.v<6 && p.h && !(desk[p.v+2][p.h-1]&Black))
              addmove(Knight,p,field(p.v+2,p.h-1),lst);
            if(p.v<6 && p.h<7 && !(desk[p.v+2][p.h+1]&Black))
              addmove(Knight,p,field(p.v+2,p.h+1),lst);
            if(p.v<7 && p.h>1 && !(desk[p.v+1][p.h-2]&Black))
              addmove(Knight,p,field(p.v+1,p.h-2),lst);
            if(p.v<7 && p.h<6 && !(desk[p.v+1][p.h+2]&Black))
              addmove(Knight,p,field(p.v+1,p.h+2),lst);
            break;
          case Pown:
            if(desk[p.v][p.h-1]==Empty)
              if(p.h>1) {
                addmove(Pown,p,field(p.v,p.h-1),lst);
                if(p.h==6 && desk[p.v][4]==Empty)
                  addmove(Pown,p,field(p.v,4),lst);
              }
              else {
                addmove(Queen,p,field(p.v,0),lst);
                addmove(Rock,p,field(p.v,0),lst);
                addmove(Bishop,p,field(p.v,0),lst);
                addmove(Knight,p,field(p.v,0),lst);
              }
            if(p.v && desk[p.v-1][p.h-1]&White)
              if(p.h>1)
                addmove(Pown,p,field(p.v-1,p.h-1),lst);
              else {
                addmove(Queen,p,field(p.v-1,0),lst);
                addmove(Rock,p,field(p.v-1,0),lst);
                addmove(Bishop,p,field(p.v-1,0),lst);
                addmove(Knight,p,field(p.v-1,0),lst);
              }
            if(p.v<7 && desk[p.v+1][p.h-1]&White)
              if(p.h>1)
                addmove(Pown,p,field(p.v+1,p.h-1),lst);
              else {
                addmove(Queen,p,field(p.v+1,0),lst);
                addmove(Rock,p,field(p.v+1,0),lst);
                addmove(Bishop,p,field(p.v+1,0),lst);
                addmove(Knight,p,field(p.v+1,0),lst);
              }
            if(p.h==3 && (last_pmove==p.v-1 || last_pmove==p.v+1))
              addmove(Pown,p,field(last_pmove,2),lst);
            break;
        }
      }
      if(enable0_0&12 && !check(bl[0].second,White)) {
        if(enable0_0&4 && desk[5][7]==Empty && desk[6][7]==Empty && !check(field(5,7),White))
          addmove(King,bl[0].second,field(6,7),lst);
        if(enable0_0&8 && desk[3][7]==Empty && desk[2][7]==Empty && desk[1][7]==Empty && !check(field(3,7),White))
          addmove(King,bl[0].second,field(2,7),lst);
      }
      break;
  }
}

void position::value(float& w, float& l) const {
  w=l=1./3;
}

bool position::check(field f, Figure p) const {
  switch(p) {
    case White:
      for(int i=0; i<16; i++) {
        field g=wh[i].second;
        if(g!=f)
        switch(wh[i].first) {
          case King:
            if(g.v>=f.v-1 && g.v<=f.v+1 && g.h>=f.h-1 && g.h<=f.h+1) return true;
            break;
          case Queen: {
            int sv,sh;
            if(g.v==f.v) {
              sv=0;
              sh=(g.h>f.h?-1:1);
            } else if(g.h==f.h) {
              sv=(g.v>f.v?-1:1);
              sh=0;
            } else if(g.v-f.v==g.h-f.h)
              sv=sh=(g.v>f.v?-1:1);
            else if(g.v-f.v==f.h-g.h)
              sh=-(sv=(g.v>f.v?-1:1));
            else break;
            for(g.v+=sv,g.h+=sh; g!=f; g.v+=sv,g.h+=sh)
              if(desk[g.v][g.h]) goto nextw;
            return true;
          }
          case Rock: {
            int sv,sh;
            if(g.v==f.v) {
              sv=0;
              sh=(g.h>f.h?-1:1);
            } else if(g.h==f.h) {
              sv=(g.v>f.v?-1:1);
              sh=0;
            } else break;
            for(g.v+=sv,g.h+=sh; g!=f; g.v+=sv,g.h+=sh)
              if(desk[g.v][g.h]) goto nextw;
            return true;
          }
          case Bishop: {
            int sv,sh;
            if(g.v-f.v==g.h-f.h)
              sv=sh=(g.v>f.v?-1:1);
            else if(g.v-f.v==f.h-g.h)
              sh=-(sv=(g.v>f.v?-1:1));
            else break;
            for(g.v+=sv,g.h+=sh; g!=f; g.v+=sv,g.h+=sh)
              if(desk[g.v][g.h]) goto nextw;
            return true;
          }
          case Knight: {
            int dv=abs(g.v-f.v), dh=abs(g.h-f.h);
            if(dv==1 && dh==2 || dv==2 && dh==1)
              return true;
            break;
          }
          case Pown:
            if(g.h+1==f.h && abs(g.v-f.v)==1)
              return true;
            break;
        }
        nextw:;
      }
      break;
    case Black:
      for(int i=0; i<16; i++) {
        field g=bl[i].second;
        if(g!=f)
        switch(bl[i].first) {
          case King:
            if(g.v>=f.v-1 && g.v<=f.v+1 && g.h>=f.h-1 && g.h<=f.h+1) return true;
            break;
          case Queen: {
            int sv,sh;
            if(g.v==f.v) {
              sv=0;
              sh=(g.h>f.h?-1:1);
            } else if(g.h==f.h) {
              sv=(g.v>f.v?-1:1);
              sh=0;
            } else if(g.v-f.v==g.h-f.h)
              sv=sh=(g.v>f.v?-1:1);
            else if(g.v-f.v==f.h-g.h)
              sh=-(sv=(g.v>f.v?-1:1));
            else break;
            for(g.v+=sv,g.h+=sh; g!=f; g.v+=sv,g.h+=sh)
              if(desk[g.v][g.h]) goto nextb;
            return true;
          }
          case Rock: {
            int sv,sh;
            if(g.v==f.v) {
              sv=0;
              sh=(g.h>f.h?-1:1);
            } else if(g.h==f.h) {
              sv=(g.v>f.v?-1:1);
              sh=0;
            } else break;
            for(g.v+=sv,g.h+=sh; g!=f; g.v+=sv,g.h+=sh)
              if(desk[g.v][g.h]) goto nextb;
            return true;
          }
          case Bishop: {
            int sv,sh;
            if(g.v-f.v==g.h-f.h)
              sv=sh=(g.v>f.v?-1:1);
            else if(g.v-f.v==f.h-g.h)
              sh=-(sv=(g.v>f.v?-1:1));
            else break;
            for(g.v+=sv,g.h+=sh; g!=f; g.v+=sv,g.h+=sh)
              if(desk[g.v][g.h]) goto nextb;
            return true;
          }
          case Knight: {
            int dv=abs(g.v-f.v), dh=abs(g.h-f.h);
            if(dv==1 && dh==2 || dv==2 && dh==1)
              return true;
            break;
          }
          case Pown:
            if(g.h-1==f.h && abs(g.v-f.v)==1)
              return true;
            break;
        }
        nextb:;
      }
      break;
  }
  return false;
}

void position::set(Figure F, field f) {
  desk[f.v][f.h]=F;
  if(F&White) {
    for(int i=0; i<16; i++)
      if(!wh[i].first) {
	wh[i]=std::pair<unsigned char,field>(F^White,f);
	break;
      }
  } else if(F&Black)
    for(int i=0; i<16; i++)
      if(!bl[i].first) {
	bl[i]=std::pair<unsigned char,field>(F^Black,f);
	break;
      }
}

bool position::verify() {
  static std::string name[]={"Pown","Knight","Bishop","Rock","Queen","King"};
  int bc=0,wc=0;
  for(int i=0; i<16; i++) {
    if(wh[i].first && ++wc && get(wh[i].second)!=(wh[i].first|White)) {
      std::cerr << "On " << char('a'+wh[i].second.v) << int(wh[i].second.h+1) << "must be white" << name[wh[i].first-1] << '\n';
      return false;
    }
    if(bl[i].first && ++bc && get(bl[i].second)!=(bl[i].first|Black)) {
      std::cerr << "On " << char('a'+bl[i].second.v) << int(bl[i].second.h+1) << "must be black" << name[bl[i].first-1] << '\n';
      return false;
    }
  }
  for(int i=0; i<8; i++)
    for(int j=0; j<8; j++) {
      if(desk[i][j]&White&Black) {
        std::cerr << "Both Black and White on " << char('a'+i) << j+1 << '\n';
        return false;
      }
      if(desk[i][j]&White) --wc;
      if(desk[i][j]&Black) --bc;
    }
  if(wc) {
    std::cerr << "Double white\n";
    return false;
  }
  if(bc) {
    std::cerr << "Double black\n";
    return false;
  }
  return true;
}

tree::~tree() {
  for(int i=0; i<lst.size(); i++)
    delete lst[i].next;
}

void tree::make(int d) {
  if(lst.size()) {
    if(d) {
      for(int i=0; i<lst.size(); i++)
        if(!lst[i].next) {
          lst[i].next=new tree((position&)(*this));
          lst[i].next->go(lst[i].m);
          lst[i].next->make(d-1);
          if(lst[i].next->size()) {
            lst[i].w=lst[i].l=1;
            for(int j=0; j<lst[i].next->size(); j++) {
              lst[i].l *= lst[i].next->lst[j].w;
              lst[i].w *= 1-lst[i].next->lst[j].l;
            }
            lst[i].w=1-lst[i].w;
          } else {
            lst[i].w=0;
            lst[i].l=lst[i].next->check()?1:0;
          }
        }
      ordering();
    }
    return;
  }
  std::vector<std::pair<move,position> > lm;
  lstmoves(lm);
  for(int i=0; i<lm.size(); i++) {
    mvalue m;
    m.m=lm[i].first;
    if(d) {
      m.next=new tree(lm[i].second);
      m.next->make(d-1);
      if(m.next->lst.size()) {
        m.w=m.l=1;
        for(int j=0; j<m.next->lst.size(); j++) {
          m.l *= m.next->lst[j].w;
          m.w *= 1-m.next->lst[j].l;
        }
        m.w = 1-m.w;
      } else {
        m.w=0;
        m.l=lm[i].second.check()?1:0;
      }
    } else {
      m.next=NULL;
      lm[i].second.value(m.w,m.l);
    }
    lst.push_back(m);
  }
  ordering();
}

bool win_ordering(mvalue* a, mvalue* b) {
  return a->w > b->w;
}

bool lose_ordering(mvalue* a, mvalue* b) {
  return a->l < b->l;
}

static mvalue* tree_lst_ptr;
static int* tree_ordering_index;

int mvalue_ordering(const void* a, const void* b) {
  int ai=tree_ordering_index[(const mvalue*)a-tree_lst_ptr], bi=tree_ordering_index[(const mvalue*)b-tree_lst_ptr];
  if(ai != bi) return ai < bi;
  return ((const mvalue*)a)->w-((const mvalue*)a)->l<((const mvalue*)b)->w-((const mvalue*)b)->l;
};

void tree::ordering() {
  if(lst.size()==0) return;
  int inx[lst.size()];
  {
    mvalue* p[lst.size()];
    for(int i=0; i<lst.size(); i++)
      p[i]= &lst[i];
    std::sort(p,p+lst.size(),win_ordering);
    for(int i=0; i<lst.size(); i++) {
      inx[p[i]-&lst[0]]=i;
      p[i]= &lst[i];
    }
    std::sort(p,p+lst.size(),lose_ordering);
    for(int i=0; i<lst.size(); i++)
      inx[p[i]-&lst[0]]+=i;
  }
  tree_lst_ptr= &lst[0];
  tree_ordering_index=inx;
  qsort(&lst[0],lst.size(),sizeof(mvalue),mvalue_ordering);
}

void tree::choose(int n) {
  tree* v=lst[n].next;
  lst[n].next=NULL;
  go(lst[n].m);
  for(int i=0; i<lst.size(); i++)
    delete lst[i].next;
  lst=v->lst;
}
