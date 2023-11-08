#ifndef MYLIST_H
#define MYLIST_H

#include <stddef.h>
#include <iostream>
#include <assert.h>

template <typename T>
class ListNode {
    public:
    ListNode<T>* next;
    ListNode<T>* prev;
    T val;

    explicit ListNode ();
    explicit ListNode (const T& val_);
};

template <typename T>
ListNode<T>::ListNode ():
    next (nullptr), 
    prev (nullptr),
    val ()
    {}

template <typename T>
ListNode<T>::ListNode (const T& val_):
    next (nullptr), 
    prev (nullptr),
    val (val_)
    {}


// !! can be used only with types with default constructor !!
template <typename T>
class MyList {
    ListNode<T>* fic;
    size_t size;

    public:
    explicit MyList();

    ~MyList();

    ListNode<T>* GetHead() const {return fic -> next;}

    ListNode<T>* GetTail() const {return fic -> prev;}

    ListNode<T>* EndOfList() const {return fic;}

    size_t GetSize() const {return size;}

    void InsertHead(const T& val);

    void InsertTail(const T& val);

    void Insert (ListNode<T>* prev_elem, const T& val);

    void Clear();
    
    void RemoveHead();

    void RemoveTail();

    void Remove (ListNode<T>* node);

    bool NodeInList (ListNode<T>* node) const;

    void InsertElems (const MyList<T>& lst);

    ListNode<T>* GetNode (const T& val) const;

    void Iterate (ListNode<T>*& node) const;
};

template <typename T>
MyList<T>::MyList():
    fic (new ListNode<T>()),
    size (0)
    {
        fic -> next = fic;
        fic -> prev = fic;
    }

template <typename T>
MyList<T>::~MyList() {
    Clear();
    delete fic;
}

template <typename T>
void MyList<T>::InsertHead(const T& val) {
    ListNode<T>* node = new ListNode<T> (val);
    node -> next = GetHead();
    GetHead() -> prev = node;
    node -> prev = fic;
    fic -> next = node;
    size++;
}

template <typename T>
void MyList<T>::InsertTail(const T& val) {
    ListNode<T>* node = new ListNode<T> (val);
    node -> prev = GetTail();
    GetTail() -> next = node;
    node -> next = fic;
    fic -> prev = node;
    size++;
}

template <typename T>
void MyList<T>::Insert(ListNode<T>* prev_elem, const T& val) {
    assert (NodeInList(prev_elem));

    ListNode<T>* node = new ListNode<T> (val);
    
    node -> next = prev_elem -> next;
    node -> prev = prev_elem;
    prev_elem -> next = node;
    node -> next -> prev = node;
    size++;
}

template <typename T>
void MyList<T>::Clear() {
    ListNode<T>* elem = fic -> next;
    while (elem != fic) {
        elem = elem -> next;
        delete elem -> prev;
    }
    fic -> next = fic;
    fic -> prev = fic;
    size = 0;
}

template <typename T>
void MyList<T>::RemoveHead() {
    if (size <= 0) return;

    ListNode<T>* new_head = GetHead() -> next;
    delete GetHead();
    fic -> next = new_head;
    new_head -> prev = fic;
    size--;
}

template <typename T>
void MyList<T>::RemoveTail() {
    if (size <= 0) return;

    ListNode<T>* new_tail = GetTail() -> prev;
    delete GetTail();
    fic -> prev = new_tail;
    new_tail -> next = fic;
    size--;
}

template <typename T>
void MyList<T>::Remove(ListNode<T>* node) {
    assert (node != nullptr);
    assert (NodeInList(node));

    node -> next -> prev = node -> prev;
    node -> prev -> next = node -> next;
    delete node;
    size--;
}

template <typename T>
bool MyList<T>::NodeInList(ListNode<T>* node) const {
    ListNode<T>* current = GetHead();
    while (current != fic) {
        if (current == node) return true;
        current = current -> next;
    }
    return false;
}

template <typename T>
void MyList<T>::InsertElems (const MyList<T>& lst) {
    ListNode<T>* end_of_list = lst.EndOfList();
    ListNode<T>* node = lst.GetHead();

    while (node != end_of_list) {
        InsertTail (node -> val);
        node = node -> next;
    }
}

template <typename T>
ListNode<T>* MyList<T>::GetNode (const T& val) const {
    ListNode<T>* current = GetHead();
    while (current != fic) {
        if (current -> val == val) return current;
        current = current -> next;
    }
    return nullptr;
}

template <typename T>
void MyList<T>::Iterate (ListNode<T>*& node) const {
    if (node == nullptr) {
        node = GetHead();
    }
    else {
        assert (NodeInList(node));
        node = node -> next;
    }
    if (node == fic) node = nullptr;
}

#endif