#pragma once
#include "Roslina.h"

class Wilcza_Jagoda : public Roslina
{
    public:
        Wilcza_Jagoda(int x, int y, Swiat& swiat);
        void kolizja(Organizm& inny) override;
        char rysowanie() const override;
        Organizm* stworzPotomka(int x, int y) const override;
};