#include "Owca.h"

using namespace std;

Owca::Owca(int x, int y, Swiat& swiat) : Zwierze(OWCA_SILA, OWCA_INICJATYWA, x, y, swiat) {}

char Owca::rysowanie() const
{
    return OWCA;
}

Organizm* Owca::stworzPotomka(int x, int y) const
{
    return new Owca(x, y, swiat);
}