#pragma once
#include "Constants.h"
#include "HardDrive.h"

class Sorting
{
private:
    unsigned int records_In_Buffer = RECORDS_IN_BUFF;
    unsigned int number_Of_Buffers = NUMBER_OF_BUFF;
    HardDrive hard_drive;

    void CheckRecords(vector<string>& records);

    unsigned int CreateRuns(const string& outputFile = PHAZE_ONE_FILE, const string& inputFile = INPUT_FILE);
    void SaveRun(const vector<string>& records, const string& filename = PHAZE_ONE_FILE);

    unsigned int MergeRuns(unsigned int rune_size, unsigned int rune_index, const string& outputFile, const string& inputFile);
    unsigned int MergeAll(bool debug, unsigned int block_index, const string& inputFile = PHAZE_ONE_FILE, const string& tmpFile = PHAZE_TWO_FILE);

public:
    static bool CompareRecords(const string& a, const string& b);
    Sorting(HardDrive h_d, unsigned int n_b);

    void SortRecords(bool debug, const string& outputFile = PHAZE_TWO_FILE, const string& inputFile = INPUT_FILE, const string& tmpFile = PHAZE_ONE_FILE);

    virtual ~Sorting() = default;
};