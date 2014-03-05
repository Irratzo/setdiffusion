//Entwurf2 -> Entwurf3: siehe Entwurf2 NOTE08, NOTE09, und im Text S 70,71

//NOTEXX < 06, TESTXX < 03, ALTXX < 03 wurden gelöscht.
//Der Zähler wird fortgesetzt
//AUFRÄUMXX wurden komplett übernommen. Der Zähler wurde fortgesetzt.

//Entwurf3
//Kommentare und Testroutinen sind mit //NOTEXX und //TEST_EntwurfX_XX gekennzeichnet
//(und auf cpp und header verteilt)
//Ist ein solcher auf mehrere Stellen verteilt, hängt ein - an
//jeder TESTXX(-) muss ein SUCCESS oder FAIL mit NOTEYY haben
//
//AUFRÄUMXX: Dinge, die initialisiert wurden oder sonstwas, und zur gelungenen Schleifen-Iteration an einer noch unbekannten Stelle weiter unten aufgeräumt werden müssen
//jedes AUFRÄUMXX(-) muss ein REMEDIED oder UNREMEDIED haben.
//
//ALTXX: durch Änderungen obsolet gewordene Codeteile
//
//COMPILEXX: Debugging: Compilerfehler
//steht an der Stelle in main, bis zu der das Programm ausgeschreiben wurde; mit Kommentar, was alles da stand
//immer Fehlermeldung mitangeben
//jedes COMPILEXX(-) muss ein RESOLVED OR UNRESOLVED haben
//
//GUARDXX: Code, der bei bestimmten Eingaben zu Fehlern führen KÖNNTE,
//         falls GUARDXX eintritt
//SEHR WICHTIG! siehe zB NOTE26 

//ANALYSEXX: Notizen zur Fehlerbehebung über die cout-Analyse (nach NOTE27)
//jedes AnalyseXX hat ein RESOLVED or UNRESOLVED

#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <climits> //für UINT_MAX, siehe unten
#include <boost/lexical_cast.hpp> //um motherrow anfangs mit Zahlen zu füllen, s.u.

#include "lerntageplaner.h"

using namespace std;


//____________________________________________________________
//Nötige Deklarationen für Eingabe und Vorbereitung
//START

typedef pair<string,unsigned int> onegroupdef;
typedef vector<onegroupdef> groupdef;
typedef vector <string> motherrowdef;

//ANALYSE04-: Die folgenden typedef sind nötig für die Funktion balance, siehe unten
typedef pair<unsigned int, int> balancepairdef;
typedef vector<balancepairdef> balancedef;

//nötig für die Sortierung des Eingabe-Pair-Vektors nach der zweiten Spalte
bool sort_pred(const onegroupdef& elementlinks, const onegroupdef& elementrechts) {
  return elementlinks.second < elementrechts.second;
}

bool sort_pred_balance(const balancepairdef& elementlinks, const balancepairdef& elementrechts) {
  return elementlinks.second < elementrechts.second;
}



//Nötige Deklarationen für Eingabe und Vorbereitung
//FINISH
//____________________________________________________________




//____________________________________________________________
//SeatRowS Deklarationen START

//Konstruktor
//SeatRowS::SeatRowS(?)
//{?}

//Dekunstruktor
//SeatRowS::~SeatRowS()
//{?}
void SeatRowS::initialize(unsigned int initiallength){
  //NOTE16-: 

  cout << "***************************" << endl;
  cout << "Entered DistOn::initialize "<< endl;
  cout << "***************************" << endl;

  //anfangs keine belegten Sitzplätze, also nur eine Reihe
  rows = 1;
  //demnach nur ein (globaler) Start (dh bzgl motherrow) bei 0
  rowstarts.push_back(0);
  //hier muss die Laufzeit-Variable 'n' (Länge motherrow, siehe main) übergeben werden
  rowlengths.push_back(initiallength);

  cout << "There are " << rows << " rows, rowstarts[0]= " << rowstarts[0] << ", rowlengths[0]= " << rowlengths[0] << endl << endl;

}

void SeatRowS::get_group_i(groupdef thegroup, unsigned int the_i){

  cout << "  *******************" << endl;
  cout << "  Entered get_group_i" << endl;
  cout << "  *******************" << endl;

  //lese von der Eingabe in der respektiven Iteration der i-Schleife ein
  groupname = thegroup[the_i].first;
  peopleON = thegroup[the_i].second;

  cout << "  groupname nr. " << the_i << " is: " << groupname << endl;
  cout << "  Number of people in that group: " << peopleON << endl;

  //berechne die Verteilungsparameter

  //NOTE13-: wenn wir in der ersten Iteration (i=0) sind, gibt es nur eine freie Sitzreihe, also hat rowstarts nur einen Eintrag, und rowlength[0] = 0

  //NOTE17: habe plast (Entwurf2) durch rows ersetzt (siehe auch Kommentar Text S.73).
  //NOTE26: Problem: was, wenn peopleON < rows? Kann das passieren?
  //Dann müssten die Einer-Pakete (also peoplebase) auf diese rows gleichverteilt werden. Also in jede rows/peopleON - te Reihe wird eine Person eingefügt.
  //Nur damit beschäftigen, falls es Probleme gibt, siehe Kommentar im Text S.74.
  //Aber GUARDs einführen.

  //GUARD01
  if (peopleON < rows)
    cout << "  GUARD01: (peopleON < rows) siehe Entwurf3 NOTE26, Text S.73" << endl;

  cout << "  Now calculating the parameters for distribution ON the " << rows << " rows." << endl;

  peoplebase = peopleON / rows;

  peoplerest = peopleON % rows;

  if (peoplerest == 0)
    restdistance = 0;
  else
    restdistance = ceil( (double)rows / (double)peoplerest );

  if (restdistance == 0)
    peoplerestrest = 0;
  else
    peoplerestrest = peopleON % (rows / restdistance);

  peopledivrest = peoplerest - peoplerestrest;
  
  if (peoplerestrest == 0)
    restrestdistance = 0;
  else
    restrestdistance = ceil( (double)rows / peoplerestrest);


  cout << "  peoplebase = p/rows = " << peoplebase << endl;
  cout << "  peoplerest = p%rows = " << peoplerest << endl;
  cout << "  peopledivrest = pR-pRR = " << peopledivrest << endl;
  cout << "  restdistance = cl((real)rows/pR) = " << restdistance << endl;
  cout << "  peoplerestrest = p % (rows/Rdistance) = " << peoplerestrest << endl;
  cout << "  restrestdistance = cl((real)rows/pRR) = " << restrestdistance << endl << endl;

  
}

