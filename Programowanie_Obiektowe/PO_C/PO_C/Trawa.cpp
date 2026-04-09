#include "Trawa.h"

Trawa::Trawa(int x, int y, Swiat& swiat) : Roslina(TRAWA_SILA, x, y, swiat) {}

void Trawa::kolizja(Organizm& atakujacy)
{
    swiat.setLog(swiat.getLog() + string(1, rysowanie()) + " (" + to_string(x) + ", " + to_string(y) + ") pod " + string(1, atakujacy.rysowanie()) + " (" + to_string(atakujacy.getX()) + ", " + to_string(atakujacy.getY()) + ")" + "\n");

    swiat.ustawOrganizmNa(atakujacy.getY(), atakujacy.getX(), nullptr);
    atakujacy.setX(x);
    atakujacy.setY(y);
    swiat.ustawOrganizmNa(atakujacy.getY(), atakujacy.getX(), &atakujacy);

    if (atakujacy.rysowanie() == OWCA)
    {
        swiat.usunOrganizm(this);
    }

    //swiat.usunOrganizm(this);
}

char Trawa::rysowanie() const
{
    return TRAWA;
}

Organizm* Trawa::stworzPotomka(int x, int y) const
{
    return new Trawa(x, y, swiat);
}