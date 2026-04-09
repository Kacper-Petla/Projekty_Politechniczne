#include "FileManager.h"

FileManager::FileManager(HardDrive* hard_drive, unsigned int page_size, float a)
{
    hard_Drive = hard_drive;
    page_Size = page_size;
    alpha = a;
    first_Overflow_Pointer = UINT32_MAX;
    first_Overflow_Key = UINT32_MAX;
    records_In_Overflow = 0;

    current_Index_File = INDEX_ONE_FILE;
    tmp_Index_File = INDEX_TWO_FILE;
    current_Primary_File = PRIMARY_ONE_FILE;
    tmp_Primary_File = PRIMARY_TWO_FILE;
    current_Overflow_File = OVERFLOW_ONE_FILE;
    tmp_Overflow_File = OVERFLOW_TWO_FILE;
}

// wczytywanie/zapisywanie blokowe z konwersja na latwe do zarzadzania dane

//index page

static bool IsPadding(const string& page, size_t offset, size_t entry_size, char padding)
{
    const char* ptr = page.data() + offset;
    for (size_t k = 0; k < entry_size; ++k)
    {
        if (ptr[k] != padding) return false;
    }
    return true;
}

string FileManager::SerializeIndexPage(const vector<IndexEntry>& buffer)
{
    string page(page_Size, PADDING);
    size_t max_entries = page_Size / INDEX_ENTRY_SIZE;
    size_t to_write = min(buffer.size(), max_entries);

    for (size_t i = 0; i < to_write; i++)
    {
        size_t offset = i * INDEX_ENTRY_SIZE;
        const IndexEntry& b = buffer[i];
        memcpy(&page[offset], &b.key, KEY_SIZE);
        memcpy(&page[offset + KEY_SIZE], &b.page, POINTER_SIZE);
    }
    return page;
}

void FileManager::DeserializeIndexPage(const string& page, vector<IndexEntry>& out)
{
    size_t max_entries = page.size() / INDEX_ENTRY_SIZE;
    out.clear();
    for (size_t i = 0; i < max_entries; i++)
    {
        size_t offset = i * INDEX_ENTRY_SIZE;

        if (IsPadding(page, offset, INDEX_ENTRY_SIZE, PADDING)) break;

        IndexEntry b;
        memcpy(&b.key, page.data() + offset, KEY_SIZE);
        memcpy(&b.page, page.data() + offset + KEY_SIZE, POINTER_SIZE);
        out.push_back(b);
    }
}

bool FileManager::SaveIndex(vector<IndexEntry>& buffer, unsigned int page_index, const string& file)
{
    
    string page = SerializeIndexPage(buffer);

    if (!hard_Drive->SaveToFile(page, page_index, file)) return false;
    return true;
}

bool FileManager::LoadIndex(vector<IndexEntry>& buffer, unsigned int page_index, const string& file)
{
    string page;

    if (!hard_Drive->LoadFromFile(page, page_index, file)) return false;

    buffer.clear();
    DeserializeIndexPage(page, buffer);
    return true;
}

//primary / overflow page

string FileManager::SerializeRecordPage(const vector<RecordEntry>& buffer)
{
    string page(page_Size, PADDING);
    size_t max_entries = page_Size / RECORD_ENTRY_SIZE;
    size_t to_write = min(buffer.size(), max_entries);

    for (size_t i = 0; i < to_write; i++)
    {
        size_t offset = i * RECORD_ENTRY_SIZE;
        const RecordEntry& b = buffer[i];
        memcpy(&page[offset], &b.key, KEY_SIZE);
        memcpy(&page[offset + KEY_SIZE], b.data, DATA_SIZE);
        memcpy(&page[offset + KEY_SIZE + DATA_SIZE], &b.next_pointer, POINTER_SIZE);
        memcpy(&page[offset + KEY_SIZE + DATA_SIZE + POINTER_SIZE], &b.is_deleted, sizeof(bool));
    }
    return page;
}

