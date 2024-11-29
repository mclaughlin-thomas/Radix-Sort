#pragma once

#include "globals.h"
#include "node.h"

class HashTable {
public:
	HashTable();
	HashTable(const int size);
	~HashTable(void);
	std::unique_ptr<Node[]> AllocateArrayNode(const int arraySize);
	int GetHead(void);
	int GetTableSize(void);
	std::unique_ptr<Node[]>& GetTable(void);
	int GetOccupiedBuckets(void);
	void IncrementOccupiedBuckets(void);
	bool SetHead(Node headNode, int& prevBucket);
	int Probe(const int key, const int i);
	int HashFunction1(const int key);
	int HashFunction2(const int key);
	int HashFunction3(const int key);
	bool HeadInsert(Node headNode, int& prevBucket);
	bool Search(const int searchKey,Node& result, int& searchAttempts);
	void PrintTable(void);
	void PrintList(void);
	int PopularBucketChain(void);
	int CalculateOnePlusBuckets(void);
private:
	int tableSize; // The size of the table: will be three times the number of records.
	int headBucket; // The first occupied bucket in ascending order.
	int occupiedBuckets; // The number of buckets that are occupied in the table.
	std::unique_ptr<Node[]> table; // Dynamically allocated array of Nodes.
};
