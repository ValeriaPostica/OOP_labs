#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <vector>
#include <list>
#include <deque>
#include <stdexcept>
#include <iostream>
using namespace std;

template <typename T> //This class can work with any type
class Queue { //Abstract base class(interface)
public:
    virtual ~Queue() = default;
    virtual void enqueue(const T& element) = 0;  // Add to back
    virtual T dequeue() = 0;                     // Remove from front  
    virtual bool isEmpty() const = 0;            // Check if empty
    virtual size_t size() const = 0;             // Get size
};

//Implementation 1(Deque-based)
template <typename T>
class DequeQueue : public Queue<T> {
private:
    deque<T> container;
public:
    void enqueue(const T& element) override {
        container.push_back(element);
    }

    T dequeue() override {
        if (container.empty()) {
            throw out_of_range("Queue is empty");
        }
        T front = container.front();  // Get first item
        container.pop_front();        // Remove first item
        return front;                 // Return removed item
    }

    bool isEmpty() const override {
        return container.empty();
    }

    size_t size() const override {
        return container.size();
    }
};

//Implementation 2(Vector-based)
template <typename T>
class VectorQueue : public Queue<T> {
private:
    vector<T> container;
public:
    void enqueue(const T& element) override {
        container.push_back(element);
    }

    T dequeue() override {
        if (container.empty()) {
            throw out_of_range("Queue is empty");
        }
        T front = container.front(); //Get first item
        container.erase(container.begin()); //begin() returns an iterator to the first element
        return front;
    }

    bool isEmpty() const override {
        return container.empty();
    }

    size_t size() const override {
        return container.size();
    }
};

//Implementation 3(List-based)
template <typename T>
class ListQueue : public Queue<T> {
private:
    list<T> container;

public:
    void enqueue(const T& element) override {
        container.push_back(element);
    }

    T dequeue() override {
        if (container.empty()) {
            throw out_of_range("Queue is empty");
        }
        T front = container.front();  // Get first item
        container.pop_front();        // Remove first item
        return front;
    }

    bool isEmpty() const override {
        return container.empty();
    }

    size_t size() const override {
        return container.size();
    }
};
#endif 