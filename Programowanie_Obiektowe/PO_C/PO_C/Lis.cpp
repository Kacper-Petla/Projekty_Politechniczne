#include "Lis.h"

Lis::Lis(int x, int y, Swiat& swiat) : Zwierze(LIS_SILA, LIS_INICJATYWA, x, y, swiat) {}

void Lis::akcja()
{
    wiek++;

    int dx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    int dy[8] = { 1, 1, 1, 0, 0, -1, -1, -1 };

    while (true)
    {
        int dir = rand() % 8;

        int newX = x + dx[dir];
        int newY = y + dy[dir];

        if (swiat.czyNaPlanszy(newX, newY))
        {
            Organizm* cel = swiat.getOrganizmNa(newX, newY);
            if (cel == nullptr)         //przesunieto zwierze
            {
                swiat.setLog(swiat.getLog() + string(1, rysowanie()) + " (" + to_string(x) + ", " + to_string(y) + ", " + to_string(sila) + ") przesuwa sie na: " + to_string(newX) + ", " + to_string(newY) + "\n");

                swiat.ustawOrganizmNa(y, x, nullptr);
                x = newX;
                y = newY;
                swiat.ustawOrganizmNa(y, x, this);
            }
            else                        //zwierze zaatakowalo
            {
                if (cel->getSila() > sila)  //lis wyczul zagrozenie
                {
                    swiat.setLog(swiat.getLog() + string(1, rysowanie()) + " (" + to_string(x) + ", " + to_string(y) + ", " + to_string(sila) + ") wyczul zagrozenie" + "\n");
                }
                else
                {
                    cel->kolizja(*this);
                }
            }
            break;
        }
    }
}

char Lis::rysowanie() const
{
    return LIS;
}

Organizm* Lis::stworzPotomka(int x, int y) const
{
    return new Lis(x, y, swiat);
}