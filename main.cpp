// Thomas McLaughlin

/*
Summary: 
          
          In this program we read in values from a text file and eventually load its associated data into a hash table for fast searches. Before this,
          we sort the dynamically allocated array where the data is stored and put it in ascending order. After this, the data is inserted into the hash
          table. Notably, the order is retained by implementing a modified insert function which considers the previously made pointers in the Numbers
          array. After all the data is loaded into the hash table, we can do a number of things:

          (1) Print the entire Hash Table
          (2) Print the list of occupied buckets in order
          (3) Search a key
          (4) Statistics/Monitoring
          (5) Quit

          
         Afterwards, the program frees up all memory space that was dynamically allocated. The goal is to search fast and to sort fast. With the below
         implementation, we do both as fast as possible, although not at the same time; we sort, then we search. The running time for both sort and search
         in this implementation is O(n).

*/



#include "Hash.h"



// Given:  arraySize    - The size of the array to be allocated.
// 
// Task:   To dynamically allocate an array of Node structs of size arraySize using a smart pointer.
// 
// Return: A std::unique_ptr to an array of Node structs of size arraySize.
std::unique_ptr<Node[]> AllocateArrayNode(const int arraySize);


// Given:  arraySize    - The size of the array to be allocated.
// 
// Task:   To dynamically allocate an array of integers structs of size arraySize using a smart pointer.
// 
// Return: A std::unique_ptr to an array of integers of size arraySize.
std::unique_ptr<int[]> AllocateArrayInt(const int arraySize);


// Given:  Numbers      - An array of Node structs.
//         arraySize    - The number of elements in the Numbers array.
//         inFile       - A reference to the input file stream.
// 
// Task:   To read in the unsorted integer numbers from the input file and store them in the Numbers array.
// 
// Return: Numbers      - An array of Node structs populated with data read from file.
void ReadData(std::unique_ptr<Node[]>& Numbers, const int arraySize, std::ifstream& inFile);


// Given:  inFile       - A reference to the input file stream.
//         maxDigits    - Number of places in the form: xx,xxx yields 5.
// 
// Task:   To determine the number of integers/records in the input file by calculating the file size
//         and dividing it by the number of bytes per line (assuming 5-character integers and newline characters).
//         The function calculates the number of integers/records based on the size of the file and returns it.
// 
// Return: An integer representing the number of records/integers in the file.
int ReadSize(std::ifstream& inFile, const int maxDigits);

// Given:  Numbers      - The array wished to be sorted.
//         B            - A temporary array to store the sorted numbers based upon the current digit place.
//         digitPlace   - The place we wish to evaluate: unit place, tens place, hundreds place, etc.
// 
// Task:   To sort the Numbers array into ascending order based on the digit indicated by the parameter digitPlace.
// 
// Return: B            - Containing the data from numbers which is sorted into ascending order by the digitPlace.
void CountingSort(std::unique_ptr<Node[]>& Numbers, std::unique_ptr<int[]>& B, const int digitPlace, const int arraySize, const int range);


// Given:  Numbers      - An array of unsorted integers.
//         maxPlace     - Integer which is the smallest power of 10 that has the number of digits specified by MAX_DIGITS.
// 
// Task:   To sort the integers in the Numbers array into ascending order using Radix Sort (with CountingSort as its stable sort).
// 
// Return: Numbers      - An array of integers, now in sorted, ascending order.
void RadixSort(std::unique_ptr<Node[]>& Numbers, const int maxPlace, const int arraySize, const int range);


// Given:  valueInQuestion      - An integer representing the value we want to check for occurrences for throughout the table.
//         table                - A hash table containing all Node data.
// 
// Task:   To tally up occurrences where valueQuestion equals the number of times this bucket was hashed to on the first attempt.
// 
// Return: result               - The total number of times where valueQuestion equaled the number of times this bucket was hashed to on the first attempt.
int CalculateCapacityChain(const int valueInQuestion, HashTable& table);


