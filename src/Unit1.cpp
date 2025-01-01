//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#include "cpart.cpp"

TForm1 *Form1;
//---------------------------------------------------------------------------

__fastcall TForm1::TForm1(TComponent *Owner) : TForm(Owner) {
  Empty = new TPicture;
  Empty->LoadFromFile(ExtractFileDir(Application->ExeName) +
                      "\\Images\\empty.bmp");
  Yellow = new TPicture;
  Yellow->LoadFromFile(ExtractFileDir(Application->ExeName) +
                       "\\Images\\yellow.bmp");
  Red = new TPicture;
  Red->LoadFromFile(ExtractFileDir(Application->ExeName) + "\\Images\\red.bmp");
  Reset();  // entleert das Spielfeld. Die Variablen Feld1 und Feld2 werden auf
            // 0 gesetzt.
  BuchLaden();  // Die Er�ffnungsdatenbank wird direkt zu beginn aus der Datei
                // eingelesen
  MaxInstance =
      100;  // Der Standardwert f�r die Schwierigkeitsstufe wird gesetzt. Die
            // Schwierigkeitsstufe ergibt sich aus (MaxInstance-2)/2
  Modus = 0;  // Da noch kein Spielmodus ausgew�hlt wurde
  HASHE = (HASHEvorlage *)malloc(HASHSIZE * sizeof(HASHEvorlage));
  LHASHE = (HASHEvorlage *)malloc(LHASHSIZE * sizeof(HASHEvorlage));
  DRHASHE = (DRVorlage *)malloc(DRHASHSIZE * sizeof(DRVorlage));
  ResetHash();
}

void __fastcall TForm1::FormCreate(TObject *Sender) {
  int left = 3, width = 60, Hoehe = 60, top = 3;
  for (int j = 0; j < 6; j++)
    for (int i = 0; i < 7; i++) {
      Shape[i][j] = new TShape(Panel1);
      Shape[i][j]->Left = i * left + (width + 3) * i + left -
                          (i * 3);  // Abstand zum linken Rand von GroupBox1
      Shape[i][j]->Top = j * top + (Hoehe + 3) * j + 3 -
                         (j * 3);       // Abstand zum oberen Rand von GroupBox1
      Shape[i][j]->Width = width + 3;   // Breite
      Shape[i][j]->Height = Hoehe + 3;  // H�he
      Shape[i][j]->Visible = true;
      Shape[i][j]->Enabled = true;
      Shape[i][j]->Shape = stRectangle;
      Shape[i][j]->Parent = Panel1;
      Shape[i][j]->Pen->Style = psClear;
      Shape[i][j]->Brush->Color = (TColor)0x004E4E4E;

      ImageSpielfeld[i][j] = new TImage(Panel1);
      ImageSpielfeld[i][j]->Left = i * left + width * i + left +
                                   1;  // Abstand zum linken Rand von GroupBox1
      ImageSpielfeld[i][j]->Top =
          j * top + Hoehe * j + 4;  // Abstand zum oberen Rand von GroupBox1
      ImageSpielfeld[i][j]->Width = width;   // Breite
      ImageSpielfeld[i][j]->Height = Hoehe;  // H�he
      ImageSpielfeld[i][j]->Visible = true;
      ImageSpielfeld[i][j]->Enabled = true;
      ImageSpielfeld[i][j]->Parent = Panel1;
      ImageSpielfeld[i][j]->Picture = Empty;
      ImageSpielfeld[i][j]->OnMouseDown =
          ImageClick;  // Wenn auf eines der Shapes geklickt wird, soll das
                       // Ereignis ShapeClick ausgel�st werden
    }
  StatusBar1->Panels->Items[0]->Width =
      Form1->Width / 2 -
      10;  // Panels in der StatusBar sollen in der Mitte geteilt werden
  ProgressBar1->Parent = StatusBar1;
  ProgressBar1->Top = 2;
  ProgressBar1->Left = 2;
  ModusEinrichten(ComputerSpieler1);
}

