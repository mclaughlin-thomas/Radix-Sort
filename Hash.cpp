#include "Hash.h"

// Given:  Nothing.
// 
// Task:   To initialize the hash table variables and create a dynamic array of size 1 for the hash table.
// 
// Return: Nothing.
HashTable::HashTable(void) {
    tableSize = 1;
    table = AllocateArrayNode(tableSize);
    headBucket = 0;
    occupiedBuckets = 0;
}

// Given:  size   - The essential number of buckets to hold all provided records.
// 
// Task:   To initialize the hash table variables and create a dynamic array of size "size" for the hash table.
// 
// Return: Nothing.
HashTable::HashTable(const int size) {
    tableSize = size * 3; // Multiplying by three to enlarge the table which will in return help reduce collisions.
    table = AllocateArrayNode(tableSize); // increasing size of hash table to reduce collisions.
    headBucket = 0;
    occupiedBuckets = 0;
    IncrementOccupiedBuckets();

}

// Given:  Nothing.
// 
// Task:   To destroy the hash table and release any allocated memory.
// 
// Return: Nothing.
HashTable::~HashTable(void) {
    //std::cout << "Deleted";
}

// Given:  arraySize    - The size of the array to be allocated.
// 
// Task:   To dynamically allocate an array of Node structs of size arraySize using a smart pointer.
// 
// Return: A std::unique_ptr to an array of Node structs of size arraySize.
std::unique_ptr<Node[]> HashTable::AllocateArrayNode(const int arraySize) {
    try {
        return std::make_unique<Node[]>(arraySize); // Allocate memory for Numbers dynamically (on the heap) | Auto deletes when out of scope.
    }
    catch (const std::bad_alloc& e) {
        // Handle failures
        std::cerr << "Memory allocation failed: " << e.what() << std::endl;
        return nullptr;
    }
}

// Given:  Nothing.
// 
// Task:   To simply return headBucket.
// 
// Return: tableSize       - The first occupied bucket in the hash table.
int HashTable::GetHead(void) {
    return headBucket;
}

// Given:  Nothing.
// 
// Task:   To simply return tableSize.
// 
// Return: tableSize       - The size of the table.
int HashTable::GetTableSize(void) {
    return tableSize;
}

// Given:  key  - Integer representing the key, which will be used to determine the hash value.
//         i    - Integer representing the current probe iteration.
// 
// Task:   To calculate the probe sequence index for the given key and probe iteration.
// 
// Return: The calculated index from the probe sequence.
int HashTable::Probe(const int key, const int i) {
    return ( (HashFunction1(key)) + (i * HashFunction2(key)) + (i*i * HashFunction3(key)) ) % GetTableSize();
}

// Given:  key  - Integer representing the key, which will be used to determine the hash value.
// 
// Task:   To calculate the hash value given the key and referring to the size of the table.
// 
// Return: The calculated hash value.
int HashTable::HashFunction1(const int key) {
    return key % GetTableSize();              // Page 296 Intro to Algorithms Textbook.
}

// Given:  key  - Integer representing the key, which will be used to determine the hash value.
// 
// Task:   To calculate the hash value given the key and referring to the size of the table.
// 
// Return: The calculated hash value.
int HashTable::HashFunction2(const int key) {
    return 1 + ( key % (GetTableSize() -1) ); // Page 296 Intro to Algorithms Textbook.
}

// Given:  key  - Integer representing the key, which will be used to determine the hash value.
// 
// Task:   To calculate the hash value given the key and referring to the size of the table.
// 
// Return: The calculated hash value.
int HashTable::HashFunction3(const int key) {
    return 1 + (key % (GetTableSize() - 3));  // Page 296 Intro to Algorithms Textbook.
}

// Given:  Nothing.
// 
// Task:   To print all buckets in the hash table and show their contents.
// 
// Return: Nothing.
void HashTable::PrintTable(void) {
    for (int i = 0; i < GetTableSize(); i++) {
        std::cout << "Bucket " << i << ":" << std::endl;
        std::cout << "Key: " << table[i].GetKey() << std::endl;
        std::cout << "ModKey: " << table[i].GetModKey() << std::endl;
        std::cout << "Attempts for Initial Insert: " << table[i].GetAttempts() << std::endl;

        if (table[i].GetOccupancy()) {
            std::cout << "Occupancy: Occupied" << std::endl;
        }
        else {
            std::cout << "Occupancy: Empty" << std::endl;
        }
        std::cout << "- - - - - - - -" << std::endl;
    }
}

// Given:  Nothing.
// 
// Task:   To print the retained sorted list in ascending order by using headBucket as the starting point.
// 
// Return: Nothing.
void HashTable::PrintList(void) {
    // Prints the list in sorted ascended order.
    for (nodePtr current = &table[GetHead()]; current != nullptr; current = current->GetNext()) {
        std::cout << "Key: " << current->GetKey() << ", ModKey: " << current->GetModKey() << std::endl;
    }
}

