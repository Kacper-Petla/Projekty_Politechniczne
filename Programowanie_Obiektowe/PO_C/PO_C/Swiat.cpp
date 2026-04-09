#include "Swiat.h"
#include "Czlowiek.h"
#include "Owca.h"
#include "Wilk.h"
#include "Lis.h"
#include "Zolw.h"
#include "Antylopa.h"
#include "Trawa.h"
#include "Mlecz.h"
#include "Guarana.h"
#include "Wilcza_Jagoda.h"
#include "Barszcz_Sosnowskiego.h"

Swiat::Swiat(int szer, int wys) : szerokosc(szer), wysokosc(wys) 
{ 
    plansza = vector<vector<Organizm*>>(wysokosc, vector<Organizm*>(szerokosc, nullptr));
    log = "";
}


//private

void Swiat::wylosujPuste(int* X, int* Y)
{
    *X = rand() % szerokosc;
    *Y = rand() % wysokosc;

    while (getOrganizmNa(*X, *Y) != nullptr)
    {
        *X = rand() % szerokosc;
        *Y = rand() % wysokosc;
    }
}

void Swiat::zapiszGre()
{
    string nazwa_pliku;

    Konsola::ustawKursor(0, wysokosc + 1);
    Konsola::wyczyscLinie();

    cout << "WPISZ NAZWE PLIKU DO ZAPISU: ";
    cin >> nazwa_pliku;
    nazwa_pliku += ".txt";

    Konsola::ustawKursor(0, wysokosc + 2);
    Konsola::wyczyscLinie();

    ofstream plik(nazwa_pliku);
    if (!plik.is_open()) {
        cout << "NIE MOZNA OTWORZYC PLIKU";
        return;
    }

    // Zapis planszy (wymiary)
    plik << szerokosc << " " << wysokosc << "\n";

    // Zapis gracza
    if (gracz)
    {
        plik << gracz->rysowanie() << " " << gracz->getX() << " " << gracz->getY() << " " << gracz->getWiek() << " " << gracz->getSila() << " " << gracz->getTmp_Sila() << " " << gracz->getUmiejetnosc() << "\n";
    }

    // Zapis pozosta³ych organizmów
    for (Organizm* org : organizmy)
    {
        if (org != gracz)
        {
            plik << org->rysowanie() << " " << org->getX() << " " << org->getY() << " " << org->getWiek() << " " << org->getSila() << "\n";
        }
    }

    cout << "ZAPISANO GRE";

    plik.close();
}

bool Swiat::inputUrzytkownika()
{
    char ruch;
    bool move = false;
    while (true)
    {
        Konsola::ustawKursor(0, wysokosc + 1);
        Konsola::wyczyscLinie();

        cout << "Wpisz komende: ";
        cin >> ruch;

        Konsola::ustawKursor(0, wysokosc + 2);
        Konsola::wyczyscLinie();

        int gx = 0, gy = 0;
        switch (ruch)
        {
        case GORA:
            gy = 1;
            move = true;
            break;
        case DOL:
            gy = -1;
            move = true;
            break;
        case LEWO:
            gx = -1;
            move = true;
            break;
        case PRAWO:
            gx = 1;
            move = true;
            break;
        case UMIEJETNOSC:
            if (CZAS_TRWANIA_UMIEJETNOSCI + CZAS_REGENERACJI_UMIEJETNOSCI + gracz->getUmiejetnosc() - gracz->getWiek() <= 0)
            {
                gracz->setUmiejetnosc(gracz->getWiek());
                cout << "AKTYWOWANO MAGICZNY ELIKSIR";
            }
            else
            {
                cout << "UMIEJETNOSC NIEDOSTEPNA PRZEZ: " << CZAS_TRWANIA_UMIEJETNOSCI + CZAS_REGENERACJI_UMIEJETNOSCI + gracz->getUmiejetnosc() - gracz->getWiek() << " TUR";
            }
            break;
        case WYJDZ:
            return false;
            break;
        case ZAPISZ:
            zapiszGre();
            break;
        case WCZYTAJ:
            cout << "ZAMKNIJ OBECNA GRE BY WCZYTAC";
            break;
        case LOG:
            Konsola::ustawKursor(0, wysokosc + 3);
            cout << "LOGI:\n" << log;
            break;
        default:
            cout << "NIEPOPRAWNA KOMENDA";
            break;
        }
        if (move)           //wybrano ruch
        {
            int newX = gracz->getX() + gx;
            int newY = gracz->getY() + gy;

            if (czyNaPlanszy(newX, newY))
            {
                gracz->setGX(newX);
                gracz->setGY(newY);
                break;
            }
            else
            {
                cout << "RUCH POZA PLANSZE";
                move = false;
            }
        }
    }
    return true;
}

