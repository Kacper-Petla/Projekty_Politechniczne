#pragma once
#include "Zwierze.h"

class Czlowiek : public Zwierze 
{
    public:
        Czlowiek(int x, int y, Swiat& swiat);
        void akcja() override;
        void rozmnazanie() override;
        char rysowanie() const override;
        Organizm* stworzPotomka(int x, int y) const override;
};