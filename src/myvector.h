#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <cstdint>
#include <assert.h>
#include <cstdlib>
#include <cstring>
#include <vector>


template <typename T>
class MyVector {
    static const size_t BASE_CAPACITY = 2;
    size_t size;
    size_t capacity;
    T* data;

    public:

    explicit MyVector();

    explicit MyVector(size_t size_, const T& val = T());

    explicit MyVector(const std::vector<T>& vec);

    ~MyVector();

    size_t GetSize() const {
        return size;
    }

    size_t GetCapacity() const {
        return capacity;
    }

    T* GetData() const {
        return data;
    }

    bool Empty() const {
        return (size == 0);
    }

    T& operator[] (size_t index) const {
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
MyVector<T>::MyVector (const std::vector<T>& vec):
    size (vec.size()),
    capacity (vec.size())
    {
        data = (T*) calloc (size, sizeof(data[0]));
        assert (data != nullptr);
        memcpy (data, vec.data(), size * sizeof(T));
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

namespace plugin {
    template<class T>
    struct Array {
        uint64_t size;
        T* data;

        Array(uint64_t _size, const T* _data): size(_size), data(new T[_size]) {
            std::copy(_data, _data + _size, data);
        }

        Array(const Array<T>& other): Array(other.size, other.data) {}
        
        Array& operator=(const Array<T>& other) {
            size = other.size;
            delete data;
            data = new T[other.size];
            std::copy(other.data, other.data + other.size, data);
	        return *this;
        }

        Array(Array<T>&& other) {
            std::swap(size, other.size);
            std::swap(data, other.data);
        }

        Array& operator=(Array<T>&& other) {
            std::swap(size, other.size);
            std::swap(data, other.data);
	        return *this;
        }

        explicit Array (const MyVector<T>& myvec):
            size (myvec.GetSize()),
            data (new T[size])
            {
                for (size_t i = 0; i < size; i++) data[i] = myvec[i];
            }

        ~Array() {
            delete[] data;
        }
    };
}

#endif