#include "Wilk.h"

using namespace std;

Wilk::Wilk(int x, int y, Swiat& swiat) : Zwierze(WILK_SILA, WILK_INICJATYWA, x, y, swiat) {}

char Wilk::rysowanie() const
{
    return WILK;
}

Organizm* Wilk::stworzPotomka(int x, int y) const
{
    return new Wilk(x, y, swiat);
}