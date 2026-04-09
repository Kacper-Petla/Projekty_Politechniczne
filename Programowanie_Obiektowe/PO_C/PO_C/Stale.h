#pragma once
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <typeinfo>
#include "Konsola.h"

using namespace std;

enum Typ_Organizmu
{
	ZWIERZE, ROSLINA
};

//Konsola

#define GORA 'w'
#define DOL 's'
#define PRAWO 'd'
#define LEWO 'a'
#define UMIEJETNOSC 'u'
#define WYJDZ 'q'
#define ZAPISZ 'z'
#define WCZYTAJ 'x'
#define NOWA_GRA 'n'
#define LOG 'l'

//Zwierzeta

#define CZLOWIEK '@'
#define CZLOWIEK_SILA 5
#define CZLOWIEK_INICJATYWA 4

#define OWCA 'O'
#define OWCA_SILA 4
#define OWCA_INICJATYWA 4

#define WILK 'W'
#define WILK_SILA 9
#define WILK_INICJATYWA 5

#define LIS 'L'
#define LIS_SILA 3
#define LIS_INICJATYWA 7

#define ZOLW 'Z'
#define ZOLW_SILA 2
#define ZOLW_INICJATYWA 1
#define ZOLW_ODPIERA 5

#define ANTYLOPA 'A'
#define ANTYLOPA_SILA 4
#define ANTYLOPA_INICJATYWA 4

//Rosliny

#define PRAWDOPODOBIENSTWO_ROZPRZESTRZENIANIA 10 //%
#define INICJATYWA_ROSLIN 0

#define TRAWA 't'
#define TRAWA_SILA 0

#define MLECZ 'm'
#define MLECZ_SILA 0
#define MLECZ_PROBY_ROZPRZESTRZENIENIA 3

#define GUARANA 'g'
#define GUARANA_SILA 0
#define GUARANA_BOOST_SILA 3

#define WILCZA_JAGODA 'j'
#define WILCZA_JAGODA_SILA 99

#define BARSZCZ_SOSNOWSKIEGO 'b'
#define BARSZCZ_SOSNOWSKIEGO_SILA 10

//Inne

#define BOOST_SILY 10
#define CZAS_TRWANIA_UMIEJETNOSCI 5
#define CZAS_REGENERACJI_UMIEJETNOSCI 5

#define PROCENT_PLANSZY 0.4
#define LICZBA_ORGANIZMOW 10

#define CYBER_OWCA 'C'