void TForm1::ModusEinrichten(TObject *Sender) {
  /*Es wird, je nachdem was f�r ein Sender �bergeben wird, einer der Vier Modi
  eingerichtet. Es gibt die Modi Spieler-Spieler, Computer-Spieler,
  Spieler-Computer, Netzwerkspiel.*/
  Reset();        // Das Spielfeld wird zur�ckgesetzt.
  ResetGrafik();  // Alle Shapes bekommen wieder die Farbe Wei�
  /*Zun�chst alle Haken im Men�punkt Spiel entfernen:*/
  SpielerSpieler1->Checked = false;
  ComputerSpieler1->Checked = false;
  SpielerComputer1->Checked = false;
  Netzwerkspiel1->Checked = false;
  StatusBar1->Panels->Items[1]->Text = "";
  GameOver = false;
  if (Sender == SpielerSpieler1) {
    SpielerSpieler1->Checked = true;
    Modus = 1;
    Spieler = 1;
    StatusBar1->Panels->Items[0]->Text = "Spieler 1 beginnt!!!";
  } else if (Sender == ComputerSpieler1) {
    ComputerSpieler1->Checked = true;
    Modus = 2;
    SteinSetzen(3, SPIELER2);
    GrafikAusgabe();
    if (MaxInstance % 2 == 1) MaxInstance++;
  } else if (Sender == SpielerComputer1) {
    SpielerComputer1->Checked = true;
    Modus = 3;
    StatusBar1->SimpleText = "Sie machen den ersten Zug!!!";
    if (MaxInstance % 2 == 0) MaxInstance--;
  } else if (Sender == Netzwerkspiel1) {
    Netzwerkspiel1->Checked = true;
    Modus = 4;
  }
}

void __fastcall TForm1::ImageClick(TObject *Sender, TMouseButton Button,
                                   TShiftState Shift, int X, int Y) {
  if (Button ==
      mbLeft)  // Nur ein Klick auf die linke Maustaste soll ausgewertet werden
  {
    int spalte;
    ProgressBar1->Visible = true;
    for (int i = 0; i < 7; i++)
      for (int j = 0; j < 6; j++)
        if (Sender ==
            ImageSpielfeld[i][j])  // Ermitteln welcher Shape der Sender
                                   // war(also auf welches Shape geklickt wurde
          spalte = i;              // Spalte ermitteln
    StatusBar1->Panels->Items[1]->Text = "Zeit: ...";
    if (Modus == 1)  // Falls es ein MEnsch-MEnsch SPiel ist
      ZweiSpieler(spalte);
    else if (Modus == 2)  // Falls es ein Computer-Mensch-Spiel ist
    {
      Panel1->Enabled = false;
      ComputerAnziehender(spalte);
      Timer1->Enabled = true;
    } else if (Modus == 3)  // Falls es ein Mensch-Computer-Spiel ist
    {
      Panel1->Enabled = false;
      // ComputerNachziehender(spalte);
      Timer1->Enabled = true;
    }
    ProgressBar1->Position = 0;
    ProgressBar1->Visible = false;
  }
}

void TForm1::ResetShapes(void) {
  /*Setzt alle Shapes(alle L�cher) des Spielfeldes wieder auf Wei�*/
  int i, j;
  for (i = 0; i < 7; i++)
    for (j = 0; j < 6; j++) Shape[i][5 - j]->Brush->Color = (TColor)0x004E4E4E;
}

void TForm1::SetShape(int x, int y) {
  Shape[x][6 - y]->Brush->Color = clSilver;
}

void TForm1::ResetGrafik(void) {
  /*Setzt alle Shapes(alle L�cher) des Spielfeldes wieder auf Wei�*/
  int i, j;
  for (i = 0; i < 7; i++)
    for (j = 0; j < 6; j++) ImageSpielfeld[i][5 - j]->Picture = Empty;
}

void TForm1::GrafikAusgabe(void) {
  /*Gibt das gesamte Spielfeld aus*/
  int i, j, stelle;
  TPicture *Spieler1, *Spieler2;
  for (i = 0; i < 7; i++)
    for (j = 0; j < 6; j++) {
      stelle = 41 - (i * 6 + j);
      if (Feld1 &
          (1i64
           << stelle))  // Ist an dieser Stelle das Bit im BitBoard gesetzt???
      {
        if (Modus == 2)
          ImageSpielfeld[i][5 - j]->Picture =
              Red;  // 5-j, da Grafik oben links anf�ngt, und nicht unten-links
        else
          ImageSpielfeld[i][5 - j]->Picture =
              Yellow;  // 5-j, da Grafik oben links anf�ngt, und nicht
                       // unten-links
      } else if (Feld2 & (1i64 << stelle)) {
        if (Modus == 2)
          ImageSpielfeld[i][5 - j]->Picture =
              Yellow;  // 5-j, da Grafik oben links anf�ngt, und nicht
                       // unten-links
        else
          ImageSpielfeld[i][5 - j]->Picture =
              Red;  // 5-j, da Grafik oben links anf�ngt, und nicht unten-links
      }
    }
}

