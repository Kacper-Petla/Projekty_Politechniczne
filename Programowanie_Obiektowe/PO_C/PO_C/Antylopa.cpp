#include "Antylopa.h"

using namespace std;

Antylopa::Antylopa(int x, int y, Swiat& swiat) : Zwierze(ANTYLOPA_SILA, ANTYLOPA_INICJATYWA, x, y, swiat) {}

void Antylopa::akcja()
{
    wiek++;

    int dx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    int dy[8] = { 1, 1, 1, 0, 0, -1, -1, -1 };

    while (true)
    {
        int dir = rand() % 8;

        int newX = x + 2*dx[dir];   //ruch o 2 pola
        int newY = y + 2*dy[dir];   //ruch o 2 pola

        if (swiat.czyNaPlanszy(newX, newY))
        {
            przesun(newX, newY);
            break;
        }
    }
}

void Antylopa::kolizja(Organizm& atakujacy)
{
    if (typeid(*this) == typeid(atakujacy))          //rozmnarzanie
    {
        rozmnazanie();
    }
    else
    {
        if (rand() % 2 == 0)    //50% szansy na ucieczke
        {
            int dx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
            int dy[8] = { 1, 1, 1, 0, 0, -1, -1, -1 };

            int indeksy[8] = { 0,1,2,3,4,5,6,7 };
            random_shuffle(indeksy, indeksy + 8);

            for (int i = 0; i < 8; i++)
            {
                int newX = x + dx[indeksy[i]];
                int newY = y + dy[indeksy[i]];

                if (swiat.czyNaPlanszy(newX, newY))
                {
                    if (swiat.getOrganizmNa(newX, newY) == nullptr)
                    {
                        swiat.setLog(swiat.getLog() + string(1, rysowanie()) + " (" + to_string(x) + ", " + to_string(y) + ", " + to_string(sila) + ") uciekl przed " + string(1, atakujacy.rysowanie()) + " (" + to_string(atakujacy.getX()) + ", " + to_string(atakujacy.getY()) + ", " + to_string(atakujacy.getSila()) + ") na pole: " + to_string(newX) + ", " + to_string(newY) + "\n");

                        swiat.ustawOrganizmNa(atakujacy.getY(), atakujacy.getX(), nullptr);
                        atakujacy.setX(x);
                        atakujacy.setY(y);
                        swiat.ustawOrganizmNa(atakujacy.getY(), atakujacy.getX(), &atakujacy);

                        x = newX;
                        y = newY;
                        swiat.ustawOrganizmNa(y, x, this);
                        return;
                    }
                }
            }

            swiat.setLog(swiat.getLog() + string(1, rysowanie()) + " (" + to_string(x) + ", " + to_string(y) + ", " + to_string(sila) + ") niezdolal uciec przed " + string(1, atakujacy.rysowanie()) + " (" + to_string(atakujacy.getX()) + ", " + to_string(atakujacy.getY()) + ", " + to_string(atakujacy.getSila()) + ")" + "\n");
            swiat.usunOrganizm(this);
        }
        atak(atakujacy);
    }
}

char Antylopa::rysowanie() const
{
    return ANTYLOPA;
}

Organizm* Antylopa::stworzPotomka(int x, int y) const
{
    return new Antylopa(x, y, swiat);
}