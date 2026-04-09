#include "Zolw.h"

Zolw::Zolw(int x, int y, Swiat& swiat) : Zwierze(ZOLW_SILA, ZOLW_INICJATYWA, x, y, swiat) {}

void Zolw::akcja()
{
    wiek++;

    if (rand() % 4 == 0)    //25% na ruch
    {
        int dx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
        int dy[8] = { 1, 1, 1, 0, 0, -1, -1, -1 };

        while (true)
        {
            int dir = rand() % 8;

            int newX = x + dx[dir];
            int newY = y + dy[dir];

            if (swiat.czyNaPlanszy(newX, newY))
            {
                przesun(newX, newY);
                break;
            }
        }
    }
    else
    {
        swiat.setLog(swiat.getLog() + string(1, rysowanie()) + " (" + to_string(x) + ", " + to_string(y) + ", " + to_string(sila) + ") nie rusza sie " + "\n");
    }
}

void Zolw::kolizja(Organizm& atakujacy)
{
    if (typeid(*this) == typeid(atakujacy))          //rozmnarzanie
    {
        rozmnazanie();
    }
    else
    {
        if (atakujacy.getSila() < ZOLW_ODPIERA)     //zolw odpiera atak
        {
            swiat.setLog(swiat.getLog() + string(1, rysowanie()) + " (" + to_string(x) + ", " + to_string(y) + ", " + to_string(sila) + ") odparl atak " + string(1, atakujacy.rysowanie()) + " (" + to_string(atakujacy.getX()) + ", " + to_string(atakujacy.getY()) + ", " + to_string(atakujacy.getSila()) + ")" + "\n");
        }
        else
        {
            atak(atakujacy);
        }
    }
}

char Zolw::rysowanie() const
{
    return ZOLW;
}

Organizm* Zolw::stworzPotomka(int x, int y) const
{
    return new Zolw(x, y, swiat);
}