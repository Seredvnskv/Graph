#include <iostream>

using namespace std;

template <typename T>
struct Node {
    T data;
    Node* next;

    Node(T data) : data(data), next(nullptr) {}
};

template <typename T>
class Stack {
    Node<T>* top;
public:
    Stack() : top(nullptr) {}

    bool isEmpty() const {
        return top == nullptr;
    }

    void push(T data) {
        Node<T>* node = new Node<T>(data);
        node->next = top;
        top = node;
    }

    T pop() {
        Node<T>* temp = top;
        T data = top->data;
        top = top->next;
        delete temp;
        return data;
    }

    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }
};

template <typename T>
class Queue {
    Node<T>* front;
    Node<T>* back;
public:
    Queue() : front(nullptr), back(nullptr) {}

    bool isEmpty() const {
        return front == nullptr;
    }

    void enqueue(T data) {
        Node<T>* node = new Node<T>(data);
        if (isEmpty()) {
            front = back = node;
        }
        else {
            back->next = node;
            back = node;
        }
    }

    T dequeue() {
        Node<T>* temp = front;
        T data = front->data;
        front = front->next;
        if (front == nullptr) {
            back = nullptr;

        }
        delete temp;
        return data;
    }

    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }
};