void Swiat::wykonajTure()
{
    log = "";

    sortowanie();

    Konsola::ustawKursor(0, wysokosc + 3);

    organizmy_do_wykonania = organizmy;
    vector<Organizm*> tmp_organizmy = organizmy;

    for (Organizm* org : tmp_organizmy)
    {
        if (find(organizmy_do_wykonania.begin(), organizmy_do_wykonania.end(), org) != organizmy_do_wykonania.end())
        {
            org->akcja();
        }
    }

    organizmyFix();
}

void Swiat::sortowanie()
{
    sort(organizmy.begin(), organizmy.end(), [](Organizm* a, Organizm* b)
        {
            if (a->getInicjatywa() != b->getInicjatywa())
            {
                return a->getInicjatywa() > b->getInicjatywa();
            }
            return a->getWiek() > b->getWiek();
        });
}

void Swiat::rysujSwiat()
{
    Konsola::wyczyscEkran();

    cout << "Tura: " << gracz->getWiek() << endl;

    for (int y = wysokosc - 1; y >= 0; y--)
    {
        for (int x = 0; x < szerokosc; x++)
        {
            if (plansza[y][x])
            {
                cout << plansza[y][x]->rysowanie();
            }
            else
            {
                cout << ".";
            }
        }
        cout << "\n";
    }
}

void Swiat::organizmyFix()
{
    for (Organizm* org : organizmy)
    {
        if (getOrganizmNa(org->getX(), org->getY()) == nullptr)
        {
            ustawOrganizmNa(org->getY(), org->getX(), org);
        }
    }
}


//public

void Swiat::zainicjujGre()
{
    int X, Y;
    wylosujPuste(&X, &Y);

    gracz = new Czlowiek(X, Y, *this);
    dodajOrganizm(gracz);

    int organizm_na_plansze = (szerokosc * wysokosc) * PROCENT_PLANSZY / LICZBA_ORGANIZMOW;

    for (int i = 0; i < organizm_na_plansze; i++)
    {
        wylosujPuste(&X, &Y);
        dodajOrganizm(new Owca(X, Y, *this));
    }
    for (int i = 0; i < organizm_na_plansze; i++)
    {
        wylosujPuste(&X, &Y);
        dodajOrganizm(new Wilk(X, Y, *this));
    }
    for (int i = 0; i < organizm_na_plansze; i++)
    {
        wylosujPuste(&X, &Y);
        dodajOrganizm(new Lis(X, Y, *this));
    }
    for (int i = 0; i < organizm_na_plansze; i++)
    {
        wylosujPuste(&X, &Y);
        dodajOrganizm(new Zolw(X, Y, *this));
    }
    for (int i = 0; i < organizm_na_plansze; i++)
    {
        wylosujPuste(&X, &Y);
        dodajOrganizm(new Antylopa(X, Y, *this));
    }
    for (int i = 0; i < organizm_na_plansze; i++)
    {
        wylosujPuste(&X, &Y);
        dodajOrganizm(new Trawa(X, Y, *this));
    }
    for (int i = 0; i < organizm_na_plansze; i++)
    {
        wylosujPuste(&X, &Y);
        dodajOrganizm(new Mlecz(X, Y, *this));
    }
    for (int i = 0; i < organizm_na_plansze; i++)
    {
        wylosujPuste(&X, &Y);
        dodajOrganizm(new Guarana(X, Y, *this));
    }
    for (int i = 0; i < organizm_na_plansze; i++)
    {
        wylosujPuste(&X, &Y);
        dodajOrganizm(new Wilcza_Jagoda(X, Y, *this));
    }
    for (int i = 0; i < organizm_na_plansze; i++)
    {
        wylosujPuste(&X, &Y);
        dodajOrganizm(new Barszcz_Sosnowskiego(X, Y, *this));
    }
    
}

