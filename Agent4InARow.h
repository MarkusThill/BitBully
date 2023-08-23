#ifndef BITBULLY_AGENT4INAROW_H
#define BITBULLY_AGENT4INAROW_H

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <algorithm>
#include <memory>

//#define EVALOFF
//#define WINSEARCH
//#define WINDOWSEARCH
//#define CLAIMDRAW
//#define CLAIMWIN
//#define PROBCUT

#define RED 1
#define YELLOW 2
#define CHANGECOLOR 3
#define MATE 1000
#define COLUMNS 7
#define MAXDEPTH 42
#define FOURROW 69
#define HASHSIZE_F    0x800000 // 32MB
#define HASHSIZEAND 0x7FFFFF
#define HASHITER 4
#define THREATHASHSIZE 0x80000
#define LOWER_F 0
#define UPPER_F 1
#define EXACT_F 2
#undef HISTORY

// 5 13 21 29 37 45 53
// 4 12 20 28 36 44 52
// 3 11 19 27 35 43 51
// 2 10 18 26 34 42 50
// 1  9 17 25 33 41 49
// 0  8 16 24 32 40 48
#define COLUMN1 0x3FL
#define COLUMN2 0x3F00L
#define COLUMN3 0x3F0000L
#define COLUMN4 0x3F000000L
#define COLUMN5 0x3F00000000L
#define COLUMN6 0x3F0000000000L
#define COLUMN7 0x3F000000000000L

#define ROW1    0x01010101010101L
#define ROW2    0x02020202020202L
#define ROW3    0x04040404040404L
#define ROW4    0x08080808080808L
#define ROW5    0x10101010101010L
#define ROW6    0x20202020202020L
#define ROW7    0x40404040404040L

#define BOARD   0x3F3F3F3F3F3F3FL

#define ODDROWS  0x15151515151515L
#define EVENROWS 0x2A2A2A2A2A2A2AL

#define ENDGAMEWIN 100

#define EVAL_SMART 0
#define EVAL_AVERAGE 1
#define EVAL_DUMB 2

class Agent4InARow {

  typedef unsigned int int32;

  typedef u_int64_t int64;

  typedef u_int64_t MOVE;

  typedef struct {
    int64 yellow;
    int64 red;
    int color;
  } POSITION;

  typedef struct {
    int n;
    int64 fourrow[16];
  } MOVEINFO;

  typedef struct {
    int32 key;                    //32
    int value: 12;                //44
    unsigned int valuetype: 2;    //46
    unsigned int depth: 8;        //54		// 14 bits = 16'000 for "effort"
    unsigned int best: 4;        //58
    unsigned int effort: 6;        // padding to 64
  } HASHENTRY;

// 4inarowBB.c:
//
// a bitboard based 4inarow program
// 2002 Martin Fierz
//
// implemented:		- bitboard architecture using 64-bit integers (seems reasonably fast)
//					- hashtable
//					- move ordering:	- static from center to edge
//										- hash move
//										- killer move
//					- check for a move which wins first instead of calculating moves, and recognize forced moves
//					- added a very simplistic evaluation
//					- threats are updated incrementally
//					- zobrist hashkey is updated incrementally
//					- implemented recursive threat eval
//					- implemented IID
//					- implemented multiple hashprobes
//					- using windowed search
//					- hash threats for more speed

// benchmarks:	- with 4M entries in hashtable, 4inarowBB solves 4inarow in 5.4 hours.
//				- with 4M entries in HT, it takes 617s to see that after 4-3 6 is best move (with value +100)
//				- with 32M entries, it takes 3.4 (12185s) hours to solve 4inarow


// TODO: learning

//typedef unsigned int (*funcptr)();  // funcptr is synonym for "pointer
// 	to function returning int"
/*typedef INT (WINAPI* PROC1)(int *board, int color, double maxtime, char str[1024], int *playnow, int info, int unused, struct CBmove *move);
typedef INT (WINAPI* PROC3)(char str[255]);
typedef unsigned int (WINAPI* PROC4)(void);
typedef unsigned int (WINAPI* PROC7)(int key);
typedef INT (WINAPI* PROC6)(int *board, int color, int from, int to, struct CBmove *move);
typedef INT (WINAPI* COMMANDPROC)(char command[256], char reply[1024]); //enginecommand
PROC1 getmove=0,getmove1=0,getmove2=0;
COMMANDPROC enginecommandtmp=0,enginecommand1=0, enginecommand2=0;*/

//#define min(X,Y) (((X) < (Y)) ? (X) : (Y))
//#define max(X,Y) (((X) > (Y)) ? (X) : (Y))

  typedef int(Agent4InARow::*proc1)(POSITION *p);

  proc1 eval = 0;

// globals
  int64 nodes;
  int realdepth;
  int maxdepth;
  int startdepth;
  int bestmovecolumn;
  int randomize;
  MOVE bestmove;
  int64 hashhits, hashmisses;
  int64 yellowthreat, redthreat;
  int64 hashkey;
  double start;
  int *playnow;
  char *status;
  int gt_values[7];

#ifdef HISTORY
  int32 redhistory[64];
  int32 yellowhistory[64];
  int32 historycount;
#endif

// 5 13 21 29 37 45 53
// 4 12 20 28 36 44 52
// 3 11 19 27 35 43 51
// 2 10 18 26 34 42 50
// 1  9 17 25 33 41 49
// 0  8 16 24 32 40 48

  MOVE killer[MAXDEPTH];

  MOVEINFO moveinfo[64];

  int64 fourrow[FOURROW]; // FOURROW = 69; these are bitmasks for possible 4-in-a-row's

  int64 hashxors[3][64];

  std::unique_ptr<HASHENTRY[]> ht;
  std::unique_ptr<HASHENTRY[]> threatht;

  FILE *fp;

  int isinit = 0; //MT: removed static

