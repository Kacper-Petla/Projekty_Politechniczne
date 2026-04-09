#pragma once
#include "Zwierze.h"

class Wilk : public Zwierze
{
    public:
        Wilk(int x, int y, Swiat& swiat);
        char rysowanie() const override;
        Organizm* stworzPotomka(int x, int y) const override;
};