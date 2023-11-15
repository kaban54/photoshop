#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <cstdint>
#include <assert.h>
#include <cstdlib>
#include <cstring>


template <typename T>
class MyVector {
    static const BASE_CAPACITY = 2;
    size_t size;
    size_t capacity;
    T* data;

    public:

    explicit MyVector();

    explicit MyVector (size_t size_, const T& val = T());

    ~MyVector();

    size_t GetSize() const {
        return size;
    }

    size_t GetCapacity() const {
        return capacity;
    }

    T* GetData() {
        return data;
    }

    bool Empty() const {
        return (size == 0);
    }

    T& operator[] (size_t index) {
        assert (index < size);
        return (data[index]);
    }

    T& Front() {
        assert (size > 0);
        return *data;
    }

    T& Back() {
        assert (size > 0);
        return data[size - 1];
    }

    void Reserve (size_t new_cap);

    void ShrinkToFit();

    void Resize (size_t new_size, const T& val = T());

    void PushBack (const T& val);

    void PopBack();

    void Insert (size_t index, const T& val);

    void Clear();

    private:

    void extend();
};

template <typename T>
MyVector<T>::MyVector ():
    size (0),
    capacity (0),
    data (nullptr)
    {}

template <typename T>
MyVector<T>::MyVector (size_t size_, const T& val):
    size (size_),
    capacity (size_)
    {
        data = (T*) calloc (size, sizeof(data[0]));
        assert (data != nullptr);

        for (size_t i = 0; i < size; i++) data[i] = val;
    }

template <typename T>
MyVector<T>::~MyVector() {
    if (capacity > 0) free (data);
}

template <typename T>
void MyVector<T>::Reserve (size_t new_cap) {
    if (new_cap <= capacity) return;
    data = (T*) realloc (data, new_cap * sizeof(data[0]));
    assert (data != nullptr);
    capacity = new_cap;
}

template <typename T>
void MyVector<T>::ShrinkToFit() {
    if (capacity == size) return;
    data = (T*) realloc (data, size * sizeof(data[0]));
    capacity = size;
}

template <typename T>
void MyVector<T>::Resize(size_t new_size, const T& val) {
    if (new_size > size) {
        Reserve (new_size);
        for (size_t i = size; i < new_size; i++) data[i] = val;
    }
    size = new_size;
}

template <typename T>
void MyVector<T>::PushBack(const T& val) {
    if (size == capacity) extend();
    data[size++] = val;
}

template <typename T>
void MyVector<T>::PopBack() {
    assert (size > 0);
    size--;
}

template <typename T>
void MyVector<T>::Insert (size_t index, const T& val) {
    assert (index <= size);
    if (size == capacity) extend();
    for (size_t i = size; i > index; i--) {
        data[i] = data[i - 1];
    }
    data[index] = val;
    size++;
}

template <typename T>
void MyVector<T>::Clear() {
    size = 0;
}

template <typename T>
void MyVector<T>::extend() {
    size_t new_cap = capacity == 0 ? BASE_CAPACITY : capacity * 2;
    data = (T*) realloc (data, new_cap * sizeof(data[0]));
    assert (data != nullptr);
    capacity = new_cap;
}

#endif