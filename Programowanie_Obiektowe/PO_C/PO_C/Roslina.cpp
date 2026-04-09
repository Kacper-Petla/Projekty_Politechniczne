#include "Roslina.h"

Roslina::Roslina(int sila, int x, int y, Swiat& swiat) : Organizm(sila, INICJATYWA_ROSLIN, x, y, swiat, ROSLINA) {}

void Roslina::akcja()
{
    wiek++;

    if (rand() % 100 < PRAWDOPODOBIENSTWO_ROZPRZESTRZENIANIA)
    {
        rozmnazanie();
    }
}

void Roslina::kolizja(Organizm& atakujacy)
{
    swiat.setLog(swiat.getLog() + string(1, rysowanie()) + " (" + to_string(x) + ", " + to_string(y) + ") zjedzony przez " + string(1, atakujacy.rysowanie()) + " (" + to_string(atakujacy.getX()) + ", " + to_string(atakujacy.getY()) + ")" + "\n");

    swiat.ustawOrganizmNa(atakujacy.getY(), atakujacy.getX(), nullptr);
    atakujacy.setX(x);
    atakujacy.setY(y);
    swiat.ustawOrganizmNa(atakujacy.getY(), atakujacy.getX(), &atakujacy);

    swiat.usunOrganizm(this);
}

void Roslina::rozmnazanie()
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
                Organizm* potomek = this->stworzPotomka(newX, newY);
                swiat.dodajOrganizm(potomek);
                
                swiat.setLog(swiat.getLog() + string(1, rysowanie()) + " (" + to_string(x) + ", " + to_string(y) + ") rozprzestrzenil sie na: " + to_string(newX) + ", " + to_string(newY) + "\n");
                return;
            }
        }
    }
    swiat.setLog(swiat.getLog() + string(1, rysowanie()) + " (" + to_string(x) + ", " + to_string(y) + ") - brak miejsca do rozprzestrzenienia" + "\n");
}