void FileManager::DeserializeRecordPage(const string& page, vector<RecordEntry>& out)
{
    size_t max_entries = page.size() / RECORD_ENTRY_SIZE;
    out.clear();
    for (size_t i = 0; i < max_entries; i++)
    {
        size_t offset = i * RECORD_ENTRY_SIZE;

        if (IsPadding(page, offset, RECORD_ENTRY_SIZE, PADDING)) break;

        RecordEntry b;
        memcpy(&b.key, page.data() + offset, KEY_SIZE);
        memcpy(b.data, page.data() + offset + KEY_SIZE, DATA_SIZE);
        memcpy(&b.next_pointer, page.data() + offset + KEY_SIZE + DATA_SIZE, POINTER_SIZE);
        memcpy(&b.is_deleted, page.data() + offset + KEY_SIZE + DATA_SIZE + POINTER_SIZE, sizeof(bool));
        out.push_back(b);
    }
}

bool FileManager::SaveRecord(vector<RecordEntry>& buffer, unsigned int page_index, const string& file)
{

    string page = SerializeRecordPage(buffer);

    if (!hard_Drive->SaveToFile(page, page_index, file)) return false;
    return true;
}

bool FileManager::LoadRecord(vector<RecordEntry>& buffer, unsigned int page_index, const string& file)
{
    string page;

    if (!hard_Drive->LoadFromFile(page, page_index, file)) return false;

    buffer.clear();
    DeserializeRecordPage(page, buffer);
    return true;
}

//wyswietlanie zawartosci plikow (rzeczywista organizacja)

void FileManager::PrintIndexStructure()
{
    cout << "\n=== STRUKTURA PLIKU INDEKSU ===" << endl;
    vector<IndexEntry> buffer;
    unsigned int page_index = 0;

    while (LoadIndex(buffer, page_index, current_Index_File))
    {
        cout << "[Strona Indeksu " << page_index << "]: " << endl;
        if (buffer.empty())
        {
            cout << "(Pusta)" << endl;
        }
        else
        {
            for (const auto& entry : buffer)
            {
                cout << "{Key:" << entry.key << " -> Page:" << entry.page << "} " << endl;
            }
            cout << endl;
        }
        page_index++;
    }
    cout << "===============================\n";
}

void FileManager::PrintPrimaryStructure()
{
    cout << "\n=== STRUKTURA PLIKU PRIMARY (GLOWNEGO) ===" << endl;
    vector<RecordEntry> buffer;
    unsigned int page_index = 0;
    unsigned int max_entries = page_Size / RECORD_ENTRY_SIZE;

    while (LoadRecord(buffer, page_index, current_Primary_File))
    {
        cout << "--- Strona " << page_index << " --- (Zajete: " << buffer.size() << "/" << max_entries << ")" << endl;

        for (unsigned int i = 0; i < buffer.size(); i++)
        {
            const RecordEntry& r = buffer[i];
            cout << " [" << i << "] Key: " << r.key;
            cout << " | Data: " << string(r.data, DATA_SIZE);
            cout << " | Pointer: " << (r.next_pointer == UINT32_MAX ? "NULL" : to_string(r.next_pointer));
            cout << " | Flag D: " << (r.is_deleted ? "TRUE" : "FALSE");
            cout << endl;
        }

        for (unsigned int i = buffer.size(); i < max_entries; i++)
        {
            cout << " [" << i << "] <WOLNE MIEJSCE>" << endl;
        }

        page_index++;
    }
    cout << "==========================================\n";
}

