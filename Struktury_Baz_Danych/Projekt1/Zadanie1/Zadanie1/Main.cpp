#include <iostream>
#include "Sorting.h"

int main()
{
    unsigned int r_in_buff;
    unsigned int n_buff;

    cout << "ile rekordow miesci buffer: ";
    cin >> r_in_buff;
    cout << "ile bufferow: ";
    cin >> n_buff;


    HardDrive dysk(r_in_buff);
    Sorting sorter(dysk, n_buff);
    unsigned int n_records;
    char komenda;
    string file;

    while (true)
    {
        system("cls");

        cout << "komendy:" << endl;
        cout << DRAW << " - wylosuj" << endl;
        cout << ENTER << " - wpisz" << endl;
        cout << COPY << " - kopjuj" << endl;
        cout << SORT << " - sortuj" << endl;
        cout << QUIT << " - wyjscie" << endl;

        cin >> komenda;

        switch (komenda)
        {
            case DRAW:
            {
                cout << "liczba rekordow: ";
                cin >> n_records;
                dysk.DrawRecords(n_records);
                break;
            }
            case ENTER:
            {
                dysk.EnterRecords();
                break;
            }
            case COPY:
            {
                cout << "plik wejsciowy: ";
                cin >> file;
                dysk.CopyRecords(file);
                break;
            }
            case SORT:
            {
                char if_debug;
                bool debug;
                cout << "wlaczyc debug?(t/f): ";
                cin >> if_debug;
                if (if_debug == 't') debug = true;
                else debug = false;
                sorter.SortRecords(debug);
                break;
            }
            case QUIT:
            {
                return 0;
            }
            default:
            {
                cout << "BLEDNA KOMENDA";
            }
        }

        cin.get();
        cin.get();
    }

    return 0;
}