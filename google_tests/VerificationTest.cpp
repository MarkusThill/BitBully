// TODO: use project-own google tests!!!
// TODO: Use a simple logger
// TODO: Log computation times using a software version into a txt file...
// TODO: Play n games against a random (or more advanced) player: It has to win every single game! ...

#include <filesystem>
#include <chrono>
#include "gtest/gtest.h"
#include "cpart.cpp"

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
        c4.setMaxInstance(
                100);  //Der Standardwert fÜr die Schwierigkeitsstufe wird gesetzt. Die Schwierigkeitsstufe ergibt sich aus (m_maxSearchDepth-2)/2
        c4.Modus = 0;         //Da noch kein Spielmodus ausgew�hlt wurde
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

TEST_F(ExampleTest, fastVerificationTest) {
    int x, i;
    time_point time_start = std::chrono::high_resolution_clock::now();
    for (i = 0; i < 86892; i += 100) {
        auto entry = c4.getOpening(i);
        c4.setFeld(entry.m_positionP1, entry.m_positionP2);
        c4.ResetHash();
        c4.HoeheErmitteln();

        c4.setDepthTie();
        c4.setMaxInstance(100);
        x = c4.WurzelMethodeComputerAnziehender(0, -9999, 9999, c4.HoeheErmitteln());
        EXPECT_FALSE(x == 1000 && entry.m_value != 2);
        EXPECT_FALSE(x == -1000 && entry.m_value != 1);
        EXPECT_FALSE(x == 0 && entry.m_value != 0);
        EXPECT_FALSE(x != -1000 && x != 1000 && x != 0);
        std::cout << "Done with " << i << std::endl;
    }
    time_point time_end = std::chrono::high_resolution_clock::now();

    float dur = float(duration(time_end - time_start).count());
    std::cout << "Total Time for fastVerificationTest(): " << dur << " seconds" << std::endl;
}