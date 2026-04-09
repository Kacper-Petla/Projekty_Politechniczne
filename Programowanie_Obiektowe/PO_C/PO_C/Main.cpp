#include "Swiat.h"

int main() 
{
    srand(time(nullptr));

    char komenda;
    while (true)
    {
        Konsola::ustawKursor(0, 0);

        cout << "Kacper Petla 198186";

        Konsola::ustawKursor(0, 2);
        Konsola::wyczyscLinie();

        cout << "WPISZ KOMENDE: ";
        cin >> komenda;

        Konsola::ustawKursor(0, 3);
        Konsola::wyczyscLinie();

        switch (komenda)
        {
            case NOWA_GRA:
            {
                int x, y;
                Konsola::ustawKursor(0, 2);
                Konsola::wyczyscLinie();

                cout << "WPISZ WYMIARY\n -szerokosc: ";
                cin >> x;
                cout <<" -wysokosc: ";
                cin >> y;

                Swiat swiat(x, y);

                swiat.zainicjujGre();
                swiat.rozpocznijGre();
                Konsola::wyczyscEkran();
                break;
            }
            case WYJDZ:
                return 0;
                break;
            case WCZYTAJ:
            {
                string nazwa_pliku;
                Konsola::ustawKursor(0, 2);
                Konsola::wyczyscLinie();

                cout << "WPISZ PLIK Z ZAPISEM: ";
                cin >> nazwa_pliku;
                nazwa_pliku += ".txt";

                ifstream plik(nazwa_pliku);
                if (!plik.is_open()) 
                {
                    cout << "NIE ZNALEZIONO TAKIEGO ZAPISU";
                    break;
                }

                int x, y;
                plik >> x >> y;

                Swiat swiat(x, y);

                swiat.wczytajGre(plik);
                swiat.rozpocznijGre();
                Konsola::wyczyscEkran();
                break;
            }
            default:
                cout << "NIEPOPRAWNA KOMENDA";
                break;
            }
    }
    return 0;
}