void SeatRowS::add_people (unsigned int addp, vector<SeatRow>& therow, unsigned int the_k) {

  cout << "    **************************" << endl;
  cout << "    Entered private add_people"  << endl;
  cout << "    **************************" << endl;

  //Verteilung der Leute AUF die freien Sitzreihen, dh Übergabe an die row[k]-Objekte
  //Unklar, ob das eine SeatRowS-Funktion oder eine SeatRow-Funktion sein soll

  therow[the_k].peopleIN += addp;
  cout << "    Old people in that row: " << therow[the_k].peopleIN-addp << "New count:  " << therow[the_k].peopleIN << endl << endl;

}

void SeatRowS::balance_people(vector<SeatRow>& therow, unsigned int the_a){
  //ANALYSE04-: nötig wegen (siehe Eintrag in paste_people),(Text S. 76 ff.)

  cout << "    *********************************" << endl;
  cout << "    Entered private balance_people"  << endl;
  cout << "    *********************************" << endl;

  unsigned int c=0;
  unsigned int m;
  //bilden zusammen einen absteigenden(!) modulo-Laufindex (dh ad infinitum)

  balancepairdef onepair;
  balancedef sorted;
  //speichert rowlength[k] mit seinem Index in aufsteigend(!) Sortierung nach den Werten

  for (unsigned int k=0; k< rows; k++) {
    if ((static_cast<int>(rowlengths[k]) - static_cast<int>(therow[k].peopleIN)) > 0) {
      //akzeptiere nur solche reihen, wo es noch freie plätze gibt

      onepair.first = k;
      //in first ist also der originale Index gespeichert
      onepair.second = static_cast<int>(rowlengths[k]) - static_cast<int>(therow[k].peopleIN);
      //in second ist also gespeichert, wie viele freie sitzplätze es pro reihe noch gibt
      sorted.push_back(onepair);

    }
  }

  sort(sorted.begin(), sorted.end(), sort_pred_balance);
  //sortiert

  cout << "    The peopleINs: " << endl << "    ";
  for (unsigned int k = 0; k < rows; k++)
    cout << therow[k].peopleIN << ' ';
  cout << endl << "    The rowlengths: " << endl << "    ";
  for (unsigned int k = 0; k < rows; k++)
    cout << rowlengths[k] << ' ';
  cout << endl << endl <<"    The sorted rowlengths-peopleIN vector looks like this:" << endl;
  cout << "      Note: only those rows with free seats were accepted." << endl << "    ";
  for (unsigned int k = 0; k < sorted.size(); k++)
    cout << sorted[k].second << ' ';
  cout << "   the differences" << endl << "    ";
  for (unsigned int k = 0; k < sorted.size(); k++)
    cout << sorted[k].first << ' ';
  cout << "   the original indices" << endl << "    Now shifting the people load:" << endl;



 while (therow[the_a].peopleIN > rowlengths[the_a]){
   //solange in der problemreihe mehr leute als sitze sind

   m = sorted.size() - 1 - (c % rows);
   //Bsp rows=3. m = 2 1 0 2 1 0 2 1 etc.
   //Weil die größten Einträge in sorted.second ja hinten sind.

   cout << "      While-loop, c = " << c << ", m = " << m <<  endl;

   cout << "      Check if rowlengths[sorted[m].first] > therow[sorted[m].first].peopleIN" << endl;
   cout << "      rowlengths[" << sorted[m].first << "] = "<< rowlengths[sorted[m].first] << endl;
   cout << "      therow[" << sorted[m].first << "].peopleIN = " << therow[sorted[m].first].peopleIN << endl;
   if (rowlengths[sorted[m].first] > therow[sorted[m].first].peopleIN){

     //und solange in allen 'aushelfer'-reihen mehr sitze als leute sind:

     therow[the_a].peopleIN--;
     //nehme eine person aus der problemreihe
     therow[sorted[m].first].peopleIN++;

     cout << endl << "      It is! So the people in problem row nr. " << the_a << " decreased to " << therow[the_a].peopleIN << endl;
     cout << "      And the people in helper row nr. " << sorted[m].first << " increased to " << therow[sorted[m].first].peopleIN << endl;
     //und füge sie in einer anderen reihe ein, wobei die mit den meisten freien sitzen als erstes dran kommen
     c++;
     //yay, C++! bzw: und erhöhe den Zähler für den Modulo-Index
     }

 }
 cout << endl << "    After balancing: "<< endl;
  cout << "    The peopleINs: " << endl << "    ";
  for (unsigned int k = 0; k < rows; k++)
    cout << therow[k].peopleIN << ' ';
  cout << endl << "    The rowlengths: " << endl << "    ";
  for (unsigned int k = 0; k < rows; k++)
    cout << rowlengths[k] << ' ';
  cout << endl << endl;


}


bool SeatRowS::row_taken(std::vector<SeatRow>& therow, std::vector<std::string>& themotherrow,unsigned int the_k){
  //Übernommen von S.47,54; korrigiert

  cout << "    **************************" << endl;
  cout << "    Entered private row_taken" << endl;
  cout << "    **************************" << endl;

  if(therow[the_k].peopleIN == rowlengths[the_k]) {
    cout << "    In the row " << the_k << ", there are " << rowlengths[the_k] << ", pardon, " << therow[the_k].length << " seats, and " << therow[the_k].peopleIN << " people." << endl;    
    cout << "    So this row is going to be erased from DistIn, rowstarts, rownlengths" << endl;

    for (unsigned int k2 = 0; k2 < therow[the_k].peopleIN; k2++)
      themotherrow[ rowstarts[the_k]+k2 ] = groupname;

    therow.erase(therow.begin()+the_k);
    rowstarts.erase(rowstarts.begin()+the_k);
    rowlengths.erase(rowlengths.begin()+the_k);

    cout << "    the motherrow now looks like this:" << endl;
    for (unsigned int y=0; y < themotherrow.size(); y++)
      cout << themotherrow[y] << ' ';
    cout << endl << endl;

    return true;

  }
  else{
    cout << "    Every row has fewer people in it than seats." << endl << endl;
    return false;
  }
}

