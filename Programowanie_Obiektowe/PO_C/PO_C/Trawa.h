#pragma once
#include "Roslina.h"

class Trawa : public Roslina
{
    public:
        Trawa(int x, int y, Swiat& swiat);
        void kolizja(Organizm& inny) override;
        char rysowanie() const override;
        Organizm* stworzPotomka(int x, int y) const override;
};