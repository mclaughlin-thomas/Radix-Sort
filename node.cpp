#include "node.h"

// Given:  Nothing.
// 
// Task:   To initialize the Node variables.
// 
// Return: Nothing.
Node::Node(void) {
    key = 0;
    modKey = 0;
    attemptsInitialInsert = 0;
    next = nullptr;
    occupancy = false;
    generalAttempts = 0;
}

// Given:  Nothing.
// 
// Task:   To destroy the Node and release any allocated memory.
// 
// Return: Nothing.
Node::~Node(void) {
    
}

// Given:  nKey     - Integer representing the new key value.
// 
// Task:   To set key equal to nKey and modKey equal to ten times nKey.
// 
// Return: Nothing.
void Node::SetKeys(const int nKey) {
    key = nKey;
    modKey = key * 10;
}

// Given:  nextNode     - A pointer to the nextNode.
// 
// Task:   To set next equal to nextNode.
// 
// Return: Nothing.
void Node::SetNext(Node* nextNode) {
    next = nextNode;
}

// Given:  Nothing.
// 
// Task:   To simply return key.
// 
// Return: key       - The key value of the node which is used as the identifier.
int Node::GetKey(void) {
    return key;
}

// Given:  Nothing.
// 
// Task:   To simply return modKey.
// 
// Return: modKey     - An altered version of key which is the value ten times that of key.
int Node::GetModKey(void) {
    return modKey;
}

// Given:  Nothing.
// 
// Task:   To simply return next.
// 
// Return: next     - Node pointer which points to the next Node or to nullptr.
Node* Node::GetNext(void) {
    return next;
}

// Given:  nOccupancy     - Boolean representing the updated occupancy of the given Node.
// 
// Task:   To set occupancy equal to nOccupancy.
// 
// Return: Nothing.
void Node::SetOccupancy(const bool nOccupancy) {
    occupancy = nOccupancy;
}

// Given:  Nothing.
// 
// Task:   To simply return occupancy.
// 
// Return: occupancy     - Occupancy of the bucket, false indicates it is free, true indicates it is taken.
bool Node::GetOccupancy(void) {
    return occupancy;
}

// Given:  nAttempt     - Integer representing the updated number of attempts for the initial insert.
// 
// Task:   To set attemptsInitialInsert equal to nAttempt.
// 
// Return: Nothing.
void Node::SetInitialAttempts(const int nAttempt) {
    attemptsInitialInsert = nAttempt;
}

// Given:  Nothing.
// 
// Task:   To simply return attemptsInitialInsert.
// 
// Return: attemptsInitialInsert     - Integer which represents the number of attempts for the initial insert.
int Node::GetAttempts(void) {
    return attemptsInitialInsert;
}

// Given:  Nothing.
// 
// Task:   To simply return generalAttempts.
// 
// Return: generalAttempts          - Integer which represents the number of times this bucket was hashed to on the first attempt.
int Node::GetGeneralBuckets(void) {
    return generalAttempts;
}

// Given:  Nothing.
// 
// Task:   To increment generalAttempts by one.
// 
// Return: Nothing.
void Node::IncrementGeneralBuckets(void) {
    generalAttempts++;
}
