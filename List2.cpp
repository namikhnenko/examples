#include <iostream>
#include <algorithm>

template<typename T>
class Node {
public:
    Node* prev;
    Node* next;
    T* value;
    Node() : prev(nullptr), next(nullptr), value(nullptr) {}
    Node(const T& val) : prev(nullptr), next(nullptr), value(new T(val)) {}
    ~Node() {
        delete value;
    }
};

template<typename T>
class ListIt {
private:
    Node<T>* ptr;

public:
    ListIt(Node<T>* n) : ptr(n) {}
    bool operator==(const ListIt& other) const {
        return ((ptr->prev == other.ptr->prev) && (ptr->next == other.ptr->next));
    }
    bool operator!=(const ListIt& other) const {
        return !((ptr->prev == other.ptr->prev) && (ptr->next == other.ptr->next));
    }
    T& operator*() {
        return *(ptr->value);
    }
    ListIt& operator++() {
        ptr = ptr->next;
        return *this;
    }
    ListIt operator++(int) {
        ListIt t = *this;
        ++(*this);
        return t;
    }
    ListIt& operator--() {
        ptr = ptr->prev;
        return *this;
    }
    ListIt operator--(int) {
        ListIt t = *this;
        --(*this);
        return t;
    }
};

template<typename T>
class ConstListIt {
private:
    Node<T>* ptr;

public:
    ConstListIt(Node<T>* n) : ptr(n) {}
    bool operator==(const ConstListIt& other) const {
        return ((ptr->prev == other.ptr->prev) && (ptr->next == other.ptr->next));
    }
    bool operator!=(const ConstListIt& other) const {
        return !((ptr->prev == other.ptr->prev) && (ptr->next == other.ptr->next));
    }
    const T& operator*() const {
        return *(ptr->value);
    }
    ConstListIt& operator++() {
        ptr = ptr->next;
        return *this;
    }
    ConstListIt operator++(int) {
        ConstListIt t = *this;
        ++(*this);
        return t;
    }
    ConstListIt& operator--() {
        ptr = ptr->prev;
        return *this;
    }
    ConstListIt operator--(int) {
        ConstListIt t = *this;
        --(*this);
        return t;
    }
};

template<typename T>
class List {
private:
    size_t sz;
    Node<T>* head;
    Node<T>* tail;

public:
    List() : sz(0), head(new Node<T>), tail(new Node<T>) {
        head->next = tail;
        tail->prev = head;
    }

    ListIt<T> begin() {
        return ListIt<T>(head->next);
    }
    ListIt<T> end() {
        return ListIt<T>(tail);
    }

    ConstListIt<T> begin() const {
        return ConstListIt<T>(head->next);
    }
    ConstListIt<T> end() const {
        return ConstListIt<T>(tail);
    }

    List(const List& other) : sz(0), head(new Node<T>), tail(new Node<T>) {
        head->next = tail;
        tail->prev = head;
        for (auto it = other.begin(); it != other.end(); ++it) {
            try {
                this->push_back(*it);
            } catch (...) {
                Node<T>* tmp = head;
                while (tmp != tail) {
                    tmp = tmp->next;
                    delete tmp->prev;
                }
                delete tmp;
                throw;
            }
        }
    }

    List& operator=(const List& other) {
        Node<T>* tmp = head;
        while (tmp != tail) {
            tmp = tmp->next;
            delete tmp->prev;
        }
        delete tmp;
        head = new Node<T>;
        tail = new Node<T>;
        head->next = tail;
        tail->prev = head;
        for (auto it = other.begin(); it != other.end(); ++it) {
            try {
                this->push_back(*it);
            }
            catch (...) {
                Node<T>* tmp = head;
                while (tmp != tail) {
                    tmp = tmp->next;
                    delete tmp->prev;
                }
                delete tmp;
                throw;
            }
        }
        return *this;
    }

    size_t size() const {
        return sz;
    }

    void push_back(const T& n) {
        try {
            Node<T>* t = new Node(n);
            (tail->prev)->next = t;
            t->prev = (tail->prev);
            t->next = tail;
            tail->prev = t;
            ++sz;
        } catch (...) {
            throw;
        }
    }

    void push_front(const T& n) {
        try {
            Node<T>* t = new Node(n);
            (head->next)->prev = t;
            t->next = head->next;
            t->prev = head;
            head->next = t;
            ++sz;
        } catch (...) {
            throw;
        }
    }
    void pop_back() {
        Node<T>* t = tail->prev;
        (t->prev)->next = tail;
        tail->prev = t->prev;
        delete t;
        --sz;
    }

    void pop_front() {
        Node<T>* t = head->next;
        (t->next)->prev = head;
        head->next = t->next;
        delete t;
        --sz;
    }

    ~List() {
        Node<T>* tmp = head;
        while (tmp != tail) {
            tmp = tmp->next;
            delete tmp->prev;
        }
        delete tmp;
    }
};
