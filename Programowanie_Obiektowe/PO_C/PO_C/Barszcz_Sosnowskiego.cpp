#include "Barszcz_Sosnowskiego.h"

Barszcz_Sosnowskiego::Barszcz_Sosnowskiego(int x, int y, Swiat& swiat) : Roslina(BARSZCZ_SOSNOWSKIEGO_SILA, x, y, swiat) {}

void Barszcz_Sosnowskiego::akcja()
{
    wiek++;

    if (rand() % 100 < PRAWDOPODOBIENSTWO_ROZPRZESTRZENIANIA)
    {
        rozmnazanie();
    }
    int dx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    int dy[8] = { 1, 1, 1, 0, 0, -1, -1, -1 };

    for (int i = 0; i < 8; i++)
    {
        int newX = x + dx[i];
        int newY = y + dy[i];

        if (swiat.czyNaPlanszy(newX, newY))
        {
            Organizm* cel = swiat.getOrganizmNa(newX, newY);
            if (cel != nullptr && cel->getTyp() == ZWIERZE && cel->rysowanie() != CYBER_OWCA)         //barszcz zabija zwierze
            {
                swiat.setLog(swiat.getLog() + string(1, rysowanie()) + " (" + to_string(x) + ", " + to_string(y) + ") zabija " + string(1, cel->rysowanie()) + " (" + to_string(cel->getX()) + ", " + to_string(cel->getY()) + ")" + "\n");
                swiat.ustawOrganizmNa(cel->getY(), cel->getX(), nullptr);
                swiat.usunOrganizm(cel);
            }
        }
    }
}

void Barszcz_Sosnowskiego::kolizja(Organizm& atakujacy)
{
    swiat.setLog(swiat.getLog() + string(1, rysowanie()) + " (" + to_string(x) + ", " + to_string(y) + ") zjedzony przez " + string(1, atakujacy.rysowanie()) + " (" + to_string(atakujacy.getX()) + ", " + to_string(atakujacy.getY()) + ")" + "\n");
    if (atakujacy.rysowanie() != CYBER_OWCA)
    {
        swiat.setLog(swiat.getLog() + string(1, atakujacy.rysowanie()) + " (" + to_string(atakujacy.getX()) + ", " + to_string(atakujacy.getY()) + ") zatruty przez " + string(1, rysowanie()) + "\n");
        swiat.ustawOrganizmNa(atakujacy.getY(), atakujacy.getX(), nullptr);
        swiat.usunOrganizm(&atakujacy);
    }
    else
    {
        swiat.setLog(swiat.getLog() + string(1, atakujacy.rysowanie()) + " (" + to_string(atakujacy.getX()) + ", " + to_string(atakujacy.getY()) + ") strawil " + string(1, rysowanie()) + "\n");
    }
    swiat.ustawOrganizmNa(y, x, nullptr);
    swiat.usunOrganizm(this);
}

char Barszcz_Sosnowskiego::rysowanie() const
{
    return BARSZCZ_SOSNOWSKIEGO;
}

Organizm* Barszcz_Sosnowskiego::stworzPotomka(int x, int y) const
{
    return new Barszcz_Sosnowskiego(x, y, swiat);
}