void SeatRowS::paste_people(std::vector<SeatRow>& therow, std::vector<std::string>& themotherrow) {

  cout << "  *********************" << endl;
  cout << "  Entered paste_people " << endl;
  cout << "  *********************" << endl;

  cout << "  All peopleIN are set to peoplebase = " << peoplebase << endl;

  unsigned int k = 0;
  bool rowtaken;


  cout << "  The peopleIN for all rows DistIN-rows now are this: " << endl << "  ";
  //Verteilen der restfreien Leute-Anteile auf alle freien Sitzreihen
  for (k=0; k < rows; k++) {
    therow[k].peopleIN = peoplebase;
    cout << therow[k].peopleIN << ' ';
    //NOTE10: Hier wäre auch add_people() möglich gewesen. Es müsste egal sein, was ich nehme, weil die row[k] ja gelöscht und am Anfang neu initialisiert werden.
    //AUFRÄUM02: nicht vergessen zu prüfen! (In diesem Fall wäre add_people überflüssig?
    //UNREMEDIED
  }cout << endl;

  //ANALYSE04-: Folgendes passierte hier für Eingabe 3,5,9,12 bei i=3:
  //therow[k].peopleIN  2 2 2 2 2 2
  //rowlengths[k]       1 2 3 3 2 1
  //Um dieses Ungleichgewicht im allgemeinen Fall aufzulösen, gibts jetzt die zusätzliche private-Funktion balance_people(), prüft [k].peopleIn > rowlength[k]
  //row_taken()                prüft danach     [k].peopleIn == rowlength[k], und 
  //korrigiert im Fall.

  for (k=0; k < rowlengths.size(); k++) {
    if (peoplebase > rowlengths[k])
      balance_people(therow, k);
  }

  //Prüfen, ob eine der oben besetzten (hier alle) Reihen voll ist
  //In diesem Fall füllt rowtaken() an entsprechender Stelle die motherrow
  //Untendran wird die Reihenanzahl und deren Index angepasst

  cout << "  Checking, if one of the rows hasn't got free seats left." << endl;
  k=0;

  do{

    rowtaken = row_taken(therow, themotherrow, k);
    if(rowtaken) {

      k--;
      rows--;
      cout << "  Back in paste_people: Row is full. DistOn::rows is now " << rows << endl;
    }

    k++;
  }while(k < rows);
  cout << endl << endl;
}


