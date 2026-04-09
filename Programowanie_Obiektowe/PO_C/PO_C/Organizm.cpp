#include "Organizm.h"

Organizm::Organizm(int sila, int inicjatywa, int x, int y, Swiat& swiat, Typ_Organizmu typ) : sila(sila), inicjatywa(inicjatywa), x(x), y(y), swiat(swiat),  wiek(0), typ(typ) {}

int Organizm::getX() const { return x; }
int Organizm::getY() const { return y; }
void Organizm::setX(int newX) { x = newX; }
void Organizm::setY(int newY) { y = newY; }

int Organizm::getSila() const { return sila; }
int Organizm::getInicjatywa() const { return inicjatywa; }
void Organizm::setSila(int newSila) { sila = newSila; }
void Organizm::setInicjatywa(int newInicjatywa) { inicjatywa = newInicjatywa; }

int Organizm::getWiek() const { return wiek; }
void Organizm::setWiek(int newWiek) { wiek = newWiek; }
Typ_Organizmu Organizm::getTyp() const { return typ; }

void Organizm::setGX(int newGX) { gx = newGX; }
void Organizm::setGY(int newGY) { gy = newGY; }
int Organizm::getTmp_Sila() const { return tmp_sila; }
void Organizm::setTmp_Sila(int newTmp_Sila) { tmp_sila = newTmp_Sila; };
int Organizm::getUmiejetnosc() const { return umiejetnosc; }
void Organizm::setUmiejetnosc(int newUmiejetnosc) { umiejetnosc = newUmiejetnosc; }