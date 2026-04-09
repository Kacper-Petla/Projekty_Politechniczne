#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <ctime>
#include <cstdlib>
#include <queue>
#include <cstdint>
#include <cstring>
#include <algorithm>


using namespace std;

#define INDEX_ONE_FILE "index_one.txt"
#define INDEX_TWO_FILE "index_two.txt"
#define PRIMARY_ONE_FILE "primary_one.txt"
#define PRIMARY_TWO_FILE "primary_two.txt"
#define OVERFLOW_ONE_FILE "overflow_one.txt"
#define OVERFLOW_TWO_FILE "overflow_two.txt"


#define KEY_SIZE (sizeof(uint32_t))
#define DATA_SIZE (9)
#define POINTER_SIZE (sizeof(uint32_t))
#define INDEX_ENTRY_SIZE (KEY_SIZE + POINTER_SIZE)
#define RECORD_ENTRY_SIZE (KEY_SIZE + DATA_SIZE + POINTER_SIZE + 1)

#define WHEN_REORGANIZE (10)

#define PADDING ' '