// Given:  headNode            - A Struct of Node which is the next Node to be added to the hash table.
//         prevBucket          - Integer representing the index of the Node inserted before headNode, here it is ignored, we just want to return that value for the HeadInsert function.
// 
// Task:   To insert the first Node into the hash table and to set headBucket.
// 
// Return: true or false                    - True indicating there is room to insert, False indicating there is no room.
//         prevBucket (via reference)       - The index of the bucket that was just inserted into the hash table to setup for the next insert.
bool HashTable::SetHead(Node headNode, int &prevBucket) {
    // Sets 
    int bucket;
    for (int i = 0; i < GetTableSize(); i++) {
        bucket = Probe(headNode.GetKey(), i);
        if (i == 0) {
            table[bucket].IncrementGeneralBuckets(); // NOT NEEDED, ONLY HERE TO MONITOR CHAINING AS WELL
        }
        if (table[bucket].GetOccupancy() == false) {
            table[bucket].SetInitialAttempts(i + 1);
            table[bucket].SetKeys(headNode.GetKey());
            table[bucket].SetOccupancy(true);
            headBucket = bucket;
            prevBucket = bucket;
            return true;
        }
    }
    return false; // return false indicating there is no room!
}

// Given:  Nothing.
// 
// Task:   To simply return occupiedBuckets.
// 
// Return: occupiedBuckets        - The number of buckets that are occupied in the table.
int HashTable::GetOccupiedBuckets(void) {
    return occupiedBuckets;
}

// Given:  Nothing.
// 
// Task:   To increment occupiedBuckets by 1.
// 
// Return: Nothing.
void HashTable::IncrementOccupiedBuckets(void) {
    occupiedBuckets++;
}

// Given:  headNode               - A Struct of Node which is the next Node to be added to the hash table.
//         prevBucket             - Integer representing the index of the Node inserted before headNode.
// 
// Task:   To insert Nodes into the hashtable whilst retaining the order of the list.
// 
// Return: true or false                    - True indicating there is room to insert, False indicating there is no room.
//         prevBucket (via reference)       - The index of the bucket that was just inserted into the hash table to setup for the next insert.
bool HashTable::HeadInsert(Node headNode, int& prevBucket) {
    
    IncrementOccupiedBuckets();
    int bucket;
    for (int i = 0; i < GetTableSize(); i++) {
        bucket = Probe(headNode.GetKey(), i);
        if (i == 0) {
            table[bucket].IncrementGeneralBuckets(); // NOT NEEDED, ONLY HERE TO MONITOR CHAINING AS WELL
        }
        if (table[bucket].GetOccupancy() == false) {
            table[bucket].SetInitialAttempts(i + 1);
            table[bucket].SetKeys(headNode.GetKey());
            table[bucket].SetOccupancy(true);
            table[prevBucket].SetNext(&table[bucket]);
            prevBucket = bucket;
            return true;
        }
    }
    return false; // return false indicating there is no room!
}

// Given:  searchKey          - An integer representing the key wished to be searched for.
//         result             - A Struct of Node which contains dummy information.
//         searchAttempts     - An integer representing the number of attempts to complete the search which currently contains dummy information.
// 
// Task:   To search through the hashtable for the bucket which contains the provided searchKey and return that bucket.
// 
// Return: true or false                    - True indicating the search value was found, False indicating the search value was not found.
//         result (via reference)           - A Struct of Node which is the goal bucket searched for.
//         searchAttempts (via reference)   - An integer representing the amount of times needed to probe 
bool HashTable::Search(const int searchKey, Node& result, int& searchAttempts) {

    int bucket;
    searchAttempts = 0;
    for (int i = 0; i < GetTableSize(); i++) {
        bucket = Probe(searchKey, i);
        searchAttempts++;
        if (table[bucket].GetOccupancy() == false) {
            return false; // return false indicating that search value is not present!
        }
        else {
            if (searchKey == table[bucket].GetKey()) {
                result = table[bucket];
                return true;
            }
        }
    }
    return false;
}

// Given:  Nothing
// 
// Task:   To determine which bucket would have the longest list (if chaining was used).
// 
// Return: listedBuckets      - An integer representing the number of items in the longest list if chaining was used.
int HashTable::PopularBucketChain(void) {
    int longestList = table[0].GetGeneralBuckets();
    for (int i = 0; i < GetTableSize(); i++) {
        if (table[i].GetGeneralBuckets() > longestList) {
            longestList = table[i].GetGeneralBuckets();
        }
    }
    return longestList;
}

// Given:  Nothing.
// 
// Task:   To calculate the total number of buckets who would have one or more items if chaining was used.
// 
// Return: onePlusBuckets      - An integer representing the number of buckets that would have one or more items if chaining was used.
int HashTable::CalculateOnePlusBuckets(void) {
    int onePlusBuckets = 0;
    for (int i = 0; i < GetTableSize(); i++) {
        if (table[i].GetGeneralBuckets() >= 1) {
            onePlusBuckets++;
        }
    }
    return onePlusBuckets;
}

// Given:  Nothing.
// 
// Task:   To return the dynamically allocated array of Nodes.
// 
// Return: table            - The dynamically allocated array of Nodes which acts as our Hash Table.
std::unique_ptr<Node[]>& HashTable::GetTable(void) {
    return table;
}
