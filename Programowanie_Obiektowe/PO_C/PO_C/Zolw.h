#pragma once
#include "Zwierze.h"

class Zolw : public Zwierze
{
    public:
        Zolw(int x, int y, Swiat& swiat);
        void akcja() override;
        void kolizja(Organizm& inny) override;
        char rysowanie() const override;
        Organizm* stworzPotomka(int x, int y) const override;
};