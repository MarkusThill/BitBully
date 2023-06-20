// TODO: use project-own google tests!!!
// TODO: Use a simple logger
// TODO: Log computation times using a software version into a txt file...
// TODO: Play n games against a random (or more advanced) player: It has to win every single game! ...

#include <filesystem>
#include <chrono>
#include "gtest/gtest.h"
#include "cpart.cpp"
#include "4inarow.h"

using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;
using duration = std::chrono::duration<float>;
namespace fs = std::filesystem;

class ExampleTest : public ::testing::Test {

protected:

    void SetUp() override {
        // TODO: We need a constructor
        c4.Reset(); //entleert das Spielfeld. Die Variablen m_fieldP1 und m_fieldP2 werden auf 0 gesetzt.
        c4.BuchLaden("../../book.dat");  //Die Er�ffnungsdatenbank wird direkt zu beginn aus der Datei eingelesen
        c4.BuchLaden2("../../data8ply.dat");
        c4.setMaxInstance(100);  //Der Standardwert fÜr die Schwierigkeitsstufe wird gesetzt. Die Schwierigkeitsstufe ergibt sich aus (m_maxSearchDepth-2)/2
        //c4.Modus = 0;         //Da noch kein Spielmodus ausgew�hlt wurde
        c4.initHash();
        c4.ResetHash();
        c4.ModusEinrichten();

        std::cout << "Version: v" << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << std::endl;
    }

    void TearDown() override {
    }

    ~ExampleTest() override {
        //resources cleanup, no exceptions allowed
    }

    Connect4 c4;
};

TEST_F(ExampleTest, randomOpponentTest) {
    srand(time(NULL));
    for(auto i=0UL;i<100;i++) {
        c4.Reset();
        c4.setFeld(0LL, 0LL);
        c4.ResetHash();
        c4.HoeheErmitteln();
        c4.setDepthTie();
        c4.setMaxInstance(100);
        c4.ModusEinrichten();


        int winner = 0;
        while(!c4.isGameOver()) {
            int randColumn = rand() % 7;
            while(c4.isColumnFull(randColumn)) randColumn = rand() % 7;

            std::cout << std::endl << c4.toString() << std::endl;
            winner = c4.ComputerAnziehender(randColumn);
        }

        //std::cout << std::endl << c4.toString() << std::endl;
        //std::cout<< "Game over" << std::endl;
        ASSERT_EQ(winner, 2);
    }

}

TEST_F(ExampleTest, fastVerificationTest) {;
    time_point time_start = std::chrono::high_resolution_clock::now();
    for (auto i = 0; i < 86892; i += 100) {
        auto entry = c4.getOpening(i);
        c4.setFeld(entry.m_positionP1, entry.m_positionP2);
        c4.ResetHash();
        c4.HoeheErmitteln();

        c4.setDepthTie();
        c4.setMaxInstance(100);
        auto [x, _] = c4.WurzelMethodeComputerAnziehender(0, -9999, 9999, c4.HoeheErmitteln());
        EXPECT_FALSE(x == 1000 && entry.m_value != 2) << "For position: " << i;
        EXPECT_FALSE(x == -1000 && entry.m_value != 1) << "For position: " << i;
        EXPECT_FALSE(x == 0 && entry.m_value != 0) << "For position: " << i;
        EXPECT_FALSE(x != -1000 && x != 1000 && x != 0) << "For position: " << i;
        if(i % 1000 == 0) std::cout << "Done with " << i << std::endl;
    }
    time_point time_end = std::chrono::high_resolution_clock::now();

    float dur = float(duration(time_end - time_start).count());
    std::cout << "Total Time for fastVerificationTest(): " << dur << " seconds" << std::endl;
}


static int level=86000000;//  =10; //level set to intermediate as default.
static int originalboard [7][6];

// settings passed to engine
int usethebook = 1;
int randomize = 0;
int searchwin = 0;
int gametheoretic = 0;
int evaltype = EVAL_SMART;

enum levels {BEGINNER,INTERMEDIATE,EXPERT,MASTER,INFLEVEL,GAMETHEORETIC,SEARCHWIN} levell;
int leveltime[7] = {0,8,30,300,8600000,86000000,86000000};

void setlevel(int l)
{
    level=l;
}

int getcomputermove(int color, int *play)
{
    char out[255];
    int bestmove;

    double maxtime;
    maxtime=((double)level)/10.0;

    sprintf(out,"calling findmove");
    bestmove = findmove(originalboard,color,maxtime,out, play,searchwin, randomize, usethebook, gametheoretic, evaltype );
    std::cout << out << std::endl;

    return(bestmove);
}

TEST_F(ExampleTest, doenerTest) {
    int color = 1; // 1== YELLOW, -1 ==RED???

    /*
    levell = INFLEVEL;
    gametheoretic = 0;
    searchwin = 0;
    setlevel(leveltime[levell]);
     */
    levell = GAMETHEORETIC;
    gametheoretic=1;
    searchwin=0;
    setlevel(leveltime[GAMETHEORETIC]);

    originalboard [3][0] = 1;
    originalboard [3][1] = -1;
    originalboard [3][2] = 1;
    originalboard [3][3] = -1;
    originalboard [3][4] = 1;
    originalboard [1][0] = -1;
    originalboard [1][1] = 1;
    originalboard [1][2] = -1;

    int playnow = 0;
    auto bestmove = getcomputermove(color, &playnow);
    std::cout << "best move:" << bestmove << std::endl;

    std::cout << "playnow:" << playnow << std::endl;

    extern int64 hashhits, hashmisses;
    std::cout << "hashhits: " << hashhits << ", hashmisses: " << hashmisses << std::endl;


    bestmove = getcomputermove(color, &playnow);
    std::cout << std::endl << "hashhits: " << hashhits << ", hashmisses: " << hashmisses << std::endl;
}