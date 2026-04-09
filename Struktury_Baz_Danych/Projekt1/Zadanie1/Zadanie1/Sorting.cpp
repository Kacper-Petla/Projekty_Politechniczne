#include "Sorting.h"

struct RecordEntry 
{
    string record;
    size_t rune_index;
};

struct RecordComparator 
{
    bool operator()(const RecordEntry& a, const RecordEntry& b) const 
    {
        if (a.record[0] == PADDING) return true;
        if (b.record[0] == PADDING) return false;
        //string numA = a.substr(3, 6);
        //string numB = b.substr(3, 6);

        //int valA = stoi(numA);
        //int valB = stoi(numB);

        //return valA < valB;
        return a.record > b.record;
    }
};

bool Sorting::CompareRecords(const string& a, const string& b)  //porownanie rekordow wedugl kryteriow
{
    if (a[0] == PADDING) return false;
    if (b[0] == PADDING) return true;
    //string numA = a.substr(3, 6);
    //string numB = b.substr(3, 6);

    //int valA = stoi(numA);
    //int valB = stoi(numB);

    //return valA < valB;
    return a < b;
}



Sorting::Sorting(HardDrive h_d, unsigned int n_b) : hard_drive(h_d)
{
    number_Of_Buffers = n_b;
    records_In_Buffer = hard_drive.getRecordsInBuffer();
}

void Sorting::CheckRecords(vector<string>& records) //sprawdzanie czy rekordy maja poprawny format
{
    for (size_t i = 0; i < records.size();)
    {
        bool valid = true;
        if (records[i].size() != RECORD_LENGTH)
        {
            valid = false;
        }
        else
        {
            for (size_t j = 0; j < 3; j++)
            {
                if (!isupper(static_cast<unsigned char>(records[i][j])))
                {
                    valid = false;
                    break;
                }
            }
            for (size_t j = 3; j < 9 && valid; j++)
            {
                if (!isdigit(static_cast<unsigned char>(records[i][j]))) {
                    valid = false;
                    break;
                }
            }
        }

        if (!valid)
        {
            records.erase(records.begin() + i);
        }
        else
        {
            i++;
        }
    }
}

unsigned int Sorting::CreateRuns(const string& outputFile, const string& inputFile)     //uproszczone tworzenie run
{
    if (!hard_drive.CleanOrCreateFile(outputFile)) return 0;

    unsigned int run_index = 0;
    vector<string> all_buffers; //wszystkie buffory sklejone w jeden wielki
    all_buffers.reserve(number_Of_Buffers * records_In_Buffer * RECORD_LENGTH);
    vector<string> buffer;
    unsigned int block_index = 0;

    while (hard_drive.LoadFromFile(buffer, block_index, inputFile))
    {
        block_index++;

        CheckRecords(buffer);

        all_buffers.insert(all_buffers.end(), buffer.begin(), buffer.end());

        //jesli zapelniono wszystkie buffory blokami
        if (block_index % number_Of_Buffers ==0)
        {
            run_index++;
            
            sort(all_buffers.begin(), all_buffers.end(), CompareRecords);

            SaveRun(all_buffers, outputFile);

            all_buffers.clear();
        }
    }

    //jesli cos zostalo w pamieci
    if (!all_buffers.empty())
    {
        run_index++;
        sort(all_buffers.begin(), all_buffers.end(), CompareRecords);
        SaveRun(all_buffers, outputFile);
    }

    return block_index;
}

void Sorting::SaveRun(const vector<string>& records, const string& filename)    //zapisanie blokow
{
    string buffer;
    buffer.reserve(records_In_Buffer * RECORD_LENGTH);

    for (size_t i = 0; i < number_Of_Buffers * records_In_Buffer; i++)
    {
        if (i < records.size())
        {
            buffer += records[i];
        }
        if ((i + 1) % records_In_Buffer == 0)
        {
            hard_drive.SaveToFile(buffer, filename);
            buffer.clear();
            if (i > records.size()) break;
        }
    }
}

