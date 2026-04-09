#include "Wilcza_Jagoda.h"

Wilcza_Jagoda::Wilcza_Jagoda(int x, int y, Swiat& swiat) : Roslina(WILCZA_JAGODA_SILA, x, y, swiat) {}

void Wilcza_Jagoda::kolizja(Organizm& atakujacy)
{
    swiat.setLog(swiat.getLog() + string(1, rysowanie()) + " (" + to_string(x) + ", " + to_string(y) + ") zjedzony przez " + string(1, atakujacy.rysowanie()) + " (" + to_string(atakujacy.getX()) + ", " + to_string(atakujacy.getY()) + ")" + "\n");
    swiat.setLog(swiat.getLog() + string(1, atakujacy.rysowanie()) + " (" + to_string(atakujacy.getX()) + ", " + to_string(atakujacy.getY()) + ") zatruty przez " + string(1, rysowanie()) + "\n");

    swiat.ustawOrganizmNa(atakujacy.getY(), atakujacy.getX(), nullptr);
    swiat.ustawOrganizmNa(y, x, nullptr);

    swiat.usunOrganizm(&atakujacy);
    swiat.usunOrganizm(this);
}

char Wilcza_Jagoda::rysowanie() const
{
    return WILCZA_JAGODA;
}

Organizm* Wilcza_Jagoda::stworzPotomka(int x, int y) const
{
    return new Wilcza_Jagoda(x, y, swiat);
}