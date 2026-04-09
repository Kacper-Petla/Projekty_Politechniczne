#include "FileManager.h"

void PrintMenu() {
    cout << "\n====================MENU====================" << endl;
    cout << "1. Dodaj rekord" << endl;
    cout << "2. Znajdz rekord" << endl;
    cout << "3. Reorganizacja" << endl;
    cout << "4. Wyswietl strukture plikow" << endl;
    cout << "5. Wyswietl rekordy logicznie" << endl;
    cout << "6. Edytuj rekord" << endl;
    cout << "7. Usun rekord" << endl;
    cout << "8. Wczytaj komendy z pliku" << endl;
    cout << "0. Wyjscie" << endl;
    cout << "Wybor: ";
}

int main()
{
    unsigned int page_size;
    float alpha;
    cout << "\n==================INICJALIZCJA==================" << endl;
    cout << "Podaj rozmiar stron (bajty): ";
    cin >> page_size;
    if (page_size < RECORD_ENTRY_SIZE) page_size = RECORD_ENTRY_SIZE;
    cout << "Podaj alpha: ";
    cin >> alpha;
    if (alpha > 1.0f) alpha = 1.0f;
    HardDrive hd(page_size);
    FileManager fm(&hd, page_size, alpha);

    int choice;
    do {
        PrintMenu();
        cin >> choice;
        system("cls");


        if(fm.getRecordsInOverflow() > WHEN_REORGANIZE) fm.Reorganize();
        hd.resetStats();

        switch (choice)
        {
        case 1: 
        {
            uint32_t key;
            string data;
            cout << "Podaj klucz: "; cin >> key;
            cout << "Podaj dane rekordu (3cyfry i 6liczb): "; cin >> data;

            if (fm.AddRecord(data, key)) cout << "Sukces." << endl;
            break;
        }
        case 2: 
        {
            vector<RecordEntry> records;
            uint32_t page_index;
            uint32_t in_page_index;
            bool in_overflow;
            uint32_t key;
            cout << "Podaj klucz: "; cin >> key;
            if (fm.FindRecord(key, records, page_index, in_page_index, in_overflow))
            {
                const RecordEntry& found_r = records[in_page_index];

                cout << "\n=== ZNALEZIONO REKORD ===" << endl;
                cout << "Klucz: " << found_r.key << endl;
                string data(records[in_page_index].data, DATA_SIZE);
                cout << "Dane:  " << data << endl;
                cout << "Gdzie: " << (in_overflow ? "Obszar Nadmiarowy (Overflow)" : "Obszar Glowny (Primary)") << endl;
                cout << "Info:  Strona " << page_index << ", Indeks na stronie " << in_page_index << endl;
                cout << "=========================\n";
            }
            else
            {
                cout << "NIE znaleziono";
            }
            break;
        }
        case 3:
            fm.Reorganize();
            break;
        case 4: 
        {
            fm.PrintIndexStructure();
            fm.PrintPrimaryStructure();
            fm.PrintOverflowStructure();
            break;
        }
        case 5:
            fm.PrintRecords();
            break;
        case 6:
        {
            uint32_t key;
            string data;
            cout << "Podaj klucz: "; cin >> key;
            cout << "Podaj nowe dane rekordu (3cyfry i 6liczb): "; cin >> data;
            if(!fm.EditRecord(data, key)) cout<<"nie znaleziono rekordu";
            break;
        }
        case 7:
        {
            uint32_t key;
            cout << "Podaj klucz: "; cin >> key;
            if (!fm.DeleteRecord(key)) cout << "nie znaleziono rekordu";
            else cout << "Rekord (Klucz " << key << ") oznaczony jako usuniety (klucz zostanie zwolniony przy nastepnej reorganizacji)" << endl;
            break;
        }
        case 8: 
        {
            string filename;
            cout << "Podaj nazwe pliku: "; cin >> filename;
            fm.RunScript(filename);
            break;
        }
        }

        // Raport I/O po każdej operacji interaktywnej
        if (choice != 0) { 
            cout << "[DISK I/O] Reads: " << hd.getReads() << ", Writes: " << hd.getWrites() << endl;
        }

    } while (choice != 0);

    return 0;
}