void FileManager::PrintOverflowStructure()
{
    cout << "\n=== STRUKTURA PLIKU OVERFLOW (NADMIAROWEGO) ===" << endl;
    cout << "First Overflow Pointer: " << (first_Overflow_Pointer == UINT32_MAX ? "NULL" : to_string(first_Overflow_Pointer)) << endl;

    vector<RecordEntry> buffer;
    unsigned int page_index = 0;
    unsigned int max_entries = page_Size / RECORD_ENTRY_SIZE;

    while (LoadRecord(buffer, page_index, current_Overflow_File))
    {
        for (unsigned int i = 0; i < buffer.size(); i++)
        {
            uint32_t global_addr = (page_index * max_entries) + i;
            const RecordEntry& r = buffer[i];

            cout << "[" << global_addr << "] Key: " << r.key;
            cout << " | Data: " << string(r.data, DATA_SIZE);
            cout << " | Pointer: " << (r.next_pointer == UINT32_MAX ? "NULL" : to_string(r.next_pointer));
            cout << " | Flag D: " << (r.is_deleted ? "TRUE" : "FALSE");
            cout << endl;
        }
        page_index++;
    }
    cout << "===============================================\n";
}

//wyswietlanie ulozenia rekordow w strukturze

void FileManager::PrintOverflow(uint32_t pointer)
{
    vector<RecordEntry> buffer;
    unsigned int max_entries = page_Size / RECORD_ENTRY_SIZE;

    while (pointer != UINT32_MAX)
    {
        unsigned int page_index = pointer / max_entries;
        unsigned int in_page_index = pointer % max_entries;
        LoadRecord(buffer, page_index, current_Overflow_File);
        if (!buffer[in_page_index].is_deleted)
        {
            cout << "Key: " << buffer[in_page_index].key << ", Data: " << string(buffer[in_page_index].data, DATA_SIZE) << endl;
        }
        pointer = buffer[in_page_index].next_pointer;
    }
}

void FileManager::PrintRecords()
{
    vector<RecordEntry> buffer;
    unsigned int max_entries = page_Size / RECORD_ENTRY_SIZE;
    unsigned int page_index = 0;

    PrintOverflow(first_Overflow_Pointer);
    while (LoadRecord(buffer, page_index, current_Primary_File))
    {
        for (unsigned int i = 0; i < buffer.size(); i++)
        {
            const RecordEntry& b = buffer[i];
            if (!b.is_deleted)
            {
                cout << "Key: " << b.key << ", Data: " << string(b.data, DATA_SIZE) << endl;
            }
            PrintOverflow(b.next_pointer);
        }
        page_index++;
    }

}

//szukanie rekordu o danym kluczu w strukturze

bool FileManager::FindInOverflow(uint32_t key, uint32_t pointer, vector<RecordEntry>& founded_records, uint32_t& founded_page_index, uint32_t& founded_in_page_index, bool& in_overflow)
{
    vector<RecordEntry> buffer;
    unsigned int max_entries = page_Size / RECORD_ENTRY_SIZE;

    while (pointer != UINT32_MAX)
    {
        unsigned int page_index = pointer / max_entries;
        unsigned int in_page_index = pointer % max_entries;

        if (!LoadRecord(buffer, page_index, current_Overflow_File))
        {
            return false;
        }

        if (in_page_index >= buffer.size())
        {
            return false;
        }

        const RecordEntry& b = buffer[in_page_index];

        if (b.key == key)
        {
            if (b.is_deleted)
            {
                return false;
            }
            founded_records = buffer;
            founded_page_index = page_index;
            founded_in_page_index = in_page_index;
            in_overflow = true;
            return true;
        }

        pointer = b.next_pointer;
    }

    return false;
}

bool FileManager::FindInPrimary(uint32_t key, uint32_t page_index, vector<RecordEntry>& founded_records, uint32_t& founded_page_index, uint32_t& founded_in_page_index, bool& in_overflow)
{
    vector<RecordEntry> buffer;
    uint32_t previous_pointer;

    if (!LoadRecord(buffer, page_index, current_Primary_File))
    {
        return false;
    }
    previous_pointer = buffer[0].next_pointer;

    for (unsigned int i = 0; i < buffer.size(); i++)
    {
        const RecordEntry& b = buffer[i];

        if (b.key == key)
        {
            if (b.is_deleted)
            {
                return false;
            }
            founded_records = buffer;
            founded_page_index = page_index;
            founded_in_page_index = i;
            in_overflow = false;
            return true;
        }

        if (b.key > key)
        {
            return FindInOverflow(key, previous_pointer, founded_records, founded_page_index, founded_in_page_index, in_overflow);
        }
        previous_pointer = b.next_pointer;
    }

    return FindInOverflow(key, previous_pointer, founded_records, founded_page_index, founded_in_page_index, in_overflow);
}

