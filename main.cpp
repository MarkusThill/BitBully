#include <iostream>
#include <string>
#include <iostream>
#include <sstream>
#include "cpart.cpp"

void verify()
{
    int x, i;
    for(i=0;i<86892;i+=7) //40
    {
        Feld1=OPENING[i].FeldData1;
        Feld2=OPENING[i].FeldData2;
        ResetHash();
        HoeheErmitteln();
        InstanceVoll=42-BrettCount();
        MaxInstance=100;
        x = WurzelMethodeComputerAnziehender(0, -9999, 9999, HoeheErmitteln());
        if(x==1000 && OPENING[i].Win!=2) {
            std::cerr << "i=" << i << ", x=" << x  << ":  if(x==1000 && OPENING[i].Win!=2)" << std::endl;
            break;
        }
        else if(x==-1000 && OPENING[i].Win!=1) {
            std::cerr << "i=" << i << ", x=" << x  << ": else if(x==-1000 && OPENING[i].Win!=1)" << std::endl;
            break;
        }
        else if(x==0 && OPENING[i].Win!=0) {
            std::cerr << "i=" << i << ", x=" << x  << ": else if(x==0 && OPENING[i].Win!=0)" << std::endl;
            break;
        }
        else if(x!= -1000 && x!= 1000 && x!=0) {
            std::cerr << "i=" << i << ", x=" << x  << ": else if(x!= -1000 && x!= 1000 && x!=0)" << std::endl;
            break;
        }
        std::cout << "Done with " << i << std::endl;
    }
}

int main() {
    Reset(); //entleert das Spielfeld. Die Variablen Feld1 und Feld2 werden auf 0 gesetzt.
    BuchLaden("../book.dat");  //Die Er�ffnungsdatenbank wird direkt zu beginn aus der Datei eingelesen
    BuchLaden2("../data8ply.dat");
    MaxInstance=100;  //Der Standardwert f�r die Schwierigkeitsstufe wird gesetzt. Die Schwierigkeitsstufe ergibt sich aus (MaxInstance-2)/2
    Modus=0;         //Da noch kein Spielmodus ausgew�hlt wurde
    HASHE = (HASHEvorlage *)malloc(HASHSIZE * sizeof(HASHEvorlage));
    LHASHE = (HASHEvorlage *)malloc(LHASHSIZE * sizeof(HASHEvorlage));
    DRHASHE = (DRVorlage *)malloc(DRHASHSIZE * sizeof(DRVorlage));
    ResetHash();
    ModusEinrichten();

    verify();

    /*
    std::cout << toString() << std::endl;
    ComputerAnziehender(2);
    std::cout << toString() << std::endl;
    ComputerAnziehender(5);
    std::cout << toString() << std::endl;
    ComputerAnziehender(4);
    std::cout << toString() << std::endl;
    ComputerAnziehender(2);
    std::cout << toString() << std::endl;
     */
    return 0;
}
