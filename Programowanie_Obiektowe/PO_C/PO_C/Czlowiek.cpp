#include "Czlowiek.h"

Czlowiek::Czlowiek(int x, int y, Swiat& swiat) : Zwierze(CZLOWIEK_SILA, CZLOWIEK_INICJATYWA, x, y, swiat) 
{
    umiejetnosc = -(CZAS_TRWANIA_UMIEJETNOSCI + CZAS_REGENERACJI_UMIEJETNOSCI);
    tmp_sila = 0;
}

void Czlowiek::akcja() 
{
    if (umiejetnosc == wiek)                //umiejetnosc aktywowana
    {
        tmp_sila = sila;
        sila = BOOST_SILY;
    }
    else if (CZAS_TRWANIA_UMIEJETNOSCI + umiejetnosc - wiek > 0)    //umiejetnosc aktywna
    {
        sila--;
    }
    else if (CZAS_TRWANIA_UMIEJETNOSCI + umiejetnosc - wiek == 0)   //umiejentonosc wygasla
    {
        sila = sila - (BOOST_SILY - CZAS_TRWANIA_UMIEJETNOSCI + 1) + tmp_sila;         //bierzemy pod uwage inne
    }

    wiek++;

    przesun(gx, gy);
}

void Czlowiek::rozmnazanie()
{
    return;
}

char Czlowiek::rysowanie() const 
{
    return CZLOWIEK;
}

Organizm* Czlowiek::stworzPotomka(int x, int y) const 
{
    return nullptr;
}