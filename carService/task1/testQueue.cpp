#include <iostream>
#include <cassert>
#include <string>
#include <stdexcept>
#include "Queue.hpp"
using namespace std;

bool allPassed = true;

void testQueue(Queue<int>* q, const string& name) {
    cout << "Testing " << name << endl;

    bool testPassed;

    // Test 1: Empty Queue Check
    testPassed = true;
    if (!(q->isEmpty() == true)) testPassed = false;
    if (!(q->size() == 0)) testPassed = false;

    cout << "Test 1: " << (testPassed ? "PASSED" : "FAILED") << endl;
    if (!testPassed) allPassed = false;

    // Test 2: Enqueue Operations
    testPassed = true;

    q->enqueue(10);
    q->enqueue(20);
    q->enqueue(30);

    if (!(q->isEmpty() == false)) testPassed = false;
    if (!(q->size() == 3)) testPassed = false;

    cout << "Test 2: " << (testPassed ? "PASSED" : "FAILED") << endl;
    if (!testPassed) allPassed = false;

    // Test 3: Dequeue (FIFO)
    testPassed = true;

    if (!(q->dequeue() == 10)) testPassed = false;
    if (!(q->size() == 2)) testPassed = false;

    if (!(q->dequeue() == 20)) testPassed = false;
    if (!(q->size() == 1)) testPassed = false;

    if (!(q->dequeue() == 30)) testPassed = false;
    if (!(q->size() == 0)) testPassed = false;
    if (!(q->isEmpty() == true)) testPassed = false;

    cout << "Test 3: " << (testPassed ? "PASSED" : "FAILED") << endl;
    if (!testPassed) allPassed = false;

    // Test 4: Exception Handling
    testPassed = true;
    try {
        q->dequeue();
        testPassed = false;
    } catch (const out_of_range&) {}

    cout << "Test 4: " << (testPassed ? "PASSED" : "FAILED") << endl;
    if (!testPassed) allPassed = false;

    cout << endl;
}

int main() {
    cout << "Queue implementation tests:" << endl;

    DequeQueue<int> dq;
    testQueue(&dq, "DequeQueue");

    VectorQueue<int> vq;
    testQueue(&vq, "VectorQueue");

    ListQueue<int> lq;
    testQueue(&lq, "ListQueue");

    if(allPassed){
        cout << "SUCCESS: All 3 queue implementations passed!" << endl;
    } else {
        cout << "FAILURE: Some tests failed!" << endl;
        return 1;
    }
    return 0;
}
