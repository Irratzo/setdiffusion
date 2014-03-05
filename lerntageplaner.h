//Entwurf2 -> Entwurf3: siehe Entwurf2 NOTE08, NOTE09, und im Text S 70,71

//NOTEXX < 06, TESTXX < 03, ALTXX < 03 wurden gelöscht.
//Der Zähler wird fortgesetzt
//AUFRÄUMXX wurden komplett übernommen. Der Zähler wurde fortgesetzt.

//Entwurf3
//Kommentare und Testroutinen sind mit //NOTEYY und //TEST_EntwurfX_XX gekennzeichnet
//(und auf cpp und header verteilt)
//Ist ein solcher auf mehrere Stellen verteilt, hängt ein - an
//jeder TESTXX(-) muss ein SUCCESS oder FAIL mit NOTEYY haben
//
//AUFÄUMXX: Dinge, die initialisiert wurden oder sonstwas, und zur gelungenen Schleifen-Iteration an einer noch unbekannten Stelle weiter unten aufgeräumt werden müssen
//jedes AUFRÄUMXX(-) muss ein REMEDIED oder UNREMEDIED haben.
//
//ALTXX: durch Änderungen obsolet gewordene Codeteile


#ifndef ENTWURF1_H
#define ENTUWRF1_H

class SeatRowS;
class SeatRow;

//____________________________________________________________________________
//Funktionen der (äußeren) i-Schleife (Verteilung AUF die <freien Sitzreihen)



//void updaterows2(); im Moment als Seatrow-Member-Funktion, siehe RC|F 31




//____________________________________________________________________________
// Gerüst der Klasse SeatRowS
//START

class SeatRowS {

  friend class SeatRow;

 private: //alle Variabeln

  unsigned int rows; //Anzahl der disjunkten freien Sitzreihen
  unsigned int peopleON;
  std::string groupname;

  std::vector<unsigned int> rowstarts; //ihre globalen Startpositionen iB auf motherrow
  std::vector<unsigned int> rowlengths; //ihre Längen

  //Die Verteilungsparameter
  unsigned int peoplebase;
  unsigned int peoplerest;
  unsigned int restdistance;
  unsigned int peoplerestrest;
  unsigned int peopledivrest;
  unsigned int restrestdistance;

  //Die folgenden zwei Funktionen werden nur innerhalb SeatRowS-public-Funktionen aufgerufen, sind deshalb private

  bool row_taken(std::vector<SeatRow>& therow, std::vector<std::string>& themotherrow,unsigned int the_k);
  //Prüft, ob die row[k] (durch die Funktionen paste_base, paste_rest) komplett besetzt wird und aus der Betrachtung herausfällt (gibt true zurück) - füllt in diesem Fall diese Reihe in der motherrow mit den Leuten der gerade betrachteten Gruppe
  //War im Entwurf2 die if-Kondition von update_rows_i()

  void add_people(unsigned int addp, std::vector<SeatRow>& therow, unsigned int the_k);
  //in i-Schleife (Verteilung Leute AUF die freien Sitzreihen) für die Übergabe der Leute an die Objekte verwantwortlich

  void balance_people(std::vector<SeatRow>& therow, unsigned int the_a);
  //ANALYSE04-: falls in paste_people der Fall (peoplebase > rowlength[a]) eintritt, siehe cpp


 public: 

  //Konstruktor
  //  SeatRowS (?);
  //u.a. (?) nötig, weil einige Werte mit += geschrieben und deshalb mit 0 initialisiert werden müssen

  //Dekonstruktor
  //  ~SeatRowS (?);
  //nötig wegen Konstruktor

  // alle (public) Methoden von SeatRowS

  void initialize(unsigned int initiallength);
  //NOTE16-: Für die Initialisierung der 3 Reihen-Werte vor der Schleife
  //ACHTUNG: Implizite Annahme, dass am Anfang alle Sitze frei sind!

  void get_group_i(std::vector<std::pair<std::string, unsigned int> > thegroup, unsigned int the_i);

  unsigned int get_rows(){ return rows; }

  void paste_people(std::vector<SeatRow>& therow, std::vector<std::string>& themotherrow);
  //Falls in der i-Schleife freie Sitzreihen voll besetzt werden, löscht die Funktion diese aus der Betrachtung und füllt die besetzten Plätze in der mottherrow aus 

