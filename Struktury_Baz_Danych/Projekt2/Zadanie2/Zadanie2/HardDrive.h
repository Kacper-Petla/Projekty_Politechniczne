#pragma once
#include "Constants.h"

class HardDrive
{
private:
    size_t page_Size;

    int writes;
    int reads;

public:
    HardDrive(unsigned int page_size);
    bool CleanOrCreateFile(const string& filename);
    bool SaveToFile(const string& buffer, unsigned int page_index, const string& filename);
    bool LoadFromFile(string& buffer, unsigned int page_index, const string& filename);

    void CopyRecords(const string& source_filename, const string& dest_filename);

    int getWrites();
    int getReads();
    void setWrites(int w);
    void setReads(int r);
    void resetStats();

    virtual ~HardDrive() = default;
};
