#pragma once
#include "Organizm.h"
#include "Swiat.h"

class Roslina : public Organizm
{
    public:
        Roslina(int sila, int x, int y, Swiat& swiat);
        void akcja() override;
        void kolizja(Organizm& inny) override;
        void rozmnazanie();
};