bool FileManager::FindRecord(uint32_t key, vector<RecordEntry>& founded_records, uint32_t& founded_page_index, uint32_t& founded_in_page_index, bool& in_overflow)
{
    vector<IndexEntry> buffer;
    unsigned int page_index = 0;
    uint32_t previous_key = first_Overflow_Key;
    uint32_t previous_pointer = first_Overflow_Pointer;

    if (previous_pointer != UINT32_MAX)
    {
        if (key < previous_key)
        {
            return false;                                           //jesli klucz mniejszy od najmniejszego klucza
        }
        if (LoadIndex(buffer, page_index, current_Index_File))
        {
            if (key < buffer[0].key)
            {
                return FindInOverflow(key, previous_pointer, founded_records, founded_page_index, founded_in_page_index, in_overflow);       //jesli klucz moze byc w overflow (przed rekordami primary)
            }
            previous_key = buffer[0].key;
            previous_pointer = buffer[0].page;
        }
        else
        {
            return FindInOverflow(key, previous_pointer, founded_records, founded_page_index, founded_in_page_index, in_overflow);           //jesli klucz moze byc w overflow (primary puste)
        }
    }
    else if (LoadIndex(buffer, page_index, current_Index_File))
    {
        if (key < buffer[0].key)
        {
            return false;                                           //jesli klucz mniejszy od najmniejszego klucza
        }
        previous_key = buffer[0].key;
        previous_pointer = buffer[0].page;
    }
    else
    {
        return false;                                               //struktura pusta
    }

    unsigned int i = 1;
    while (true)
    {
        for (; i < buffer.size(); i++)
        {
            if (key >= previous_key && key < buffer[i].key)
            {
                return FindInPrimary(key, previous_pointer, founded_records, founded_page_index, founded_in_page_index, in_overflow);       //jesli klucz moze byc na danej stronie w primary
            }
            previous_pointer = buffer[i].page;
            previous_key = buffer[i].key;
        }

        i = 0;
        page_index++;
        if (!LoadIndex(buffer, page_index, current_Index_File)) break;
    }
    return FindInPrimary(key, previous_pointer, founded_records, founded_page_index, founded_in_page_index, in_overflow);                    //jesli klucz moze byc na ostatniej stronie w primary                                     
    
}


//dodanie nowego rekordu

uint32_t FileManager::AddNewRecordToOverflow(RecordEntry record)             //dodaje rekord do overflow i zwraca pointer
{
    vector<RecordEntry> buffer;
    unsigned int max_entries = page_Size / RECORD_ENTRY_SIZE;
    unsigned int page_index = records_In_Overflow / max_entries;
    unsigned int records_in_page = records_In_Overflow % max_entries;

    if (!LoadRecord(buffer, page_index, current_Overflow_File))
    {
        buffer.clear();
    }

    buffer.push_back(record);
    SaveRecord(buffer, page_index, current_Overflow_File);

    records_In_Overflow++;
    return records_In_Overflow - 1;
}

uint32_t FileManager::FindPageToInsert(uint32_t key)
{
    vector<IndexEntry> index_buffer;
    unsigned int page_index = 0;
    uint32_t primary_page_to_check = UINT32_MAX;

    //edge-case (klucz mniejszy ni¿ najmniejszy rekord w primary / primary puste / primary i overflow puste)
    if (LoadIndex(index_buffer, page_index, current_Index_File))
    {
        if (key < index_buffer[0].key)
        {
            return UINT32_MAX;
        }
        primary_page_to_check = index_buffer[0].page;
    }
    else
    {
        return UINT32_MAX;
    }

    
    unsigned int i = 1;
    while (true)
    {
        for (; i < index_buffer.size(); i++)
        {
            if (key < index_buffer[i].key)
            {
                return primary_page_to_check;
            }
            primary_page_to_check = index_buffer[i].page;
        }

        i = 0;
        page_index++;
        if (!LoadIndex(index_buffer, page_index, current_Index_File)) break;
    }

    return primary_page_to_check;
}