  // Added by Markus:
  //levell = GAMETHEORETIC;
  enum levels { BEGINNER, INTERMEDIATE, EXPERT, MASTER, INFLEVEL, GAMETHEORETIC, SEARCHWIN } levell;
  double maxtime;
  int level;
  int leveltime[7] = {0, 8, 30, 300, 8600000, 86000000, 86000000};
  int evaltype = EVAL_SMART;
  int gametheoretic = 1;
  int searchwin = 0;

  int usethebook = 1;


// search engine below
 public:

  Agent4InARow() {
    ht = std::make_unique<HASHENTRY[]>(HASHSIZE_F + HASHITER);
    threatht = std::make_unique<HASHENTRY[]>(THREATHASHSIZE);

    level = leveltime[GAMETHEORETIC];
    maxtime = ((double) level) / 10.0;
  }

  int getColorToMoveFromBoard(int board[7][6]) {
    int sum=0;
    for (auto i = 0UL; i < 7; ++i) {
      for (auto j = 0UL; j < 6; ++j) {
        sum += board[i][j];
      }
    }
    // sum can be 0 or 1: 0->yellow to move, 1->red to move
    // 0-> 1, 1->-1
    return -2 * sum + 1;
  }

  std::vector<int> getGTvalues() {
    std::vector<int> p;
    for(const auto& gt : gt_values) {
      p.emplace_back(gt);
    }
    return p;
  }

  inline int findmove(int board[7][6], char *out) {
    int color = getColorToMoveFromBoard(board);
    int playnow = 0; // TODO: What is this?
    int randomize = 0; // How does this affect the results?
    return findmove(board, color, maxtime, out, &playnow, searchwin, randomize, usethebook, gametheoretic, evaltype);
  }

  int findmove(int board[7][6],
               int color,
               double maxtime,
               char *out,
               int *play,
               int sw,
               int rl,
               int usebook,
               int gt,
               int evaltype) {
    // findmove is the entry point to 4inarow.c
    // we transform the array board to a bitboard, and call search()
    POSITION p;
    int move;
    init();
    boardtobitboard(board, color, &p);
    playnow = play;
    status = out;
    randomize = rl;

    // select eval:
    switch (evaltype) {
      case EVAL_SMART:eval = &Agent4InARow::evaluation;
        break;
      case EVAL_AVERAGE:eval = &Agent4InARow::dumbevaluation;
        break;
      case EVAL_DUMB:eval = &Agent4InARow::stupidevaluation;
        break;
      default:eval = &Agent4InARow::evaluation;
    }

    move = search(&p, maxtime, out, sw, gt);
    return move;
  }

  void boardtobitboard(int b[7][6], int color, POSITION *p) {

    // initialize the fourrows bitboard
    // board representation:
    //
    //
    // 5 13 21 29 37 45 53
    // 4 12 20 28 36 44 52
    // 3 11 19 27 35 43 51
    // 2 10 18 26 34 42 50
    // 1  9 17 25 33 41 49
    // 0  8 16 24 32 40 48
    // diagonal one (12)

    int i, j;
    int64 one = 1L;

    if (color == 1)
      p->color = YELLOW;
    else
      p->color = RED;
    p->red = 0L;
    p->yellow = 0L;
    for (i = 0; i < 7; i++) {
      for (j = 0; j < 6; j++) {
        if (b[i][j] == 1)
          p->yellow |= (one << (8 * i + (j)));
        if (b[i][j] == -1)
          p->red |= (one << (8 * i + (j)));
      }
    }
    printf("\ndone");
  }

