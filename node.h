#pragma once

struct Node {
    Node();
    ~Node();
    void SetKeys(const int nKey);
    void SetNext(Node* nextNode);
    void SetOccupancy(const bool nOccupancy);
    void SetInitialAttempts(const int nAttempt);
    int GetGeneralBuckets(void);
    int GetKey(void);
    int GetModKey(void);
    Node* GetNext(void);
    bool GetOccupancy(void);
    int GetAttempts(void);
    void IncrementGeneralBuckets(void);
private:
    int key;
    int modKey;
    int attemptsInitialInsert; // The number of attempts for the initial insert.
    int generalAttempts; // The number of times this bucket was hashed to on the first attempt.
    Node* next; // Points to next Node, otherwise points to nullptr.
    bool occupancy; // Occupancy of the bucket, false indicates it is free, true indicates it is taken.
};

typedef Node* nodePtr;
