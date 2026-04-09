#pragma once
#include "Roslina.h"

class Mlecz : public Roslina
{
    public:
        Mlecz(int x, int y, Swiat& swiat);
        void akcja() override;
        char rysowanie() const override;
        Organizm* stworzPotomka(int x, int y) const override;
};