void SeatRowS::paste_rests(std::vector<SeatRow>& therow, std::vector<std::string>& themotherrow) {
  //übernommen von S.48

  cout << "  *******************" << endl;
  cout << "  Entered paste_rests" << endl;
  cout << "  *******************" << endl;


  //Zuerst für divrest, also der Teil von (rows mod peopleON), der restlos auf die Sitzreihen aufteilbar ist (restrest ist der Rest davon)

  //1.) Initialisiere Vektoren, die aussagen, zu welcher Sitzreihe laut Verteilungsparametern zusätzliche Leute (aus divrest, restrest) hinzukommen sollen.
  unsigned int v;

  vector<unsigned int> rowforrest, rowforrestrest;
  vector<bool> foundrowforrest, foundrowforrestrest;

  cout << "  Positions for the peopledivrest = " << peopledivrest << endl << "  ";
  for (v=0; v < peopledivrest; v++){
    rowforrest.push_back(v * restdistance);
    foundrowforrest.push_back(false);
    cout << rowforrest[v] << ' ';
  }cout << endl;

  //Diese Initialisierung der nötigen Vektoren (siehe S.48) nimmt an, dass diese SeatRowS-Member beim Aufruf der Funktion als leere Vektoren initialisiert sind.
  //AUFRÄUM01: Entsprechend müssen sie am Ende (weil das SeatRowS-Objekt nach jetzigem Kenntnisstand die i-Schleifen-Iterationen ja überlebt) gecleaned, also wieder leer werden.
  //REMEDIED: diese Vektoren existieren nur funktionsintern

  //2.) Wenn eine dieser Reihen tatsächlich existiert (sie könnte schon verschwunden sein, siehe S.48), füge die hinzugekommenen Personen (aus divrest, restrest separat, da es sich ja um unterschiedliche Reihen handelt) hinzu - und zwar einen nach dem anderen und prüfe jedesmal, ob die betreffende Reihe dadurch schon voll ist und nicht mehr betrachtet werden darf (update füllt in diesem Fall die Reihe in motherrow aus)

  unsigned int k = 0;
  bool rowtaken;


  //NOTE11: jede verschwundene Reihe hat einen niedrigeren Index als die nächtbetrachtete. Das heisst, alle nachfolgenden Vektorglieder rücken eins nach vorne, k muss verharren, rows abnehmen.
  // rowforrest und sein bool-Vektor bleiben davon unbehelligt, gerade WEIL in der verschwundenen Reihe eine Person der betrachteten Gruppe eingefügt wurde (in Entwurf2 falsch gedacht an dieser Stelle)

  do{

    //check, ob reihe k noch jemanden dazukriegt
    for (v=0; v < peopledivrest; v++) {
      if (k == rowforrest[v]) {
	//ANALYSE01-: hier stand vorher
	//       if (rowstarts[k] == rowforrest[v]) {
	//das heißt, ich hatte hier den Reihen-Index rowforrest[v] mit den globalen Startpositionen der Reihen verglichen. Denkfehler. Ich muss natürlich mit k vergleichen.
	cout << "  The row " << k << " with start " << rowstarts[k] << " is in rowforrest: add one guy:" << endl;
	add_people(1,therow,k);
	cout << "  Back in paste_rests. check if row " << k << " is full: " << endl;
	foundrowforrest[v] = true;
      }
    }

    //check, ob reihe k jetzt vollbesetzt
    rowtaken = row_taken(therow, themotherrow, k);
    if(rowtaken) {
      k--;
      rows--;
      cout << "  Back in paste_rests: Row is full. DistOn::rows is now " << rows << endl;
    }

    //mach weiter
    k++;
  }while(k < rows);
  cout << "  Back in paste_rests: If no >Row is full< appeared, no rows were erased" << endl;

  //3.) Es kann Reihen geben, auf die zusätzliche Leute verteilt werden sollen, die aber schon voll besetzt und durch updaterows aus der Betrachtung verschwunden sind. Diese wurden im Schritt 2.) ermittelt und in den bool-Vektoren aus Schritt 1.) gespeichert (false für "in Schritt 2.) nicht verwendet"). Für diese wird nun die nächstliegende freie Reihe gesucht (separat für divrest und restrest), und die Leute dort, einen nach dem anderen eingesetzt, ebenfalls mit der Nachprüfung, ob die Reihe dadurch gefüllt wurde.
  //NOTE12: Hier haben wir das Problem, das die möglicherweise verschwindende Reihe nicht nur vor, sondern auch nach der nächstbetrachteten Reihe liegen kann.
  //Es ist egal, ob das k-while oder die v-for-loop außenrum liegt.

  unsigned int nearestfreerowdistance = UINT_MAX;
  unsigned int nearestfreerow = 0;

  cout << "  The undistributed divrest is left. They are here: " << endl << "  ";
  for (v=0; v < peopledivrest; v++)
    cout << rowforrest[v] << ' ';
  cout << endl << "  ";
  for (v=0; v < peopledivrest; v++)
    cout << foundrowforrest[v] << ' ';
  cout << endl;
  cout << "  Now find the next free row for them and add them to the resp. DistIn[k]";
  cout << "  For comparison: rowstarts[k] looks like this:" << endl << "  ";
  for (k=0; k < rows; k++)
    cout << rowstarts[k] << "  ";
  cout << endl;
  cout << "  ANALYSE01-: rowstarts[k] is not important, but k itself! see code." << endl;

  for (v=0; v < peopledivrest; v++) {
    //Wenn einer der Einer-Reste noch nicht eingefügt wurde,
    if (!foundrowforrest[v]) {

      //finde eine freie Reihe, die am nächsten zur ursprünglich vorgesehenen liegt
      for (k=0; k < rows; k++) {
	//ANALYSE01-: siehe Eintrag oben. Alt:
	//	if ( abs(rowstarts[k]-rowforrest[v]) < nearestfreerowdistance ) {
	if ( abs(k-rowforrest[v]) < nearestfreerowdistance ) {

	  //ANALYSE01-: Alt:  nearestfreerowdistance = abs(rowstarts[k]-rowforrest[v]);
	  nearestfreerowdistance = abs(k-rowforrest[v]);
	  nearestfreerow = k;
	}
	cout << "  The nearestfreerow is " << k << ", so add one guy here." << endl;
      }

      //füge hier den Einer-Rest ein
      add_people(1,therow,nearestfreerow);
      cout << "  Back in paste_rests. check if row " << k << " is full: " << endl;

      //Prüfe, ob die Reihe nun voll ist. Wenn ja, passe rows an.
      rowtaken = row_taken(therow, themotherrow, nearestfreerow);
      if(rowtaken) 
	rows--;
      cout << "  Back in paste_rests: DistOn::rows is now " << rows << endl;
    }
  }



  //analog mit restrest

  //1.) finde die für die Verteilung vorgesehene Reihenindizes
  cout << endl << endl << "  Positions for the peoplerestrest = " << peoplerestrest << endl << "  ";
  for (v=0; v < peoplerestrest; v++){
    rowforrestrest.push_back(v * restrestdistance);
    foundrowforrestrest.push_back(false);
    cout << rowforrest[v] << ' ';
  }cout << endl;

  //2.) wenn sie existieren, fülle ein

  k=0;
  do{

    //check, ob reihe k noch jemanden dazukriegt
    for (v=0; v < peoplerestrest; v++) {
      //ANALYSE01-: siehe Eintrag oben. Alt:
      //      if (rowstarts[k] == rowforrestrest[v]) {
      if (k == rowforrestrest[v]) {
	cout << "  The row " << k << " with start " << rowstarts[k] << " is in rowforrestrest: add one guy:" << endl;
	add_people(1,therow,k);
	cout << "  Back in paste_rests. check if row " << k << " is full: " << endl;
	foundrowforrestrest[v] = true;
      }
    }

    //check, ob reihe k jetzt vollbesetzt
    rowtaken = row_taken(therow, themotherrow, k);
    if(rowtaken) {
      k--;
      rows--;
      cout << "  Back in paste_rests: Row is full. DistOn::rows is now " << rows << endl;
    }

    //mach weiter
    k++;
  }while(k < rows);
  cout << "  Back in paste_rests: If no >Row is full< appeared, no rows were erased" << endl;

  //3.) wenn nicht, finde die nächstelegenen

  nearestfreerowdistance = UINT_MAX;
  nearestfreerow = 0;

  cout << "  The undistributed restrest is left. They are here: " << endl << "  ";
  for (v=0; v < peoplerestrest; v++)
    cout << rowforrestrest[v] << ' ';
  cout << endl << "  ";
  for (v=0; v < peoplerestrest; v++)
    cout << foundrowforrestrest[v] << ' ';
  cout << endl;
  cout << "  Now find the next free row for them and add them to the resp. DistIn[k]";
  cout << "  For comparison: rowstarts[k] looks like this:" << endl << "  ";
  for (k=0; k < rows; k++)
    cout << rowstarts[k] << "  ";
  cout << endl;
  cout << "  ANALYSE01-: rowstarts[k] is not important, but k itself! see code." << endl;

  for (v=0; v < peoplerestrest; v++) {
    //Wenn einer der Einer-Reste noch nicht eingefügt wurde,
    if (!foundrowforrestrest[v]) {

      //finde eine freie Reihe, die am nächsten zur ursprünglich vorgesehenen liegt
      for (k=0; k < rows; k++) {
	//ANALYSE01-: siehe Eintrag oben. Alt:
	//	if ( abs(rowstarts[k]-rowforrestrest[v]) < nearestfreerowdistance ) {
	if ( abs(k-rowforrestrest[v]) < nearestfreerowdistance ) {

	  //ANALYSE01-: Alt: nearestfreerowdistance = abs(rowstarts[k]-rowforrestrest[v]);
	  nearestfreerowdistance = abs(k-rowforrestrest[v]);
	  nearestfreerow = k;
	}
	cout << "  The nearestfreerow is " << k << ", so add one guy here." << endl;
      }

      //füge hier den Einer-Rest ein
      add_people(1,therow,nearestfreerow);
      cout << "  Back in paste_rests. check if row " << k << " is full: " << endl;

      //Prüfe, ob die Reihe nun voll ist. Wenn ja, passe rows an.
      rowtaken = row_taken(therow, themotherrow, nearestfreerow);
      if(rowtaken) 
	rows--;
      cout << "  Back in paste_rests: DistOn::rows is now " << rows << endl;

    }
  }

  cout << endl << "  The rowstarts[k], rowlenghts[k] now look like this: " << endl << "  ";
  for (k=0; k < rows; k++)
    cout << rowstarts[k] << ' ';
  cout << endl << "  ";
  for (k=0; k < rows; k++)
    cout << rowlengths[k] << ' ';
  cout << endl;

  cout << endl << "  The peopleIN for all rows DistIN-rows now are this: " << endl << "  ";
  for (k=0; k < rows; k++) {
    cout << therow[k].peopleIN << ' ';
  }cout << endl;


  cout << endl << endl;

}