int main(void) {
    
    Node result;
    int searchKey;
    int searchAttempts;
    bool menu = true;
    int userChoice;
    int popularBucket;
    int openAddressResult;

    std::ifstream inFile;
    inFile.open("keys.txt");  // Open File
    if (inFile.fail()) {
        std::cout << "File Failed To Open" << std::endl;
        exit(1);
    }

    int arraySize = ReadSize(inFile, MAX_DIGITS);

    std::unique_ptr<Node[]> Numbers = AllocateArrayNode(arraySize);

    ReadData(Numbers, arraySize, inFile);

    inFile.close(); // Close File

    RadixSort(Numbers, MAX_PLACE, arraySize, RANGE); // Sort the Numbers array in ascending order.

    HashTable hashTable(arraySize);
    int prevBucket = 0;
    hashTable.SetHead(Numbers[0], prevBucket);

    for (int i = 1; i < arraySize; i++) {
        hashTable.HeadInsert(Numbers[i], prevBucket);
    }

    Numbers.reset(); // Deallocate the array and set it equal to Nullptr as we do not need it anymore.

    while (menu) {
        std::cout << "Select an operation:" << std::endl;
        std::cout << "\t(1) Print the entire Hash Table" << std::endl;
        std::cout << "\t(2) Print the list of occupied buckets in order" << std::endl;
        std::cout << "\t(3) Search a key" << std::endl;
        std::cout << "\t(4) Statistics" << std::endl;
        std::cout << "\t(5) Quit" << std::endl;
        std::cin >> userChoice;

        switch (userChoice) {
        case 1:
            std::cout << std::endl;
            std::cout << "- - - - - - - - - - - - - - - - - - - - - - -" << std::endl;
            hashTable.PrintTable();
            std::cout << std::endl << "- - - - - - - - - - - - - - - - - - - - - - -" << std::endl;
            std::cout << std::endl;
            break;
        case 2:
            std::cout << std::endl;
            std::cout << "- - - - - - - - - - - - - - - - - - - - - - -" << std::endl;
            hashTable.PrintList();
            std::cout << "- - - - - - - - - - - - - - - - - - - - - - -" << std::endl;
            std::cout << std::endl;
            break;
        case 3:
            std::cout << std::endl;
            std::cout << "- - - - - - - - - - - - - - - - - - - - - - -" << std::endl;
            std::cout << "Enter a key you would like to search for:" << std::endl;
            std::cin >> searchKey;
            if (hashTable.Search(searchKey, result, searchAttempts)) {
                std::cout << std::endl << "Search Successful For " << searchKey << ":" << std::endl;
                std::cout << "key: " << result.GetKey() << std::endl;
                std::cout << "modKey: " << result.GetModKey() << std::endl;
                std::cout << "Times probed for initial insert: " << result.GetAttempts() << std::endl << std::endl;
                std::cout << "Times probed for search: " << searchAttempts << std::endl << std::endl;
            }
            else {
                std::cout << "Search Unsuccessful For " << searchKey << ":" << std::endl;
                std::cout << "# of attempts search: " << searchAttempts << std::endl << std::endl;
            }
            std::cout << "- - - - - - - - - - - - - - - - - - - - - - -" << std::endl;
            std::cout << std::endl;
            break;
        case 4:
            std::cout << std::endl;
            std::cout << "- - - - - - - - - - - - - - - - - - - - - - -" << std::endl;
            std::cout << "Number of occupied buckets: " << hashTable.GetOccupiedBuckets() << std::endl;
            std::cout << "Number of unoccupied buckets: " << hashTable.GetTableSize() << std::endl;
            std::cout << "\t" << hashTable.GetOccupiedBuckets() << " out of the " << hashTable.GetTableSize() << " buckets contains relevant data." << std::endl;
            // chain
            std::cout << std::endl << "Monitoring of Chaining (I did not go with this implementation): " << std::endl;
            popularBucket = hashTable.PopularBucketChain();
            std::cout << "\tLongest List of items if chaining was used: " << popularBucket << std::endl;
            std::cout << std::endl << "\tNumber of buckets with 1 or more items if chaining was used: " << hashTable.CalculateOnePlusBuckets() << std::endl<< std::endl;

            for (int i = 0; i <= popularBucket; i++) {
                std::cout << "\tNumber of buckets with " << i << " items: " << CalculateCapacityChain(i, hashTable) << std::endl;
            }
            //chain
            //open ad
            std::cout << std::endl << "Monitoring of Open Addressing (I went with this implementation): " << std::endl;
            popularBucket = 1;
            std::cout << "\tLongest List of items with Open Addressing: "<< popularBucket<< std::endl;
            std::cout << std::endl << "\tNumber of buckets with 1 or more items with Open Addressing: " << hashTable.GetOccupiedBuckets() << std::endl << std::endl;

            std::cout << "\tNumber of buckets with 0 items: " << hashTable.GetTableSize() - hashTable.GetOccupiedBuckets() << std::endl;
            std::cout << "\tNumber of buckets with 1 items: " << hashTable.GetOccupiedBuckets() << std::endl;

            std::cout << "- - - - - - - - - - - - - - - - - - - - - - -" << std::endl;
            std::cout << std::endl;
            break;
        case 5:
            menu = false;
            break;
        default:
            std::cout << std::endl;
            std::cout << "- - - - - - - - - - - - - - - - - - - - - - -" << std::endl;
            std::cout << "Not an option..." << std::endl;
            std::cout << "- - - - - - - - - - - - - - - - - - - - - - -" << std::endl;
            std::cout << std::endl;
            break;
        }
    }
    
    return 0;
}