  int search(POSITION *p, double time, char out[255], int searchwin, int theoretic) {
    // search in position p, with iterative deepening, until run out of time in seconds
    int depth;
    int value;
    //double start;
    double t;
    int i;
    int remainingmoves;
    int column;
    float knodes;
    int64 allmoves, move;
    int forced = 0;
    int64 columns[7] = {COLUMN1, COLUMN2, COLUMN3, COLUMN4, COLUMN5, COLUMN6, COLUMN7};
    char str[256], str2[256];
    int absvalue, dolearn = 0;
    //fp = fopen("4inarow.txt","w");
    // if there is only one move, play it immediately



    allmoves = ((p->red | p->yellow) << 1);
    allmoves |= ROW1;
    allmoves ^= (p->red | p->yellow);    // now allmoves contains the bitboard of possible moves
    allmoves &= BOARD;

    // set threats: yellowthreat and redthreat are updated
    setthreats(p);

    // if there is only one move, play it:
    if (bitcount(allmoves) == 1)
      forced = 1;

    // TODO: if all moves lose, then 4 in a row won't play any more
    // => need to do something so that it detects whether all open
    // moves are threats of the opponent, and then play one at random.
    if ((((allmoves << 1) & yellowthreat) == (allmoves << 1)) && (p->color == RED)) {
      forced = 1;
    }
    if ((((allmoves << 1) & redthreat) == (allmoves << 1)) && (p->color == YELLOW)) {
      forced = 1;
    }

    if (forced) {
      sprintf(status, "forced move");
      column = movetocolumn(allmoves);
      return column - 1;
    }

    // if there is an open threat of the opponent, set
    // forced to 1 and break in the iterative deepening loop

    if (allmoves & (redthreat | yellowthreat))
      forced = 1;

    // Markus Thill: Added #ifdef
#ifdef CLEAR_HASH
    // clear hashtable
    memset(ht, 0, (HASHSIZE_F+HASHITER)*sizeof(HASHENTRY));
    // clear threat hashtable
    memset(threatht, 0 , THREATHASHSIZE*sizeof(HASHENTRY));
#endif

    // read in learn file
    //readlearnfile();


#ifdef HISTORY
    for(i=0;i<64;i++)
        {
        redhistory[i] = 0;
        yellowhistory[i] = 0;
        }
    historycount = 0;
#endif

    start = clock();
    nodes = 0;
    realdepth = 0;
    startdepth = bitcount(p->yellow | p->red);
    maxdepth = 0;
    hashhits = 0;
    hashmisses = 0;
    hashkey = absolutehashkey(p);
    value = 0;


    // how many moves are left?
    remainingmoves = MAXDEPTH - bitcount(p->red | p->yellow);

    fp = fopen("4inarow.txt", "a");
    if (fp != NULL)
      printboardtofile(p, fp);
    fclose(fp);

//	fp = fopen("theoretic.txt","w");

    // initialize gt_values array to zero for game-theoretic search
    for (i = 0; i < 7; i++)
      gt_values[i] = 0;

    // now initialize instant winners for game-theoretic search
    // i have to use -MATE because the display of the game-theoretic
    // gt_values flips the gt_values from win to loss and vice versa
    for (i = 0; i < 7; i++) {
      move = allmoves & columns[i];
      if (p->color == YELLOW) {
        if (move & yellowthreat)
          gt_values[i] = -MATE;
      } else {
        if (move & redthreat)
          gt_values[i] = -MATE;
      }
    }

    for (depth = 1; depth <= remainingmoves; depth += 2) {
      if (theoretic) {
        //fprintf(fp,"\n---------\ndepth%i\n----------\n",depth);
        // make a movelist

        // the allmoves bitboard was computed earlier
        for (i = 0; i < 7; i++) {
          move = allmoves & columns[i];
          if (move == 0) {
            gt_values[i] = -1;
            continue;
          }

          // if we already concluded that a certain move is a win or a loss,
          // then don't search it again!
          if (abs(gt_values[i]) > 300)
            continue;

          domove(p, &move);
          //fprintf(fp,"\nsearching move %i",i);
          //printboardtofile(p,fp);
          // set threats:
          setthreats(p);
          hashkey = absolutehashkey(p);
          realdepth = 0;
          startdepth = bitcount(p->yellow | p->red);
          //gt_values[i] = negamax(p,depth, -MATE, MATE);
          gt_values[i] = windowsearch(p, depth - 1, gt_values[i]);
          //fprintf(fp,"result: %i",gt_values[i]);
          undomove(p, &move);
        }
        sprintf(str, "depth %i:", depth);
        for (i = 0; i < 7; i++) {
          sprintf(str2, " ? ");
          if (gt_values[i] > 300)
            sprintf(str2, " loss ");
          if (gt_values[i] < -300)
            sprintf(str2, " win ");
          if (gt_values[i] == -1)
            sprintf(str2, " - ");

          strcat(str, str2);
        }
        sprintf(status, "%s", str);
      } else {
        if (searchwin)
          value = negamax(p, depth, 300, 301);
        else {
#ifdef WINDOWSEARCH
          value = windowsearch(p, depth, value);
#else
          value = negamax(p, depth, -MATE, MATE);
#endif
        }
        t = (clock() - start) / CLOCKS_PER_SEC;

        column = movetocolumn(bestmove);

        bestmovecolumn = column;
        //printmove(&bestmove);
        knodes = 0;

        if (t > 0) {
          knodes = nodes / t;
          knodes /= 1000;
        }
        if (searchwin) {
          if (value <= 300)
            sprintf(status, "depth %i/%i time %.2fs: no win found", depth, maxdepth, t);
          else
            sprintf(status, "depth %i/%i time %.2fs: win found: %i", depth, maxdepth, t, column);
        } else
          sprintf(status,
                  "depth %i/%i move %i value %i nodes %lu time %.2f hash %.1f %.1fkN/s",
                  depth,
                  maxdepth,
                  column,
                  value,
                  nodes,
                  t,
                  (float) (100 * hashhits) / ((float) (hashhits + hashmisses)),
                  knodes);

        logtofile(depth, value, bestmovecolumn);
        //fflush(fp);
#ifndef WINSEARCH
        if (t > time && time != 0)
          break;
#endif
        if (abs(value) > 300)
          break;
        if (*playnow)
          break;
        // make instant level into a depth 1 search level
        if (time == 0 && depth > 1)
          break;
        if (forced)
          break;
      }
    }
    if (theoretic) {
      sprintf(str, "done: %d", depth);
      for (i = 0; i < 7; i++) {
        sprintf(str2, " = ");
        if (gt_values[i] > 300)
          sprintf(str2, " loss ");
        if (gt_values[i] < -300)
          sprintf(str2, " win ");
        if (gt_values[i] == -1)
          sprintf(str2, " - ");

        strcat(str, str2);
      }
      sprintf(status, "%s", str);
    }

    // save wins in learn file
    if (value < 0)
      absvalue = -value;
    else
      absvalue = value;

    if ((depth >= remainingmoves) && absvalue < 50 && depth > 9) {
      value = 0;
      dolearn = 1;
    }

    if (absvalue > 900 && absvalue < 991)
      dolearn = 1;

    if (dolearn == 1) {
      fp = fopen("c4.lrn", "a+b");
      fwrite(&(p->yellow), sizeof(int64), 1, fp);
      fwrite(&(p->red), sizeof(int64), 1, fp);
      fwrite(&(value), sizeof(int), 1, fp);
      fclose(fp);
    }

    domove(p, &bestmove);

    column = movetocolumn(bestmove);
    return column - 1;
  }

  void readlearnfile(void) {
    FILE *fp, *fp2;
    POSITION q;
    int value;
    fp = fopen("c4.lrn", "rb");
    if (fp == NULL)
      return;
    fp2 = fopen("learn.txt", "w");
    while (!feof(fp)) {
      size_t x = fread(&q.yellow, sizeof(int64), 1, fp);
      x = fread(&q.red, sizeof(int64), 1, fp);
      x = fread(&value, sizeof(int), 1, fp);
      hashkey = absolutehashkey(&q);
      store(&q, 42, -MATE, MATE, value, 0);
      q.color = YELLOW;
      if (bitcount(q.yellow) > bitcount(q.red))
        q.color = RED;
      printboardtofile(&q, fp2);
      fprintf(fp2, "value = %i\n\n", value);
    }
    fclose(fp);
    fclose(fp2);
  }

  void setthreats(POSITION *p) {
    int i;

    redthreat = 0;
    yellowthreat = 0;
    for (i = 0; i < FOURROW; i++) {
      if (!(fourrow[i] & p->yellow) && bitcount(fourrow[i] & p->red) == 3)
        redthreat |= (~p->red) & fourrow[i];
      if (!(fourrow[i] & p->red) && bitcount(fourrow[i] & p->yellow) == 3)
        yellowthreat |= (~p->yellow) & fourrow[i];
    }
  }