void SeatRowS::clean_up(unsigned int& therowstmp, std::vector<unsigned int>& therowstartstmp, std::vector<unsigned int>& therowlengthstmp, std::vector<SeatRow> therow) {
  //übernommen S.59 unten (Teil in i-Schleife nach Beendigung k-Schleife)
  //die member mit den drei nötigen reihen-informationen (anzahl, globaler start, länge) werden aktualisiert, und die temporären speicher geleert

  cout << "  *************************" << endl;
  cout << "  Entered clean_up (i-loop)" << endl;
  cout << "  *************************" << endl;

  cout << "  The old row-info: rows = " << rows << ", rowstart was " << endl << "  ";
  for (unsigned int z=0; z < rows; z++)
    cout << rowstarts[z] << ' ';
  cout << endl << "  And rowlengths was " << endl << "  ";
  for (unsigned int z=0; z < rows; z++)
    cout << rowlengths[z] << ' ';

  cout << endl << "  Now Overwriting the old rows,rowstarts,rowlengths in DistOn" << endl;
  cout << "  with the data from the tmps with the  assignment operator= ." << endl;
  rows = therowstmp;
  rowstarts = therowstartstmp;
  rowlengths = therowlengthstmp;

  cout << "  The row-info now looks like this: rows = " << rows << ", rowstart is " << endl << "  ";
  for (unsigned int z=0; z < rows; z++)
    cout << rowstarts[z] << ' ';
  cout << endl << "  And rowlengths is " << endl << "  ";
  for (unsigned int z=0; z < rows; z++)
    cout << rowlengths[z] << ' ';


  cout << endl << "  Reset the tmp storage and DistIn." << endl << endl;
  therowstmp = 0;
  therowstartstmp.clear();
  therowlengthstmp.clear();

  therow.clear();
  
  //NOTE15-: Jetzt sollte ich noch schauen, dass alle ÜBRIGEN member von SeatRowS wieder auf anfang gesetzt sind
  //das ist bereits der Fall (Sachen wie rowforrest sind funktionsgebunden). Dann geht es jetzt um die Initialisierung der drei Reihen-Werte von SeatRowS vor der Schleife als Funktion.


}



//SeatRowS Deklarationen FINISH
//____________________________________________________________



//____________________________________________________________
//SeatRow Deklarationen START

//Konstruktor
//SeatRow::SeatRow(?)
//{?}

//Dekunstruktor
//SeatRow::~SeatRow()
//{?}

void SeatRow::initialize(SeatRowS therows, unsigned int k) {

  length = therows.rowlengths[k]; 
  cout << length << ' ';

}

void SeatRow::calc_distribution() {

  cout << "      **********************************" << endl;
  cout << "      Entered calc_distribution (k-loop)" << endl;
  cout << "      **********************************" << endl;

  peopledistance = length / peopleIN;

  seatsrest = length % peopleIN;

  if (seatsrest == 0)
    seatsrestdistance = 0;
  else
    seatsrestdistance = length / seatsrest;

  cout << "      peopleIN = " << peopleIN << ", length = " << length << endl << endl;

  cout << "      peopledistance = l/p = " << peopledistance << endl;
  cout << "      seatsrest = l%p = " << seatsrest << endl;
  cout << "      seatsrestdistance = l/sR = " << seatsrestdistance << endl << endl;

  //NOTE25: im Text hab ich das hier:
  //  seatsrestdistance = peopleIN / seatsrest;
  //Das kann aber nicht stimmen: die freien Sitze müssen doch auf die ganze Länge dieser kten freien Sitzreihe verteilt werden. Deshalb hab ich peopleIN im Zähler durch length ersetzt.
}

void SeatRow::paste_people() {

  cout << "      *****************************" << endl;
  cout << "      Entered paste_people (k-loop)" << endl;
  cout << "      *****************************" << endl;

  vector<unsigned int> seatforperson(peopleIN);
  cout << "      initialized tmp vector seatforperson. now filling." << endl;

  cout << "      ";
  for(unsigned int u=0; u< peopleIN; u++){
    seatforperson[u] = u * peopledistance;
    cout << seatforperson[u] << ' ';
  }

  cout << endl << "      now setting these positions in seattaken to true" << endl << "      ";
  for (unsigned int l=0; l< length; l++){
    seattaken.push_back(false);
    for(unsigned int u = 0; u < peopleIN; u++) {
      if (l == seatforperson[u])
	seattaken[l] = true;
    }
    cout << seattaken[l] << ' ';
  } cout << endl << endl;
  //Da es hier nicht passieren kann, dass rowlength[k] = peopleIN, muss ich es auch nicht nachprüfen
}

