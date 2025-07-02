#include <iostream>
#include <cstring>
#include <stdexcept>

class GraphicObject {
public:
    virtual ~GraphicObject() {}
    virtual void draw() const = 0;
};

class Circle : public GraphicObject {
public:
    void draw() const override {
        std::cout << "Drawing Circle" << std::endl;
    }
};

template <typename T>
class CommonQueue {
protected:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };
    Node* front;
    Node* rear;
    size_t count;

    void clear() {
        while (front) {
            Node* temp = front;
            front = front->next;
            delete temp;
        }
        front = rear = nullptr;
        count = 0;
    }

public:
    CommonQueue() : front(nullptr), rear(nullptr), count(0) {}
    virtual ~CommonQueue() {
        clear();
    }

    void enqueue(const T& value) {
        Node* newNode = new Node(value);
        if (!rear) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        count++;
    }

    bool dequeue(T& value) {
        if (!front) 
            return false;
        Node* temp = front;
        value = front->data;
        front = front->next;
        if (!front) 
            rear = nullptr;
        delete temp;
        count--;
        return true;
    }

    size_t size() const { 
        return count; 
    }
};

template <typename T>
class Queue : public CommonQueue<T> {
public:
    Queue<T>& operator<<(const T& value) {
        this->enqueue(value);
        return *this;
    }
};

template <>
class Queue<int> : public CommonQueue<int> {
public:
    int sum() const {
        int total = 0;
        typename CommonQueue<int>::Node* current = this->front;
        while (current) {
            total += current->data;
            current = current->next;
        }
        return total;
    }

    Queue<int>& operator<<(const int& value) {
        this->enqueue(value);
        return *this;
    }
};

template <>
class Queue<char> {
private:
    char* data;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;

public:
    explicit Queue(size_t size = 100) 
        : capacity(size), head(0), tail(0), count(0) {
        data = new char[capacity];
    }

    ~Queue() {
        delete[] data;
    }

    void enqueue(char value) {
        if (count >= capacity) 
            throw std::runtime_error("Queue is full");
        data[tail] = value;
        tail = (tail + 1) % capacity;
        count++;
    }

    bool dequeue(char& value) {
        if (count == 0) 
            return false;
        value = data[head];
        head = (head + 1) % capacity;
        count--;
        return true;
    }

    Queue<char>& operator<<(char value) {
        enqueue(value);
        return *this;
    }

    size_t size() const { 
        return count; 
    }
};

template <>
class Queue<const char*> {
private:
    struct Node {
        char* data;
        Node* next;
        Node(const char* value) : next(nullptr) {
            data = new char[std::strlen(value) + 1];
            std::strcpy(data, value);
        }
        ~Node() { 
            delete[] data; 
        }
    };
    Node* front;
    Node* rear;
    size_t count;

public:
    Queue() : front(nullptr), rear(nullptr), count(0) {}
    ~Queue() {
        while (front) {
            Node* temp = front;
            front = front->next;
            delete temp;
        }
    }

    void enqueue(const char* value) {
        Node* newNode = new Node(value);
        if (!rear) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        count++;
    }

    bool dequeue(char*& output) {
        if (!front) 
            return false;
        output = new char[std::strlen(front->data) + 1];
        std::strcpy(output, front->data);

        Node* temp = front;
        front = front->next;
        if (!front) 
            rear = nullptr;
        delete temp;
        count--;
        return true;
    }

    Queue<const char*>& operator<<(const char* value) {
        enqueue(value);
        return *this;
    }

    size_t size() const { 
        return count; 
    }
};

int main() {
    // Демонстрация для int (с исправлением)
    Queue<int> intQueue;
    intQueue << 10 << 20 << 30;
    
    // Сумма вычисляется ДО извлечения элементов
    std::cout << "Sum of ints: " << intQueue.sum() << std::endl;
    
    int intValue;
    while (intQueue.dequeue(intValue)) {
        std::cout << "Dequeued int: " << intValue << std::endl;
    }
    
    // Добавляем новые элементы
    intQueue << 40 << 50;
    std::cout << "Sum of new ints: " << intQueue.sum() << std::endl;
    
    // Демонстрация для char
    Queue<char> charQueue(5);
    charQueue << 'A' << 'B' << 'C';
    char charValue;
    while (charQueue.dequeue(charValue)) {
        std::cout << "Dequeued char: " << charValue << std::endl;
    }

    // Демонстрация для const char*
    Queue<const char*> strQueue;
    strQueue << "Hello" << "World";
    char* strValue;
    while (strQueue.dequeue(strValue)) {
        std::cout << "Dequeued string: " << strValue << std::endl;
        delete[] strValue;
    }

    // Демонстрация для указателей на графические объекты
    Queue<GraphicObject*> objQueue;
    Circle circle1, circle2;
    objQueue << &circle1 << &circle2;
    GraphicObject* objValue;
    while (objQueue.dequeue(objValue)) {
        objValue->draw();
    }

    return 0;
}