  int movetocolumn(MOVE m) {
    int column = 11;
    if (m & COLUMN1)
      column = 1;
    if (m & COLUMN2)
      column = 2;
    if (m & COLUMN3)
      column = 3;
    if (m & COLUMN4)
      column = 4;
    if (m & COLUMN5)
      column = 5;
    if (m & COLUMN6)
      column = 6;
    if (m & COLUMN7)
      column = 7;
    return column;
  }

  void logtofile(int depth, int value, int move) {
    double t;

    t = clock() - start;
    t /= CLOCKS_PER_SEC;
    fp = fopen("4inarow.txt", "a");

    fprintf(fp, "\ndepth %i/%i move %i value %i nodes %lu time %.2f", depth, maxdepth, move, value, nodes, t);
    fclose(fp);
  }

  int windowsearch(POSITION *p, int depth, int guess) {
    int alpha = guess - 1;
    int beta = guess + 1;
    int value;

    value = negamax(p, depth, alpha, beta);

    if (value >= beta) {
      fp = fopen("4inarow.txt", "a");
      fprintf(fp, "+");
      fclose(fp);
      logtofile(depth, value, bestmovecolumn);
      alpha = -MATE;
      beta = MATE;
      value = negamax(p, depth, alpha, beta);
      return value;
    }

    if (value <= alpha) {
      fp = fopen("4inarow.txt", "a");
      fprintf(fp, "-");
      fclose(fp);
      logtofile(depth, value, bestmovecolumn);
      alpha = -MATE;
      beta = MATE;
      value = negamax(p, depth, alpha, beta);
      return value;
    }

    return value;
  }

