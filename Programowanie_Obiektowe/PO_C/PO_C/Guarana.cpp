#include "Guarana.h"

Guarana::Guarana(int x, int y, Swiat& swiat) : Roslina(GUARANA_SILA, x, y, swiat) {}

void Guarana::kolizja(Organizm& atakujacy)
{
    swiat.setLog(swiat.getLog() + string(1, rysowanie()) + " (" + to_string(x) + ", " + to_string(y) + ") zjedzony przez " + string(1, atakujacy.rysowanie()) + " (" + to_string(atakujacy.getX()) + ", " + to_string(atakujacy.getY()) + ")" + "\n");

    atakujacy.setSila(atakujacy.getSila() + GUARANA_BOOST_SILA);

    swiat.ustawOrganizmNa(atakujacy.getY(), atakujacy.getX(), nullptr);
    atakujacy.setX(x);
    atakujacy.setY(y);
    swiat.ustawOrganizmNa(atakujacy.getY(), atakujacy.getX(), &atakujacy);

    swiat.usunOrganizm(this);
}

char Guarana::rysowanie() const
{
    return GUARANA;
}

Organizm* Guarana::stworzPotomka(int x, int y) const
{
    return new Guarana(x, y, swiat);
}