  void paste_rests(std::vector<SeatRow>& therow, std::vector<std::string>& themotherrow);
  //siehe S.48, verteilt den PeopleRest und Restrest auf die freigebliebenen Sitzreihen. Wäre schön als Funktion (noch schöner als Member-Funktion einer Klasse SeatrowS, muss aber nicht.

  //kein Name: ev. zweite Funktion zum Verteilen auf motherrow, siehe RC|F 29.

  void clean_up(unsigned int& therowstmp, std::vector<unsigned int>& therowstartstmp, std::vector<unsigned int>& therowlengthstmp, std::vector<SeatRow> therow);
  //



};

//Gerüst der Klasse SeatRowS
//FINISH
//____________________________________________________________________________




//____________________________________________________________________________
//Geruest der Klasse SeatRow 
//(Funktionen und Variablen der (inneren) k-Schleife(Verteilung IN die freien Sitzreihen))
//START

//SeatRow stellt eine zusammenhängende Teilreihe der motherrow-Sitzreihe dar, zusammen mit einer Teilmenge der Leute aus einer Gruppe. Ihr Zweck ist es, deren Gleichverteilung IN dieser Sitzreihe, und die Ausgabe des Ergebnisses zu vereinfachen.

class SeatRow {

  friend class SeatRowS;

 private: //alle Variablen

  unsigned int length;
  unsigned int peopleIN;

  //Die Verteilungsparameter, u.a. für die Funktion calc_distribution()
  unsigned int peopledistance;
  unsigned int seatsrest;
  unsigned int seatsrestdistance;

  std::vector<bool> seattaken;
  //Länge length, Laufindex l
  //ist true, falls Person dem Sitz zugeteilt wurde - dynamisch wegen späterer Initialisierung mit row.length (?nötig?). Die Initialisierungsfunktion enthält den temporären int-Vektor seatforperson (siehe S. 50,51).

  //  std::vector<unsigned int> restseats;
  //Länge rowseatsrest, Laufindex q
  //enthält die Positionen, an denen die anfangs am Ende hängenden freien Reststize in seattaken eingefügt werden.
  //NOTE23: Überflüssig, wird in paste_rests() nur intern verwendet

  //unsigned int inscount;
  //Zähler für die public(?)-Funktion takeseats, siehe RC|O 27,30. Unklar, ob als Member nötig.



 public: 

  //Konstruktor
  //  SeatRowS (?);
  //u.a. (?) nötig, weil einige Werte mit += geschrieben und deshalb mit 0 initialisiert werden müssen

  //Dekonstruktor
  //  ~SeatRowS (?);
  //nötig wegen Konstruktor


  //alle Methoden: das Interface

  void initialize(SeatRowS therows, unsigned int k);
  //Übergibt (in einer Loop über den SeatRow-Vektor) die Längen aus dem rowstarts[]

  void calc_distribution();
  //berechnet, sobald die Werte für length und peopleIN festgesetzt sind (am Anfang der k-Schleife), die Verteilungsparameter

  bool seat_taken(unsigned int seat){ return seattaken[seat];}
  //gibt den Wert seattaken[seat] zurück (also ob dieser Sitz besetzt ist)

  void paste_people();
  //füllt den Vektor seattaken; dafür müssen die Parameter berechnet sein

  void paste_rests(std::vector<std::string>& themotherrow, SeatRowS therows, unsigned int the_k);
  //Der Teil in der k-Schleife auf S.59 (Zwischenspeichern der durch die Verteilung IN den freien Sitzreihen resultierten Änderungen an selbigen). Ob das als Member-Funktion geht, ist noch unklar, siehe u.a. S.61.

  void clean_up(unsigned int& therowstmp, std::vector<unsigned int>& therowstartstmp, std::vector<unsigned int>& therowlengthstmp, SeatRowS therows, unsigned int the_k);
  //Speichert die durch die Verteilung der Leute IN den Reihen (k-Schleife) neu entstandenen freien Sitzreihen (Anzahl, Länge und globaler Start (dh in Bezug auf motherrow)) in Temporärspeichern (die ersten drei Eingabeargumente)


};


//Geruest der Klasse SeatRow 
//FINISH
//____________________________________________________________________________





#endif
