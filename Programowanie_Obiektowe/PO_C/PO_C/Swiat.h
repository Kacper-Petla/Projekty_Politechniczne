#pragma once
#include "Organizm.h"

using namespace std;

class Swiat 
{
    private:
        int szerokosc, wysokosc;
        vector<Organizm*> organizmy;
        vector<Organizm*> organizmy_do_wykonania;
        vector<vector<Organizm*>> plansza;
        Organizm* gracz;
        string log;

        void wylosujPuste(int* X, int* Y);
        void zapiszGre();
        bool inputUrzytkownika();
        void wykonajTure();
        void sortowanie();
        void rysujSwiat();
        void organizmyFix();

    public:
        Swiat(int szer, int wys);

        void zainicjujGre();
        void wczytajGre(ifstream& plik);
        void dodajOrganizm(Organizm* org);
        void rozpocznijGre();

        bool czyNaPlanszy(int x, int y) const;
        void ustawOrganizmNa(int y, int x, Organizm* org);
        Organizm* getOrganizmNa(int x, int y);
        int getSzerokosc() const;
        int getWysokosc() const;
        string getLog() const;
        void setLog(string newlog);

        void usunOrganizm(Organizm* org);
        ~Swiat();
};