  int negamax(POSITION *p, int depth, int alpha, int beta) {
    int i, j, n;
    int value, bestvalue = -MATE + realdepth + 1;
    int bestindex = 0;
    int lastbestindex = -1;
    int ordervalues[COLUMNS] = {0, 0, 0, 0, 0, 0, 0};
    int index;
    int bestorder;
    int64 l_yt, l_rt;
    int64 storedhashkey;
    int64 allmoves;
    int isforced = 0;
    int moveindex;
    int original_alpha = alpha, original_beta = beta;
    int32 effort = nodes;
    MOVE m[COLUMNS];

    nodes++;
    if ((*playnow) != 0)
      return 0;

    if (realdepth > maxdepth)
      maxdepth = realdepth;

    // is board full?
    if (realdepth + startdepth == 42)
      return 0;

    // hashlookup
    if (depth > 0) {
      if (retrieve(p, depth, &alpha, &beta, &value, &lastbestindex))
        return value;
    }

    // a quick check whether we win:
    allmoves = ((p->red | p->yellow) << 1);
    allmoves |= ROW1;
    allmoves ^= (p->red | p->yellow);    // now allmoves contains the bitboard of possible moves
    if (allmoves & (yellowthreat | redthreat)) {
      isforced = 1;
      if (p->color == YELLOW) {
        if (allmoves & yellowthreat) {
          if (realdepth == 0)
            bestmove = allmoves & yellowthreat;
          return MATE - realdepth;
        }
      }
      if (p->color == RED) {
        if (allmoves & redthreat) {
          if (realdepth == 0)
            bestmove = allmoves & redthreat;
          return MATE - realdepth;
        }
      }
    }

    // makemovelist inlined - if done like this instead of a separate function,
    // 4inarowBB is 10% faster, therefore i keep it in this ugly form.

    // allmoves contains all places where the side to move can set a stone.

    // we remove moves which lose instantly:
    // 5 13 21 29 37 45 53
    // 4 12 20 28 36 44 52
    // 3 11 19 27 35 43 51
    // 2 10 18 26 34 42 50
    // 1  9 17 25 33 41 49
    // 0  8 16 24 32 40 48

    if (p->color == RED)
      allmoves &= ~(yellowthreat >> 1);
    else
      allmoves &= ~(redthreat >> 1);

    n = 0;
    m[n] = allmoves & COLUMN4;
    if (m[n]) {
      ordervalues[n] = moveinfo[lsb(m[n])].n;
      n++;
    }
    m[n] = allmoves & COLUMN3;
    if (m[n]) {
      ordervalues[n] = moveinfo[lsb(m[n])].n;
      n++;
    }
    m[n] = allmoves & COLUMN5;
    if (m[n]) {
      ordervalues[n] = moveinfo[lsb(m[n])].n;
      n++;
    }
    m[n] = allmoves & COLUMN2;
    if (m[n]) {
      ordervalues[n] = moveinfo[lsb(m[n])].n;
      n++;
    }
    m[n] = allmoves & COLUMN6;
    if (m[n]) {
      ordervalues[n] = moveinfo[lsb(m[n])].n;
      n++;
    }
    m[n] = allmoves & COLUMN1;
    if (m[n]) {
      //ordervalues[n] = moveinfo[lsb(m[n])].n;
      n++;
    }
    m[n] = allmoves & COLUMN7;
    if (m[n]) {
      //ordervalues[n] = moveinfo[lsb(m[n])].n;
      n++;
    }

    //printboard(p);
    //printmove(&allmoves);
    //getch();

    /*for(i=0;i<n;i++)

    getch();*/
    /*for(i=0;i<n;i++)
        {
        printmove(&m[i]);
        printf("%i:%i  ",lsb(m[i]),ordervalues[i]);
        getch();
        }
*/
    // draw claim if only one row is open and no threats
    /*if(n==1 && realdepth+startdepth>=36)
        {
        empty = p->red|p->yellow;
        empty = ~empty;
        empty &= (redthreat|yellowthreat);
        //if no threats on empty squares return 0
        if( !empty)
            {
            //printboard(p);
            //getch();
            return 0;
            }
        }*/

    // depth return condition: if depth < zero we return, unless there are open threats!!
    if (depth <= 0) {
      if (!isforced)
        return (this->*eval)(p);
    }
    isforced = 0;

    // check win using threats; at the same time, order moves to front if they are forced
    // remember the fact that a move is forced in the boolean "isforced"
    if (p->color == YELLOW) {
      for (i = 0; i < n; i++) {
        if (m[i] & redthreat) {
          ordervalues[i] += 1024;
          isforced = 1;
        }
#ifdef HISTORY
        ordervalues[i] += ((32*yellowhistory[lsb(m[i])])/(historycount+1));
#endif
      }
    } else {
      for (i = 0; i < n; i++) {
        if (m[i] & yellowthreat) {
          ordervalues[i] += 1024;
          isforced = 1;
        }
#ifdef HISTORY
        ordervalues[i] += ((32*redhistory[lsb(m[i])])/(historycount+1));
#endif
      }
    }

#ifdef CLAIMDRAW
    // check if we can claim a draw:
    if(realdepth+startdepth >=20 && ! ( (yellowthreat|redthreat)&(~(p->yellow|p->red)) ) )
        {
        if(claimdraw(p))
            return 0;
        }
#endif

#ifdef CLAIMWIN
    if(realdepth+startdepth >=36 && ! ( (yellowthreat|redthreat)&(~(p->yellow|p->red)) ) )
        {
        if(bitcount((p->red|p->yellow)&ROW6)==6)
            return 4*evaluation(p);
        }
#endif



    // order movelist:
    // hashmove
    if (lastbestindex != -1)
      ordervalues[lastbestindex] += 256;
    // killer move
    for (i = 0; i < n; i++) {
      if (m[i] == killer[realdepth]) {
        ordervalues[i] += 64;
        break;
      }
    }

    // save threats:
    l_yt = yellowthreat;
    l_rt = redthreat;

    // for all moves do: domove, recurse, undomove, update alpha&beta
    storedhashkey = hashkey;
    for (i = 0; i < n; i++) {
      // get index
      bestorder = -1;
      for (j = 0; j < n; j++) {
        if (ordervalues[j] > bestorder) {
          index = j;
          bestorder = ordervalues[j];
        }
      }
      ordervalues[index] = -1;

      // recurse
      domove(p, &m[index]);

      // update threats:
      moveindex = lsb(m[index]);
      if (p->color == RED) // look for new yellow threats
      {
        for (j = 0; j < moveinfo[moveindex].n; j++) {
          if (!(moveinfo[moveindex].fourrow[j] & p->red)) {
            if (bitcount(moveinfo[moveindex].fourrow[j] & p->yellow) == 3) {
              yellowthreat |= (moveinfo[moveindex].fourrow[j] & (~p->yellow));
            }
          }
        }
      } else {
        for (j = 0; j < moveinfo[moveindex].n; j++) {
          if (!(moveinfo[moveindex].fourrow[j] & p->yellow)) {
            if (bitcount(moveinfo[moveindex].fourrow[j] & p->red) == 3) {
              redthreat |= (moveinfo[moveindex].fourrow[j] & (~p->red));
            }
          }
        }
      }

      realdepth++;
      hashkey ^= hashxors[p->color ^ CHANGECOLOR][moveindex]; // hotspot - 10% of negamax time

      value = -negamax(p, depth - 1, -beta, -alpha);

      hashkey = storedhashkey;
      realdepth--;

      undomove(p, &m[index]);
      // revert threats
      yellowthreat = l_yt;
      redthreat = l_rt;

      if (value >= beta) {
        bestvalue = value;
        bestindex = index;
#ifdef HISTORY
        // update history table
        if(p->color == YELLOW)
            {
            yellowhistory[moveindex]+=(1<<(depth/2));
            historycount+=(1<<(depth/2));
            }
        else
            {
            redhistory[moveindex]+=(1<<(depth/2));
            historycount+=(1<<(depth/2));
            }
#endif
        break;
      }

      if (value > alpha) {
        alpha = value;
        //bestvalue = value;
        bestindex = index;
      }

      bestvalue = std::max(bestvalue, value);

      if (isforced) {
        //bestvalue = max(bestvalue,alpha);
        break;
      }
    }

    // why am i doing this? this is basically making 4inarow do a fail-hard alpha-beta
    // do i really want this?
    // bestvalue = max(bestvalue, alpha);

    /*if(bestvalue < alpha)
        {
        printboard(p);
        printf("\nbestvalue = %i, alpha = %i, beta = %i", bestvalue, alpha, beta);
        printf("\nn = %i, value = %i",n, value);
        getch();
        }*/



    // store position in hashtable
    if (depth > 0)
      store(p, depth, original_alpha, original_beta, bestvalue, bestindex);

    // set killer move
    killer[realdepth] = m[bestindex];

    if (realdepth == 0)
      bestmove = m[bestindex];

    return bestvalue;
  }

  void store(POSITION *p, int depth, int alpha, int beta, int bestvalue, int bestindex/*, int32 effort*/) {
    unsigned int index;
    HASHENTRY he;
    int i = 0;
    int lowestindex = 0;
    int lowestdepth = MAXDEPTH;
    int fom;
    // get index to write to:
    index = hashkey & HASHSIZEAND;

    he.best = bestindex;
    he.value = bestvalue;
    he.depth = depth;
    he.key = (int32) (hashkey >> 32);
    he.valuetype = EXACT_F;
    if (bestvalue >= beta)
      he.valuetype = LOWER_F;
    if (bestvalue <= alpha)
      he.valuetype = UPPER_F;

    // find place to write
    while (i < HASHITER) {

      if (ht[index + i].key == 0 || ht[index + i].key == (int32) (hashkey >> 32)) {
        // update or empty entry
        ht[index + i] = he;
        return;
      }

      fom = ht[index + i].depth;

      if (fom <= lowestdepth) {
        lowestindex = index + i;
        lowestdepth = fom;
      }

      i++;
    }


    // if we drop out here, that's because we couldn't find a place to write!
    // overwrite the least valuable entry:
    //if(depth < lowestdepth+14)
    ht[lowestindex] = he;

  }