void TForm1::ZeitmessungStop(void) {
  /*Nach der Berechnung wird die Zeit gestoppt. Bei der Berechnung wurde die
Startzeit ermittelt. Es wird die Startzeit von der Stop-Zeit abgezogen und das
Ergebnis in MSec ausgegeben!!!*/
  Word h, min, sec, msec;
  TDateTime Stopzeit, Differenz;
  Stopzeit = Time();
  Differenz = Stopzeit - Startzeit;
  DecodeTime(Differenz, h, min, sec, msec);
  StatusBar1->Panels->Items[1]->Text =
      "Zeit: " + (AnsiString)(h * 3600 + min * 60 + sec) + "s" + "   " + msec +
      "ms";
}

void __fastcall TForm1::Timer1Timer(TObject *Sender) {
  /*Nachdem der Computer einen Zug gemacht hat, wird der Timer gestartet und
  nach einigen MillSec wird diese Methode aufgerufen. Hier wird das PAnel wieder
  aktiviert, sodass der Spieler wieder einen Zug machen kann. Diese L�sung mit
  dem Timer ist notwendig, da das Panel bevor der Berechnung deaktiviert wird,
  sodass der Spieler w�hrend der Berechnung nich ausversehen auf ein Shape
  klickt und dadurch schon den n�chsten Zug gemacht hat. Nach der Berechnung
  w�rde der Zug trotz der vorherigen Deaktivierung des Panels gemacht werden, da
  erst das Panel wieder aktiviert wird und dann die Botschaften verarbeitet
  werden. Daher wird das Panel erst nach einigen MiiliSec wieder aktiviert, also
  dann, wenn die Botschaften-Warteschleife abgearbeitet ist!!!*/
  Panel1->Enabled = true;
  Timer1->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer2Timer(TObject *Sender) { FourBlink(); }

void TForm1::FourBlink(void) {
  int stelle;
  Form1->Refresh();
  if (BlinkCounter > 6) {
    Timer2->Enabled = false;
    Form1->Enabled = true;
    return;
  }
  BlinkCounter++;
  for (int i = 0; i < 7; i++)
    for (int j = 0; j < 6; j++) {
      stelle = 41 - (i * 6 + j);
      if ((BlinkFeld & (1i64 << stelle)) && (BlinkFeld & Feld1)) {
        if (Modus == 2) {
          if (BlinkCounter % 2 == 0)
            ImageSpielfeld[i][5 - j]->Picture = Empty;
          else
            ImageSpielfeld[i][5 - j]->Picture = Red;
        } else {
          if (BlinkCounter % 2 == 0)
            ImageSpielfeld[i][5 - j]->Picture = Empty;
          else
            ImageSpielfeld[i][5 - j]->Picture = Yellow;
        }
      } else if ((BlinkFeld & (1i64 << stelle)) && (BlinkFeld & Feld2)) {
        if (Modus == 2) {
          if (BlinkCounter % 2 == 0)
            ImageSpielfeld[i][5 - j]->Picture = Empty;
          else
            ImageSpielfeld[i][5 - j]->Picture = Yellow;
        } else {
          if (BlinkCounter % 2 == 0)
            ImageSpielfeld[i][5 - j]->Picture = Empty;
          else
            ImageSpielfeld[i][5 - j]->Picture = Red;
        }
      }
    }
  Form1->Refresh();
  Timer2->Interval = 400;
  if (BlinkCounter == 1) Timer2->Interval = 250;
  Timer2->Enabled = true;
}

void TForm1::Blink(int spalte, Boolean Spieler) {
  BlinkFeld = GewinnstellungReihe(spalte, Hoehe[spalte] - 1, Spieler);
  BlinkCounter = 0;
  Form1->Enabled = false;
  FourBlink();
}

void TForm1::ZweiSpieler(int spalte) {
  /*Wird ausgef�hrt, wenn es sich um ein Mensch-MEnsch SPiel handelt und der
   Spieler, der an der Reihe ist, in eine Spalte geklickt hat. Der Stein wird
   dann f�r den betreffenden Spieler gesetzt und es wird �berpr�ft, ob er mit
   diesem Zug gewonnen h�tte. Ansonsten wird der Spieler gewechselt*/
  if (GameOver) return;
  if (Spieler == 1)  // Wenn Spieler 1 an der Reihe ist
  {
    if (Hoehe[spalte] < 6)  // Wenn die Spalte noch nicht voll ist.....
    {
      SteinSetzen(spalte, SPIELER1);
      if (Gewinnstellung1(spalte, Hoehe[spalte] - 1)) {
        StatusBar1->Panels->Items[0]->Text = "Spieler1 hat gewonnen!!!";
        GameOver = true;
        Blink(spalte, SPIELER1);
      } else {
        StatusBar1->Panels->Items[0]->Text = "Spieler 2 ist an der Reihe!!!";
        Spieler = 2;
      }
    }
  } else if (Spieler == 2)  // Wenn Spieler2 an der Reihe ist
  {
    if (Hoehe[spalte] < 6)  // Wenn die Spalte noch nciht voll ist
    {
      SteinSetzen(spalte, SPIELER2);
      if (Gewinnstellung2(spalte, Hoehe[spalte] - 1)) {
        StatusBar1->Panels->Items[0]->Text = "Spieler 2 hat gewonnen!!!";
        GameOver = true;
        Blink(spalte, SPIELER2);
      } else {
        StatusBar1->Panels->Items[0]->Text = "Spieler 1 ist an der Reihe!!!";
        Spieler = 1;
      }
    }
  }
  if ((Feld1 | Feld2) == 0x3FFFFFFFFFFi64)  // Wenn das Spielfeld voll ist....
  {
    StatusBar1->Panels->Items[0]->Text = "Das Spiel endet Unentschieden!!!";
    GameOver = true;  // Damit keienr der beiden einen Stein mehr setzen kann
  }
  GrafikAusgabe();
}

void TForm1::ComputerAnziehender(int spalte) {
  /*Wird ausgef�hrt, wenn der Spieler auf ein Shape geklcikt hat und es sich um
  ein Computer-Mensch Spiel handelt. Es wird zun�chst gepr�ft, ob die Spalte, in
  die der Spieler gesetzt hat noch nicht voll ist. Dann wird der Stein f�r den
  Spieler gestezt und der Computer berechnet einen g�ltigen Zug. Die
  Berechnungs-Zeit wird gesoppt und ausgegeben!!!*/
  int x;
  if (GameOver)  // Falls das Spiel bereits vorbei ist, soll der Spieler keinen
                 // Zug mehr machen d�rfen
    return;
  if (Hoehe[spalte] < 6 && Modus == 2) {
    Startzeit = Time();  // Die Startzeit setzen
    SteinSetzen(spalte, SPIELER1);
    if (Gewinnstellung1(
            spalte, Hoehe[spalte] -
                        1))  // Falls der Spieler mit seinem Zug gewonnen hat
    {
      StatusBar1->Panels->Items[0]->Text = "Sie Haben das Spiel gewonnen!!!";
      GameOver = true;
      Blink(spalte, SPIELER1);
      return;
    }
    GrafikAusgabe();
    ResetShapes();
    Form1->Refresh();
    // ResetHash();
    if ((Feld1 | Feld2) == 0x3FFFFFFFFFFi64)  // Falls das Spielfeld Voll ist
    {
      StatusBar1->Panels->Items[0]->Text = "Das Spiel endet Unentschieden!!!";
      GameOver = true;  // Damit keienr der beiden einen Stein mehr setzen kann
      return;
    }
    if (SpielBeenden2())  // Es wird vor dem Zug des Computers bereits geschaut,
                          // ob er gewinnen kann, wodurch dann nachher ein
                          // weiterer Zug des Spielers verhindert werden kann
      GameOver =
          true;  // Wird auf true gesetzt, da bei Gewinn das Spiel vorbei ist
                 // und der Spieler keine weiteren Z�ge machen k�nnen soll
    else
      StatusBar1->Panels->Items[0]->Text =
          "Warten Sie bitte, das Programm rechnet...";
    if (BrettCount() < 8) {
      x = WurzelMethodeStartComputerAnziehender(0, -9999, 9999,
                                                HoeheErmitteln());
      SteinSetzen(x, SPIELER2);
      SetShape(x, Hoehe[x]);
    } else {
      // Feld1 = Feld2 = 0i64;
      // HoeheErmitteln();
      ProgressBar1->Position = 0;
      InstanceVoll = 42 - BrettCount();
      MaxInstance = 100;
      HoeheErmitteln();
      x = WurzelMethodeComputerAnziehender(0, -9999, 9999, HoeheErmitteln());
      SteinSetzen(x, SPIELER2);
      SetShape(x, Hoehe[x]);
    }
    GrafikAusgabe();
    StatusBar1->Panels->Items[0]->Text = "Sie sind an der Reihe!!!";
    if (GameOver) {
      Blink(x, SPIELER2);
      StatusBar1->Panels->Items[0]->Text =
          "Sie haben das Spiel leider verloren!!!";
    }
    ZeitmessungStop();
  }
}
