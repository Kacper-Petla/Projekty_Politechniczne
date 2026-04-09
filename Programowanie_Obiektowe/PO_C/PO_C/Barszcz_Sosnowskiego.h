#pragma once
#include "Roslina.h"

class Barszcz_Sosnowskiego : public Roslina
{
    public:
        Barszcz_Sosnowskiego(int x, int y, Swiat& swiat);
        void akcja() override;
        void kolizja(Organizm& inny) override;
        char rysowanie() const override;
        Organizm* stworzPotomka(int x, int y) const override;
};