void ReadData(std::unique_ptr<Node[]>& Numbers, const int arraySize, std::ifstream& inFile) {
    int fileValue;
    for (int i = 0; i < arraySize; i++) {
        inFile >> fileValue;
        Numbers[i].SetKeys(fileValue);
    }
}

int ReadSize(std::ifstream& inFile, const int maxDigits) {

    inFile.seekg(0l, std::ios::end);        // Seek to end.
    int itemSize = sizeof(char) * (maxDigits + 2);        // This is for 5 characters, followed by carriage return, line feed.
    // Those 2 characters mark the end of a line of text in Windows.
    int fileSize = ((int)inFile.tellg()) / itemSize;  // Divide by 7 to get number of characters in the file.
    inFile.seekg(0l, std::ios::beg); // Seek back to beginning.

    return (fileSize + 1); // returning (fileSize + 1) as fileSize is one less than the number of records.
}

std::unique_ptr<Node[]> AllocateArrayNode(const int arraySize) {
    // Smart pointers make our lives easier. Smart pointers automate the managing of our pointers, and in general are safer than raw pointers.
    // By using smart pointers, I do not have to worry about deallocating the memory as it is automatically deallocated and cleaned up when it goes out of scope.


    try {
        return std::make_unique<Node[]>(arraySize); // Allocate memory for Numbers dynamically (on the heap) | Auto deletes when out of scope.
    }
    catch (const std::bad_alloc& e) {
        // Handle failures.
        std::cerr << "Memory allocation failed: " << e.what() << std::endl;
        return nullptr;
    }
}

std::unique_ptr<int[]> AllocateArrayInt(const int arraySize) {
    // Smart pointers make our lives easier. Smart pointers automate the managing of our pointers, and in general are safer than raw pointers.
    // By using smart pointers, I do not have to worry about deallocating the memory as it is automatically deallocated and cleaned up when it goes out of scope.


    try {
        return std::make_unique<int[]>(arraySize); // Allocate memory for Numbers dynamically (on the heap) | Auto deletes when out of scope.
    }
    catch (const std::bad_alloc& e) {
        // Handle failures.
        std::cerr << "Memory allocation failed: " << e.what() << std::endl;
        return nullptr;
    }
}