  int retrieve(POSITION *p, int depth, int *alpha, int *beta, int *value, int *bestindex) {
    unsigned int index;
    int i = 0;
    // get index to write to:
    //index = hashkey%HASHSIZE;
    index = hashkey & HASHSIZEAND;
    *bestindex = -1;

    while (i < HASHITER) {
      if (ht[index + i].key == (int32) (hashkey >> 32)) // super-hotspot: 30% of program!
      {
        hashhits++;
        // we found the position
        *bestindex = ht[index + i].best;

        // enough depth to do something?
        if (ht[index + i].depth < depth)
          return 0;

        // yes!
        if (ht[index + i].valuetype == EXACT_F) {
          *value = ht[index + i].value;
          return 1;
        }

        if (ht[index + i].valuetype == LOWER_F) {
          if (ht[index + i].value >= *beta) {
            *value = ht[index + i].value;
            return 1;
          }
          if (ht[index + i].value > *alpha) {
            *alpha = ht[index + i].value;
            return 0;
          }
        }

        if (ht[index + i].valuetype == UPPER_F) {
          if (ht[index + i].value <= *alpha) {
            *value = ht[index + i].value;
            return 1;
          }
          if (ht[index + i].value < *beta) {
            *beta = ht[index + i].value;
            return 0;
          }
        }
      }
      i++;
    }

    hashmisses++;

    return 0;
  }

  void threatstore(int64 yt, int64 rt, int value) {
    unsigned int index;
    int64 key;
    POSITION p;
    HASHENTRY he;

    p.yellow = yt;
    p.red = rt;
    key = absolutehashkey(&p);

    // get index to write to:
    index = key % THREATHASHSIZE;

    he.value = value;
    he.key = (int32) (key >> 32);

    threatht[index] = he;
  }

  int threatretrieve(int64 yt, int64 rt, int *value) {
    unsigned int index;
    int64 key;
    POSITION p;
    // get index to write to:
    p.yellow = yt;
    p.red = rt;

    key = absolutehashkey(&p);

    index = key % THREATHASHSIZE;

    if (threatht[index].key == (int32) (key >> 32)) {
      *value = threatht[index].value;
      return 1;
    }

    return 0;
  }

  int claimdraw(POSITION *p) {
    // checks whether all non-vertical four-rows have a piece of both
    // sides in. if yes, it returns 1, else 0.
    int i;

    // we start at the highest horizontal rows, which should lead to a rather quick
    // return 0 in most cases.
    for (i = 47; i >= 0; i--) {
      if (!((p->yellow & fourrow[i]) && (p->red & fourrow[i])))
        return 0;
    }
    return 1;
  }

  int dumbevaluation(POSITION *p) {
    // calculates the value as seen from the yellow side
    // returns value/-value depending on who is to move
    // this version of eval is piece-square dependent, and dumb.
    // it is used if the human wants a weaker opponent.
    int value = 0;
    int64 free = ~(p->red | p->yellow);
    int64 yt, rt;
    yt = free & yellowthreat;
    rt = free & redthreat;
    value += 3 * bitcount(p->yellow & COLUMN4);
    value += 2 * bitcount(p->yellow & (COLUMN3 | COLUMN5));
    value += bitcount(p->yellow & (COLUMN2 | COLUMN6));
    value -= 3 * bitcount(p->red & COLUMN4);
    value -= 2 * bitcount(p->red & (COLUMN3 | COLUMN5));
    value -= bitcount(p->red & (COLUMN2 | COLUMN6));

    value += 10 * bitcount(yt);
    value -= 10 * bitcount(rt);

    return p->color == YELLOW ? value : -value;
  }

  int stupidevaluation(POSITION *p) {
    // stupideval is identical to dumbeval except that it changes the sign!
    // this makes it really stupid, so that it is hard not to win a game
    int value = 0;
    int64 free = ~(p->red | p->yellow);
    int64 yt, rt;
    yt = free & yellowthreat;
    rt = free & redthreat;
    value += 3 * bitcount(p->yellow & COLUMN4);
    value += 2 * bitcount(p->yellow & (COLUMN3 | COLUMN5));
    value += bitcount(p->yellow & (COLUMN2 | COLUMN6));
    value -= 3 * bitcount(p->red & COLUMN4);
    value -= 2 * bitcount(p->red & (COLUMN3 | COLUMN5));
    value -= bitcount(p->red & (COLUMN2 | COLUMN6));

    value += 10 * bitcount(yt);
    value -= 10 * bitcount(rt);

    return p->color == YELLOW ? -value : value;
  }

  int evaluation(POSITION *p) {
    // calculates the value as seen from the yellow side
    // returns value/-value depending on who is to move
    int value = 0;
    int64 free = ~(p->red | p->yellow);
    int64 yt, rt, dt;

#ifdef EVALOFF
    return 0;
#endif

    yt = free & yellowthreat;
    rt = free & redthreat;
    if (!threatretrieve(yt, rt, &value)) {
      dt = rt & yt;
      yt ^= dt;
      rt ^= dt;
      value = threatevaluate(yt, rt, dt);
      threatstore(yt | dt, rt | dt, value);
    }

    if (randomize)
      value += ((rand() % 9) - 5);

    return p->color == YELLOW ? value : -value;
  }

