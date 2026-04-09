#include "HardDrive.h"

HardDrive::HardDrive(unsigned int records_in_buffer)
{ 
    records_In_Buffer = records_in_buffer;
    block_Size = (records_in_buffer * RECORD_LENGTH) + 1;
}

//bezpo�rednie operacje na pliku

bool HardDrive::CleanOrCreateFile(const string& filename)   //czyszczenie pliku
{
    ofstream file(filename);
    if (!file.is_open())
    {
        cout << "Nie mozna otworzyc pliku.\n";
        return false;
    }

    file.close();
    return true;
}

bool HardDrive::SaveToFile(const string& buffer, const string& filename)    //dopisywanie bloku do pliku
{
    ofstream file(filename, ios::app | ios::binary);
    if (!file.is_open()) 
    {
        cout << "Nie mozna otworzyc pliku do zapisu.\n";
        return false;
    }

    writes++;

    string block(block_Size, PADDING);
    block.replace(0, min(buffer.size(), (size_t)block_Size), buffer.substr(0, block_Size));      //jesli buffer jest mniejszy od bloku to zostana dopisany padding, a jesli wiekszy bajty zostana uciente

    file.write(block.data(), block.size());
    file.close();
    return true;
}

bool HardDrive::LoadFromFile(vector<string>& buffer, unsigned int block_index, const string& filename) //wczytuje dany blok z pliku i probuje wsadzic do buffora
{
    ifstream file(filename, ios::binary);
    if (!file.is_open()) 
    {
        cout << "Nie mozna otworzyc pliku do odczytu.\n";
        return false;
    }
 
    const size_t buffer_size = records_In_Buffer * RECORD_LENGTH;
    size_t offset = block_index * block_Size;
    vector<char> block(block_Size);

    file.seekg(offset, ios::beg);

    file.read(block.data(), block.size());
    size_t bytes_read = static_cast<size_t>(file.gcount());
    if (bytes_read == 0)                                     //pusty blok
    {
        return false;
    }

    reads++;

    size_t records_in_block = bytes_read / RECORD_LENGTH;
    buffer.clear();
    for (size_t i = 0; i < min(records_In_Buffer, records_in_block); i++)
    {
        buffer.emplace_back(block.data() + i * RECORD_LENGTH, RECORD_LENGTH);
    }

    file.close();
    return true;
}

//posrednie operacje na pliku

bool HardDrive::SaveSingleRecord(const string& record, const string& filename)
{
    return SaveToFile(record, filename);
}

bool HardDrive::LoadSingleRecord(string& record, unsigned int block_index, unsigned int record_index, const string& filename)
{
    vector<string> buffer;
    bool result = LoadFromFile(buffer, block_index, filename);
    if (!result || (record_index) >= buffer.size())
    {
        return false;
    }

    record = buffer[record_index];
    return true;
}

//funkcje generujace i wyswietlajace rekordy

void HardDrive::DrawRecords(unsigned int n_records, const string& filename)     //wygerenowanie n rekordow
{
    if (!CleanOrCreateFile(filename)) return;

    unsigned int seed = static_cast<unsigned int>(time(nullptr));
    mt19937 rng(seed);
    uniform_int_distribution<int> letterDist(0, 25);
    uniform_int_distribution<int> numberDist(0, 9);

    string buffer;
    buffer.reserve(records_In_Buffer * RECORD_LENGTH);

    for (unsigned int i = 0; i < n_records; i++) 
    {
        // 3 litery
        buffer.push_back('A' + letterDist(rng));
        buffer.push_back('A' + letterDist(rng));
        buffer.push_back('A' + letterDist(rng));
        // 6 cyfr
        buffer.push_back('0' + numberDist(rng));
        buffer.push_back('0' + numberDist(rng));
        buffer.push_back('0' + numberDist(rng));
        buffer.push_back('0' + numberDist(rng));
        buffer.push_back('0' + numberDist(rng));
        buffer.push_back('0' + numberDist(rng));

        // Zapis blokowy
        if ((i + 1) % records_In_Buffer == 0)
        {
            SaveToFile(buffer, filename);
            buffer.clear();
        }
    }

    // Zapis reszty
    if (!buffer.empty()) 
    {
        SaveToFile(buffer, filename);
    }

    cout << "Wygenerowano " << n_records << " rekordow do pliku: " << filename << "\n";
}

void HardDrive::EnterRecords(const string& filename)    //wczytywanie rekordow z klawiatury
{
    if (!CleanOrCreateFile(filename)) return;

    system("cls");

    string buffer;
    buffer.reserve(records_In_Buffer * RECORD_LENGTH);
    string input;
    int i = 0;

    cout << "Wpisz rekordy (np. ABC123456)\n ";
    cout << "Wpisz \"save\" by zapisac:\n\n ";

    while (true)
    {
        cin >> input;
        
        if (input == "save" || input == "SAVE") break;

        if (input.size() != RECORD_LENGTH)
        {
            cout << "Blad: rekord musi miec dokladnie 9 znakow(3 litery + 6 cyfr).\n";
            continue;
        }

        bool valid = true;
        for (size_t j = 0; j < 3; j++) 
        {
            if (!isupper(static_cast<unsigned char>(input[j]))) 
            {
                valid = false;
                break;
            }
        }
        for (size_t j = 3; j < 9 && valid; j++) 
        {
            if (!isdigit(static_cast<unsigned char>(input[j]))) {
                valid = false;
                break;
            }
        }

        if (!valid) 
        {
            cout << "Blad: rekord musi miec format 3 wielkie litery + 6 cyfr (np. ABC123456).\n";
            continue;
        }

        buffer += input;
        i++;

        // Zapis blokowy
        if (i % records_In_Buffer == 0)
        {
            i = 0;
            SaveToFile(buffer, filename);
            buffer.clear();
        }
    }

    // Zapis reszty
    if (!buffer.empty())
    {
        SaveToFile(buffer, filename);
    }

    cout << "Zapisano rekordy do pliku: " << filename << "\n";
}

void HardDrive::CopyRecords(const string& source_filename, const string& dest_filename)     //skopiowanie rekordow z innego pliku
{
    if (!CleanOrCreateFile(dest_filename)) return;

    vector<string> buffer;
    unsigned int block_number = 0;
    while (LoadFromFile(buffer, block_number, source_filename))
    {
        block_number++;
        string block;
        for (size_t i = 0; i < buffer.size(); i++)
        {
            block += buffer[i];
        }
        SaveToFile(block, dest_filename);
    }
}

void HardDrive::WriteRecords(const string& filename)    //wypisanie rekordow
{
    vector<string> buffer;
    unsigned int block_number = 0;
    while (LoadFromFile(buffer, block_number, filename))
    {
        block_number++;
        
        for (size_t i = 0; i < buffer.size(); i++) 
        {
            if (buffer[i].empty()) continue;
            cout << buffer[i] << '\n';
        }
    }
}

size_t HardDrive::getRecordsInBuffer()
{
    return records_In_Buffer;
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