void SeatRow::paste_rests(vector<string>& themotherrow, SeatRowS therows, unsigned int the_k) {

  cout << "      ****************************" << endl;
  cout << "      Entered paste_rests (k-loop)" << endl;
  cout << "      ****************************" << endl;

  vector<int> restseats(seatsrest);

  cout << "      seattaken size: " << seattaken.size() << endl;

  //ANALYSE02- : cout zeigte, dass seattaken nicht gecleared wurde. Bsp:
  //           i  k   length  seattaken.size()
  //           0  0   29      29
  //           1  0   9       38   usw.
  //korrigiere das mit clear() in SeatRow::clean_up(), siehe dort.

  cout << "      initialized tmp vector restseats. now filling." << endl;
  cout << "      btw: seatsrestdistance = " << seatsrestdistance << endl << "      ";
  for (unsigned int q=0; q < seatsrest; q++){
    restseats[q] = q * seatsrestdistance;
    cout << restseats[q] << ' ';
    seattaken.pop_back();
  }
  cout << endl << "      seattaken size: " << seattaken.size() << endl;

  int inscount = 1;
  //insert fügt VOR der angegebenen Position ein, ich will aber danach. Außerdem berücksichtigt inscount das Wachsen des Array-Index durch das Einfügen (alle nächstbetrachteten haben einen größeren Index als das zuletzt eingefügte)

  cout << "      now inserting these positions into seattaken:" << endl << "      ";

  for (unsigned int q=0; q < seatsrest; q++){

    seattaken.insert(seattaken.begin()+restseats[q]+inscount,false);
    inscount++;
  }

  for (unsigned int l=0; l < length; l++){
    cout << seattaken[l] << ' ';
    if (seattaken[l])
      themotherrow[therows.rowstarts[the_k]+l] = therows.groupname;
  }cout << endl << "      the motherrow now looks like this:" << endl << "      ";

  for (unsigned int r=0; r < themotherrow.size(); r++)
    cout << themotherrow[r] << ' ';
  cout << endl << endl;
}


void SeatRow::clean_up(unsigned int& therowstmp, vector<unsigned int>& therowstartstmp, vector<unsigned int>& therowlengthstmp, SeatRowS therows, unsigned int the_k) {
  //von S.59 oben (der Teil in der k-Schleife)
  //Die (anfangs leeren, siehe SeatRowS::clean_up) tmp-Vektoren werden mit den neuen Sitzreihen-Daten gefüllt (call by ref!). Weiter gehts bei SeatRowS::clean_up

  cout << "      *************************" << endl;
  cout << "      Entered clean_up (k-loop)" << endl;
  cout << "      *************************" << endl;
  cout << "      seattaken size is " << seattaken.size() << endl;
  cout << "      The loop runs from l=0 to length= " << length << endl;

  //NOTE27: 
  //COMPILE03-: die Schleife lief nicht, weil da stand
  //  for (unsigned int l=0; l < therows.rows; l++) {
  //in der ersten Iteration war therows.rows = 1.
  //Ich glaube, ich habe hier (im Text S.59 schon) einen Denkfehler gemacht. Die Schleife muss ja einfach einmal seattaken laufen. Also hab ichs durch length ersetzt.

  //ANALYSE03-: Folgende Situation gabs: Eingabe 3,5,9,12. In i=2,k=0 führte das zu
  // seattaken     1 0 1 1
  // rowstartstmp    3 0
  // rowlengthstmp   1 0
  //Der letzte Geisteintrag rührte daher, dass ich in der Fallunterscheidung auf den tmp-Vektoren im Fall [0 1] die Operation push_back(0) ausführte.
  //Wenn aber danach nur noch [1 1]-Situationen folgen, bleibt dieser Eintrag leer.
  //Lösung: Ich nehme das push_back direkt in die [1 0]-Situation rein.
  //NOTE28: Ich glaube, rowstmp++ bleibt aber an der selben Stelle, [0 1].
  //Okay, das ging total daneben: in motherrow werden jetzt Werte überschrieben!
  //Belasse es beim alten und korrigiere nach, indem ich am Ende der Fallunterscheidung alle 0-Einträge aus den tmp-Vektoren erase.

  for (unsigned int l=0; l < length; l++) {
    //    cout << "      Loop iteration " << l << endl;

    if (l == 0) {
      //      cout << "      l==0: push_back and continue" << endl;
      //      cout << "      therowstmp = " << therowstmp << endl;
      therowstartstmp.push_back(0);
      therowlengthstmp.push_back(0);
      continue;
    }
    else
      {

	if (!seattaken[l]) {
	  //	  cout << "      Current Seat " << l << " is free -> free row region. Examine last seat:" << endl;
	  if (seattaken[l-1]) {
	    //	    cout << "      Last Seat " << l-1 << "is taken -> write into tmp" << endl;

	    	    therowstartstmp[therowstmp]=therows.rowstarts[the_k] + l;
	    	    therowlengthstmp[therowstmp]++;

	    //ANALYSE03-: Neu, aber führt zu Fehlern: (siehe Eintrag oben)
	    //	    therowstartstmp.push_back(therows.rowstarts[the_k] + l);
	    //	    therowlengthstmp.push_back(0);
	    //	    therowlengthstmp[therowstmp]++;
	    //	    	    therowstmp++;

	  }

	  else{
	    //	    cout << "      Last Seat " << l-1 << "is free -> just increase tmp length" << endl;
	    therowlengthstmp[therowstmp]++;
	  }
	}

	else{
	  //	  cout << "      Current Seat " << l << "is taken. Examine last seat:" << endl;
	  if (!seattaken[l-1]){
	    //	    cout << "      Last Seat " << l-1 << "is free. push tmp and increase counter"<< endl;
	            therowstartstmp.push_back(0);
	    	    therowlengthstmp.push_back(0);
	    	    	    therowstmp++;
               //ANALYSE03-: Neu: die obigen drei raus, aber führt zu Fehlern (siehe oben)
	  }
	}

      }
  }


  //ANALYSE02-: seattaken muss gecleared werden, siehe Eintrag oben.
  seattaken.clear();

  //ANALYSE03-: brute-force-Lösung: lösche alle 0-Einträge aus den tmp-Vektoren (siehe oben)
  //Dabei gehe ich von hinten nach vorne, damit ich den Laufindex nicht anpassen muss
  //Wenn die Länge 0 ist, muss ich nicht mehr nachprüfen (rowstartstmp[k] = 0 kann dagegen durchaus legitim sein!)
  unsigned int k = 0;

  do{
if (therowlengthstmp[k] == 0) {
      therowstartstmp.erase(therowstartstmp.begin()+k);
      therowlengthstmp.erase(therowlengthstmp.begin()+k);
      therowstmp--;
      k--;
    }
 k++;
  }while(k < therowstartstmp.size());
  //ANALYSE03-: Version, die nicht funktionierte (verm. weil therowstmp-- vergessen, egal
  // for (unsigned int k = (unsigned int)therowstartstmp.size()-1 ; k > 0; k--) {
  // cout << "loop: k = " << k << endl;
  //   if (therowlengthstmp[k] == 0) {
  //     therowstartstmp.erase(therowstartstmp.begin()+k);
  //     therowlengthstmp.erase(therowlengthstmp.begin()+k);
  //   }
  // }

  therowstmp++;
  //weil es in der Schleife ein Array-Iterator war
  
  cout << "      Check whether the temporary storage ist correct" << endl;
  cout << "      (compare with cout output paste_rests of motherrow" << endl;
  cout << "      rowlengthstmp size is " << therowlengthstmp.size() << endl;
  cout << "      rowstartstmp size is " << therowstartstmp.size() << endl;
  cout << "      rowstmp = " << therowstmp << endl;

  cout << "      rowstartstmp looks like this:" << endl << "      ";
  for (unsigned int g=0; g < therowstartstmp.size(); g++){
    cout << therowstartstmp[g] << ' ';
  }

  cout << endl << "rowlengthstmp looks like this:" << endl << "      ";
  for (unsigned int g=0; g < therowlengthstmp.size(); g++){
    cout << therowlengthstmp[g] << ' ';
  }cout << endl << endl << endl;

}

