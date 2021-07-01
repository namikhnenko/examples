#include <iostream>
#include <cstddef>
#include <memory>
#include <tuple>

template<typename T, typename Deleter = std::default_delete<T>>
class UniquePtr {
private:
    std::tuple<T*, Deleter> ptr;

public:
    UniquePtr() noexcept {}
    UniquePtr(T * other) noexcept : ptr{other, Deleter()} {}
    UniquePtr(T * other, Deleter d) noexcept : ptr{other, d} {}
    UniquePtr(const UniquePtr& other) noexcept = delete;
    UniquePtr(UniquePtr&& other) noexcept {
        std::get<0>(ptr) = std::get<0>(other.ptr);
        std::get<0>(other.ptr) = nullptr;
        std::get<1>(ptr) = Deleter();
    }
    UniquePtr<T>& operator = (const UniquePtr& other) noexcept = delete;
    UniquePtr<T>& operator = (std::nullptr_t t) noexcept {
        std::get<1>(ptr)(std::get<0>(ptr));
        std::get<0>(ptr) = t;
        return *this;
    }
    UniquePtr& operator = (UniquePtr&& other) noexcept {
        std::get<1>(ptr)(std::get<0>(ptr));
        std::get<0>(ptr) = std::get<0>(other.ptr);
        std::get<0>(other.ptr) = nullptr;
        return *this;
    }
    ~UniquePtr() noexcept {
        std::get<1>(ptr)(std::get<0>(ptr));
    }
    const T& operator *() const {
        return *(std::get<0>(ptr));
    }
    const T* operator ->() const noexcept {
        return std::get<0>(ptr);
    }
    T * release() noexcept {
        T * t = std::get<0>(ptr);
        std::get<0>(ptr) = nullptr;
        return t;
    }
    void reset(T * other) noexcept {
        std::get<1>(ptr)(std::get<0>(ptr));
        std::get<0>(ptr) = other;
    }
    void swap(UniquePtr& other) noexcept {
        std::swap(this->ptr, other.ptr);
    }
    T * get() const noexcept {
        return std::get<0>(ptr);
    }
    explicit operator bool() const noexcept {
        return (get() != nullptr);
    }
    const Deleter& get_deleter() const noexcept {
        return std::get<1>(ptr);
    }
    Deleter& get_deleter() noexcept {
        return std::get<1>(ptr);
    }
};
