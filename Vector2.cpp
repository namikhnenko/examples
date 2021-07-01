#include <iostream>
#include <cstddef>
#include <memory>

template<typename T>
struct VectorBase {
    T* data;
    size_t cp;
    VectorBase() {
        data = nullptr;
        cp = 0;
    }
    VectorBase(size_t n) : cp(n) {
        data = static_cast<T*>(operator new(n * sizeof(T)));
    }
    ~VectorBase() {
        operator delete(data);
    }
};

template<typename T>
class Vector : private VectorBase<T> {
    size_t sz;
    using VectorBase<T>::data;
    using VectorBase<T>::cp;

    void rellocate(size_t cp_, size_t sz_, T* data_) {
        Vector temp(cp_);
        temp.sz = sz_;
        std::uninitialized_copy_n(data, sz, temp.data);
        swap(temp);
    }

public:
    Vector() : VectorBase<T>(), sz(0) {}
    Vector(size_t n) : VectorBase<T>(n), sz(0) {}
    Vector(Vector& other) : VectorBase<T>(other.sz), sz(other.size) {
        std::uninitialized_copy_n(other.data, other.sz, data);
    }
    T* begin() {
        return data;
    }
    T* end() {
        return data + sz;
    }
    ~Vector() {
        std::destroy_n(data, sz);
    }
    Vector & operator = (Vector & other) {
        reallocate(other.sz, other.sz, other.data);
        return *this;
    }
    T& operator[] (size_t i) {
        return data[i];
    }

    void swap(Vector& other) {
        std::swap(sz, other.sz);
        std::swap(data, other.data);
        std::swap(cp, other.cp);
    }

    const T& operator[] (size_t i) const {
        return data[i];
    }
    size_t capacity() const {
        return cp;
    }
    size_t size() const {
        return sz;
    }
    void reserve(size_t n) {
        if (n > cp) {
            rellocate(n, sz, data);
        }
    }
    void clear() {
        std::destroy_n(data, sz);
        sz = 0;
    }
    void resize(size_t new_sz) {
        if (new_sz <= sz) {
            std::destroy_n(data + new_sz, sz - new_sz);
        } else if (new_sz <= cp) {
            std::uninitialized_default_construct(data + sz, data + new_sz);
        } else {
            Vector temp(new_sz);
            temp.sz = new_sz;
            std::uninitialized_copy_n(data, sz, temp.data);
            std::uninitialized_default_construct(temp.data + sz, temp.data + new_sz);
            swap(temp);
        }
        sz = new_sz;
    }
    void pop_back() {
        --sz;
        std::destroy_at(data + sz);
    }

    void push_back(const T& value) {
        if (sz == cp) {
            size_t new_cp = 2 * cp;
            if (new_cp == 0)
                new_cp++;

            Vector temp(new_cp);
            temp.sz = sz;
            std::uninitialized_copy_n(data, sz, temp.data);
            new (temp.data + temp.sz) T(value);
            ++temp.sz;
            swap(temp);
        } else {
            new (data + sz++) T(value);
        }
    }

    void push_back(T && value) {
        if (sz == cp) {
            size_t new_cp = 2 * cp;
            if (new_cp == 0)
                new_cp++;

            Vector temp(new_cp);
            temp.sz = sz;
            std::uninitialized_copy_n(data, sz, temp.data);
            new (temp.data + temp.sz) T(std::move(value));
            ++temp.sz;
            swap(temp);
        } else {
            new (data + sz++) T(std::move(value));
        }
    }
};
