#include <string>

#define SPIELER1 true
#define SPIELER2 false
#define HASHSIZE (4194304LL /2LL) //Beliebige Zweierpotenz. Maximal: 2^25   //Bei NICHT perfekter Stufe kann key auch auf 32 Bit gesetzt werden
#define LHASHSIZE (HASHSIZE / 8LL)
#define DRHASHSIZE 4096LL
#define EXACT 1
#define UPPER 2
#define LOWER 3


const short int ZugBewertung[42] =
        {7, 9, 11, 10, 8, 6,
         9, 13, 17, 16, 12, 8,
         11, 17, 23, 22, 16, 10,
         15, 21, 27, 26, 20, 14,
         11, 17, 23, 22, 16, 10,
         9, 13, 17, 16, 12, 8,
         7, 9, 11, 10, 8, 6};

const short int ZugBewertung0[6] = {7, 9, 11, 10, 8, 6};
const short int ZugBewertung1[6] = {9, 13, 17, 16, 12, 8};
const short int ZugBewertung2[6] = {11, 17, 23, 22, 16, 10};
const short int ZugBewertung3[6] = {15, 21, 27, 26, 20, 14};
const short int ZugBewertung4[6] = {11, 17, 23, 22, 16, 10};
const short int ZugBewertung5[6] = {9, 13, 17, 16, 12, 8};
const short int ZugBewertung6[6] = {7, 9, 11, 10, 8, 6};

const uint64_t spalte[7] =
        {0x3F000000000LL,
         0xFC0000000LL,
         0x3F000000LL,
         0xFC0000LL,
         0x3F000LL,
         0xFC0LL,
         0x3FLL};


const uint64_t zufall[2][42] =
        {2704506115994628LL,
         886077597871704230LL,
         49389502572435258LL,
         82333996817139652LL,
         263967204879563328LL,
         81975673952415600LL,
         73398886193708064LL,
         1595863713887220963LL,
         46261610206381440LL,
         58705059883690202LL,
         231696507446129885LL,
         22606427398883328LL,
         12595346104058426LL,
         4097820997223100LL,
         101324622437045280LL,
         31779374605300125LL,
         35633797708573350LL,
         22416112427922675LL,
         36363358841386824LL,
         4173667863902779612LL,
         9131869703157656LL,
         14138249969764235LL,
         214348955873908032LL,
         58547228054472360LL,
         55740094356093127LL,
         1777939723684020LL,
         362858203316162568LL,
         28975890403315160LL,
         1242349240448115806LL,
         59601464106441712LL,
         9110872168202946LL,
         10631234269963860LL,
         16888881020037981LL,
         1159792823631456LL,
         36205525950397736LL,
         47068546447093808LL,
         375817237236357603LL,
         32189775283681470LL,
         1493718293429439LL,
         20793138156733824LL,
         101478045365676084LL,
         110552240521049760LL,
         19298943901485610LL,
         6548220796761019LL,
         1777628907278452803LL,
         11891769178478592LL,
         3564258696970080LL,
         236708853179436288LL,
         349182760342233125LL,
         429289086240375LL,
         121921717543355343LL,
         31495429917193824LL,
         5694462647075520LL,
         30758051047680284LL,
         1365712501364505581LL,
         17363511325679223LL,
         119226791868480LL,
         6220173073409360LL,
         11647770880598424LL,
         24507207907919492LL,
         551903736927872LL,
         2097977134396858LL,
         3108717381973636075LL,
         25389306976498643LL,
         254362479754036508LL,
         119080142037405540LL,
         65628472867223040LL,
         116416206906490816LL,
         130703539652185785LL,
         1541174198942728362LL,
         37852277734190556LL,
         22426187114508354LL,
         290694253237906321LL,
         3460150747465650LL,
         12108862858045894LL,
         124792798959719156LL,
         6572334569141376LL,
         2726416766762766LL,
         378828340783306008LL,
         72087612995472200LL,
         113983283880328672LL,
         376285078915283LL,
         62397498210717124LL,
         1193066389676430LL};

const uint64_t zufall10[7] =
        {0LL,
         2704506115994628LL,
         886077597871704230LL,
         49389502572435258LL,
         82333996817139652LL,
         263967204879563328LL,
         81975673952415600LL};

const uint64_t zufall11[7] =
        {0LL,
         73398886193708064LL,
         1595863713887220963LL,
         46261610206381440LL,
         58705059883690202LL,
         231696507446129885LL,
         22606427398883328LL};

const uint64_t zufall12[7] =
        {0LL,
         12595346104058426LL,
         4097820997223100LL,
         101324622437045280LL,
         31779374605300125LL,
         35633797708573350LL,
         22416112427922675LL};

const uint64_t zufall13[7] =
        {0LL,
         36363358841386824LL,
         4173667863902779612LL,
         9131869703157656LL,
         14138249969764235LL,
         214348955873908032LL,
         58547228054472360LL};

const uint64_t zufall14[7] =
        {0LL,
         55740094356093127LL,
         1777939723684020LL,
         362858203316162568LL,
         28975890403315160LL,
         1242349240448115806LL,
         59601464106441712LL};

const uint64_t zufall15[7] =
        {0LL,
         9110872168202946LL,
         10631234269963860LL,
         16888881020037981LL,
         1159792823631456LL,
         36205525950397736LL,
         47068546447093808LL};

const uint64_t zufall16[7] =
        {0LL,
         375817237236357603LL,
         32189775283681470LL,
         1493718293429439LL,
         20793138156733824LL,
         101478045365676084LL,
         110552240521049760LL};

const uint64_t zufall20[7] =
        {0LL,
         19298943901485610LL,
         6548220796761019LL,
         1777628907278452803LL,
         11891769178478592LL,
         3564258696970080LL,
         236708853179436288LL};

const uint64_t zufall21[7] =
        {0LL,
         349182760342233125LL,
         429289086240375LL,
         121921717543355343LL,
         31495429917193824LL,
         5694462647075520LL,
         30758051047680284LL};

const uint64_t zufall22[7] =
        {0LL,
         1365712501364505581LL,
         17363511325679223LL,
         119226791868480LL,
         6220173073409360LL,
         11647770880598424LL,
         24507207907919492LL};

const uint64_t zufall23[7] =
        {0LL,
         551903736927872LL,
         2097977134396858LL,
         3108717381973636075LL,
         25389306976498643LL,
         254362479754036508LL,
         119080142037405540LL};

const uint64_t zufall24[7] =
        {0LL,
         65628472867223040LL,
         116416206906490816LL,
         130703539652185785LL,
         1541174198942728362LL,
         37852277734190556LL,
         22426187114508354LL};

const uint64_t zufall25[7] =
        {0LL,
         290694253237906321LL,
         3460150747465650LL,
         12108862858045894LL,
         124792798959719156LL,
         6572334569141376LL,
         2726416766762766LL};

const uint64_t zufall26[7] =
        {0LL,
         378828340783306008LL,
         72087612995472200LL,
         113983283880328672LL,
         376285078915283LL,
         62397498210717124LL,
         1193066389676430LL};


#include <cstdint>

const uint64_t Feldwert0[6] =
        {2199023255552ULL,
         1099511627776ULL,
         549755813888ULL,
         274877906944ULL,
         137438953472ULL,
         68719476736ULL};

const uint64_t Feldwert1[6] =
        {34359738368ULL,
         17179869184ULL,
         8589934592ULL,
         4294967296ULL,
         2147483648ULL,
         1073741824ULL};

const uint64_t Feldwert2[6] =
        {536870912ULL,
         268435456ULL,
         134217728ULL,
         67108864ULL,
         33554432ULL,
         16777216ULL};

const uint64_t Feldwert3[6] =
        {8388608ULL,
         4194304ULL,
         2097152ULL,
         1048576ULL,
         524288ULL,
         262144ULL};

const uint64_t Feldwert4[6] =
        {131072ULL,
         65536ULL,
         32768ULL,
         16384ULL,
         8192ULL,
         4096ULL};

const uint64_t Feldwert5[6] =
        {2048ULL,
         1024ULL,
         512ULL,
         256ULL,
         128ULL,
         64ULL};

const uint64_t Feldwert6[6] =
        {32ULL,
         16ULL,
         8ULL,
         4ULL,
         2ULL,
         1ULL};

const uint64_t Feldwert[7][6] =
        {2199023255552ULL,
         1099511627776ULL,
         549755813888ULL,
         274877906944ULL,
         137438953472ULL,
         68719476736ULL,
         34359738368ULL,
         17179869184ULL,
         8589934592ULL,
         4294967296ULL,
         2147483648ULL,
         1073741824ULL,
         536870912ULL,
         268435456ULL,
         134217728ULL,
         67108864ULL,
         33554432ULL,
         16777216ULL,
         8388608ULL,
         4194304ULL,
         2097152ULL,
         1048576ULL,
         524288ULL,
         262144ULL,
         131072ULL,
         65536ULL,
         32768ULL,
         16384ULL,
         8192ULL,
         4096ULL,
         2048ULL,
         1024ULL,
         512ULL,
         256ULL,
         128ULL,
         64ULL,
         32ULL,
         16ULL,
         8ULL,
         4ULL,
         2ULL,
         1ULL};

const uint64_t Feldwertinv0[6] =
        {18446741874686296063ULL,
         18446742974197923839ULL,
         18446743523953737727ULL,
         18446743798831644671ULL,
         18446743936270598143ULL,
         18446744004990074879ULL};

const uint64_t Feldwertinv1[6] =
        {18446744039349813247ULL,
         18446744056529682431ULL,
         18446744065119617023ULL,
         18446744069414584319ULL,
         18446744071562067967ULL,
         18446744072635809791ULL};

const uint64_t Feldwertinv2[6] =
        {18446744073172680703ULL,
         18446744073441116159ULL,
         18446744073575333887ULL,
         18446744073642442751ULL,
         18446744073675997183ULL,
         18446744073692774399ULL};

const uint64_t Feldwertinv3[6] =
        {18446744073701163007ULL,
         18446744073705357311ULL,
         18446744073707454463ULL,
         18446744073708503039ULL,
         18446744073709027327ULL,
         18446744073709289471ULL};

const uint64_t Feldwertinv4[6] =
        {18446744073709420543ULL,
         18446744073709486079ULL,
         18446744073709518847ULL,
         18446744073709535231ULL,
         18446744073709543423ULL,
         18446744073709547519ULL};

const uint64_t Feldwertinv5[6] =
        {18446744073709549567ULL,
         18446744073709550591ULL,
         18446744073709551103ULL,
         18446744073709551359ULL,
         18446744073709551487ULL,
         18446744073709551551ULL};

const uint64_t Feldwertinv6[6] =
        {18446744073709551583ULL,
         18446744073709551599ULL,
         18446744073709551607ULL,
         18446744073709551611ULL,
         18446744073709551613ULL,
         18446744073709551614ULL};

const uint64_t Feldwertinv[7][6] =
        {18446741874686296063ULL,
         18446742974197923839ULL,
         18446743523953737727ULL,
         18446743798831644671ULL,
         18446743936270598143ULL,
         18446744004990074879ULL,
         18446744039349813247ULL,
         18446744056529682431ULL,
         18446744065119617023ULL,
         18446744069414584319ULL,
         18446744071562067967ULL,
         18446744072635809791ULL,
         18446744073172680703ULL,
         18446744073441116159ULL,
         18446744073575333887ULL,
         18446744073642442751ULL,
         18446744073675997183ULL,
         18446744073692774399ULL,
         18446744073701163007ULL,
         18446744073705357311ULL,
         18446744073707454463ULL,
         18446744073708503039ULL,
         18446744073709027327ULL,
         18446744073709289471ULL,
         18446744073709420543ULL,
         18446744073709486079ULL,
         18446744073709518847ULL,
         18446744073709535231ULL,
         18446744073709543423ULL,
         18446744073709547519ULL,
         18446744073709549567ULL,
         18446744073709550591ULL,
         18446744073709551103ULL,
         18446744073709551359ULL,
         18446744073709551487ULL,
         18446744073709551551ULL,
         18446744073709551583ULL,
         18446744073709551599ULL,
         18446744073709551607ULL,
         18446744073709551611ULL,
         18446744073709551613ULL,
         18446744073709551614ULL};


uint64_t Feld1, Feld2;
uint64_t *FeldData1;
uint64_t *FeldData2;
short *Win;
short MaxInstance;
short InstanceVoll;
//short rueck;
short Hoehe[7];

int book[34287];

struct HASHEvorlage {
    uint64_t key;
    short value;
    char flag;
} *HASHE, *LHASHE;

struct DRVorlage {
    uint64_t key;
    short value;
} *DRHASHE;

struct OPENINGvorlage {
    uint64_t FeldData1;
    uint64_t FeldData2;
    short Win;                     //WARUM UM ALLES IN DER WELT NEHME ICH HIER SHORT, WENN BYTE DOCH VÖLLIG AUSREICHT
} *OPENING;


bool SpielBeenden(bool spieler);

bool SpielBeenden1(void);

bool SpielBeenden2(void);

bool SpielBeenden1Generate(short *moves);

bool SpielBeenden2Generate(short *moves);

short FindThreat1(void);

short FindThreat2(void);

short FindEvenThreat1(void);

short FindEvenThreat2(void);

short FindOddThreat1(void);

short FindOddThreat2(void);

short int FindOtherThreat1(const short x);

short int FindOtherThreat2(const short x);

short int FindOtherEvenThreat1(const short x);

short int FindOtherEvenThreat2(const short x);

short int FindOtherOddThreat1(const short x);

short int FindOtherOddThreat2(const short x);

short SpielBaum1Generate(const short instance, short alpha, short beta, const uint64_t ZKey, bool symM);

short SpielBaum2Generate(const short instance, short alpha, short beta, const uint64_t ZKey, bool symM);

bool Gewinnstellung(const short x, const short yy, bool spieler);

bool Gewinnstellung1(const short x, const short yy);

bool Gewinnstellung2(const short x, const short yy);

int BewertungComputerAnziehender(void);

short WurzelMethodeComputerAnziehender(short instance, short alpha, short beta, const uint64_t ZKey);

short Spielbaum1ComputerAnziehender(const short instance, short alpha, short beta, const uint64_t ZKey);

short Spielbaum2ComputerAnziehender(const short instance, short alpha, short beta, const uint64_t ZKey);

short FreieSpalten(short *spalten);

short SiegerErmitteln1(const short *x);

short SiegerErmitteln2(const short *s);

int64_t GewinnstellungReihe(const short x, const short y, bool spieler);

int SpielBeendenStellung(bool spieler);

inline void SteinSetzen(int x, bool spieler);

inline void SteinLoeschen(int x, bool spieler);

int BrettCount(void);

void Reset(void);

void ResetHash(void);

uint64_t HoeheErmitteln(void);

void BuchLaden(std::string pfad);

short WurzelMethodeStartComputerAnziehender(const short instance, short alpha, short beta, const uint64_t ZKey);

short SpielbaumStart1ComputerAnziehender(const short instance, short alpha, short beta, const uint64_t ZKey);

short SpielbaumStart2ComputerAnziehender(const short instance, short alpha, short beta, const uint64_t ZKey);

bool IsNumber(const std::string &text);

void Reset(void) {
    /*Setzt das Spielfeld zurück. Dafür müssen die beiden Variablen Feld1 und Feld2
    auf 0 gesetzt werden und die Höhe für jede Spalte muss auf 0 gesetzt werden.*/
    Feld1 = 0LL;
    Feld2 = 0LL;
    for (int i = 0; i < 7; i++)
        Hoehe[i] = 0;
}

void ResetHash(void) {
    for (long i = 0; i < HASHSIZE; i++) {
        if (HASHE[i].key != 0LL) {
            //if((Feld1 & HASHE[i].key)!= HoeheErmitteln())
            {
                HASHE[i].key = 0LL;
                HASHE[i].value = 0;
                HASHE[i].flag = 0;
                if (i < LHASHSIZE) {
                    LHASHE[i].key = 0LL;
                    LHASHE[i].value = 0;
                    LHASHE[i].flag = 0;
                }
            }
        }
    }
}

uint64_t HoeheErmitteln() {
    int i, j, stelle;
    int64_t temp;
    uint64_t ZobristKey = 0LL;
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 6; j++) {
            stelle = 41 - (i * 6 + j);
            temp = (1LL << stelle);
            if ((Feld1 & temp) == temp)
                ZobristKey ^= zufall[0][i * 6 + j];
            else if ((Feld2 & temp) == temp)
                ZobristKey ^= zufall[1][i * 6 + j];
            else
                break;
        }
        Hoehe[i] = j;
    }
    return ZobristKey;
}

uint64_t toZobrist(uint64_t f1, uint64_t f2) {
    int i, j, stelle;
    int64_t temp;
    uint64_t ZobristKey = 0LL;
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 6; j++) {
            stelle = 41 - (i * 6 + j);
            temp = (1LL << stelle);
            if ((f1 & temp) == temp)
                ZobristKey ^= zufall[0][i * 6 + j];
            else if ((f2 & temp) == temp)
                ZobristKey ^= zufall[1][i * 6 + j];
            else
                break;
        }
    }
    return ZobristKey;
}

uint64_t spiegel(uint64_t f) {
    uint64_t mirrored = 0LL;
    mirrored |= ((f & spalte[0]) >> 36);
    mirrored |= ((f & spalte[1]) >> 24);
    mirrored |= ((f & spalte[2]) >> 12);
    mirrored |= (f & spalte[3]);
    mirrored |= ((f & spalte[4]) << 12);
    mirrored |= ((f & spalte[5]) << 24);
    mirrored |= ((f & spalte[6]) << 36);
    return mirrored;
}

inline void SteinSetzen(int x, bool spieler) {
    /*Setzt einen Stein im Bitboard für den betreffenden Spieler*/
    int stelle = 41 - (x * 6 + Hoehe[x]++); //Das Bit, was im Bitboard gesetzt werden muss
    if (spieler == SPIELER1)
        Feld1 |= 1LL << stelle;
    else
        Feld2 |= 1LL << stelle;
}

inline void SteinLoeschen(int x, bool spieler) {
    /*Löscht einen Stein im Bitboard für den betreffenden Spieler*/
    int stelle = 41 - (--Hoehe[x] + x * 6); //Das Bit, was im Bitboard gelöscht werden muss
    if (spieler == SPIELER1)
        Feld1 &= ~(1LL << stelle);
    else
        Feld2 &= ~(1LL << stelle);
}

int BrettCount() {
    /*Zählt die Steine, die auf dem Spielfeld gesetzt wurden und gibt die Anzahl zurück*/
    return (Hoehe[0] + Hoehe[1] + Hoehe[2] + Hoehe[3] + Hoehe[4] + Hoehe[5] + Hoehe[6]);
}

/*char DrohungenErmitteln()
{
	char x=0;
	int i,j;
	for(i=0;i<7;i++)
		for(j=Hoehe[i];j<6;j++)
			if(Gewinnstellung(i,j,SPIELER1) || Gewinnstellung(i,j,SPIELER2))
				x |= (1 << (6-i));
	return x;
}
*/

int istSymmetrisch() {
    uint64_t Feld1Gespiegelt = 0LL, Feld2Gespiegelt = 0LL;

    Feld1Gespiegelt |= ((Feld1 & spalte[0]) >> 36);
    Feld1Gespiegelt |= ((Feld1 & spalte[1]) >> 24);
    Feld1Gespiegelt |= ((Feld1 & spalte[2]) >> 12);
    Feld1Gespiegelt |= (Feld1 & spalte[3]);
    Feld1Gespiegelt |= ((Feld1 & spalte[4]) << 12);
    Feld1Gespiegelt |= ((Feld1 & spalte[5]) << 24);
    Feld1Gespiegelt |= ((Feld1 & spalte[6]) << 36);
    Feld2Gespiegelt |= ((Feld2 & spalte[0]) >> 36);
    Feld2Gespiegelt |= ((Feld2 & spalte[1]) >> 24);
    Feld2Gespiegelt |= ((Feld2 & spalte[2]) >> 12);
    Feld2Gespiegelt |= (Feld2 & spalte[3]);
    Feld2Gespiegelt |= ((Feld2 & spalte[4]) << 12);
    Feld2Gespiegelt |= ((Feld2 & spalte[5]) << 24);
    Feld2Gespiegelt |= ((Feld2 & spalte[6]) << 36);
    return (Feld1Gespiegelt == Feld1 && Feld2Gespiegelt == Feld2);
}

bool symMoeglich() {
    uint64_t Feld1Gespiegelt = 0LL, Feld2Gespiegelt = 0LL;
    Feld1Gespiegelt |= ((Feld1 & spalte[0]) >> 36);
    Feld1Gespiegelt |= ((Feld1 & spalte[1]) >> 24);
    Feld1Gespiegelt |= ((Feld1 & spalte[2]) >> 12);
    Feld1Gespiegelt |= (Feld1 & spalte[3]);
    Feld1Gespiegelt |= ((Feld1 & spalte[4]) << 12);
    Feld1Gespiegelt |= ((Feld1 & spalte[5]) << 24);
    Feld1Gespiegelt |= ((Feld1 & spalte[6]) << 36);
    Feld2Gespiegelt |= ((Feld2 & spalte[0]) >> 36);
    Feld2Gespiegelt |= ((Feld2 & spalte[1]) >> 24);
    Feld2Gespiegelt |= ((Feld2 & spalte[2]) >> 12);
    Feld2Gespiegelt |= (Feld2 & spalte[3]);
    Feld2Gespiegelt |= ((Feld2 & spalte[4]) << 12);
    Feld2Gespiegelt |= ((Feld2 & spalte[5]) << 24);
    Feld2Gespiegelt |= ((Feld2 & spalte[6]) << 36);
    return (!(Feld1Gespiegelt & Feld2) && !(Feld2Gespiegelt & Feld1));
}

bool GameOver = false;
int Modus;

void ModusEinrichten() {
    /*Es wird, je nachdem was f?r ein Sender ?bergeben wird, einer der Vier Modi eingerichtet.
    Es gibt die Modi Spieler-Spieler, Computer-Spieler, Spieler-Computer, Netzwerkspiel.*/
    Reset();        //Das Spielfeld wird zur?ckgesetzt.
    GameOver = false;

    Modus = 2;
    SteinSetzen(3, SPIELER2);
    if (MaxInstance % 2 == 1)
        MaxInstance++;
}

void ComputerAnziehender(int spalte) {
    /*Wird ausgef?hrt, wenn der Spieler auf ein Shape geklcikt hat und es sich um
    ein Computer-Mensch Spiel handelt. Es wird zun?chst gepr?ft, ob die Spalte, in die
    der Spieler gesetzt hat noch nicht voll ist. Dann wird der Stein f?r den Spieler gestezt
    und der Computer berechnet einen g?ltigen Zug. Die Berechnungs-Zeit wird gesoppt und ausgegeben!!!*/
    int x;
    if (GameOver) //Falls das Spiel bereits vorbei ist, soll der Spieler keinen Zug mehr machen d?rfen
        return;
    if (Hoehe[spalte] < 6 && Modus == 2) {
        SteinSetzen(spalte, SPIELER1);
        if (Gewinnstellung1(spalte, Hoehe[spalte] - 1))  //Falls der Spieler mit seinem Zug gewonnen hat
        {
            std::cout << "Sie Haben das Spiel gewonnen!!!";
            GameOver = true;
            return;
        }

        if ((Feld1 | Feld2) == 0x3FFFFFFFFFFLL)  //Falls das Spielfeld Voll ist
        {
            std::cout << "Das Spiel endet Unentschieden!!!";
            GameOver = true; //Damit keienr der beiden einen Stein mehr setzen kann
            return;
        }
        if (SpielBeenden2()) //Es wird vor dem Zug des Computers bereits geschaut, ob er gewinnen kann, wodurch dann nachher ein weiterer Zug des Spielers verhindert werden kann
            GameOver = true;        //Wird auf true gesetzt, da bei Gewinn das Spiel vorbei ist und der Spieler keine weiteren Z?ge machen k?nnen soll
        else
            std::cout << "Warten Sie bitte, das Programm rechnet...";
        if (BrettCount() < 8) {
            x = WurzelMethodeStartComputerAnziehender(0, -9999, 9999, HoeheErmitteln());
            SteinSetzen(x, SPIELER2);
        } else {
            InstanceVoll = 42 - BrettCount();
            MaxInstance = 100;
            HoeheErmitteln();
            x = WurzelMethodeComputerAnziehender(0, -9999, 9999, HoeheErmitteln());
            SteinSetzen(x, SPIELER2);
        }

        std::cout << "Sie sind an der Reihe!!!";
        if (GameOver) {
            std::cout << "Sie haben das Spiel leider verloren!!!";
        }
    }
}

std::string toString() {
    std::stringstream ss;
    ss << "\n";
    for (int r = 5; r >= 0; r--) {
        for (int c = 0; c < 7; c++) {
            if (Feld1 & Feldwert[c][r]) {
                ss << "x";
            } else if (Feld2 & Feldwert[c][r]) {
                ss << "o";
            } else {
                ss << ".";
            }
        }
        ss << "\n";
    }
    return ss.str();
}


void gMoves1(short *moves) {
    short cn[7], count, i, j = 0, index;

    switch (Hoehe[0]) {
        case 0:
            count = 7;
            if (Feld2 & 0x20408100000LL) count--;
            if (Feld2 & 0x20820800000LL) count--;
            if (Feld2 & 0x3C000000000LL) count--;
            break;
        case 1:
            count = 9;
            if (Feld2 & 0x10204080000LL) count--;
            if (Feld2 & 0x10410400000LL) count--;
            if (Feld2 & 0x3C000000000LL) count--;
            if (Feld2 & 0x1E000000000LL) count--;
            break;
        case 2:
            count = 11;
            if (Feld2 & 0x8102040000LL) count--;
            if (Feld2 & 0x8208200000LL) count--;
            if (Feld2 & 0x3C000000000LL) count--;
            if (Feld2 & 0x1E000000000LL) count--;
            if (Feld2 & 0xF000000000LL) count--;
            break;
        case 3:
            count = 10;
            if (Feld2 & 0x4104100000LL) count--;
            if (Feld2 & 0x4210800000LL) count--;
            if (Feld2 & 0x3C000000000LL) count--;
            if (Feld2 & 0x1E000000000LL) count--;
            if (Feld2 & 0xF000000000LL) count--;
            break;
        case 4:
            count = 8;
            if (Feld2 & 0x2082080000LL) count--;
            if (Feld2 & 0x2108400000LL) count--;
            if (Feld2 & 0x1E000000000LL) count--;
            if (Feld2 & 0xF000000000LL) count--;
            break;
        case 5:
            count = 6;
            if (Feld2 & 0x1041040000LL) count--;
            if (Feld2 & 0x1084200000LL) count--;
            if (Feld2 & 0xF000000000LL) count--;
            break;
        default:
            count = 0;
            break;
    }
    cn[0] = count;


    switch (Hoehe[1]) {
        case 0:
            count = 9;
            if (Feld2 & 0x810204000LL) count--;
            if (Feld2 & 0x20820800000LL) count--;
            if (Feld2 & 0x820820000LL) count--;
            if (Feld2 & 0xF00000000LL) count--;
            break;
        case 1:
            count = 13;
            if (Feld2 & 0x408102000LL) count--;
            if (Feld2 & 0x20408100000LL) count--;
            if (Feld2 & 0x10410400000LL) count--;
            if (Feld2 & 0x410410000LL) count--;
            if (Feld2 & 0xF00000000LL) count--;
            if (Feld2 & 0x780000000LL) count--;
            break;
        case 2:
            count = 17;
            if (Feld2 & 0x204081000LL) count--;
            if (Feld2 & 0x10204080000LL) count--;
            if (Feld2 & 0x8208200000LL) count--;
            if (Feld2 & 0x208208000LL) count--;
            if (Feld2 & 0x4210800000LL) count--;
            if (Feld2 & 0xF00000000LL) count--;
            if (Feld2 & 0x780000000LL) count--;
            if (Feld2 & 0x3C0000000LL) count--;
            break;
        case 3:
            count = 16;
            if (Feld2 & 0x8102040000LL) count--;
            if (Feld2 & 0x4104100000LL) count--;
            if (Feld2 & 0x104104000LL) count--;
            if (Feld2 & 0x2108400000LL) count--;
            if (Feld2 & 0x108420000LL) count--;
            if (Feld2 & 0xF00000000LL) count--;
            if (Feld2 & 0x780000000LL) count--;
            if (Feld2 & 0x3C0000000LL) count--;
            break;
        case 4:
            count = 12;
            if (Feld2 & 0x2082080000LL) count--;
            if (Feld2 & 0x82082000LL) count--;
            if (Feld2 & 0x1084200000LL) count--;
            if (Feld2 & 0x84210000LL) count--;
            if (Feld2 & 0x780000000LL) count--;
            if (Feld2 & 0x3C0000000LL) count--;
            break;
        case 5:
            count = 8;
            if (Feld2 & 0x1041040000LL) count--;
            if (Feld2 & 0x41041000LL) count--;
            if (Feld2 & 0x42108000LL) count--;
            if (Feld2 & 0x3C0000000LL) count--;
            break;
        default:
            count = 0;
            break;
    }
    cn[1] = count;

    switch (Hoehe[2]) {
        case 0:
            count = 11;
            if (Feld2 & 0x20408100LL) count--;
            if (Feld2 & 0x20820800000LL) count--;
            if (Feld2 & 0x820820000LL) count--;
            if (Feld2 & 0x20820800LL) count--;
            if (Feld2 & 0x3C000000LL) count--;
            break;
        case 1:
            count = 17;
            if (Feld2 & 0x10204080LL) count--;
            if (Feld2 & 0x4210800000LL) count--;
            if (Feld2 & 0x810204000LL) count--;
            if (Feld2 & 0x10410400000LL) count--;
            if (Feld2 & 0x410410000LL) count--;
            if (Feld2 & 0x10410400LL) count--;
            if (Feld2 & 0x3C000000LL) count--;
            if (Feld2 & 0x1E000000LL) count--;
            break;
        case 2:
            count = 23;
            if (Feld2 & 0x8102040LL) count--;
            if (Feld2 & 0x2108400000LL) count--;
            if (Feld2 & 0x408102000LL) count--;
            if (Feld2 & 0x8208200000LL) count--;
            if (Feld2 & 0x208208000LL) count--;
            if (Feld2 & 0x8208200LL) count--;
            if (Feld2 & 0x108420000LL) count--;
            if (Feld2 & 0x20408100000LL) count--;
            if (Feld2 & 0x3C000000LL) count--;
            if (Feld2 & 0x1E000000LL) count--;
            if (Feld2 & 0xF000000LL) count--;
            break;
        case 3:
            count = 22;
            if (Feld2 & 0x1084200000LL) count--;
            if (Feld2 & 0x204081000LL) count--;
            if (Feld2 & 0x4104100000LL) count--;
            if (Feld2 & 0x104104000LL) count--;
            if (Feld2 & 0x4104100LL) count--;
            if (Feld2 & 0x84210000LL) count--;
            if (Feld2 & 0x10204080000LL) count--;
            if (Feld2 & 0x4210800LL) count--;
            if (Feld2 & 0x3C000000LL) count--;
            if (Feld2 & 0x1E000000LL) count--;
            if (Feld2 & 0xF000000LL) count--;
            break;
        case 4:
            count = 16;
            if (Feld2 & 0x2082080000LL) count--;
            if (Feld2 & 0x82082000LL) count--;
            if (Feld2 & 0x2082080LL) count--;
            if (Feld2 & 0x42108000LL) count--;
            if (Feld2 & 0x8102040000LL) count--;
            if (Feld2 & 0x2108400LL) count--;
            if (Feld2 & 0x1E000000LL) count--;
            if (Feld2 & 0xF000000LL) count--;
            break;
        case 5:
            count = 10;
            if (Feld2 & 0x1041040000LL) count--;
            if (Feld2 & 0x41041000LL) count--;
            if (Feld2 & 0x1041040LL) count--;
            if (Feld2 & 0x1084200LL) count--;
            if (Feld2 & 0xF000000LL) count--;
            break;
        default:
            count = 0;
            break;
    }
    cn[2] = count;

    switch (Hoehe[3]) {
        case 0:
            count = 15;
            if (Feld2 & 0x4210800000LL) count--;
            if (Feld2 & 0x810204LL) count--;
            if (Feld2 & 0x20820800000LL) count--;
            if (Feld2 & 0x820820000LL) count--;
            if (Feld2 & 0x20820800LL) count--;
            if (Feld2 & 0x820820LL) count--;
            if (Feld2 & 0xF00000LL) count--;
            break;
        case 1:
            count = 21;
            if (Feld2 & 0x2108400000LL) count--;
            if (Feld2 & 0x408102LL) count--;
            if (Feld2 & 0x108420000LL) count--;
            if (Feld2 & 0x20408100LL) count--;
            if (Feld2 & 0x10410400000LL) count--;
            if (Feld2 & 0x410410000LL) count--;
            if (Feld2 & 0x10410400LL) count--;
            if (Feld2 & 0x410410LL) count--;
            if (Feld2 & 0xF00000LL) count--;
            if (Feld2 & 0x780000LL) count--;
            break;
        case 2:
            count = 27;
            if (Feld2 & 0x1084200000LL) count--;
            if (Feld2 & 0x204081LL) count--;
            if (Feld2 & 0x84210000LL) count--;
            if (Feld2 & 0x10204080LL) count--;
            if (Feld2 & 0x8208200000LL) count--;
            if (Feld2 & 0x208208000LL) count--;
            if (Feld2 & 0x8208200LL) count--;
            if (Feld2 & 0x208208LL) count--;
            if (Feld2 & 0x4210800LL) count--;
            if (Feld2 & 0x810204000LL) count--;
            if (Feld2 & 0xF00000LL) count--;
            if (Feld2 & 0x780000LL) count--;
            if (Feld2 & 0x3C0000LL) count--;
            break;
        case 3:
            count = 26;
            if (Feld2 & 0x42108000LL) count--;
            if (Feld2 & 0x8102040LL) count--;
            if (Feld2 & 0x4104100000LL) count--;
            if (Feld2 & 0x104104000LL) count--;
            if (Feld2 & 0x4104100LL) count--;
            if (Feld2 & 0x104104LL) count--;
            if (Feld2 & 0x2108400LL) count--;
            if (Feld2 & 0x408102000LL) count--;
            if (Feld2 & 0x108420LL) count--;
            if (Feld2 & 0x20408100000LL) count--;
            if (Feld2 & 0xF00000LL) count--;
            if (Feld2 & 0x780000LL) count--;
            if (Feld2 & 0x3C0000LL) count--;
            break;
        case 4:
            count = 20;
            if (Feld2 & 0x2082080000LL) count--;
            if (Feld2 & 0x82082000LL) count--;
            if (Feld2 & 0x2082080LL) count--;
            if (Feld2 & 0x82082LL) count--;
            if (Feld2 & 0x1084200LL) count--;
            if (Feld2 & 0x204081000LL) count--;
            if (Feld2 & 0x84210LL) count--;
            if (Feld2 & 0x10204080000LL) count--;
            if (Feld2 & 0x780000LL) count--;
            if (Feld2 & 0x3C0000LL) count--;
            break;
        case 5:
            count = 14;
            if (Feld2 & 0x1041040000LL) count--;
            if (Feld2 & 0x41041000LL) count--;
            if (Feld2 & 0x1041040LL) count--;
            if (Feld2 & 0x41041LL) count--;
            if (Feld2 & 0x42108LL) count--;
            if (Feld2 & 0x8102040000LL) count--;
            if (Feld2 & 0x3C0000LL) count--;
            break;
        default:
            count = 0;
            break;
    }
    cn[3] = count;

    switch (Hoehe[4]) {
        case 0:
            count = 11;
            if (Feld2 & 0x108420000LL) count--;
            if (Feld2 & 0x820820000LL) count--;
            if (Feld2 & 0x20820800LL) count--;
            if (Feld2 & 0x820820LL) count--;
            if (Feld2 & 0x3C000LL) count--;
            break;
        case 1:
            count = 17;
            if (Feld2 & 0x84210000LL) count--;
            if (Feld2 & 0x4210800LL) count--;
            if (Feld2 & 0x810204LL) count--;
            if (Feld2 & 0x410410000LL) count--;
            if (Feld2 & 0x10410400LL) count--;
            if (Feld2 & 0x410410LL) count--;
            if (Feld2 & 0x3C000LL) count--;
            if (Feld2 & 0x1E000LL) count--;
            break;
        case 2:
            count = 23;
            if (Feld2 & 0x42108000LL) count--;
            if (Feld2 & 0x2108400LL) count--;
            if (Feld2 & 0x408102LL) count--;
            if (Feld2 & 0x208208000LL) count--;
            if (Feld2 & 0x8208200LL) count--;
            if (Feld2 & 0x208208LL) count--;
            if (Feld2 & 0x108420LL) count--;
            if (Feld2 & 0x20408100LL) count--;
            if (Feld2 & 0x3C000LL) count--;
            if (Feld2 & 0x1E000LL) count--;
            if (Feld2 & 0xF000LL) count--;
            break;
        case 3:
            count = 22;
            if (Feld2 & 0x1084200LL) count--;
            if (Feld2 & 0x204081LL) count--;
            if (Feld2 & 0x104104000LL) count--;
            if (Feld2 & 0x4104100LL) count--;
            if (Feld2 & 0x104104LL) count--;
            if (Feld2 & 0x84210LL) count--;
            if (Feld2 & 0x10204080LL) count--;
            if (Feld2 & 0x810204000LL) count--;
            if (Feld2 & 0x3C000LL) count--;
            if (Feld2 & 0x1E000LL) count--;
            if (Feld2 & 0xF000LL) count--;
            break;
        case 4:
            count = 16;
            if (Feld2 & 0x82082000LL) count--;
            if (Feld2 & 0x2082080LL) count--;
            if (Feld2 & 0x82082LL) count--;
            if (Feld2 & 0x42108LL) count--;
            if (Feld2 & 0x8102040LL) count--;
            if (Feld2 & 0x408102000LL) count--;
            if (Feld2 & 0x1E000LL) count--;
            if (Feld2 & 0xF000LL) count--;
            break;
        case 5:
            count = 10;
            if (Feld2 & 0x41041000LL) count--;
            if (Feld2 & 0x1041040LL) count--;
            if (Feld2 & 0x41041LL) count--;
            if (Feld2 & 0x204081000LL) count--;
            if (Feld2 & 0xF000LL) count--;
            break;
        default:
            count = 0;
            break;
    }
    cn[4] = count;

    switch (Hoehe[5]) {
        case 0:
            count = 9;
            if (Feld2 & 0x4210800LL) count--;
            if (Feld2 & 0x20820800LL) count--;
            if (Feld2 & 0x820820LL) count--;
            if (Feld2 & 0xF00LL) count--;
            break;
        case 1:
            count = 13;
            if (Feld2 & 0x2108400LL) count--;
            if (Feld2 & 0x108420LL) count--;
            if (Feld2 & 0x10410400LL) count--;
            if (Feld2 & 0x410410LL) count--;
            if (Feld2 & 0xF00LL) count--;
            if (Feld2 & 0x780LL) count--;
            break;
        case 2:
            count = 17;
            if (Feld2 & 0x1084200LL) count--;
            if (Feld2 & 0x84210LL) count--;
            if (Feld2 & 0x8208200LL) count--;
            if (Feld2 & 0x208208LL) count--;
            if (Feld2 & 0x810204LL) count--;
            if (Feld2 & 0xF00LL) count--;
            if (Feld2 & 0x780LL) count--;
            if (Feld2 & 0x3C0LL) count--;
            break;
        case 3:
            count = 16;
            if (Feld2 & 0x42108LL) count--;
            if (Feld2 & 0x4104100LL) count--;
            if (Feld2 & 0x104104LL) count--;
            if (Feld2 & 0x408102LL) count--;
            if (Feld2 & 0x20408100LL) count--;
            if (Feld2 & 0xF00LL) count--;
            if (Feld2 & 0x780LL) count--;
            if (Feld2 & 0x3C0LL) count--;
            break;
        case 4:
            count = 12;
            if (Feld2 & 0x2082080LL) count--;
            if (Feld2 & 0x82082LL) count--;
            if (Feld2 & 0x204081LL) count--;
            if (Feld2 & 0x10204080LL) count--;
            if (Feld2 & 0x780LL) count--;
            if (Feld2 & 0x3C0LL) count--;
            break;
        case 5:
            count = 8;
            if (Feld2 & 0x1041040LL) count--;
            if (Feld2 & 0x41041LL) count--;
            if (Feld2 & 0x8102040LL) count--;
            if (Feld2 & 0x3C0LL) count--;
            break;
        default:
            count = 0;
            break;
    }
    cn[5] = count;

    switch (Hoehe[6]) {
        case 0:
            count = 7;
            if (Feld2 & 0x108420LL) count--;
            if (Feld2 & 0x820820LL) count--;
            if (Feld2 & 0x3CLL) count--;
            break;
        case 1:
            count = 9;
            if (Feld2 & 0x84210LL) count--;
            if (Feld2 & 0x410410LL) count--;
            if (Feld2 & 0x3CLL) count--;
            if (Feld2 & 0x1ELL) count--;
            break;
        case 2:
            count = 11;
            if (Feld2 & 0x42108LL) count--;
            if (Feld2 & 0x208208LL) count--;
            if (Feld2 & 0x3CLL) count--;
            if (Feld2 & 0x1ELL) count--;
            if (Feld2 & 0xFLL) count--;
            break;
        case 3:
            count = 10;
            if (Feld2 & 0x104104LL) count--;
            if (Feld2 & 0x810204LL) count--;
            if (Feld2 & 0x3CLL) count--;
            if (Feld2 & 0x1ELL) count--;
            if (Feld2 & 0xFLL) count--;
            break;
        case 4:
            count = 8;
            if (Feld2 & 0x82082LL) count--;
            if (Feld2 & 0x408102LL) count--;
            if (Feld2 & 0x1ELL) count--;
            if (Feld2 & 0xFLL) count--;
            break;
        case 5:
            count = 6;
            if (Feld2 & 0x41041LL) count--;
            if (Feld2 & 0x204081LL) count--;
            if (Feld2 & 0xFLL) count--;
            break;
        default:
            count = 0;
            break;
    }
    cn[6] = count;

    do {
        count = 0;
        for (i = 6; i >= 0; i--) {
            if (cn[i] > count) {
                count = cn[i];
                index = i;
            }
        }
        cn[index] = 0;
        moves[j++] = index;
    } while (count);
    moves[j - 1] = -1;
}

void gMoves2(short *moves) {
    short cn[7], count, i, j = 0, index;

    switch (Hoehe[0]) {
        case 0:
            count = 7;
            if (Feld1 & 0x20408100000LL) count--;
            if (Feld1 & 0x20820800000LL) count--;
            if (Feld1 & 0x3C000000000LL) count--;
            break;
        case 1:
            count = 9;
            if (Feld1 & 0x10204080000LL) count--;
            if (Feld1 & 0x10410400000LL) count--;
            if (Feld1 & 0x3C000000000LL) count--;
            if (Feld1 & 0x1E000000000LL) count--;
            break;
        case 2:
            count = 11;
            if (Feld1 & 0x8102040000LL) count--;
            if (Feld1 & 0x8208200000LL) count--;
            if (Feld1 & 0x3C000000000LL) count--;
            if (Feld1 & 0x1E000000000LL) count--;
            if (Feld1 & 0xF000000000LL) count--;
            break;
        case 3:
            count = 10;
            if (Feld1 & 0x4104100000LL) count--;
            if (Feld1 & 0x4210800000LL) count--;
            if (Feld1 & 0x3C000000000LL) count--;
            if (Feld1 & 0x1E000000000LL) count--;
            if (Feld1 & 0xF000000000LL) count--;
            break;
        case 4:
            count = 8;
            if (Feld1 & 0x2082080000LL) count--;
            if (Feld1 & 0x2108400000LL) count--;
            if (Feld1 & 0x1E000000000LL) count--;
            if (Feld1 & 0xF000000000LL) count--;
            break;
        case 5:
            count = 6;
            if (Feld1 & 0x1041040000LL) count--;
            if (Feld1 & 0x1084200000LL) count--;
            if (Feld1 & 0xF000000000LL) count--;
            break;
        default:
            count = 0;
            break;
    }
    cn[0] = count;


    switch (Hoehe[1]) {
        case 0:
            count = 9;
            if (Feld1 & 0x810204000LL) count--;
            if (Feld1 & 0x20820800000LL) count--;
            if (Feld1 & 0x820820000LL) count--;
            if (Feld1 & 0xF00000000LL) count--;
            break;
        case 1:
            count = 13;
            if (Feld1 & 0x408102000LL) count--;
            if (Feld1 & 0x20408100000LL) count--;
            if (Feld1 & 0x10410400000LL) count--;
            if (Feld1 & 0x410410000LL) count--;
            if (Feld1 & 0xF00000000LL) count--;
            if (Feld1 & 0x780000000LL) count--;
            break;
        case 2:
            count = 17;
            if (Feld1 & 0x204081000LL) count--;
            if (Feld1 & 0x10204080000LL) count--;
            if (Feld1 & 0x8208200000LL) count--;
            if (Feld1 & 0x208208000LL) count--;
            if (Feld1 & 0x4210800000LL) count--;
            if (Feld1 & 0xF00000000LL) count--;
            if (Feld1 & 0x780000000LL) count--;
            if (Feld1 & 0x3C0000000LL) count--;
            break;
        case 3:
            count = 16;
            if (Feld1 & 0x8102040000LL) count--;
            if (Feld1 & 0x4104100000LL) count--;
            if (Feld1 & 0x104104000LL) count--;
            if (Feld1 & 0x2108400000LL) count--;
            if (Feld1 & 0x108420000LL) count--;
            if (Feld1 & 0xF00000000LL) count--;
            if (Feld1 & 0x780000000LL) count--;
            if (Feld1 & 0x3C0000000LL) count--;
            break;
        case 4:
            count = 12;
            if (Feld1 & 0x2082080000LL) count--;
            if (Feld1 & 0x82082000LL) count--;
            if (Feld1 & 0x1084200000LL) count--;
            if (Feld1 & 0x84210000LL) count--;
            if (Feld1 & 0x780000000LL) count--;
            if (Feld1 & 0x3C0000000LL) count--;
            break;
        case 5:
            count = 8;
            if (Feld1 & 0x1041040000LL) count--;
            if (Feld1 & 0x41041000LL) count--;
            if (Feld1 & 0x42108000LL) count--;
            if (Feld1 & 0x3C0000000LL) count--;
            break;
        default:
            count = 0;
            break;
    }
    cn[1] = count;

    switch (Hoehe[2]) {
        case 0:
            count = 11;
            if (Feld1 & 0x20408100LL) count--;
            if (Feld1 & 0x20820800000LL) count--;
            if (Feld1 & 0x820820000LL) count--;
            if (Feld1 & 0x20820800LL) count--;
            if (Feld1 & 0x3C000000LL) count--;
            break;
        case 1:
            count = 17;
            if (Feld1 & 0x10204080LL) count--;
            if (Feld1 & 0x4210800000LL) count--;
            if (Feld1 & 0x810204000LL) count--;
            if (Feld1 & 0x10410400000LL) count--;
            if (Feld1 & 0x410410000LL) count--;
            if (Feld1 & 0x10410400LL) count--;
            if (Feld1 & 0x3C000000LL) count--;
            if (Feld1 & 0x1E000000LL) count--;
            break;
        case 2:
            count = 23;
            if (Feld1 & 0x8102040LL) count--;
            if (Feld1 & 0x2108400000LL) count--;
            if (Feld1 & 0x408102000LL) count--;
            if (Feld1 & 0x8208200000LL) count--;
            if (Feld1 & 0x208208000LL) count--;
            if (Feld1 & 0x8208200LL) count--;
            if (Feld1 & 0x108420000LL) count--;
            if (Feld1 & 0x20408100000LL) count--;
            if (Feld1 & 0x3C000000LL) count--;
            if (Feld1 & 0x1E000000LL) count--;
            if (Feld1 & 0xF000000LL) count--;
            break;
        case 3:
            count = 22;
            if (Feld1 & 0x1084200000LL) count--;
            if (Feld1 & 0x204081000LL) count--;
            if (Feld1 & 0x4104100000LL) count--;
            if (Feld1 & 0x104104000LL) count--;
            if (Feld1 & 0x4104100LL) count--;
            if (Feld1 & 0x84210000LL) count--;
            if (Feld1 & 0x10204080000LL) count--;
            if (Feld1 & 0x4210800LL) count--;
            if (Feld1 & 0x3C000000LL) count--;
            if (Feld1 & 0x1E000000LL) count--;
            if (Feld1 & 0xF000000LL) count--;
            break;
        case 4:
            count = 16;
            if (Feld1 & 0x2082080000LL) count--;
            if (Feld1 & 0x82082000LL) count--;
            if (Feld1 & 0x2082080LL) count--;
            if (Feld1 & 0x42108000LL) count--;
            if (Feld1 & 0x8102040000LL) count--;
            if (Feld1 & 0x2108400LL) count--;
            if (Feld1 & 0x1E000000LL) count--;
            if (Feld1 & 0xF000000LL) count--;
            break;
        case 5:
            count = 10;
            if (Feld1 & 0x1041040000LL) count--;
            if (Feld1 & 0x41041000LL) count--;
            if (Feld1 & 0x1041040LL) count--;
            if (Feld1 & 0x1084200LL) count--;
            if (Feld1 & 0xF000000LL) count--;
            break;
        default:
            count = 0;
            break;
    }
    cn[2] = count;

    switch (Hoehe[3]) {
        case 0:
            count = 15;
            if (Feld1 & 0x4210800000LL) count--;
            if (Feld1 & 0x810204LL) count--;
            if (Feld1 & 0x20820800000LL) count--;
            if (Feld1 & 0x820820000LL) count--;
            if (Feld1 & 0x20820800LL) count--;
            if (Feld1 & 0x820820LL) count--;
            if (Feld1 & 0xF00000LL) count--;
            break;
        case 1:
            count = 21;
            if (Feld1 & 0x2108400000LL) count--;
            if (Feld1 & 0x408102LL) count--;
            if (Feld1 & 0x108420000LL) count--;
            if (Feld1 & 0x20408100LL) count--;
            if (Feld1 & 0x10410400000LL) count--;
            if (Feld1 & 0x410410000LL) count--;
            if (Feld1 & 0x10410400LL) count--;
            if (Feld1 & 0x410410LL) count--;
            if (Feld1 & 0xF00000LL) count--;
            if (Feld1 & 0x780000LL) count--;
            break;
        case 2:
            count = 27;
            if (Feld1 & 0x1084200000LL) count--;
            if (Feld1 & 0x204081LL) count--;
            if (Feld1 & 0x84210000LL) count--;
            if (Feld1 & 0x10204080LL) count--;
            if (Feld1 & 0x8208200000LL) count--;
            if (Feld1 & 0x208208000LL) count--;
            if (Feld1 & 0x8208200LL) count--;
            if (Feld1 & 0x208208LL) count--;
            if (Feld1 & 0x4210800LL) count--;
            if (Feld1 & 0x810204000LL) count--;
            if (Feld1 & 0xF00000LL) count--;
            if (Feld1 & 0x780000LL) count--;
            if (Feld1 & 0x3C0000LL) count--;
            break;
        case 3:
            count = 26;
            if (Feld1 & 0x42108000LL) count--;
            if (Feld1 & 0x8102040LL) count--;
            if (Feld1 & 0x4104100000LL) count--;
            if (Feld1 & 0x104104000LL) count--;
            if (Feld1 & 0x4104100LL) count--;
            if (Feld1 & 0x104104LL) count--;
            if (Feld1 & 0x2108400LL) count--;
            if (Feld1 & 0x408102000LL) count--;
            if (Feld1 & 0x108420LL) count--;
            if (Feld1 & 0x20408100000LL) count--;
            if (Feld1 & 0xF00000LL) count--;
            if (Feld1 & 0x780000LL) count--;
            if (Feld1 & 0x3C0000LL) count--;
            break;
        case 4:
            count = 20;
            if (Feld1 & 0x2082080000LL) count--;
            if (Feld1 & 0x82082000LL) count--;
            if (Feld1 & 0x2082080LL) count--;
            if (Feld1 & 0x82082LL) count--;
            if (Feld1 & 0x1084200LL) count--;
            if (Feld1 & 0x204081000LL) count--;
            if (Feld1 & 0x84210LL) count--;
            if (Feld1 & 0x10204080000LL) count--;
            if (Feld1 & 0x780000LL) count--;
            if (Feld1 & 0x3C0000LL) count--;
            break;
        case 5:
            count = 14;
            if (Feld1 & 0x1041040000LL) count--;
            if (Feld1 & 0x41041000LL) count--;
            if (Feld1 & 0x1041040LL) count--;
            if (Feld1 & 0x41041LL) count--;
            if (Feld1 & 0x42108LL) count--;
            if (Feld1 & 0x8102040000LL) count--;
            if (Feld1 & 0x3C0000LL) count--;
            break;
        default:
            count = 0;
            break;
    }
    cn[3] = count;

    switch (Hoehe[4]) {
        case 0:
            count = 11;
            if (Feld1 & 0x108420000LL) count--;
            if (Feld1 & 0x820820000LL) count--;
            if (Feld1 & 0x20820800LL) count--;
            if (Feld1 & 0x820820LL) count--;
            if (Feld1 & 0x3C000LL) count--;
            break;
        case 1:
            count = 17;
            if (Feld1 & 0x84210000LL) count--;
            if (Feld1 & 0x4210800LL) count--;
            if (Feld1 & 0x810204LL) count--;
            if (Feld1 & 0x410410000LL) count--;
            if (Feld1 & 0x10410400LL) count--;
            if (Feld1 & 0x410410LL) count--;
            if (Feld1 & 0x3C000LL) count--;
            if (Feld1 & 0x1E000LL) count--;
            break;
        case 2:
            count = 23;
            if (Feld1 & 0x42108000LL) count--;
            if (Feld1 & 0x2108400LL) count--;
            if (Feld1 & 0x408102LL) count--;
            if (Feld1 & 0x208208000LL) count--;
            if (Feld1 & 0x8208200LL) count--;
            if (Feld1 & 0x208208LL) count--;
            if (Feld1 & 0x108420LL) count--;
            if (Feld1 & 0x20408100LL) count--;
            if (Feld1 & 0x3C000LL) count--;
            if (Feld1 & 0x1E000LL) count--;
            if (Feld1 & 0xF000LL) count--;
            break;
        case 3:
            count = 22;
            if (Feld1 & 0x1084200LL) count--;
            if (Feld1 & 0x204081LL) count--;
            if (Feld1 & 0x104104000LL) count--;
            if (Feld1 & 0x4104100LL) count--;
            if (Feld1 & 0x104104LL) count--;
            if (Feld1 & 0x84210LL) count--;
            if (Feld1 & 0x10204080LL) count--;
            if (Feld1 & 0x810204000LL) count--;
            if (Feld1 & 0x3C000LL) count--;
            if (Feld1 & 0x1E000LL) count--;
            if (Feld1 & 0xF000LL) count--;
            break;
        case 4:
            count = 16;
            if (Feld1 & 0x82082000LL) count--;
            if (Feld1 & 0x2082080LL) count--;
            if (Feld1 & 0x82082LL) count--;
            if (Feld1 & 0x42108LL) count--;
            if (Feld1 & 0x8102040LL) count--;
            if (Feld1 & 0x408102000LL) count--;
            if (Feld1 & 0x1E000LL) count--;
            if (Feld1 & 0xF000LL) count--;
            break;
        case 5:
            count = 10;
            if (Feld1 & 0x41041000LL) count--;
            if (Feld1 & 0x1041040LL) count--;
            if (Feld1 & 0x41041LL) count--;
            if (Feld1 & 0x204081000LL) count--;
            if (Feld1 & 0xF000LL) count--;
            break;
        default:
            count = 0;
            break;
    }
    cn[4] = count;

    switch (Hoehe[5]) {
        case 0:
            count = 9;
            if (Feld1 & 0x4210800LL) count--;
            if (Feld1 & 0x20820800LL) count--;
            if (Feld1 & 0x820820LL) count--;
            if (Feld1 & 0xF00LL) count--;
            break;
        case 1:
            count = 13;
            if (Feld1 & 0x2108400LL) count--;
            if (Feld1 & 0x108420LL) count--;
            if (Feld1 & 0x10410400LL) count--;
            if (Feld1 & 0x410410LL) count--;
            if (Feld1 & 0xF00LL) count--;
            if (Feld1 & 0x780LL) count--;
            break;
        case 2:
            count = 17;
            if (Feld1 & 0x1084200LL) count--;
            if (Feld1 & 0x84210LL) count--;
            if (Feld1 & 0x8208200LL) count--;
            if (Feld1 & 0x208208LL) count--;
            if (Feld1 & 0x810204LL) count--;
            if (Feld1 & 0xF00LL) count--;
            if (Feld1 & 0x780LL) count--;
            if (Feld1 & 0x3C0LL) count--;
            break;
        case 3:
            count = 16;
            if (Feld1 & 0x42108LL) count--;
            if (Feld1 & 0x4104100LL) count--;
            if (Feld1 & 0x104104LL) count--;
            if (Feld1 & 0x408102LL) count--;
            if (Feld1 & 0x20408100LL) count--;
            if (Feld1 & 0xF00LL) count--;
            if (Feld1 & 0x780LL) count--;
            if (Feld1 & 0x3C0LL) count--;
            break;
        case 4:
            count = 12;
            if (Feld1 & 0x2082080LL) count--;
            if (Feld1 & 0x82082LL) count--;
            if (Feld1 & 0x204081LL) count--;
            if (Feld1 & 0x10204080LL) count--;
            if (Feld1 & 0x780LL) count--;
            if (Feld1 & 0x3C0LL) count--;
            break;
        case 5:
            count = 8;
            if (Feld1 & 0x1041040LL) count--;
            if (Feld1 & 0x41041LL) count--;
            if (Feld1 & 0x8102040LL) count--;
            if (Feld1 & 0x3C0LL) count--;
            break;
        default:
            count = 0;
            break;
    }
    cn[5] = count;

    switch (Hoehe[6]) {
        case 0:
            count = 7;
            if (Feld1 & 0x108420LL) count--;
            if (Feld1 & 0x820820LL) count--;
            if (Feld1 & 0x3CLL) count--;
            break;
        case 1:
            count = 9;
            if (Feld1 & 0x84210LL) count--;
            if (Feld1 & 0x410410LL) count--;
            if (Feld1 & 0x3CLL) count--;
            if (Feld1 & 0x1ELL) count--;
            break;
        case 2:
            count = 11;
            if (Feld1 & 0x42108LL) count--;
            if (Feld1 & 0x208208LL) count--;
            if (Feld1 & 0x3CLL) count--;
            if (Feld1 & 0x1ELL) count--;
            if (Feld1 & 0xFLL) count--;
            break;
        case 3:
            count = 10;
            if (Feld1 & 0x104104LL) count--;
            if (Feld1 & 0x810204LL) count--;
            if (Feld1 & 0x3CLL) count--;
            if (Feld1 & 0x1ELL) count--;
            if (Feld1 & 0xFLL) count--;
            break;
        case 4:
            count = 8;
            if (Feld1 & 0x82082LL) count--;
            if (Feld1 & 0x408102LL) count--;
            if (Feld1 & 0x1ELL) count--;
            if (Feld1 & 0xFLL) count--;
            break;
        case 5:
            count = 6;
            if (Feld1 & 0x41041LL) count--;
            if (Feld1 & 0x204081LL) count--;
            if (Feld1 & 0xFLL) count--;
            break;
        default:
            count = 0;
            break;
    }
    cn[6] = count;

    do {
        count = 0;
        for (i = 6; i >= 0; i--) {
            if (cn[i] > count) {
                count = cn[i];
                index = i;
            }
        }
        cn[index] = 0;
        moves[j++] = index;
    } while (count);
    moves[j - 1] = -1;
}

#include <cstdint>

bool Gewinnstellung1(const short x, const short yy) {
    /*Überprüft, ob der übergebene Spieler gewonnen hätte, wenn er in
    das übergebene Loch setzen würde*/
    const int y = Feld1;
    switch (x * 6 +
            yy) //X und Y Position in eine feste Position(0-41) umgerechnet, da die switch-anweisung schneller ausgeführt wird
    {
        case 0:
            if ((0x408100000LL & Feld1) == 0x408100000LL) return true;
            if ((0x820800000LL & Feld1) == 0x820800000LL) return true;
            break;
        case 1:
            if ((0x410400000LL & Feld1) == 0x410400000LL) return true;
            if ((0x204080000LL & Feld1) == 0x204080000LL) return true;
            break;
        case 2:
            if ((0x208200000LL & Feld1) == 0x208200000LL) return true;
            if ((0x102040000LL & Feld1) == 0x102040000LL) return true;
            break;
        case 3:
            if ((0x38000000000LL & Feld1) == 0x38000000000LL) return true;
            if ((0x210800000LL & Feld1) == 0x210800000LL) return true;
            if ((0x104100000LL & Feld1) == 0x104100000LL) return true;
            break;
        case 4:
            if ((0x108400000LL & Feld1) == 0x108400000LL) return true;
            if ((0x1C000000000LL & Feld1) == 0x1C000000000LL) return true;
            if ((0x82080000 & y) == 0x82080000) return true;
            break;
        case 5:
            if ((0x84200000 & y) == 0x84200000) return true;
            if ((0xE000000000LL & Feld1) == 0xE000000000LL) return true;
            if ((0x41040000 & y) == 0x41040000) return true;
            break;
        case 6:
            if ((0x10204000 & y) == 0x10204000) return true;
            if ((0x20820000 & y) == 0x20820000) return true;
            if ((0x20020800000LL & Feld1) == 0x20020800000LL) return true;
            break;
        case 7:
            if ((0x10010400000LL & Feld1) == 0x10010400000LL) return true;
            if ((0x20008100000LL & Feld1) == 0x20008100000LL) return true;
            if ((0x10410000 & y) == 0x10410000) return true;
            if ((0x8102000 & y) == 0x8102000) return true;
            break;
        case 8:
            if ((0x8008200000LL & Feld1) == 0x8008200000LL) return true;
            if ((0x4010800000LL & Feld1) == 0x4010800000LL) return true;
            if ((0x8208000 & y) == 0x8208000) return true;
            if ((0x10004080000LL & Feld1) == 0x10004080000LL) return true;
            if ((0x4081000 & y) == 0x4081000) return true;
            break;
        case 9:
            if ((0x8420000 & y) == 0x8420000) return true;
            if ((0xE00000000LL & Feld1) == 0xE00000000LL) return true;
            if ((0x4104000 & y) == 0x4104000) return true;
            if ((0x4004100000LL & Feld1) == 0x4004100000LL) return true;
            if ((0x8002040000LL & Feld1) == 0x8002040000LL) return true;
            if ((0x2008400000LL & Feld1) == 0x2008400000LL) return true;
            break;
        case 10:
            if ((0x4210000 & y) == 0x4210000) return true;
            if ((0x2082000 & y) == 0x2082000) return true;
            if ((0x700000000LL & Feld1) == 0x700000000LL) return true;
            if ((0x2002080000LL & Feld1) == 0x2002080000LL) return true;
            if ((0x1004200000LL & Feld1) == 0x1004200000LL) return true;
            break;
        case 11:
            if ((0x2108000 & y) == 0x2108000) return true;
            if ((0x1041000 & y) == 0x1041000) return true;
            if ((0x380000000LL & Feld1) == 0x380000000LL) return true;
            if ((0x1001040000LL & Feld1) == 0x1001040000LL) return true;
            break;
        case 12:
            if ((0x408100 & y) == 0x408100) return true;
            if ((0x20800800000LL & Feld1) == 0x20800800000LL) return true;
            if ((0x820800 & y) == 0x820800) return true;
            if ((0x800820000LL & Feld1) == 0x800820000LL) return true;
            break;
        case 13:
            if ((0x4200800000LL & Feld1) == 0x4200800000LL) return true;
            if ((0x10400400000LL & Feld1) == 0x10400400000LL) return true;
            if ((0x800204000LL & Feld1) == 0x800204000LL) return true;
            if ((0x410400 & y) == 0x410400) return true;
            if ((0x400410000LL & Feld1) == 0x400410000LL) return true;
            if ((0x204080 & y) == 0x204080) return true;
            break;
        case 14:
            if ((0x100420000LL & Feld1) == 0x100420000LL) return true;
            if ((0x8200200000LL & Feld1) == 0x8200200000LL) return true;
            if ((0x20400100000LL & Feld1) == 0x20400100000LL) return true;
            if ((0x200208000LL & Feld1) == 0x200208000LL) return true;
            if ((0x208200 & y) == 0x208200) return true;
            if ((0x400102000LL & Feld1) == 0x400102000LL) return true;
            if ((0x2100400000LL & Feld1) == 0x2100400000LL) return true;
            if ((0x102040 & y) == 0x102040) return true;
            break;
        case 15:
            if ((0x210800 & y) == 0x210800) return true;
            if ((0x10200080000LL & Feld1) == 0x10200080000LL) return true;
            if ((0x4100100000LL & Feld1) == 0x4100100000LL) return true;
            if ((0x100104000LL & Feld1) == 0x100104000LL) return true;
            if ((0x200081000LL & Feld1) == 0x200081000LL) return true;
            if ((0x80210000 & y) == 0x80210000) return true;
            if ((0x38000000 & y) == 0x38000000) return true;
            if ((0x104100 & y) == 0x104100) return true;
            if ((0x1080200000LL & Feld1) == 0x1080200000LL) return true;
            break;
        case 16:
            if ((0x108400 & y) == 0x108400) return true;
            if ((0x8100040000LL & Feld1) == 0x8100040000LL) return true;
            if ((0x80082000 & y) == 0x80082000) return true;
            if ((0x40108000 & y) == 0x40108000) return true;
            if ((0x82080 & y) == 0x82080) return true;
            if ((0x1C000000 & y) == 0x1C000000) return true;
            if ((0x2080080000LL & Feld1) == 0x2080080000LL) return true;
            break;
        case 17:
            if ((0x84200 & y) == 0x84200) return true;
            if ((0xE000000 & y) == 0xE000000) return true;
            if ((0x40041000 & y) == 0x40041000) return true;
            if ((0x41040 & y) == 0x41040) return true;
            if ((0x1040040000LL & Feld1) == 0x1040040000LL) return true;
            break;
        case 18:
            if ((0x20820000000LL & Feld1) == 0x20820000000LL) return true;
            if ((0x820020000LL & Feld1) == 0x820020000LL) return true;
            if ((0x20020800 & y) == 0x20020800) return true;
            if ((0x20820 & y) == 0x20820) return true;
            if ((0x10204 & y) == 0x10204) return true;
            if ((0x4210000000LL & Feld1) == 0x4210000000LL) return true;
            break;
        case 19:
            if ((0x20008100 & y) == 0x20008100) return true;
            if ((0x10410 & y) == 0x10410) return true;
            if ((0x10410000000LL & Feld1) == 0x10410000000LL) return true;
            if ((0x2108000000LL & Feld1) == 0x2108000000LL) return true;
            if ((0x108020000LL & Feld1) == 0x108020000LL) return true;
            if ((0x10010400 & y) == 0x10010400) return true;
            if ((0x8102 & y) == 0x8102) return true;
            if ((0x410010000LL & Feld1) == 0x410010000LL) return true;
            break;
        case 20:
            if ((0x810004000LL & Feld1) == 0x810004000LL) return true;
            if ((0x4010800 & y) == 0x4010800) return true;
            if ((0x8208000000LL & Feld1) == 0x8208000000LL) return true;
            if ((0x208008000LL & Feld1) == 0x208008000LL) return true;
            if ((0x8008200 & y) == 0x8008200) return true;
            if ((0x8208 & y) == 0x8208) return true;
            if ((0x84010000 & y) == 0x84010000) return true;
            if ((0x10004080 & y) == 0x10004080) return true;
            if ((0x4081 & y) == 0x4081) return true;
            if ((0x1084000000LL & Feld1) == 0x1084000000LL) return true;
            break;
        case 21:
            if ((0x20408000000LL & Feld1) == 0x20408000000LL) return true;
            if ((0x8420 & y) == 0x8420) return true;
            if ((0xE00000 & y) == 0xE00000) return true;
            if ((0x4104000000LL & Feld1) == 0x4104000000LL) return true;
            if ((0x2008400 & y) == 0x2008400) return true;
            if ((0x408002000LL & Feld1) == 0x408002000LL) return true;
            if ((0x4004100 & y) == 0x4004100) return true;
            if ((0x104004000LL & Feld1) == 0x104004000LL) return true;
            if ((0x42008000 & y) == 0x42008000) return true;
            if ((0x8002040 & y) == 0x8002040) return true;
            if ((0x4104 & y) == 0x4104) return true;
            break;
        case 22:
            if ((0x10204000000LL & Feld1) == 0x10204000000LL) return true;
            if ((0x700000 & y) == 0x700000) return true;
            if ((0x4210 & y) == 0x4210) return true;
            if ((0x204001000LL & Feld1) == 0x204001000LL) return true;
            if ((0x2082000000LL & Feld1) == 0x2082000000LL) return true;
            if ((0x1004200 & y) == 0x1004200) return true;
            if ((0x82002000 & y) == 0x82002000) return true;
            if ((0x2002080 & y) == 0x2002080) return true;
            if ((0x2082 & y) == 0x2082) return true;
            break;
        case 23:
            if ((0x380000 & y) == 0x380000) return true;
            if ((0x8102000000LL & Feld1) == 0x8102000000LL) return true;
            if ((0x2108 & y) == 0x2108) return true;
            if ((0x1041000000LL & Feld1) == 0x1041000000LL) return true;
            if ((0x41001000 & y) == 0x41001000) return true;
            if ((0x1001040 & y) == 0x1001040) return true;
            if ((0x1041 & y) == 0x1041) return true;
            break;
        case 24:
            if ((0x108400000LL & Feld1) == 0x108400000LL) return true;
            if ((0x800820 & y) == 0x800820) return true;
            if ((0x20800800 & y) == 0x20800800) return true;
            if ((0x820800000LL & Feld1) == 0x820800000LL) return true;
            break;
        case 25:
            if ((0x800204 & y) == 0x800204) return true;
            if ((0x4200800 & y) == 0x4200800) return true;
            if ((0x400410 & y) == 0x400410) return true;
            if ((0x10400400 & y) == 0x10400400) return true;
            if ((0x410400000LL & Feld1) == 0x410400000LL) return true;
            if ((0x84200000 & y) == 0x84200000) return true;
            break;
        case 26:
            if ((0x100420 & y) == 0x100420) return true;
            if ((0x20400100 & y) == 0x20400100) return true;
            if ((0x8200200 & y) == 0x8200200) return true;
            if ((0x208200000LL & Feld1) == 0x208200000LL) return true;
            if ((0x200208 & y) == 0x200208) return true;
            if ((0x2100400 & y) == 0x2100400) return true;
            if ((0x400102 & y) == 0x400102) return true;
            if ((0x42100000 & y) == 0x42100000) return true;
            break;
        case 27:
            if ((0x810200000LL & Feld1) == 0x810200000LL) return true;
            if ((0x80210 & y) == 0x80210) return true;
            if ((0x38000 & y) == 0x38000) return true;
            if ((0x4100100 & y) == 0x4100100) return true;
            if ((0x104100000LL & Feld1) == 0x104100000LL) return true;
            if ((0x10200080 & y) == 0x10200080) return true;
            if ((0x1080200 & y) == 0x1080200) return true;
            if ((0x100104 & y) == 0x100104) return true;
            if ((0x200081 & y) == 0x200081) return true;
            break;
        case 28:
            if ((0x408100000LL & Feld1) == 0x408100000LL) return true;
            if ((0x40108 & y) == 0x40108) return true;
            if ((0x1C000 & y) == 0x1C000) return true;
            if ((0x2080080 & y) == 0x2080080) return true;
            if ((0x82080000 & y) == 0x82080000) return true;
            if ((0x8100040 & y) == 0x8100040) return true;
            if ((0x80082 & y) == 0x80082) return true;
            break;
        case 29:
            if ((0x204080000LL & Feld1) == 0x204080000LL) return true;
            if ((0xE000 & y) == 0xE000) return true;
            if ((0x1040040 & y) == 0x1040040) return true;
            if ((0x41040000 & y) == 0x41040000) return true;
            if ((0x40041 & y) == 0x40041) return true;
            break;
        case 30:
            if ((0x4210000 & y) == 0x4210000) return true;
            if ((0x20820000 & y) == 0x20820000) return true;
            if ((0x820020 & y) == 0x820020) return true;
            break;
        case 31:
            if ((0x10410000 & y) == 0x10410000) return true;
            if ((0x410010 & y) == 0x410010) return true;
            if ((0x108020 & y) == 0x108020) return true;
            if ((0x2108000 & y) == 0x2108000) return true;
            break;
        case 32:
            if ((0x8208000 & y) == 0x8208000) return true;
            if ((0x208008 & y) == 0x208008) return true;
            if ((0x84010 & y) == 0x84010) return true;
            if ((0x810004 & y) == 0x810004) return true;
            if ((0x1084000 & y) == 0x1084000) return true;
            break;
        case 33:
            if ((0x20408000 & y) == 0x20408000) return true;
            if ((0x4104000 & y) == 0x4104000) return true;
            if ((0xE00 & y) == 0xE00) return true;
            if ((0x104004 & y) == 0x104004) return true;
            if ((0x42008 & y) == 0x42008) return true;
            if ((0x408002 & y) == 0x408002) return true;
            break;
        case 34:
            if ((0x10204000 & y) == 0x10204000) return true;
            if ((0x2082000 & y) == 0x2082000) return true;
            if ((0x700 & y) == 0x700) return true;
            if ((0x82002 & y) == 0x82002) return true;
            if ((0x204001 & y) == 0x204001) return true;
            break;
        case 35:
            if ((0x8102000 & y) == 0x8102000) return true;
            if ((0x1041000 & y) == 0x1041000) return true;
            if ((0x380 & y) == 0x380) return true;
            if ((0x41001 & y) == 0x41001) return true;
            break;
        case 36:
            if ((0x108400 & y) == 0x108400) return true;
            if ((0x820800 & y) == 0x820800) return true;
            break;
        case 37:
            if ((0x410400 & y) == 0x410400) return true;
            if ((0x84200 & y) == 0x84200) return true;
            break;
        case 38:
            if ((0x208200 & y) == 0x208200) return true;
            if ((0x42100 & y) == 0x42100) return true;
            break;
        case 39:
            if ((0x810200 & y) == 0x810200) return true;
            if ((0x38 & y) == 0x38) return true;
            if ((0x104100 & y) == 0x104100) return true;
            break;
        case 40:
            if ((0x408100 & y) == 0x408100) return true;
            if ((0x1C & y) == 0x1C) return true;
            if ((0x82080 & y) == 0x82080) return true;
            break;
        case 41:
            if ((0x204080 & y) == 0x204080) return true;
            if ((0xE & y) == 0xE) return true;
            if ((0x41040 & y) == 0x41040) return true;
            break;
    }
    return false;
}


bool Gewinnstellung2(const short x, const short yy) {
    /*Überprüft, ob der übergebene Spieler gewonnen hätte, wenn er in
    das übergebene Loch setzen würde*/
    const int y = Feld2;
    switch (x * 6 +
            yy) //X und Y Position in eine feste Position(0-41) umgerechnet, da die switch-anweisung schneller ausgeführt wird
    {
        case 0:
            if ((0x408100000LL & Feld2) == 0x408100000LL) return true;
            if ((0x820800000LL & Feld2) == 0x820800000LL) return true;
            break;
        case 1:
            if ((0x410400000LL & Feld2) == 0x410400000LL) return true;
            if ((0x204080000LL & Feld2) == 0x204080000LL) return true;
            break;
        case 2:
            if ((0x208200000LL & Feld2) == 0x208200000LL) return true;
            if ((0x102040000LL & Feld2) == 0x102040000LL) return true;
            break;
        case 3:
            if ((0x38000000000LL & Feld2) == 0x38000000000LL) return true;
            if ((0x210800000LL & Feld2) == 0x210800000LL) return true;
            if ((0x104100000LL & Feld2) == 0x104100000LL) return true;
            break;
        case 4:
            if ((0x108400000LL & Feld2) == 0x108400000LL) return true;
            if ((0x1C000000000LL & Feld2) == 0x1C000000000LL) return true;
            if ((0x82080000 & y) == 0x82080000) return true;
            break;
        case 5:
            if ((0x84200000 & y) == 0x84200000) return true;
            if ((0xE000000000LL & Feld2) == 0xE000000000LL) return true;
            if ((0x41040000 & y) == 0x41040000) return true;
            break;
        case 6:
            if ((0x10204000 & y) == 0x10204000) return true;
            if ((0x20820000 & y) == 0x20820000) return true;
            if ((0x20020800000LL & Feld2) == 0x20020800000LL) return true;
            break;
        case 7:
            if ((0x10010400000LL & Feld2) == 0x10010400000LL) return true;
            if ((0x20008100000LL & Feld2) == 0x20008100000LL) return true;
            if ((0x10410000 & y) == 0x10410000) return true;
            if ((0x8102000 & y) == 0x8102000) return true;
            break;
        case 8:
            if ((0x8008200000LL & Feld2) == 0x8008200000LL) return true;
            if ((0x4010800000LL & Feld2) == 0x4010800000LL) return true;
            if ((0x8208000 & y) == 0x8208000) return true;
            if ((0x10004080000LL & Feld2) == 0x10004080000LL) return true;
            if ((0x4081000 & y) == 0x4081000) return true;
            break;
        case 9:
            if ((0x8420000 & y) == 0x8420000) return true;
            if ((0xE00000000LL & Feld2) == 0xE00000000LL) return true;
            if ((0x4104000 & y) == 0x4104000) return true;
            if ((0x4004100000LL & Feld2) == 0x4004100000LL) return true;
            if ((0x8002040000LL & Feld2) == 0x8002040000LL) return true;
            if ((0x2008400000LL & Feld2) == 0x2008400000LL) return true;
            break;
        case 10:
            if ((0x4210000 & y) == 0x4210000) return true;
            if ((0x2082000 & y) == 0x2082000) return true;
            if ((0x700000000LL & Feld2) == 0x700000000LL) return true;
            if ((0x2002080000LL & Feld2) == 0x2002080000LL) return true;
            if ((0x1004200000LL & Feld2) == 0x1004200000LL) return true;
            break;
        case 11:
            if ((0x2108000 & y) == 0x2108000) return true;
            if ((0x1041000 & y) == 0x1041000) return true;
            if ((0x380000000LL & Feld2) == 0x380000000LL) return true;
            if ((0x1001040000LL & Feld2) == 0x1001040000LL) return true;
            break;
        case 12:
            if ((0x408100 & y) == 0x408100) return true;
            if ((0x20800800000LL & Feld2) == 0x20800800000LL) return true;
            if ((0x820800 & y) == 0x820800) return true;
            if ((0x800820000LL & Feld2) == 0x800820000LL) return true;
            break;
        case 13:
            if ((0x4200800000LL & Feld2) == 0x4200800000LL) return true;
            if ((0x10400400000LL & Feld2) == 0x10400400000LL) return true;
            if ((0x800204000LL & Feld2) == 0x800204000LL) return true;
            if ((0x410400 & y) == 0x410400) return true;
            if ((0x400410000LL & Feld2) == 0x400410000LL) return true;
            if ((0x204080 & y) == 0x204080) return true;
            break;
        case 14:
            if ((0x100420000LL & Feld2) == 0x100420000LL) return true;
            if ((0x8200200000LL & Feld2) == 0x8200200000LL) return true;
            if ((0x20400100000LL & Feld2) == 0x20400100000LL) return true;
            if ((0x200208000LL & Feld2) == 0x200208000LL) return true;
            if ((0x208200 & y) == 0x208200) return true;
            if ((0x400102000LL & Feld2) == 0x400102000LL) return true;
            if ((0x2100400000LL & Feld2) == 0x2100400000LL) return true;
            if ((0x102040 & y) == 0x102040) return true;
            break;
        case 15:
            if ((0x210800 & y) == 0x210800) return true;
            if ((0x10200080000LL & Feld2) == 0x10200080000LL) return true;
            if ((0x4100100000LL & Feld2) == 0x4100100000LL) return true;
            if ((0x100104000LL & Feld2) == 0x100104000LL) return true;
            if ((0x200081000LL & Feld2) == 0x200081000LL) return true;
            if ((0x80210000 & y) == 0x80210000) return true;
            if ((0x38000000 & y) == 0x38000000) return true;
            if ((0x104100 & y) == 0x104100) return true;
            if ((0x1080200000LL & Feld2) == 0x1080200000LL) return true;
            break;
        case 16:
            if ((0x108400 & y) == 0x108400) return true;
            if ((0x8100040000LL & Feld2) == 0x8100040000LL) return true;
            if ((0x80082000 & y) == 0x80082000) return true;
            if ((0x40108000 & y) == 0x40108000) return true;
            if ((0x82080 & y) == 0x82080) return true;
            if ((0x1C000000 & y) == 0x1C000000) return true;
            if ((0x2080080000LL & Feld2) == 0x2080080000LL) return true;
            break;
        case 17:
            if ((0x84200 & y) == 0x84200) return true;
            if ((0xE000000 & y) == 0xE000000) return true;
            if ((0x40041000 & y) == 0x40041000) return true;
            if ((0x41040 & y) == 0x41040) return true;
            if ((0x1040040000LL & Feld2) == 0x1040040000LL) return true;
            break;
        case 18:
            if ((0x20820000000LL & Feld2) == 0x20820000000LL) return true;
            if ((0x820020000LL & Feld2) == 0x820020000LL) return true;
            if ((0x20020800 & y) == 0x20020800) return true;
            if ((0x20820 & y) == 0x20820) return true;
            if ((0x10204 & y) == 0x10204) return true;
            if ((0x4210000000LL & Feld2) == 0x4210000000LL) return true;
            break;
        case 19:
            if ((0x20008100 & y) == 0x20008100) return true;
            if ((0x10410 & y) == 0x10410) return true;
            if ((0x10410000000LL & Feld2) == 0x10410000000LL) return true;
            if ((0x2108000000LL & Feld2) == 0x2108000000LL) return true;
            if ((0x108020000LL & Feld2) == 0x108020000LL) return true;
            if ((0x10010400 & y) == 0x10010400) return true;
            if ((0x8102 & y) == 0x8102) return true;
            if ((0x410010000LL & Feld2) == 0x410010000LL) return true;
            break;
        case 20:
            if ((0x810004000LL & Feld2) == 0x810004000LL) return true;
            if ((0x4010800 & y) == 0x4010800) return true;
            if ((0x8208000000LL & Feld2) == 0x8208000000LL) return true;
            if ((0x208008000LL & Feld2) == 0x208008000LL) return true;
            if ((0x8008200 & y) == 0x8008200) return true;
            if ((0x8208 & y) == 0x8208) return true;
            if ((0x84010000 & y) == 0x84010000) return true;
            if ((0x10004080 & y) == 0x10004080) return true;
            if ((0x4081 & y) == 0x4081) return true;
            if ((0x1084000000LL & Feld2) == 0x1084000000LL) return true;
            break;
        case 21:
            if ((0x20408000000LL & Feld2) == 0x20408000000LL) return true;
            if ((0x8420 & y) == 0x8420) return true;
            if ((0xE00000 & y) == 0xE00000) return true;
            if ((0x4104000000LL & Feld2) == 0x4104000000LL) return true;
            if ((0x2008400 & y) == 0x2008400) return true;
            if ((0x408002000LL & Feld2) == 0x408002000LL) return true;
            if ((0x4004100 & y) == 0x4004100) return true;
            if ((0x104004000LL & Feld2) == 0x104004000LL) return true;
            if ((0x42008000 & y) == 0x42008000) return true;
            if ((0x8002040 & y) == 0x8002040) return true;
            if ((0x4104 & y) == 0x4104) return true;
            break;
        case 22:
            if ((0x10204000000LL & Feld2) == 0x10204000000LL) return true;
            if ((0x700000 & y) == 0x700000) return true;
            if ((0x4210 & y) == 0x4210) return true;
            if ((0x204001000LL & Feld2) == 0x204001000LL) return true;
            if ((0x2082000000LL & Feld2) == 0x2082000000LL) return true;
            if ((0x1004200 & y) == 0x1004200) return true;
            if ((0x82002000 & y) == 0x82002000) return true;
            if ((0x2002080 & y) == 0x2002080) return true;
            if ((0x2082 & y) == 0x2082) return true;
            break;
        case 23:
            if ((0x380000 & y) == 0x380000) return true;
            if ((0x8102000000LL & Feld2) == 0x8102000000LL) return true;
            if ((0x2108 & y) == 0x2108) return true;
            if ((0x1041000000LL & Feld2) == 0x1041000000LL) return true;
            if ((0x41001000 & y) == 0x41001000) return true;
            if ((0x1001040 & y) == 0x1001040) return true;
            if ((0x1041 & y) == 0x1041) return true;
            break;
        case 24:
            if ((0x108400000LL & Feld2) == 0x108400000LL) return true;
            if ((0x800820 & y) == 0x800820) return true;
            if ((0x20800800 & y) == 0x20800800) return true;
            if ((0x820800000LL & Feld2) == 0x820800000LL) return true;
            break;
        case 25:
            if ((0x800204 & y) == 0x800204) return true;
            if ((0x4200800 & y) == 0x4200800) return true;
            if ((0x400410 & y) == 0x400410) return true;
            if ((0x10400400 & y) == 0x10400400) return true;
            if ((0x410400000LL & Feld2) == 0x410400000LL) return true;
            if ((0x84200000 & y) == 0x84200000) return true;
            break;
        case 26:
            if ((0x100420 & y) == 0x100420) return true;
            if ((0x20400100 & y) == 0x20400100) return true;
            if ((0x8200200 & y) == 0x8200200) return true;
            if ((0x208200000LL & Feld2) == 0x208200000LL) return true;
            if ((0x200208 & y) == 0x200208) return true;
            if ((0x2100400 & y) == 0x2100400) return true;
            if ((0x400102 & y) == 0x400102) return true;
            if ((0x42100000 & y) == 0x42100000) return true;
            break;
        case 27:
            if ((0x810200000LL & Feld2) == 0x810200000LL) return true;
            if ((0x80210 & y) == 0x80210) return true;
            if ((0x38000 & y) == 0x38000) return true;
            if ((0x4100100 & y) == 0x4100100) return true;
            if ((0x104100000LL & Feld2) == 0x104100000LL) return true;
            if ((0x10200080 & y) == 0x10200080) return true;
            if ((0x1080200 & y) == 0x1080200) return true;
            if ((0x100104 & y) == 0x100104) return true;
            if ((0x200081 & y) == 0x200081) return true;
            break;
        case 28:
            if ((0x408100000LL & Feld2) == 0x408100000LL) return true;
            if ((0x40108 & y) == 0x40108) return true;
            if ((0x1C000 & y) == 0x1C000) return true;
            if ((0x2080080 & y) == 0x2080080) return true;
            if ((0x82080000 & y) == 0x82080000) return true;
            if ((0x8100040 & y) == 0x8100040) return true;
            if ((0x80082 & y) == 0x80082) return true;
            break;
        case 29:
            if ((0x204080000LL & Feld2) == 0x204080000LL) return true;
            if ((0xE000 & y) == 0xE000) return true;
            if ((0x1040040 & y) == 0x1040040) return true;
            if ((0x41040000 & y) == 0x41040000) return true;
            if ((0x40041 & y) == 0x40041) return true;
            break;
        case 30:
            if ((0x4210000 & y) == 0x4210000) return true;
            if ((0x20820000 & y) == 0x20820000) return true;
            if ((0x820020 & y) == 0x820020) return true;
            break;
        case 31:
            if ((0x10410000 & y) == 0x10410000) return true;
            if ((0x410010 & y) == 0x410010) return true;
            if ((0x108020 & y) == 0x108020) return true;
            if ((0x2108000 & y) == 0x2108000) return true;
            break;
        case 32:
            if ((0x8208000 & y) == 0x8208000) return true;
            if ((0x208008 & y) == 0x208008) return true;
            if ((0x84010 & y) == 0x84010) return true;
            if ((0x810004 & y) == 0x810004) return true;
            if ((0x1084000 & y) == 0x1084000) return true;
            break;
        case 33:
            if ((0x20408000 & y) == 0x20408000) return true;
            if ((0x4104000 & y) == 0x4104000) return true;
            if ((0xE00 & y) == 0xE00) return true;
            if ((0x104004 & y) == 0x104004) return true;
            if ((0x42008 & y) == 0x42008) return true;
            if ((0x408002 & y) == 0x408002) return true;
            break;
        case 34:
            if ((0x10204000 & y) == 0x10204000) return true;
            if ((0x2082000 & y) == 0x2082000) return true;
            if ((0x700 & y) == 0x700) return true;
            if ((0x82002 & y) == 0x82002) return true;
            if ((0x204001 & y) == 0x204001) return true;
            break;
        case 35:
            if ((0x8102000 & y) == 0x8102000) return true;
            if ((0x1041000 & y) == 0x1041000) return true;
            if ((0x380 & y) == 0x380) return true;
            if ((0x41001 & y) == 0x41001) return true;
            break;
        case 36:
            if ((0x108400 & y) == 0x108400) return true;
            if ((0x820800 & y) == 0x820800) return true;
            break;
        case 37:
            if ((0x410400 & y) == 0x410400) return true;
            if ((0x84200 & y) == 0x84200) return true;
            break;
        case 38:
            if ((0x208200 & y) == 0x208200) return true;
            if ((0x42100 & y) == 0x42100) return true;
            break;
        case 39:
            if ((0x810200 & y) == 0x810200) return true;
            if ((0x38 & y) == 0x38) return true;
            if ((0x104100 & y) == 0x104100) return true;
            break;
        case 40:
            if ((0x408100 & y) == 0x408100) return true;
            if ((0x1C & y) == 0x1C) return true;
            if ((0x82080 & y) == 0x82080) return true;
            break;
        case 41:
            if ((0x204080 & y) == 0x204080) return true;
            if ((0xE & y) == 0xE) return true;
            if ((0x41040 & y) == 0x41040) return true;
            break;
    }
    return false;
}


int64_t GewinnstellungReihe(const short x, const short y, bool spieler) {
    /*Überprüft, ob der übergebene Spieler gewonnen hätte, wenn er in
    das übergebene Loch setzen würde*/
    int64_t temp;
    if (spieler == SPIELER1)
        temp = Feld1;
    else
        temp = Feld2;
    switch (x * 6 +
            y) //X und Y Position in eine feste Position(0-41) umgerechnet, da die switch-anweisung schneller ausgeführt wird
    {
        case 0:
            if ((0x408100000LL & temp) == 0x408100000LL) return 0x20408100000LL;
            if ((0x820800000LL & temp) == 0x820800000LL) return 0x20820800000LL;
            break;
        case 1:
            if ((0x410400000LL & temp) == 0x410400000LL) return 0x10410400000LL;
            if ((0x204080000LL & temp) == 0x204080000LL) return 0x10204080000LL;
            break;
        case 2:
            if ((0x208200000LL & temp) == 0x208200000LL) return 0x8208200000LL;
            if ((0x102040000LL & temp) == 0x102040000LL) return 0x8102040000LL;
            break;
        case 3:
            if ((0x38000000000LL & temp) == 0x38000000000LL) return 0x3C000000000LL;
            if ((0x210800000LL & temp) == 0x210800000LL) return 0x4210800000LL;
            if ((0x104100000LL & temp) == 0x104100000LL) return 0x4104100000LL;
            break;
        case 4:
            if ((0x108400000LL & temp) == 0x108400000LL) return 0x2108400000LL;
            if ((0x1C000000000LL & temp) == 0x1C000000000LL) return 0x1E000000000LL;
            if ((0x82080000LL & temp) == 0x82080000LL) return 0x2082080000LL;
            break;
        case 5:
            if ((0x84200000LL & temp) == 0x84200000LL) return 0x1084200000LL;
            if ((0xE000000000LL & temp) == 0xE000000000LL) return 0xF000000000LL;
            if ((0x41040000LL & temp) == 0x41040000LL) return 0x1041040000LL;
            break;
        case 6:
            if ((0x10204000LL & temp) == 0x10204000LL) return 0x810204000LL;
            if ((0x20820000LL & temp) == 0x20820000LL) return 0x820820000LL;
            if ((0x20020800000LL & temp) == 0x20020800000LL) return 0x20820800000LL;
            break;
        case 7:
            if ((0x10010400000LL & temp) == 0x10010400000LL) return 0x10410400000LL;
            if ((0x20008100000LL & temp) == 0x20008100000LL) return 0x20408100000LL;
            if ((0x10410000LL & temp) == 0x10410000LL) return 0x410410000LL;
            if ((0x8102000LL & temp) == 0x8102000LL) return 0x408102000LL;
            break;
        case 8:
            if ((0x8008200000LL & temp) == 0x8008200000LL) return 0x8208200000LL;
            if ((0x4010800000LL & temp) == 0x4010800000LL) return 0x4210800000LL;
            if ((0x8208000LL & temp) == 0x8208000LL) return 0x208208000LL;
            if ((0x10004080000LL & temp) == 0x10004080000LL) return 0x10204080000LL;
            if ((0x4081000LL & temp) == 0x4081000LL) return 0x204081000LL;
            break;
        case 9:
            if ((0x8420000LL & temp) == 0x8420000LL) return 0x108420000LL;
            if ((0xE00000000LL & temp) == 0xE00000000LL) return 0xF00000000LL;
            if ((0x4104000LL & temp) == 0x4104000LL) return 0x104104000LL;
            if ((0x4004100000LL & temp) == 0x4004100000LL) return 0x4104100000LL;
            if ((0x8002040000LL & temp) == 0x8002040000LL) return 0x8102040000LL;
            if ((0x2008400000LL & temp) == 0x2008400000LL) return 0x2108400000LL;
            break;
        case 10:
            if ((0x4210000LL & temp) == 0x4210000LL) return 0x84210000LL;
            if ((0x2082000LL & temp) == 0x2082000LL) return 0x82082000LL;
            if ((0x700000000LL & temp) == 0x700000000LL) return 0x780000000LL;
            if ((0x2002080000LL & temp) == 0x2002080000LL) return 0x2082080000LL;
            if ((0x1004200000LL & temp) == 0x1004200000LL) return 0x1084200000LL;
            break;
        case 11:
            if ((0x2108000LL & temp) == 0x2108000LL) return 0x42108000LL;
            if ((0x1041000LL & temp) == 0x1041000LL) return 0x41041000LL;
            if ((0x380000000LL & temp) == 0x380000000LL) return 0x3C0000000LL;
            if ((0x1001040000LL & temp) == 0x1001040000LL) return 0x1041040000LL;
            break;
        case 12:
            if ((0x408100LL & temp) == 0x408100LL) return 0x20408100LL;
            if ((0x20800800000LL & temp) == 0x20800800000LL) return 0x20820800000LL;
            if ((0x820800LL & temp) == 0x820800LL) return 0x20820800LL;
            if ((0x800820000LL & temp) == 0x800820000LL) return 0x820820000LL;
            break;
        case 13:
            if ((0x4200800000LL & temp) == 0x4200800000LL) return 0x4210800000LL;
            if ((0x10400400000LL & temp) == 0x10400400000LL) return 0x10410400000LL;
            if ((0x800204000LL & temp) == 0x800204000LL) return 0x810204000LL;
            if ((0x410400LL & temp) == 0x410400LL) return 0x10410400LL;
            if ((0x400410000LL & temp) == 0x400410000LL) return 0x410410000LL;
            if ((0x204080LL & temp) == 0x204080LL) return 0x10204080LL;
            break;
        case 14:
            if ((0x100420000LL & temp) == 0x100420000LL) return 0x108420000LL;
            if ((0x8200200000LL & temp) == 0x8200200000LL) return 0x8208200000LL;
            if ((0x20400100000LL & temp) == 0x20400100000LL) return 0x20408100000LL;
            if ((0x200208000LL & temp) == 0x200208000LL) return 0x208208000LL;
            if ((0x208200LL & temp) == 0x208200LL) return 0x8208200LL;
            if ((0x400102000LL & temp) == 0x400102000LL) return 0x408102000LL;
            if ((0x2100400000LL & temp) == 0x2100400000LL) return 0x2108400000LL;
            if ((0x102040LL & temp) == 0x102040LL) return 0x8102040LL;
            break;
        case 15:
            if ((0x210800LL & temp) == 0x210800LL) return 0x4210800LL;
            if ((0x10200080000LL & temp) == 0x10200080000LL) return 0x10204080000LL;
            if ((0x4100100000LL & temp) == 0x4100100000LL) return 0x4104100000LL;
            if ((0x100104000LL & temp) == 0x100104000LL) return 0x104104000LL;
            if ((0x200081000LL & temp) == 0x200081000LL) return 0x204081000LL;
            if ((0x80210000LL & temp) == 0x80210000LL) return 0x84210000LL;
            if ((0x38000000LL & temp) == 0x38000000LL) return 0x3C000000LL;
            if ((0x104100LL & temp) == 0x104100LL) return 0x4104100LL;
            if ((0x1080200000LL & temp) == 0x1080200000LL) return 0x1084200000LL;
            break;
        case 16:
            if ((0x108400LL & temp) == 0x108400LL) return 0x2108400LL;
            if ((0x8100040000LL & temp) == 0x8100040000LL) return 0x8102040000LL;
            if ((0x80082000LL & temp) == 0x80082000LL) return 0x82082000LL;
            if ((0x40108000LL & temp) == 0x40108000LL) return 0x42108000LL;
            if ((0x82080LL & temp) == 0x82080LL) return 0x2082080LL;
            if ((0x1C000000LL & temp) == 0x1C000000LL) return 0x1E000000LL;
            if ((0x2080080000LL & temp) == 0x2080080000LL) return 0x2082080000LL;
            break;
        case 17:
            if ((0x84200LL & temp) == 0x84200LL) return 0x1084200LL;
            if ((0xE000000LL & temp) == 0xE000000LL) return 0xF000000LL;
            if ((0x40041000LL & temp) == 0x40041000LL) return 0x41041000LL;
            if ((0x41040LL & temp) == 0x41040LL) return 0x1041040LL;
            if ((0x1040040000LL & temp) == 0x1040040000LL) return 0x1041040000LL;
            break;
        case 18:
            if ((0x20820000000LL & temp) == 0x20820000000LL) return 0x20820800000LL;
            if ((0x820020000LL & temp) == 0x820020000LL) return 0x820820000LL;
            if ((0x20020800LL & temp) == 0x20020800LL) return 0x20820800LL;
            if ((0x20820LL & temp) == 0x20820LL) return 0x820820LL;
            if ((0x10204LL & temp) == 0x10204LL) return 0x810204LL;
            if ((0x4210000000LL & temp) == 0x4210000000LL) return 0x4210800000LL;
            break;
        case 19:
            if ((0x20008100LL & temp) == 0x20008100LL) return 0x20408100LL;
            if ((0x10410LL & temp) == 0x10410LL) return 0x410410LL;
            if ((0x10410000000LL & temp) == 0x10410000000LL) return 0x10410400000LL;
            if ((0x2108000000LL & temp) == 0x2108000000LL) return 0x2108400000LL;
            if ((0x108020000LL & temp) == 0x108020000LL) return 0x2108400000LL;
            if ((0x10010400LL & temp) == 0x10010400LL) return 0x10410400LL;
            if ((0x8102LL & temp) == 0x8102LL) return 0x408102LL;
            if ((0x410010000LL & temp) == 0x410010000LL) return 0x410410000LL;
            break;
        case 20:
            if ((0x810004000LL & temp) == 0x810004000LL) return 0x810204000LL;
            if ((0x4010800LL & temp) == 0x4010800LL) return 0x4210800LL;
            if ((0x8208000000LL & temp) == 0x8208000000LL) return 0x8208200000LL;
            if ((0x208008000LL & temp) == 0x208008000LL) return 0x208208000LL;
            if ((0x8008200LL & temp) == 0x8008200LL) return 0x8208200LL;
            if ((0x8208LL & temp) == 0x8208LL) return 0x208208LL;
            if ((0x84010000LL & temp) == 0x84010000LL) return 0x84210000LL;
            if ((0x10004080LL & temp) == 0x10004080LL) return 0x10204080LL;
            if ((0x4081LL & temp) == 0x4081LL) return 0x204081LL;
            if ((0x1084000000LL & temp) == 0x1084000000LL) return 0x1084200000LL;
            break;
        case 21:
            if ((0x20408000000LL & temp) == 0x20408000000LL) return 0x20408100000LL;
            if ((0x8420LL & temp) == 0x8420LL) return 0x108420LL;
            if ((0xE00000LL & temp) == 0xE00000LL) return 0xF00000LL;
            if ((0x4104000000LL & temp) == 0x4104000000LL) return 0x4104100000LL;
            if ((0x2008400LL & temp) == 0x2008400LL) return 0x2108400LL;
            if ((0x408002000LL & temp) == 0x408002000LL) return 0x408102000LL;
            if ((0x4004100LL & temp) == 0x4004100LL) return 0x4104100LL;
            if ((0x104004000LL & temp) == 0x104004000LL) return 0x104104000LL;
            if ((0x42008000LL & temp) == 0x42008000LL) return 0x42108000LL;
            if ((0x8002040LL & temp) == 0x8002040LL) return 0x8102040LL;
            if ((0x4104LL & temp) == 0x4104LL) return 0x104104LL;
            break;
        case 22:
            if ((0x10204000000LL & temp) == 0x10204000000LL) return 0x10204080000LL;
            if ((0x700000LL & temp) == 0x700000LL) return 0x780000LL;
            if ((0x4210LL & temp) == 0x4210LL) return 0x84210LL;
            if ((0x204001000LL & temp) == 0x204001000LL) return 0x204081000LL;
            if ((0x2082000000LL & temp) == 0x2082000000LL) return 0x2082080000LL;
            if ((0x1004200LL & temp) == 0x1004200LL) return 0x1084200LL;
            if ((0x82002000LL & temp) == 0x82002000LL) return 0x82082000LL;
            if ((0x2002080LL & temp) == 0x2002080LL) return 0x2082080LL;
            if ((0x2082LL & temp) == 0x2082LL) return 0x82082LL;
            break;
        case 23:
            if ((0x380000LL & temp) == 0x380000LL) return 0x3C0000LL;
            if ((0x8102000000LL & temp) == 0x8102000000LL) return 0x8102040000LL;
            if ((0x2108LL & temp) == 0x2108LL) return 0x42108LL;
            if ((0x1041000000LL & temp) == 0x1041000000LL) return 0x1041040000LL;
            if ((0x41001000LL & temp) == 0x41001000LL) return 0x41041000LL;
            if ((0x1001040LL & temp) == 0x1001040LL) return 0x1041040LL;
            if ((0x1041LL & temp) == 0x1041LL) return 0x41041LL;
            break;
        case 24:
            if ((0x108400000LL & temp) == 0x108400000LL) return 0x108420000LL;
            if ((0x800820LL & temp) == 0x800820LL) return 0x820820LL;
            if ((0x20800800LL & temp) == 0x20800800LL) return 0x20820800LL;
            if ((0x820800000LL & temp) == 0x820800000LL) return 0x820820000LL;
            break;
        case 25:
            if ((0x800204LL & temp) == 0x800204LL) return 0x810204LL;
            if ((0x4200800LL & temp) == 0x4200800LL) return 0x4210800LL;
            if ((0x400410LL & temp) == 0x400410LL) return 0x410410LL;
            if ((0x10400400LL & temp) == 0x10400400LL) return 0x10410400LL;
            if ((0x410400000LL & temp) == 0x410400000LL) return 0x410410000LL;
            if ((0x84200000LL & temp) == 0x84200000LL) return 0x84210000LL;
            break;
        case 26:
            if ((0x100420LL & temp) == 0x100420LL) return 0x108420LL;
            if ((0x20400100LL & temp) == 0x20400100LL) return 0x20408100LL;
            if ((0x8200200LL & temp) == 0x8200200LL) return 0x8208200LL;
            if ((0x208200000LL & temp) == 0x208200000LL) return 0x208208000LL;
            if ((0x200208LL & temp) == 0x200208LL) return 0x208208LL;
            if ((0x2100400LL & temp) == 0x2100400LL) return 0x2108400LL;
            if ((0x400102LL & temp) == 0x400102LL) return 0x408102LL;
            if ((0x42100000LL & temp) == 0x42100000LL) return 0x42108000LL;
            break;
        case 27:
            if ((0x810200000LL & temp) == 0x810200000LL) return 0x810204000LL;
            if ((0x80210LL & temp) == 0x80210LL) return 0x84210LL;
            if ((0x38000LL & temp) == 0x38000LL) return 0x3C000LL;
            if ((0x4100100LL & temp) == 0x4100100LL) return 0x4104100LL;
            if ((0x104100000LL & temp) == 0x104100000LL) return 0x104104000LL;
            if ((0x10200080LL & temp) == 0x10200080LL) return 0x10204080LL;
            if ((0x1080200LL & temp) == 0x1080200LL) return 0x1084200LL;
            if ((0x100104LL & temp) == 0x100104LL) return 0x104104LL;
            if ((0x200081LL & temp) == 0x200081LL) return 0x204081LL;
            break;
        case 28:
            if ((0x408100000LL & temp) == 0x408100000LL) return 0x408102000LL;
            if ((0x40108LL & temp) == 0x40108LL) return 0x42108LL;
            if ((0x1C000LL & temp) == 0x1C000LL) return 0x1E000LL;
            if ((0x2080080LL & temp) == 0x2080080LL) return 0x2082080LL;
            if ((0x82080000LL & temp) == 0x82080000LL) return 0x82082000LL;
            if ((0x8100040LL & temp) == 0x8100040LL) return 0x8102040LL;
            if ((0x80082LL & temp) == 0x80082LL) return 0x82082LL;
            break;
        case 29:
            if ((0x204080000LL & temp) == 0x204080000LL) return 0x204081000LL;
            if ((0xE000LL & temp) == 0xE000LL) return 0xF000LL;
            if ((0x1040040LL & temp) == 0x1040040LL) return 0x1041040LL;
            if ((0x41040000LL & temp) == 0x41040000LL) return 0x41041000LL;
            if ((0x40041LL & temp) == 0x40041LL) return 0x41041LL;
            break;
        case 30:
            if ((0x4210000LL & temp) == 0x4210000LL) return 0x4210800LL;
            if ((0x20820000LL & temp) == 0x20820000LL) return 0x20820800LL;
            if ((0x820020LL & temp) == 0x820020LL) return 0x820820LL;
            break;
        case 31:
            if ((0x10410000LL & temp) == 0x10410000LL) return 0x10410400LL;
            if ((0x410010LL & temp) == 0x410010LL) return 0x410410LL;
            if ((0x108020LL & temp) == 0x108020LL) return 0x108420LL;
            if ((0x2108000LL & temp) == 0x2108000LL) return 0x2108400LL;
            break;
        case 32:
            if ((0x8208000LL & temp) == 0x8208000LL) return 0x8208200LL;
            if ((0x208008LL & temp) == 0x208008LL) return 0x208208LL;
            if ((0x84010LL & temp) == 0x84010LL) return 0x84210LL;
            if ((0x810004LL & temp) == 0x810004LL) return 0x810204LL;
            if ((0x1084000LL & temp) == 0x1084000LL) return 0x1084200LL;
            break;
        case 33:
            if ((0x20408000LL & temp) == 0x20408000LL) return 0x20408100LL;
            if ((0x4104000LL & temp) == 0x4104000LL) return 0x4104100LL;
            if ((0xE00LL & temp) == 0xE00LL) return 0xF00LL;
            if ((0x104004LL & temp) == 0x104004LL) return 0x104104LL;
            if ((0x42008LL & temp) == 0x42008LL) return 0x42108LL;
            if ((0x408002LL & temp) == 0x408002LL) return 0x408102LL;
            break;
        case 34:
            if ((0x10204000LL & temp) == 0x10204000LL) return 0x10204080LL;
            if ((0x2082000LL & temp) == 0x2082000LL) return 0x2082080LL;
            if ((0x700LL & temp) == 0x700LL) return 0x780LL;
            if ((0x82002LL & temp) == 0x82002LL) return 0x82082LL;
            if ((0x204001LL & temp) == 0x204001LL) return 0x204081LL;
            break;
        case 35:
            if ((0x8102000LL & temp) == 0x8102000LL) return 0x8102040LL;
            if ((0x1041000LL & temp) == 0x1041000LL) return 0x1041040LL;
            if ((0x380LL & temp) == 0x380LL) return 0x3C0LL;
            if ((0x41001LL & temp) == 0x41001LL) return 0x41041LL;
            break;
        case 36:
            if ((0x108400LL & temp) == 0x108400LL) return 0x108420LL;
            if ((0x820800LL & temp) == 0x820800LL) return 0x820820LL;
            break;
        case 37:
            if ((0x410400LL & temp) == 0x410400LL) return 0x410410LL;
            if ((0x84200LL & temp) == 0x84200LL) return 0x84210LL;
            break;
        case 38:
            if ((0x208200LL & temp) == 0x208200LL) return 0x208208LL;
            if ((0x42100LL & temp) == 0x42100LL) return 0x42108LL;
            break;
        case 39:
            if ((0x810200LL & temp) == 0x810200LL) return 0x810204LL;
            if ((0x38LL & temp) == 0x38LL) return 0x3CLL;
            if ((0x104100LL & temp) == 0x104100LL) return 0x104104LL;
            break;
        case 40:
            if ((0x408100LL & temp) == 0x408100LL) return 0x408102LL;
            if ((0x1CLL & temp) == 0x1CLL) return 0x1ELL;
            if ((0x82080LL & temp) == 0x82080LL) return 0x82082LL;
            break;
        case 41:
            if ((0x204080LL & temp) == 0x204080LL) return 0x204081LL;
            if ((0xELL & temp) == 0xELL) return 0xFLL;
            if ((0x41040LL & temp) == 0x41040LL) return 0x41041LL;
            break;
    }
    return 0LL;
}

#include <cstdint>

bool SpielBeenden1() {
    const int64_t x = ~Feld1;
    int y = x;
    switch (Hoehe[3]) {
        case 0:
            if (!(x & 0x20820000000LL &&
                  x & 0x820020000LL &&
                  y & 0x20020800 &&
                  y & 0x20820 &&
                  y & 0x10204 &&
                  x & 0x4210000000LL))
                return true;
            break;
        case 1:
            if (!(y & 0x20008100 &&
                  y & 0x10410 &&
                  x & 0x10410000000LL &&
                  x & 0x2108000000LL &&
                  x & 0x108020000LL &&
                  y & 0x10010400 &&
                  y & 0x8102 &&
                  x & 0x410010000LL))
                return true;
            break;
        case 2:
            if (!(x & 0x810004000LL &&
                  y & 0x4010800 &&
                  x & 0x8208000000LL &&
                  x & 0x208008000LL &&
                  y & 0x8008200 &&
                  y & 0x8208 &&
                  y & 0x84010000 &&
                  y & 0x10004080 &&
                  y & 0x4081 &&
                  x & 0x1084000000LL))
                return true;
            break;
        case 3:
            if (!(x & 0x20408000000LL &&
                  y & 0x8420 &&
                  y & 0xE00000 &&
                  x & 0x4104000000LL &&
                  y & 0x2008400 &&
                  x & 0x408002000LL &&
                  y & 0x4004100 &&
                  x & 0x104004000LL &&
                  y & 0x42008000 &&
                  y & 0x8002040 &&
                  y & 0x4104))
                return true;
            break;
        case 4:
            if (!(x & 0x10204000000LL &&
                  y & 0x700000 &&
                  y & 0x4210 &&
                  x & 0x204001000LL &&
                  x & 0x2082000000LL &&
                  y & 0x1004200 &&
                  y & 0x82002000 &&
                  y & 0x2002080 &&
                  y & 0x2082))
                return true;
            break;
        case 5:
            if (!(y & 0x380000 &&
                  x & 0x8102000000LL &&
                  y & 0x2108 &&
                  x & 0x1041000000LL &&
                  y & 0x41001000 &&
                  y & 0x1001040 &&
                  y & 0x1041))
                return true;
            break;
        default:
            break;
    }
    switch (Hoehe[4]) {
        case 0:
            if (!(x & 0x108400000LL &&
                  y & 0x800820 &&
                  y & 0x20800800 &&
                  x & 0x820800000LL))
                return true;
            break;
        case 1:
            if (!(y & 0x800204 &&
                  y & 0x4200800 &&
                  y & 0x400410 &&
                  y & 0x10400400 &&
                  x & 0x410400000LL &&
                  y & 0x84200000))
                return true;
            break;
        case 2:
            if (!(y & 0x100420 &&
                  y & 0x20400100 &&
                  y & 0x8200200 &&
                  x & 0x208200000LL &&
                  y & 0x200208 &&
                  y & 0x2100400 &&
                  y & 0x400102 &&
                  y & 0x42100000))
                return true;
            break;
        case 3:
            if (!(x & 0x810200000LL &&
                  y & 0x80210 &&
                  y & 0x38000 &&
                  y & 0x4100100 &&
                  x & 0x104100000LL &&
                  y & 0x10200080 &&
                  y & 0x1080200 &&
                  y & 0x100104 &&
                  y & 0x200081))
                return true;
            break;
        case 4:
            if (!(x & 0x408100000LL &&
                  y & 0x40108 &&
                  y & 0x1C000 &&
                  y & 0x2080080 &&
                  y & 0x82080000 &&
                  y & 0x8100040 &&
                  y & 0x80082))
                return true;
            break;
        case 5:
            if (!(x & 0x204080000LL &&
                  y & 0xE000 &&
                  y & 0x1040040 &&
                  y & 0x41040000 &&
                  y & 0x40041))
                return true;
            break;
        default:
            break;
    }
    switch (Hoehe[2]) {
        case 0:
            if (!(y & 0x408100 &&
                  x & 0x20800800000LL &&
                  y & 0x820800 &&
                  x & 0x800820000LL))
                return true;
            break;
        case 1:
            if (!(x & 0x4200800000LL &&
                  x & 0x10400400000LL &&
                  x & 0x800204000LL &&
                  y & 0x410400 &&
                  x & 0x400410000LL &&
                  y & 0x204080))
                return true;
            break;
        case 2:
            if (!(x & 0x100420000LL &&
                  x & 0x8200200000LL &&
                  x & 0x20400100000LL &&
                  x & 0x200208000LL &&
                  y & 0x208200 &&
                  x & 0x400102000LL &&
                  x & 0x2100400000LL &&
                  y & 0x102040))
                return true;
            break;
        case 3:
            if (!(y & 0x210800 &&
                  x & 0x10200080000LL &&
                  x & 0x4100100000LL &&
                  x & 0x100104000LL &&
                  x & 0x200081000LL &&
                  y & 0x80210000 &&
                  y & 0x38000000 &&
                  y & 0x104100 &&
                  x & 0x1080200000LL))
                return true;
            break;
        case 4:
            if (!(y & 0x108400 &&
                  x & 0x8100040000LL &&
                  y & 0x80082000 &&
                  y & 0x40108000 &&
                  y & 0x82080 &&
                  y & 0x1C000000 &&
                  x & 0x2080080000LL))
                return true;
            break;
        case 5:
            if (!(y & 0x84200 &&
                  y & 0xE000000 &&
                  y & 0x40041000 &&
                  y & 0x41040 &&
                  x & 0x1040040000LL))
                return true;
            break;
        default:
            break;
    }
    switch (Hoehe[5]) {
        case 0:
            if (!(y & 0x4210000 &&
                  y & 0x20820000 &&
                  y & 0x820020))
                return true;
            break;
        case 1:
            if (!(y & 0x10410000 &&
                  y & 0x410010 &&
                  y & 0x108020 &&
                  y & 0x2108000))
                return true;
            break;
        case 2:
            if (!(y & 0x8208000 &&
                  y & 0x208008 &&
                  y & 0x84010 &&
                  y & 0x810004 &&
                  y & 0x1084000))
                return true;
            break;
        case 3:
            if (!(y & 0x20408000 &&
                  y & 0x4104000 &&
                  y & 0xE00 &&
                  y & 0x104004 &&
                  y & 0x42008 &&
                  y & 0x408002))
                return true;
            break;
        case 4:
            if (!(y & 0x10204000 &&
                  y & 0x2082000 &&
                  y & 0x700 &&
                  y & 0x82002 &&
                  y & 0x204001))
                return true;
            break;
        case 5:
            if (!(y & 0x8102000 &&
                  y & 0x1041000 &&
                  y & 0x380 &&
                  y & 0x41001))
                return true;
            break;
        default:
            break;
    }
    switch (Hoehe[1]) {
        case 0:
            if (!(y & 0x10204000 &&
                  y & 0x20820000 &&
                  x & 0x20020800000LL))
                return true;
            break;
        case 1:
            if (!(x & 0x10010400000LL &&
                  x & 0x20008100000LL &&
                  y & 0x10410000 &&
                  y & 0x8102000))
                return true;
            break;
        case 2:
            if (!(x & 0x8008200000LL &&
                  x & 0x4010800000LL &&
                  y & 0x8208000 &&
                  x & 0x10004080000LL &&
                  y & 0x4081000))
                return true;
            break;
        case 3:
            if (!(y & 0x8420000 &&
                  x & 0xE00000000LL &&
                  y & 0x4104000 &&
                  x & 0x4004100000LL &&
                  x & 0x8002040000LL &&
                  x & 0x2008400000LL))
                return true;
            break;
        case 4:
            if (!(y & 0x4210000 &&
                  y & 0x2082000 &&
                  x & 0x700000000LL &&
                  x & 0x2002080000LL &&
                  x & 0x1004200000LL))
                return true;
            break;
        case 5:
            if (!(y & 0x2108000 &&
                  y & 0x1041000 &&
                  x & 0x380000000LL &&
                  x & 0x1001040000LL))
                return true;
            break;
        default:
            break;
    }
    switch (Hoehe[6]) {
        case 0:
            if (!(y & 0x108400 &&
                  y & 0x820800))
                return true;
            break;
        case 1:
            if (!(y & 0x410400 &&
                  y & 0x84200))
                return true;
            break;
        case 2:
            if (!(y & 0x208200 &&
                  y & 0x42100))
                return true;
            break;
        case 3:
            if (!(y & 0x810200 &&
                  y & 0x38 &&
                  y & 0x104100))
                return true;
            break;
        case 4:
            if (!(y & 0x408100 &&
                  y & 0x1C &&
                  y & 0x82080))
                return true;
            break;
        case 5:
            if (!(y & 0x204080 &&
                  y & 0xE &&
                  y & 0x41040))
                return true;
            break;
        default:
            break;
    }
    switch (Hoehe[0]) {
        case 0:
            if (!(x & 0x408100000LL &&
                  x & 0x820800000LL))
                return true;
            break;
        case 1:
            if (!(x & 0x410400000LL &&
                  x & 0x204080000LL))
                return true;
            break;
        case 2:
            if (!(x & 0x208200000LL &&
                  x & 0x102040000LL))
                return true;
            break;
        case 3:
            if (!(x & 0x38000000000LL &&
                  x & 0x210800000LL &&
                  x & 0x104100000LL))
                return true;
            break;
        case 4:
            if (!(x & 0x108400000LL &&
                  x & 0x1C000000000LL &&
                  y & 0x82080000))
                return true;
            break;
        case 5:
            if (!(y & 0x84200000 &&
                  x & 0xE000000000LL &&
                  y & 0x41040000))
                return true;
            break;
        default:
            break;
    }
    return false;
}

bool SpielBeenden2(void) {
    const int64_t x = ~Feld2;
    int y = x;
    switch (Hoehe[3]) {
        case 0:
            if (!(x & 0x20820000000LL &&
                  x & 0x820020000LL &&
                  y & 0x20020800 &&
                  y & 0x20820 &&
                  y & 0x10204 &&
                  x & 0x4210000000LL))
                return true;
            break;
        case 1:
            if (!(y & 0x20008100 &&
                  y & 0x10410 &&
                  x & 0x10410000000LL &&
                  x & 0x2108000000LL &&
                  x & 0x108020000LL &&
                  y & 0x10010400 &&
                  y & 0x8102 &&
                  x & 0x410010000LL))
                return true;
            break;
        case 2:
            if (!(x & 0x810004000LL &&
                  y & 0x4010800 &&
                  x & 0x8208000000LL &&
                  x & 0x208008000LL &&
                  y & 0x8008200 &&
                  y & 0x8208 &&
                  y & 0x84010000 &&
                  y & 0x10004080 &&
                  y & 0x4081 &&
                  x & 0x1084000000LL))
                return true;
            break;
        case 3:
            if (!(x & 0x20408000000LL &&
                  y & 0x8420 &&
                  y & 0xE00000 &&
                  x & 0x4104000000LL &&
                  y & 0x2008400 &&
                  x & 0x408002000LL &&
                  y & 0x4004100 &&
                  x & 0x104004000LL &&
                  y & 0x42008000 &&
                  y & 0x8002040 &&
                  y & 0x4104))
                return true;
            break;
        case 4:
            if (!(x & 0x10204000000LL &&
                  y & 0x700000 &&
                  y & 0x4210 &&
                  x & 0x204001000LL &&
                  x & 0x2082000000LL &&
                  y & 0x1004200 &&
                  y & 0x82002000 &&
                  y & 0x2002080 &&
                  y & 0x2082))
                return true;
            break;
        case 5:
            if (!(y & 0x380000 &&
                  x & 0x8102000000LL &&
                  y & 0x2108 &&
                  x & 0x1041000000LL &&
                  y & 0x41001000 &&
                  y & 0x1001040 &&
                  y & 0x1041))
                return true;
            break;
        default:
            break;
    }
    switch (Hoehe[4]) {
        case 0:
            if (!(x & 0x108400000LL &&
                  y & 0x800820 &&
                  y & 0x20800800 &&
                  x & 0x820800000LL))
                return true;
            break;
        case 1:
            if (!(y & 0x800204 &&
                  y & 0x4200800 &&
                  y & 0x400410 &&
                  y & 0x10400400 &&
                  x & 0x410400000LL &&
                  y & 0x84200000))
                return true;
            break;
        case 2:
            if (!(y & 0x100420 &&
                  y & 0x20400100 &&
                  y & 0x8200200 &&
                  x & 0x208200000LL &&
                  y & 0x200208 &&
                  y & 0x2100400 &&
                  y & 0x400102 &&
                  y & 0x42100000))
                return true;
            break;
        case 3:
            if (!(x & 0x810200000LL &&
                  y & 0x80210 &&
                  y & 0x38000 &&
                  y & 0x4100100 &&
                  x & 0x104100000LL &&
                  y & 0x10200080 &&
                  y & 0x1080200 &&
                  y & 0x100104 &&
                  y & 0x200081))
                return true;
            break;
        case 4:
            if (!(x & 0x408100000LL &&
                  y & 0x40108 &&
                  y & 0x1C000 &&
                  y & 0x2080080 &&
                  y & 0x82080000 &&
                  y & 0x8100040 &&
                  y & 0x80082))
                return true;
            break;
        case 5:
            if (!(x & 0x204080000LL &&
                  y & 0xE000 &&
                  y & 0x1040040 &&
                  y & 0x41040000 &&
                  y & 0x40041))
                return true;
            break;
        default:
            break;
    }
    switch (Hoehe[2]) {
        case 0:
            if (!(y & 0x408100 &&
                  x & 0x20800800000LL &&
                  y & 0x820800 &&
                  x & 0x800820000LL))
                return true;
            break;
        case 1:
            if (!(x & 0x4200800000LL &&
                  x & 0x10400400000LL &&
                  x & 0x800204000LL &&
                  y & 0x410400 &&
                  x & 0x400410000LL &&
                  y & 0x204080))
                return true;
            break;
        case 2:
            if (!(x & 0x100420000LL &&
                  x & 0x8200200000LL &&
                  x & 0x20400100000LL &&
                  x & 0x200208000LL &&
                  y & 0x208200 &&
                  x & 0x400102000LL &&
                  x & 0x2100400000LL &&
                  y & 0x102040))
                return true;
            break;
        case 3:
            if (!(y & 0x210800 &&
                  x & 0x10200080000LL &&
                  x & 0x4100100000LL &&
                  x & 0x100104000LL &&
                  x & 0x200081000LL &&
                  y & 0x80210000 &&
                  y & 0x38000000 &&
                  y & 0x104100 &&
                  x & 0x1080200000LL))
                return true;
            break;
        case 4:
            if (!(y & 0x108400 &&
                  x & 0x8100040000LL &&
                  y & 0x80082000 &&
                  y & 0x40108000 &&
                  y & 0x82080 &&
                  y & 0x1C000000 &&
                  x & 0x2080080000LL))
                return true;
            break;
        case 5:
            if (!(y & 0x84200 &&
                  y & 0xE000000 &&
                  y & 0x40041000 &&
                  y & 0x41040 &&
                  x & 0x1040040000LL))
                return true;
            break;
        default:
            break;
    }
    switch (Hoehe[5]) {
        case 0:
            if (!(y & 0x4210000 &&
                  y & 0x20820000 &&
                  y & 0x820020))
                return true;
            break;
        case 1:
            if (!(y & 0x10410000 &&
                  y & 0x410010 &&
                  y & 0x108020 &&
                  y & 0x2108000))
                return true;
            break;
        case 2:
            if (!(y & 0x8208000 &&
                  y & 0x208008 &&
                  y & 0x84010 &&
                  y & 0x810004 &&
                  y & 0x1084000))
                return true;
            break;
        case 3:
            if (!(y & 0x20408000 &&
                  y & 0x4104000 &&
                  y & 0xE00 &&
                  y & 0x104004 &&
                  y & 0x42008 &&
                  y & 0x408002))
                return true;
            break;
        case 4:
            if (!(y & 0x10204000 &&
                  y & 0x2082000 &&
                  y & 0x700 &&
                  y & 0x82002 &&
                  y & 0x204001))
                return true;
            break;
        case 5:
            if (!(y & 0x8102000 &&
                  y & 0x1041000 &&
                  y & 0x380 &&
                  y & 0x41001))
                return true;
            break;
        default:
            break;
    }
    switch (Hoehe[1]) {
        case 0:
            if (!(y & 0x10204000 &&
                  y & 0x20820000 &&
                  x & 0x20020800000LL))
                return true;
            break;
        case 1:
            if (!(x & 0x10010400000LL &&
                  x & 0x20008100000LL &&
                  y & 0x10410000 &&
                  y & 0x8102000))
                return true;
            break;
        case 2:
            if (!(x & 0x8008200000LL &&
                  x & 0x4010800000LL &&
                  y & 0x8208000 &&
                  x & 0x10004080000LL &&
                  y & 0x4081000))
                return true;
            break;
        case 3:
            if (!(y & 0x8420000 &&
                  x & 0xE00000000LL &&
                  y & 0x4104000 &&
                  x & 0x4004100000LL &&
                  x & 0x8002040000LL &&
                  x & 0x2008400000LL))
                return true;
            break;
        case 4:
            if (!(y & 0x4210000 &&
                  y & 0x2082000 &&
                  x & 0x700000000LL &&
                  x & 0x2002080000LL &&
                  x & 0x1004200000LL))
                return true;
            break;
        case 5:
            if (!(y & 0x2108000 &&
                  y & 0x1041000 &&
                  x & 0x380000000LL &&
                  x & 0x1001040000LL))
                return true;
            break;
        default:
            break;
    }
    switch (Hoehe[6]) {
        case 0:
            if (!(y & 0x108400 &&
                  y & 0x820800))
                return true;
            break;
        case 1:
            if (!(y & 0x410400 &&
                  y & 0x84200))
                return true;
            break;
        case 2:
            if (!(y & 0x208200 &&
                  y & 0x42100))
                return true;
            break;
        case 3:
            if (!(y & 0x810200 &&
                  y & 0x38 &&
                  y & 0x104100))
                return true;
            break;
        case 4:
            if (!(y & 0x408100 &&
                  y & 0x1C &&
                  y & 0x82080))
                return true;
            break;
        case 5:
            if (!(y & 0x204080 &&
                  y & 0xE &&
                  y & 0x41040))
                return true;
            break;
        default:
            break;
    }
    switch (Hoehe[0]) {
        case 0:
            if (!(x & 0x408100000LL &&
                  x & 0x820800000LL))
                return true;
            break;
        case 1:
            if (!(x & 0x410400000LL &&
                  x & 0x204080000LL))
                return true;
            break;
        case 2:
            if (!(x & 0x208200000LL &&
                  x & 0x102040000LL))
                return true;
            break;
        case 3:
            if (!(x & 0x38000000000LL &&
                  x & 0x210800000LL &&
                  x & 0x104100000LL))
                return true;
            break;
        case 4:
            if (!(x & 0x108400000LL &&
                  x & 0x1C000000000LL &&
                  y & 0x82080000))
                return true;
            break;
        case 5:
            if (!(y & 0x84200000 &&
                  x & 0xE000000000LL &&
                  y & 0x41040000))
                return true;
            break;
        default:
            break;
    }
    return false;
}


int SpielBeendenStellung(bool spieler) {
    /*Überprüft, ob der übergebene Spieler gewinnen könnte. Wenn er gewinnen
    kann, dann wird die spalte, in die er setzen müsste, um zu gewinnnen, zurückgegeben,
    ansonsten (-1).*/
    int x;
    for (x = 0; x < 7; x++) {
        if (Hoehe[x] < 6) {
            if (spieler == SPIELER1) {
                if (Gewinnstellung1(x, Hoehe[x]))
                    return x;
            } else if (spieler == SPIELER2) {
                if (Gewinnstellung2(x, Hoehe[x]))
                    return x;
            }
        }
    }
    return (-1); //Wenn keine Siegstellung gefunden wurde
}

int SpielBeendenStellungD(bool spieler, int *arr) {
    int x, j = 0;
    for (x = 0; x < 7; x++) {
        if (Hoehe[x] < 6) {
            if (spieler == SPIELER1) {
                if (Gewinnstellung1(x, Hoehe[x])) {
                    *arr = x;
                    j++;
                }
            } else if (spieler == SPIELER2) {
                if (Gewinnstellung2(x, Hoehe[x])) {
                    *arr = x;
                    j++;
                }
            }
        }
    }
    return j;
}


#include <cstdint>

bool SpielBeenden1Generate(short int *moves) {
    short int wert[6];
    const int64_t x = ~Feld1;
    switch (Hoehe[3]) {
        case 0:
            if (!(x & 0x20820000000LL &&
                  x & 0x820020000LL &&
                  x & 0x20020800LL &&
                  x & 0x20820LL &&
                  x & 0x10204LL &&
                  x & 0x4210000000LL))
                return true;
            moves[0] = 3;
            wert[0] = 15;
            break;
        case 1:
            if (!(x & 0x20008100LL &&
                  x & 0x10410LL &&
                  x & 0x10410000000LL &&
                  x & 0x2108000000LL &&
                  x & 0x108020000LL &&
                  x & 0x10010400LL &&
                  x & 0x8102LL &&
                  x & 0x410010000LL))
                return true;
            moves[0] = 3;
            wert[0] = 21;
            break;
        case 2:
            if (!(x & 0x810004000LL &&
                  x & 0x4010800LL &&
                  x & 0x8208000000LL &&
                  x & 0x208008000LL &&
                  x & 0x8008200LL &&
                  x & 0x8208LL &&
                  x & 0x84010000LL &&
                  x & 0x10004080LL &&
                  x & 0x4081LL &&
                  x & 0x1084000000LL))
                return true;
            moves[0] = 3;
            wert[0] = 27;
            break;
        case 3:
            if (!(x & 0x20408000000LL &&
                  x & 0x8420LL &&
                  x & 0xE00000LL &&
                  x & 0x4104000000LL &&
                  x & 0x2008400LL &&
                  x & 0x408002000LL &&
                  x & 0x4004100LL &&
                  x & 0x104004000LL &&
                  x & 0x42008000LL &&
                  x & 0x8002040LL &&
                  x & 0x4104LL))
                return true;
            moves[0] = 3;
            wert[0] = 26;
            break;
        case 4:
            if (!(x & 0x10204000000LL &&
                  x & 0x700000LL &&
                  x & 0x4210LL &&
                  x & 0x204001000LL &&
                  x & 0x2082000000LL &&
                  x & 0x1004200LL &&
                  x & 0x82002000LL &&
                  x & 0x2002080LL &&
                  x & 0x2082LL))
                return true;
            moves[0] = 3;
            wert[0] = 20;
            break;
        case 5:
            if (!(x & 0x380000LL &&
                  x & 0x8102000000LL &&
                  x & 0x2108LL &&
                  x & 0x1041000000LL &&
                  x & 0x41001000LL &&
                  x & 0x1001040LL &&
                  x & 0x1041LL))
                return true;
            moves[0] = 3;
            wert[0] = 14;
            break;
        default:
            moves[0] = -1;
            wert[0] = 0;
            break;
    }
    switch (Hoehe[4]) {
        case 0:
            if (!(x & 0x108400000LL &&
                  x & 0x800820LL &&
                  x & 0x20800800LL &&
                  x & 0x820800000LL))
                return true;
            if (wert[0] < 11) {
                moves[1] = moves[0];
                wert[1] = wert[0];
                moves[0] = 4;
                wert[0] = 11;
            } else {
                moves[1] = 4;
                wert[1] = 11;
            }
            break;
        case 1:
            if (!(x & 0x800204LL &&
                  x & 0x4200800LL &&
                  x & 0x400410LL &&
                  x & 0x10400400LL &&
                  x & 0x410400000LL &&
                  x & 0x84200000LL))
                return true;
            if (wert[0] < 17) {
                moves[1] = moves[0];
                wert[1] = wert[0];
                moves[0] = 4;
                wert[0] = 17;
            } else {
                moves[1] = 4;
                wert[1] = 17;
            }
            break;
        case 2:
            if (!(x & 0x100420LL &&
                  x & 0x20400100LL &&
                  x & 0x8200200LL &&
                  x & 0x208200000LL &&
                  x & 0x200208LL &&
                  x & 0x2100400LL &&
                  x & 0x400102LL &&
                  x & 0x42100000LL))
                return true;
            if (wert[0] < 23) {
                moves[1] = moves[0];
                wert[1] = wert[0];
                moves[0] = 4;
                wert[0] = 23;
            } else {
                moves[1] = 4;
                wert[1] = 23;
            }
            break;
        case 3:
            if (!(x & 0x810200000LL &&
                  x & 0x80210LL &&
                  x & 0x38000LL &&
                  x & 0x4100100LL &&
                  x & 0x104100000LL &&
                  x & 0x10200080LL &&
                  x & 0x1080200LL &&
                  x & 0x100104LL &&
                  x & 0x200081LL))
                return true;
            if (wert[0] < 22) {
                moves[1] = moves[0];
                wert[1] = wert[0];
                moves[0] = 4;
                wert[0] = 22;
            } else {
                moves[1] = 4;
                wert[1] = 22;
            }
            break;
        case 4:
            if (!(x & 0x408100000LL &&
                  x & 0x40108LL &&
                  x & 0x1C000LL &&
                  x & 0x2080080LL &&
                  x & 0x82080000LL &&
                  x & 0x8100040LL &&
                  x & 0x80082LL))
                return true;
            if (wert[0] < 16) {
                moves[1] = moves[0];
                wert[1] = wert[0];
                moves[0] = 4;
                wert[0] = 16;
            } else {
                moves[1] = 4;
                wert[1] = 16;
            }
            break;
        case 5:
            if (!(x & 0x204080000LL &&
                  x & 0xE000LL &&
                  x & 0x1040040LL &&
                  x & 0x41040000LL &&
                  x & 0x40041LL))
                return true;
            if (wert[0] < 10) {
                moves[1] = moves[0];
                wert[1] = wert[0];
                moves[0] = 4;
                wert[0] = 10;
            } else {
                moves[1] = 4;
                wert[1] = 10;
            }
            break;
        default:
            moves[1] = -1;
            wert[1] = 0;
            break;
    }
    switch (Hoehe[2]) {
        case 0:
            if (!(x & 0x408100LL &&
                  x & 0x20800800000LL &&
                  x & 0x820800LL &&
                  x & 0x800820000LL))
                return true;
            if (wert[1] < 11) {
                moves[2] = moves[1];
                wert[2] = wert[1];
                if (wert[0] < 11) {
                    moves[1] = moves[0];
                    wert[1] = wert[0];
                    moves[0] = 2;
                    wert[0] = 11;
                } else {
                    moves[1] = 2;
                    wert[1] = 11;
                }
            } else {
                moves[2] = 2;
                wert[2] = 11;
            }
            break;
        case 1:
            if (!(x & 0x4200800000LL &&
                  x & 0x10400400000LL &&
                  x & 0x800204000LL &&
                  x & 0x410400LL &&
                  x & 0x400410000LL &&
                  x & 0x204080LL))
                return true;
            if (wert[1] < 17) {
                moves[2] = moves[1];
                wert[2] = wert[1];
                if (wert[0] < 17) {
                    moves[1] = moves[0];
                    wert[1] = wert[0];
                    moves[0] = 2;
                    wert[0] = 17;
                } else {
                    moves[1] = 2;
                    wert[1] = 17;
                }
            } else {
                moves[2] = 2;
                wert[2] = 17;
            }
            break;
        case 2:
            if (!(x & 0x100420000LL &&
                  x & 0x8200200000LL &&
                  x & 0x20400100000LL &&
                  x & 0x200208000LL &&
                  x & 0x208200LL &&
                  x & 0x400102000LL &&
                  x & 0x2100400000LL &&
                  x & 0x102040LL))
                return true;
            if (wert[1] < 23) {
                moves[2] = moves[1];
                wert[2] = wert[1];
                if (wert[0] < 23) {
                    moves[1] = moves[0];
                    wert[1] = wert[0];
                    moves[0] = 2;
                    wert[0] = 23;
                } else {
                    moves[1] = 2;
                    wert[1] = 23;
                }
            } else {
                moves[2] = 2;
                wert[2] = 23;
            }
            break;
        case 3:
            if (!(x & 0x210800LL &&
                  x & 0x10200080000LL &&
                  x & 0x4100100000LL &&
                  x & 0x100104000LL &&
                  x & 0x200081000LL &&
                  x & 0x80210000LL &&
                  x & 0x38000000LL &&
                  x & 0x104100LL &&
                  x & 0x1080200000LL))
                return true;
            if (wert[1] < 22) {
                moves[2] = moves[1];
                wert[2] = wert[1];
                if (wert[0] < 22) {
                    moves[1] = moves[0];
                    wert[1] = wert[0];
                    moves[0] = 2;
                    wert[0] = 22;
                } else {
                    moves[1] = 2;
                    wert[1] = 22;
                }
            } else {
                moves[2] = 2;
                wert[2] = 22;
            }
            break;
        case 4:
            if (!(x & 0x108400LL &&
                  x & 0x8100040000LL &&
                  x & 0x80082000LL &&
                  x & 0x40108000LL &&
                  x & 0x82080LL &&
                  x & 0x1C000000LL &&
                  x & 0x2080080000LL))
                return true;
            if (wert[1] < 16) {
                moves[2] = moves[1];
                wert[2] = wert[1];
                if (wert[0] < 16) {
                    moves[1] = moves[0];
                    wert[1] = wert[0];
                    moves[0] = 2;
                    wert[0] = 16;
                } else {
                    moves[1] = 2;
                    wert[1] = 16;
                }
            } else {
                moves[2] = 2;
                wert[2] = 16;
            }
            break;
        case 5:
            if (!(x & 0x84200LL &&
                  x & 0xE000000LL &&
                  x & 0x40041000LL &&
                  x & 0x41040LL &&
                  x & 0x1040040000LL))
                return true;
            if (wert[1] < 10) {
                moves[2] = moves[1];
                wert[2] = wert[1];
                if (wert[0] < 10) {
                    moves[1] = moves[0];
                    wert[1] = wert[0];
                    moves[0] = 2;
                    wert[0] = 10;
                } else {
                    moves[1] = 2;
                    wert[1] = 10;
                }
            } else {
                moves[2] = 2;
                wert[2] = 10;
            }
            break;
        default:
            moves[2] = -1;
            wert[2] = 0;
            break;
    }
    switch (Hoehe[5]) {
        case 0:
            if (!(x & 0x4210000LL &&
                  x & 0x20820000LL &&
                  x & 0x820020LL))
                return true;
            if (wert[2] < 9) {
                moves[3] = moves[2];
                wert[3] = wert[2];
                if (wert[1] < 9) {
                    moves[2] = moves[1];
                    wert[2] = wert[1];
                    if (wert[0] < 9) {
                        moves[1] = moves[0];
                        wert[1] = wert[0];
                        moves[0] = 5;
                        wert[0] = 9;
                    } else {
                        moves[1] = 5;
                        wert[1] = 9;
                    }
                } else {
                    moves[2] = 5;
                    wert[2] = 9;
                }
            } else {
                moves[3] = 5;
                wert[3] = 9;
            }
            break;
        case 1:
            if (!(x & 0x10410000LL &&
                  x & 0x410010LL &&
                  x & 0x108020LL &&
                  x & 0x2108000LL))
                return true;
            if (wert[2] < 13) {
                moves[3] = moves[2];
                wert[3] = wert[2];
                if (wert[1] < 13) {
                    moves[2] = moves[1];
                    wert[2] = wert[1];
                    if (wert[0] < 13) {
                        moves[1] = moves[0];
                        wert[1] = wert[0];
                        moves[0] = 5;
                        wert[0] = 13;
                    } else {
                        moves[1] = 5;
                        wert[1] = 13;
                    }
                } else {
                    moves[2] = 5;
                    wert[2] = 13;
                }
            } else {
                moves[3] = 5;
                wert[3] = 13;
            }
            break;
        case 2:
            if (!(x & 0x8208000LL &&
                  x & 0x208008LL &&
                  x & 0x84010LL &&
                  x & 0x810004LL &&
                  x & 0x1084000LL))
                return true;
            if (wert[2] < 17) {
                moves[3] = moves[2];
                wert[3] = wert[2];
                if (wert[1] < 17) {
                    moves[2] = moves[1];
                    wert[2] = wert[1];
                    if (wert[0] < 17) {
                        moves[1] = moves[0];
                        wert[1] = wert[0];
                        moves[0] = 5;
                        wert[0] = 17;
                    } else {
                        moves[1] = 5;
                        wert[1] = 17;
                    }
                } else {
                    moves[2] = 5;
                    wert[2] = 17;
                }
            } else {
                moves[3] = 5;
                wert[3] = 17;
            }
            break;
        case 3:
            if (!(x & 0x20408000LL &&
                  x & 0x4104000LL &&
                  x & 0xE00LL &&
                  x & 0x104004LL &&
                  x & 0x42008LL &&
                  x & 0x408002LL))
                return true;
            if (wert[2] < 16) {
                moves[3] = moves[2];
                wert[3] = wert[2];
                if (wert[1] < 16) {
                    moves[2] = moves[1];
                    wert[2] = wert[1];
                    if (wert[0] < 16) {
                        moves[1] = moves[0];
                        wert[1] = wert[0];
                        moves[0] = 5;
                        wert[0] = 16;
                    } else {
                        moves[1] = 5;
                        wert[1] = 16;
                    }
                } else {
                    moves[2] = 5;
                    wert[2] = 16;
                }
            } else {
                moves[3] = 5;
                wert[3] = 16;
            }
            break;
        case 4:
            if (!(x & 0x10204000LL &&
                  x & 0x2082000LL &&
                  x & 0x700LL &&
                  x & 0x82002LL &&
                  x & 0x204001LL))
                return true;
            if (wert[2] < 12) {
                moves[3] = moves[2];
                wert[3] = wert[2];
                if (wert[1] < 12) {
                    moves[2] = moves[1];
                    wert[2] = wert[1];
                    if (wert[0] < 12) {
                        moves[1] = moves[0];
                        wert[1] = wert[0];
                        moves[0] = 5;
                        wert[0] = 12;
                    } else {
                        moves[1] = 5;
                        wert[1] = 12;
                    }
                } else {
                    moves[2] = 5;
                    wert[2] = 12;
                }
            } else {
                moves[3] = 5;
                wert[3] = 12;
            }
            break;
        case 5:
            if (!(x & 0x8102000LL &&
                  x & 0x1041000LL &&
                  x & 0x380LL &&
                  x & 0x41001LL))
                return true;
            if (wert[2] < 8) {
                moves[3] = moves[2];
                wert[3] = wert[2];
                if (wert[1] < 8) {
                    moves[2] = moves[1];
                    wert[2] = wert[1];
                    if (wert[0] < 8) {
                        moves[1] = moves[0];
                        wert[1] = wert[0];
                        moves[0] = 5;
                        wert[0] = 8;
                    } else {
                        moves[1] = 5;
                        wert[1] = 8;
                    }
                } else {
                    moves[2] = 5;
                    wert[2] = 8;
                }
            } else {
                moves[3] = 5;
                wert[3] = 8;
            }
            break;
        default:
            moves[3] = -1;
            wert[3] = 0;
            break;
    }
    switch (Hoehe[1]) {
        case 0:
            if (!(x & 0x10204000LL &&
                  x & 0x20820000LL &&
                  x & 0x20020800000LL))
                return true;
            if (wert[3] < 9) {
                moves[4] = moves[3];
                wert[4] = wert[3];
                if (wert[2] < 9) {
                    moves[3] = moves[2];
                    wert[3] = wert[2];
                    if (wert[1] < 9) {
                        moves[2] = moves[1];
                        wert[2] = wert[1];
                        if (wert[0] < 9) {
                            moves[1] = moves[0];
                            wert[1] = wert[0];
                            moves[0] = 1;
                            wert[0] = 9;
                        } else {
                            moves[1] = 1;
                            wert[1] = 9;
                        }
                    } else {
                        moves[2] = 1;
                        wert[2] = 9;
                    }
                } else {
                    moves[3] = 1;
                    wert[3] = 9;
                }
            } else {
                moves[4] = 1;
                wert[4] = 9;
            }
            break;
        case 1:
            if (!(x & 0x10010400000LL &&
                  x & 0x20008100000LL &&
                  x & 0x10410000LL &&
                  x & 0x8102000LL))
                return true;
            if (wert[3] < 13) {
                moves[4] = moves[3];
                wert[4] = wert[3];
                if (wert[2] < 13) {
                    moves[3] = moves[2];
                    wert[3] = wert[2];
                    if (wert[1] < 13) {
                        moves[2] = moves[1];
                        wert[2] = wert[1];
                        if (wert[0] < 13) {
                            moves[1] = moves[0];
                            wert[1] = wert[0];
                            moves[0] = 1;
                            wert[0] = 13;
                        } else {
                            moves[1] = 1;
                            wert[1] = 13;
                        }
                    } else {
                        moves[2] = 1;
                        wert[2] = 13;
                    }
                } else {
                    moves[3] = 1;
                    wert[3] = 13;
                }
            } else {
                moves[4] = 1;
                wert[4] = 13;
            }
            break;
        case 2:
            if (!(x & 0x8008200000LL &&
                  x & 0x4010800000LL &&
                  x & 0x8208000LL &&
                  x & 0x10004080000LL &&
                  x & 0x4081000LL))
                return true;
            if (wert[3] < 17) {
                moves[4] = moves[3];
                wert[4] = wert[3];
                if (wert[2] < 17) {
                    moves[3] = moves[2];
                    wert[3] = wert[2];
                    if (wert[1] < 17) {
                        moves[2] = moves[1];
                        wert[2] = wert[1];
                        if (wert[0] < 17) {
                            moves[1] = moves[0];
                            wert[1] = wert[0];
                            moves[0] = 1;
                            wert[0] = 17;
                        } else {
                            moves[1] = 1;
                            wert[1] = 17;
                        }
                    } else {
                        moves[2] = 1;
                        wert[2] = 17;
                    }
                } else {
                    moves[3] = 1;
                    wert[3] = 17;
                }
            } else {
                moves[4] = 1;
                wert[4] = 17;
            }
            break;
        case 3:
            if (!(x & 0x8420000LL &&
                  x & 0xE00000000LL &&
                  x & 0x4104000LL &&
                  x & 0x4004100000LL &&
                  x & 0x8002040000LL &&
                  x & 0x2008400000LL))
                return true;
            if (wert[3] < 16) {
                moves[4] = moves[3];
                wert[4] = wert[3];
                if (wert[2] < 16) {
                    moves[3] = moves[2];
                    wert[3] = wert[2];
                    if (wert[1] < 16) {
                        moves[2] = moves[1];
                        wert[2] = wert[1];
                        if (wert[0] < 16) {
                            moves[1] = moves[0];
                            wert[1] = wert[0];
                            moves[0] = 1;
                            wert[0] = 16;
                        } else {
                            moves[1] = 1;
                            wert[1] = 16;
                        }
                    } else {
                        moves[2] = 1;
                        wert[2] = 16;
                    }
                } else {
                    moves[3] = 1;
                    wert[3] = 16;
                }
            } else {
                moves[4] = 1;
                wert[4] = 16;
            }
            break;
        case 4:
            if (!(x & 0x4210000LL &&
                  x & 0x2082000LL &&
                  x & 0x700000000LL &&
                  x & 0x2002080000LL &&
                  x & 0x1004200000LL))
                return true;
            if (wert[3] < 12) {
                moves[4] = moves[3];
                wert[4] = wert[3];
                if (wert[2] < 12) {
                    moves[3] = moves[2];
                    wert[3] = wert[2];
                    if (wert[1] < 12) {
                        moves[2] = moves[1];
                        wert[2] = wert[1];
                        if (wert[0] < 12) {
                            moves[1] = moves[0];
                            wert[1] = wert[0];
                            moves[0] = 1;
                            wert[0] = 12;
                        } else {
                            moves[1] = 1;
                            wert[1] = 12;
                        }
                    } else {
                        moves[2] = 1;
                        wert[2] = 12;
                    }
                } else {
                    moves[3] = 1;
                    wert[3] = 12;
                }
            } else {
                moves[4] = 1;
                wert[4] = 12;
            }
            break;
        case 5:
            if (!(x & 0x2108000LL &&
                  x & 0x1041000LL &&
                  x & 0x380000000LL &&
                  x & 0x1001040000LL))
                return true;
            if (wert[3] < 8) {
                moves[4] = moves[3];
                wert[4] = wert[3];
                if (wert[2] < 8) {
                    moves[3] = moves[2];
                    wert[3] = wert[2];
                    if (wert[1] < 8) {
                        moves[2] = moves[1];
                        wert[2] = wert[1];
                        if (wert[0] < 8) {
                            moves[1] = moves[0];
                            wert[1] = wert[0];
                            moves[0] = 1;
                            wert[0] = 8;
                        } else {
                            moves[1] = 1;
                            wert[1] = 8;
                        }
                    } else {
                        moves[2] = 1;
                        wert[2] = 8;
                    }
                } else {
                    moves[3] = 1;
                    wert[3] = 8;
                }
            } else {
                moves[4] = 1;
                wert[4] = 8;
            }
            break;
        default:
            moves[4] = -1;
            wert[4] = 0;
            break;
    }
    switch (Hoehe[6]) {
        case 0:
            if (!(x & 0x108400LL &&
                  x & 0x820800LL))
                return true;
            if (wert[4] < 7) {
                moves[5] = moves[4];
                wert[5] = wert[4];
                if (wert[3] < 7) {
                    moves[4] = moves[3];
                    wert[4] = wert[3];
                    if (wert[2] < 7) {
                        moves[3] = moves[2];
                        wert[3] = wert[2];
                        if (wert[1] < 7) {
                            moves[2] = moves[1];
                            wert[2] = wert[1];
                            if (wert[0] < 7) {
                                moves[1] = moves[0];
                                wert[1] = wert[0];
                                moves[0] = 6;
                                wert[0] = 7;
                            } else {
                                moves[1] = 6;
                                wert[1] = 7;
                            }
                        } else {
                            moves[2] = 6;
                            wert[2] = 7;
                        }
                    } else {
                        moves[3] = 6;
                        wert[3] = 7;
                    }
                } else {
                    moves[4] = 6;
                    wert[4] = 7;
                }
            } else {
                moves[5] = 6;
                wert[5] = 7;
            }
            break;
        case 1:
            if (!(x & 0x410400LL &&
                  x & 0x84200LL))
                return true;
            if (wert[4] < 9) {
                moves[5] = moves[4];
                wert[5] = wert[4];
                if (wert[3] < 9) {
                    moves[4] = moves[3];
                    wert[4] = wert[3];
                    if (wert[2] < 9) {
                        moves[3] = moves[2];
                        wert[3] = wert[2];
                        if (wert[1] < 9) {
                            moves[2] = moves[1];
                            wert[2] = wert[1];
                            if (wert[0] < 9) {
                                moves[1] = moves[0];
                                wert[1] = wert[0];
                                moves[0] = 6;
                                wert[0] = 9;
                            } else {
                                moves[1] = 6;
                                wert[1] = 9;
                            }
                        } else {
                            moves[2] = 6;
                            wert[2] = 9;
                        }
                    } else {
                        moves[3] = 6;
                        wert[3] = 9;
                    }
                } else {
                    moves[4] = 6;
                    wert[4] = 9;
                }
            } else {
                moves[5] = 6;
                wert[5] = 9;
            }
            break;
        case 2:
            if (!(x & 0x208200LL &&
                  x & 0x42100LL))
                return true;
            if (wert[4] < 11) {
                moves[5] = moves[4];
                wert[5] = wert[4];
                if (wert[3] < 11) {
                    moves[4] = moves[3];
                    wert[4] = wert[3];
                    if (wert[2] < 11) {
                        moves[3] = moves[2];
                        wert[3] = wert[2];
                        if (wert[1] < 11) {
                            moves[2] = moves[1];
                            wert[2] = wert[1];
                            if (wert[0] < 11) {
                                moves[1] = moves[0];
                                wert[1] = wert[0];
                                moves[0] = 6;
                                wert[0] = 11;
                            } else {
                                moves[1] = 6;
                                wert[1] = 11;
                            }
                        } else {
                            moves[2] = 6;
                            wert[2] = 11;
                        }
                    } else {
                        moves[3] = 6;
                        wert[3] = 11;
                    }
                } else {
                    moves[4] = 6;
                    wert[4] = 11;
                }
            } else {
                moves[5] = 6;
                wert[5] = 11;
            }
            break;
        case 3:
            if (!(x & 0x810200LL &&
                  x & 0x38LL &&
                  x & 0x104100LL))
                return true;
            if (wert[4] < 10) {
                moves[5] = moves[4];
                wert[5] = wert[4];
                if (wert[3] < 10) {
                    moves[4] = moves[3];
                    wert[4] = wert[3];
                    if (wert[2] < 10) {
                        moves[3] = moves[2];
                        wert[3] = wert[2];
                        if (wert[1] < 10) {
                            moves[2] = moves[1];
                            wert[2] = wert[1];
                            if (wert[0] < 10) {
                                moves[1] = moves[0];
                                wert[1] = wert[0];
                                moves[0] = 6;
                                wert[0] = 10;
                            } else {
                                moves[1] = 6;
                                wert[1] = 10;
                            }
                        } else {
                            moves[2] = 6;
                            wert[2] = 10;
                        }
                    } else {
                        moves[3] = 6;
                        wert[3] = 10;
                    }
                } else {
                    moves[4] = 6;
                    wert[4] = 10;
                }
            } else {
                moves[5] = 6;
                wert[5] = 10;
            }
            break;
        case 4:
            if (!(x & 0x408100LL &&
                  x & 0x1CLL &&
                  x & 0x82080LL))
                return true;
            if (wert[4] < 8) {
                moves[5] = moves[4];
                wert[5] = wert[4];
                if (wert[3] < 8) {
                    moves[4] = moves[3];
                    wert[4] = wert[3];
                    if (wert[2] < 8) {
                        moves[3] = moves[2];
                        wert[3] = wert[2];
                        if (wert[1] < 8) {
                            moves[2] = moves[1];
                            wert[2] = wert[1];
                            if (wert[0] < 8) {
                                moves[1] = moves[0];
                                wert[1] = wert[0];
                                moves[0] = 6;
                                wert[0] = 8;
                            } else {
                                moves[1] = 6;
                                wert[1] = 8;
                            }
                        } else {
                            moves[2] = 6;
                            wert[2] = 8;
                        }
                    } else {
                        moves[3] = 6;
                        wert[3] = 8;
                    }
                } else {
                    moves[4] = 6;
                    wert[4] = 8;
                }
            } else {
                moves[5] = 6;
                wert[5] = 8;
            }
            break;
        case 5:
            if (!(x & 0x204080LL &&
                  x & 0xELL &&
                  x & 0x41040LL))
                return true;
            if (wert[4] < 6) {
                moves[5] = moves[4];
                wert[5] = wert[4];
                if (wert[3] < 6) {
                    moves[4] = moves[3];
                    wert[4] = wert[3];
                    if (wert[2] < 6) {
                        moves[3] = moves[2];
                        wert[3] = wert[2];
                        if (wert[1] < 6) {
                            moves[2] = moves[1];
                            wert[2] = wert[1];
                            if (wert[0] < 6) {
                                moves[1] = moves[0];
                                wert[1] = wert[0];
                                moves[0] = 6;
                                wert[0] = 6;
                            } else {
                                moves[1] = 6;
                                wert[1] = 6;
                            }
                        } else {
                            moves[2] = 6;
                            wert[2] = 6;
                        }
                    } else {
                        moves[3] = 6;
                        wert[3] = 6;
                    }
                } else {
                    moves[4] = 6;
                    wert[4] = 6;
                }
            } else {
                moves[5] = 6;
                wert[5] = 6;
            }
            break;
        default:
            moves[5] = -1;
            wert[5] = 0;
            break;
    }
    switch (Hoehe[0]) {
        case 0:
            if (!(x & 0x408100000LL &&
                  x & 0x820800000LL))
                return true;
            if (wert[5] < 7) {
                moves[6] = moves[5];
                if (wert[4] < 7) {
                    moves[5] = moves[4];
                    wert[5] = wert[4];
                    if (wert[3] < 7) {
                        moves[4] = moves[3];
                        wert[4] = wert[3];
                        if (wert[2] < 7) {
                            moves[3] = moves[2];
                            wert[3] = wert[2];
                            if (wert[1] < 7) {
                                moves[2] = moves[1];
                                wert[2] = wert[1];
                                if (wert[0] < 7) {
                                    moves[1] = moves[0];
                                    wert[1] = wert[0];
                                    moves[0] = 0;
                                } else
                                    moves[1] = 0;
                            } else
                                moves[2] = 0;
                        } else
                            moves[3] = 0;
                    } else
                        moves[4] = 0;
                } else
                    moves[5] = 0;
            } else
                moves[6] = 0;
            break;
        case 1:
            if (!(x & 0x410400000LL &&
                  x & 0x204080000LL))
                return true;
            if (wert[5] < 9) {
                moves[6] = moves[5];
                if (wert[4] < 9) {
                    moves[5] = moves[4];
                    wert[5] = wert[4];
                    if (wert[3] < 9) {
                        moves[4] = moves[3];
                        wert[4] = wert[3];
                        if (wert[2] < 9) {
                            moves[3] = moves[2];
                            wert[3] = wert[2];
                            if (wert[1] < 9) {
                                moves[2] = moves[1];
                                wert[2] = wert[1];
                                if (wert[0] < 9) {
                                    moves[1] = moves[0];
                                    wert[1] = wert[0];
                                    moves[0] = 0;
                                } else
                                    moves[1] = 0;
                            } else
                                moves[2] = 0;
                        } else
                            moves[3] = 0;
                    } else
                        moves[4] = 0;
                } else
                    moves[5] = 0;
            } else
                moves[6] = 0;
            break;
        case 2:
            if (!(x & 0x208200000LL &&
                  x & 0x102040000LL))
                return true;
            if (wert[5] < 11) {
                moves[6] = moves[5];
                if (wert[4] < 11) {
                    moves[5] = moves[4];
                    wert[5] = wert[4];
                    if (wert[3] < 11) {
                        moves[4] = moves[3];
                        wert[4] = wert[3];
                        if (wert[2] < 11) {
                            moves[3] = moves[2];
                            wert[3] = wert[2];
                            if (wert[1] < 11) {
                                moves[2] = moves[1];
                                wert[2] = wert[1];
                                if (wert[0] < 11) {
                                    moves[1] = moves[0];
                                    wert[1] = wert[0];
                                    moves[0] = 0;
                                } else
                                    moves[1] = 0;
                            } else
                                moves[2] = 0;
                        } else
                            moves[3] = 0;
                    } else
                        moves[4] = 0;
                } else
                    moves[5] = 0;
            } else
                moves[6] = 0;
            break;
        case 3:
            if (!(x & 0x38000000000LL &&
                  x & 0x210800000LL &&
                  x & 0x104100000LL))
                return true;
            if (wert[5] < 10) {
                moves[6] = moves[5];
                if (wert[4] < 10) {
                    moves[5] = moves[4];
                    wert[5] = wert[4];
                    if (wert[3] < 10) {
                        moves[4] = moves[3];
                        wert[4] = wert[3];
                        if (wert[2] < 10) {
                            moves[3] = moves[2];
                            wert[3] = wert[2];
                            if (wert[1] < 10) {
                                moves[2] = moves[1];
                                wert[2] = wert[1];
                                if (wert[0] < 10) {
                                    moves[1] = moves[0];
                                    wert[1] = wert[0];
                                    moves[0] = 0;
                                } else
                                    moves[1] = 0;
                            } else
                                moves[2] = 0;
                        } else
                            moves[3] = 0;
                    } else
                        moves[4] = 0;
                } else
                    moves[5] = 0;
            } else
                moves[6] = 0;
            break;
        case 4:
            if (!(x & 0x108400000LL &&
                  x & 0x1C000000000LL &&
                  x & 0x82080000LL))
                return true;
            if (wert[5] < 8) {
                moves[6] = moves[5];
                if (wert[4] < 8) {
                    moves[5] = moves[4];
                    wert[5] = wert[4];
                    if (wert[3] < 8) {
                        moves[4] = moves[3];
                        wert[4] = wert[3];
                        if (wert[2] < 8) {
                            moves[3] = moves[2];
                            wert[3] = wert[2];
                            if (wert[1] < 8) {
                                moves[2] = moves[1];
                                wert[2] = wert[1];
                                if (wert[0] < 8) {
                                    moves[1] = moves[0];
                                    wert[1] = wert[0];
                                    moves[0] = 0;
                                } else
                                    moves[1] = 0;
                            } else
                                moves[2] = 0;
                        } else
                            moves[3] = 0;
                    } else
                        moves[4] = 0;
                } else
                    moves[5] = 0;
            } else
                moves[6] = 0;
            break;
        case 5:
            if (!(x & 0x84200000LL &&
                  x & 0xE000000000LL &&
                  x & 0x41040000LL))
                return true;
            if (wert[5] < 6) {
                moves[6] = moves[5];
                if (wert[4] < 6) {
                    moves[5] = moves[4];
                    wert[5] = wert[4];
                    if (wert[3] < 6) {
                        moves[4] = moves[3];
                        wert[4] = wert[3];
                        if (wert[2] < 6) {
                            moves[3] = moves[2];
                            wert[3] = wert[2];
                            if (wert[1] < 6) {
                                moves[2] = moves[1];
                                wert[2] = wert[1];
                                if (wert[0] < 6) {
                                    moves[1] = moves[0];
                                    wert[1] = wert[0];
                                    moves[0] = 0;
                                } else
                                    moves[1] = 0;
                            } else
                                moves[2] = 0;
                        } else
                            moves[3] = 0;
                    } else
                        moves[4] = 0;
                } else
                    moves[5] = 0;
            } else
                moves[6] = 0;
            break;
        default:
            moves[6] = -1;
            break;
    }
    moves[7] = -1;
    return false;
}


bool SpielBeenden2Generate(short int *moves) {
    short int wert[6];
    int64_t x = ~Feld2;
    switch (Hoehe[3]) {
        case 0:
            if (!(x & 0x20820000000LL &&
                  x & 0x820020000LL &&
                  x & 0x20020800LL &&
                  x & 0x20820LL &&
                  x & 0x10204LL &&
                  x & 0x4210000000LL))
                return true;
            moves[0] = 3;
            wert[0] = 15;
            break;
        case 1:
            if (!(x & 0x20008100LL &&
                  x & 0x10410LL &&
                  x & 0x10410000000LL &&
                  x & 0x2108000000LL &&
                  x & 0x108020000LL &&
                  x & 0x10010400LL &&
                  x & 0x8102LL &&
                  x & 0x410010000LL))
                return true;
            moves[0] = 3;
            wert[0] = 21;
            break;
        case 2:
            if (!(x & 0x810004000LL &&
                  x & 0x4010800LL &&
                  x & 0x8208000000LL &&
                  x & 0x208008000LL &&
                  x & 0x8008200LL &&
                  x & 0x8208LL &&
                  x & 0x84010000LL &&
                  x & 0x10004080LL &&
                  x & 0x4081LL &&
                  x & 0x1084000000LL))
                return true;
            moves[0] = 3;
            wert[0] = 27;
            break;
        case 3:
            if (!(x & 0x20408000000LL &&
                  x & 0x8420LL &&
                  x & 0xE00000LL &&
                  x & 0x4104000000LL &&
                  x & 0x2008400LL &&
                  x & 0x408002000LL &&
                  x & 0x4004100LL &&
                  x & 0x104004000LL &&
                  x & 0x42008000LL &&
                  x & 0x8002040LL &&
                  x & 0x4104LL))
                return true;
            moves[0] = 3;
            wert[0] = 26;
            break;
        case 4:
            if (!(x & 0x10204000000LL &&
                  x & 0x700000LL &&
                  x & 0x4210LL &&
                  x & 0x204001000LL &&
                  x & 0x2082000000LL &&
                  x & 0x1004200LL &&
                  x & 0x82002000LL &&
                  x & 0x2002080LL &&
                  x & 0x2082LL))
                return true;
            moves[0] = 3;
            wert[0] = 20;
            break;
        case 5:
            if (!(x & 0x380000LL &&
                  x & 0x8102000000LL &&
                  x & 0x2108LL &&
                  x & 0x1041000000LL &&
                  x & 0x41001000LL &&
                  x & 0x1001040LL &&
                  x & 0x1041LL))
                return true;
            moves[0] = 3;
            wert[0] = 14;
            break;
        default:
            moves[0] = -1;
            wert[0] = 0;
            break;
    }
    switch (Hoehe[4]) {
        case 0:
            if (!(x & 0x108400000LL &&
                  x & 0x800820LL &&
                  x & 0x20800800LL &&
                  x & 0x820800000LL))
                return true;
            if (wert[0] < 11) {
                moves[1] = moves[0];
                wert[1] = wert[0];
                moves[0] = 4;
                wert[0] = 11;
            } else {
                moves[1] = 4;
                wert[1] = 11;
            }
            break;
        case 1:
            if (!(x & 0x800204LL &&
                  x & 0x4200800LL &&
                  x & 0x400410LL &&
                  x & 0x10400400LL &&
                  x & 0x410400000LL &&
                  x & 0x84200000LL))
                return true;
            if (wert[0] < 17) {
                moves[1] = moves[0];
                wert[1] = wert[0];
                moves[0] = 4;
                wert[0] = 17;
            } else {
                moves[1] = 4;
                wert[1] = 17;
            }
            break;
        case 2:
            if (!(x & 0x100420LL &&
                  x & 0x20400100LL &&
                  x & 0x8200200LL &&
                  x & 0x208200000LL &&
                  x & 0x200208LL &&
                  x & 0x2100400LL &&
                  x & 0x400102LL &&
                  x & 0x42100000LL))
                return true;
            if (wert[0] < 23) {
                moves[1] = moves[0];
                wert[1] = wert[0];
                moves[0] = 4;
                wert[0] = 23;
            } else {
                moves[1] = 4;
                wert[1] = 23;
            }
            break;
        case 3:
            if (!(x & 0x810200000LL &&
                  x & 0x80210LL &&
                  x & 0x38000LL &&
                  x & 0x4100100LL &&
                  x & 0x104100000LL &&
                  x & 0x10200080LL &&
                  x & 0x1080200LL &&
                  x & 0x100104LL &&
                  x & 0x200081LL))
                return true;
            if (wert[0] < 22) {
                moves[1] = moves[0];
                wert[1] = wert[0];
                moves[0] = 4;
                wert[0] = 22;
            } else {
                moves[1] = 4;
                wert[1] = 22;
            }
            break;
        case 4:
            if (!(x & 0x408100000LL &&
                  x & 0x40108LL &&
                  x & 0x1C000LL &&
                  x & 0x2080080LL &&
                  x & 0x82080000LL &&
                  x & 0x8100040LL &&
                  x & 0x80082LL))
                return true;
            if (wert[0] < 16) {
                moves[1] = moves[0];
                wert[1] = wert[0];
                moves[0] = 4;
                wert[0] = 16;
            } else {
                moves[1] = 4;
                wert[1] = 16;
            }
            break;
        case 5:
            if (!(x & 0x204080000LL &&
                  x & 0xE000LL &&
                  x & 0x1040040LL &&
                  x & 0x41040000LL &&
                  x & 0x40041LL))
                return true;
            if (wert[0] < 10) {
                moves[1] = moves[0];
                wert[1] = wert[0];
                moves[0] = 4;
                wert[0] = 10;
            } else {
                moves[1] = 4;
                wert[1] = 10;
            }
            break;
        default:
            moves[1] = -1;
            wert[1] = 0;
            break;
    }
    switch (Hoehe[2]) {
        case 0:
            if (!(x & 0x408100LL &&
                  x & 0x20800800000LL &&
                  x & 0x820800LL &&
                  x & 0x800820000LL))
                return true;
            if (wert[1] < 11) {
                moves[2] = moves[1];
                wert[2] = wert[1];
                if (wert[0] < 11) {
                    moves[1] = moves[0];
                    wert[1] = wert[0];
                    moves[0] = 2;
                    wert[0] = 11;
                } else {
                    moves[1] = 2;
                    wert[1] = 11;
                }
            } else {
                moves[2] = 2;
                wert[2] = 11;
            }
            break;
        case 1:
            if (!(x & 0x4200800000LL &&
                  x & 0x10400400000LL &&
                  x & 0x800204000LL &&
                  x & 0x410400LL &&
                  x & 0x400410000LL &&
                  x & 0x204080LL))
                return true;
            if (wert[1] < 17) {
                moves[2] = moves[1];
                wert[2] = wert[1];
                if (wert[0] < 17) {
                    moves[1] = moves[0];
                    wert[1] = wert[0];
                    moves[0] = 2;
                    wert[0] = 17;
                } else {
                    moves[1] = 2;
                    wert[1] = 17;
                }
            } else {
                moves[2] = 2;
                wert[2] = 17;
            }
            break;
        case 2:
            if (!(x & 0x100420000LL &&
                  x & 0x8200200000LL &&
                  x & 0x20400100000LL &&
                  x & 0x200208000LL &&
                  x & 0x208200LL &&
                  x & 0x400102000LL &&
                  x & 0x2100400000LL &&
                  x & 0x102040LL))
                return true;
            if (wert[1] < 23) {
                moves[2] = moves[1];
                wert[2] = wert[1];
                if (wert[0] < 23) {
                    moves[1] = moves[0];
                    wert[1] = wert[0];
                    moves[0] = 2;
                    wert[0] = 23;
                } else {
                    moves[1] = 2;
                    wert[1] = 23;
                }
            } else {
                moves[2] = 2;
                wert[2] = 23;
            }
            break;
        case 3:
            if (!(x & 0x210800LL &&
                  x & 0x10200080000LL &&
                  x & 0x4100100000LL &&
                  x & 0x100104000LL &&
                  x & 0x200081000LL &&
                  x & 0x80210000LL &&
                  x & 0x38000000LL &&
                  x & 0x104100LL &&
                  x & 0x1080200000LL))
                return true;
            if (wert[1] < 22) {
                moves[2] = moves[1];
                wert[2] = wert[1];
                if (wert[0] < 22) {
                    moves[1] = moves[0];
                    wert[1] = wert[0];
                    moves[0] = 2;
                    wert[0] = 22;
                } else {
                    moves[1] = 2;
                    wert[1] = 22;
                }
            } else {
                moves[2] = 2;
                wert[2] = 22;
            }
            break;
        case 4:
            if (!(x & 0x108400LL &&
                  x & 0x8100040000LL &&
                  x & 0x80082000LL &&
                  x & 0x40108000LL &&
                  x & 0x82080LL &&
                  x & 0x1C000000LL &&
                  x & 0x2080080000LL))
                return true;
            if (wert[1] < 16) {
                moves[2] = moves[1];
                wert[2] = wert[1];
                if (wert[0] < 16) {
                    moves[1] = moves[0];
                    wert[1] = wert[0];
                    moves[0] = 2;
                    wert[0] = 16;
                } else {
                    moves[1] = 2;
                    wert[1] = 16;
                }
            } else {
                moves[2] = 2;
                wert[2] = 16;
            }
            break;
        case 5:
            if (!(x & 0x84200LL &&
                  x & 0xE000000LL &&
                  x & 0x40041000LL &&
                  x & 0x41040LL &&
                  x & 0x1040040000LL))
                return true;
            if (wert[1] < 10) {
                moves[2] = moves[1];
                wert[2] = wert[1];
                if (wert[0] < 10) {
                    moves[1] = moves[0];
                    wert[1] = wert[0];
                    moves[0] = 2;
                    wert[0] = 10;
                } else {
                    moves[1] = 2;
                    wert[1] = 10;
                }
            } else {
                moves[2] = 2;
                wert[2] = 10;
            }
            break;
        default:
            moves[2] = -1;
            wert[2] = 0;
            break;
    }
    switch (Hoehe[5]) {
        case 0:
            if (!(x & 0x4210000LL &&
                  x & 0x20820000LL &&
                  x & 0x820020LL))
                return true;
            if (wert[2] < 9) {
                moves[3] = moves[2];
                wert[3] = wert[2];
                if (wert[1] < 9) {
                    moves[2] = moves[1];
                    wert[2] = wert[1];
                    if (wert[0] < 9) {
                        moves[1] = moves[0];
                        wert[1] = wert[0];
                        moves[0] = 5;
                        wert[0] = 9;
                    } else {
                        moves[1] = 5;
                        wert[1] = 9;
                    }
                } else {
                    moves[2] = 5;
                    wert[2] = 9;
                }
            } else {
                moves[3] = 5;
                wert[3] = 9;
            }
            break;
        case 1:
            if (!(x & 0x10410000LL &&
                  x & 0x410010LL &&
                  x & 0x108020LL &&
                  x & 0x2108000LL))
                return true;
            if (wert[2] < 13) {
                moves[3] = moves[2];
                wert[3] = wert[2];
                if (wert[1] < 13) {
                    moves[2] = moves[1];
                    wert[2] = wert[1];
                    if (wert[0] < 13) {
                        moves[1] = moves[0];
                        wert[1] = wert[0];
                        moves[0] = 5;
                        wert[0] = 13;
                    } else {
                        moves[1] = 5;
                        wert[1] = 13;
                    }
                } else {
                    moves[2] = 5;
                    wert[2] = 13;
                }
            } else {
                moves[3] = 5;
                wert[3] = 13;
            }
            break;
        case 2:
            if (!(x & 0x8208000LL &&
                  x & 0x208008LL &&
                  x & 0x84010LL &&
                  x & 0x810004LL &&
                  x & 0x1084000LL))
                return true;
            if (wert[2] < 17) {
                moves[3] = moves[2];
                wert[3] = wert[2];
                if (wert[1] < 17) {
                    moves[2] = moves[1];
                    wert[2] = wert[1];
                    if (wert[0] < 17) {
                        moves[1] = moves[0];
                        wert[1] = wert[0];
                        moves[0] = 5;
                        wert[0] = 17;
                    } else {
                        moves[1] = 5;
                        wert[1] = 17;
                    }
                } else {
                    moves[2] = 5;
                    wert[2] = 17;
                }
            } else {
                moves[3] = 5;
                wert[3] = 17;
            }
            break;
        case 3:
            if (!(x & 0x20408000LL &&
                  x & 0x4104000LL &&
                  x & 0xE00LL &&
                  x & 0x104004LL &&
                  x & 0x42008LL &&
                  x & 0x408002LL))
                return true;
            if (wert[2] < 16) {
                moves[3] = moves[2];
                wert[3] = wert[2];
                if (wert[1] < 16) {
                    moves[2] = moves[1];
                    wert[2] = wert[1];
                    if (wert[0] < 16) {
                        moves[1] = moves[0];
                        wert[1] = wert[0];
                        moves[0] = 5;
                        wert[0] = 16;
                    } else {
                        moves[1] = 5;
                        wert[1] = 16;
                    }
                } else {
                    moves[2] = 5;
                    wert[2] = 16;
                }
            } else {
                moves[3] = 5;
                wert[3] = 16;
            }
            break;
        case 4:
            if (!(x & 0x10204000LL &&
                  x & 0x2082000LL &&
                  x & 0x700LL &&
                  x & 0x82002LL &&
                  x & 0x204001LL))
                return true;
            if (wert[2] < 12) {
                moves[3] = moves[2];
                wert[3] = wert[2];
                if (wert[1] < 12) {
                    moves[2] = moves[1];
                    wert[2] = wert[1];
                    if (wert[0] < 12) {
                        moves[1] = moves[0];
                        wert[1] = wert[0];
                        moves[0] = 5;
                        wert[0] = 12;
                    } else {
                        moves[1] = 5;
                        wert[1] = 12;
                    }
                } else {
                    moves[2] = 5;
                    wert[2] = 12;
                }
            } else {
                moves[3] = 5;
                wert[3] = 12;
            }
            break;
        case 5:
            if (!(x & 0x8102000LL &&
                  x & 0x1041000LL &&
                  x & 0x380LL &&
                  x & 0x41001LL))
                return true;
            if (wert[2] < 8) {
                moves[3] = moves[2];
                wert[3] = wert[2];
                if (wert[1] < 8) {
                    moves[2] = moves[1];
                    wert[2] = wert[1];
                    if (wert[0] < 8) {
                        moves[1] = moves[0];
                        wert[1] = wert[0];
                        moves[0] = 5;
                        wert[0] = 8;
                    } else {
                        moves[1] = 5;
                        wert[1] = 8;
                    }
                } else {
                    moves[2] = 5;
                    wert[2] = 8;
                }
            } else {
                moves[3] = 5;
                wert[3] = 8;
            }
            break;
        default:
            moves[3] = -1;
            wert[3] = 0;
            break;
    }
    switch (Hoehe[1]) {
        case 0:
            if (!(x & 0x10204000LL &&
                  x & 0x20820000LL &&
                  x & 0x20020800000LL))
                return true;
            if (wert[3] < 9) {
                moves[4] = moves[3];
                wert[4] = wert[3];
                if (wert[2] < 9) {
                    moves[3] = moves[2];
                    wert[3] = wert[2];
                    if (wert[1] < 9) {
                        moves[2] = moves[1];
                        wert[2] = wert[1];
                        if (wert[0] < 9) {
                            moves[1] = moves[0];
                            wert[1] = wert[0];
                            moves[0] = 1;
                            wert[0] = 9;
                        } else {
                            moves[1] = 1;
                            wert[1] = 9;
                        }
                    } else {
                        moves[2] = 1;
                        wert[2] = 9;
                    }
                } else {
                    moves[3] = 1;
                    wert[3] = 9;
                }
            } else {
                moves[4] = 1;
                wert[4] = 9;
            }
            break;
        case 1:
            if (!(x & 0x10010400000LL &&
                  x & 0x20008100000LL &&
                  x & 0x10410000LL &&
                  x & 0x8102000LL))
                return true;
            if (wert[3] < 13) {
                moves[4] = moves[3];
                wert[4] = wert[3];
                if (wert[2] < 13) {
                    moves[3] = moves[2];
                    wert[3] = wert[2];
                    if (wert[1] < 13) {
                        moves[2] = moves[1];
                        wert[2] = wert[1];
                        if (wert[0] < 13) {
                            moves[1] = moves[0];
                            wert[1] = wert[0];
                            moves[0] = 1;
                            wert[0] = 13;
                        } else {
                            moves[1] = 1;
                            wert[1] = 13;
                        }
                    } else {
                        moves[2] = 1;
                        wert[2] = 13;
                    }
                } else {
                    moves[3] = 1;
                    wert[3] = 13;
                }
            } else {
                moves[4] = 1;
                wert[4] = 13;
            }
            break;
        case 2:
            if (!(x & 0x8008200000LL &&
                  x & 0x4010800000LL &&
                  x & 0x8208000LL &&
                  x & 0x10004080000LL &&
                  x & 0x4081000LL))
                return true;
            if (wert[3] < 17) {
                moves[4] = moves[3];
                wert[4] = wert[3];
                if (wert[2] < 17) {
                    moves[3] = moves[2];
                    wert[3] = wert[2];
                    if (wert[1] < 17) {
                        moves[2] = moves[1];
                        wert[2] = wert[1];
                        if (wert[0] < 17) {
                            moves[1] = moves[0];
                            wert[1] = wert[0];
                            moves[0] = 1;
                            wert[0] = 17;
                        } else {
                            moves[1] = 1;
                            wert[1] = 17;
                        }
                    } else {
                        moves[2] = 1;
                        wert[2] = 17;
                    }
                } else {
                    moves[3] = 1;
                    wert[3] = 17;
                }
            } else {
                moves[4] = 1;
                wert[4] = 17;
            }
            break;
        case 3:
            if (!(x & 0x8420000LL &&
                  x & 0xE00000000LL &&
                  x & 0x4104000LL &&
                  x & 0x4004100000LL &&
                  x & 0x8002040000LL &&
                  x & 0x2008400000LL))
                return true;
            if (wert[3] < 16) {
                moves[4] = moves[3];
                wert[4] = wert[3];
                if (wert[2] < 16) {
                    moves[3] = moves[2];
                    wert[3] = wert[2];
                    if (wert[1] < 16) {
                        moves[2] = moves[1];
                        wert[2] = wert[1];
                        if (wert[0] < 16) {
                            moves[1] = moves[0];
                            wert[1] = wert[0];
                            moves[0] = 1;
                            wert[0] = 16;
                        } else {
                            moves[1] = 1;
                            wert[1] = 16;
                        }
                    } else {
                        moves[2] = 1;
                        wert[2] = 16;
                    }
                } else {
                    moves[3] = 1;
                    wert[3] = 16;
                }
            } else {
                moves[4] = 1;
                wert[4] = 16;
            }
            break;
        case 4:
            if (!(x & 0x4210000LL &&
                  x & 0x2082000LL &&
                  x & 0x700000000LL &&
                  x & 0x2002080000LL &&
                  x & 0x1004200000LL))
                return true;
            if (wert[3] < 12) {
                moves[4] = moves[3];
                wert[4] = wert[3];
                if (wert[2] < 12) {
                    moves[3] = moves[2];
                    wert[3] = wert[2];
                    if (wert[1] < 12) {
                        moves[2] = moves[1];
                        wert[2] = wert[1];
                        if (wert[0] < 12) {
                            moves[1] = moves[0];
                            wert[1] = wert[0];
                            moves[0] = 1;
                            wert[0] = 12;
                        } else {
                            moves[1] = 1;
                            wert[1] = 12;
                        }
                    } else {
                        moves[2] = 1;
                        wert[2] = 12;
                    }
                } else {
                    moves[3] = 1;
                    wert[3] = 12;
                }
            } else {
                moves[4] = 1;
                wert[4] = 12;
            }
            break;
        case 5:
            if (!(x & 0x2108000LL &&
                  x & 0x1041000LL &&
                  x & 0x380000000LL &&
                  x & 0x1001040000LL))
                return true;
            if (wert[3] < 8) {
                moves[4] = moves[3];
                wert[4] = wert[3];
                if (wert[2] < 8) {
                    moves[3] = moves[2];
                    wert[3] = wert[2];
                    if (wert[1] < 8) {
                        moves[2] = moves[1];
                        wert[2] = wert[1];
                        if (wert[0] < 8) {
                            moves[1] = moves[0];
                            wert[1] = wert[0];
                            moves[0] = 1;
                            wert[0] = 8;
                        } else {
                            moves[1] = 1;
                            wert[1] = 8;
                        }
                    } else {
                        moves[2] = 1;
                        wert[2] = 8;
                    }
                } else {
                    moves[3] = 1;
                    wert[3] = 8;
                }
            } else {
                moves[4] = 1;
                wert[4] = 8;
            }
            break;
        default:
            moves[4] = -1;
            wert[4] = 0;
            break;
    }
    switch (Hoehe[6]) {
        case 0:
            if (!(x & 0x108400LL &&
                  x & 0x820800LL))
                return true;
            if (wert[4] < 7) {
                moves[5] = moves[4];
                wert[5] = wert[4];
                if (wert[3] < 7) {
                    moves[4] = moves[3];
                    wert[4] = wert[3];
                    if (wert[2] < 7) {
                        moves[3] = moves[2];
                        wert[3] = wert[2];
                        if (wert[1] < 7) {
                            moves[2] = moves[1];
                            wert[2] = wert[1];
                            if (wert[0] < 7) {
                                moves[1] = moves[0];
                                wert[1] = wert[0];
                                moves[0] = 6;
                                wert[0] = 7;
                            } else {
                                moves[1] = 6;
                                wert[1] = 7;
                            }
                        } else {
                            moves[2] = 6;
                            wert[2] = 7;
                        }
                    } else {
                        moves[3] = 6;
                        wert[3] = 7;
                    }
                } else {
                    moves[4] = 6;
                    wert[4] = 7;
                }
            } else {
                moves[5] = 6;
                wert[5] = 7;
            }
            break;
        case 1:
            if (!(x & 0x410400LL &&
                  x & 0x84200LL))
                return true;
            if (wert[4] < 9) {
                moves[5] = moves[4];
                wert[5] = wert[4];
                if (wert[3] < 9) {
                    moves[4] = moves[3];
                    wert[4] = wert[3];
                    if (wert[2] < 9) {
                        moves[3] = moves[2];
                        wert[3] = wert[2];
                        if (wert[1] < 9) {
                            moves[2] = moves[1];
                            wert[2] = wert[1];
                            if (wert[0] < 9) {
                                moves[1] = moves[0];
                                wert[1] = wert[0];
                                moves[0] = 6;
                                wert[0] = 9;
                            } else {
                                moves[1] = 6;
                                wert[1] = 9;
                            }
                        } else {
                            moves[2] = 6;
                            wert[2] = 9;
                        }
                    } else {
                        moves[3] = 6;
                        wert[3] = 9;
                    }
                } else {
                    moves[4] = 6;
                    wert[4] = 9;
                }
            } else {
                moves[5] = 6;
                wert[5] = 9;
            }
            break;
        case 2:
            if (!(x & 0x208200LL &&
                  x & 0x42100LL))
                return true;
            if (wert[4] < 11) {
                moves[5] = moves[4];
                wert[5] = wert[4];
                if (wert[3] < 11) {
                    moves[4] = moves[3];
                    wert[4] = wert[3];
                    if (wert[2] < 11) {
                        moves[3] = moves[2];
                        wert[3] = wert[2];
                        if (wert[1] < 11) {
                            moves[2] = moves[1];
                            wert[2] = wert[1];
                            if (wert[0] < 11) {
                                moves[1] = moves[0];
                                wert[1] = wert[0];
                                moves[0] = 6;
                                wert[0] = 11;
                            } else {
                                moves[1] = 6;
                                wert[1] = 11;
                            }
                        } else {
                            moves[2] = 6;
                            wert[2] = 11;
                        }
                    } else {
                        moves[3] = 6;
                        wert[3] = 11;
                    }
                } else {
                    moves[4] = 6;
                    wert[4] = 11;
                }
            } else {
                moves[5] = 6;
                wert[5] = 11;
            }
            break;
        case 3:
            if (!(x & 0x810200LL &&
                  x & 0x38LL &&
                  x & 0x104100LL))
                return true;
            if (wert[4] < 10) {
                moves[5] = moves[4];
                wert[5] = wert[4];
                if (wert[3] < 10) {
                    moves[4] = moves[3];
                    wert[4] = wert[3];
                    if (wert[2] < 10) {
                        moves[3] = moves[2];
                        wert[3] = wert[2];
                        if (wert[1] < 10) {
                            moves[2] = moves[1];
                            wert[2] = wert[1];
                            if (wert[0] < 10) {
                                moves[1] = moves[0];
                                wert[1] = wert[0];
                                moves[0] = 6;
                                wert[0] = 10;
                            } else {
                                moves[1] = 6;
                                wert[1] = 10;
                            }
                        } else {
                            moves[2] = 6;
                            wert[2] = 10;
                        }
                    } else {
                        moves[3] = 6;
                        wert[3] = 10;
                    }
                } else {
                    moves[4] = 6;
                    wert[4] = 10;
                }
            } else {
                moves[5] = 6;
                wert[5] = 10;
            }
            break;
        case 4:
            if (!(x & 0x408100LL &&
                  x & 0x1CLL &&
                  x & 0x82080LL))
                return true;
            if (wert[4] < 8) {
                moves[5] = moves[4];
                wert[5] = wert[4];
                if (wert[3] < 8) {
                    moves[4] = moves[3];
                    wert[4] = wert[3];
                    if (wert[2] < 8) {
                        moves[3] = moves[2];
                        wert[3] = wert[2];
                        if (wert[1] < 8) {
                            moves[2] = moves[1];
                            wert[2] = wert[1];
                            if (wert[0] < 8) {
                                moves[1] = moves[0];
                                wert[1] = wert[0];
                                moves[0] = 6;
                                wert[0] = 8;
                            } else {
                                moves[1] = 6;
                                wert[1] = 8;
                            }
                        } else {
                            moves[2] = 6;
                            wert[2] = 8;
                        }
                    } else {
                        moves[3] = 6;
                        wert[3] = 8;
                    }
                } else {
                    moves[4] = 6;
                    wert[4] = 8;
                }
            } else {
                moves[5] = 6;
                wert[5] = 8;
            }
            break;
        case 5:
            if (!(x & 0x204080LL &&
                  x & 0xELL &&
                  x & 0x41040LL))
                return true;
            if (wert[4] < 6) {
                moves[5] = moves[4];
                wert[5] = wert[4];
                if (wert[3] < 6) {
                    moves[4] = moves[3];
                    wert[4] = wert[3];
                    if (wert[2] < 6) {
                        moves[3] = moves[2];
                        wert[3] = wert[2];
                        if (wert[1] < 6) {
                            moves[2] = moves[1];
                            wert[2] = wert[1];
                            if (wert[0] < 6) {
                                moves[1] = moves[0];
                                wert[1] = wert[0];
                                moves[0] = 6;
                                wert[0] = 6;
                            } else {
                                moves[1] = 6;
                                wert[1] = 6;
                            }
                        } else {
                            moves[2] = 6;
                            wert[2] = 6;
                        }
                    } else {
                        moves[3] = 6;
                        wert[3] = 6;
                    }
                } else {
                    moves[4] = 6;
                    wert[4] = 6;
                }
            } else {
                moves[5] = 6;
                wert[5] = 6;
            }
            break;
        default:
            moves[5] = -1;
            wert[5] = 0;
            break;
    }
    switch (Hoehe[0]) {
        case 0:
            if (!(x & 0x408100000LL &&
                  x & 0x820800000LL))
                return true;
            if (wert[5] < 7) {
                moves[6] = moves[5];
                if (wert[4] < 7) {
                    moves[5] = moves[4];
                    wert[5] = wert[4];
                    if (wert[3] < 7) {
                        moves[4] = moves[3];
                        wert[4] = wert[3];
                        if (wert[2] < 7) {
                            moves[3] = moves[2];
                            wert[3] = wert[2];
                            if (wert[1] < 7) {
                                moves[2] = moves[1];
                                wert[2] = wert[1];
                                if (wert[0] < 7) {
                                    moves[1] = moves[0];
                                    wert[1] = wert[0];
                                    moves[0] = 0;
                                } else
                                    moves[1] = 0;
                            } else
                                moves[2] = 0;
                        } else
                            moves[3] = 0;
                    } else
                        moves[4] = 0;
                } else
                    moves[5] = 0;
            } else
                moves[6] = 0;
            break;
        case 1:
            if (!(x & 0x410400000LL &&
                  x & 0x204080000LL))
                return true;
            if (wert[5] < 9) {
                moves[6] = moves[5];
                if (wert[4] < 9) {
                    moves[5] = moves[4];
                    wert[5] = wert[4];
                    if (wert[3] < 9) {
                        moves[4] = moves[3];
                        wert[4] = wert[3];
                        if (wert[2] < 9) {
                            moves[3] = moves[2];
                            wert[3] = wert[2];
                            if (wert[1] < 9) {
                                moves[2] = moves[1];
                                wert[2] = wert[1];
                                if (wert[0] < 9) {
                                    moves[1] = moves[0];
                                    wert[1] = wert[0];
                                    moves[0] = 0;
                                } else
                                    moves[1] = 0;
                            } else
                                moves[2] = 0;
                        } else
                            moves[3] = 0;
                    } else
                        moves[4] = 0;
                } else
                    moves[5] = 0;
            } else
                moves[6] = 0;
            break;
        case 2:
            if (!(x & 0x208200000LL &&
                  x & 0x102040000LL))
                return true;
            if (wert[5] < 11) {
                moves[6] = moves[5];
                if (wert[4] < 11) {
                    moves[5] = moves[4];
                    wert[5] = wert[4];
                    if (wert[3] < 11) {
                        moves[4] = moves[3];
                        wert[4] = wert[3];
                        if (wert[2] < 11) {
                            moves[3] = moves[2];
                            wert[3] = wert[2];
                            if (wert[1] < 11) {
                                moves[2] = moves[1];
                                wert[2] = wert[1];
                                if (wert[0] < 11) {
                                    moves[1] = moves[0];
                                    wert[1] = wert[0];
                                    moves[0] = 0;
                                } else
                                    moves[1] = 0;
                            } else
                                moves[2] = 0;
                        } else
                            moves[3] = 0;
                    } else
                        moves[4] = 0;
                } else
                    moves[5] = 0;
            } else
                moves[6] = 0;
            break;
        case 3:
            if (!(x & 0x38000000000LL &&
                  x & 0x210800000LL &&
                  x & 0x104100000LL))
                return true;
            if (wert[5] < 10) {
                moves[6] = moves[5];
                if (wert[4] < 10) {
                    moves[5] = moves[4];
                    wert[5] = wert[4];
                    if (wert[3] < 10) {
                        moves[4] = moves[3];
                        wert[4] = wert[3];
                        if (wert[2] < 10) {
                            moves[3] = moves[2];
                            wert[3] = wert[2];
                            if (wert[1] < 10) {
                                moves[2] = moves[1];
                                wert[2] = wert[1];
                                if (wert[0] < 10) {
                                    moves[1] = moves[0];
                                    wert[1] = wert[0];
                                    moves[0] = 0;
                                } else
                                    moves[1] = 0;
                            } else
                                moves[2] = 0;
                        } else
                            moves[3] = 0;
                    } else
                        moves[4] = 0;
                } else
                    moves[5] = 0;
            } else
                moves[6] = 0;
            break;
        case 4:
            if (!(x & 0x108400000LL &&
                  x & 0x1C000000000LL &&
                  x & 0x82080000LL))
                return true;
            if (wert[5] < 8) {
                moves[6] = moves[5];
                if (wert[4] < 8) {
                    moves[5] = moves[4];
                    wert[5] = wert[4];
                    if (wert[3] < 8) {
                        moves[4] = moves[3];
                        wert[4] = wert[3];
                        if (wert[2] < 8) {
                            moves[3] = moves[2];
                            wert[3] = wert[2];
                            if (wert[1] < 8) {
                                moves[2] = moves[1];
                                wert[2] = wert[1];
                                if (wert[0] < 8) {
                                    moves[1] = moves[0];
                                    wert[1] = wert[0];
                                    moves[0] = 0;
                                } else
                                    moves[1] = 0;
                            } else
                                moves[2] = 0;
                        } else
                            moves[3] = 0;
                    } else
                        moves[4] = 0;
                } else
                    moves[5] = 0;
            } else
                moves[6] = 0;
            break;
        case 5:
            if (!(x & 0x84200000LL &&
                  x & 0xE000000000LL &&
                  x & 0x41040000LL))
                return true;
            if (wert[5] < 6) {
                moves[6] = moves[5];
                if (wert[4] < 6) {
                    moves[5] = moves[4];
                    wert[5] = wert[4];
                    if (wert[3] < 6) {
                        moves[4] = moves[3];
                        wert[4] = wert[3];
                        if (wert[2] < 6) {
                            moves[3] = moves[2];
                            wert[3] = wert[2];
                            if (wert[1] < 6) {
                                moves[2] = moves[1];
                                wert[2] = wert[1];
                                if (wert[0] < 6) {
                                    moves[1] = moves[0];
                                    wert[1] = wert[0];
                                    moves[0] = 0;
                                } else
                                    moves[1] = 0;
                            } else
                                moves[2] = 0;
                        } else
                            moves[3] = 0;
                    } else
                        moves[4] = 0;
                } else
                    moves[5] = 0;
            } else
                moves[6] = 0;
            break;
        default:
            moves[6] = -1;
            break;
    }
    moves[7] = -1;
    return false;
}

short int FindThreat1(void) {
    int y = Feld1;
    switch (Hoehe[0]) {
        case 0:
            if ((Feld1 & 0x808100000LL) == 0x8100000LL && Hoehe[1] < 1) return 0;
            if ((Feld1 & 0x410100000LL) == 0x400100000LL && Hoehe[2] < 2) return 0;
            if ((Feld1 & 0x408200000LL) == 0x408000000LL && Hoehe[3] < 3) return 0;
            break;
        case 1:
            if ((Feld1 & 0x404080000LL) == 0x4080000LL && Hoehe[1] < 2) return 0;
            if ((Feld1 & 0x208080000LL) == 0x200080000LL && Hoehe[2] < 3) return 0;
            if ((Feld1 & 0x204100000LL) == 0x204000000LL && Hoehe[3] < 4) return 0;
            if ((Feld1 & 0x810400000LL) == 0x10400000LL && Hoehe[1] < 1) return 0;
            if ((Feld1 & 0x420400000LL) == 0x400400000LL && Hoehe[2] < 1) return 0;
            if ((Feld1 & 0x410800000LL) == 0x410000000LL && Hoehe[3] < 1) return 0;
            break;
        case 2:
            if ((Feld1 & 0x202040000LL) == 0x2040000LL && Hoehe[1] < 3) return 0;
            if ((Feld1 & 0x104040000LL) == 0x100040000LL && Hoehe[2] < 4) return 0;
            if ((Feld1 & 0x102080000LL) == 0x102000000LL && Hoehe[3] < 5) return 0;
            if ((Feld1 & 0x408200000LL) == 0x8200000LL && Hoehe[1] < 2) return 0;
            if ((Feld1 & 0x210200000LL) == 0x200200000LL && Hoehe[2] < 2) return 0;
            if ((Feld1 & 0x208400000LL) == 0x208000000LL && Hoehe[3] < 2) return 0;
            break;
        case 3:
            if ((Feld1 & 0x204100000LL) == 0x4100000LL && Hoehe[1] < 3) return 0;
            if ((Feld1 & 0x108100000LL) == 0x100100000LL && Hoehe[2] < 3) return 0;
            if ((Feld1 & 0x104200000LL) == 0x104000000LL && Hoehe[3] < 3) return 0;
            if ((Feld1 & 0x410800000LL) == 0x10800000LL && Hoehe[1] < 2) return 0;
            if ((Feld1 & 0x220800000LL) == 0x200800000LL && Hoehe[2] < 1) return 0;
            break;
        case 4:
            if ((Feld1 & 0x102080000LL) == 0x2080000LL && Hoehe[1] < 4) return 0;
            if ((y & 0x84080000) == 0x80080000 && Hoehe[2] < 4) return 0;
            if ((y & 0x82100000) == 0x82000000 && Hoehe[3] < 4) return 0;
            if ((Feld1 & 0x208400000LL) == 0x8400000LL && Hoehe[1] < 3) return 0;
            if ((Feld1 & 0x110400000LL) == 0x100400000LL && Hoehe[2] < 2) return 0;
            if ((Feld1 & 0x108800000LL) == 0x108000000LL && Hoehe[3] < 1) return 0;
            break;
        case 5:
            if ((y & 0x81040000) == 0x1040000 && Hoehe[1] < 5) return 0;
            if ((y & 0x42040000) == 0x40040000 && Hoehe[2] < 5) return 0;
            if ((y & 0x41080000) == 0x41000000 && Hoehe[3] < 5) return 0;
            if ((Feld1 & 0x104200000LL) == 0x4200000LL && Hoehe[1] < 4) return 0;
            if ((y & 0x88200000) == 0x80200000 && Hoehe[2] < 3) return 0;
            if ((y & 0x84400000) == 0x84000000 && Hoehe[3] < 2) return 0;
            break;
        default:
            break;
    }
    switch (Hoehe[1]) {
        case 0:
            if ((y & 0x20204000) == 0x204000 && Hoehe[2] < 1) return 1;
            if ((y & 0x10404000) == 0x10004000 && Hoehe[3] < 2) return 1;
            if ((y & 0x10208000) == 0x10200000 && Hoehe[4] < 3) return 1;
            break;
        case 1:
            if ((y & 0x10102000) == 0x102000 && Hoehe[2] < 2) return 1;
            if ((y & 0x8202000) == 0x8002000 && Hoehe[3] < 3) return 1;
            if ((y & 0x8104000) == 0x8100000 && Hoehe[4] < 4) return 1;
            if ((Feld1 & 0x20010100000LL) == 0x20000100000LL && Hoehe[2] < 2) return 1;
            if ((Feld1 & 0x20008200000LL) == 0x20008000000LL && Hoehe[3] < 3) return 1;
            if ((Feld1 & 0x10400000LL) == 0x10400000LL && (Hoehe[0] < 1 || Hoehe[4] < 1)) return 1;
            if ((Feld1 & 0x10020400000LL) == 0x10000400000LL && Hoehe[2] < 1) return 1;
            if ((Feld1 & 0x10010800000LL) == 0x10010000000LL && Hoehe[3] < 1) return 1;
            if ((y & 0x20410000) == 0x410000 && Hoehe[2] < 1) return 1;
            if ((y & 0x10810000) == 0x10010000 && Hoehe[3] < 1) return 1;
            break;
        case 2:
            if ((y & 0x8081000) == 0x81000 && Hoehe[2] < 3) return 1;
            if ((y & 0x4101000) == 0x4001000 && Hoehe[3] < 4) return 1;
            if ((Feld1 & 0x4080000LL) == 0x4080000LL && (Hoehe[0] < 1 || Hoehe[4] < 5)) return 1;
            if ((Feld1 & 0x10008080000LL) == 0x10000080000LL && Hoehe[2] < 3) return 1;
            if ((Feld1 & 0x10004100000LL) == 0x10004000000LL && Hoehe[3] < 4) return 1;
            if ((Feld1 & 0x8200000LL) == 0x8200000LL && (Hoehe[0] < 2 || Hoehe[4] < 2)) return 1;
            if ((Feld1 & 0x8010200000LL) == 0x8000200000LL && Hoehe[2] < 2) return 1;
            if ((Feld1 & 0x8008400000LL) == 0x8008000000LL && Hoehe[3] < 2) return 1;
            if ((y & 0x10208000) == 0x208000 && Hoehe[2] < 2) return 1;
            if ((y & 0x8408000) == 0x8008000 && Hoehe[3] < 2) return 1;
            if ((Feld1 & 0x8010800000LL) == 0x10800000LL && Hoehe[0] < 3) return 1;
            if ((Feld1 & 0x4020800000LL) == 0x4000800000LL && Hoehe[2] < 1) return 1;
            break;
        case 3:
            if ((Feld1 & 0x10002040000LL) == 0x2040000LL && Hoehe[0] < 2) return 1;
            if ((Feld1 & 0x8004040000LL) == 0x8000040000LL && Hoehe[2] < 4) return 1;
            if ((Feld1 & 0x8002080000LL) == 0x8002000000LL && Hoehe[3] < 5) return 1;
            if ((Feld1 & 0x4100000LL) == 0x4100000LL && (Hoehe[0] < 3 || Hoehe[4] < 3)) return 1;
            if ((Feld1 & 0x4008100000LL) == 0x4000100000LL && Hoehe[2] < 3) return 1;
            if ((Feld1 & 0x4004200000LL) == 0x4004000000LL && Hoehe[3] < 3) return 1;
            if ((y & 0x8104000) == 0x104000 && Hoehe[2] < 3) return 1;
            if ((y & 0x4204000) == 0x4004000 && Hoehe[3] < 3) return 1;
            if ((Feld1 & 0x4008400000LL) == 0x8400000LL && Hoehe[0] < 4) return 1;
            if ((Feld1 & 0x2010400000LL) == 0x2000400000LL && Hoehe[2] < 2) return 1;
            if ((Feld1 & 0x2008800000LL) == 0x2008000000LL && Hoehe[3] < 1) return 1;
            if ((y & 0x10420000) == 0x420000 && Hoehe[2] < 2) return 1;
            if ((y & 0x8820000) == 0x8020000 && Hoehe[3] < 1) return 1;
            break;
        case 4:
            if ((Feld1 & 0x2080000LL) == 0x2080000LL && (Hoehe[0] < 4 || Hoehe[4] < 4)) return 1;
            if ((Feld1 & 0x2004080000LL) == 0x2000080000LL && Hoehe[2] < 4) return 1;
            if ((Feld1 & 0x2002100000LL) == 0x2002000000LL && Hoehe[3] < 4) return 1;
            if ((y & 0x4082000) == 0x82000 && Hoehe[2] < 4) return 1;
            if ((y & 0x2102000) == 0x2002000 && Hoehe[3] < 4) return 1;
            if ((Feld1 & 0x4200000LL) == 0x4200000LL && (Hoehe[0] < 5 || Hoehe[4] < 1)) return 1;
            if ((Feld1 & 0x1008200000LL) == 0x1000200000LL && Hoehe[2] < 3) return 1;
            if ((Feld1 & 0x1004400000LL) == 0x1004000000LL && Hoehe[3] < 2) return 1;
            if ((y & 0x8210000) == 0x210000 && Hoehe[2] < 3) return 1;
            if ((y & 0x4410000) == 0x4010000 && Hoehe[3] < 2) return 1;
            break;
        case 5:
            if ((Feld1 & 0x1040000LL) == 0x1040000LL && (Hoehe[0] < 5 || Hoehe[4] < 5)) return 1;
            if ((Feld1 & 0x1002040000LL) == 0x1000040000LL && Hoehe[2] < 5) return 1;
            if ((Feld1 & 0x1001080000LL) == 0x1001000000LL && Hoehe[3] < 5) return 1;
            if ((y & 0x2041000) == 0x41000 && Hoehe[2] < 5) return 1;
            if ((y & 0x1081000) == 0x1001000 && Hoehe[3] < 5) return 1;
            if ((y & 0x4108000) == 0x108000 && Hoehe[2] < 4) return 1;
            if ((y & 0x2208000) == 0x2008000 && Hoehe[3] < 3) return 1;
            if ((y & 0x2110000) == 0x2100000 && Hoehe[4] < 2) return 1;
            break;
        default:
            break;
    }
    switch (Hoehe[2]) {
        case 0:
            if ((y & 0x808100) == 0x8100 && Hoehe[3] < 1) return 2;
            if ((y & 0x410100) == 0x400100 && Hoehe[4] < 2) return 2;
            if ((y & 0x408200) == 0x408000 && Hoehe[5] < 3) return 2;
            break;
        case 1:
            if ((y & 0x404080) == 0x4080 && Hoehe[3] < 2) return 2;
            if ((y & 0x208080) == 0x200080 && Hoehe[4] < 3) return 2;
            if ((y & 0x204100) == 0x204000 && Hoehe[5] < 4) return 2;
            if ((Feld1 & 0x8200800000LL) == 0x200800000LL && Hoehe[0] < 3) return 2;
            if ((Feld1 & 0x4400800000LL) == 0x4000800000LL && Hoehe[1] < 2) return 2;
            if ((Feld1 & 0x800404000LL) == 0x800004000LL && Hoehe[3] < 2) return 2;
            if ((Feld1 & 0x800208000LL) == 0x800200000LL && Hoehe[4] < 3) return 2;
            if ((Feld1 & 0x400400000LL) == 0x400400000LL && (Hoehe[0] < 1 || Hoehe[4] < 1)) return 2;
            if ((Feld1 & 0x10800400000LL) == 0x10000400000LL && Hoehe[1] < 1) return 2;
            if ((Feld1 & 0x10400800000LL) == 0x10400000000LL && Hoehe[3] < 1) return 2;
            if ((Feld1 & 0x410000LL) == 0x410000LL && (Hoehe[1] < 1 || Hoehe[5] < 1)) return 2;
            if ((Feld1 & 0x400810000LL) == 0x400010000LL && Hoehe[3] < 1) return 2;
            if ((y & 0x810400) == 0x10400 && Hoehe[3] < 1) return 2;
            if ((y & 0x420400) == 0x400400 && Hoehe[4] < 1) return 2;
            break;
        case 2:
            if ((y & 0x202040) == 0x2040 && Hoehe[3] < 3) return 2;
            if ((y & 0x104040) == 0x100040 && Hoehe[4] < 4) return 2;
            if ((Feld1 & 0x102000LL) == 0x102000LL && (Hoehe[1] < 1 || Hoehe[5] < 5)) return 2;
            if ((Feld1 & 0x4100400000LL) == 0x100400000LL && Hoehe[0] < 4) return 2;
            if ((Feld1 & 0x2200400000LL) == 0x2000400000LL && Hoehe[1] < 3) return 2;
            if ((Feld1 & 0x2100800000LL) == 0x2100000000LL && Hoehe[3] < 1) return 2;
            if ((Feld1 & 0x400202000LL) == 0x400002000LL && Hoehe[3] < 3) return 2;
            if ((Feld1 & 0x400104000LL) == 0x400100000LL && Hoehe[4] < 4) return 2;
            if ((Feld1 & 0x200200000LL) == 0x200200000LL && (Hoehe[0] < 2 || Hoehe[4] < 2)) return 2;
            if ((Feld1 & 0x8400200000LL) == 0x8000200000LL && Hoehe[1] < 2) return 2;
            if ((Feld1 & 0x8200400000LL) == 0x8200000000LL && Hoehe[3] < 2) return 2;
            if ((Feld1 & 0x208000LL) == 0x208000LL && (Hoehe[1] < 2 || Hoehe[5] < 2)) return 2;
            if ((Feld1 & 0x200408000LL) == 0x200008000LL && Hoehe[3] < 2) return 2;
            if ((y & 0x408200) == 0x8200 && Hoehe[3] < 2) return 2;
            if ((y & 0x210200) == 0x200200 && Hoehe[4] < 2) return 2;
            if ((Feld1 & 0x200420000LL) == 0x420000LL && Hoehe[1] < 3) return 2;
            if ((Feld1 & 0x100820000LL) == 0x100020000LL && Hoehe[3] < 1) return 2;
            if ((Feld1 & 0x20800100000LL) == 0x20000100000LL && Hoehe[1] < 1) return 2;
            if ((Feld1 & 0x20400200000LL) == 0x20400000000LL && Hoehe[3] < 3) return 2;
            break;
        case 3:
            if ((Feld1 & 0x80200000LL) == 0x80200000LL && (Hoehe[0] < 5 || Hoehe[4] < 1)) return 2;
            if ((Feld1 & 0x1100200000LL) == 0x1000200000LL && Hoehe[1] < 4) return 2;
            if ((Feld1 & 0x1080400000LL) == 0x1080000000LL && Hoehe[3] < 2) return 2;
            if ((Feld1 & 0x400081000LL) == 0x81000LL && Hoehe[1] < 2) return 2;
            if ((Feld1 & 0x200101000LL) == 0x200001000LL && Hoehe[3] < 4) return 2;
            if ((Feld1 & 0x200080000LL) == 0x200080000LL && (Hoehe[0] < 1 || Hoehe[4] < 5)) return 2;
            if ((Feld1 & 0x100100000LL) == 0x100100000LL && (Hoehe[0] < 3 || Hoehe[4] < 3)) return 2;
            if ((Feld1 & 0x4200100000LL) == 0x4000100000LL && Hoehe[1] < 3) return 2;
            if ((Feld1 & 0x4100200000LL) == 0x4100000000LL && Hoehe[3] < 3) return 2;
            if ((Feld1 & 0x104000LL) == 0x104000LL && (Hoehe[1] < 3 || Hoehe[5] < 3)) return 2;
            if ((Feld1 & 0x100204000LL) == 0x100004000LL && Hoehe[3] < 3) return 2;
            if ((y & 0x204100) == 0x4100 && Hoehe[3] < 3) return 2;
            if ((y & 0x108100) == 0x100100 && Hoehe[4] < 3) return 2;
            if ((Feld1 & 0x100210000LL) == 0x210000LL && Hoehe[1] < 4) return 2;
            if ((y & 0x80410000) == 0x80010000 && Hoehe[3] < 2) return 2;
            if ((Feld1 & 0x10400080000LL) == 0x10000080000LL && Hoehe[1] < 2) return 2;
            if ((Feld1 & 0x10200100000LL) == 0x10200000000LL && Hoehe[3] < 4) return 2;
            if ((y & 0x410800) == 0x10800 && Hoehe[3] < 2) return 2;
            if ((y & 0x220800) == 0x200800 && Hoehe[4] < 1) return 2;
            break;
        case 4:
            if ((Feld1 & 0x80080000LL) == 0x80080000LL && (Hoehe[0] < 4 || Hoehe[4] < 4)) return 2;
            if ((Feld1 & 0x2100080000LL) == 0x2000080000LL && Hoehe[1] < 4) return 2;
            if ((Feld1 & 0x2080100000LL) == 0x2080000000LL && Hoehe[3] < 4) return 2;
            if ((Feld1 & 0x82000LL) == 0x82000LL && (Hoehe[1] < 4 || Hoehe[5] < 4)) return 2;
            if ((y & 0x80102000) == 0x80002000 && Hoehe[3] < 4) return 2;
            if ((y & 0x102080) == 0x2080 && Hoehe[3] < 4) return 2;
            if ((y & 0x84080) == 0x80080 && Hoehe[4] < 4) return 2;
            if ((y & 0x108000) == 0x108000 && (Hoehe[1] < 5 || Hoehe[5] < 1)) return 2;
            if ((y & 0x40208000) == 0x40008000 && Hoehe[3] < 3) return 2;
            if ((y & 0x40110000) == 0x40100000 && Hoehe[4] < 2) return 2;
            if ((Feld1 & 0x10100040000LL) == 0x100040000LL && Hoehe[0] < 2) return 2;
            if ((Feld1 & 0x8200040000LL) == 0x8000040000LL && Hoehe[1] < 3) return 2;
            if ((Feld1 & 0x8100080000LL) == 0x8100000000LL && Hoehe[3] < 5) return 2;
            if ((y & 0x208400) == 0x8400 && Hoehe[3] < 3) return 2;
            if ((y & 0x110400) == 0x100400 && Hoehe[4] < 2) return 2;
            break;
        case 5:
            if ((Feld1 & 0x40040000LL) == 0x40040000LL && (Hoehe[0] < 5 || Hoehe[4] < 5)) return 2;
            if ((Feld1 & 0x1080040000LL) == 0x1000040000LL && Hoehe[1] < 5) return 2;
            if ((Feld1 & 0x1040080000LL) == 0x1040000000LL && Hoehe[3] < 5) return 2;
            if ((y & 0x80041080) == 0x41000 && (Hoehe[1] < 5 || Hoehe[5] < 5)) return 2;
            if ((y & 0x40081000) == 0x40001000 && Hoehe[3] < 5) return 2;
            if ((y & 0x81040) == 0x1040 && Hoehe[3] < 5) return 2;
            if ((y & 0x42040) == 0x40040 && Hoehe[4] < 5) return 2;
            if ((y & 0x104200) == 0x4200 && Hoehe[3] < 4) return 2;
            if ((y & 0x88200) == 0x80200 && Hoehe[4] < 3) return 2;
            if ((y & 0x84400) == 0x84000 && Hoehe[5] < 2) return 2;
            break;
        default:
            break;
    }
    switch (Hoehe[3]) {
        case 0:
            if ((Feld1 & 0x8210000000LL) == 0x210000000LL && Hoehe[0] < 3) return 3;
            if ((Feld1 & 0x4410000000LL) == 0x4010000000LL && Hoehe[1] < 2) return 3;
            if ((Feld1 & 0x4220000000LL) == 0x4200000000LL && Hoehe[2] < 1) return 3;
            if ((y & 0x20204) == 0x204 && Hoehe[4] < 1) return 3;
            if ((y & 0x10404) == 0x10004 && Hoehe[5] < 2) return 3;
            if ((y & 0x10208) == 0x10200 && Hoehe[6] < 3) return 3;
            break;
        case 1:
            if ((Feld1 & 0x4108000000LL) == 0x108000000LL && Hoehe[0] < 4) return 3;
            if ((Feld1 & 0x2208000000LL) == 0x2008000000LL && Hoehe[1] < 3) return 3;
            if ((Feld1 & 0x2110000000LL) == 0x2100000000LL && Hoehe[2] < 2) return 3;
            if ((y & 0x10102) == 0x102 && Hoehe[4] < 2) return 3;
            if ((y & 0x8202) == 0x8002 && Hoehe[5] < 3) return 3;
            if ((y & 0x8104) == 0x8100 && Hoehe[6] < 4) return 3;
            if ((Feld1 & 0x208020000LL) == 0x8020000LL && Hoehe[1] < 3) return 3;
            if ((Feld1 & 0x110020000LL) == 0x100020000LL && Hoehe[2] < 2) return 3;
            if ((y & 0x20010100) == 0x20000100 && Hoehe[4] < 2) return 3;
            if ((y & 0x20008200) == 0x20008000 && Hoehe[5] < 3) return 3;
            if ((Feld1 & 0x410000000LL) == 0x410000000LL && (Hoehe[0] < 1 || Hoehe[4] < 1)) return 3;
            if ((Feld1 & 0x10810000000LL) == 0x10010000000LL && Hoehe[1] < 1) return 3;
            if ((Feld1 & 0x10420000000LL) == 0x10400000000LL && Hoehe[2] < 1) return 3;
            if ((Feld1 & 0x10010000LL) == 0x10010000LL && (Hoehe[1] < 1 || Hoehe[5] < 1)) return 3;
            if ((Feld1 & 0x420010000LL) == 0x400010000LL && Hoehe[2] < 1) return 3;
            if ((y & 0x10400) == 0x10400 && (Hoehe[2] < 1 || Hoehe[6] < 1)) return 3;
            if ((y & 0x10020400) == 0x10000400 && Hoehe[4] < 1) return 3;
            if ((y & 0x20410) == 0x410 && Hoehe[4] < 1) return 3;
            if ((y & 0x10810) == 0x10010 && Hoehe[5] < 1) return 3;
            break;
        case 2:
            if ((Feld1 & 0x84000000LL) == 0x84000000LL && (Hoehe[0] < 5 || Hoehe[4] < 1)) return 3;
            if ((Feld1 & 0x1104000000LL) == 0x1004000000LL && Hoehe[1] < 4) return 3;
            if ((Feld1 & 0x1088000000LL) == 0x1080000000LL && Hoehe[2] < 3) return 3;
            if ((y & 0x8081) == 0x81 && Hoehe[4] < 3) return 3;
            if ((y & 0x4101) == 0x4001 && Hoehe[5] < 4) return 3;
            if ((y & 0x4080) == 0x4080 && (Hoehe[2] < 1 || Hoehe[6] < 5)) return 3;
            if ((Feld1 & 0x104010000LL) == 0x4010000LL && Hoehe[1] < 4) return 3;
            if ((y & 0x88010000) == 0x80010000 && Hoehe[2] < 3) return 3;
            if ((y & 0x10008080) == 0x10000080 && Hoehe[4] < 3) return 3;
            if ((y & 0x10004100) == 0x10004000 && Hoehe[5] < 4) return 3;
            if ((Feld1 & 0x208000000LL) == 0x208000000LL && (Hoehe[0] < 2 || Hoehe[4] < 2)) return 3;
            if ((Feld1 & 0x8408000000LL) == 0x8008000000LL && Hoehe[1] < 2) return 3;
            if ((Feld1 & 0x8210000000LL) == 0x8200000000LL && Hoehe[2] < 2) return 3;
            if ((Feld1 & 0x8008000LL) == 0x8008000LL && (Hoehe[1] < 2 || Hoehe[5] < 2)) return 3;
            if ((Feld1 & 0x210008000LL) == 0x200008000LL && Hoehe[2] < 2) return 3;
            if ((y & 0x8200) == 0x8200 && (Hoehe[2] < 2 || Hoehe[6] < 2)) return 3;
            if ((y & 0x8010200) == 0x8000200 && Hoehe[4] < 2) return 3;
            if ((y & 0x10208) == 0x208 && Hoehe[4] < 2) return 3;
            if ((y & 0x8408) == 0x8008 && Hoehe[5] < 2) return 3;
            if ((y & 0x8010800) == 0x10800 && Hoehe[2] < 3) return 3;
            if ((y & 0x4020800) == 0x4000800 && Hoehe[4] < 1) return 3;
            if ((Feld1 & 0x820004000LL) == 0x800004000LL && Hoehe[2] < 1) return 3;
            if ((Feld1 & 0x810008000LL) == 0x810000000LL && Hoehe[4] < 3) return 3;
            break;
        case 3:
            if ((y & 0x2008000) == 0x2008000 && (Hoehe[1] < 5 || Hoehe[5] < 1)) return 3;
            if ((y & 0x44008000) == 0x40008000 && Hoehe[2] < 4) return 3;
            if ((y & 0x42010000) == 0x42000000 && Hoehe[4] < 2) return 3;
            if ((y & 0x10002040) == 0x2040 && Hoehe[2] < 2) return 3;
            if ((y & 0x8004040) == 0x8000040 && Hoehe[4] < 4) return 3;
            if ((Feld1 & 0x8002000LL) == 0x8002000LL && (Hoehe[1] < 1 || Hoehe[5] < 5)) return 3;
            if ((Feld1 & 0x104000000LL) == 0x104000000LL && (Hoehe[0] < 3 || Hoehe[4] < 3)) return 3;
            if ((Feld1 & 0x4204000000LL) == 0x4004000000LL && Hoehe[1] < 3) return 3;
            if ((Feld1 & 0x4108000000LL) == 0x4100000000LL && Hoehe[2] < 3) return 3;
            if ((Feld1 & 0x4004000LL) == 0x4004000LL && (Hoehe[1] < 3 || Hoehe[5] < 3)) return 3;
            if ((Feld1 & 0x108004000LL) == 0x100004000LL && Hoehe[2] < 3) return 3;
            if ((y & 0x4100) == 0x4100 && (Hoehe[2] < 3 || Hoehe[6] < 3)) return 3;
            if ((y & 0x4008100) == 0x4000100 && Hoehe[4] < 3) return 3;
            if ((y & 0x8104) == 0x104 && Hoehe[4] < 3) return 3;
            if ((y & 0x4204) == 0x4004 && Hoehe[5] < 3) return 3;
            if ((y & 0x4008400) == 0x8400 && Hoehe[2] < 4) return 3;
            if ((y & 0x2010400) == 0x2000400 && Hoehe[4] < 2) return 3;
            if ((Feld1 & 0x410002000LL) == 0x400002000LL && Hoehe[2] < 2) return 3;
            if ((Feld1 & 0x408004000LL) == 0x408000000LL && Hoehe[4] < 4) return 3;
            if ((y & 0x10420) == 0x420 && Hoehe[4] < 2) return 3;
            if ((y & 0x8820) == 0x8020 && Hoehe[5] < 1) return 3;
            if ((Feld1 & 0x20808000000LL) == 0x20008000000LL && Hoehe[1] < 1) return 3;
            if ((Feld1 & 0x20410000000LL) == 0x20400000000LL && Hoehe[2] < 2) return 3;
            break;
        case 4:
            if ((Feld1 & 0x82000000LL) == 0x82000000LL && (Hoehe[0] < 4 || Hoehe[4] < 4)) return 3;
            if ((Feld1 & 0x2102000000LL) == 0x2002000000LL && Hoehe[1] < 4) return 3;
            if ((Feld1 & 0x2084000000LL) == 0x2080000000LL && Hoehe[2] < 4) return 3;
            if ((Feld1 & 0x2002000LL) == 0x2002000LL && (Hoehe[1] < 4 || Hoehe[5] < 4)) return 3;
            if ((y & 0x84002000) == 0x80002000 && Hoehe[2] < 4) return 3;
            if ((y & 0x2080) == 0x2080 && (Hoehe[2] < 4 || Hoehe[6] < 4)) return 3;
            if ((y & 0x2004080) == 0x2000080 && Hoehe[4] < 4) return 3;
            if ((y & 0x4082) == 0x82 && Hoehe[4] < 4) return 3;
            if ((y & 0x2102) == 0x2002 && Hoehe[5] < 4) return 3;
            if ((y & 0x4200) == 0x4200 && (Hoehe[2] < 5 || Hoehe[6] < 1)) return 3;
            if ((y & 0x1008200) == 0x1000200 && Hoehe[4] < 3) return 3;
            if ((y & 0x1004400) == 0x1004000 && Hoehe[5] < 2) return 3;
            if ((Feld1 & 0x404001000LL) == 0x4001000LL && Hoehe[1] < 2) return 3;
            if ((Feld1 & 0x208001000LL) == 0x200001000LL && Hoehe[2] < 3) return 3;
            if ((Feld1 & 0x204000000LL) == 0x204000000LL && (Hoehe[0] < 1 || Hoehe[4] < 5)) return 3;
            if ((y & 0x8210) == 0x210 && Hoehe[4] < 3) return 3;
            if ((y & 0x4410) == 0x4010 && Hoehe[5] < 2) return 3;
            if ((Feld1 & 0x10404000000LL) == 0x10004000000LL && Hoehe[1] < 2) return 3;
            if ((Feld1 & 0x10208000000LL) == 0x10200000000LL && Hoehe[2] < 3) return 3;
            break;
        case 5:
            if ((Feld1 & 0x41000000LL) == 0x41000000LL && (Hoehe[0] < 5 || Hoehe[4] < 5)) return 3;
            if ((Feld1 & 0x1081000000LL) == 0x1001000000LL && Hoehe[1] < 5) return 3;
            if ((Feld1 & 0x1042000000LL) == 0x1040000000LL && Hoehe[2] < 5) return 3;
            if ((y & 0x1001000) == 0x1001000 && (Hoehe[1] < 5 || Hoehe[5] < 5)) return 3;
            if ((y & 0x42001000) == 0x40001000 && Hoehe[2] < 5) return 3;
            if ((y & 0x1040) == 0x1040 && (Hoehe[2] < 5 || Hoehe[6] < 5)) return 3;
            if ((y & 0x1002040) == 0x1000040 && Hoehe[4] < 5) return 3;
            if ((y & 0x2041) == 0x41 && Hoehe[4] < 5) return 3;
            if ((y & 0x1081) == 0x1001 && Hoehe[5] < 5) return 3;
            if ((y & 0x4108) == 0x108 && Hoehe[4] < 4) return 3;
            if ((y & 0x2208) == 0x2008 && Hoehe[5] < 3) return 3;
            if ((y & 0x2110) == 0x2100 && Hoehe[6] < 2) return 3;
            if ((Feld1 & 0x10102000000LL) == 0x102000000LL && Hoehe[0] < 2) return 3;
            if ((Feld1 & 0x8202000000LL) == 0x8002000000LL && Hoehe[1] < 3) return 3;
            if ((Feld1 & 0x8104000000LL) == 0x8100000000LL && Hoehe[2] < 4) return 3;
            break;
        default:
            break;
    }
    switch (Hoehe[4]) {
        case 0:
            if ((Feld1 & 0x208400000LL) == 0x8400000LL && Hoehe[1] < 3) return 4;
            if ((Feld1 & 0x110400000LL) == 0x100400000LL && Hoehe[2] < 2) return 4;
            if ((Feld1 & 0x108800000LL) == 0x108000000LL && Hoehe[3] < 1) return 4;
            break;
        case 1:
            if ((Feld1 & 0x104200000LL) == 0x4200000LL && Hoehe[1] < 4) return 4;
            if ((y & 0x88200000) == 0x80200000 && Hoehe[2] < 3) return 4;
            if ((y & 0x84400000) == 0x84000000 && Hoehe[3] < 2) return 4;
            if ((y & 0x8200800) == 0x200800 && Hoehe[2] < 3) return 4;
            if ((y & 0x4400800) == 0x4000800 && Hoehe[3] < 2) return 4;
            if ((y & 0x800404) == 0x800004 && Hoehe[5] < 2) return 4;
            if ((y & 0x800208) == 0x800200 && Hoehe[6] < 3) return 4;
            if ((Feld1 & 0x10400000LL) == 0x10400000LL && (Hoehe[1] < 1 || Hoehe[5] < 1)) return 4;
            if ((Feld1 & 0x420400000LL) == 0x400400000LL && Hoehe[2] < 1) return 4;
            if ((Feld1 & 0x410800000LL) == 0x410000000LL && Hoehe[3] < 1) return 4;
            if ((y & 0x400400) == 0x400400 && (Hoehe[2] < 1 || Hoehe[6] < 1)) return 4;
            if ((y & 0x10800400) == 0x10000400 && Hoehe[3] < 1) return 4;
            if ((y & 0x800410) == 0x410 && Hoehe[3] < 1) return 4;
            if ((y & 0x400810) == 0x400010 && Hoehe[5] < 1) return 4;
            break;
        case 2:
            if ((y & 0x2100000) == 0x2100000 && (Hoehe[1] < 5 || Hoehe[5] < 1)) return 4;
            if ((y & 0x44100000) == 0x40100000 && Hoehe[2] < 4) return 4;
            if ((y & 0x42200000) == 0x42000000 && Hoehe[3] < 3) return 4;
            if ((y & 0x4100400) == 0x100400 && Hoehe[2] < 4) return 4;
            if ((y & 0x2200400) == 0x2000400 && Hoehe[3] < 3) return 4;
            if ((y & 0x800102) == 0x102 && Hoehe[3] < 1) return 4;
            if ((y & 0x400202) == 0x400002 && Hoehe[5] < 3) return 4;
            if ((y & 0x400104) == 0x400100 && Hoehe[6] < 4) return 4;
            if ((Feld1 & 0x8200000LL) == 0x8200000LL && (Hoehe[1] < 2 || Hoehe[5] < 2)) return 4;
            if ((Feld1 & 0x210200000LL) == 0x200200000LL && Hoehe[2] < 2) return 4;
            if ((Feld1 & 0x208400000LL) == 0x208000000LL && Hoehe[3] < 2) return 4;
            if ((y & 0x200200) == 0x200200 && (Hoehe[2] < 2 || Hoehe[6] < 2)) return 4;
            if ((y & 0x8400200) == 0x8000200 && Hoehe[3] < 2) return 4;
            if ((y & 0x400208) == 0x208 && Hoehe[3] < 2) return 4;
            if ((y & 0x200408) == 0x200008 && Hoehe[5] < 2) return 4;
            if ((y & 0x200420) == 0x420 && Hoehe[3] < 3) return 4;
            if ((y & 0x100820) == 0x100020 && Hoehe[5] < 1) return 4;
            if ((y & 0x20800100) == 0x20000100 && Hoehe[3] < 1) return 4;
            if ((y & 0x20400200) == 0x20400000 && Hoehe[5] < 3) return 4;
            break;
        case 3:
            if ((y & 0x80200) == 0x80200 && (Hoehe[2] < 5 || Hoehe[6] < 1)) return 4;
            if ((y & 0x1100200) == 0x1000200 && Hoehe[3] < 4) return 4;
            if ((y & 0x1080400) == 0x1080000 && Hoehe[5] < 2) return 4;
            if ((y & 0x400081) == 0x81 && Hoehe[3] < 2) return 4;
            if ((y & 0x200101) == 0x200001 && Hoehe[5] < 4) return 4;
            if ((y & 0x200080) == 0x200080 && (Hoehe[2] < 1 || Hoehe[6] < 5)) return 4;
            if ((Feld1 & 0x204100200LL) == 0x4100000LL && (Hoehe[1] < 3 || Hoehe[5] < 3)) return 4;
            if ((Feld1 & 0x108100000LL) == 0x100100000LL && Hoehe[2] < 3) return 4;
            if ((Feld1 & 0x104200000LL) == 0x104000000LL && Hoehe[3] < 3) return 4;
            if ((y & 0x100100) == 0x100100 && (Hoehe[2] < 3 || Hoehe[6] < 3)) return 4;
            if ((y & 0x4200100) == 0x4000100 && Hoehe[3] < 3) return 4;
            if ((y & 0x200104) == 0x104 && Hoehe[3] < 3) return 4;
            if ((y & 0x100204) == 0x100004 && Hoehe[5] < 3) return 4;
            if ((y & 0x100210) == 0x210 && Hoehe[3] < 4) return 4;
            if ((y & 0x80410) == 0x80010 && Hoehe[5] < 2) return 4;
            if ((y & 0x10400080) == 0x10000080 && Hoehe[3] < 2) return 4;
            if ((y & 0x10200100) == 0x10200000 && Hoehe[5] < 4) return 4;
            if ((Feld1 & 0x820200000LL) == 0x800200000LL && Hoehe[2] < 1) return 4;
            if ((Feld1 & 0x810400000LL) == 0x810000000LL && Hoehe[3] < 2) return 4;
            break;
        case 4:
            if ((Feld1 & 0x2080000LL) == 0x2080000LL && (Hoehe[1] < 4 || Hoehe[5] < 4)) return 4;
            if ((y & 0x84080000) == 0x80080000 && Hoehe[2] < 4) return 4;
            if ((y & 0x82100000) == 0x82000000 && Hoehe[3] < 4) return 4;
            if ((y & 0x80080) == 0x80080 && (Hoehe[2] < 4 || Hoehe[6] < 4)) return 4;
            if ((y & 0x2100080) == 0x2000080 && Hoehe[3] < 4) return 4;
            if ((y & 0x100082) == 0x82 && Hoehe[3] < 4) return 4;
            if ((y & 0x80102) == 0x80002 && Hoehe[5] < 4) return 4;
            if ((y & 0x80108) == 0x108 && Hoehe[3] < 5) return 4;
            if ((y & 0x40208) == 0x40008 && Hoehe[5] < 3) return 4;
            if ((y & 0x40110) == 0x40100 && Hoehe[6] < 2) return 4;
            if ((y & 0x10100040) == 0x100040 && Hoehe[2] < 2) return 4;
            if ((y & 0x8200040) == 0x8000040 && Hoehe[3] < 3) return 4;
            if ((Feld1 & 0x8100000LL) == 0x8100000LL && (Hoehe[1] < 1 || Hoehe[5] < 5)) return 4;
            if ((Feld1 & 0x410100000LL) == 0x400100000LL && Hoehe[2] < 2) return 4;
            if ((Feld1 & 0x408200000LL) == 0x408000000LL && Hoehe[3] < 3) return 4;
            break;
        case 5:
            if ((y & 0x1040000) == 0x1040000 && (Hoehe[1] < 5 || Hoehe[5] < 5)) return 4;
            if ((y & 0x42040000) == 0x40040000 && Hoehe[2] < 5) return 4;
            if ((y & 0x41080000) == 0x41000000 && Hoehe[3] < 5) return 4;
            if ((y & 0x40040) == 0x40040 && (Hoehe[2] < 5 || Hoehe[6] < 5)) return 4;
            if ((y & 0x1080040) == 0x1000040 && Hoehe[3] < 5) return 4;
            if ((y & 0x80041) == 0x41 && Hoehe[3] < 5) return 4;
            if ((y & 0x40081) == 0x40001 && Hoehe[5] < 5) return 4;
            if ((Feld1 & 0x404080000LL) == 0x4080000LL && Hoehe[1] < 2) return 4;
            if ((Feld1 & 0x208080000LL) == 0x200080000LL && Hoehe[2] < 3) return 4;
            if ((Feld1 & 0x204100000LL) == 0x204000000LL && Hoehe[3] < 4) return 4;
            break;
        default:
            break;
    }
    switch (Hoehe[5]) {
        case 0:
            if ((y & 0x8210000) == 0x210000 && Hoehe[2] < 3) return 5;
            if ((y & 0x4410000) == 0x4010000 && Hoehe[3] < 2) return 5;
            if ((y & 0x4220000) == 0x4200000 && Hoehe[4] < 1) return 5;
            break;
        case 1:
            if ((y & 0x4108000) == 0x108000 && Hoehe[2] < 4) return 5;
            if ((y & 0x2208000) == 0x2008000 && Hoehe[3] < 3) return 5;
            if ((y & 0x2110000) == 0x2100000 && Hoehe[4] < 2) return 5;
            if ((y & 0x208020) == 0x8020 && Hoehe[3] < 3) return 5;
            if ((y & 0x110020) == 0x100020 && Hoehe[4] < 2) return 5;
            if ((y & 0x410000) == 0x410000 && (Hoehe[2] < 1 || Hoehe[6] < 1)) return 5;
            if ((y & 0x10810000) == 0x10010000 && Hoehe[3] < 1) return 5;
            if ((y & 0x10420000) == 0x10400000 && Hoehe[4] < 1) return 5;
            if ((y & 0x810010) == 0x10010 && Hoehe[3] < 1) return 5;
            if ((y & 0x420010) == 0x400010 && Hoehe[4] < 1) return 5;
            break;
        case 2:
            if ((y & 0x84000) == 0x84000 && (Hoehe[2] < 5 || Hoehe[6] < 1)) return 5;
            if ((y & 0x1104000) == 0x1004000 && Hoehe[3] < 4) return 5;
            if ((y & 0x1088000) == 0x1080000 && Hoehe[4] < 3) return 5;
            if ((y & 0x104010) == 0x4010 && Hoehe[3] < 4) return 5;
            if ((y & 0x88010) == 0x80010 && Hoehe[4] < 3) return 5;
            if ((y & 0x208000) == 0x208000 && (Hoehe[2] < 2 || Hoehe[6] < 2)) return 5;
            if ((y & 0x8408000) == 0x8008000 && Hoehe[3] < 2) return 5;
            if ((y & 0x8210000) == 0x8200000 && Hoehe[4] < 2) return 5;
            if ((y & 0x408008) == 0x8008 && Hoehe[3] < 2) return 5;
            if ((y & 0x210008) == 0x200008 && Hoehe[4] < 2) return 5;
            if ((y & 0x820004) == 0x800004 && Hoehe[4] < 1) return 5;
            if ((y & 0x810008) == 0x810000 && Hoehe[6] < 3) return 5;
            break;
        case 3:
            if ((y & 0x82008) == 0x2008 && Hoehe[3] < 5) return 5;
            if ((y & 0x44008) == 0x40008 && Hoehe[4] < 4) return 5;
            if ((y & 0x42010) == 0x42000 && Hoehe[6] < 2) return 5;
            if ((y & 0x104000) == 0x104000 && (Hoehe[2] < 3 || Hoehe[6] < 3)) return 5;
            if ((y & 0x4204000) == 0x4004000 && Hoehe[3] < 3) return 5;
            if ((y & 0x4108000) == 0x4100000 && Hoehe[4] < 3) return 5;
            if ((y & 0x204004) == 0x4004 && Hoehe[3] < 3) return 5;
            if ((y & 0x108004) == 0x100004 && Hoehe[4] < 3) return 5;
            if ((y & 0x808002) == 0x8002 && Hoehe[3] < 1) return 5;
            if ((y & 0x410002) == 0x400002 && Hoehe[4] < 2) return 5;
            if ((y & 0x408004) == 0x408000 && Hoehe[6] < 4) return 5;
            if ((y & 0x20808000) == 0x20008000 && Hoehe[3] < 1) return 5;
            if ((y & 0x20410000) == 0x20400000 && Hoehe[4] < 2) return 5;
            break;
        case 4:
            if ((y & 0x82000) == 0x82000 && (Hoehe[2] < 4 || Hoehe[6] < 4)) return 5;
            if ((y & 0x2102000) == 0x2002000 && Hoehe[3] < 4) return 5;
            if ((y & 0x2084000) == 0x2080000 && Hoehe[4] < 4) return 5;
            if ((y & 0x102002) == 0x2002 && Hoehe[3] < 4) return 5;
            if ((y & 0x84002) == 0x80002 && Hoehe[4] < 4) return 5;
            if ((y & 0x404001) == 0x4001 && Hoehe[3] < 2) return 5;
            if ((y & 0x208001) == 0x200001 && Hoehe[4] < 3) return 5;
            if ((y & 0x204000) == 0x204000 && (Hoehe[2] < 1 || Hoehe[6] < 5)) return 5;
            if ((y & 0x10404000) == 0x10004000 && Hoehe[3] < 2) return 5;
            if ((y & 0x10208000) == 0x10200000 && Hoehe[4] < 3) return 5;
            break;
        case 5:
            if ((y & 0x41000) == 0x41000 && (Hoehe[2] < 5 || Hoehe[6] < 5)) return 5;
            if ((y & 0x1081000) == 0x1001000 && Hoehe[3] < 5) return 5;
            if ((y & 0x1042000) == 0x1040000 && Hoehe[4] < 5) return 5;
            if ((y & 0x81001) == 0x1001 && Hoehe[3] < 5) return 5;
            if ((y & 0x42001) == 0x40001 && Hoehe[4] < 5) return 5;
            if ((y & 0x10102000) == 0x102000 && Hoehe[2] < 2) return 5;
            if ((y & 0x8202000) == 0x8002000 && Hoehe[3] < 3) return 5;
            if ((y & 0x8104000) == 0x8100000 && Hoehe[4] < 4) return 5;
            break;
        default:
            break;
    }
    switch (Hoehe[6]) {
        case 0:
            if ((y & 0x208400) == 0x8400 && Hoehe[3] < 3) return 6;
            if ((y & 0x110400) == 0x100400 && Hoehe[4] < 2) return 6;
            if ((y & 0x108800) == 0x108000 && Hoehe[5] < 1) return 6;
            break;
        case 1:
            if ((y & 0x104200) == 0x4200 && Hoehe[3] < 4) return 6;
            if ((y & 0x88200) == 0x80200 && Hoehe[4] < 3) return 6;
            if ((y & 0x84400) == 0x84000 && Hoehe[5] < 2) return 6;
            if ((y & 0x810400) == 0x10400 && Hoehe[3] < 1) return 6;
            if ((y & 0x420400) == 0x400400 && Hoehe[4] < 1) return 6;
            if ((y & 0x410800) == 0x410000 & Hoehe[5] < 1) return 6;
            break;
        case 2:
            if ((y & 0x82100) == 0x2100 && Hoehe[3] < 5) return 6;
            if ((y & 0x44100) == 0x40100 && Hoehe[4] < 4) return 6;
            if ((y & 0x42200) == 0x42000 && Hoehe[5] < 3) return 6;
            if ((y & 0x408200) == 0x8200 && Hoehe[3] < 2) return 6;
            if ((y & 0x210200) == 0x200200 && Hoehe[4] < 2) return 6;
            if ((y & 0x208400) == 0x208000 && Hoehe[5] < 2) return 6;
            break;
        case 3:
            if ((y & 0x204100) == 0x4100 && Hoehe[3] < 3) return 6;
            if ((y & 0x108100) == 0x100100 && Hoehe[4] < 3) return 6;
            if ((y & 0x104200) == 0x104000 && Hoehe[5] < 3) return 6;
            if ((y & 0x820200) == 0x800200 && Hoehe[4] < 1) return 6;
            if ((y & 0x810400) == 0x810000 && Hoehe[5] < 2) return 6;
            break;
        case 4:
            if ((y & 0x102080) == 0x2080 && Hoehe[3] < 4) return 6;
            if ((y & 0x84080) == 0x80080 && Hoehe[4] < 4) return 6;
            if ((y & 0x82100) == 0x82000 && Hoehe[5] < 4) return 6;
            if ((y & 0x808100) == 0x8100 && Hoehe[3] < 1) return 6;
            if ((y & 0x410100) == 0x400100 && Hoehe[4] < 2) return 6;
            if ((y & 0x408200) == 0x408000 && Hoehe[5] < 3) return 6;
            break;
        case 5:
            if ((y & 0x81040) == 0x1040 && Hoehe[3] < 5) return 6;
            if ((y & 0x42040) == 0x40040 && Hoehe[4] < 5) return 6;
            if ((y & 0x41080) == 0x41000 && Hoehe[5] < 5) return 6;
            if ((y & 0x404080) == 0x4080 && Hoehe[3] < 2) return 6;
            if ((y & 0x208080) == 0x200080 && Hoehe[4] < 3) return 6;
            if ((y & 0x204100) == 0x204000 && Hoehe[5] < 4) return 6;
            break;
        default:
            break;
    }
    return (-1);
}

short int FindEvenThreat1(void) {
    int y = Feld1;
    switch (Hoehe[0]) {
        case 0:
            if ((y & 0x8100000) == 0x8100000 && Hoehe[1] < 1) return 0;
            if ((Feld1 & 0x408000000LL) == 0x408000000LL && Hoehe[3] < 3) return 0;
            break;
        case 1:
            if ((Feld1 & 0x200080000LL) == 0x200080000LL && Hoehe[2] < 3) return 0;
            if ((y & 0x10400000) == 0x10400000 && Hoehe[1] < 1) return 0;
            if ((Feld1 & 0x400400000LL) == 0x400400000LL && Hoehe[2] < 1) return 0;
            if ((Feld1 & 0x410000000LL) == 0x410000000LL && Hoehe[3] < 1) return 0;
            break;
        case 2:
            if ((y & 0x2040000) == 0x2040000 && Hoehe[1] < 3) return 0;
            if ((Feld1 & 0x102000000LL) == 0x102000000LL && Hoehe[3] < 5) return 0;
            break;
        case 3:
            if ((y & 0x4100000) == 0x4100000 && Hoehe[1] < 3) return 0;
            if ((Feld1 & 0x100100000LL) == 0x100100000LL && Hoehe[2] < 3) return 0;
            if ((Feld1 & 0x104000000LL) == 0x104000000LL && Hoehe[3] < 3) return 0;
            if ((Feld1 & 0x200800000LL) == 0x200800000LL && Hoehe[2] < 1) return 0;
            break;
        case 4:
            if ((y & 0x8400000) == 0x8400000 && Hoehe[1] < 3) return 0;
            if ((Feld1 & 0x108000000LL) == 0x108000000LL && Hoehe[3] < 1) return 0;
            break;
        case 5:
            if ((y & 0x1040000) == 0x1040000 && Hoehe[1] < 5) return 0;
            if ((y & 0x40040000) == 0x40040000 && Hoehe[2] < 5) return 0;
            if ((y & 0x41000000) == 0x41000000 && Hoehe[3] < 5) return 0;
            if ((y & 0x80200000) == 0x80200000 && Hoehe[2] < 3) return 0;
            break;
        default:
            break;
    }
    switch (Hoehe[1]) {
        case 0:
            if ((y & 0x204000) == 0x204000 && Hoehe[2] < 1) return 1;
            if ((y & 0x10200000) == 0x10200000 && Hoehe[4] < 3) return 1;
            break;
        case 1:
            if ((y & 0x8002000) == 0x8002000 && Hoehe[3] < 3) return 1;
            if ((Feld1 & 0x20008000000LL) == 0x20008000000LL && Hoehe[3] < 3) return 1;
            if ((y & 0x10400000) == 0x10400000 && (Hoehe[0] < 1 || Hoehe[4] < 1)) return 1;
            if ((Feld1 & 0x10000400000LL) == 0x10000400000LL && Hoehe[2] < 1) return 1;
            if ((Feld1 & 0x10010000000LL) == 0x10010000000LL && Hoehe[3] < 1) return 1;
            if ((y & 0x410000) == 0x410000 && Hoehe[2] < 1) return 1;
            if ((y & 0x10010000) == 0x10010000 && Hoehe[3] < 1) return 1;
            break;
        case 2:
            if ((y & 0x81000) == 0x81000 && Hoehe[2] < 3) return 1;
            if ((y & 0x4080000) == 0x4080000 && (Hoehe[0] < 1 || Hoehe[4] < 5)) return 1;
            if ((Feld1 & 0x10000080000LL) == 0x10000080000LL && Hoehe[2] < 3) return 1;
            if ((y & 0x10800000) == 0x10800000 && Hoehe[0] < 3) return 1;
            if ((Feld1 & 0x4000800000LL) == 0x4000800000LL && Hoehe[2] < 1) return 1;
            break;
        case 3:
            if ((Feld1 & 0x8002000000LL) == 0x8002000000LL && Hoehe[3] < 5) return 1;
            if ((y & 0x4100000) == 0x4100000 && (Hoehe[0] < 3 || Hoehe[4] < 3)) return 1;
            if ((Feld1 & 0x4000100000LL) == 0x4000100000LL && Hoehe[2] < 3) return 1;
            if ((Feld1 & 0x4004000000LL) == 0x4004000000LL && Hoehe[3] < 3) return 1;
            if ((y & 0x104000) == 0x104000 && Hoehe[2] < 3) return 1;
            if ((y & 0x4004000) == 0x4004000 && Hoehe[3] < 3) return 1;
            if ((Feld1 & 0x2008000000LL) == 0x2008000000LL && Hoehe[3] < 1) return 1;
            if ((y & 0x8020000) == 0x8020000 && Hoehe[3] < 1) return 1;
            break;
        case 4:
            if ((y & 0x4200000) == 0x4200000 && (Hoehe[0] < 5 || Hoehe[4] < 1)) return 1;
            if ((Feld1 & 0x1000200000LL) == 0x1000200000LL && Hoehe[2] < 3) return 1;
            if ((y & 0x210000) == 0x210000 && Hoehe[2] < 3) return 1;
            break;
        case 5:
            if ((y & 0x1040000) == 0x1040000 && (Hoehe[0] < 5 || Hoehe[4] < 5)) return 1;
            if ((Feld1 & 0x1000040000LL) == 0x1000040000LL && Hoehe[2] < 5) return 1;
            if ((Feld1 & 0x1001000000LL) == 0x1001000000LL && Hoehe[3] < 5) return 1;
            if ((y & 0x41000) == 0x41000 && Hoehe[2] < 5) return 1;
            if ((y & 0x1001000) == 0x1001000 && Hoehe[3] < 5) return 1;
            if ((y & 0x2008000) == 0x2008000 && Hoehe[3] < 3) return 1;
            break;
        default:
            break;
    }
    switch (Hoehe[2]) {
        case 0:
            if ((y & 0x8100) == 0x8100 && Hoehe[3] < 1) return 2;
            if ((y & 0x408000) == 0x408000 && Hoehe[5] < 3) return 2;
            break;
        case 1:
            if ((y & 0x200080) == 0x200080 && Hoehe[4] < 3) return 2;
            if ((Feld1 & 0x200800000LL) == 0x200800000LL && Hoehe[0] < 3) return 2;
            if ((Feld1 & 0x800200000LL) == 0x800200000LL && Hoehe[4] < 3) return 2;
            if ((Feld1 & 0x400400000LL) == 0x400400000LL && (Hoehe[0] < 1 || Hoehe[4] < 1)) return 2;
            if ((Feld1 & 0x10000400000LL) == 0x10000400000LL && Hoehe[1] < 1) return 2;
            if ((Feld1 & 0x10400000000LL) == 0x10400000000LL && Hoehe[3] < 1) return 2;
            if ((y & 0x410000) == 0x410000 && (Hoehe[1] < 1 || Hoehe[5] < 1)) return 2;
            if ((Feld1 & 0x400010000LL) == 0x400010000LL && Hoehe[3] < 1) return 2;
            if ((y & 0x10400) == 0x10400 && Hoehe[3] < 1) return 2;
            if ((y & 0x400400) == 0x400400 && Hoehe[4] < 1) return 2;
            break;
        case 2:
            if ((y & 0x2040) == 0x2040 && Hoehe[3] < 3) return 2;
            if ((y & 0x102000) == 0x102000 && (Hoehe[1] < 1 || Hoehe[5] < 5)) return 2;
            if ((Feld1 & 0x2000400000LL) == 0x2000400000LL && Hoehe[1] < 3) return 2;
            if ((Feld1 & 0x2100000000LL) == 0x2100000000LL && Hoehe[3] < 1) return 2;
            if ((Feld1 & 0x400002000LL) == 0x400002000LL && Hoehe[3] < 3) return 2;
            if ((y & 0x420000) == 0x420000 && Hoehe[1] < 3) return 2;
            if ((Feld1 & 0x100020000LL) == 0x100020000LL && Hoehe[3] < 1) return 2;
            if ((Feld1 & 0x20000100000LL) == 0x20000100000LL && Hoehe[1] < 1) return 2;
            if ((Feld1 & 0x20400000000LL) == 0x20400000000LL && Hoehe[3] < 3) return 2;
            break;
        case 3:
            if ((y & 0x80200000) == 0x80200000 && (Hoehe[0] < 5 || Hoehe[4] < 1)) return 2;
            if ((Feld1 & 0x200080000LL) == 0x200080000LL && (Hoehe[0] < 1 || Hoehe[4] < 5)) return 2;
            if ((Feld1 & 0x100100000LL) == 0x100100000LL && (Hoehe[0] < 3 || Hoehe[4] < 3)) return 2;
            if ((Feld1 & 0x4000100000LL) == 0x4000100000LL && Hoehe[1] < 3) return 2;
            if ((Feld1 & 0x4100000000LL) == 0x4100000000LL && Hoehe[3] < 3) return 2;
            if ((y & 0x104000) == 0x104000 && (Hoehe[1] < 3 || Hoehe[5] < 3)) return 2;
            if ((Feld1 & 0x100004000LL) == 0x100004000LL && Hoehe[3] < 3) return 2;
            if ((y & 0x4100) == 0x4100 && Hoehe[3] < 3) return 2;
            if ((y & 0x100100) == 0x100100 && Hoehe[4] < 3) return 2;
            if ((y & 0x200800) == 0x200800 && Hoehe[4] < 1) return 2;
            break;
        case 4:
            if ((y & 0x108000) == 0x108000 && (Hoehe[1] < 5 || Hoehe[5] < 1)) return 2;
            if ((y & 0x40008000) == 0x40008000 && Hoehe[3] < 3) return 2;
            if ((Feld1 & 0x8000040000LL) == 0x8000040000LL && Hoehe[1] < 3) return 2;
            if ((Feld1 & 0x8100000000LL) == 0x8100000000LL && Hoehe[3] < 5) return 2;
            if ((y & 0x8400) == 0x8400 && Hoehe[3] < 3) return 2;
            break;
        case 5:
            if ((y & 0x40040000) == 0x40040000 && (Hoehe[0] < 5 || Hoehe[4] < 5)) return 2;
            if ((Feld1 & 0x1000040000LL) == 0x1000040000LL && Hoehe[1] < 5) return 2;
            if ((Feld1 & 0x1040000000LL) == 0x1040000000LL && Hoehe[3] < 5) return 2;
            if ((y & 0x41000) == 0x41000 && (Hoehe[1] < 5 || Hoehe[5] < 5)) return 2;
            if ((y & 0x40001000) == 0x40001000 && Hoehe[3] < 5) return 2;
            if ((y & 0x1040) == 0x1040 && Hoehe[3] < 5) return 2;
            if ((y & 0x40040) == 0x40040 && Hoehe[4] < 5) return 2;
            if ((y & 0x80200) == 0x80200 && Hoehe[4] < 3) return 2;
            break;
        default:
            break;
    }
    switch (Hoehe[3]) {
        case 0:
            if ((Feld1 & 0x210000000LL) == 0x210000000LL && Hoehe[0] < 3) return 3;
            if ((Feld1 & 0x4200000000LL) == 0x4200000000LL && Hoehe[2] < 1) return 3;
            if ((y & 0x204) == 0x204 && Hoehe[4] < 1) return 3;
            if ((y & 0x10200) == 0x10200 && Hoehe[6] < 3) return 3;
            break;
        case 1:
            if ((Feld1 & 0x2008000000LL) == 0x2008000000LL && Hoehe[1] < 3) return 3;
            if ((y & 0x8002) == 0x8002 && Hoehe[5] < 3) return 3;
            if ((y & 0x8020000) == 0x8020000 && Hoehe[1] < 3) return 3;
            if ((y & 0x20008000) == 0x20008000 && Hoehe[5] < 3) return 3;
            if ((Feld1 & 0x410000000LL) == 0x410000000LL && (Hoehe[0] < 1 || Hoehe[4] < 1)) return 3;
            if ((Feld1 & 0x10010000000LL) == 0x10010000000LL && Hoehe[1] < 1) return 3;
            if ((Feld1 & 0x10400000000LL) == 0x10400000000LL && Hoehe[2] < 1) return 3;
            if ((y & 0x10010000) == 0x10010000 && (Hoehe[1] < 1 || Hoehe[5] < 1)) return 3;
            if ((Feld1 & 0x400010000LL) == 0x400010000LL && Hoehe[2] < 1) return 3;
            if ((y & 0x10400) == 0x10400 && (Hoehe[2] < 1 || Hoehe[6] < 1)) return 3;
            if ((y & 0x10000400) == 0x10000400 && Hoehe[4] < 1) return 3;
            if ((y & 0x410) == 0x410 && Hoehe[4] < 1) return 3;
            if ((y & 0x10010) == 0x10010 && Hoehe[5] < 1) return 3;
            break;
        case 2:
            if ((y & 0x84000000) == 0x84000000 && (Hoehe[0] < 5 || Hoehe[4] < 1)) return 3;
            if ((Feld1 & 0x1080000000LL) == 0x1080000000LL && Hoehe[2] < 3) return 3;
            if ((y & 0x81) == 0x81 && Hoehe[4] < 3) return 3;
            if ((y & 0x4080) == 0x4080 && (Hoehe[2] < 1 || Hoehe[6] < 5)) return 3;
            if ((y & 0x80010000) == 0x80010000 && Hoehe[2] < 3) return 3;
            if ((y & 0x10000080) == 0x10000080 && Hoehe[4] < 3) return 3;
            if ((y & 0x10800) == 0x10800 && Hoehe[2] < 3) return 3;
            if ((y & 0x4000800) == 0x4000800 && Hoehe[4] < 1) return 3;
            if ((Feld1 & 0x800004000LL) == 0x800004000LL && Hoehe[2] < 1) return 3;
            if ((Feld1 & 0x810000000LL) == 0x810000000LL && Hoehe[4] < 3) return 3;
            break;
        case 3:
            if ((y & 0x2008000) == 0x2008000 && (Hoehe[1] < 5 || Hoehe[5] < 1)) return 3;
            if ((y & 0x8002000) == 0x8002000 && (Hoehe[1] < 1 || Hoehe[5] < 5)) return 3;
            if ((Feld1 & 0x104000000LL) == 0x104000000LL && (Hoehe[0] < 3 || Hoehe[4] < 3)) return 3;
            if ((Feld1 & 0x4004000000LL) == 0x4004000000LL && Hoehe[1] < 3) return 3;
            if ((Feld1 & 0x4100000000LL) == 0x4100000000LL && Hoehe[2] < 3) return 3;
            if ((y & 0x4004000) == 0x4004000 && (Hoehe[1] < 3 || Hoehe[5] < 3)) return 3;
            if ((Feld1 & 0x100004000LL) == 0x100004000LL && Hoehe[2] < 3) return 3;
            if ((y & 0x4100) == 0x4100 && (Hoehe[2] < 3 || Hoehe[6] < 3)) return 3;
            if ((y & 0x4000100) == 0x4000100 && Hoehe[4] < 3) return 3;
            if ((y & 0x104) == 0x104 && Hoehe[4] < 3) return 3;
            if ((y & 0x4004) == 0x4004 && Hoehe[5] < 3) return 3;
            if ((y & 0x8020) == 0x8020 && Hoehe[5] < 1) return 3;
            if ((Feld1 & 0x20008000000LL) == 0x20008000000LL && Hoehe[1] < 1) return 3;
            break;
        case 4:
            if ((y & 0x4200) == 0x4200 && (Hoehe[2] < 5 || Hoehe[6] < 1)) return 3;
            if ((y & 0x1000200) == 0x1000200 && Hoehe[4] < 3) return 3;
            if ((Feld1 & 0x200001000LL) == 0x200001000LL && Hoehe[2] < 3) return 3;
            if ((Feld1 & 0x204000000LL) == 0x204000000LL && (Hoehe[0] < 1 || Hoehe[4] < 5)) return 3;
            if ((y & 0x210) == 0x210 && Hoehe[4] < 3) return 3;
            if ((Feld1 & 0x10200000000LL) == 0x10200000000LL && Hoehe[2] < 3) return 3;
            break;
        case 5:
            if ((y & 0x41000000) == 0x41000000 && (Hoehe[0] < 5 || Hoehe[4] < 5)) return 3;
            if ((Feld1 & 0x1001000000LL) == 0x1001000000LL && Hoehe[1] < 5) return 3;
            if ((Feld1 & 0x1040000000LL) == 0x1040000000LL && Hoehe[2] < 5) return 3;
            if ((y & 0x1001000) == 0x1001000 && (Hoehe[1] < 5 || Hoehe[5] < 5)) return 3;
            if ((y & 0x40001000) == 0x40001000 && Hoehe[2] < 5) return 3;
            if ((y & 0x1040) == 0x1040 && (Hoehe[2] < 5 || Hoehe[6] < 5)) return 3;
            if ((y & 0x1000040) == 0x1000040 && Hoehe[4] < 5) return 3;
            if ((y & 0x41) == 0x41 && Hoehe[4] < 5) return 3;
            if ((y & 0x1001) == 0x1001 && Hoehe[5] < 5) return 3;
            if ((y & 0x2008) == 0x2008 && Hoehe[5] < 3) return 3;
            if ((Feld1 & 0x8002000000LL) == 0x8002000000LL && Hoehe[1] < 3) return 3;
            break;
        default:
            break;
    }
    switch (Hoehe[4]) {
        case 0:
            if ((y & 0x8400000) == 0x8400000 && Hoehe[1] < 3) return 4;
            if ((Feld1 & 0x108000000LL) == 0x108000000LL && Hoehe[3] < 1) return 4;
            break;
        case 1:
            if ((y & 0x80200000) == 0x80200000 && Hoehe[2] < 3) return 4;
            if ((y & 0x200800) == 0x200800 && Hoehe[2] < 3) return 4;
            if ((y & 0x800200) == 0x800200 && Hoehe[6] < 3) return 4;
            if ((y & 0x10400000) == 0x10400000 && (Hoehe[1] < 1 || Hoehe[5] < 1)) return 4;
            if ((Feld1 & 0x400400000LL) == 0x400400000LL && Hoehe[2] < 1) return 4;
            if ((Feld1 & 0x410000000LL) == 0x410000000LL && Hoehe[3] < 1) return 4;
            if ((y & 0x400400) == 0x400400 && (Hoehe[2] < 1 || Hoehe[6] < 1)) return 4;
            if ((y & 0x10000400) == 0x10000400 && Hoehe[3] < 1) return 4;
            if ((y & 0x410) == 0x410 && Hoehe[3] < 1) return 4;
            if ((y & 0x400010) == 0x400010 && Hoehe[5] < 1) return 4;
            break;
        case 2:
            if ((y & 0x2100000) == 0x2100000 && (Hoehe[1] < 5 || Hoehe[5] < 1)) return 4;
            if ((y & 0x42000000) == 0x42000000 && Hoehe[3] < 3) return 4;
            if ((y & 0x2000400) == 0x2000400 && Hoehe[3] < 3) return 4;
            if ((y & 0x102) == 0x102 && Hoehe[3] < 1) return 4;
            if ((y & 0x400002) == 0x400002 && Hoehe[5] < 3) return 4;
            if ((y & 0x420) == 0x420 && Hoehe[3] < 3) return 4;
            if ((y & 0x100020) == 0x100020 && Hoehe[5] < 1) return 4;
            if ((y & 0x20000100) == 0x20000100 && Hoehe[3] < 1) return 4;
            if ((y & 0x20400000) == 0x20400000 && Hoehe[5] < 3) return 4;
            break;
        case 3:
            if ((y & 0x80200) == 0x80200 && (Hoehe[2] < 5 || Hoehe[6] < 1)) return 4;
            if ((y & 0x200080) == 0x200080 && (Hoehe[2] < 1 || Hoehe[6] < 5)) return 4;
            if ((y & 0x4100000) == 0x4100000 && (Hoehe[1] < 3 || Hoehe[5] < 3)) return 4;
            if ((Feld1 & 0x100100000LL) == 0x100100000LL && Hoehe[2] < 3) return 4;
            if ((Feld1 & 0x104000000LL) == 0x104000000LL && Hoehe[3] < 3) return 4;
            if ((y & 0x100100) == 0x100100 && (Hoehe[2] < 3 || Hoehe[6] < 3)) return 4;
            if ((y & 0x4000100) == 0x4000100 && Hoehe[3] < 3) return 4;
            if ((y & 0x104) == 0x104 && Hoehe[3] < 3) return 4;
            if ((y & 0x100004) == 0x100004 && Hoehe[5] < 3) return 4;
            if ((Feld1 & 0x800200000LL) == 0x800200000LL && Hoehe[2] < 1) return 4;
            break;
        case 4:
            if ((y & 0x108) == 0x108 && Hoehe[3] < 5) return 4;
            if ((y & 0x40008) == 0x40008 && Hoehe[5] < 3) return 4;
            if ((y & 0x8000040) == 0x8000040 && Hoehe[3] < 3) return 4;
            if ((y & 0x8100000) == 0x8100000 && (Hoehe[1] < 1 || Hoehe[5] < 5)) return 4;
            if ((Feld1 & 0x408000000LL) == 0x408000000LL && Hoehe[3] < 3) return 4;
            break;
        case 5:
            if ((y & 0x1040000) == 0x1040000 && (Hoehe[1] < 5 || Hoehe[5] < 5)) return 4;
            if ((y & 0x40040000) == 0x40040000 && Hoehe[2] < 5) return 4;
            if ((y & 0x41000000) == 0x41000000 && Hoehe[3] < 5) return 4;
            if ((y & 0x40040) == 0x40040 && (Hoehe[2] < 5 || Hoehe[6] < 5)) return 4;
            if ((y & 0x1000040) == 0x1000040 && Hoehe[3] < 5) return 4;
            if ((y & 0x41) == 0x41 && Hoehe[3] < 5) return 4;
            if ((y & 0x40001) == 0x40001 && Hoehe[5] < 5) return 4;
            if ((Feld1 & 0x200080000LL) == 0x200080000LL && Hoehe[2] < 3) return 4;
            break;
        default:
            break;
    }
    switch (Hoehe[5]) {
        case 0:
            if ((y & 0x210000) == 0x210000 && Hoehe[2] < 3) return 5;
            if ((y & 0x4200000) == 0x4200000 && Hoehe[4] < 1) return 5;
            break;
        case 1:
            if ((y & 0x2008000) == 0x2008000 && Hoehe[3] < 3) return 5;
            if ((y & 0x8020) == 0x8020 && Hoehe[3] < 3) return 5;
            if ((y & 0x410000) == 0x410000 && (Hoehe[2] < 1 || Hoehe[6] < 1)) return 5;
            if ((y & 0x10010000) == 0x10010000 && Hoehe[3] < 1) return 5;
            if ((y & 0x10400000) == 0x10400000 && Hoehe[4] < 1) return 5;
            if ((y & 0x10010) == 0x10010 && Hoehe[3] < 1) return 5;
            if ((y & 0x400010) == 0x400010 && Hoehe[4] < 1) return 5;
            break;
        case 2:
            if ((y & 0x84000) == 0x84000 && (Hoehe[2] < 5 || Hoehe[6] < 1)) return 5;
            if ((y & 0x1080000) == 0x1080000 && Hoehe[4] < 3) return 5;
            if ((y & 0x80010) == 0x80010 && Hoehe[4] < 3) return 5;
            if ((y & 0x800004) == 0x800004 && Hoehe[4] < 1) return 5;
            if ((y & 0x810000) == 0x810000 && Hoehe[6] < 3) return 5;
            break;
        case 3:
            if ((y & 0x2008) == 0x2008 && Hoehe[3] < 5) return 5;
            if ((y & 0x104000) == 0x104000 && (Hoehe[2] < 3 || Hoehe[6] < 3)) return 5;
            if ((y & 0x4004000) == 0x4004000 && Hoehe[3] < 3) return 5;
            if ((y & 0x4100000) == 0x4100000 && Hoehe[4] < 3) return 5;
            if ((y & 0x4004) == 0x4004 && Hoehe[3] < 3) return 5;
            if ((y & 0x100004) == 0x100004 && Hoehe[4] < 3) return 5;
            if ((y & 0x8002) == 0x8002 && Hoehe[3] < 1) return 5;
            if ((y & 0x20008000) == 0x20008000 && Hoehe[3] < 1) return 5;
            break;
        case 4:
            if ((y & 0x200001) == 0x200001 && Hoehe[4] < 3) return 5;
            if ((y & 0x204000) == 0x204000 && (Hoehe[2] < 1 || Hoehe[6] < 5)) return 5;
            if ((y & 0x10200000) == 0x10200000 && Hoehe[4] < 3) return 5;
            break;
        case 5:
            if ((y & 0x41000) == 0x41000 && (Hoehe[2] < 5 || Hoehe[6] < 5)) return 5;
            if ((y & 0x1001000) == 0x1001000 && Hoehe[3] < 5) return 5;
            if ((y & 0x1040000) == 0x1040000 && Hoehe[4] < 5) return 5;
            if ((y & 0x1001) == 0x1001 && Hoehe[3] < 5) return 5;
            if ((y & 0x40001) == 0x40001 && Hoehe[4] < 5) return 5;
            if ((y & 0x8002000) == 0x8002000 && Hoehe[3] < 3) return 5;
            break;
        default:
            break;
    }
    switch (Hoehe[6]) {
        case 0:
            if ((y & 0x8400) == 0x8400 && Hoehe[3] < 3) return 6;
            if ((y & 0x108000) == 0x108000 && Hoehe[5] < 1) return 6;
            break;
        case 1:
            if ((y & 0x80200) == 0x80200 && Hoehe[4] < 3) return 6;
            if ((y & 0x10400) == 0x10400 && Hoehe[3] < 1) return 6;
            if ((y & 0x400400) == 0x400400 && Hoehe[4] < 1) return 6;
            if ((y & 0x410000) == 0x410000 && Hoehe[5] < 1) return 6;
            break;
        case 2:
            if ((y & 0x2100) == 0x2100 && Hoehe[3] < 5) return 6;
            if ((y & 0x42000) == 0x42000 && Hoehe[5] < 3) return 6;
            break;
        case 3:
            if ((y & 0x4100) == 0x4100 && Hoehe[3] < 3) return 6;
            if ((y & 0x100100) == 0x100100 && Hoehe[4] < 3) return 6;
            if ((y & 0x104000) == 0x104000 && Hoehe[5] < 3) return 6;
            if ((y & 0x800200) == 0x800200 && Hoehe[4] < 1) return 6;
            break;
        case 4:
            if ((y & 0x8100) == 0x8100 && Hoehe[3] < 1) return 6;
            if ((y & 0x408000) == 0x408000 && Hoehe[5] < 3) return 6;
            break;
        case 5:
            if ((y & 0x1040) == 0x1040 && Hoehe[3] < 5) return 6;
            if ((y & 0x40040) == 0x40040 && Hoehe[4] < 5) return 6;
            if ((y & 0x41000) == 0x41000 && Hoehe[5] < 5) return 6;
            if ((y & 0x200080) == 0x200080 && Hoehe[4] < 3) return 6;
            break;
        default:
            break;
    }
    return (-1);
}

short int FindOddThreat2(void) {
    int y = Feld2;
    switch (Hoehe[0]) {
        case 0:
            if ((Feld2 & 0x400100000LL) == 0x400100000LL && Hoehe[2] < 2) return 0;
            break;
        case 1:
            if ((y & 0x4080000) == 0x4080000 && Hoehe[1] < 2) return 0;
            if ((Feld2 & 0x204000000LL) == 0x204000000LL && Hoehe[3] < 4) return 0;
            break;
        case 2:
            if ((Feld2 & 0x100040000LL) == 0x100040000LL && Hoehe[2] < 4) return 0;
            if ((y & 0x8200000) == 0x8200000 && Hoehe[1] < 2) return 0;
            if ((Feld2 & 0x200200000LL) == 0x200200000LL && Hoehe[2] < 2) return 0;
            if ((Feld2 & 0x208000000LL) == 0x208000000LL && Hoehe[3] < 2) return 0;
            break;
        case 3:
            if ((y & 0x10800000) == 0x10800000 && Hoehe[1] < 2) return 0;
            break;
        case 4:
            if ((y & 0x2080000) == 0x2080000 && Hoehe[1] < 4) return 0;
            if ((y & 0x80080000) == 0x80080000 && Hoehe[2] < 4) return 0;
            if ((y & 0x82000000) == 0x82000000 && Hoehe[3] < 4) return 0;
            if ((Feld2 & 0x100400000LL) == 0x100400000LL && Hoehe[2] < 2) return 0;
            break;
        case 5:
            if ((y & 0x4200000) == 0x4200000 && Hoehe[1] < 4) return 0;
            if ((y & 0x84000000) == 0x84000000 && Hoehe[3] < 2) return 0;
            break;
        default:
            break;
    }
    switch (Hoehe[1]) {
        case 0:
            if ((y & 0x10004000) == 0x10004000 && Hoehe[3] < 2) return 1;
            break;
        case 1:
            if ((y & 0x102000) == 0x102000 && Hoehe[2] < 2) return 1;
            if ((y & 0x8100000) == 0x8100000 && Hoehe[4] < 4) return 1;
            if ((Feld2 & 0x20000100000LL) == 0x20000100000LL && Hoehe[2] < 2) return 1;
            break;
        case 2:
            if ((y & 0x4001000) == 0x4001000 && Hoehe[3] < 4) return 1;
            if ((Feld2 & 0x10004000000LL) == 0x10004000000LL && Hoehe[3] < 4) return 1;
            if ((y & 0x8200000) == 0x8200000 && (Hoehe[0] < 2 || Hoehe[4] < 2)) return 1;
            if ((Feld2 & 0x8000200000LL) == 0x8000200000LL && Hoehe[2] < 2) return 1;
            if ((Feld2 & 0x8008000000LL) == 0x8008000000LL && Hoehe[3] < 2) return 1;
            if ((y & 0x208000) == 0x208000 && Hoehe[2] < 2) return 1;
            if ((y & 0x8008000) == 0x8008000 && Hoehe[3] < 2) return 1;
            break;
        case 3:
            if ((y & 0x2040000) == 0x2040000 && Hoehe[0] < 2) return 1;
            if ((Feld2 & 0x8000040000LL) == 0x8000040000LL && Hoehe[2] < 4) return 1;
            if ((y & 0x8400000) == 0x8400000 && Hoehe[0] < 4) return 1;
            if ((Feld2 & 0x2000400000LL) == 0x2000400000LL && Hoehe[2] < 2) return 1;
            if ((y & 0x420000) == 0x420000 && Hoehe[2] < 2) return 1;
            break;
        case 4:
            if ((y & 0x2080000) == 0x2080000 && (Hoehe[0] < 4 || Hoehe[4] < 4)) return 1;
            if ((Feld2 & 0x2000080000LL) == 0x2000080000LL && Hoehe[2] < 4) return 1;
            if ((Feld2 & 0x2002000000LL) == 0x2002000000LL && Hoehe[3] < 4) return 1;
            if ((y & 0x82000) == 0x82000 && Hoehe[2] < 4) return 1;
            if ((y & 0x2002000) == 0x2002000 && Hoehe[3] < 4) return 1;
            if ((Feld2 & 0x1004000000LL) == 0x1004000000LL && Hoehe[3] < 2) return 1;
            if ((y & 0x4010000) == 0x4010000 && Hoehe[3] < 2) return 1;
            break;
        case 5:
            if ((y & 0x108000) == 0x108000 && Hoehe[2] < 4) return 1;
            if ((y & 0x2100000) == 0x2100000 && Hoehe[4] < 2) return 1;
            break;
        default:
            break;
    }
    switch (Hoehe[2]) {
        case 0:
            if ((y & 0x400100) == 0x400100 && Hoehe[4] < 2) return 2;
            break;
        case 1:
            if ((y & 0x4080) == 0x4080 && Hoehe[3] < 2) return 2;
            if ((y & 0x204000) == 0x204000 && Hoehe[5] < 4) return 2;
            if ((Feld2 & 0x4000800000LL) == 0x4000800000LL && Hoehe[1] < 2) return 2;
            if ((Feld2 & 0x800004000LL) == 0x800004000LL && Hoehe[3] < 2) return 2;
            break;
        case 2:
            if ((y & 0x100040) == 0x100040 && Hoehe[4] < 4) return 2;
            if ((Feld2 & 0x100400000LL) == 0x100400000LL && Hoehe[0] < 4) return 2;
            if ((Feld2 & 0x400100000LL) == 0x400100000LL && Hoehe[4] < 4) return 2;
            if ((Feld2 & 0x200200000LL) == 0x200200000LL && (Hoehe[0] < 2 || Hoehe[4] < 2)) return 2;
            if ((Feld2 & 0x8000200000LL) == 0x8000200000LL && Hoehe[1] < 2) return 2;
            if ((Feld2 & 0x8200000000LL) == 0x8200000000LL && Hoehe[3] < 2) return 2;
            if ((y & 0x208000) == 0x208000 && (Hoehe[1] < 2 || Hoehe[5] < 2)) return 2;
            if ((Feld2 & 0x200008000LL) == 0x200008000LL && Hoehe[3] < 2) return 2;
            if ((y & 0x8200) == 0x8200 && Hoehe[3] < 2) return 2;
            if ((y & 0x200200) == 0x200200 && Hoehe[4] < 2) return 2;
            break;
        case 3:
            if ((Feld2 & 0x1000200000LL) == 0x1000200000LL && Hoehe[1] < 4) return 2;
            if ((Feld2 & 0x1080000000LL) == 0x1080000000LL && Hoehe[3] < 2) return 2;
            if ((y & 0x81000) == 0x81000 && Hoehe[1] < 2) return 2;
            if ((Feld2 & 0x200001000LL) == 0x200001000LL && Hoehe[3] < 4) return 2;
            if ((y & 0x210000) == 0x210000 && Hoehe[1] < 4) return 2;
            if ((y & 0x80010000) == 0x80010000 && Hoehe[3] < 2) return 2;
            if ((Feld2 & 0x10000080000LL) == 0x10000080000LL && Hoehe[1] < 2) return 2;
            if ((Feld2 & 0x10200000000LL) == 0x10200000000LL && Hoehe[3] < 4) return 2;
            if ((y & 0x10800) == 0x10800 && Hoehe[3] < 2) return 2;
            break;
        case 4:
            if ((y & 0x80080000) == 0x80080000 && (Hoehe[0] < 4 || Hoehe[4] < 4)) return 2;
            if ((Feld2 & 0x2000080000LL) == 0x2000080000LL && Hoehe[1] < 4) return 2;
            if ((Feld2 & 0x2080000000LL) == 0x2080000000LL && Hoehe[3] < 4) return 2;
            if ((y & 0x82000) == 0x82000 && (Hoehe[1] < 4 || Hoehe[5] < 4)) return 2;
            if ((y & 0x80002000) == 0x80002000 && Hoehe[3] < 4) return 2;
            if ((y & 0x2080) == 0x2080 && Hoehe[3] < 4) return 2;
            if ((y & 0x80080) == 0x80080 && Hoehe[4] < 4) return 2;
            if ((y & 0x40100000) == 0x40100000 && Hoehe[4] < 2) return 2;
            if ((Feld2 & 0x100040000LL) == 0x100040000LL && Hoehe[0] < 2) return 2;
            if ((y & 0x100400) == 0x100400 && Hoehe[4] < 2) return 2;
            break;
        case 5:
            if ((y & 0x4200) == 0x4200 && Hoehe[3] < 4) return 2;
            if ((y & 0x84000) == 0x84000 && Hoehe[5] < 2) return 2;
            break;
        default:
            break;
    }
    switch (Hoehe[3]) {
        case 0:
            if ((Feld2 & 0x4010000000LL) == 0x4010000000LL && Hoehe[1] < 2) return 3;
            if ((y & 0x10004) == 0x10004 && Hoehe[5] < 2) return 3;
            break;
        case 1:
            if ((Feld2 & 0x108000000LL) == 0x108000000LL && Hoehe[0] < 4) return 3;
            if ((Feld2 & 0x2100000000LL) == 0x2100000000LL && Hoehe[2] < 2) return 3;
            if ((y & 0x102) == 0x102 && Hoehe[4] < 2) return 3;
            if ((y & 0x8100) == 0x8100 && Hoehe[6] < 4) return 3;
            if ((Feld2 & 0x100020000LL) == 0x100020000LL && Hoehe[2] < 2) return 3;
            if ((y & 0x20000100) == 0x20000100 && Hoehe[4] < 2) return 3;
            break;
        case 2:
            if ((Feld2 & 0x1004000000LL) == 0x1004000000LL && Hoehe[1] < 4) return 3;
            if ((y & 0x4001) == 0x4001 && Hoehe[5] < 4) return 3;
            if ((y & 0x4010000) == 0x4010000 && Hoehe[1] < 4) return 3;
            if ((y & 0x10004000) == 0x10004000 && Hoehe[5] < 4) return 3;
            if ((Feld2 & 0x208000000LL) == 0x208000000LL && (Hoehe[0] < 2 || Hoehe[4] < 2)) return 3;
            if ((Feld2 & 0x8008000000LL) == 0x8008000000LL && Hoehe[1] < 2) return 3;
            if ((Feld2 & 0x8200000000LL) == 0x8200000000LL && Hoehe[2] < 2) return 3;
            if ((y & 0x8008000) == 0x8008000 && (Hoehe[1] < 2 || Hoehe[5] < 2)) return 3;
            if ((Feld2 & 0x200008000LL) == 0x200008000LL && Hoehe[2] < 2) return 3;
            if ((y & 0x8200) == 0x8200 && (Hoehe[2] < 2 || Hoehe[6] < 2)) return 3;
            if ((y & 0x8000200) == 0x8000200 && Hoehe[4] < 2) return 3;
            if ((y & 0x208) == 0x208 && Hoehe[4] < 2) return 3;
            if ((y & 0x8008) == 0x8008 && Hoehe[5] < 2) return 3;
            break;
        case 3:
            if ((y & 0x40008000) == 0x40008000 && Hoehe[2] < 4) return 3;
            if ((y & 0x42000000) == 0x42000000 && Hoehe[4] < 2) return 3;
            if ((y & 0x2040) == 0x2040 && Hoehe[2] < 2) return 3;
            if ((y & 0x8000040) == 0x8000040 && Hoehe[4] < 4) return 3;
            if ((y & 0x8400) == 0x8400 && Hoehe[2] < 4) return 3;
            if ((y & 0x2000400) == 0x2000400 && Hoehe[4] < 2) return 3;
            if ((Feld2 & 0x400002000LL) == 0x400002000LL && Hoehe[2] < 2) return 3;
            if ((Feld2 & 0x408000000LL) == 0x408000000LL && Hoehe[4] < 4) return 3;
            if ((y & 0x420) == 0x420 && Hoehe[4] < 2) return 3;
            if ((Feld2 & 0x20400000000LL) == 0x20400000000LL && Hoehe[2] < 2) return 3;
            break;
        case 4:
            if ((y & 0x82000000) == 0x82000000 && (Hoehe[0] < 4 || Hoehe[4] < 4)) return 3;
            if ((Feld2 & 0x2002000000LL) == 0x2002000000LL && Hoehe[1] < 4) return 3;
            if ((Feld2 & 0x2080000000LL) == 0x2080000000LL && Hoehe[2] < 4) return 3;
            if ((y & 0x2002000) == 0x2002000 && (Hoehe[1] < 4 || Hoehe[5] < 4)) return 3;
            if ((y & 0x80002000) == 0x80002000 && Hoehe[2] < 4) return 3;
            if ((y & 0x2080) == 0x2080 && (Hoehe[2] < 4 || Hoehe[6] < 4)) return 3;
            if ((y & 0x2000080) == 0x2000080 && Hoehe[4] < 4) return 3;
            if ((y & 0x82) == 0x82 && Hoehe[4] < 4) return 3;
            if ((y & 0x2002) == 0x2002 && Hoehe[5] < 4) return 3;
            if ((y & 0x1004000) == 0x1004000 && Hoehe[5] < 2) return 3;
            if ((y & 0x4001000) == 0x4001000 && Hoehe[1] < 2) return 3;
            if ((y & 0x4010) == 0x4010 && Hoehe[5] < 2) return 3;
            if ((Feld2 & 0x10004000000LL) == 0x10004000000LL && Hoehe[1] < 2) return 3;
            break;
        case 5:
            if ((y & 0x108) == 0x108 && Hoehe[4] < 4) return 3;
            if ((y & 0x2100) == 0x2100 && Hoehe[6] < 2) return 3;
            if ((Feld2 & 0x102000000LL) == 0x102000000LL && Hoehe[0] < 2) return 3;
            if ((Feld2 & 0x8100000000LL) == 0x8100000000LL && Hoehe[2] < 4) return 3;
            break;
        default:
            break;
    }
    switch (Hoehe[4]) {
        case 0:
            if ((Feld2 & 0x100400000LL) == 0x100400000LL && Hoehe[2] < 2) return 4;
            break;
        case 1:
            if ((y & 0x4200000) == 0x4200000 && Hoehe[1] < 4) return 4;
            if ((y & 0x84000000) == 0x84000000 && Hoehe[3] < 2) return 4;
            if ((y & 0x4000800) == 0x4000800 && Hoehe[3] < 2) return 4;
            if ((y & 0x800004) == 0x800004 && Hoehe[5] < 2) return 4;
            break;
        case 2:
            if ((y & 0x40100000) == 0x40100000 && Hoehe[2] < 4) return 4;
            if ((y & 0x100400) == 0x100400 && Hoehe[2] < 4) return 4;
            if ((y & 0x400100) == 0x400100 && Hoehe[6] < 4) return 4;
            if ((y & 0x8200000) == 0x8200000 && (Hoehe[1] < 2 || Hoehe[5] < 2)) return 4;
            if ((Feld2 & 0x200200000LL) == 0x200200000LL && Hoehe[2] < 2) return 4;
            if ((Feld2 & 0x208000000LL) == 0x208000000LL && Hoehe[3] < 2) return 4;
            if ((y & 0x200200) == 0x200200 && (Hoehe[2] < 2 || Hoehe[6] < 2)) return 4;
            if ((y & 0x8000200) == 0x8000200 && Hoehe[3] < 2) return 4;
            if ((y & 0x208) == 0x208 && Hoehe[3] < 2) return 4;
            if ((y & 0x200008) == 0x200008 && Hoehe[5] < 2) return 4;
            break;
        case 3:
            if ((y & 0x1000200) == 0x1000200 && Hoehe[3] < 4) return 4;
            if ((y & 0x1080000) == 0x1080000 && Hoehe[5] < 2) return 4;
            if ((y & 0x81) == 0x81 && Hoehe[3] < 2) return 4;
            if ((y & 0x200001) == 0x200001 && Hoehe[5] < 4) return 4;
            if ((y & 0x210) == 0x210 && Hoehe[3] < 4) return 4;
            if ((y & 0x80010) == 0x80010 && Hoehe[5] < 2) return 4;
            if ((y & 0x10000080) == 0x10000080 && Hoehe[3] < 2) return 4;
            if ((y & 0x10200000) == 0x10200000 && Hoehe[5] < 4) return 4;
            if ((Feld2 & 0x810000000LL) == 0x810000000LL && Hoehe[3] < 2) return 4;
            break;
        case 4:
            if ((y & 0x2080000) == 0x2080000 && (Hoehe[1] < 4 || Hoehe[5] < 4)) return 4;
            if ((y & 0x80080000) == 0x80080000 && Hoehe[2] < 4) return 4;
            if ((y & 0x82000000) == 0x82000000 && Hoehe[3] < 4) return 4;
            if ((y & 0x80080) == 0x80080 && (Hoehe[2] < 4 || Hoehe[6] < 4)) return 4;
            if ((y & 0x2000080) == 0x2000080 && Hoehe[3] < 4) return 4;
            if ((y & 0x82) == 0x82 && Hoehe[3] < 4) return 4;
            if ((y & 0x80002) == 0x80002 && Hoehe[5] < 4) return 4;
            if ((y & 0x40100) == 0x40100 && Hoehe[6] < 2) return 4;
            if ((y & 0x100040) == 0x100040 && Hoehe[2] < 2) return 4;
            if ((Feld2 & 0x400100000LL) == 0x400100000LL && Hoehe[2] < 2) return 4;
            break;
        case 5:
            if ((y & 0x4080000) == 0x4080000 && Hoehe[1] < 2) return 4;
            if ((Feld2 & 0x204000000LL) == 0x204000000LL && Hoehe[3] < 4) return 4;
            break;
        default:
            break;
    }
    switch (Hoehe[5]) {
        case 0:
            if ((y & 0x4010000) == 0x4010000 && Hoehe[3] < 2) return 5;
            break;
        case 1:
            if ((y & 0x108000) == 0x108000 && Hoehe[2] < 4) return 5;
            if ((y & 0x2100000) == 0x2100000 && Hoehe[4] < 2) return 5;
            if ((y & 0x100020) == 0x100020 && Hoehe[4] < 2) return 5;
            break;
        case 2:
            if ((y & 0x1004000) == 0x1004000 && Hoehe[3] < 4) return 5;
            if ((y & 0x4010) == 0x4010 && Hoehe[3] < 4) return 5;
            if ((y & 0x208000) == 0x208000 && (Hoehe[2] < 2 || Hoehe[6] < 2)) return 5;
            if ((y & 0x8008000) == 0x8008000 && Hoehe[3] < 2) return 5;
            if ((y & 0x8200000) == 0x8200000 && Hoehe[4] < 2) return 5;
            if ((y & 0x8008) == 0x8008 && Hoehe[3] < 2) return 5;
            if ((y & 0x200008) == 0x200008 && Hoehe[4] < 2) return 5;
            break;
        case 3:
            if ((y & 0x40008) == 0x40008 && Hoehe[4] < 4) return 5;
            if ((y & 0x42000) == 0x42000 && Hoehe[6] < 2) return 5;
            if ((y & 0x400002) == 0x400002 && Hoehe[4] < 2) return 5;
            if ((y & 0x408000) == 0x408000 && Hoehe[6] < 4) return 5;
            if ((y & 0x20400000) == 0x20400000 && Hoehe[4] < 2) return 5;
            break;
        case 4:
            if ((y & 0x82000) == 0x82000 && (Hoehe[2] < 4 || Hoehe[6] < 4)) return 5;
            if ((y & 0x2002000) == 0x2002000 && Hoehe[3] < 4) return 5;
            if ((y & 0x2080000) == 0x2080000 && Hoehe[4] < 4) return 5;
            if ((y & 0x2002) == 0x2002 && Hoehe[3] < 4) return 5;
            if ((y & 0x80002) == 0x80002 && Hoehe[4] < 4) return 5;
            if ((y & 0x4001) == 0x4001 && Hoehe[3] < 2) return 5;
            if ((y & 0x10004000) == 0x10004000 && Hoehe[3] < 2) return 5;
            break;
        case 5:
            if ((y & 0x102000) == 0x102000 && Hoehe[2] < 2) return 5;
            if ((y & 0x8100000) == 0x8100000 && Hoehe[4] < 4) return 5;
            break;
        default:
            break;
    }
    switch (Hoehe[6]) {
        case 0:
            if ((y & 0x100400) == 0x100400 && Hoehe[4] < 2) return 6;
            break;
        case 1:
            if ((y & 0x4200) == 0x4200 && Hoehe[3] < 4) return 6;
            if ((y & 0x84000) == 0x84000 && Hoehe[5] < 2) return 6;
            break;
        case 2:
            if ((y & 0x40100) == 0x40100 && Hoehe[4] < 4) return 6;
            if ((y & 0x8200) == 0x8200 && Hoehe[3] < 2) return 6;
            if ((y & 0x200200) == 0x200200 && Hoehe[4] < 2) return 6;
            if ((y & 0x208000) == 0x208000 && Hoehe[5] < 2) return 6;
            break;
        case 3:
            if ((y & 0x810000) == 0x810000 && Hoehe[5] < 2) return 6;
            break;
        case 4:
            if ((y & 0x2080) == 0x2080 && Hoehe[3] < 4) return 6;
            if ((y & 0x80080) == 0x80080 && Hoehe[4] < 4) return 6;
            if ((y & 0x82000) == 0x82000 && Hoehe[5] < 4) return 6;
            if ((y & 0x400100) == 0x400100 && Hoehe[4] < 2) return 6;
            break;
        case 5:
            if ((y & 0x4080) == 0x4080 && Hoehe[3] < 2) return 6;
            if ((y & 0x204000) == 0x204000 && Hoehe[5] < 4) return 6;
            break;
        default:
            break;
    }
    return (-1);
}


short int FindOtherThreat1(const short x) {
    int y = Feld1;
    switch (x + 1) {
        case 1:
            switch (Hoehe[1]) {
                case 0:
                    if ((y & 0x20204000) == 0x204000 && Hoehe[2] < 1) return 1;
                    if ((y & 0x10404000) == 0x10004000 && Hoehe[3] < 2) return 1;
                    if ((y & 0x10208000) == 0x10200000 && Hoehe[4] < 3) return 1;
                    break;
                case 1:
                    if ((y & 0x10102000) == 0x102000 && Hoehe[2] < 2) return 1;
                    if ((y & 0x8202000) == 0x8002000 && Hoehe[3] < 3) return 1;
                    if ((y & 0x8104000) == 0x8100000 && Hoehe[4] < 4) return 1;
                    if ((Feld1 & 0x20010100000LL) == 0x20000100000LL && Hoehe[2] < 2) return 1;
                    if ((Feld1 & 0x20008200000LL) == 0x20008000000LL && Hoehe[3] < 3) return 1;
                    if ((Feld1 & 0x10400000LL) == 0x10400000LL && (Hoehe[0] < 1 || Hoehe[4] < 1)) return 1;
                    if ((Feld1 & 0x10020400000LL) == 0x10000400000LL && Hoehe[2] < 1) return 1;
                    if ((Feld1 & 0x10010800000LL) == 0x10010000000LL && Hoehe[3] < 1) return 1;
                    if ((y & 0x20410000) == 0x410000 && Hoehe[2] < 1) return 1;
                    if ((y & 0x10810000) == 0x10010000 && Hoehe[3] < 1) return 1;
                    break;
                case 2:
                    if ((y & 0x8081000) == 0x81000 && Hoehe[2] < 3) return 1;
                    if ((y & 0x4101000) == 0x4001000 && Hoehe[3] < 4) return 1;
                    if ((Feld1 & 0x4080000LL) == 0x4080000LL && (Hoehe[0] < 1 || Hoehe[4] < 5)) return 1;
                    if ((Feld1 & 0x10008080000LL) == 0x10000080000LL && Hoehe[2] < 3) return 1;
                    if ((Feld1 & 0x10004100000LL) == 0x10004000000LL && Hoehe[3] < 4) return 1;
                    if ((Feld1 & 0x8200000LL) == 0x8200000LL && (Hoehe[0] < 2 || Hoehe[4] < 2)) return 1;
                    if ((Feld1 & 0x8010200000LL) == 0x8000200000LL && Hoehe[2] < 2) return 1;
                    if ((Feld1 & 0x8008400000LL) == 0x8008000000LL && Hoehe[3] < 2) return 1;
                    if ((y & 0x10208000) == 0x208000 && Hoehe[2] < 2) return 1;
                    if ((y & 0x8408000) == 0x8008000 && Hoehe[3] < 2) return 1;
                    if ((Feld1 & 0x8010800000LL) == 0x10800000LL && Hoehe[0] < 3) return 1;
                    if ((Feld1 & 0x4020800000LL) == 0x4000800000LL && Hoehe[2] < 1) return 1;
                    break;
                case 3:
                    if ((Feld1 & 0x10002040000LL) == 0x2040000LL && Hoehe[0] < 2) return 1;
                    if ((Feld1 & 0x8004040000LL) == 0x8000040000LL && Hoehe[2] < 4) return 1;
                    if ((Feld1 & 0x8002080000LL) == 0x8002000000LL && Hoehe[3] < 5) return 1;
                    if ((Feld1 & 0x4100000LL) == 0x4100000LL && (Hoehe[0] < 3 || Hoehe[4] < 3)) return 1;
                    if ((Feld1 & 0x4008100000LL) == 0x4000100000LL && Hoehe[2] < 3) return 1;
                    if ((Feld1 & 0x4004200000LL) == 0x4004000000LL && Hoehe[3] < 3) return 1;
                    if ((y & 0x8104000) == 0x104000 && Hoehe[2] < 3) return 1;
                    if ((y & 0x4204000) == 0x4004000 && Hoehe[3] < 3) return 1;
                    if ((Feld1 & 0x4008400000LL) == 0x8400000LL && Hoehe[0] < 4) return 1;
                    if ((Feld1 & 0x2010400000LL) == 0x2000400000LL && Hoehe[2] < 2) return 1;
                    if ((Feld1 & 0x2008800000LL) == 0x2008000000LL && Hoehe[3] < 1) return 1;
                    if ((y & 0x10420000) == 0x420000 && Hoehe[2] < 2) return 1;
                    if ((y & 0x8820000) == 0x8020000 && Hoehe[3] < 1) return 1;
                    break;
                case 4:
                    if ((Feld1 & 0x2080000LL) == 0x2080000LL && (Hoehe[0] < 4 || Hoehe[4] < 4)) return 1;
                    if ((Feld1 & 0x2004080000LL) == 0x2000080000LL && Hoehe[2] < 4) return 1;
                    if ((Feld1 & 0x2002100000LL) == 0x2002000000LL && Hoehe[3] < 4) return 1;
                    if ((y & 0x4082000) == 0x82000 && Hoehe[2] < 4) return 1;
                    if ((y & 0x2102000) == 0x2002000 && Hoehe[3] < 4) return 1;
                    if ((Feld1 & 0x4200000LL) == 0x4200000LL && (Hoehe[0] < 5 || Hoehe[4] < 1)) return 1;
                    if ((Feld1 & 0x1008200000LL) == 0x1000200000LL && Hoehe[2] < 3) return 1;
                    if ((Feld1 & 0x1004400000LL) == 0x1004000000LL && Hoehe[3] < 2) return 1;
                    if ((y & 0x8210000) == 0x210000 && Hoehe[2] < 3) return 1;
                    if ((y & 0x4410000) == 0x4010000 && Hoehe[3] < 2) return 1;
                    break;
                case 5:
                    if ((Feld1 & 0x1040000LL) == 0x1040000LL && (Hoehe[0] < 5 || Hoehe[4] < 5)) return 1;
                    if ((Feld1 & 0x1002040000LL) == 0x1000040000LL && Hoehe[2] < 5) return 1;
                    if ((Feld1 & 0x1001080000LL) == 0x1001000000LL && Hoehe[3] < 5) return 1;
                    if ((y & 0x2041000) == 0x41000 && Hoehe[2] < 5) return 1;
                    if ((y & 0x1081000) == 0x1001000 && Hoehe[3] < 5) return 1;
                    if ((y & 0x4108000) == 0x108000 && Hoehe[2] < 4) return 1;
                    if ((y & 0x2208000) == 0x2008000 && Hoehe[3] < 3) return 1;
                    if ((y & 0x2110000) == 0x2100000 && Hoehe[4] < 2) return 1;
                    break;
                default:
                    break;
            }
        case 2:
            switch (Hoehe[2]) {
                case 0:
                    if ((y & 0x808100) == 0x8100 && Hoehe[3] < 1) return 2;
                    if ((y & 0x410100) == 0x400100 && Hoehe[4] < 2) return 2;
                    if ((y & 0x408200) == 0x408000 && Hoehe[5] < 3) return 2;
                    break;
                case 1:
                    if ((y & 0x404080) == 0x4080 && Hoehe[3] < 2) return 2;
                    if ((y & 0x208080) == 0x200080 && Hoehe[4] < 3) return 2;
                    if ((y & 0x204100) == 0x204000 && Hoehe[5] < 4) return 2;
                    if ((Feld1 & 0x8200800000LL) == 0x200800000LL && Hoehe[0] < 3) return 2;
                    if ((Feld1 & 0x4400800000LL) == 0x4000800000LL && Hoehe[1] < 2) return 2;
                    if ((Feld1 & 0x800404000LL) == 0x800004000LL && Hoehe[3] < 2) return 2;
                    if ((Feld1 & 0x800208000LL) == 0x800200000LL && Hoehe[4] < 3) return 2;
                    if ((Feld1 & 0x400400000LL) == 0x400400000LL && (Hoehe[0] < 1 || Hoehe[4] < 1)) return 2;
                    if ((Feld1 & 0x10800400000LL) == 0x10000400000LL && Hoehe[1] < 1) return 2;
                    if ((Feld1 & 0x10400800000LL) == 0x10400000000LL && Hoehe[3] < 1) return 2;
                    if ((Feld1 & 0x410000LL) == 0x410000LL && (Hoehe[1] < 1 || Hoehe[5] < 1)) return 2;
                    if ((Feld1 & 0x400810000LL) == 0x400010000LL && Hoehe[3] < 1) return 2;
                    if ((y & 0x810400) == 0x10400 && Hoehe[3] < 1) return 2;
                    if ((y & 0x420400) == 0x400400 && Hoehe[4] < 1) return 2;
                    break;
                case 2:
                    if ((y & 0x202040) == 0x2040 && Hoehe[3] < 3) return 2;
                    if ((y & 0x104040) == 0x100040 && Hoehe[4] < 4) return 2;
                    if ((Feld1 & 0x102000LL) == 0x102000LL && (Hoehe[1] < 1 || Hoehe[5] < 5)) return 2;
                    if ((Feld1 & 0x4100400000LL) == 0x100400000LL && Hoehe[0] < 4) return 2;
                    if ((Feld1 & 0x2200400000LL) == 0x2000400000LL && Hoehe[1] < 3) return 2;
                    if ((Feld1 & 0x2100800000LL) == 0x2100000000LL && Hoehe[3] < 1) return 2;
                    if ((Feld1 & 0x400202000LL) == 0x400002000LL && Hoehe[3] < 3) return 2;
                    if ((Feld1 & 0x400104000LL) == 0x400100000LL && Hoehe[4] < 4) return 2;
                    if ((Feld1 & 0x200200000LL) == 0x200200000LL && (Hoehe[0] < 2 || Hoehe[4] < 2)) return 2;
                    if ((Feld1 & 0x8400200000LL) == 0x8000200000LL && Hoehe[1] < 2) return 2;
                    if ((Feld1 & 0x8200400000LL) == 0x8200000000LL && Hoehe[3] < 2) return 2;
                    if ((Feld1 & 0x208000LL) == 0x208000LL && (Hoehe[1] < 2 || Hoehe[5] < 2)) return 2;
                    if ((Feld1 & 0x200408000LL) == 0x200008000LL && Hoehe[3] < 2) return 2;
                    if ((y & 0x408200) == 0x8200 && Hoehe[3] < 2) return 2;
                    if ((y & 0x210200) == 0x200200 && Hoehe[4] < 2) return 2;
                    if ((Feld1 & 0x200420000LL) == 0x420000LL && Hoehe[1] < 3) return 2;
                    if ((Feld1 & 0x100820000LL) == 0x100020000LL && Hoehe[3] < 1) return 2;
                    if ((Feld1 & 0x20800100000LL) == 0x20000100000LL && Hoehe[1] < 1) return 2;
                    if ((Feld1 & 0x20400200000LL) == 0x20400000000LL && Hoehe[3] < 3) return 2;
                    break;
                case 3:
                    if ((Feld1 & 0x80200000LL) == 0x80200000LL && (Hoehe[0] < 5 || Hoehe[4] < 1)) return 2;
                    if ((Feld1 & 0x1100200000LL) == 0x1000200000LL && Hoehe[1] < 4) return 2;
                    if ((Feld1 & 0x1080400000LL) == 0x1080000000LL && Hoehe[3] < 2) return 2;
                    if ((Feld1 & 0x400081000LL) == 0x81000LL && Hoehe[1] < 2) return 2;
                    if ((Feld1 & 0x200101000LL) == 0x200001000LL && Hoehe[3] < 4) return 2;
                    if ((Feld1 & 0x200080000LL) == 0x200080000LL && (Hoehe[0] < 1 || Hoehe[4] < 5)) return 2;
                    if ((Feld1 & 0x100100000LL) == 0x100100000LL && (Hoehe[0] < 3 || Hoehe[4] < 3)) return 2;
                    if ((Feld1 & 0x4200100000LL) == 0x4000100000LL && Hoehe[1] < 3) return 2;
                    if ((Feld1 & 0x4100200000LL) == 0x4100000000LL && Hoehe[3] < 3) return 2;
                    if ((Feld1 & 0x104000LL) == 0x104000LL && (Hoehe[1] < 3 || Hoehe[5] < 3)) return 2;
                    if ((Feld1 & 0x100204000LL) == 0x100004000LL && Hoehe[3] < 3) return 2;
                    if ((y & 0x204100) == 0x4100 && Hoehe[3] < 3) return 2;
                    if ((y & 0x108100) == 0x100100 && Hoehe[4] < 3) return 2;
                    if ((Feld1 & 0x100210000LL) == 0x210000LL && Hoehe[1] < 4) return 2;
                    if ((y & 0x80410000) == 0x80010000 && Hoehe[3] < 2) return 2;
                    if ((Feld1 & 0x10400080000LL) == 0x10000080000LL && Hoehe[1] < 2) return 2;
                    if ((Feld1 & 0x10200100000LL) == 0x10200000000LL && Hoehe[3] < 4) return 2;
                    if ((y & 0x410800) == 0x10800 && Hoehe[3] < 2) return 2;
                    if ((y & 0x220800) == 0x200800 && Hoehe[4] < 1) return 2;
                    break;
                case 4:
                    if ((Feld1 & 0x80080000LL) == 0x80080000LL && (Hoehe[0] < 4 || Hoehe[4] < 4)) return 2;
                    if ((Feld1 & 0x2100080000LL) == 0x2000080000LL && Hoehe[1] < 4) return 2;
                    if ((Feld1 & 0x2080100000LL) == 0x2080000000LL && Hoehe[3] < 4) return 2;
                    if ((Feld1 & 0x82000LL) == 0x82000LL && (Hoehe[1] < 4 || Hoehe[5] < 4)) return 2;
                    if ((y & 0x80102000) == 0x80002000 && Hoehe[3] < 4) return 2;
                    if ((y & 0x102080) == 0x2080 && Hoehe[3] < 4) return 2;
                    if ((y & 0x84080) == 0x80080 && Hoehe[4] < 4) return 2;
                    if ((y & 0x108000) == 0x108000 && (Hoehe[1] < 5 || Hoehe[5] < 1)) return 2;
                    if ((y & 0x40208000) == 0x40008000 && Hoehe[3] < 3) return 2;
                    if ((y & 0x40110000) == 0x40100000 && Hoehe[4] < 2) return 2;
                    if ((Feld1 & 0x10100040000LL) == 0x100040000LL && Hoehe[0] < 2) return 2;
                    if ((Feld1 & 0x8200040000LL) == 0x8000040000LL && Hoehe[1] < 3) return 2;
                    if ((Feld1 & 0x8100080000LL) == 0x8100000000LL && Hoehe[3] < 5) return 2;
                    if ((y & 0x208400) == 0x8400 && Hoehe[3] < 3) return 2;
                    if ((y & 0x110400) == 0x100400 && Hoehe[4] < 2) return 2;
                    break;
                case 5:
                    if ((Feld1 & 0x40040000LL) == 0x40040000LL && (Hoehe[0] < 5 || Hoehe[4] < 5)) return 2;
                    if ((Feld1 & 0x1080040000LL) == 0x1000040000LL && Hoehe[1] < 5) return 2;
                    if ((Feld1 & 0x1040080000LL) == 0x1040000000LL && Hoehe[3] < 5) return 2;
                    if ((y & 0x80041080) == 0x41000 && (Hoehe[1] < 5 || Hoehe[5] < 5)) return 2;
                    if ((y & 0x40081000) == 0x40001000 && Hoehe[3] < 5) return 2;
                    if ((y & 0x81040) == 0x1040 && Hoehe[3] < 5) return 2;
                    if ((y & 0x42040) == 0x40040 && Hoehe[4] < 5) return 2;
                    if ((y & 0x104200) == 0x4200 && Hoehe[3] < 4) return 2;
                    if ((y & 0x88200) == 0x80200 && Hoehe[4] < 3) return 2;
                    if ((y & 0x84400) == 0x84000 && Hoehe[5] < 2) return 2;
                    break;
                default:
                    break;
            }
        case 3:
            switch (Hoehe[3]) {
                case 0:
                    if ((Feld1 & 0x8210000000LL) == 0x210000000LL && Hoehe[0] < 3) return 3;
                    if ((Feld1 & 0x4410000000LL) == 0x4010000000LL && Hoehe[1] < 2) return 3;
                    if ((Feld1 & 0x4220000000LL) == 0x4200000000LL && Hoehe[2] < 1) return 3;
                    if ((y & 0x20204) == 0x204 && Hoehe[4] < 1) return 3;
                    if ((y & 0x10404) == 0x10004 && Hoehe[5] < 2) return 3;
                    if ((y & 0x10208) == 0x10200 && Hoehe[6] < 3) return 3;
                    break;
                case 1:
                    if ((Feld1 & 0x4108000000LL) == 0x108000000LL && Hoehe[0] < 4) return 3;
                    if ((Feld1 & 0x2208000000LL) == 0x2008000000LL && Hoehe[1] < 3) return 3;
                    if ((Feld1 & 0x2110000000LL) == 0x2100000000LL && Hoehe[2] < 2) return 3;
                    if ((y & 0x10102) == 0x102 && Hoehe[4] < 2) return 3;
                    if ((y & 0x8202) == 0x8002 && Hoehe[5] < 3) return 3;
                    if ((y & 0x8104) == 0x8100 && Hoehe[6] < 4) return 3;
                    if ((Feld1 & 0x208020000LL) == 0x8020000LL && Hoehe[1] < 3) return 3;
                    if ((Feld1 & 0x110020000LL) == 0x100020000LL && Hoehe[2] < 2) return 3;
                    if ((y & 0x20010100) == 0x20000100 && Hoehe[4] < 2) return 3;
                    if ((y & 0x20008200) == 0x20008000 && Hoehe[5] < 3) return 3;
                    if ((Feld1 & 0x410000000LL) == 0x410000000LL && (Hoehe[0] < 1 || Hoehe[4] < 1)) return 3;
                    if ((Feld1 & 0x10810000000LL) == 0x10010000000LL && Hoehe[1] < 1) return 3;
                    if ((Feld1 & 0x10420000000LL) == 0x10400000000LL && Hoehe[2] < 1) return 3;
                    if ((Feld1 & 0x10010000LL) == 0x10010000LL && (Hoehe[1] < 1 || Hoehe[5] < 1)) return 3;
                    if ((Feld1 & 0x420010000LL) == 0x400010000LL && Hoehe[2] < 1) return 3;
                    if ((y & 0x10400) == 0x10400 && (Hoehe[2] < 1 || Hoehe[6] < 1)) return 3;
                    if ((y & 0x10020400) == 0x10000400 && Hoehe[4] < 1) return 3;
                    if ((y & 0x20410) == 0x410 && Hoehe[4] < 1) return 3;
                    if ((y & 0x10810) == 0x10010 && Hoehe[5] < 1) return 3;
                    break;
                case 2:
                    if ((Feld1 & 0x84000000LL) == 0x84000000LL && (Hoehe[0] < 5 || Hoehe[4] < 1)) return 3;
                    if ((Feld1 & 0x1104000000LL) == 0x1004000000LL && Hoehe[1] < 4) return 3;
                    if ((Feld1 & 0x1088000000LL) == 0x1080000000LL && Hoehe[2] < 3) return 3;
                    if ((y & 0x8081) == 0x81 && Hoehe[4] < 3) return 3;
                    if ((y & 0x4101) == 0x4001 && Hoehe[5] < 4) return 3;
                    if ((y & 0x4080) == 0x4080 && (Hoehe[2] < 1 || Hoehe[6] < 5)) return 3;
                    if ((Feld1 & 0x104010000LL) == 0x4010000LL && Hoehe[1] < 4) return 3;
                    if ((y & 0x88010000) == 0x80010000 && Hoehe[2] < 3) return 3;
                    if ((y & 0x10008080) == 0x10000080 && Hoehe[4] < 3) return 3;
                    if ((y & 0x10004100) == 0x10004000 && Hoehe[5] < 4) return 3;
                    if ((Feld1 & 0x208000000LL) == 0x208000000LL && (Hoehe[0] < 2 || Hoehe[4] < 2)) return 3;
                    if ((Feld1 & 0x8408000000LL) == 0x8008000000LL && Hoehe[1] < 2) return 3;
                    if ((Feld1 & 0x8210000000LL) == 0x8200000000LL && Hoehe[2] < 2) return 3;
                    if ((Feld1 & 0x8008000LL) == 0x8008000LL && (Hoehe[1] < 2 || Hoehe[5] < 2)) return 3;
                    if ((Feld1 & 0x210008000LL) == 0x200008000LL && Hoehe[2] < 2) return 3;
                    if ((y & 0x8200) == 0x8200 && (Hoehe[2] < 2 || Hoehe[6] < 2)) return 3;
                    if ((y & 0x8010200) == 0x8000200 && Hoehe[4] < 2) return 3;
                    if ((y & 0x10208) == 0x208 && Hoehe[4] < 2) return 3;
                    if ((y & 0x8408) == 0x8008 && Hoehe[5] < 2) return 3;
                    if ((y & 0x8010800) == 0x10800 && Hoehe[2] < 3) return 3;
                    if ((y & 0x4020800) == 0x4000800 && Hoehe[4] < 1) return 3;
                    if ((Feld1 & 0x820004000LL) == 0x800004000LL && Hoehe[2] < 1) return 3;
                    if ((Feld1 & 0x810008000LL) == 0x810000000LL && Hoehe[4] < 3) return 3;
                    break;
                case 3:
                    if ((y & 0x2008000) == 0x2008000 && (Hoehe[1] < 5 || Hoehe[5] < 1)) return 3;
                    if ((y & 0x44008000) == 0x40008000 && Hoehe[2] < 4) return 3;
                    if ((y & 0x42010000) == 0x42000000 && Hoehe[4] < 2) return 3;
                    if ((y & 0x10002040) == 0x2040 && Hoehe[2] < 2) return 3;
                    if ((y & 0x8004040) == 0x8000040 && Hoehe[4] < 4) return 3;
                    if ((Feld1 & 0x8002000LL) == 0x8002000LL && (Hoehe[1] < 1 || Hoehe[5] < 5)) return 3;
                    if ((Feld1 & 0x104000000LL) == 0x104000000LL && (Hoehe[0] < 3 || Hoehe[4] < 3)) return 3;
                    if ((Feld1 & 0x4204000000LL) == 0x4004000000LL && Hoehe[1] < 3) return 3;
                    if ((Feld1 & 0x4108000000LL) == 0x4100000000LL && Hoehe[2] < 3) return 3;
                    if ((Feld1 & 0x4004000LL) == 0x4004000LL && (Hoehe[1] < 3 || Hoehe[5] < 3)) return 3;
                    if ((Feld1 & 0x108004000LL) == 0x100004000LL && Hoehe[2] < 3) return 3;
                    if ((y & 0x4100) == 0x4100 && (Hoehe[2] < 3 || Hoehe[6] < 3)) return 3;
                    if ((y & 0x4008100) == 0x4000100 && Hoehe[4] < 3) return 3;
                    if ((y & 0x8104) == 0x104 && Hoehe[4] < 3) return 3;
                    if ((y & 0x4204) == 0x4004 && Hoehe[5] < 3) return 3;
                    if ((y & 0x4008400) == 0x8400 && Hoehe[2] < 4) return 3;
                    if ((y & 0x2010400) == 0x2000400 && Hoehe[4] < 2) return 3;
                    if ((Feld1 & 0x410002000LL) == 0x400002000LL && Hoehe[2] < 2) return 3;
                    if ((Feld1 & 0x408004000LL) == 0x408000000LL && Hoehe[4] < 4) return 3;
                    if ((y & 0x10420) == 0x420 && Hoehe[4] < 2) return 3;
                    if ((y & 0x8820) == 0x8020 && Hoehe[5] < 1) return 3;
                    if ((Feld1 & 0x20808000000LL) == 0x20008000000LL && Hoehe[1] < 1) return 3;
                    if ((Feld1 & 0x20410000000LL) == 0x20400000000LL && Hoehe[2] < 2) return 3;
                    break;
                case 4:
                    if ((Feld1 & 0x82000000LL) == 0x82000000LL && (Hoehe[0] < 4 || Hoehe[4] < 4)) return 3;
                    if ((Feld1 & 0x2102000000LL) == 0x2002000000LL && Hoehe[1] < 4) return 3;
                    if ((Feld1 & 0x2084000000LL) == 0x2080000000LL && Hoehe[2] < 4) return 3;
                    if ((Feld1 & 0x2002000LL) == 0x2002000LL && (Hoehe[1] < 4 || Hoehe[5] < 4)) return 3;
                    if ((y & 0x84002000) == 0x80002000 && Hoehe[2] < 4) return 3;
                    if ((y & 0x2080) == 0x2080 && (Hoehe[2] < 4 || Hoehe[6] < 4)) return 3;
                    if ((y & 0x2004080) == 0x2000080 && Hoehe[4] < 4) return 3;
                    if ((y & 0x4082) == 0x82 && Hoehe[4] < 4) return 3;
                    if ((y & 0x2102) == 0x2002 && Hoehe[5] < 4) return 3;
                    if ((y & 0x4200) == 0x4200 && (Hoehe[2] < 5 || Hoehe[6] < 1)) return 3;
                    if ((y & 0x1008200) == 0x1000200 && Hoehe[4] < 3) return 3;
                    if ((y & 0x1004400) == 0x1004000 && Hoehe[5] < 2) return 3;
                    if ((Feld1 & 0x404001000LL) == 0x4001000LL && Hoehe[1] < 2) return 3;
                    if ((Feld1 & 0x208001000LL) == 0x200001000LL && Hoehe[2] < 3) return 3;
                    if ((Feld1 & 0x204000000LL) == 0x204000000LL && (Hoehe[0] < 1 || Hoehe[4] < 5)) return 3;
                    if ((y & 0x8210) == 0x210 && Hoehe[4] < 3) return 3;
                    if ((y & 0x4410) == 0x4010 && Hoehe[5] < 2) return 3;
                    if ((Feld1 & 0x10404000000LL) == 0x10004000000LL && Hoehe[1] < 2) return 3;
                    if ((Feld1 & 0x10208000000LL) == 0x10200000000LL && Hoehe[2] < 3) return 3;
                    break;
                case 5:
                    if ((Feld1 & 0x41000000LL) == 0x41000000LL && (Hoehe[0] < 5 || Hoehe[4] < 5)) return 3;
                    if ((Feld1 & 0x1081000000LL) == 0x1001000000LL && Hoehe[1] < 5) return 3;
                    if ((Feld1 & 0x1042000000LL) == 0x1040000000LL && Hoehe[2] < 5) return 3;
                    if ((y & 0x1001000) == 0x1001000 && (Hoehe[1] < 5 || Hoehe[5] < 5)) return 3;
                    if ((y & 0x42001000) == 0x40001000 && Hoehe[2] < 5) return 3;
                    if ((y & 0x1040) == 0x1040 && (Hoehe[2] < 5 || Hoehe[6] < 5)) return 3;
                    if ((y & 0x1002040) == 0x1000040 && Hoehe[4] < 5) return 3;
                    if ((y & 0x2041) == 0x41 && Hoehe[4] < 5) return 3;
                    if ((y & 0x1081) == 0x1001 && Hoehe[5] < 5) return 3;
                    if ((y & 0x4108) == 0x108 && Hoehe[4] < 4) return 3;
                    if ((y & 0x2208) == 0x2008 && Hoehe[5] < 3) return 3;
                    if ((y & 0x2110) == 0x2100 && Hoehe[6] < 2) return 3;
                    if ((Feld1 & 0x10102000000LL) == 0x102000000LL && Hoehe[0] < 2) return 3;
                    if ((Feld1 & 0x8202000000LL) == 0x8002000000LL && Hoehe[1] < 3) return 3;
                    if ((Feld1 & 0x8104000000LL) == 0x8100000000LL && Hoehe[2] < 4) return 3;
                    break;
                default:
                    break;
            }
        case 4:
            switch (Hoehe[4]) {
                case 0:
                    if ((Feld1 & 0x208400000LL) == 0x8400000LL && Hoehe[1] < 3) return 4;
                    if ((Feld1 & 0x110400000LL) == 0x100400000LL && Hoehe[2] < 2) return 4;
                    if ((Feld1 & 0x108800000LL) == 0x108000000LL && Hoehe[3] < 1) return 4;
                    break;
                case 1:
                    if ((Feld1 & 0x104200000LL) == 0x4200000LL && Hoehe[1] < 4) return 4;
                    if ((y & 0x88200000) == 0x80200000 && Hoehe[2] < 3) return 4;
                    if ((y & 0x84400000) == 0x84000000 && Hoehe[3] < 2) return 4;
                    if ((y & 0x8200800) == 0x200800 && Hoehe[2] < 3) return 4;
                    if ((y & 0x4400800) == 0x4000800 && Hoehe[3] < 2) return 4;
                    if ((y & 0x800404) == 0x800004 && Hoehe[5] < 2) return 4;
                    if ((y & 0x800208) == 0x800200 && Hoehe[6] < 3) return 4;
                    if ((Feld1 & 0x10400000LL) == 0x10400000LL && (Hoehe[1] < 1 || Hoehe[5] < 1)) return 4;
                    if ((Feld1 & 0x420400000LL) == 0x400400000LL && Hoehe[2] < 1) return 4;
                    if ((Feld1 & 0x410800000LL) == 0x410000000LL && Hoehe[3] < 1) return 4;
                    if ((y & 0x400400) == 0x400400 && (Hoehe[2] < 1 || Hoehe[6] < 1)) return 4;
                    if ((y & 0x10800400) == 0x10000400 && Hoehe[3] < 1) return 4;
                    if ((y & 0x800410) == 0x410 && Hoehe[3] < 1) return 4;
                    if ((y & 0x400810) == 0x400010 && Hoehe[5] < 1) return 4;
                    break;
                case 2:
                    if ((y & 0x2100000) == 0x2100000 && (Hoehe[1] < 5 || Hoehe[5] < 1)) return 4;
                    if ((y & 0x44100000) == 0x40100000 && Hoehe[2] < 4) return 4;
                    if ((y & 0x42200000) == 0x42000000 && Hoehe[3] < 3) return 4;
                    if ((y & 0x4100400) == 0x100400 && Hoehe[2] < 4) return 4;
                    if ((y & 0x2200400) == 0x2000400 && Hoehe[3] < 3) return 4;
                    if ((y & 0x800102) == 0x102 && Hoehe[3] < 1) return 4;
                    if ((y & 0x400202) == 0x400002 && Hoehe[5] < 3) return 4;
                    if ((y & 0x400104) == 0x400100 && Hoehe[6] < 4) return 4;
                    if ((Feld1 & 0x8200000LL) == 0x8200000LL && (Hoehe[1] < 2 || Hoehe[5] < 2)) return 4;
                    if ((Feld1 & 0x210200000LL) == 0x200200000LL && Hoehe[2] < 2) return 4;
                    if ((Feld1 & 0x208400000LL) == 0x208000000LL && Hoehe[3] < 2) return 4;
                    if ((y & 0x200200) == 0x200200 && (Hoehe[2] < 2 || Hoehe[6] < 2)) return 4;
                    if ((y & 0x8400200) == 0x8000200 && Hoehe[3] < 2) return 4;
                    if ((y & 0x400208) == 0x208 && Hoehe[3] < 2) return 4;
                    if ((y & 0x200408) == 0x200008 && Hoehe[5] < 2) return 4;
                    if ((y & 0x200420) == 0x420 && Hoehe[3] < 3) return 4;
                    if ((y & 0x100820) == 0x100020 && Hoehe[5] < 1) return 4;
                    if ((y & 0x20800100) == 0x20000100 && Hoehe[3] < 1) return 4;
                    if ((y & 0x20400200) == 0x20400000 && Hoehe[5] < 3) return 4;
                    break;
                case 3:
                    if ((y & 0x80200) == 0x80200 && (Hoehe[2] < 5 || Hoehe[6] < 1)) return 4;
                    if ((y & 0x1100200) == 0x1000200 && Hoehe[3] < 4) return 4;
                    if ((y & 0x1080400) == 0x1080000 && Hoehe[5] < 2) return 4;
                    if ((y & 0x400081) == 0x81 && Hoehe[3] < 2) return 4;
                    if ((y & 0x200101) == 0x200001 && Hoehe[5] < 4) return 4;
                    if ((y & 0x200080) == 0x200080 && (Hoehe[2] < 1 || Hoehe[6] < 5)) return 4;
                    if ((Feld1 & 0x204100200LL) == 0x4100000LL && (Hoehe[1] < 3 || Hoehe[5] < 3)) return 4;
                    if ((Feld1 & 0x108100000LL) == 0x100100000LL && Hoehe[2] < 3) return 4;
                    if ((Feld1 & 0x104200000LL) == 0x104000000LL && Hoehe[3] < 3) return 4;
                    if ((y & 0x100100) == 0x100100 && (Hoehe[2] < 3 || Hoehe[6] < 3)) return 4;
                    if ((y & 0x4200100) == 0x4000100 && Hoehe[3] < 3) return 4;
                    if ((y & 0x200104) == 0x104 && Hoehe[3] < 3) return 4;
                    if ((y & 0x100204) == 0x100004 && Hoehe[5] < 3) return 4;
                    if ((y & 0x100210) == 0x210 && Hoehe[3] < 4) return 4;
                    if ((y & 0x80410) == 0x80010 && Hoehe[5] < 2) return 4;
                    if ((y & 0x10400080) == 0x10000080 && Hoehe[3] < 2) return 4;
                    if ((y & 0x10200100) == 0x10200000 && Hoehe[5] < 4) return 4;
                    if ((Feld1 & 0x820200000LL) == 0x800200000LL && Hoehe[2] < 1) return 4;
                    if ((Feld1 & 0x810400000LL) == 0x810000000LL && Hoehe[3] < 2) return 4;
                    break;
                case 4:
                    if ((Feld1 & 0x2080000LL) == 0x2080000LL && (Hoehe[1] < 4 || Hoehe[5] < 4)) return 4;
                    if ((y & 0x84080000) == 0x80080000 && Hoehe[2] < 4) return 4;
                    if ((y & 0x82100000) == 0x82000000 && Hoehe[3] < 4) return 4;
                    if ((y & 0x80080) == 0x80080 && (Hoehe[2] < 4 || Hoehe[6] < 4)) return 4;
                    if ((y & 0x2100080) == 0x2000080 && Hoehe[3] < 4) return 4;
                    if ((y & 0x100082) == 0x82 && Hoehe[3] < 4) return 4;
                    if ((y & 0x80102) == 0x80002 && Hoehe[5] < 4) return 4;
                    if ((y & 0x80108) == 0x108 && Hoehe[3] < 5) return 4;
                    if ((y & 0x40208) == 0x40008 && Hoehe[5] < 3) return 4;
                    if ((y & 0x40110) == 0x40100 && Hoehe[6] < 2) return 4;
                    if ((y & 0x10100040) == 0x100040 && Hoehe[2] < 2) return 4;
                    if ((y & 0x8200040) == 0x8000040 && Hoehe[3] < 3) return 4;
                    if ((Feld1 & 0x8100000LL) == 0x8100000LL && (Hoehe[1] < 1 || Hoehe[5] < 5)) return 4;
                    if ((Feld1 & 0x410100000LL) == 0x400100000LL && Hoehe[2] < 2) return 4;
                    if ((Feld1 & 0x408200000LL) == 0x408000000LL && Hoehe[3] < 3) return 4;
                    break;
                case 5:
                    if ((y & 0x1040000) == 0x1040000 && (Hoehe[1] < 5 || Hoehe[5] < 5)) return 4;
                    if ((y & 0x42040000) == 0x40040000 && Hoehe[2] < 5) return 4;
                    if ((y & 0x41080000) == 0x41000000 && Hoehe[3] < 5) return 4;
                    if ((y & 0x40040) == 0x40040 && (Hoehe[2] < 5 || Hoehe[6] < 5)) return 4;
                    if ((y & 0x1080040) == 0x1000040 && Hoehe[3] < 5) return 4;
                    if ((y & 0x80041) == 0x41 && Hoehe[3] < 5) return 4;
                    if ((y & 0x40081) == 0x40001 && Hoehe[5] < 5) return 4;
                    if ((Feld1 & 0x404080000LL) == 0x4080000LL && Hoehe[1] < 2) return 4;
                    if ((Feld1 & 0x208080000LL) == 0x200080000LL && Hoehe[2] < 3) return 4;
                    if ((Feld1 & 0x204100000LL) == 0x204000000LL && Hoehe[3] < 4) return 4;
                    break;
                default:
                    break;
            }
        case 5:
            switch (Hoehe[5]) {
                case 0:
                    if ((y & 0x8210000) == 0x210000 && Hoehe[2] < 3) return 5;
                    if ((y & 0x4410000) == 0x4010000 && Hoehe[3] < 2) return 5;
                    if ((y & 0x4220000) == 0x4200000 && Hoehe[4] < 1) return 5;
                    break;
                case 1:
                    if ((y & 0x4108000) == 0x108000 && Hoehe[2] < 4) return 5;
                    if ((y & 0x2208000) == 0x2008000 && Hoehe[3] < 3) return 5;
                    if ((y & 0x2110000) == 0x2100000 && Hoehe[4] < 2) return 5;
                    if ((y & 0x208020) == 0x8020 && Hoehe[3] < 3) return 5;
                    if ((y & 0x110020) == 0x100020 && Hoehe[4] < 2) return 5;
                    if ((y & 0x410000) == 0x410000 && (Hoehe[2] < 1 || Hoehe[6] < 1)) return 5;
                    if ((y & 0x10810000) == 0x10010000 && Hoehe[3] < 1) return 5;
                    if ((y & 0x10420000) == 0x10400000 && Hoehe[4] < 1) return 5;
                    if ((y & 0x810010) == 0x10010 && Hoehe[3] < 1) return 5;
                    if ((y & 0x420010) == 0x400010 && Hoehe[4] < 1) return 5;
                    break;
                case 2:
                    if ((y & 0x84000) == 0x84000 && (Hoehe[2] < 5 || Hoehe[6] < 1)) return 5;
                    if ((y & 0x1104000) == 0x1004000 && Hoehe[3] < 4) return 5;
                    if ((y & 0x1088000) == 0x1080000 && Hoehe[4] < 3) return 5;
                    if ((y & 0x104010) == 0x4010 && Hoehe[3] < 4) return 5;
                    if ((y & 0x88010) == 0x80010 && Hoehe[4] < 3) return 5;
                    if ((y & 0x208000) == 0x208000 && (Hoehe[2] < 2 || Hoehe[6] < 2)) return 5;
                    if ((y & 0x8408000) == 0x8008000 && Hoehe[3] < 2) return 5;
                    if ((y & 0x8210000) == 0x8200000 && Hoehe[4] < 2) return 5;
                    if ((y & 0x408008) == 0x8008 && Hoehe[3] < 2) return 5;
                    if ((y & 0x210008) == 0x200008 && Hoehe[4] < 2) return 5;
                    if ((y & 0x820004) == 0x800004 && Hoehe[4] < 1) return 5;
                    if ((y & 0x810008) == 0x810000 && Hoehe[6] < 3) return 5;
                    break;
                case 3:
                    if ((y & 0x82008) == 0x2008 && Hoehe[3] < 5) return 5;
                    if ((y & 0x44008) == 0x40008 && Hoehe[4] < 4) return 5;
                    if ((y & 0x42010) == 0x42000 && Hoehe[6] < 2) return 5;
                    if ((y & 0x104000) == 0x104000 && (Hoehe[2] < 3 || Hoehe[6] < 3)) return 5;
                    if ((y & 0x4204000) == 0x4004000 && Hoehe[3] < 3) return 5;
                    if ((y & 0x4108000) == 0x4100000 && Hoehe[4] < 3) return 5;
                    if ((y & 0x204004) == 0x4004 && Hoehe[3] < 3) return 5;
                    if ((y & 0x108004) == 0x100004 && Hoehe[4] < 3) return 5;
                    if ((y & 0x808002) == 0x8002 && Hoehe[3] < 1) return 5;
                    if ((y & 0x410002) == 0x400002 && Hoehe[4] < 2) return 5;
                    if ((y & 0x408004) == 0x408000 && Hoehe[6] < 4) return 5;
                    if ((y & 0x20808000) == 0x20008000 && Hoehe[3] < 1) return 5;
                    if ((y & 0x20410000) == 0x20400000 && Hoehe[4] < 2) return 5;
                    break;
                case 4:
                    if ((y & 0x82000) == 0x82000 && (Hoehe[2] < 4 || Hoehe[6] < 4)) return 5;
                    if ((y & 0x2102000) == 0x2002000 && Hoehe[3] < 4) return 5;
                    if ((y & 0x2084000) == 0x2080000 && Hoehe[4] < 4) return 5;
                    if ((y & 0x102002) == 0x2002 && Hoehe[3] < 4) return 5;
                    if ((y & 0x84002) == 0x80002 && Hoehe[4] < 4) return 5;
                    if ((y & 0x404001) == 0x4001 && Hoehe[3] < 2) return 5;
                    if ((y & 0x208001) == 0x200001 && Hoehe[4] < 3) return 5;
                    if ((y & 0x204000) == 0x204000 && (Hoehe[2] < 1 || Hoehe[6] < 5)) return 5;
                    if ((y & 0x10404000) == 0x10004000 && Hoehe[3] < 2) return 5;
                    if ((y & 0x10208000) == 0x10200000 && Hoehe[4] < 3) return 5;
                    break;
                case 5:
                    if ((y & 0x41000) == 0x41000 && (Hoehe[2] < 5 || Hoehe[6] < 5)) return 5;
                    if ((y & 0x1081000) == 0x1001000 && Hoehe[3] < 5) return 5;
                    if ((y & 0x1042000) == 0x1040000 && Hoehe[4] < 5) return 5;
                    if ((y & 0x81001) == 0x1001 && Hoehe[3] < 5) return 5;
                    if ((y & 0x42001) == 0x40001 && Hoehe[4] < 5) return 5;
                    if ((y & 0x10102000) == 0x102000 && Hoehe[2] < 2) return 5;
                    if ((y & 0x8202000) == 0x8002000 && Hoehe[3] < 3) return 5;
                    if ((y & 0x8104000) == 0x8100000 && Hoehe[4] < 4) return 5;
                    break;
                default:
                    break;
            }
        case 6:
            switch (Hoehe[6]) {
                case 0:
                    if ((y & 0x208400) == 0x8400 && Hoehe[3] < 3) return 6;
                    if ((y & 0x110400) == 0x100400 && Hoehe[4] < 2) return 6;
                    if ((y & 0x108800) == 0x108000 && Hoehe[5] < 1) return 6;
                    break;
                case 1:
                    if ((y & 0x104200) == 0x4200 && Hoehe[3] < 4) return 6;
                    if ((y & 0x88200) == 0x80200 && Hoehe[4] < 3) return 6;
                    if ((y & 0x84400) == 0x84000 && Hoehe[5] < 2) return 6;
                    if ((y & 0x810400) == 0x10400 && Hoehe[3] < 1) return 6;
                    if ((y & 0x420400) == 0x400400 && Hoehe[4] < 1) return 6;
                    if ((y & 0x410800) == 0x410000 & Hoehe[5] < 1) return 6;
                    break;
                case 2:
                    if ((y & 0x82100) == 0x2100 && Hoehe[3] < 5) return 6;
                    if ((y & 0x44100) == 0x40100 && Hoehe[4] < 4) return 6;
                    if ((y & 0x42200) == 0x42000 && Hoehe[5] < 3) return 6;
                    if ((y & 0x408200) == 0x8200 && Hoehe[3] < 2) return 6;
                    if ((y & 0x210200) == 0x200200 && Hoehe[4] < 2) return 6;
                    if ((y & 0x208400) == 0x208000 && Hoehe[5] < 2) return 6;
                    break;
                case 3:
                    if ((y & 0x204100) == 0x4100 && Hoehe[3] < 3) return 6;
                    if ((y & 0x108100) == 0x100100 && Hoehe[4] < 3) return 6;
                    if ((y & 0x104200) == 0x104000 && Hoehe[5] < 3) return 6;
                    if ((y & 0x820200) == 0x800200 && Hoehe[4] < 1) return 6;
                    if ((y & 0x810400) == 0x810000 && Hoehe[5] < 2) return 6;
                    break;
                case 4:
                    if ((y & 0x102080) == 0x2080 && Hoehe[3] < 4) return 6;
                    if ((y & 0x84080) == 0x80080 && Hoehe[4] < 4) return 6;
                    if ((y & 0x82100) == 0x82000 && Hoehe[5] < 4) return 6;
                    if ((y & 0x808100) == 0x8100 && Hoehe[3] < 1) return 6;
                    if ((y & 0x410100) == 0x400100 && Hoehe[4] < 2) return 6;
                    if ((y & 0x408200) == 0x408000 && Hoehe[5] < 3) return 6;
                    break;
                case 5:
                    if ((y & 0x81040) == 0x1040 && Hoehe[3] < 5) return 6;
                    if ((y & 0x42040) == 0x40040 && Hoehe[4] < 5) return 6;
                    if ((y & 0x41080) == 0x41000 && Hoehe[5] < 5) return 6;
                    if ((y & 0x404080) == 0x4080 && Hoehe[3] < 2) return 6;
                    if ((y & 0x208080) == 0x200080 && Hoehe[4] < 3) return 6;
                    if ((y & 0x204100) == 0x204000 && Hoehe[5] < 4) return 6;
                    break;
                default:
                    break;
            }
    }
    return (-1);
}

short int FindOtherOddThreat2(const short x) {
    switch (x + 1) {
        case 1:
            switch (Hoehe[1]) {
                case 0:
                    if ((Feld2 & 0x10004000LL) == 0x10004000LL && Hoehe[3] < 2) return 1;
                    break;
                case 1:
                    if ((Feld2 & 0x102000LL) == 0x102000LL && Hoehe[2] < 2) return 1;
                    if ((Feld2 & 0x8100000LL) == 0x8100000LL && Hoehe[4] < 4) return 1;
                    if ((Feld2 & 0x20000100000LL) == 0x20000100000LL && Hoehe[2] < 2) return 1;
                    break;
                case 2:
                    if ((Feld2 & 0x4001000LL) == 0x4001000LL && Hoehe[3] < 4) return 1;
                    if ((Feld2 & 0x10004000000LL) == 0x10004000000LL && Hoehe[3] < 4) return 1;
                    if ((Feld2 & 0x8200000LL) == 0x8200000LL && (Hoehe[0] < 2 || Hoehe[4] < 2)) return 1;
                    if ((Feld2 & 0x8000200000LL) == 0x8000200000LL && Hoehe[2] < 2) return 1;
                    if ((Feld2 & 0x8008000000LL) == 0x8008000000LL && Hoehe[3] < 2) return 1;
                    if ((Feld2 & 0x208000LL) == 0x208000LL && Hoehe[2] < 2) return 1;
                    if ((Feld2 & 0x8008000LL) == 0x8008000LL && Hoehe[3] < 2) return 1;
                    break;
                case 3:
                    if ((Feld2 & 0x2040000LL) == 0x2040000LL && Hoehe[0] < 2) return 1;
                    if ((Feld2 & 0x8000040000LL) == 0x8000040000LL && Hoehe[2] < 4) return 1;
                    if ((Feld2 & 0x8400000LL) == 0x8400000LL && Hoehe[0] < 4) return 1;
                    if ((Feld2 & 0x2000400000LL) == 0x2000400000LL && Hoehe[2] < 2) return 1;
                    if ((Feld2 & 0x420000LL) == 0x420000LL && Hoehe[2] < 2) return 1;
                    break;
                case 4:
                    if ((Feld2 & 0x2080000LL) == 0x2080000LL && (Hoehe[0] < 4 || Hoehe[4] < 4)) return 1;
                    if ((Feld2 & 0x2000080000LL) == 0x2000080000LL && Hoehe[2] < 4) return 1;
                    if ((Feld2 & 0x2002000000LL) == 0x2002000000LL && Hoehe[3] < 4) return 1;
                    if ((Feld2 & 0x82000LL) == 0x82000LL && Hoehe[2] < 4) return 1;
                    if ((Feld2 & 0x2002000LL) == 0x2002000LL && Hoehe[3] < 4) return 1;
                    if ((Feld2 & 0x1004000000LL) == 0x1004000000LL && Hoehe[3] < 2) return 1;
                    if ((Feld2 & 0x4010000LL) == 0x4010000LL && Hoehe[3] < 2) return 1;
                    break;
                case 5:
                    if ((Feld2 & 0x108000LL) == 0x108000LL && Hoehe[2] < 4) return 1;
                    if ((Feld2 & 0x2100000LL) == 0x2100000LL && Hoehe[4] < 2) return 1;
                    break;
                default:
                    break;
            }
        case 2:
            switch (Hoehe[2]) {
                case 0:
                    if ((Feld2 & 0x400100LL) == 0x400100LL && Hoehe[4] < 2) return 2;
                    break;
                case 1:
                    if ((Feld2 & 0x4080LL) == 0x4080LL && Hoehe[3] < 2) return 2;
                    if ((Feld2 & 0x204000LL) == 0x204000LL && Hoehe[5] < 4) return 2;
                    if ((Feld2 & 0x4000800000LL) == 0x4000800000LL && Hoehe[1] < 2) return 2;
                    if ((Feld2 & 0x800004000LL) == 0x800004000LL && Hoehe[3] < 2) return 2;
                    break;
                case 2:
                    if ((Feld2 & 0x100040LL) == 0x100040LL && Hoehe[4] < 4) return 2;
                    if ((Feld2 & 0x100400000LL) == 0x100400000LL && Hoehe[0] < 4) return 2;
                    if ((Feld2 & 0x400100000LL) == 0x400100000LL && Hoehe[4] < 4) return 2;
                    if ((Feld2 & 0x200200000LL) == 0x200200000LL && (Hoehe[0] < 2 || Hoehe[4] < 2)) return 2;
                    if ((Feld2 & 0x8000200000LL) == 0x8000200000LL && Hoehe[1] < 2) return 2;
                    if ((Feld2 & 0x8200000000LL) == 0x8200000000LL && Hoehe[3] < 2) return 2;
                    if ((Feld2 & 0x208000LL) == 0x208000LL && (Hoehe[1] < 2 || Hoehe[5] < 2)) return 2;
                    if ((Feld2 & 0x200008000LL) == 0x200008000LL && Hoehe[3] < 2) return 2;
                    if ((Feld2 & 0x8200LL) == 0x8200LL && Hoehe[3] < 2) return 2;
                    if ((Feld2 & 0x200200LL) == 0x200200LL && Hoehe[4] < 2) return 2;
                    break;
                case 3:
                    if ((Feld2 & 0x1000200000LL) == 0x1000200000LL && Hoehe[1] < 4) return 2;
                    if ((Feld2 & 0x1080000000LL) == 0x1080000000LL && Hoehe[3] < 2) return 2;
                    if ((Feld2 & 0x81000LL) == 0x81000LL && Hoehe[1] < 2) return 2;
                    if ((Feld2 & 0x200001000LL) == 0x200001000LL && Hoehe[3] < 4) return 2;
                    if ((Feld2 & 0x210000LL) == 0x210000LL && Hoehe[1] < 4) return 2;
                    if ((Feld2 & 0x80010000LL) == 0x80010000LL && Hoehe[3] < 2) return 2;
                    if ((Feld2 & 0x10000080000LL) == 0x10000080000LL && Hoehe[1] < 2) return 2;
                    if ((Feld2 & 0x10200000000LL) == 0x10200000000LL && Hoehe[3] < 4) return 2;
                    if ((Feld2 & 0x10800LL) == 0x10800LL && Hoehe[3] < 2) return 2;
                    break;
                case 4:
                    if ((Feld2 & 0x80080000LL) == 0x80080000LL && (Hoehe[0] < 4 || Hoehe[4] < 4)) return 2;
                    if ((Feld2 & 0x2000080000LL) == 0x2000080000LL && Hoehe[1] < 4) return 2;
                    if ((Feld2 & 0x2080000000LL) == 0x2080000000LL && Hoehe[3] < 4) return 2;
                    if ((Feld2 & 0x82000LL) == 0x82000LL && (Hoehe[1] < 4 || Hoehe[5] < 4)) return 2;
                    if ((Feld2 & 0x80002000LL) == 0x80002000LL && Hoehe[3] < 4) return 2;
                    if ((Feld2 & 0x2080LL) == 0x2080LL && Hoehe[3] < 4) return 2;
                    if ((Feld2 & 0x80080LL) == 0x80080LL && Hoehe[4] < 4) return 2;
                    if ((Feld2 & 0x40100000LL) == 0x40100000LL && Hoehe[4] < 2) return 2;
                    if ((Feld2 & 0x100040000LL) == 0x100040000LL && Hoehe[0] < 2) return 2;
                    if ((Feld2 & 0x100400LL) == 0x100400LL && Hoehe[4] < 2) return 2;
                    break;
                case 5:
                    if ((Feld2 & 0x4200LL) == 0x4200LL && Hoehe[3] < 4) return 2;
                    if ((Feld2 & 0x84000LL) == 0x84000LL && Hoehe[5] < 2) return 2;
                    break;
                default:
                    break;
            }
        case 3:
            switch (Hoehe[3]) {
                case 0:
                    if ((Feld2 & 0x4010000000LL) == 0x4010000000LL && Hoehe[1] < 2) return 3;
                    if ((Feld2 & 0x10004LL) == 0x10004LL && Hoehe[5] < 2) return 3;
                    break;
                case 1:
                    if ((Feld2 & 0x108000000LL) == 0x108000000LL && Hoehe[0] < 4) return 3;
                    if ((Feld2 & 0x2100000000LL) == 0x2100000000LL && Hoehe[2] < 2) return 3;
                    if ((Feld2 & 0x102LL) == 0x102LL && Hoehe[4] < 2) return 3;
                    if ((Feld2 & 0x8100LL) == 0x8100LL && Hoehe[6] < 4) return 3;
                    if ((Feld2 & 0x100020000LL) == 0x100020000LL && Hoehe[2] < 2) return 3;
                    if ((Feld2 & 0x20000100LL) == 0x20000100LL && Hoehe[4] < 2) return 3;
                    break;
                case 2:
                    if ((Feld2 & 0x1004000000LL) == 0x1004000000LL && Hoehe[1] < 4) return 3;
                    if ((Feld2 & 0x4001LL) == 0x4001LL && Hoehe[5] < 4) return 3;
                    if ((Feld2 & 0x4010000LL) == 0x4010000LL && Hoehe[1] < 4) return 3;
                    if ((Feld2 & 0x10004000LL) == 0x10004000LL && Hoehe[5] < 4) return 3;
                    if ((Feld2 & 0x208000000LL) == 0x208000000LL && (Hoehe[0] < 2 || Hoehe[4] < 2)) return 3;
                    if ((Feld2 & 0x8008000000LL) == 0x8008000000LL && Hoehe[1] < 2) return 3;
                    if ((Feld2 & 0x8200000000LL) == 0x8200000000LL && Hoehe[2] < 2) return 3;
                    if ((Feld2 & 0x8008000LL) == 0x8008000LL && (Hoehe[1] < 2 || Hoehe[5] < 2)) return 3;
                    if ((Feld2 & 0x200008000LL) == 0x200008000LL && Hoehe[2] < 2) return 3;
                    if ((Feld2 & 0x8200LL) == 0x8200LL && (Hoehe[2] < 2 || Hoehe[6] < 2)) return 3;
                    if ((Feld2 & 0x8000200LL) == 0x8000200LL && Hoehe[4] < 2) return 3;
                    if ((Feld2 & 0x208LL) == 0x208LL && Hoehe[4] < 2) return 3;
                    if ((Feld2 & 0x8008LL) == 0x8008LL && Hoehe[5] < 2) return 3;
                    break;
                case 3:
                    if ((Feld2 & 0x40008000LL) == 0x40008000LL && Hoehe[2] < 4) return 3;
                    if ((Feld2 & 0x42000000LL) == 0x42000000LL && Hoehe[4] < 2) return 3;
                    if ((Feld2 & 0x2040LL) == 0x2040LL && Hoehe[2] < 2) return 3;
                    if ((Feld2 & 0x8000040LL) == 0x8000040LL && Hoehe[4] < 4) return 3;
                    if ((Feld2 & 0x8400LL) == 0x8400LL && Hoehe[2] < 4) return 3;
                    if ((Feld2 & 0x2000400LL) == 0x2000400LL && Hoehe[4] < 2) return 3;
                    if ((Feld2 & 0x400002000LL) == 0x400002000LL && Hoehe[2] < 2) return 3;
                    if ((Feld2 & 0x408000000LL) == 0x408000000LL && Hoehe[4] < 4) return 3;
                    if ((Feld2 & 0x420LL) == 0x420LL && Hoehe[4] < 2) return 3;
                    if ((Feld2 & 0x20400000000LL) == 0x20400000000LL && Hoehe[2] < 2) return 3;
                    break;
                case 4:
                    if ((Feld2 & 0x82000000LL) == 0x82000000LL && (Hoehe[0] < 4 || Hoehe[4] < 4)) return 3;
                    if ((Feld2 & 0x2002000000LL) == 0x2002000000LL && Hoehe[1] < 4) return 3;
                    if ((Feld2 & 0x2080000000LL) == 0x2080000000LL && Hoehe[2] < 4) return 3;
                    if ((Feld2 & 0x2002000LL) == 0x2002000LL && (Hoehe[1] < 4 || Hoehe[5] < 4)) return 3;
                    if ((Feld2 & 0x80002000LL) == 0x80002000LL && Hoehe[2] < 4) return 3;
                    if ((Feld2 & 0x2080LL) == 0x2080LL && (Hoehe[2] < 4 || Hoehe[6] < 4)) return 3;
                    if ((Feld2 & 0x2000080LL) == 0x2000080LL && Hoehe[4] < 4) return 3;
                    if ((Feld2 & 0x82LL) == 0x82LL && Hoehe[4] < 4) return 3;
                    if ((Feld2 & 0x2002LL) == 0x2002LL && Hoehe[5] < 4) return 3;
                    if ((Feld2 & 0x1004000LL) == 0x1004000LL && Hoehe[5] < 2) return 3;
                    if ((Feld2 & 0x4001000LL) == 0x4001000LL && Hoehe[1] < 2) return 3;
                    if ((Feld2 & 0x4010LL) == 0x4010LL && Hoehe[5] < 2) return 3;
                    if ((Feld2 & 0x10004000000LL) == 0x10004000000LL && Hoehe[1] < 2) return 3;
                    break;
                case 5:
                    if ((Feld2 & 0x108LL) == 0x108LL && Hoehe[4] < 4) return 3;
                    if ((Feld2 & 0x2100LL) == 0x2100LL && Hoehe[6] < 2) return 3;
                    if ((Feld2 & 0x102000000LL) == 0x102000000LL && Hoehe[0] < 2) return 3;
                    if ((Feld2 & 0x8100000000LL) == 0x8100000000LL && Hoehe[2] < 4) return 3;
                    break;
                default:
                    break;
            }
        case 4:
            switch (Hoehe[4]) {
                case 0:
                    if ((Feld2 & 0x100400000LL) == 0x100400000LL && Hoehe[2] < 2) return 4;
                    break;
                case 1:
                    if ((Feld2 & 0x4200000LL) == 0x4200000LL && Hoehe[1] < 4) return 4;
                    if ((Feld2 & 0x84000000LL) == 0x84000000LL && Hoehe[3] < 2) return 4;
                    if ((Feld2 & 0x4000800LL) == 0x4000800LL && Hoehe[3] < 2) return 4;
                    if ((Feld2 & 0x800004LL) == 0x800004LL && Hoehe[5] < 2) return 4;
                    break;
                case 2:
                    if ((Feld2 & 0x40100000LL) == 0x40100000LL && Hoehe[2] < 4) return 4;
                    if ((Feld2 & 0x100400LL) == 0x100400LL && Hoehe[2] < 4) return 4;
                    if ((Feld2 & 0x400100LL) == 0x400100LL && Hoehe[6] < 4) return 4;
                    if ((Feld2 & 0x8200000LL) == 0x8200000LL && (Hoehe[1] < 2 || Hoehe[5] < 2)) return 4;
                    if ((Feld2 & 0x200200000LL) == 0x200200000LL && Hoehe[2] < 2) return 4;
                    if ((Feld2 & 0x208000000LL) == 0x208000000LL && Hoehe[3] < 2) return 4;
                    if ((Feld2 & 0x200200LL) == 0x200200LL && (Hoehe[2] < 2 || Hoehe[6] < 2)) return 4;
                    if ((Feld2 & 0x8000200LL) == 0x8000200LL && Hoehe[3] < 2) return 4;
                    if ((Feld2 & 0x208LL) == 0x208LL && Hoehe[3] < 2) return 4;
                    if ((Feld2 & 0x200008LL) == 0x200008LL && Hoehe[5] < 2) return 4;
                    break;
                case 3:
                    if ((Feld2 & 0x1000200LL) == 0x1000200LL && Hoehe[3] < 4) return 4;
                    if ((Feld2 & 0x1080000LL) == 0x1080000LL && Hoehe[5] < 2) return 4;
                    if ((Feld2 & 0x81LL) == 0x81LL && Hoehe[3] < 2) return 4;
                    if ((Feld2 & 0x200001LL) == 0x200001LL && Hoehe[5] < 4) return 4;
                    if ((Feld2 & 0x210LL) == 0x210LL && Hoehe[3] < 4) return 4;
                    if ((Feld2 & 0x80010LL) == 0x80010LL && Hoehe[5] < 2) return 4;
                    if ((Feld2 & 0x10000080LL) == 0x10000080LL && Hoehe[3] < 2) return 4;
                    if ((Feld2 & 0x10200000LL) == 0x10200000LL && Hoehe[5] < 4) return 4;
                    if ((Feld2 & 0x810000000LL) == 0x810000000LL && Hoehe[3] < 2) return 4;
                    break;
                case 4:
                    if ((Feld2 & 0x2080000LL) == 0x2080000LL && (Hoehe[1] < 4 || Hoehe[5] < 4)) return 4;
                    if ((Feld2 & 0x80080000LL) == 0x80080000LL && Hoehe[2] < 4) return 4;
                    if ((Feld2 & 0x82000000LL) == 0x82000000LL && Hoehe[3] < 4) return 4;
                    if ((Feld2 & 0x80080LL) == 0x80080LL && (Hoehe[2] < 4 || Hoehe[6] < 4)) return 4;
                    if ((Feld2 & 0x2000080LL) == 0x2000080LL && Hoehe[3] < 4) return 4;
                    if ((Feld2 & 0x82LL) == 0x82LL && Hoehe[3] < 4) return 4;
                    if ((Feld2 & 0x80002LL) == 0x80002LL && Hoehe[5] < 4) return 4;
                    if ((Feld2 & 0x40100LL) == 0x40100LL && Hoehe[6] < 2) return 4;
                    if ((Feld2 & 0x100040LL) == 0x100040LL && Hoehe[2] < 2) return 4;
                    if ((Feld2 & 0x400100000LL) == 0x400100000LL && Hoehe[2] < 2) return 4;
                    break;
                case 5:
                    if ((Feld2 & 0x4080000LL) == 0x4080000LL && Hoehe[1] < 2) return 4;
                    if ((Feld2 & 0x204000000LL) == 0x204000000LL && Hoehe[3] < 4) return 4;
                    break;
                default:
                    break;
            }
        case 5:
            switch (Hoehe[5]) {
                case 0:
                    if ((Feld2 & 0x4010000LL) == 0x4010000LL && Hoehe[3] < 2) return 5;
                    break;
                case 1:
                    if ((Feld2 & 0x108000LL) == 0x108000LL && Hoehe[2] < 4) return 5;
                    if ((Feld2 & 0x2100000LL) == 0x2100000LL && Hoehe[4] < 2) return 5;
                    if ((Feld2 & 0x100020LL) == 0x100020LL && Hoehe[4] < 2) return 5;
                    break;
                case 2:
                    if ((Feld2 & 0x1004000LL) == 0x1004000LL && Hoehe[3] < 4) return 5;
                    if ((Feld2 & 0x4010LL) == 0x4010LL && Hoehe[3] < 4) return 5;
                    if ((Feld2 & 0x208000LL) == 0x208000LL && (Hoehe[2] < 2 || Hoehe[6] < 2)) return 5;
                    if ((Feld2 & 0x8008000LL) == 0x8008000LL && Hoehe[3] < 2) return 5;
                    if ((Feld2 & 0x8200000LL) == 0x8200000LL && Hoehe[4] < 2) return 5;
                    if ((Feld2 & 0x8008LL) == 0x8008LL && Hoehe[3] < 2) return 5;
                    if ((Feld2 & 0x200008LL) == 0x200008LL && Hoehe[4] < 2) return 5;
                    break;
                case 3:
                    if ((Feld2 & 0x40008LL) == 0x40008LL && Hoehe[4] < 4) return 5;
                    if ((Feld2 & 0x42000LL) == 0x42000LL && Hoehe[6] < 2) return 5;
                    if ((Feld2 & 0x400002LL) == 0x400002LL && Hoehe[4] < 2) return 5;
                    if ((Feld2 & 0x408000LL) == 0x408000LL && Hoehe[6] < 4) return 5;
                    if ((Feld2 & 0x20400000LL) == 0x20400000LL && Hoehe[4] < 2) return 5;
                    break;
                case 4:
                    if ((Feld2 & 0x82000LL) == 0x82000LL && (Hoehe[2] < 4 || Hoehe[6] < 4)) return 5;
                    if ((Feld2 & 0x2002000LL) == 0x2002000LL && Hoehe[3] < 4) return 5;
                    if ((Feld2 & 0x2080000LL) == 0x2080000LL && Hoehe[4] < 4) return 5;
                    if ((Feld2 & 0x2002LL) == 0x2002LL && Hoehe[3] < 4) return 5;
                    if ((Feld2 & 0x80002LL) == 0x80002LL && Hoehe[4] < 4) return 5;
                    if ((Feld2 & 0x4001LL) == 0x4001LL && Hoehe[3] < 2) return 5;
                    if ((Feld2 & 0x10004000LL) == 0x10004000LL && Hoehe[3] < 2) return 5;
                    break;
                case 5:
                    if ((Feld2 & 0x102000LL) == 0x102000LL && Hoehe[2] < 2) return 5;
                    if ((Feld2 & 0x8100000LL) == 0x8100000LL && Hoehe[4] < 4) return 5;
                    break;
                default:
                    break;
            }
        case 6:
            switch (Hoehe[6]) {
                case 0:
                    if ((Feld2 & 0x100400LL) == 0x100400LL && Hoehe[4] < 2) return 6;
                    break;
                case 1:
                    if ((Feld2 & 0x4200LL) == 0x4200LL && Hoehe[3] < 4) return 6;
                    if ((Feld2 & 0x84000LL) == 0x84000LL && Hoehe[5] < 2) return 6;
                    break;
                case 2:
                    if ((Feld2 & 0x40100LL) == 0x40100LL && Hoehe[4] < 4) return 6;
                    if ((Feld2 & 0x8200LL) == 0x8200LL && Hoehe[3] < 2) return 6;
                    if ((Feld2 & 0x200200LL) == 0x200200LL && Hoehe[4] < 2) return 6;
                    if ((Feld2 & 0x208000LL) == 0x208000LL && Hoehe[5] < 2) return 6;
                    break;
                case 3:
                    if ((Feld2 & 0x810000LL) == 0x810000LL && Hoehe[5] < 2) return 6;
                    break;
                case 4:
                    if ((Feld2 & 0x2080LL) == 0x2080LL && Hoehe[3] < 4) return 6;
                    if ((Feld2 & 0x80080LL) == 0x80080LL && Hoehe[4] < 4) return 6;
                    if ((Feld2 & 0x82000LL) == 0x82000LL && Hoehe[5] < 4) return 6;
                    if ((Feld2 & 0x400100LL) == 0x400100LL && Hoehe[4] < 2) return 6;
                    break;
                case 5:
                    if ((Feld2 & 0x4080LL) == 0x4080LL && Hoehe[3] < 2) return 6;
                    if ((Feld2 & 0x204000LL) == 0x204000LL && Hoehe[5] < 4) return 6;
                    break;
                default:
                    break;
            }
        default:
            break;
    }
    return (-1);
}


#include <string>

bool IsNumber(const std::string &text) {
    /*Methode, die überprüft, ob der übergebene std::string eine Integer-Zahl ist.#
    Wenn ja, dann wird ein true zurückgeben, wenn nicht dann ein false*/
    for (int i = 1; i <= text.size(); ++i)
        if (!isdigit(text[i])) return false;
    return true;
}

bool UnentschiedenUeberpruefen(void) {
    // Reihe 6 Horizontal
    if (!((Feld1 & 0x1041040000LL) && (Feld2 & 0x1041040000LL))) return false;
    if (!((Feld1 & 0x41041000LL) && (Feld2 & 0x41041000LL))) return false;
    if (!((Feld1 & 0x1041040LL) && (Feld2 & 0x1041040LL))) return false;
    if (!((Feld1 & 0x41041LL) && (Feld2 & 0x41041LL))) return false;

    // Reihe 5 Horizontal
    if (!((Feld1 & 0x2082080000LL) && (Feld2 & 0x2082080000LL))) return false;
    if (!((Feld1 & 0x82082000LL) && (Feld2 & 0x82082000LL))) return false;
    if (!((Feld1 & 0x2082080LL) && (Feld2 & 0x2082080LL))) return false;
    if (!((Feld1 & 0x82082LL) && (Feld2 & 0x82082LL))) return false;

    // Reihe 6 Diagonal 1
    if (!((Feld1 & 0x1084200000LL) && (Feld2 & 0x1084200000LL))) return false;
    if (!((Feld1 & 0x42108000LL) && (Feld2 & 0x42108000LL))) return false;
    if (!((Feld1 & 0x1084200LL) && (Feld2 & 0x1084200LL))) return false;
    if (!((Feld1 & 0x42108LL) && (Feld2 & 0x42108LL))) return false;


    // Reihe 3 Diagonal 2
    if (!((Feld1 & 0x8102040000LL) && (Feld2 & 0x8102040000LL))) return false;
    if (!((Feld1 & 0x204081000LL) && (Feld2 & 0x204081000LL))) return false;
    if (!((Feld1 & 0x8102040LL) && (Feld2 & 0x8102040LL))) return false;
    if (!((Feld1 & 0x204081LL) && (Feld2 & 0x204081LL))) return false;

    // Reihe 4 Horizontal
    if (!((Feld1 & 0x4104100000LL) && (Feld2 & 0x4104100000LL))) return false;
    if (!((Feld1 & 0x104104000LL) && (Feld2 & 0x104104000LL))) return false;
    if (!((Feld1 & 0x4104100LL) && (Feld2 & 0x4104100LL))) return false;
    if (!((Feld1 & 0x104104LL) && (Feld2 & 0x104104LL))) return false;

    // Reihe 5 Diagonal 1
    if (!((Feld1 & 0x2108400000LL) && (Feld2 & 0x2108400000LL))) return false;
    if (!((Feld1 & 0x84210000LL) && (Feld2 & 0x84210000LL))) return false;
    if (!((Feld1 & 0x2108400LL) && (Feld2 & 0x2108400LL))) return false;
    if (!((Feld1 & 0x84210LL) && (Feld2 & 0x84210LL))) return false;

    // Reihe 2 Diagonal 2
    if (!((Feld1 & 0x10204080000LL) && (Feld2 & 0x10204080000LL))) return false;
    if (!((Feld1 & 0x408102000LL) && (Feld2 & 0x408102000LL))) return false;
    if (!((Feld1 & 0x10204080LL) && (Feld2 & 0x10204080LL))) return false;
    if (!((Feld1 & 0x408102LL) && (Feld2 & 0x408102LL))) return false;


    // Reihe 3 Horizontal
    if (!((Feld1 & 0x8208200000LL) && (Feld2 & 0x8208200000LL))) return false;
    if (!((Feld1 & 0x208208000LL) && (Feld2 & 0x208208000LL))) return false;
    if (!((Feld1 & 0x8208200LL) && (Feld2 & 0x8208200LL))) return false;
    if (!((Feld1 & 0x208208LL) && (Feld2 & 0x208208LL))) return false;

    // Reihe 4 Diagonal 1
    if (!((Feld1 & 0x4210800000LL) && (Feld2 & 0x4210800000LL))) return false;
    if (!((Feld1 & 0x108420000LL) && (Feld2 & 0x108420000LL))) return false;
    if (!((Feld1 & 0x4210800LL) && (Feld2 & 0x4210800LL))) return false;
    if (!((Feld1 & 0x108420LL) && (Feld2 & 0x108420LL))) return false;

    // Reihe 1 Diagonal 2
    if (!((Feld1 & 0x20408100000LL) && (Feld2 & 0x20408100000LL))) return false;
    if (!((Feld1 & 0x810204000LL) && (Feld2 & 0x810204000LL))) return false;
    if (!((Feld1 & 0x20408100LL) && (Feld2 & 0x20408100LL))) return false;
    if (!((Feld1 & 0x810204LL) && (Feld2 & 0x810204LL))) return false;

    // Reihe 2 Horizontal
    if (!((Feld1 & 0x10410400000LL) && (Feld2 & 0x10410400000LL))) return false;
    if (!((Feld1 & 0x410410000LL) && (Feld2 & 0x410410000LL))) return false;
    if (!((Feld1 & 0x10410400LL) && (Feld2 & 0x10410400LL))) return false;
    if (!((Feld1 & 0x410410LL) && (Feld2 & 0x410410LL))) return false;

    // Reihe 1 Horizontal
    if (!((Feld1 & 0x20820800000LL) && (Feld2 & 0x20820800000LL))) return false;
    if (!((Feld1 & 0x820820000LL) && (Feld2 & 0x820820000LL))) return false;
    if (!((Feld1 & 0x20820800LL) && (Feld2 & 0x20820800LL))) return false;
    if (!((Feld1 & 0x820820LL) && (Feld2 & 0x820820LL))) return false;
    return true;
}

/*short SiegerErmittelnGenau2(const short *s) //Ist ein wenig langsamer als die ursprüngliche Funktion
{
	short Hs=Hoehe[s[0]]+1, anz1=0, anz2=0;
	__int64 dro1=0LL, dro2=0LL;

	while(Hs<6)
	{
		if(Gewinnstellung1(s[0], Hs))
		{
			dro1 |= Feldwert[s[0]][Hs];
			anz1++;
		}
		if((Hs & 1)==0)
			if(Gewinnstellung2(s[0], Hs))
			{
				dro2 |= Feldwert[s[0]][Hs];
				anz1++;
			}
			Hs++;
	}
	Hs=Hoehe[s[1]]+1;
	while(Hs<6)
	{
		if(Gewinnstellung1(s[1], Hs))
		{
			dro1 |= Feldwert[s[1]][Hs];
			anz2++;
		}
		if((Hs & 1)==0)
			if(Gewinnstellung2(s[1], Hs))
			{
				dro2 |= Feldwert[s[1]][Hs];
				anz2++;
			}
		if(anz2>1 && anz1>1) //Probieren ob weglassen dieser beiden Zeilen schneller ist...
				return (-1);
		Hs++;
	}
	//Prüfen ob Spieler2 eine Vertikale Drohung aufbauen kann und somit den Gegner in Zugzwang bringen kann,
	//wodurch möglicherweise auch eine gerade Drohung von Spieler2 zum Sieg führt
	if(Hoehe[s[0]]>1)
			if( (Feld2 & Feldwert[s[0]][Hoehe[s[0]]-1]) )  //Wenn oberster Stein von Spieler2 ist
			{
				if(anz1==0)
				{
					if( (Feld2 & Feldwert[s[0]][Hoehe[s[0]]-2]) ) //Wenn der Stein darunter auch von Spieler2 ist
						if(Hoehe[s[0]]<4 && Gewinnstellung2(s[0], Hoehe[s[0]]+2)) //Wenn Spieler2 einen Gewinn durch eine vertikale Drohung erzwingen kann
							return 1000;
				}
				else if( (Feld2 & Feldwert[s[0]][Hoehe[s[0]]-2]) )
					return (-1);
			}
	if(Hoehe[s[1]]>1)
			if( (Feld2 & Feldwert[s[1]][Hoehe[s[1]]-1]) )  //Wenn oberster Stein von Spieler2 ist
			{
				if(anz2==0)
				{
					if( (Feld2 & Feldwert[s[1]][Hoehe[s[1]]-2]) ) //Wenn der Stein darunter auch von Spieler2 ist
						if(Hoehe[s[1]]<4 && Gewinnstellung2(s[1], Hoehe[s[1]]+2)) //Wenn Spieler2 einen Gewinn durch eine vertikale Drohung erzwingen kann
							return 1000;
				}
				else if( (Feld2 & Feldwert[s[1]][Hoehe[s[1]]-2]) )
					return (-1);
			}
	if(anz1==0)
	{
		if(anz2==0)
			return 0;
		//Wenn nur die zweite Spalte Drohungen enthält..
		if( (dro1 & GERADEREIHEN) > (dro2 & UNGERADEREIHEN)) //Wenn eine  gerade Drohung von Spieler1 am untersten ist
			return -1000;
		if( (dro2 & UNGERADEREIHEN) > (dro1 & GERADEREIHEN)) //Wenn eine ungerade Drohung von Spieler2 am untersten ist
			return 1000;
		return 0;  //Wenn Spieler1 nur ungerade und Spieler2 nur gerade Drohungen hat
	}
	if(anz2==0)
	{

		if( (dro1 & GERADEREIHEN) > (dro2 & UNGERADEREIHEN)) //Wenn eine  gerade Drohung von Spieler1 am untersten ist
			return -1000;
		if( (dro2 & UNGERADEREIHEN) > (dro1 & GERADEREIHEN)) //Wenn eine ungerade Drohung von Spieler2 am untersten ist
			return 1000;
		return 0;  //Wenn Spieler1 nur ungerade und Spieler2 nur gerade Drohungen hat
	}
	/*if(dro1==0LL)   //Scheint länger zu dauern...
	{
		if(dro2 & UNGERADEREIHEN)
			return 1000;
	}
	if(dro2==0LL)
	{
		if(dro1 & GERADEREIHEN)
        	return -1000;
	}*/
/*if(anz1==1 && anz2==1) //Erkennt keine Doppeldrohungen...
{
    if(dro2 & UNGERADEREIHEN)
        if(dro1 & UNGERADEREIHEN)
            return 0;
        else
            return 1000;
    if(dro1 & GERADEREIHEN)
        return -1000;
}*/
/*return (-1);
} */

#include <cstdint>

short SpielBaum1Generate(const short instance, short alpha, short beta, const uint64_t ZKey, bool symM) {
    if (SpielBeenden1())
        return -1000;
    short moves[8];

    short x;
    uint64_t t;
    int HashPosition;

    //dynamische Zugsortierung
    gMoves1(moves);

    //Enhanced Transposition Cutoff
    for (x = 0; moves[x] != (-1); x++) {
        t = ZKey ^ zufall[0][moves[x] * 6 + Hoehe[moves[x]]];
        HASHEvorlage *hsh;

        //Hash-Table ist in zwei Stufen unterteilt, daher die Unterscheidung
        if (instance > 13) {
            HashPosition = int(t & (HASHSIZE - 1));
            hsh = &HASHE[HashPosition];
        } else {
            HashPosition = int(t & (LHASHSIZE - 1));
            hsh = &LHASHE[HashPosition];
        }
        if (hsh->key == t && hsh->flag != LOWER && hsh->value <= alpha)
            return hsh->value;
    }

    short y = 0, later = -1, z = -1, q = -1;
    int drlt, anz;

    if (symM) //War beim letzten Zug eine symmetrische Stellung möglich
        if ((symM = symMoeglich())) //ist immer noch eine möglich
            if (istSymmetrisch()) {  //liegt eine echte Symmetrie vor
                //Nur eine Seite des Spielfeldes muss ausprobiert werden
                short tmp[8];
                for (x = 0; moves[x] != -1; x++) {
                    if (moves[x] < 4)
                        tmp[y++] = moves[x];
                }
                for (x = 0; x < y; x++)
                    moves[x] = tmp[x];
                moves[x] = -1;
            }

    if (instance < 12 &&
        !SpielBeenden2()) //12 oder 14??? //Wenn Spieler 2 eine akute Drohung hat, dann lohnt es sich nicht, diese Stellung genauuer zu untersuchen
    {
        for (x = 0; moves[x] > -1; x++) {
            t = Feld1;
            Feld1 |= Feldwert[moves[x]][Hoehe[moves[x]]++];
            if (Gewinnstellung2(moves[x],
                                Hoehe[moves[x]])) //Wenn Spieler1 einen Zug macht und Spieler2 daraufhin gewinnt, dann abbrechen
            {
                Feld1 = t;
                Hoehe[moves[x]]--;
                continue;
            }

            anz = SpielBeendenStellungD(SPIELER1, &drlt);
            if (anz > 1) {
                Feld1 = t;
                Hoehe[moves[x]]--;
                return -1000;
            } else if (anz) {
                if (Hoehe[drlt] < 5 &&
                    Gewinnstellung1(drlt, Hoehe[drlt] + 1)) //Wenn Spieler1 aber doch noch eine Drohung hatte
                {
                    Feld1 = t;
                    Hoehe[moves[x]]--;
                    return -1000;
                }
                Hoehe[drlt]++;
                if (q == -1 && FindThreat1() != -1) //Kann eine Drohung erzwungen werden
                    q = moves[x];
                Hoehe[drlt]--;
            }
            Feld1 = t;
            Hoehe[moves[x]]--;
        }
    } else {
        //Nach Doppeldrohungen suchen, die direkt übereinander sind
        for (x = 0; moves[x] != -1; x++) {
            if (Hoehe[moves[x]] < 4) {
                Hoehe[moves[x]]++;
                if (Gewinnstellung1(moves[x], Hoehe[moves[x]]))
                    if (Gewinnstellung1(moves[x], Hoehe[moves[x]] + 1) && !SpielBeenden2()) {
                        Hoehe[moves[x]]--;
                        return -1000;
                    }
                Hoehe[moves[x]]--;
            }
        }
    }
    short rueck;
    y = FindThreat1();
    if (y > -1) {
        t = Feld1;
        Feld1 |= Feldwert[y][Hoehe[y]++];
        if (instance > 13)
            rueck = Spielbaum2ComputerAnziehender(instance + 1, alpha, beta, ZKey ^ zufall[0][y * 6 - 1 + Hoehe[y]]);
        else
            rueck = SpielBaum2Generate(instance + 1, alpha, beta, ZKey ^ zufall[0][y * 6 - 1 + Hoehe[y]], symM);

        Feld1 = t;
        Hoehe[y]--;
        if (rueck <= alpha)
            return rueck;
        else if (rueck == -1000) //Bei Sieg (eindeutig) abbrechen
            return -1000;
        else if (rueck < beta)
            beta = rueck;
        z = FindOtherThreat1(y);
        if (z > -1) {
            t = Feld1;
            Feld1 |= Feldwert[z][Hoehe[z]++];
            if (instance > 13)
                rueck = rueck = Spielbaum2ComputerAnziehender(instance + 1, alpha, beta,
                                                              ZKey ^ zufall[0][z * 6 - 1 + Hoehe[z]]);
            else
                rueck = SpielBaum2Generate(instance + 1, alpha, beta, ZKey ^ zufall[0][z * 6 - 1 + Hoehe[z]], symM);

            Feld1 = t;
            Hoehe[z]--;
            if (rueck <= alpha)
                return rueck;
            else if (rueck == -1000)
                return -1000;
            else if (rueck < beta)
                beta = rueck;
        }
    }
    if (q != -1) {
        if (Hoehe[q] < 5 && Hoehe[q] % 2 == 0 && later == -1 && Gewinnstellung1(q, Hoehe[q] + 1))
            later = q;
        else {
            t = Feld1;
            Feld1 |= Feldwert[q][Hoehe[q]++];
            rueck = SpielBaum2Generate(instance + 1, alpha, beta, ZKey ^ zufall[0][q * 6 - 1 + Hoehe[q]], symM);
            Feld1 = t;
            Hoehe[q]--;
            if (rueck <= alpha)
                return rueck;
            else if (rueck == -1000)
                return -1000;
            else if (rueck < beta)
                beta = rueck;
        }
    }

    for (x = 0; moves[x] > -1; x++) {
        if (y == moves[x] || z == moves[x] || q == moves[x])
            continue;
        if (Hoehe[moves[x]] < 5 && Hoehe[moves[x]] % 2 == 0 && later == -1 &&
            Gewinnstellung1(moves[x], Hoehe[moves[x]] + 1))
            later = moves[x];
        else {
            t = Feld1;
            Feld1 |= Feldwert[moves[x]][Hoehe[moves[x]]++];
            if (instance > 13)
                rueck = Spielbaum2ComputerAnziehender(instance + 1, alpha, beta,
                                                      ZKey ^ zufall[0][moves[x] * 6 - 1 + Hoehe[moves[x]]]);
            else
                rueck = SpielBaum2Generate(instance + 1, alpha, beta,
                                           ZKey ^ zufall[0][moves[x] * 6 - 1 + Hoehe[moves[x]]], symM);

            Feld1 = t;
            Hoehe[moves[x]]--;
            if (rueck <= alpha)
                return rueck;
            else if (rueck == -1000)
                return -1000;
            else if (rueck < beta)
                beta = rueck;
        }
    }
    if (later > -1) {
        t = Feld1;
        Feld1 |= Feldwert[later][Hoehe[later]++];
        if (instance > 13)
            rueck = rueck = Spielbaum2ComputerAnziehender(instance + 1, alpha, beta,
                                                          ZKey ^ zufall[0][later * 6 - 1 + Hoehe[later]]);
        else
            rueck = SpielBaum2Generate(instance + 1, alpha, beta, ZKey ^ zufall[0][later * 6 - 1 + Hoehe[later]], symM);

        Feld1 = t;
        Hoehe[later]--;
        if (rueck <= alpha)
            return rueck;
        else if (rueck == -1000)
            return -1000;
        else if (rueck < beta)
            beta = rueck;
    }
    return beta;
}

short SpielBaum2Generate(const short instance, short alpha, short beta, const uint64_t ZKey, bool symM) {
    short moves[8];
    if (SpielBeenden2())
        return 1000;
    if (instance == MaxInstance)
        return BewertungComputerAnziehender();
    if (instance == InstanceVoll)
        return 0;
    int HashPosition = int(ZKey & (LHASHSIZE - 1));
    HASHEvorlage *hsh = &LHASHE[HashPosition];
    if (hsh->key == ZKey) {
        switch (hsh->flag) {
            case EXACT:
                return hsh->value;
            case LOWER:
                if (hsh->value >= beta)
                    return hsh->value;
                if (hsh->value > alpha)
                    alpha = hsh->value;
                break;
            case UPPER:
                if (hsh->value <= alpha)
                    return hsh->value;
                if (hsh->value < beta)
                    beta = hsh->value;
                break;
        }
    }
    bool IstExact = false;

    short x, y = 0, later = -1, z = -1, q = -1;
    uint64_t t;

    int anz, drlt, j;

    //Stellung spiegeln
    uint64_t f1 = spiegel(Feld1);
    uint64_t f2 = spiegel(Feld2);

    //dynmische Zugsoriterung
    gMoves2(moves);

    //Wie oben...
    if (symM)
        if ((symM = (!(f1 & Feld2) && !(f2 & Feld1))))
            if ((f1 == Feld1 && f2 == Feld2)) {
                short tmp[8];
                for (x = 0; moves[x] != -1; x++) {
                    if (moves[x] < 4)
                        tmp[y++] = moves[x];
                }
                for (x = 0; x < y; x++)
                    moves[x] = tmp[x];
                moves[x] = -1;
            }

    //Versuchen gespiegelte Stellungen aus Hash-Table zu extrahieren
    //ETC??????
    if (instance < 16 && symM) {
        uint64_t NKey = toZobrist(f1, f2);
        int HashPositionN = int(NKey & (LHASHSIZE - 1));
        HASHEvorlage *hshN = &LHASHE[HashPositionN];
        if (hshN->key == NKey) {
            switch (hshN->flag) {
                case EXACT:
                    return hshN->value;
                case LOWER:
                    if (hshN->value >= beta)
                        return hshN->value;
                    if (hshN->value > alpha)
                        alpha = hshN->value;
                    break;
                case UPPER:
                    if (hshN->value <= alpha)
                        return hshN->value;
                    if (hshN->value < beta)
                        beta = hshN->value;
                    break;
            }
        }
    }

    if (instance < 14 &&
        !SpielBeenden1()) //12 oder 14??? //Wenn Spieler 1 eine akute Drohung hat, dann lohnt es sich nicht, diese Stellung genauuer zu untersuchen
    {
        for (x = 0; moves[x] > -1; x++) {
            t = Feld2;
            SteinSetzen(moves[x], SPIELER2);
            if (Gewinnstellung1(moves[x],
                                Hoehe[moves[x]])) //Wenn Spieler1 einen Zug macht und Spieler2 daraufhin gewinnt, dann abbrechen
            {
                Feld2 = t;
                Hoehe[moves[x]]--;
                continue;
            }
            anz = SpielBeendenStellungD(SPIELER2, &drlt); //Nach einer akuten Drohung für Spieler2 suchen
            if (anz > 1) {
                Feld2 = t;
                Hoehe[moves[x]]--;
                hsh->flag = EXACT;
                hsh->key = ZKey;
                hsh->value = 1000;
                return 1000;
            } else if (anz) {
                if (Hoehe[drlt] < 5 &&
                    Gewinnstellung2(drlt, Hoehe[drlt] + 1)) //Wenn Spieler2 aber doch noch eine Drohung hatte
                {
                    Feld2 = t;
                    Hoehe[moves[x]]--;
                    hsh->flag = EXACT;
                    hsh->key = ZKey;
                    hsh->value = 1000;
                    return 1000;
                }
                Hoehe[drlt]++;
                if (q == -1 && FindOddThreat2() != -1) //Kann eine Drohung erzwungen werden
                    q = moves[x];
                Hoehe[drlt]--;
            }
            Feld2 = t;
            Hoehe[moves[x]]--;
        }
    } else {
        for (x = 0; moves[x] != -1; x++) {
            if (Hoehe[moves[x]] < 4) {
                Hoehe[moves[x]]++;
                if (Gewinnstellung2(moves[x], Hoehe[moves[x]]))
                    if (Gewinnstellung2(moves[x], Hoehe[moves[x]] + 1) && !SpielBeenden1()) {
                        Hoehe[moves[x]]--;
                        return 1000;
                    }
                Hoehe[moves[x]]--;
            }
        }
    }

    short rueck;
    y = FindOddThreat2();
    if (y > -1) {
        z = FindOtherOddThreat2(y);
        if (z != -1 && Hoehe[z] < Hoehe[y]) {
            x = z;
            z = y;
            y = x;
        }
        t = Feld2;
        Feld2 |= Feldwert[y][Hoehe[y]++];
        rueck = SpielBaum1Generate(instance + 1, alpha, beta, ZKey ^ zufall[1][y * 6 - 1 + Hoehe[y]], symM);

        Feld2 = t;
        Hoehe[y]--;
        if (rueck >= beta) {
            hsh->key = ZKey;
            hsh->value = rueck;
            hsh->flag = LOWER;
            return rueck;
        } else if (rueck > alpha) {
            alpha = rueck;
            IstExact = true;
        }
        if (rueck == 1000) {
            hsh->key = ZKey;
            hsh->value = rueck;
            hsh->flag = (IstExact ? EXACT : UPPER);
            return rueck;
        }


        if (z > -1) {
            t = Feld2;
            Feld2 |= Feldwert[z][Hoehe[z]++];
            rueck = SpielBaum1Generate(instance + 1, alpha, beta, ZKey ^ zufall[1][z * 6 - 1 + Hoehe[z]], symM);

            Feld2 = t;
            Hoehe[z]--;
            if (rueck >= beta) {
                hsh->key = ZKey;
                hsh->value = rueck;
                hsh->flag = LOWER;
                return rueck;
            } else if (rueck > alpha) {
                alpha = rueck;
                IstExact = true;
            }
            if (rueck == 1000) {
                hsh->key = ZKey;
                hsh->value = rueck;
                hsh->flag = (IstExact ? EXACT : UPPER);
                return rueck;
            }
        }
    }

    if (q != -1) {
        if (Hoehe[q] < 4 && Hoehe[q] % 2 && later == -1 && Gewinnstellung2(q, Hoehe[q] + 1))
            later = q;
        else {
            t = Feld2;
            Feld2 |= Feldwert[q][Hoehe[q]++];
            rueck = SpielBaum1Generate(instance + 1, alpha, beta, ZKey ^ zufall[1][q * 6 - 1 + Hoehe[q]], symM);

            Feld2 = t;
            Hoehe[q]--;
            if (rueck >= beta) {
                hsh->key = ZKey;
                hsh->value = rueck;
                hsh->flag = LOWER;
                return rueck;
            } else if (rueck > alpha) {
                alpha = rueck;
                IstExact = true;
            }
            if (rueck == 1000) {
                hsh->key = ZKey;
                hsh->value = rueck;
                hsh->flag = (IstExact ? EXACT : UPPER);
                return rueck;
            }
        }
    }

    for (x = 0; moves[x] > -1; x++) {
        if (y == moves[x] || z == moves[x] || q == moves[x])
            continue;
        if (Hoehe[moves[x]] < 4 && Hoehe[moves[x]] % 2 && later == -1 && Gewinnstellung2(moves[x], Hoehe[moves[x]] + 1))
            later = moves[x];
        else {
            t = Feld2;
            Feld2 |= Feldwert[moves[x]][Hoehe[moves[x]]++];
            rueck = SpielBaum1Generate(instance + 1, alpha, beta, ZKey ^ zufall[1][moves[x] * 6 - 1 + Hoehe[moves[x]]],
                                       symM);

            Feld2 = t;
            Hoehe[moves[x]]--;
            if (rueck >= beta) {
                hsh->key = ZKey;
                hsh->value = rueck;
                hsh->flag = LOWER;
                return rueck;
            } else if (rueck > alpha) {
                alpha = rueck;
                IstExact = true;
            }
            if (rueck == 1000) {
                hsh->key = ZKey;
                hsh->value = rueck;
                hsh->flag = (IstExact ? EXACT : UPPER);
                return rueck;
            }
        }
    }
    if (later > -1) {
        t = Feld2;
        Feld2 |= Feldwert[later][Hoehe[later]++];
        rueck = SpielBaum1Generate(instance + 1, alpha, beta, ZKey ^ zufall[1][later * 6 - 1 + Hoehe[later]], symM);

        Feld2 = t;
        Hoehe[later]--;
        if (rueck >= beta) {
            hsh->key = ZKey;
            hsh->value = rueck;
            hsh->flag = LOWER;
            return rueck;
        } else if (rueck > alpha) {
            alpha = rueck;
            IstExact = true;
        }
    }
    if (IstExact)
        hsh->flag = EXACT;
    else
        hsh->flag = UPPER;
    hsh->key = ZKey;
    hsh->value = alpha;
    return alpha;
}

void GenerateMoves(short *feld) {
    short x, position, wert[7], groesster, i;  //wert müsste man auslagern können
    for (x = 0; x < 7; x++) {
        if (Hoehe[x] < 6) {
            position = x * 6 + Hoehe[x];
            wert[x] = ZugBewertung[position];
        } else
            wert[x] = 0;
    }
    for (x = 0; x < 7; x++) {
        groesster = 0;
        for (i = 0; i < 7; i++) {
            if (wert[i] > groesster) {
                position = i;
                groesster = wert[i];
            }
        }
        if (groesster == 0) {
            feld[x] = -1;
            return;
        }
        feld[x] = position;
        wert[position] = 0;
    }
    feld[x] = -1;
}

#include <cstdint>

#if false
                                                                                                                        short WurzelMethodeComputerAnziehender(short instance, short alpha, short beta, uint64_t ZKey)
{
	short x, y=0, rueck,xret, moves[8];
	if(SpielBeenden2())
			return SpielBeendenStellung(SPIELER2);
	gMoves2(moves);
	if(istSymmetrisch()) {
        short tmp[8];
		for(x=0;moves[x] != -1;x++) {
			if(moves[x] <4)
				tmp[y++] = moves[x];
		}
		for(x=0;x<y;x++)
			moves[x] = tmp[x];
		moves[x] = -1;
	}

	for(x=0;moves[x]>-1;x++)
    {
		SteinSetzen(moves[x],SPIELER2);
		rueck=SpielBaum1Generate(instance+1,alpha,beta,ZKey^zufall[1][moves[x]*6+Hoehe[moves[x]]-1], symMoeglich());
		SteinLoeschen(moves[x],SPIELER2);
		if(rueck>alpha)
        {
            alpha=rueck;
            xret=moves[x];
		}
		if(rueck == 1000)
        	return xret;
    }
	return xret;
}
#else

short WurzelMethodeComputerAnziehender(short instance, short alpha, short beta, uint64_t ZKey) {
    short x, y = 0, rueck, xret, moves[8];
    if (SpielBeenden2())
        return -123;
    gMoves2(moves);
    if (istSymmetrisch()) {
        short tmp[8];
        for (x = 0; moves[x] != -1; x++) {
            if (moves[x] < 4)
                tmp[y++] = moves[x];
        }
        //tmp[y] = -1;
        for (x = 0; x < y; x++)
            moves[x] = tmp[x];
        moves[x] = -1;
    }

    for (x = 0; moves[x] > -1; x++) {
        SteinSetzen(moves[x], SPIELER2);
        rueck = SpielBaum1Generate(instance + 1, alpha, beta, ZKey ^ zufall[1][moves[x] * 6 + Hoehe[moves[x]] - 1],
                                   symMoeglich());
        SteinLoeschen(moves[x], SPIELER2);
        if (rueck > alpha) {
            alpha = rueck;
            xret = moves[x];
        }
        if (rueck == 1000)
            return 1000;
    }
    return alpha;
}

#endif

short Spielbaum1ComputerAnziehender(const short instance, short alpha, short beta, const uint64_t ZKey) {
    if (SpielBeenden1())
        return -1000;

    short later = -1, x, rueck;
    uint64_t t;

    if (instance < 20)    //20?? oder 22??
        for (x = 0; x < 7; x++) {
            if (Hoehe[x] != 6) {
                t = ZKey ^ zufall[0][x * 6 + Hoehe[x]];
                HASHEvorlage *hsh = &HASHE[int(t & (HASHSIZE - 1))];
                if (hsh->key == t && hsh->flag != LOWER && hsh->value <= alpha)
                    return hsh->value;
            }
        }

    x = -1;
    if (instance < 20) {
        x = FindThreat1();
        if (x > -1) {
            t = Feld1;
            Feld1 |= Feldwert[x][Hoehe[x]++];
            rueck = Spielbaum2ComputerAnziehender(instance + 1, alpha, beta, ZKey ^ zufall[0][x * 6 - 1 + Hoehe[x]]);

            Feld1 = t;
            Hoehe[x]--;
            if (rueck <= alpha)
                return rueck;
            else if (rueck < beta)
                beta = rueck;
        }
    }

    if (x != 3 && *(Hoehe + 3) < 6) {
        if (Hoehe[3] < 5 /*&& *(Hoehe+3)%2==0*/ && Gewinnstellung1(3, *(Hoehe + 3) + 1))
            later = 3;
        else {
            t = Feld1;
            Feld1 |= Feldwert3[(*(Hoehe + 3))++];
            rueck = Spielbaum2ComputerAnziehender(instance + 1, alpha, beta, ZKey ^ zufall13[Hoehe[3]]);

            Feld1 = t;
            Hoehe[3]--;
            if (rueck <= alpha)
                return rueck;
            else if (rueck < beta)
                beta = rueck;
        }
    }
    if (x != 4 && *(Hoehe + 4) < 6) {
        if (later == -1 && Hoehe[4] < 5 /*&& *(Hoehe+4)%2==0*/ && Gewinnstellung1(4, *(Hoehe + 4) + 1))
            later = 4;
        else {
            t = Feld1;
            Feld1 |= Feldwert4[(*(Hoehe + 4))++];
            rueck = Spielbaum2ComputerAnziehender(instance + 1, alpha, beta, ZKey ^ zufall14[Hoehe[4]]);

            Feld1 = t;
            Hoehe[4]--;
            if (rueck <= alpha)
                return rueck;
            else if (rueck < beta)
                beta = rueck;
        }
    }
    if (x != 2 && *(Hoehe + 2) < 6) {
        if (later == -1 && Hoehe[2] < 5 /*&& *(Hoehe+2)%2==0*/ && Gewinnstellung1(2, *(Hoehe + 2) + 1))
            later = 2;
        else {
            t = Feld1;
            Feld1 |= Feldwert2[(*(Hoehe + 2))++];
            rueck = Spielbaum2ComputerAnziehender(instance + 1, alpha, beta, ZKey ^ zufall12[Hoehe[2]]);

            Feld1 = t;
            Hoehe[2]--;
            if (rueck <= alpha)
                return rueck;
            else if (rueck < beta)
                beta = rueck;
        }
    }
    if (x != 5 && *(Hoehe + 5) < 6) {
        if (later == -1 && Hoehe[5] < 5 /*&& *(Hoehe+5)%2==0*/ && Gewinnstellung1(5, *(Hoehe + 5) + 1))
            later = 5;
        else {
            t = Feld1;
            Feld1 |= Feldwert5[(*(Hoehe + 5))++];
            rueck = Spielbaum2ComputerAnziehender(instance + 1, alpha, beta, ZKey ^ zufall15[Hoehe[5]]);

            Feld1 = t;
            Hoehe[5]--;
            if (rueck <= alpha)
                return rueck;
            else if (rueck < beta)
                beta = rueck;
        }
    }
    if (x != 1 && *(Hoehe + 1) < 6) {
        if (later == -1 && Hoehe[1] < 5 /*&& *(Hoehe+1)%2==0*/ && Gewinnstellung1(1, *(Hoehe + 1) + 1))
            later = 1;
        else {
            t = Feld1;
            Feld1 |= Feldwert1[(*(Hoehe + 1))++];
            rueck = Spielbaum2ComputerAnziehender(instance + 1, alpha, beta, ZKey ^ zufall11[Hoehe[1]]);

            Feld1 = t;
            Hoehe[1]--;
            if (rueck <= alpha)
                return rueck;
            else if (rueck < beta)
                beta = rueck;
        }
    }
    if (x != 6 && *(Hoehe + 6) < 6) {
        if (later == -1 && Hoehe[6] < 5 /*&& *(Hoehe+6)%2==0*/ && Gewinnstellung1(6, *(Hoehe + 6) + 1))
            later = 6;
        else {
            t = Feld1;
            Feld1 |= Feldwert6[(*(Hoehe + 6))++];
            rueck = Spielbaum2ComputerAnziehender(instance + 1, alpha, beta, ZKey ^ zufall16[Hoehe[6]]);

            Feld1 = t;
            Hoehe[6]--;
            if (rueck <= alpha)
                return rueck;
            else if (rueck < beta)
                beta = rueck;
        }
    }
    if (x != 0 && (*Hoehe) < 6) {
        t = Feld1;
        Feld1 |= Feldwert0[(*Hoehe)++];
        rueck = Spielbaum2ComputerAnziehender(instance + 1, alpha, beta, ZKey ^ zufall10[Hoehe[0]]);

        Feld1 = t;
        Hoehe[0]--;
        if (rueck <= alpha)
            return rueck;
        else if (rueck < beta)
            beta = rueck;
    }
    if (later != (-1)) {
        t = Feld1;
        Feld1 |= Feldwert[later][Hoehe[later]++];
        rueck = Spielbaum2ComputerAnziehender(instance + 1, alpha, beta,
                                              ZKey ^ zufall[0][later * 6 - 1 + Hoehe[later]]);

        Feld1 = t;
        Hoehe[later]--;
        if (rueck <= alpha)
            return rueck;
        else if (rueck < beta)
            beta = rueck;
    }
    return beta;
}

short Spielbaum2ComputerAnziehender(const short instance, short alpha, short beta, const uint64_t ZKey) {
    if (SpielBeenden2())
        return 1000;

    if (instance == InstanceVoll - 2) {
        const int64_t f = (~(Feld1 | Feld2)) & 0x1041041041LL;
        if (f & (f - 1LL)) { //Wenn beide leeren Felder in oberster Reihe sind
            //if(!SpielBeenden1())
            //	return 0;
            short v[2], i = 0;
            if (Hoehe[0] != 6)
                v[i++] = 0;
            if (Hoehe[6] != 6)
                v[i++] = 6;
            if (Hoehe[1] != 6)
                v[i++] = 1;
            if (Hoehe[5] != 6)
                v[i++] = 5;
            if (Hoehe[2] != 6)
                v[i++] = 2;
            if (Hoehe[4] != 6)
                v[i++] = 4;
            if (Hoehe[3] != 6)
                v[i] = 3;
            if (Gewinnstellung1(v[0], Hoehe[v[0]]) && Gewinnstellung1(v[1], Hoehe[v[1]]))
                return -1000;
            return 0;

        } else {
            short v;
            if (Hoehe[0] != 6)
                v = 0;
            else if (Hoehe[6] != 6)
                v = 6;
            else if (Hoehe[1] != 6)
                v = 1;
            else if (Hoehe[5] != 6)
                v = 5;
            else if (Hoehe[2] != 6)
                v = 2;
            else if (Hoehe[4] != 6)
                v = 4;
            else if (Hoehe[3] != 6)
                v = 3;
            if (Gewinnstellung1(v, Hoehe[v] + 1))
                return -1000;
            return 0;

        }
    }

    if (instance == InstanceVoll)
        return 0;
    if (instance == MaxInstance)
        return BewertungComputerAnziehender();

    if (instance > 26) {
        int pos = int(ZKey & (DRHASHSIZE - 1));
        if (DRHASHE[pos].key == ZKey)
            return DRHASHE[pos].value;
        short fs[2], anz;
        anz = FreieSpalten(fs);
        if (anz == 1)
            return SiegerErmitteln1(fs);
        else if (anz == 2) {
            if (!(Gewinnstellung1(fs[0], Hoehe[fs[0]]) ||
                  Gewinnstellung1(fs[1], Hoehe[fs[1]]))) //?berlegen wie diese einbezogen werden k?nnen
            {
                short bewert = SiegerErmitteln2(fs);
                if (bewert != -1) {
                    DRHASHE[pos].key = ZKey;
                    DRHASHE[pos].value = bewert;
                    return bewert;
                }
            }
        }
    }

    int HashPosition = int(ZKey & (HASHSIZE - 1));
    HASHEvorlage *hsh = &HASHE[HashPosition];
    if (hsh->key == ZKey) {
        switch (hsh->flag) {
            case EXACT:
                return hsh->value;
            case LOWER:
                if (hsh->value >= beta)
                    return hsh->value;
                if (hsh->value > alpha)
                    alpha = hsh->value;
                break;
            case UPPER:
                if (hsh->value <= alpha)
                    return hsh->value;
                if (hsh->value < beta)
                    beta = hsh->value;
                break;
        }
    }

    bool IstExact = false;
    short later = -1, x = -1, rueck;
    uint64_t t;
    if (instance < 24) {
        x = FindOddThreat2();
        if (x > -1) {
            t = Feld2;
            Feld2 |= Feldwert[x][Hoehe[x]++];
            rueck = Spielbaum1ComputerAnziehender(instance + 1, alpha, beta, ZKey ^ zufall[1][x * 6 - 1 + Hoehe[x]]);

            Feld2 = t;
            Hoehe[x]--;
            if (rueck >= beta) {
                hsh->key = ZKey;
                hsh->value = rueck;
                hsh->flag = LOWER;
                return rueck;
            } else if (rueck > alpha) {
                alpha = rueck;
                IstExact = true;
            }
        }
    }
    if (x != 3 && *(Hoehe + 3) < 6) {
        if (Hoehe[3] < 4 && *(Hoehe + 3) % 2 && Gewinnstellung2(3, *(Hoehe + 3) + 1))
            later = 3;
        else {
            t = Feld2;
            Feld2 |= Feldwert3[(*(Hoehe + 3))++];
            rueck = Spielbaum1ComputerAnziehender(instance + 1, alpha, beta, ZKey ^ zufall23[Hoehe[3]]);

            Feld2 = t;
            Hoehe[3]--;
            if (rueck >= beta) {
                hsh->key = ZKey;
                hsh->value = rueck;
                hsh->flag = LOWER;
                return rueck;
            } else if (rueck > alpha) {
                alpha = rueck;
                IstExact = true;
            }
        }
    }
    if (x != 4 && *(Hoehe + 4) < 6) {
        if (Hoehe[4] < 4 && later == -1 && *(Hoehe + 4) % 2 && Gewinnstellung2(4, *(Hoehe + 4) + 1))
            later = 4;
        else {
            t = Feld2;
            Feld2 |= Feldwert4[(*(Hoehe + 4))++];
            rueck = Spielbaum1ComputerAnziehender(instance + 1, alpha, beta, ZKey ^ zufall24[Hoehe[4]]);

            Feld2 = t;
            Hoehe[4]--;
            if (rueck >= beta) {
                hsh->key = ZKey;
                hsh->value = rueck;
                hsh->flag = LOWER;
                return rueck;
            } else if (rueck > alpha) {
                alpha = rueck;
                IstExact = true;
            }
        }
    }
    if (x != 2 && *(Hoehe + 2) < 6) {
        if (later == -1 && Hoehe[2] < 4 && *(Hoehe + 2) % 2 && Gewinnstellung2(2, *(Hoehe + 2) + 1))
            later = 2;
        else {
            t = Feld2;
            Feld2 |= Feldwert2[(*(Hoehe + 2))++];
            rueck = Spielbaum1ComputerAnziehender(instance + 1, alpha, beta, ZKey ^ zufall22[Hoehe[2]]);

            Feld2 = t;
            Hoehe[2]--;
            if (rueck >= beta) {
                hsh->key = ZKey;
                hsh->value = rueck;
                hsh->flag = LOWER;
                return rueck;
            } else if (rueck > alpha) {
                alpha = rueck;
                IstExact = true;
            }
        }
    }
    if (x != 5 && *(Hoehe + 5) < 6) {
        if (later == -1 && Hoehe[5] < 4 && *(Hoehe + 5) % 2 && Gewinnstellung2(5, *(Hoehe + 5) + 1))
            later = 5;
        else {
            t = Feld2;
            Feld2 |= Feldwert5[(*(Hoehe + 5))++];
            rueck = Spielbaum1ComputerAnziehender(instance + 1, alpha, beta, ZKey ^ zufall25[Hoehe[5]]);

            Feld2 = t;
            Hoehe[5]--;
            if (rueck >= beta) {
                hsh->key = ZKey;
                hsh->value = rueck;
                hsh->flag = LOWER;
                return rueck;
            } else if (rueck > alpha) {
                alpha = rueck;
                IstExact = true;
            }
        }
    }
    if (x != 1 && *(Hoehe + 1) < 6) {
        if (later == -1 && Hoehe[1] < 4 && *(Hoehe + 1) % 2 && Gewinnstellung2(1, *(Hoehe + 1) + 1))
            later = 1;
        else {
            t = Feld2;
            Feld2 |= Feldwert1[(*(Hoehe + 1))++];
            rueck = Spielbaum1ComputerAnziehender(instance + 1, alpha, beta, ZKey ^ zufall21[Hoehe[1]]);

            Feld2 = t;
            Hoehe[1]--;
            if (rueck >= beta) {
                hsh->key = ZKey;
                hsh->value = rueck;
                hsh->flag = LOWER;
                return rueck;
            } else if (rueck > alpha) {
                alpha = rueck;
                IstExact = true;
            }
        }
    }
    if (x != 6 && *(Hoehe + 6) < 6) {
        if (later == -1 && Hoehe[6] < 4 && *(Hoehe + 6) % 2 && Gewinnstellung2(6, *(Hoehe + 6) + 1))
            later = 6;
        else {
            t = Feld2;
            Feld2 |= Feldwert6[(*(Hoehe + 6))++];
            rueck = Spielbaum1ComputerAnziehender(instance + 1, alpha, beta, ZKey ^ zufall26[Hoehe[6]]);

            Feld2 = t;
            Hoehe[6]--;
            if (rueck >= beta) {
                hsh->key = ZKey;
                hsh->value = rueck;
                hsh->flag = LOWER;
                return rueck;
            } else if (rueck > alpha) {
                alpha = rueck;
                IstExact = true;
            }
        }
    }
    if (x != 0 && *(Hoehe) < 6) {
        t = Feld2;
        Feld2 |= Feldwert0[(*Hoehe)++];
        rueck = Spielbaum1ComputerAnziehender(instance + 1, alpha, beta, ZKey ^ zufall20[Hoehe[0]]);

        Feld2 = t;
        Hoehe[0]--;
        if (rueck >= beta) {
            hsh->key = ZKey;
            hsh->value = rueck;
            hsh->flag = LOWER;
            return rueck;
        } else if (rueck > alpha) {
            alpha = rueck;
            IstExact = true;
        }
    }
    if (later != (-1)) {
        t = Feld2;
        Feld2 |= Feldwert[later][Hoehe[later]++];
        rueck = Spielbaum1ComputerAnziehender(instance + 1, alpha, beta,
                                              ZKey ^ zufall[1][later * 6 - 1 + Hoehe[later]]);

        Feld2 = t;
        Hoehe[later]--;
        if (rueck >= beta) {
            hsh->key = ZKey;
            hsh->value = rueck;
            hsh->flag = LOWER;
            return rueck;
        } else if (rueck > alpha) {
            alpha = rueck;
            IstExact = true;
        }
    }
    hsh->flag = (IstExact ? EXACT : UPPER);
    hsh->key = ZKey;
    hsh->value = alpha;
    return alpha;
}

int BewertungComputerAnziehender(void) {
    return 3333;
}

short FreieSpalten(short *spalten) {
    short i = 0;
    /*int64_t x= (Feld1 | Feld2) & 0x1041041041LL;  //Bits z?hlen... Wenn weniger als 5 Spalten voll sind funktion abbrechen
    short n = 0;
        if (x) do
              n++;
        while (0LL != (x = x&(x-1LL))) ;
    if(n<5)
        return 0;*/
    if (Hoehe[0] < 6)
        spalten[i++] = 0;
    if (Hoehe[6] < 6)
        spalten[i++] = 6;

    if (Hoehe[1] < 6)
        if (i != 2)
            spalten[i++] = 1;
        else
            return 0;

    if (Hoehe[5] < 6)
        if (i != 2)
            spalten[i++] = 5;
        else
            return 0;

    if (Hoehe[2] < 6)
        if (i != 2)
            spalten[i++] = 2;
        else
            return 0;

    if (Hoehe[4] < 6)
        if (i != 2)
            spalten[i++] = 4;
        else
            return 0;

    if (Hoehe[3] < 6)
        if (i != 2)
            spalten[i++] = 3;
        else
            return 0;
    if (i == 2)
        if (abs(spalten[0] - spalten[1]) <= 3) //Wenn die beiden Spalten nicht mehr als 3 Spalten auseinanderliegen
            return 0;
    return i;
}

/*short SiegerErmitteln1(const short x)  //scheint ein ganz kleines bisschen langsamer zu sein
{
	short y=Hoehe[x]+1;
	while(y<6)
	{
		if( y & 1)
		{
			if(Gewinnstellung1(x, y))
				return -1000;
		}
		else
		{
			if(Gewinnstellung2(x, y))
				return 1000;
		}
		++y;
	}
	return 0;
}*/

short SiegerErmitteln1(const short *x) {
    switch (Hoehe[*x] +
            1) //Wenn Spieler1 eine akute Drohung h?tte, dann w?rde diese sowieso geschlossen werden, daher kann man 1 inkrementieren
    {
        case 1:
            if (Gewinnstellung1(*x, 1)) return -1000;
        case 2:
            if (Gewinnstellung2(*x, 2)) return 1000;
        case 3:
            if (Gewinnstellung1(*x, 3)) return -1000;
        case 4:
            if (Gewinnstellung2(*x, 4)) return 1000;
        case 5:
            if (Gewinnstellung1(*x, 5)) return -1000;
    }
    return 0;
}

short SiegerErmitteln2(const short *s) {
    short Hs = Hoehe[*s] + 1, dro1 = 0, dro2 = 0;
    while (Hs < 6) {
        if (Gewinnstellung1(*s, Hs)) {
            if (dro1)
                return -1;
            dro1 = (Hs & 1) + 10;
        }
        if (Gewinnstellung2(*s, Hs)) {
            if (dro1)
                return -1;
            dro1 = (Hs & 1) + 20;
        }
        Hs++;
    }
    Hs = Hoehe[*(s + 1)] + 1;
    while (Hs < 6) {
        if (Gewinnstellung1(*(s + 1), Hs)) {
            if (dro2)
                return -1;
            dro2 = (Hs & 1) + 10;
        }
        if (Gewinnstellung2(*(s + 1), Hs)) {
            if (dro2)
                return -1;
            dro2 = (Hs & 1) + 20;
        }
        Hs++;
    }

    if (dro2 == 0) {
        switch (dro1) {
            case 0:
            default:
                return 0;
            case 20:
                return 1000;
            case 11:
                return -1000;
        }
    }

    if (dro1 == 0) {
        switch (dro2) {
            case 0:
            default:
                return 0;
            case 20:
                return 1000;
            case 11:
                return -1000;
        }
    }

    if (dro1 == 20 || dro2 == 20)
        if (dro1 != 10 && dro2 != 10)
            return 1000;
        else
            return 0;

    if (dro1 == 10 && dro2 == 10)
        return -1000;

    if (dro1 == 11 || dro2 == 11)
        return -1000;
    return 0;

    /*switch(dro2*100 + dro1)
    {
        case 2021: case 2011: case 2120: case 2020: case 1120: case 20: case 2000:
            return 1000;
        case 1011: case 1111: case 2111: case 1110: case 1121:case 1010: case 11: case 1100:
            return -1000;
        default: //case 2010: //case 1020: //case 0:
            return 0;
    }*/
}

#include <fstream>
#include <string>
#include <iostream>

void BuchLaden(std::string pfad) {
    /*Öffnet die Datenbank(die Datenbank muss sich im gleichen Verzeichnis wie die Anwendung befinden)
    und lädt die Stellungen in die Variablen FeldData1 und FeldData2. Die Bewertungen der Stellungen
    werden im Array Win[] gespeichert.*/
    int dateigroesse;
    long k = 0;
    //AnsiString pfad=ExtractFileDir(Application->ExeName)+"\\book.dat"; //Pfad der Datenbank
    // std::string pfad = "../book.dat";

    std::ifstream fdatei;
    fdatei.open(pfad.c_str(), std::ios::in | std::ios::binary);
    if (fdatei.fail()) {
        std::cerr << "8-Steine Datenbank konnt nicht geöffnet werden. Das Programm wird beendet!!!" << std::endl;
        exit(EXIT_FAILURE);
    }

    fdatei.seekg(0, std::ios::end);
    dateigroesse = fdatei.tellg();
    if (dateigroesse != 102858) {
        std::cerr << "8-Steine Datenbank wurde manipuliert. Das Programm wird beendet!!!" << std::endl;
        exit(EXIT_FAILURE);
    }
    fdatei.seekg(0, std::ios::beg);

    unsigned char one, two, three;

    while (k < 34286) {
        fdatei.read(reinterpret_cast<char *> (&one), sizeof(char));
        fdatei.read(reinterpret_cast<char *> (&two), sizeof(char));
        fdatei.read(reinterpret_cast<char *> (&three), sizeof(char));

        //fdatei.read(reinterpret_cast<char *> (&OPENING[k].FeldData1), sizeof(int64_t));

        book[k] = 0;
        book[k] |= int(one) << 16;
        book[k] |= int(two) << 8;
        book[k++] |= int(three);

    }
    book[k] = -1;
    fdatei.close();
}

void BuchLaden2(std::string pfad) {
    /*Öffnet die Datenbank(die Datenbank muss sich im gleichen Verzeichnis wie die Anwendung befinden)
    und lädt die Stellungen in die Variablen FeldData1 und FeldData2. Die Bewertungen der Stellungen
    werden im Array Win[] gespeichert.*/
    int dateigroesse;
    long k = 0;
    //std::string pfad = "../data8ply.dat";

    OPENING = new OPENINGvorlage[86894];
    std::ifstream fdatei;
    fdatei.open(pfad.c_str(), std::ios::in | std::ios::binary);
    if (fdatei.fail()) {
        std::cerr << "8-Steine Datenbank (2) konnt nicht geöffnet werden. Das Programm wird beendet!!!";
        exit(0);
    }

    fdatei.seekg(0, std::ios::end);
    dateigroesse = fdatei.tellg();
    if (dateigroesse != 1477198) {
        std::cerr << "8-Steine Datenbank (2) wurde manipuliert. Das Programm wird beendet!!!";
        exit(0);
    }
    fdatei.seekg(0, std::ios::beg);
    while (k < 86893) {
        fdatei.read(reinterpret_cast<char *> (&OPENING[k].FeldData1), sizeof(uint64_t));
        fdatei.read(reinterpret_cast<char *> (&OPENING[k].FeldData2), sizeof(uint64_t));
        fdatei.read(reinterpret_cast<char *> (&OPENING[k++].Win), sizeof(char));
    }

    OPENING[k].FeldData1 = -1; //mit Endekennung zu vergleichen
    OPENING[k].FeldData2 = -1; //mit Endekennung zu vergleichen
    OPENING[k].Win = -1;
    fdatei.close();
}

short WurzelMethodeStartComputerAnziehender(const short instance, short alpha, short beta, const uint64_t ZKey) {
    short moves[8], rueck;
    if (SpielBeenden2Generate(moves))//Wenn Computer gewinnen würde...
        return SpielBeendenStellung(SPIELER2);//Falls es die Wurzel-Methode ist...

    short x, xret, later = -1;
    for (x = 0; moves[x] > -1; x++) {
        if (Hoehe[moves[x]] % 2 && later == -1 && Gewinnstellung2(moves[x], Hoehe[moves[x]] + 1))
            later = moves[x];
        else {
            Feld2 |= Feldwert[moves[x]][Hoehe[moves[x]]++];
            rueck = SpielbaumStart1ComputerAnziehender(instance + 1, alpha, beta,
                                                       ZKey ^ zufall[1][moves[x] * 6 - 1 + Hoehe[moves[x]]]);
            Feld2 &= Feldwertinv[moves[x]][--Hoehe[moves[x]]];
            if (rueck > alpha) {
                alpha = rueck;
                xret = moves[x];
            }
        }
    }
    if (later > -1) {
        Feld2 |= Feldwert[later][Hoehe[later]++];
        rueck = SpielbaumStart1ComputerAnziehender(instance + 1, alpha, beta,
                                                   ZKey ^ zufall[1][later * 6 - 1 + Hoehe[later]]);
        Feld2 &= Feldwertinv[later][--Hoehe[later]];
        if (rueck > alpha)
            xret = moves[later];
    }
    return xret;
}

short SpielbaumStart1ComputerAnziehender(const short instance, short alpha, short beta, const uint64_t ZKey) {
    short moves[8], x, later = -1, rueck;
    if (SpielBeenden1Generate(moves))
        return -1000;

    for (x = 0; moves[x] > -1; x++) {
        if (Hoehe[moves[x]] % 2 == 0 && later == -1 && Gewinnstellung1(moves[x], Hoehe[moves[x]] + 1))
            later = moves[x];
        else {
            Feld1 |= Feldwert[moves[x]][Hoehe[moves[x]]++];
            rueck = SpielbaumStart2ComputerAnziehender(instance + 1, alpha, beta,
                                                       ZKey ^ zufall[0][moves[x] * 6 - 1 + Hoehe[moves[x]]]);
            Feld1 &= Feldwertinv[moves[x]][--Hoehe[moves[x]]];
            if (rueck <= alpha)
                return rueck;
            else if (rueck < beta)
                beta = rueck;
        }
    }
    if (later > -1) {
        Feld1 |= Feldwert[later][Hoehe[later]++];
        rueck = SpielbaumStart2ComputerAnziehender(instance + 1, alpha, beta,
                                                   ZKey ^ zufall[0][later * 6 - 1 + Hoehe[later]]);
        Feld1 &= Feldwertinv[later][--Hoehe[later]];
        if (rueck <= alpha)
            return rueck;
        else if (rueck < beta)
            beta = rueck;
    }
    return beta;
}

int convert(int64_t f1, int64_t f2) {
    int temp = 0, j;
    int64_t mask;

    //Stellung Codieren, Huffmann Code
    for (j = 41; j >= 0; j--) {
        mask = 1LL << j;
        if (f1 & mask) {
            temp <<= 2;
            temp |= 3;
        } else if (f2 & mask) {
            temp <<= 2;
            temp |= 2;
        }
        if (!(j % 6))
            temp <<= 1;
    }
    return temp << 1;
}

int convert(int64_t f1, int64_t f2, bool mir) {
    int temp = 0, j, i;
    int64_t mask;

    //STellung Codieren
    for (j = (mir ? 6 : 0); (mir && j >= 0) || (!mir && j < 7); mir ? j-- : j++) {
        for (i = 0; i < Hoehe[j]; i++) {
            mask = 1LL << (mir ? 5 + 6 * j - i : 41 - (j * 6 + i));

            if (f1 & mask) {
                temp <<= 2;
                temp |= 3;
            } else if (f2 & mask) {
                temp <<= 2;
                temp |= 2;
            }
        }
        temp <<= 1;
    }
    return temp << 1;
}

short SpielbaumStart2ComputerAnziehender(const short instance, short alpha, short beta, const uint64_t ZKey) {
    short moves[8];
    if (SpielBeenden2Generate(moves))
        return 1000;

    if (BrettCount() == 8) {
        /*Feld1 = 0x15400000LL;
        Feld2 = 0x2A800000LL;
        HoeheErmitteln();*/
        uint64_t Feld1Gespiegelt = 0LL, Feld2Gespiegelt = 0LL;
        //int stelle1, stelle2;

        Feld1Gespiegelt |= ((Feld1 & spalte[0]) >> 36);
        Feld1Gespiegelt |= ((Feld1 & spalte[1]) >> 24);
        Feld1Gespiegelt |= ((Feld1 & spalte[2]) >> 12);
        Feld1Gespiegelt |= (Feld1 & spalte[3]);
        Feld1Gespiegelt |= ((Feld1 & spalte[4]) << 12);
        Feld1Gespiegelt |= ((Feld1 & spalte[5]) << 24);
        Feld1Gespiegelt |= ((Feld1 & spalte[6]) << 36);
        Feld2Gespiegelt |= ((Feld2 & spalte[0]) >> 36);
        Feld2Gespiegelt |= ((Feld2 & spalte[1]) >> 24);
        Feld2Gespiegelt |= ((Feld2 & spalte[2]) >> 12);
        Feld2Gespiegelt |= (Feld2 & spalte[3]);
        Feld2Gespiegelt |= ((Feld2 & spalte[4]) << 12);
        Feld2Gespiegelt |= ((Feld2 & spalte[5]) << 24);
        Feld2Gespiegelt |= ((Feld2 & spalte[6]) << 36);

        int pos = 34286, pos2 = 34286;
        int schritt = 34286;
        int code, code2, /*val,*/ codeNormal, codeMirrored;
        codeNormal = convert(Feld1, Feld2, false);
        codeMirrored = convert(Feld1Gespiegelt, Feld2Gespiegelt, true);

        while (schritt > 0) {
            schritt = (schritt != 1 ? (schritt + (schritt & 1)) >> 1 : 0);
            //if(pos < 34286)
            code = book[pos] & 0xFFFFFC;
            //if(pos2 < 34286)
            code2 = book[pos2] & 0xFFFFFC;

            if (codeNormal < code)
                pos -= schritt;
            else if (codeNormal > code)
                pos += schritt;
            else if (codeNormal == code)
                return (-1000);

            if (codeMirrored < code2)
                pos2 -= schritt;
            else if (codeMirrored > code2)
                pos2 += schritt;
            else if (codeMirrored == code2)
                return (-1000);
        }
        return 1000; //Stellung ist nicht in der Datenbank: daher Sieg
    }

    long HashPosition = long(ZKey & (HASHSIZE - 1));
    if (HASHE[HashPosition].key == ZKey) {
        switch (HASHE[HashPosition].flag) {
            case EXACT:
                return HASHE[HashPosition].value;
            case LOWER:
                if (HASHE[HashPosition].value >= beta) return HASHE[HashPosition].value;
                break;
            case UPPER:
                if (HASHE[HashPosition].value <= alpha) return HASHE[HashPosition].value;
                break;
        }
    }
    bool IstExact = false;
    short x, later = -1, rueck;
    for (x = 0; moves[x] > -1; x++) {

        if (Hoehe[moves[x]] % 2 && later == -1 && Gewinnstellung2(moves[x], Hoehe[moves[x]] + 1))
            later = moves[x];
        else {
            Feld2 |= Feldwert[moves[x]][Hoehe[moves[x]]++];
            rueck = SpielbaumStart1ComputerAnziehender(instance + 1, alpha, beta,
                                                       ZKey ^ zufall[1][moves[x] * 6 - 1 + Hoehe[moves[x]]]);
            Feld2 &= Feldwertinv[moves[x]][--Hoehe[moves[x]]];
            if (rueck >= beta) {
                HASHE[HashPosition].key = ZKey;
                HASHE[HashPosition].value = rueck;
                HASHE[HashPosition].flag = LOWER;
                return rueck;
            } else if (rueck > alpha) {
                alpha = rueck;
                IstExact = true;
            }
        }
    }
    if (later > -1) {
        Feld2 |= Feldwert[later][Hoehe[later]++];
        rueck = SpielbaumStart1ComputerAnziehender(instance + 1, alpha, beta,
                                                   ZKey ^ zufall[1][later * 6 - 1 + Hoehe[later]]);
        Feld2 &= Feldwertinv[later][--Hoehe[later]];
        if (rueck >= beta) {
            HASHE[HashPosition].key = ZKey;
            HASHE[HashPosition].value = rueck;
            HASHE[HashPosition].flag = LOWER;
            return rueck;
        } else if (rueck > alpha) {
            alpha = rueck;
            IstExact = true;
        }
    }
    if (IstExact)
        HASHE[HashPosition].flag = EXACT;
    else
        HASHE[HashPosition].flag = UPPER;
    HASHE[HashPosition].key = ZKey;
    HASHE[HashPosition].value = alpha;
    return alpha;
}

