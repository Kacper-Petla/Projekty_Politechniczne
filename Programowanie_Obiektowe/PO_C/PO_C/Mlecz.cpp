#include "Mlecz.h"

Mlecz::Mlecz(int x, int y, Swiat& swiat) : Roslina(MLECZ_SILA, x, y, swiat) {}

void Mlecz::akcja()
{
    wiek++;

    for (int i = 0; i < MLECZ_PROBY_ROZPRZESTRZENIENIA; i++)
    {
        if (rand() % 100 < PRAWDOPODOBIENSTWO_ROZPRZESTRZENIANIA)
        {
            rozmnazanie();
        }
    }
}

char Mlecz::rysowanie() const
{
    return MLECZ;
}

Organizm* Mlecz::stworzPotomka(int x, int y) const
{
    return new Mlecz(x, y, swiat);
}