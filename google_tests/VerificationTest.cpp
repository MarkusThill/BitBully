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
        Reset(); //entleert das Spielfeld. Die Variablen Feld1 und Feld2 werden auf 0 gesetzt.
        BuchLaden("../../book.dat");  //Die Er�ffnungsdatenbank wird direkt zu beginn aus der Datei eingelesen
        BuchLaden2("../../data8ply.dat");
        MaxInstance=100;  //Der Standardwert f�r die Schwierigkeitsstufe wird gesetzt. Die Schwierigkeitsstufe ergibt sich aus (MaxInstance-2)/2
        Modus=0;         //Da noch kein Spielmodus ausgew�hlt wurde
        HASHE = (HASHEvorlage *)malloc(HASHSIZE * sizeof(HASHEvorlage));
        LHASHE = (HASHEvorlage *)malloc(LHASHSIZE * sizeof(HASHEvorlage));
        DRHASHE = (DRVorlage *)malloc(DRHASHSIZE * sizeof(DRVorlage));
        ResetHash();
        ModusEinrichten();

        std::cout << "Version: v" << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << std::endl;
    }

    void TearDown() override {
    }

    ~ExampleTest() override {
        //resources cleanup, no exceptions allowed
    }
};

TEST_F(ExampleTest, fastVerificationTest) {
    int x, i;
    time_point time_start = std::chrono::high_resolution_clock::now();
    for(i=0;i<86892;i+=100)
    {
        Feld1=OPENING[i].FeldData1;
        Feld2=OPENING[i].FeldData2;
        ResetHash();
        HoeheErmitteln();
        InstanceVoll=42-BrettCount();
        MaxInstance=100;
        x = WurzelMethodeComputerAnziehender(0, -9999, 9999, HoeheErmitteln());
        EXPECT_FALSE(x==1000 && OPENING[i].Win!=2);
        EXPECT_FALSE(x==-1000 && OPENING[i].Win!=1);
        EXPECT_FALSE(x==0 && OPENING[i].Win!=0);
        EXPECT_FALSE(x!= -1000 && x!= 1000 && x!=0);
        std::cout << "Done with " << i << std::endl;
    }
    time_point time_end = std::chrono::high_resolution_clock::now();

    float dur = float(duration(time_end - time_start).count());
    std::cout << "Total Time for fastVerificationTest(): " << dur << "seconds" << std::endl;
}