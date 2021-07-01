#include <iostream>
#include <algorithm>

template<typename T>
class SharedPtr {
private:
    T* ptr;
    size_t* counter;

public:
    SharedPtr() noexcept : ptr(nullptr), counter(nullptr) {}
    SharedPtr(T* other) : ptr(other), counter(nullptr) {
        if (other != nullptr) {
            counter = new size_t(1);
        }
    }
    SharedPtr(const SharedPtr& other) noexcept : ptr(other.ptr), counter(other.counter) {
        if (ptr != nullptr)
            ++(*counter);
    }
    SharedPtr(SharedPtr&& other) noexcept : ptr(nullptr), counter(nullptr) {
        other.swap(*this);
    }

    void swap(SharedPtr& other) noexcept {
        std::swap(ptr, other.ptr);
        std::swap(counter, other.counter);
    }

    SharedPtr& operator=(const SharedPtr& other) noexcept {
        SharedPtr(other).swap(*this);
        return *this;
    }
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        SharedPtr(std::move(other)).swap(*this);
        return *this;
    }
    SharedPtr& operator=(T* other) noexcept {
        SharedPtr(other).swap(*this);
        return *this;
    }
    ~SharedPtr() noexcept {
        if (counter != nullptr) {
            if (*counter > 1) {
                --(*counter);
            } else {
                delete counter;
                delete ptr;
            }
        }
        counter = nullptr;
        ptr = nullptr;
    }
    const T& operator*() const {
        return *ptr;
    }
    T& operator*() {
        return *ptr;
    }
    T* operator->() const noexcept {
        return ptr;
    }
    void reset(T* other) {
        SharedPtr(other).swap(*this);
    }
    T* get() const noexcept {
        return ptr;
    }
    explicit operator bool() const noexcept {
        return (ptr != nullptr);
    }
};