void CountingSort(std::unique_ptr<Node[]>& Numbers, std::unique_ptr<int[]>& B, const int digitPlace, const int arraySize, const int range) {

    // Counting Sort has a worst case time of theta ( k + n ).
    // Counting Sort has an average case time of theta ( k + n ).
    // Where k is the # of values.

    //      COUNTING-SORT(A, n, k)
    // 1         let B[1:n] and C[0:k] be new arrays   | I instead initialized all of C's elements to 0 w/o a loop, and passed in array B.
    // 2         for i = 0 to k
    // 3             C[i] = 0
    // 4         for j = 1 to n
    // 5             C[A[j]] = C[A[j]] + 1
    // 6         // C[i] now contains the number of elements equal to i.
    // 7         for i = 1 to k
    // 8             C[i] = C[i] + C[i - 1]
    // 9         // C[i] now contains the number of elements less than or equal to i.
    // 10        // Copy A to B, starting from the end of A.
    // 11        for j = n down to 1
    // 12            B[C[A[j]]] = A[j]
    // 13            C[A[j]] = C[A[j]] - 1
    // 14        return B                              | B[] was passed in by reference as a parameter, we 'return' B this way.


    std::unique_ptr<int[]> C = AllocateArrayInt(range); // Declaring a smart pointer, unique_ptr, to manage the dynamic array.

    // range acts as K: if max number goes up to 7, the range is 8 (0-7).
    for (int i = 0; i < range; i++) {
        C[i] = 0;
    }

    int digit;

    // Count the occurrences.
    for (int j = 0; j < arraySize; j++) {
        digit = (Numbers[j].GetKey() / digitPlace) % range; // Get the digit at the current place value.
        C[digit] = C[digit] + 1;
        // C[i] now contains the number of elements equal to i.
    }

    // Sum up elements across array.
    for (int i = 1; i < range; i++) {
        C[i] = C[i] + C[i - 1];
    }

    // Copy numbers to array B, starting from the end of the Numbers array.
    for (int j = arraySize - 1; j >= 0; j--) {
        digit = (Numbers[j].GetKey() / digitPlace) % range; // Get the digit at the current place value.
        B[C[digit] - 1] = Numbers[j].GetKey(); // Put the element in its proper location in B based on the digit.
        C[digit] = C[digit] - 1; // Decrement the count so we can handle duplicates.
    }

}

void RadixSort(std::unique_ptr<Node[]>& Numbers, const int maxPlace, const int arraySize, const int range) {

    // Radix Sort has a worst case time of theta ( d (n + k) ).
    // Radix Sort has an average case time of theta ( d (n + k) ).
    // Where k is the # of values, d is the # of digits, and n is the length of the array.

    //      RADIX-SORT(A, n, d)
    // 1         for i = 1 to d
    // 2             use a stable sort to sort array A[1:n] on digit i

    std::unique_ptr<int[]> B = AllocateArrayInt(arraySize); // Declaring a smart pointer, unique_ptr, to manage the dynamic array.


    // Iterate over each digit place
    for (int digitPlace = 1; digitPlace <= maxPlace; digitPlace *= 10) {

        CountingSort(Numbers, B, digitPlace, arraySize, range); // CountingSort will sort based on the provided digit place.

        // Now copying B to Numbers array. Note that this wastes some time, O(n) time, where n is the length of the array.
        for (int k = 0; k < arraySize; k++) {
            Numbers[k].SetKeys(B[k]);
        }
    }
}

int CalculateCapacityChain(const int valueInQuestion, HashTable& table) {

    int result = 0;
    std::unique_ptr<Node[]>& tempTable = table.GetTable();
    for (int i = 0; i < table.GetTableSize(); i++) {
        if (tempTable[i].GetGeneralBuckets() == valueInQuestion) {
            result++; // Tallying up occurrences where valueQuestion equals the number of times this bucket was hashed to on the first attempt.
        }
    }
    return result;
}
