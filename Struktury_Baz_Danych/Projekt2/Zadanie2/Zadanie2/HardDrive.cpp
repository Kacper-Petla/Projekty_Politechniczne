#include "HardDrive.h"

HardDrive::HardDrive(unsigned int page_size)
{
    reads = 0;
    writes = 0;
    page_Size = page_size;
    CleanOrCreateFile(INDEX_ONE_FILE);
    CleanOrCreateFile(INDEX_TWO_FILE);
    CleanOrCreateFile(PRIMARY_ONE_FILE);
    CleanOrCreateFile(PRIMARY_TWO_FILE);
    CleanOrCreateFile(OVERFLOW_ONE_FILE);
    CleanOrCreateFile(OVERFLOW_TWO_FILE);
}

//bezposrednie operacje na pliku

bool HardDrive::CleanOrCreateFile(const string& filename)   //czyszczenie pliku
{
    ofstream file(filename, ios::binary | ios::trunc);
    if (!file.is_open())
    {
        cout << "Nie mozna otworzyc pliku.\n";
        return false;
    }

    file.close();
    return true;
}

bool HardDrive::SaveToFile(const string& buffer, unsigned int page_index, const string& filename)    //dopisywanie bloku do pliku
{
    fstream file(filename, ios::in | ios::out | ios::binary);
    if (!file.is_open())
    {
        cout << "Nie mozna otworzyc pliku do zapisu.\n";
        return false;
    }

    size_t offset = page_index * page_Size;

    file.seekg(0, ios::end);
    size_t file_size = file.tellg();

    if (file_size < offset + page_Size)
    {
        file.clear();
        file.seekp(0, ios::end);
        size_t bytes_to_add = (offset + page_Size) - file_size;
        string page(page_Size, PADDING);
        while (bytes_to_add > 0)
        {
            size_t write_now = (min(bytes_to_add, page_Size));
            file.write(page.data(), write_now);
            if (!file) 
            {
                cout << "Blad zapisu podczas powiekszania pliku.\n";
                file.close();
                return false;
            }
            bytes_to_add -= write_now;
        }
        file.flush();
    }

    file.clear();
    file.seekp(offset, ios::beg);
    if (!file)
    {
        cout << "Nie mozna ustawic pozycji zapisu.\n";
        file.close();
        return false;
    }

    string page(page_Size, PADDING);
    page.replace(0, min(buffer.size(), (size_t)page_Size), buffer.substr(0, page_Size));      //jesli buffer jest mniejszy od bloku to zostana dopisany padding, a jesli wiekszy bajty zostana uciente

    file.write(page.data(), page.size());
    if (!file)
    {
        cout << "Blad zapisu strony.\n";
        file.close();
        return false;
    }
    file.close();
    writes++;
    return true;
}

bool HardDrive::LoadFromFile(string& buffer, unsigned int page_index, const string& filename) //wczytuje dany blok z pliku i probuje wsadzic do buffora
{
    ifstream file(filename, ios::binary);
    if (!file.is_open())
    {
        cout << "Nie mozna otworzyc pliku do odczytu.\n";
        return false;
    }

    size_t offset = page_index * page_Size;
    vector<char> page(page_Size);

    file.seekg(offset, ios::beg);

    file.read(page.data(), page.size());
    size_t bytes_read = static_cast<size_t>(file.gcount());
    if (bytes_read == 0)                                     //pusty blok
    {
        file.close();
        return false;
    }

    buffer.clear();
    buffer.assign(page.data(), bytes_read);

    file.close();
    reads++;
    return true;
}


void HardDrive::CopyRecords(const string& source_filename, const string& dest_filename)     //skopiowanie rekordow z innego pliku
{
    if (!CleanOrCreateFile(dest_filename)) return;

    string buffer;
    unsigned int page_index = 0;
    while (LoadFromFile(buffer, page_index, source_filename))
    {
        SaveToFile(buffer, page_index, dest_filename);
        page_index++;
    }
}


int HardDrive::getWrites()
{
    return writes;
}

int HardDrive::getReads()
{
    return reads;
}

void HardDrive::setWrites(int w)
{
    writes = w;
}

void HardDrive::setReads(int r)
{
    reads = r;
}

void HardDrive::resetStats()
{
    reads = 0;
    writes = 0;
}