//przeszukuje overflow i decyduje gdzie zapisac pointer na nowy rekord, start_node_ptr zwraca pointer do zapisania w rekordzie, prev_node_ptr zwraca pointer do wpisania pointera na rekord
bool FileManager::WhereToInsertPointerInOverflow(uint32_t key, uint32_t& start_node_ptr, uint32_t& prev_node_ptr)
{
    vector<RecordEntry> buffer;
    unsigned int max_entries = page_Size / RECORD_ENTRY_SIZE;

    uint32_t current = start_node_ptr;
    uint32_t prev = UINT32_MAX;

    while (current != UINT32_MAX)
    {
        unsigned int page_index = current / max_entries;
        unsigned int in_page_index = current % max_entries;

        if (!LoadRecord(buffer, page_index, current_Overflow_File))
        {
            return false;
        }

        if (in_page_index >= buffer.size())
        {
            return false;
        }

        const RecordEntry& b = buffer[in_page_index];

        if (b.key == key)
        {
            return false;
        }
        if (b.key > key)
        {
            start_node_ptr = current;
            prev_node_ptr = prev;
            return true;
        }

        prev = current;
        current = b.next_pointer;
    }

    start_node_ptr = UINT32_MAX;
    prev_node_ptr = prev;
    return true;
}

void FileManager::UpdateOverflowPointer(uint32_t pointer_to_update, uint32_t new_next_pointer)
{
    unsigned int max_entries = page_Size / RECORD_ENTRY_SIZE;
    unsigned int page_index = pointer_to_update / max_entries;
    unsigned int slot_index = pointer_to_update % max_entries;

    vector<RecordEntry> buffer;
    if (LoadRecord(buffer, page_index, current_Overflow_File))
    {
        if (slot_index < buffer.size())
        {
            buffer[slot_index].next_pointer = new_next_pointer;
            SaveRecord(buffer, page_index, current_Overflow_File);
        }
    }
}
bool FileManager::AddRecord(string data, uint32_t key)
{
    RecordEntry record;
    record.key = key;
    record.next_pointer = UINT32_MAX;
    record.is_deleted = false;
    memset(record.data, PADDING, DATA_SIZE);
    size_t bytes_to_copy = min(data.size(), (size_t)DATA_SIZE);
    memcpy(record.data, data.data(), bytes_to_copy);

    uint32_t primary_page_index;
    unsigned int max_entries = page_Size / RECORD_ENTRY_SIZE;

    primary_page_index = FindPageToInsert(key);

    // Klucz jest mniejszy ni¿ najmniejszy klucz w Primary lub Primary jest puste
    if (primary_page_index == UINT32_MAX)
    {
        uint32_t next_ptr = first_Overflow_Pointer;
        uint32_t prev_ptr = UINT32_MAX;

        if (!WhereToInsertPointerInOverflow(key, next_ptr, prev_ptr))
        {
            cout << "Blad: Rekord o kluczu " << key << " juz istnieje (Indep. Overflow)." << endl;
            return false;
        }

        record.next_pointer = next_ptr;

        uint32_t new_record_address = AddNewRecordToOverflow(record);

        if (prev_ptr == UINT32_MAX)
        {
            first_Overflow_Pointer = new_record_address;

            if (first_Overflow_Key == UINT32_MAX || key < first_Overflow_Key)
            {
                first_Overflow_Key = key;
            }
        }
        else
        {
            UpdateOverflowPointer(prev_ptr, new_record_address);
        }
        return true;
    }

    //rekord potencjalnie mozna dodac na danej stronie
    vector<RecordEntry> primary_buffer;
    if (!LoadRecord(primary_buffer, primary_page_index, current_Primary_File))
    {
        return false;
    }

    //rekord zmiesci sie na stronie
    if (primary_buffer.size() < max_entries)
    {
        auto it = primary_buffer.begin();
        while (it != primary_buffer.end() && it->key < key)
        {
            if (it->key == key)
            {
                cout << "Blad: Duplikat w Primary." << endl;
                return false;
            }
            it++;
        }

        if (it != primary_buffer.end() && it->key == key)
        {
            cout << "Blad: Duplikat w Primary." << endl;
            return false;
        }

        primary_buffer.insert(it, record);

        SaveRecord(primary_buffer, primary_page_index, current_Primary_File);
        return true;
    }
    //rekord trafi do overflow
    else
    {
        int anchor_slot = 0;

        for (int i = 0; i < primary_buffer.size(); i++)
        {
            if (primary_buffer[i].key == key) return false;

            if (primary_buffer[i].key < key)
            {
                anchor_slot = i;
            }
            else
            {
                break;
            }
        }

        uint32_t next_ptr = primary_buffer[anchor_slot].next_pointer;
        uint32_t prev_ptr = UINT32_MAX;

        if (!WhereToInsertPointerInOverflow(key, next_ptr, prev_ptr))
        {
            cout << "Blad: Duplikat w Overflow." << endl;
            return false;
        }

        record.next_pointer = next_ptr;
        uint32_t new_record_address = AddNewRecordToOverflow(record);

        if (prev_ptr == UINT32_MAX)
        {
            primary_buffer[anchor_slot].next_pointer = new_record_address;
            SaveRecord(primary_buffer, primary_page_index, current_Primary_File);
        }
        else
        {
            UpdateOverflowPointer(prev_ptr, new_record_address);
        }
        return true;
    }
}

