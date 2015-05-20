#include <vector>
<<<<<<< HEAD
#include <map>
=======
>>>>>>> 271a2dc582392dc39009dd86c84afbcb11e54ba5

struct field {
  unsigned char v:4, h:4;
  field() {}
  field(int _v, int _h)
  : v(_v), h(_h) {}
  bool operator==(field f) {
    return v==f.v && h==f.h;
  }
  bool operator!=(field f) {
    return v!=f.v || h!=f.h;
  }
};

enum Figure { Empty=0, King=6, Queen=5, Rock=4, Bishop=3, Knight=2, Pown=1, White=8, Black=16 };

struct move {
  field from, to;
  Figure f;
};

class position {
  std::pair<unsigned char,field> wh[16],bl[16];
  unsigned char desk[8][8], enable0_0;
  int last_pmove;
  Figure pl;
  void addmove(Figure F, field from, field to, std::vector<std::pair<move,position> >& lst) const {
    move m={from,to,F};
    position p(*this);
    if(!p.go(m)) throw m;
    if(!p.check((pl==White?p.wh[0].second:p.bl[0].second),p.pl))
      lst.push_back(std::pair<move,position>(m,p));
  }
public:
  position();
  bool go(move);
  int lstmoves(std::vector<std::pair<move,position> >&) const;
  void value(float& w, float& l) const;
  bool check(field f, Figure p) const;
  void set(Figure, field);
  Figure get(field f) const {
    return Figure(desk[f.v][f.h]);
  }
  bool check() const {
    return check(pl==White?wh[0].second:bl[0].second, Figure(pl^(Black|White)));
  }
  bool verify();
};

struct mvalue {
  move m;
  float w,l;
  class tree* next;
};

class tree: public position {
  std::vector<mvalue> lst;
public:
  tree() {}
  tree(const position& p)
  : position(p) { }
  ~tree();
  void make(int depth);
  mvalue& operator[] (int n) { return lst[n]; }
  size_t size() const { return lst.size(); }
  void ordering();
  void choose(int);
};
<<<<<<< HEAD

enum MOVE_FLAGS {
  CHECK=1, CHECKMATE=3, PROMOTION=4, TAKE=8, APP_HORIZ=16, APP_VERT=32, ROCKING=64
};

struct shortmove {
  Figure fig;
  short flags;
  unsigned char fld, app;
};

class game {
  std::map<std::string, std::string> hdr;
  std::vector<shortmove> moves;
  int result;
public:
  game(std::map<std::string, std::string>*, std::vector<shortmove>*,int);
};
=======
>>>>>>> 271a2dc582392dc39009dd86c84afbcb11e54ba5