//SeatRow Deklarationen FINISH
//____________________________________________________________




int main () {


  //____________________________________________________________
  //Einlesen der Eingabe, Vorbereitung auf Betreten der i-Schleife
  //START

  unsigned int m;
  //Die Anzahl Gruppen, Laufindex i
  unsigned int n = 0;
  //Die Summe der Leute aller Gruppen, und Länge des Ausgabevektors motherrow, Laufindex j

  cout << "________________________________________" << endl;
  cout << "               INPUT                    " << endl;
  cout << "________________________________________" << endl;

  cin >> m;

  groupdef group;
  onegroupdef onegroup;

  for (unsigned int i=0; i<m; i++) {

    cin >> onegroup.first;
    cin >> onegroup.second;
    n+=onegroup.second;

    group.push_back(onegroup);
    
  }

  cout << "The motherrow looks like this: " << endl;
  motherrowdef motherrow(n);
  for (unsigned int iforcout = 0; iforcout < n; iforcout++) {
    motherrow[iforcout] = boost::lexical_cast<string>(iforcout+1);
    cout << motherrow[iforcout] << ' ';
  } cout << endl;
  //Der Vektor für die Endausgabe. Die for-Schleife füllt motherrow mit Zahlen, um mit cout die richtige Positionierung von Einfüllungen zu überprüfen.

  sort(group.begin(), group.end(), sort_pred);
  //Aufsteigendes Sortieren der Gruppen nach zweiter Spalte, realisiert mit predicate wie von bit.ly/OTF8p1

  cout << "Sorted, the groups look like this: " << endl;
  for (unsigned int i=0; i<m; i++) {
    cout << group[i].first << ' ';
    cout << group[i].second << endl;
  } cout << endl;

  //initialisiere das SeatRowS-Objekt, und einen (leeren) SeatRow-Vektor
  SeatRowS DistOn;
  DistOn.initialize(n);

  vector<SeatRow> DistIn;
  SeatRow onerow; //leeres SeatRow-Objekt für das Füllen des obigen SeatRow-Vektors mittels push_back in der i-Schleife
  //'DistOn' steht für 'Verteile Leute AUF die Sitzreihen'
  //'DistIn' steht für 'Verteile Leute INnerhalb der jeweiligen Sitzreihe'

  //NOTE24-: Alte Version war: DistIn in i-Schleife leer mit bekannter Länge initialisieren, nach clean_ups löschen. Neue Version: vor der i-Schleife initialisieren und mit push_back und clean() arbeiten.

  //erstelle Temporärspeicher für die immer kleinder werdenden freien Sitzreihen
  //werden in den beiden clean_up-Funktionen gefüllt, bzw. geleert.
  unsigned int rowstmp = 0; //Anzahl
  vector<unsigned int> rowstartstmp; //globale Startposition (bzgl motherrow)
  vector<unsigned int> rowlengthstmp; //Länge

  cout << "Finished INPUT" << endl << endl << endl << endl;

  //Einlesen der Eingabe, Vorbereitung
  //FINISH
  //____________________________________________________________




  //____________________________________________________________
  //i-Schleife (Verteilung der Leute AUF die Sitzreihen)
  //vor Betreten der k-Schleife
  //START

  unsigned int i;

  for (i=0; i < m; i++) {

    cout << "  ________________________________________" << endl;
    cout << "  ----------------------------------------" << endl;
    cout << "          i-loop:  i = " << i << endl;
    cout << "  ________________________________________" << endl;
    cout << "  ----------------------------------------" << endl;
  


    //setze aktuelle Gruppenwerte ein und berechne die Verteilungsparameter
    DistOn.get_group_i(group,i);
    //COMPILE01-: Diston initialize(), get_group_i(); liegt an get_group_i()
    //Meldung: Floating point exception (core dumped)
    //RESOLVED
    //NOTE18: wieso floating point? ich hab doch nur unsigned int...
    //Aha: Es war x/0 in mehreren Parametern. 'if(0) set 0'-Konditionen zugefügt. 
  
    //vector<SeatRow> DistIn(DistOn.get_rows());
    //Bevor irgendwas gemacht wird, muss der SeatRow-Vektor initialisiert werden, weil er für die folgenden DistOn-Funktionen als Speicherplatz benötigt wird.

    //NOTE24-: (zu obigem) Alte Version war: DistIn in i-Schleife leer mit bekannter Länge initialisieren, nach clean_ups löschen. Neue Version: vor der i-Schleife initialisieren und mit push_back und clean() arbeiten. siehe nächste for-Schleife


    //NOTE19: muss hier etwas initialisiert werden?
    //Ja: length. Also neue Funktion.
    //NOTE20: Die rowstarts und lengths werden NUR in SeatRow::clean_up() geupdatet. Ich hoffe, das berücksichtigt alle Änderungen, aber sicher bin ich nicht mehr. Sonst muss ich später hierhin zurückkehren.

    cout << "  Initializing DistIn: the respective rowlengths are: " << endl << "  ";
    for (unsigned int k = 0; k < DistOn.get_rows(); k++){

      DistIn.push_back(onerow);    
      DistIn[k].initialize(DistOn,k);

    }cout << endl;
    
    //Übergebe Längen von SeatRowS an SeatRow[k]

    DistOn.paste_people(DistIn,motherrow);
    //Verteilung der Leute AUF die Sitzreihen

    DistOn.paste_rests(DistIn,motherrow);
    //Verteilung der Restleute (einzeln) AUF die entsprechenden freien Sitzreihen

    //TEST03: Klar ist jetzt motherrow noch leer
    // motherrow[12]="test";

    // for (unsigned int h = 0; h < n; h++)
    //   cout << motherrow[h] << endl;

    cout << endl;

    //i-Schleife vor Betreten der k-Schleife
    //FINISH
    //____________________________________________________________




    //____________________________________________________________
    //k-Schleife (Verteilung der Leute IN den Sitzreihen)
    //START

    //Nun hat DistOn gespeichert: wie viele freie Sitzreihen es gibt, wie lang sie sind, und wie viele Leute jeweils verteilt werden müssen. Wir können also in die k-Schleife, die Verteilung der Leute INnerhalb der freien Sitzreihen
    //Zur Visualisierung: in der i-Schleife geht das Programm "von oben nach unten".
    //In der k-Schleife geht es "von links nach rechts" die Sitzreihen durch.
    /*
      Freie Sitzreihen:
      i=0:    ________________________________________________
      k:      _________ ____ _______ ______________ __________
      i=1:    _________ ____ _______ ______________ __________
      k:       _ ___ __       __ ___  __ _ _ _ __ _  ____ ____
      ...
          
    */
    //NOTE21: ABER: paste_people und paste_rests können dazu geführt haben, dass Reihen verschwunden sind. Was muss dann alles geändert werden?
    //In SeatRows: rows, rowstarts, rowlengths, 
    //In SeatRow[k]: entsprechendes Element löschen
    //Check: Jo, alles da. Weiter.

    unsigned int k;

    for (k=0; k < DistOn.get_rows(); k++) {

      cout << "      ----------------------------------------" << endl;
      cout << "              k-loop: k = " << k << endl;
      cout << "      ----------------------------------------" << endl;

      DistIn[k].calc_distribution();
      //berechnet die Verteilungsparameter für die betrachtete Sitzreihe
      //NOTE22: Habe hier auch noch 'if(0) set 0'-Klausel eingefügt

      DistIn[k].paste_people();
      //Füllt den bool-Vektor seattaken für die betrachtete Sitzreihe

      //COMPILE02: DistIn[k].paste_people()
      //Meldung: Segmentation fault (core dumped)
      //RESOLVED
      //Trivial: Ich initialisierte den Vektor seatforperson in paste_rests() ohne Länge

      DistIn[k].paste_rests(motherrow, DistOn, k);
      //Am Ende bleibt dann idR eine längere Reihe freier Sitze übrig. Diese werden 
      //weggepoppt und in gleicher Anzahl in der verbleibenden seattaken-Reihe 
      //gleichverteilt wieder eingefügt.

      DistIn[k].clean_up(rowstmp,rowstartstmp,rowlengthstmp,DistOn,k);
      //Füllt die Temporärspeicher mit der neuen Information

      //NOTE23: ich bin mir nicht sicher, ob clean_up es tatsächlich schafft, die globalen Startpositionen bzgl motherrow zu speichern und weiterzugeben.

      //COMPILE03: DistIn[k].clean_up(...);
      //Meldung: Segmentation fault (core dumped)
      //RESOLVED (siehe unten)
      //Ah. Für l=0 hat er mit l-1 ein Problem (unsigned int, macht aber auch keinen Sinn). Also auch hier 'if(0)'-Klausel einfügen.
      //Aber seattaken hat Größe 0. Sollte aber Größe von motherrow haben im ersten Schritt. Spurensuche anhand main rückwärts von clean_up aus.
      //Auch in SeatRow::paste_rests hat er anfangs schon Länge 0, obwohl da der freie Rest hinten weggepoppt wird.
      //Habs Problem glaub in SeatRow::paste_people gefunden. Der Vektor hat als private member Länge 0, und in dieser Funktion hatte ich nicht push_back, sondern [] verwendet
      //Neues Problem: er hat size 3. das gestellte problem ist (3,5,9,12), also sollte er Länge 3+5+9+12 = 29 haben. Achso: das push_back hatte ich hier an der falschen Stelle verwendet, muss es extra machen, bzw eine loop drüber. jetzt is ok (mit cout ausgabe getestet).
      //paste_rests funktioniert auch (über Ausgabe getestet, inklusive motherrow)
      //RESOLVE: habe den Schleifenbereich von therows.rows nach length angepasst, siehe NOTE27 in clean_up. length ist die länge von seattaken (mit cout nachgeprüft).

      //COMPILE04: clean_up()
      //Meldung: Segmentation fault (core dumped)
      //Problem ist: therowstmp war nicht initialisiert. Jetzt funzts richtig (beide vektoren und rowstmp über cout mit motherrow cout von paste_rests verglichen).

    
    }

    cout << "      ----------------------------------------" << endl;
    cout << "                     Exit  k-loop             " << endl;
    cout << "      ----------------------------------------" << endl << endl << endl;

    //k-Schleife FINISH
    //____________________________________________________________




    //____________________________________________________________
    //i-Schleife 
    //Aufräumen, Vorbereiten der Ausgabe für die nächste i-Schleifen-Iteration
    //START

    DistOn.clean_up(rowstmp, rowstartstmp, rowlengthstmp, DistIn);

  }
  //i-Schleife nach k-Schleife FINISH
  //____________________________________________________________




  //____________________________________________________________
  //Aufräumen nach Beendigung der i-Schleife
  //START

  cout << "________________________________________" << endl;
  cout << "               OUPUT                    " << endl;
  cout << "________________________________________" << endl << endl;




  cout << endl << "the motherrow now looks like this:" << endl;

  for (i=0; i < motherrow.size(); i++)
    cout << motherrow[i] << endl;
  cout << endl << endl;

  cout << "Equidistribution measure: sigma = " << endl;
  //TODO

  //Aufräumen nach Beendigung der i-Schleife
  //FINISH
  //____________________________________________________________


}
