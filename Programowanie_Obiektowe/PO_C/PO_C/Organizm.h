#pragma once
#include "stale.h"

class Swiat;

class Organizm 
{
    protected:
        int sila;
        int inicjatywa;
        int x, y;
        Swiat& swiat;
        int wiek;
        Typ_Organizmu typ;

        int gx, gy;
        int tmp_sila;
        int umiejetnosc;

    public:
        Organizm(int sila, int inicjatywa, int x, int y, Swiat& swiat, Typ_Organizmu typ);
        virtual void akcja() = 0;
        virtual void kolizja(Organizm& inny) = 0;
        virtual void rozmnazanie() = 0;
        virtual char rysowanie() const = 0;
        virtual Organizm* stworzPotomka(int x, int y) const = 0;

        int getX() const;
        int getY() const;
        void setX(int newX);
        void setY(int newY);

        int getSila() const;
        int getInicjatywa() const;
        void setSila(int newSila);
        void setInicjatywa(int newInicjatywa);

        int getWiek() const;
        void setWiek(int newWiek);
        Typ_Organizmu getTyp() const;

        void setGX(int newGX);
        void setGY(int newGY);
        int getTmp_Sila() const;
        void setTmp_Sila(int newTmp_Sila);
        int getUmiejetnosc() const;
        void setUmiejetnosc(int newUmiejetnosc);

        virtual ~Organizm() = default;
};