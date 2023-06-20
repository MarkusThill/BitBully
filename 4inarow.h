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

//int findmove(int board[7][6],int color,double maxtime, char out[255], int *play, int sw, int rl, int usebook, int gt, int dumb);
int findmove(int board[7][6], int color, double maxtime, char *out, int *play, int sw, int rl, int usebook, int gt,
             int evaltype);

int64 absolutehashkey(POSITION *p);

int bitcount(int64 a);

void boardtobitboard(int b[7][6], int color, POSITION *p);

int checkfour(POSITION *p);

int claimdraw(POSITION *p);

void domove(POSITION *p, MOVE *m);

int dumbevaluation(POSITION *p);

int evaluation(POSITION *p);

int fourinarow_newgame(POSITION *p);

int fourinarow_printboard(POSITION *p);

int iidnegamax(POSITION *p, int depth, int alpha, int beta, int *returnbestindex);

int init(void);

void logtofile(int depth, int value, int move);

int lsb(int64 a);

int makemove(POSITION *p, int column, MOVE *m);

int makemovelist(POSITION *p, MOVE m[COLUMNS]);

int movetocolumn(MOVE m);

int msb(int64 a);

int64 myrand(void);

int negamax(POSITION *p, int depth, int alpha, int beta);

int printboardtofile(POSITION *p, FILE *fp);

int printmove(MOVE *m);

void readlearnfile(void);

int retrieve(POSITION *p, int depth, int *alpha, int *beta, int *value, int *bestindex);

int search(POSITION *p, double time, char out[255], int searchwin, int theoretic);

void setthreats(POSITION *p);

void store(POSITION *p, int depth, int alpha, int beta, int bestvalue, int bestindex/*, int32 effort*/);

int stupidevaluation(POSITION *p);

int threatevaluate(int64 yt, int64 rt, int64 dt);

void threatstore(int64 yt, int64 rt, int value);

int threatretrieve(int64 yt, int64 rt, int *value);

void undomove(POSITION *p, MOVE *m);

int windowsearch(POSITION *p, int depth, int guess);

