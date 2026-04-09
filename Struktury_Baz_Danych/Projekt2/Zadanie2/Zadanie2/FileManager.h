#pragma once
#include "Constants.h"
#include "HardDrive.h"

struct IndexEntry
{
    uint32_t key;   // 4 B
    uint32_t page;  // 4 B
};

struct RecordEntry
{
    uint32_t key;           // 4 B
    char data[9];           // 9 B: rekord
    uint32_t next_pointer;  // 4 B: numer strony lub UINT32_MAX == null
    bool is_deleted;        // 1 B
};

class FileManager
{
private:
    unsigned int page_Size;
    float alpha;
    string current_Index_File;
    string tmp_Index_File;
    string current_Primary_File;
    string tmp_Primary_File;
    string current_Overflow_File;
    string tmp_Overflow_File;
    HardDrive* hard_Drive;
    uint32_t first_Overflow_Key = UINT32_MAX;
    uint32_t first_Overflow_Pointer = UINT32_MAX;
    uint32_t records_In_Overflow = 0;

    string SerializeIndexPage(const vector<IndexEntry>& buffer);
    void DeserializeIndexPage(const string& page, vector<IndexEntry>& out);

    string SerializeRecordPage(const vector<RecordEntry>& buffer);
    void DeserializeRecordPage(const string& page, vector<RecordEntry>& out);

    void PrintOverflow(uint32_t pointer);

    bool FindInOverflow(uint32_t key, uint32_t pointer, vector<RecordEntry>& founded_records, uint32_t& founded_page_index, uint32_t& founded_in_page_index, bool& in_overflow);
    bool FindInPrimary(uint32_t key, uint32_t page_index, vector<RecordEntry>& founded_records, uint32_t& founded_page_index, uint32_t& founded_in_page_index, bool& in_overflow);

    uint32_t AddNewRecordToOverflow(RecordEntry record);
    uint32_t FindPageToInsert(uint32_t key);
    bool WhereToInsertPointerInOverflow(uint32_t key, uint32_t& start_node_ptr, uint32_t& prev_node_ptr);
    void UpdateOverflowPointer(uint32_t pointer_to_update, uint32_t new_next_pointer);

public:
    bool SaveIndex(vector<IndexEntry>& buffer, unsigned int page_index, const string& file);
    bool LoadIndex(vector<IndexEntry>& buffer, unsigned int page_index, const string& file);

    bool SaveRecord(vector<RecordEntry>& buffer, unsigned int page_index, const string& file);
    bool LoadRecord(vector<RecordEntry>& buffer, unsigned int page_index, const string& file);

    void PrintIndexStructure();
    void PrintPrimaryStructure();
    void PrintOverflowStructure();

    bool FindRecord(uint32_t key, vector<RecordEntry>& founded_records, uint32_t& founded_page_index, uint32_t& founded_in_page_index, bool& in_overflow);
    void PrintRecords();
    bool AddRecord(string data, uint32_t key);
    void Reorganize();
    bool EditRecord(string data, uint32_t key);
    bool DeleteRecord(uint32_t key);

    void RunScript(const string& filename);

    uint32_t getRecordsInOverflow();

    FileManager(HardDrive* hard_drive, unsigned int page_size, float a);
    virtual ~FileManager() = default;
};