  int threatevaluate(int64 yt, int64 rt, int64 dt) {
    // evaluates the threats: returns value as seen from the YELLOW side
    int i;
    int64 allthreats = yt | rt | dt;
    int64 threatmask = (allthreats << 1) & BOARD;
    int64 playablesquares;
    int64 tmpthreat;
    int64 giveup;
    int bestvalue;

//	POSITION p;

    if (allthreats == 0)
      return 0;
    // board representation:
    //
    //
    // 5 13 21 29 37 45 53
    // 4 12 20 28 36 44 52
    // 3 11 19 27 35 43 51
    // 2 10 18 26 34 42 50
    // 1  9 17 25 33 41 49
    // 0  8 16 24 32 40 48

    //if(threatretrieve(yt|dt,rt|dt,&bestvalue))
    //	return bestvalue;
    // get threatmask: this masks off the part of the board above threats.
    for (i = 0; i < 3; i++) {
      threatmask |= ((threatmask << 1) & BOARD);
    }

    // example:
    // 5  * 21 29 37  * 53
    // 4  * 20 28 36  * 52
    // 3  * 19 27 35  * 51
    // 2  T 18 26 34  * 50
    // 1  9 17 25 33  T 49
    // 0  8 16 24 32 40 48

    // now, all squares which are not in the threatmask or two below can be played
    playablesquares = ~((threatmask | (threatmask >> 1) | (threatmask >> 2)) & BOARD);

    // if the number of playable squares is odd, then yellow has an advantage, because
    // he can play the last square below all threats
    if ((bitcount(playablesquares) - 22) & 1) {
      // red must give up a threat
      if (!(rt & (~threatmask))) {
        //printf("!rt ");
        return ENDGAMEWIN;
      } else {
        // iterate over all red threats that can be given up, call
        // threatevaluate again without this threat, and return the
        // minimum of this eval, as red will give up the threat which
        // helps him most.
        // save red threats that can be given up:
        bestvalue = MATE;
        tmpthreat = rt & (~threatmask);
        while (tmpthreat) {
          giveup = (tmpthreat & (tmpthreat - 1)) ^ tmpthreat; // least significant bit
          bestvalue = std::min(bestvalue, threatevaluate(yt, rt ^ giveup, dt));
          if (bestvalue == -ENDGAMEWIN)
            return bestvalue;
          tmpthreat = tmpthreat & (tmpthreat - 1);
        }
        return bestvalue;
      }
    } else {
      // yellow must give up a threat
      if (!(yt & (~threatmask))) {
        //printf("!yt ");
        return -ENDGAMEWIN;
      } else {
        // iterate over all yellow threats that can be given up, and
        // recursively call threatevaluate again, and return the maximum
        // of these values, as yellow seeks to maximize
        bestvalue = -MATE;
        tmpthreat = yt & (~threatmask);
        while (tmpthreat) {
          giveup = (tmpthreat & (tmpthreat - 1)) ^ tmpthreat; // least significant bit
          bestvalue = std::max(bestvalue, threatevaluate(yt ^ giveup, rt, dt));
          if (bestvalue == ENDGAMEWIN)
            return bestvalue;
          tmpthreat = tmpthreat & (tmpthreat - 1);
        }
        return bestvalue;

      }
    }
    return 0;
  }

  void domove(POSITION *p, MOVE *m) {
    // domove does a move m on the board p
    if (p->color == YELLOW) {
      p->yellow ^= *m;
      p->color = RED;
    } else {
      p->red ^= *m;
      p->color = YELLOW;
    }
  }

  void undomove(POSITION *p, MOVE *m) {
    // undo a move on the board - same as domove, but renamed for clarity
    if (p->color == YELLOW) {
      p->red ^= *m;
      p->color = RED;
    } else {
      p->yellow ^= *m;
      p->color = YELLOW;
    }
  }

  int bitcount(int64 a) {
    int c = 0;
    int64 x = a;

    while (x) {
      c++;
      x &= x - 1;
    }
    return (c);
  }


/*int msb32(int32 x)
	{
	// most significant bit for 32-bit numbers
	__asm
		{
		mov eax, -1
		bsr eax, dword ptr x
		}
	}*/

/*
int lsb(int64 a) {

  __asm volatile {
		mov eax, 0
		bsf edx, dword ptr a
		jnz l1
		bsf edx, dword ptr a+4
		mov eax, 32
		jnz l1
		mov edx, -32
l1:		add eax, edx
  }
}*/

  int lsb(int64 a) {
    return ffsl(a);
  }

/*  old version, 4inarow works with it
int lsb(int64 a) {

  __asm {
        bsf     edx, dword ptr a
        mov     eax, 63
        jnz     l1
        bsf     edx, dword ptr a+4
        mov     eax, 31
        jnz     l1
        mov     edx, -33
  l1:   sub     eax, edx
  }
}
*/

  int64 myrand(void) {
    // returns a 64bit random number
    int64 a = 0;
    int i;
    for (i = 0; i < 6; i++)
      a += ((int64) rand()) << 8 * i;
    return a;
  }

  int64 absolutehashkey(POSITION *p) {
    int64 key = 0;
    int64 x;

    x = p->yellow;
    while (x) {
      key ^= hashxors[YELLOW][lsb(x)];
      x = x & (x - 1);
    }

    x = p->red;
    while (x) {
      key ^= hashxors[RED][lsb(x)];
      x = x & (x - 1);
    }

    return key;
  }


// search engine above