bool FileManager::EditRecord(string data, uint32_t key)
{
    vector<RecordEntry> records;
    uint32_t page_index;
    uint32_t in_page_index;
    bool in_overflow;
    if (!FindRecord(key, records, page_index, in_page_index, in_overflow))
    {
        return false;
    }

    string old_data(records[in_page_index].data, DATA_SIZE);

    memset(records[in_page_index].data, PADDING, DATA_SIZE);
    size_t bytes_to_copy = min(data.size(), (size_t)DATA_SIZE);
    memcpy(records[in_page_index].data, data.data(), bytes_to_copy);

    cout << "Edycja (Klucz " << key << "): '" << old_data << "' -> '" << data << "'" << endl;

    if (in_overflow)
    {
        SaveRecord(records, page_index, current_Overflow_File);
    }
    else
    {
        SaveRecord(records, page_index, current_Primary_File);
    }
    return true;
}

bool FileManager::DeleteRecord(uint32_t key)
{
    vector<RecordEntry> records;
    uint32_t page_index;
    uint32_t in_page_index;
    bool in_overflow;
    if (!FindRecord(key, records, page_index, in_page_index, in_overflow))
    {
        return false;
    }

    records[in_page_index].is_deleted = true;

    if (in_overflow)
    {
        SaveRecord(records, page_index, current_Overflow_File);
    }
    else
    {
        SaveRecord(records, page_index, current_Primary_File);
    }
    return true;
}

