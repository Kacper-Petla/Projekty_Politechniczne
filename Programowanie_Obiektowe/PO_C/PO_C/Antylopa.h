#pragma once
#include "Zwierze.h"

class Antylopa : public Zwierze
{
    public:
        Antylopa(int x, int y, Swiat& swiat);
        void akcja() override;
        void kolizja(Organizm& inny) override;
        char rysowanie() const override;
        Organizm* stworzPotomka(int x, int y) const override;
};