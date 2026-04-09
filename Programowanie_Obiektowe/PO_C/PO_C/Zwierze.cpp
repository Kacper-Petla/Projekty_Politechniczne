#include "Zwierze.h"

Zwierze::Zwierze(int sila, int inicjatywa, int x, int y, Swiat& swiat) : Organizm(sila, inicjatywa, x, y, swiat, ZWIERZE) {}

void Zwierze::akcja() 
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
            przesun(newX, newY);
            break;
        }
    }
}

void Zwierze::kolizja(Organizm& atakujacy)
{
    if (typeid(*this) == typeid(atakujacy))          //rozmnarzanie
    {
        rozmnazanie();
    }
    else 
    {
        atak(atakujacy);
    }
}

void Zwierze::rozmnazanie()
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

                swiat.setLog(swiat.getLog() + string(1, rysowanie()) + " (" + to_string(x) + ", " + to_string(y) + ", " + to_string(sila) + ") wydal potomka " + string(1, rysowanie()) + " (" + to_string(newX) + ", " + to_string(newY) + ", " + to_string(potomek->getSila()) + ")" + "\n");
                return;
            }
        }
    }
    swiat.setLog(swiat.getLog() + "Potomek " + string(1, rysowanie()) + " (" + to_string(x) + ", " + to_string(y) + ", " + to_string(sila) + ") zostal zgnieciony" + "\n");
}

void Zwierze::przesun(int newX, int newY)
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
        cel->kolizja(*this);
    }
}

void Zwierze::atak(Organizm& atakujacy)
{
    if (atakujacy.getSila() >= sila)             //atakujacy wygral
    {
        swiat.setLog(swiat.getLog() + string(1, rysowanie()) + " (" + to_string(x) + ", " + to_string(y) + ", " + to_string(sila) + ") zabity przez " + string(1, atakujacy.rysowanie()) + " (" + to_string(atakujacy.getX()) + ", " + to_string(atakujacy.getY()) + ", " + to_string(atakujacy.getSila()) + ")" + "\n");

        swiat.ustawOrganizmNa(atakujacy.getY(), atakujacy.getX(), nullptr);
        atakujacy.setX(x);
        atakujacy.setY(y);
        swiat.ustawOrganizmNa(atakujacy.getY(), atakujacy.getX(), &atakujacy);

        swiat.usunOrganizm(this);
    }
    else                                        //atakujacy przegral
    {
        swiat.setLog(swiat.getLog() + string(1, rysowanie()) + " (" + to_string(x) + ", " + to_string(y) + ", " + to_string(sila) + ") obronil sie przed " + string(1, atakujacy.rysowanie()) + " (" + to_string(atakujacy.getX()) + ", " + to_string(atakujacy.getY()) + ", " + to_string(atakujacy.getSila()) + ")" + "\n");

        swiat.ustawOrganizmNa(atakujacy.getY(), atakujacy.getX(), nullptr);
        swiat.usunOrganizm(&atakujacy);
    }
}