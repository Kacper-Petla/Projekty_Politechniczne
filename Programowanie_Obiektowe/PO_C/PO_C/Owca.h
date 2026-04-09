#pragma once
#include "Zwierze.h"

class Owca : public Zwierze
{
    public:
        Owca(int x, int y, Swiat& swiat);
        char rysowanie() const override;
        Organizm* stworzPotomka(int x, int y) const override;
};