unsigned int Sorting::MergeRuns(unsigned int rune_size, unsigned int rune_index, const string& outputFile, const string& inputFile) //scalanie b-1 run
{
    const unsigned int number_of_runes = number_Of_Buffers - 1;
    unsigned int count_saved_buffers = 0;
    vector<vector<string>> input_buffers(number_of_runes);
    vector<unsigned int> runes_indexes(number_of_runes, 0);

    for (unsigned int i = 0; i < number_of_runes; i++)  //poczatkowe zapelnienie buffora
    {
        unsigned int first_block = rune_index + (rune_size * i);
        hard_drive.LoadFromFile(input_buffers[i], first_block, inputFile);
        runes_indexes[i] = first_block;
    }

    priority_queue<RecordEntry, vector<RecordEntry>, RecordComparator> pq;

    for (size_t i = 0; i < number_of_runes; i++)    //poczatkowe zapelnienie kolejki
    {
        if (!input_buffers[i].empty()) 
        {
            //if (input_buffers[i][0][0] != PADDING)
            //{
                pq.push({ input_buffers[i][0], i });
            //}
            input_buffers[i].erase(input_buffers[i].begin());
        }
    }

    string output_buffer;
    output_buffer.reserve(records_In_Buffer * RECORD_LENGTH);

    while (!pq.empty()) 
    {
        RecordEntry min_rec = pq.top();
        pq.pop();

        output_buffer += min_rec.record;

        if (output_buffer.size() >= records_In_Buffer * RECORD_LENGTH)  //zapisanie pelnego buffora wyjsciowego
        {
            count_saved_buffers++;
            hard_drive.SaveToFile(output_buffer, outputFile);
            output_buffer.clear();
        }

        size_t rune_idx = min_rec.rune_index;

        if (!input_buffers[rune_idx].empty())   //kolejny rekord do kolejki
        {
            //if (input_buffers[rune_idx][0][0] != PADDING)
            //{
                pq.push({ input_buffers[rune_idx][0], rune_idx });
            //}
            input_buffers[rune_idx].erase(input_buffers[rune_idx].begin());
        }
        else    //uzupelnienie pustego buffora wejsciowego i wpisanie kolejnego rekordu do kolejki
        {
            unsigned int current_block = runes_indexes[rune_idx];
            unsigned int rune_end_block = rune_index + (rune_idx * rune_size) + rune_size - 1;
            if (current_block < rune_end_block)
            {
                runes_indexes[rune_idx]++;
                if (hard_drive.LoadFromFile(input_buffers[rune_idx], runes_indexes[rune_idx], inputFile))
                {
                    if (!input_buffers[rune_idx].empty())
                    {
                        //if (input_buffers[rune_idx][0][0] != PADDING)
                        //{
                            pq.push({ input_buffers[rune_idx][0], rune_idx });
                        //}
                        input_buffers[rune_idx].erase(input_buffers[rune_idx].begin());
                    }
                }
            }
        }
    }

    if (!output_buffer.empty())     //zapisanie niepelnego buffora
    {
        count_saved_buffers++;
        hard_drive.SaveToFile(output_buffer, outputFile);
        output_buffer.clear();
    }

    return count_saved_buffers;
}

unsigned int Sorting::MergeAll(bool debug, unsigned int tmp_block_index, const string& inputFile, const string& outputFile)   //sklejenie wszystkich rekordow
{
    vector<string> buffer;
    unsigned int block_index = tmp_block_index;
    unsigned int number_of_blocks = tmp_block_index;
    string current_input = inputFile;
    string current_output = outputFile;
    unsigned int pass = 1;
    unsigned int start;
    unsigned int runes_number = 0;

    if (!hard_drive.CleanOrCreateFile(current_output)) return 0;

    unsigned int rune_size = number_Of_Buffers;
    const unsigned int M = number_Of_Buffers - 1;

    while (true)
    {

        if (number_of_blocks <= rune_size)   //jedna runa
        {
            break;
        }

        start = 0;
        while (true)
        {
            //if (!hard_drive.LoadFromFile(buffer, start, current_input)) break;
            if (start >= number_of_blocks) break;
            
            block_index = MergeRuns(rune_size, start, current_output, current_input);
            start += rune_size * M;
        }
        if (debug)
        {
            int reads = hard_drive.getReads();
            int writes = hard_drive.getWrites();

            cout << "\n=== REKORDY PO FAZIE SCALANIA #" << pass << " (rune_size = " << rune_size << " blokow) ===\n";
            hard_drive.WriteRecords(current_output);

            hard_drive.setReads(reads);
            hard_drive.setWrites(writes);
        }
        hard_drive.CleanOrCreateFile(current_input);
        swap(current_input, current_output);

        rune_size = rune_size * M;
        pass++;
    }

    if (current_input != outputFile)
    {
        int reads = hard_drive.getReads();
        int writes = hard_drive.getWrites();

        hard_drive.CopyRecords(current_input, current_output);

        hard_drive.setReads(reads);
        hard_drive.setWrites(writes);
    }

    return pass-1;
}

void Sorting::SortRecords(bool debug, const string& outputFile, const string& inputFile, const string& tmpFile)
{
    system("cls");
    cout << "\n=== REKORDY PRZED SORTOWANIEM ===\n"<<endl;
    hard_drive.WriteRecords(inputFile);

    hard_drive.setReads(0);
    hard_drive.setWrites(0);

    unsigned int block_number = CreateRuns(tmpFile, inputFile);
    if (debug)
    {
        int reads = hard_drive.getReads();
        int writes = hard_drive.getWrites();

        cout << "\n=== REKORDY PO QUICKSORT ===\n";
        hard_drive.WriteRecords(tmpFile);

        hard_drive.setReads(reads);
        hard_drive.setWrites(writes);
    }
    unsigned int pass = MergeAll(debug, block_number, tmpFile, outputFile);

    int reads = hard_drive.getReads();
    int writes = hard_drive.getWrites();

    cout << "\n=== REKORDY PO SORTOWANIU ===\n";
    hard_drive.WriteRecords(outputFile);
    cout << "\n=== FAZY SCALANIA ===\n";
    cout << pass+1;
    cout << "\n=== OPERACJE DYSKOWE ===\n";
    cout << "zapisy: " << writes << ", odczyty: " << reads;
}
