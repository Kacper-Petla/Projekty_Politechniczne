#pragma once
#include "Constants.h"

class HardDrive
{
    private:
        size_t block_Size = BLOCK_SIZE;
        size_t records_In_Buffer = RECORDS_IN_BUFF;

        int writes = 0;
        int reads = 0;

    public:
        HardDrive(unsigned int records_in_buffer);
        bool CleanOrCreateFile(const string& filename = INPUT_FILE);
        bool SaveToFile(const string& buffer, const string& filename = INPUT_FILE);
        bool LoadFromFile(vector<string>& buffer, unsigned int block_index, const string& filename = INPUT_FILE);

        bool SaveSingleRecord(const string& record, const string& filename = INPUT_FILE);
        bool LoadSingleRecord(string& record, unsigned int block_index, unsigned int record_index, const string& filename = INPUT_FILE);
        
        void DrawRecords(unsigned int n_records, const string& filename = INPUT_FILE);
        void EnterRecords(const string& filename = INPUT_FILE);
        void CopyRecords(const string& source_filename , const string& dest_filename = INPUT_FILE);
        void WriteRecords(const string& filename = INPUT_FILE);

        size_t getRecordsInBuffer();

        int getWrites();
        int getReads();
        void setWrites(int w);
        void setReads(int r);
        void resetStats(); 
        
        virtual ~HardDrive() = default;
};
