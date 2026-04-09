#pragma once
#include "Roslina.h"

class Guarana : public Roslina
{
    public:
        Guarana(int x, int y, Swiat& swiat);
        void kolizja(Organizm& inny) override;
        char rysowanie() const override;
        Organizm* stworzPotomka(int x, int y) const override;
};