void Swiat::wczytajGre(ifstream& plik)
{
    char symbol;
    int x, y, sila, wiek;
    while (plik >> symbol) 
    {
        if (symbol == CZLOWIEK) 
        {
            int tmp_sila, umiejetnosc;
            plik >> x >> y >> wiek >> sila >> tmp_sila >> umiejetnosc;
            gracz = new Czlowiek(x, y, *this);
            gracz->setWiek(wiek);
            gracz->setSila(sila);
            gracz->setTmp_Sila(tmp_sila);
            gracz->setUmiejetnosc(umiejetnosc);
            dodajOrganizm(gracz);
        }
        else 
        {
            plik >> x >> y >> wiek >> sila;
            Organizm* org = nullptr;

            if (symbol == OWCA) org = new Owca(x, y, *this);
            else if (symbol == WILK) org = new Wilk(x, y, *this);
            else if (symbol == LIS) org = new Lis(x, y, *this);
            else if (symbol == ZOLW) org = new Zolw(x, y, *this);
            else if (symbol == ANTYLOPA) org = new Antylopa(x, y, *this);
            else if (symbol == TRAWA) org = new Trawa(x, y, *this);
            else if (symbol == MLECZ) org = new Mlecz(x, y, *this);
            else if (symbol == GUARANA) org = new Guarana(x, y, *this);
            else if (symbol == WILCZA_JAGODA) org = new Wilcza_Jagoda(x, y, *this);
            else if (symbol == BARSZCZ_SOSNOWSKIEGO) org = new Barszcz_Sosnowskiego(x, y, *this);

            if (org) 
            {
                Organizm* org2 = getOrganizmNa(x, y);
                org->setWiek(wiek);
                org->setSila(sila);
                dodajOrganizm(org);
                if (symbol == TRAWA && org2 != nullptr)
                {
                    ustawOrganizmNa(y, x, org2);
                }
            }
        }
    }

    plik.close();
}

void Swiat::dodajOrganizm(Organizm* org)
{
    organizmy.push_back(org);
    plansza[org->getY()][org->getX()] = org;
}

void Swiat::rozpocznijGre()
{
    while (true)
    {
        if (gracz != nullptr)
        {
            rysujSwiat();
            if (inputUrzytkownika())
            {
                wykonajTure();
            }
            else
            {
                break;
            }
        }
        else
        {
            Konsola::ustawKursor(0, wysokosc + 1);
            Konsola::wyczyscLinie();
            cout << "GAME OVER";

            Konsola::ustawKursor(0, wysokosc + 3);
            cout << "LOGI:\n" << log;
            char a;
            cin >> a;
            break;
        }
    }
}


bool Swiat::czyNaPlanszy(int x, int y) const 
{
    return x >= 0 && x < szerokosc && y >= 0 && y < wysokosc;
}


void Swiat::ustawOrganizmNa(int y, int x, Organizm* org) 
{
    if (czyNaPlanszy(x, y))
    {
        plansza[y][x] = org;
    }
}

Organizm* Swiat::getOrganizmNa(int x, int y) 
{
    if (czyNaPlanszy(x, y))
    {
        return plansza[y][x];
    }
    return nullptr;
}

int Swiat::getSzerokosc() const { return szerokosc; }
int Swiat::getWysokosc() const { return wysokosc; }

string Swiat::getLog() const { return log; }
void Swiat::setLog(string newLog) { log = newLog; }


void Swiat::usunOrganizm(Organizm* org) 
{
    auto it = find(organizmy_do_wykonania.begin(), organizmy_do_wykonania.end(), org);
    if (it != organizmy_do_wykonania.end())
    {
        organizmy_do_wykonania.erase(it);
    }

   auto it2 = find(organizmy.begin(), organizmy.end(), org);
    if (it2 != organizmy.end()) 
    {
        if (org == gracz)
        {
            gracz = nullptr;
        }
        organizmy.erase(it2);
        delete org;
    }
}

Swiat::~Swiat() 
{
    for (Organizm* org : organizmy) 
    {
        delete org;
    }
}