void FileManager::Reorganize()
{

    if (!hard_Drive->CleanOrCreateFile(tmp_Primary_File) || !hard_Drive->CleanOrCreateFile(tmp_Overflow_File) || !hard_Drive->CleanOrCreateFile(tmp_Index_File))
    {
        cout << "Blad: Nie mozna utworzyc plikow tymczasowych." << endl;
        return;
    }

    vector<RecordEntry> write_buffer;
    vector<IndexEntry> new_index_buffer;
    unsigned int new_record_page_index = 0;
    unsigned int new_index_page_index = 0;

    unsigned int max_record_entries = page_Size / RECORD_ENTRY_SIZE;
    unsigned int max_index_entries = page_Size / INDEX_ENTRY_SIZE;
    unsigned int primary_entries_limit = static_cast<unsigned int>(max_record_entries * alpha);
    if (primary_entries_limit == 0) primary_entries_limit = 1;


    auto FlushPage = [&](bool force = false)
        {
            if (write_buffer.empty()) return;

            if (write_buffer.size() >= primary_entries_limit || force)
            {
                IndexEntry idx;
                idx.key = write_buffer[0].key;
                idx.page = new_record_page_index;
                new_index_buffer.push_back(idx);

                if (new_index_buffer.size() >= max_index_entries)
                {
                    SaveIndex(new_index_buffer, new_index_page_index, tmp_Index_File);
                    new_index_buffer.clear();
                    new_index_page_index++;
                }

                SaveRecord(write_buffer, new_record_page_index, tmp_Primary_File);

                write_buffer.clear();
                new_record_page_index++;
            }
        };

    auto ProcessRecord = [&](RecordEntry& r)
        {
            if (r.is_deleted) return;

            r.next_pointer = UINT32_MAX;
            write_buffer.push_back(r);
            FlushPage(false);
        };


    // Rekordy mniejsze ni¿ cokolwiek w Primary
    uint32_t current_ptr = first_Overflow_Pointer;
    vector<RecordEntry> read_buffer;

    while (current_ptr != UINT32_MAX)
    {
        unsigned int page_index = current_ptr / max_record_entries;
        unsigned int in_page_index = current_ptr % max_record_entries;

        if (LoadRecord(read_buffer, page_index, current_Overflow_File))
        {
            RecordEntry r = read_buffer[in_page_index];
            current_ptr = r.next_pointer;

            ProcessRecord(r); // Dodaj do nowej struktury
        }
        else break;
    }

    // Rekordy w primary
    unsigned int old_page_index = 0;
    while (LoadRecord(read_buffer, old_page_index, current_Primary_File))
    {
        for (unsigned int i = 0; i < read_buffer.size(); i++)
        {
            RecordEntry r = read_buffer[i];

            uint32_t overflow_ptr = r.next_pointer;
            ProcessRecord(r);

            // Overflow tego rekordu
            while (overflow_ptr != UINT32_MAX)
            {
                unsigned int page_index = overflow_ptr / max_record_entries;
                unsigned int in_page_index = overflow_ptr % max_record_entries;

                vector<RecordEntry> o_e;
                if (LoadRecord(o_e, page_index, current_Overflow_File))
                {
                    overflow_ptr = o_e[in_page_index].next_pointer;

                    ProcessRecord(o_e[in_page_index]);
                }
                else break;
            }
        }
        old_page_index++;
    }

    
    FlushPage(true);
    if (!new_index_buffer.empty())
    {
        SaveIndex(new_index_buffer, new_index_page_index, tmp_Index_File);
    }

    first_Overflow_Pointer = UINT32_MAX;
    first_Overflow_Key = UINT32_MAX;
    records_In_Overflow = 0;

    swap(current_Primary_File, tmp_Primary_File);
    swap(current_Overflow_File, tmp_Overflow_File);
    swap(current_Index_File, tmp_Index_File);

}

void FileManager::RunScript(const string& filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Nie mozna otworzyc pliku skryptu: " << filename << endl;
        return;
    }

    char command;
    while (file >> command)
    {

        if (command == 'A') // ADD: A <key> <data>
        {
            uint32_t key;
            string data;
            file >> key >> data;

            AddRecord(data, key);
        }
        else if (command == 'R') // REORGANIZE; R
        {
            Reorganize();
        }
        else if (command == 'E') // EDIT: E <key> <new_data>
        {
            uint32_t key;
            string data;
            file >> key >> data;
            
            EditRecord(data, key);
        }
        else if (command == 'D') // DELETE: D <key>
        {
            uint32_t key;
            file >> key;

            DeleteRecord(key);
        }
    }
    file.close();
}

uint32_t FileManager::getRecordsInOverflow()
{
    return records_In_Overflow;
}