  int init(void) {
    int i, j;
    int n = 0;
    int64 one = 1L;
    MOVE m;
    int index;
//	int val;


    if (isinit)
      return 0;
    // initialize the fourrows bitboard masks:
    // board representation:
    //
    //
    // 5 13 21 29 37 45 53
    // 4 12 20 28 36 44 52
    // 3 11 19 27 35 43 51
    // 2 10 18 26 34 42 50
    // 1  9 17 25 33 41 49
    // 0  8 16 24 32 40 48
    // diagonal one (12)


    fp = fopen("4inarow.txt", "w");
    //fprintf(fp,"\ninitialization!");
    fclose(fp);

    for (i = 0; i < COLUMNS - 3; i++) {
      for (j = 3; j < 6; j++) {
        fourrow[n] = 0;
        fourrow[n] |= one << (i * 8 + j);
        fourrow[n] |= one << (i * 8 + j + 7);
        fourrow[n] |= one << (i * 8 + j + 14);
        fourrow[n] |= one << (i * 8 + j + 21);
        n++;
      }
    }
    // diagonal two (12)
    for (i = 0; i < COLUMNS - 3; i++) {
      for (j = 0; j < 3; j++) {
        fourrow[n] = 0;
        fourrow[n] |= one << (i * 8 + j);
        fourrow[n] |= one << (i * 8 + j + 9);
        fourrow[n] |= one << (i * 8 + j + 18);
        fourrow[n] |= one << (i * 8 + j + 27);
        n++;
      }
    }
    // horizontal (24)
    for (i = 0; i < COLUMNS - 3; i++) {
      for (j = 0; j < 6; j++) {
        fourrow[n] = 0;
        fourrow[n] |= one << (i * 8 + j);
        fourrow[n] |= one << (i * 8 + j + 8);
        fourrow[n] |= one << (i * 8 + j + 16);
        fourrow[n] |= one << (i * 8 + j + 24);
        n++;
      }
    }
    // vertical (21)
    for (i = 0; i < COLUMNS; i++) {
      for (j = 0; j < 3; j++) {
        fourrow[n] = 0;
        fourrow[n] |= one << (i * 8 + j);
        fourrow[n] |= one << (i * 8 + j + 1);
        fourrow[n] |= one << (i * 8 + j + 2);
        fourrow[n] |= one << (i * 8 + j + 3);
        n++;
      }
    }

    // initialize MOVEINFO structure:
    // for every possible move, we get an index by index=lsb(move)
    // this index points to a MOVEINFO structure which tells how many
    // four-rows this move participates in, and then lists them.

    for (i = 0; i < 64; i++) {
      m = one << i;
      // check if this is a legal move at all
      if (!(m & BOARD))
        continue;

      n = 0;
      index = lsb(m);

      // board representation:
      //
      //
      // 5 13 21 29 37 45 53
      // 4 12 20 28 36 44 52
      // 3 11 19 27 35 43 51
      // 2 10 18 26 34 42 50
      // 1  9 17 25 33 41 49
      // 0  8 16 24 32 40 48
      for (j = 0; j < FOURROW; j++) {
        if (fourrow[j] & m) {
          if (!(fourrow[j] & (m << 1) || fourrow[j] & (m >> 1))) {
            // it's not a vertical row - use in any case
            moveinfo[index].fourrow[n] = fourrow[j];
            n++;
          } else {
            // it is a vertical row - use only if m is the third stone in the row
            if ((fourrow[j] & (m << 1)) && (fourrow[j] & (m >> 2))) {
              moveinfo[index].fourrow[n] = fourrow[j];
              n++;
            }
          }
        }
      }
      moveinfo[index].n = n;
    }

    // initialize hashxors
    for (i = 0; i < 3; i++) {
      for (j = 0; j < 64; j++)
        hashxors[i][j] = myrand();
    }

    isinit = 1;
    // open file

    return 1;
  }

  int makemove(POSITION *p, int column, MOVE *m) {
    // user enters column number, and this function turns it into a move
    int64 occupied = p->red | p->yellow;
    MOVE move;

    // shift column i into column 0 and mask off rest
    move = ((occupied >> (8 * column)) & 0xFFL);
    // now get the highest bit of this column. possible values of move are
    // 0,1, 3, 7, 15, 31 and 63. these correspond to moves 1,2,4,8...
    // so the move to be done is just move++;
    move++;
    // is this column full?
    if (move == 64)
      return 0;
    // not full, generate move
    *m = move << (8 * column);
    return 1;
  }

  int fourinarow_newgame(POSITION *p) {
    // resets the board for a new game
    p->color = YELLOW;
    p->yellow = 0;
    p->red = 0;
//	yellowthreat = 0;
//	redthreat = 0;
    return 1;
  }

  int fourinarow_printboard(POSITION *p) {
    int64 one = 1;
    int i, j;
    int64 occupied = p->red | p->yellow;
    // board representation:
    //
    //
    // 5 13 21 29 37 45 53
    // 4 12 20 28 36 44 52
    // 3 11 19 27 35 43 51
    // 2 10 18 26 34 42 50
    // 1  9 17 25 33 41 49
    // 0  8 16 24 32 40 48
    printf("\n");
    for (i = 5; i >= 0; i--) {
      for (j = 0; j < COLUMNS; j++) {
        if ((p->red) & (one << (i + 8 * j)))
          printf(" x ");
        if ((p->yellow) & (one << (i + 8 * j)))
          printf(" o ");
        if ((~occupied) & (one << (i + 8 * j)))
          printf(" . ");
      }
      printf("\n");
    }
    printf("\n");
    return 1;
  }

  int printboardtofile(POSITION *p, FILE *fp) {
    int64 one = 1L;
    int i, j;
    int64 occupied = p->red | p->yellow;
    // board representation:
    //
    //
    // 5 13 21 29 37 45 53
    // 4 12 20 28 36 44 52
    // 3 11 19 27 35 43 51
    // 2 10 18 26 34 42 50
    // 1  9 17 25 33 41 49
    // 0  8 16 24 32 40 48
    fprintf(fp, "\ny=%lu, r=%lu \n", p->yellow, p->red);
    for (i = 5; i >= 0; i--) {
      for (j = 0; j < COLUMNS; j++) {
        if ((p->red) & (one << (i + 8 * j)))
          fprintf(fp, " x ");
        if ((p->yellow) & (one << (i + 8 * j)))
          fprintf(fp, " o ");
        if ((~occupied) & (one << (i + 8 * j)))
          fprintf(fp, " . ");
      }
      fprintf(fp, "\n");
    }
    fprintf(fp, "%i to move (yellow %i, red %i)\n\n", p->color, YELLOW, RED);
    return 1;
  }

  int printmove(MOVE *m) {
    int64 one = 1;
    int i, j;
    // board representation:
    //
    //
    // 5 13 21 29 37 45 53
    // 4 12 20 28 36 44 52
    // 3 11 19 27 35 43 51
    // 2 10 18 26 34 42 50
    // 1  9 17 25 33 41 49
    // 0  8 16 24 32 40 48
    printf("\n");
    for (i = 5; i >= 0; i--) {
      for (j = 0; j < COLUMNS; j++) {
        if ((*m) & (one << (i + 8 * j)))
          printf(" * ");
        else
          printf(" . ");
      }
      printf("\n");
    }
    printf("\n");
    return 1;
  }

};

#endif //BITBULLY_AGENT4INAROW_H
