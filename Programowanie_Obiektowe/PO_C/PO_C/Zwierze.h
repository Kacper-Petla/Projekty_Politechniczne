#pragma once
#include "Organizm.h"
#include "Swiat.h"

class Zwierze : public Organizm 
{
    public:
        Zwierze(int sila, int inicjatywa, int x, int y, Swiat& swiat);
        void akcja() override;
        void kolizja(Organizm& atakujacy) override;
        void rozmnazanie();
        void przesun(int newX, int newY);
        void atak(Organizm& atakujacy);
};