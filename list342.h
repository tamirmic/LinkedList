#ifndef LIST342_H_
#define LIST342_H_

#include <iostream>
#include <fstream>
#include <ostream>
using namespace std;

/*
    This program defines a templated linked list class `List342` that has several methods to it including: Insertion, removal, peeking, merging, size, and deletion of lists.
    This class also includes operator overloads for comparisons such as: (==) (!=) (+) (+=) (<<).
*/

template <class T>
struct Node {
    T* data;
    Node* next = nullptr;
};

template <class T>
class List342{
    public:
        //constructors and destructors
        List342();
        List342(const List342& source);
        ~List342();

        //methods
        bool BuildList(string file_name);
        bool Insert(T* obj);
        bool Remove(T target, T& result);
        bool Peek(T target, T& result) const;
        int Size() const;
        void DeleteList();
        bool Merge(List342& list1);

        //operator overloads
        List342<T> operator+(const List342<T>& other) const;
        List342<T>& operator+=(const List342<T>& other);
        template <class U>
        friend ostream& operator<<(ostream& stream, const List342<U>& list);
        bool operator==(const List342<T>& other) const;
        bool operator!=(const List342<T>& other) const;
        List342<T>& operator=(const List342<T>& list);

    private:
        Node<T>* head_;
};

template <class T>
List342<T>::List342() : head_(nullptr){}

template <class T>
List342<T>::List342(const List342 &source) : head_(nullptr){
    *this = source;
}

template <class T>
List342<T>::~List342(){
    DeleteList();
}

template <class T>
bool List342<T>::BuildList(string file_name){
    ifstream in_file;
    in_file.open(file_name);
    if(!in_file){
        return false;
    }

    T temp;
    while(!in_file.eof()){
        in_file >> temp;
        Insert(&temp);
    }
    in_file.close();
    return true;
}

template <class T>
bool List342<T>::Insert(T *obj){
    //make sure obj isn't passed in as nullptr
    if(obj == nullptr){
        return false;
    }

    Node<T>* insert_node = new Node<T>();
    insert_node->data = new T(*obj);

    //check for empty list
    if(head_ == nullptr){
        head_ = insert_node;
        return true;
    }

    Node<T>* current = head_;
    Node<T>* prev = nullptr;

    while(current != nullptr){
        //check for duplicates first
        if(*(current->data) == *(insert_node->data)){
            delete insert_node->data;
            delete insert_node;
            return false;
        }
        if(*(insert_node->data) < *(current->data)){
            break;
        }
        prev = current;
        current = current->next;
    }

    //case for first node
    if(prev == nullptr){
        insert_node->next = head_;
        head_ = insert_node;
    //case for anything but first node
    } else{
        prev->next = insert_node;
        insert_node->next = current;
    }
    return true;
}

template <class T>
bool List342<T>::Remove(T target, T& result){
    if(head_ == nullptr){
        return false;
    }

    //check first node
    if(*(head_->data) == target){
        result = *(head_->data);
        Node<T>* temp = head_;
        head_ = head_->next;
        delete temp->data;
        delete temp;
        return true;
    }

    Node<T>* current = head_;
    while(current->next != nullptr){
        if(*(current->next->data) == target){
            result = *(current->next->data);
            Node<T>* temp = current->next;
            current->next = current->next->next;
            delete temp->data;
            delete temp;
            return true;
        }
        current = current->next;
    }
    //target not found
    return false;
}

template <class T>
bool List342<T>::Peek(T target, T& result) const{
    if(head_ == nullptr){
        return false;
    }

    if(*(head_->data) == target){
        result = *(head_->data);
        return true;
    }

    Node<T>* current = head_;
    while(current->next != nullptr){
        if(*(current->next->data) == target){
            result = *(current->next->data);
            return true;
        }
        current = current->next;
    }
    return false;
}

template <class T>
int List342<T>::Size() const{
    int size = 0;
    Node<T>* current = head_;

    while(current != nullptr){
        size++;
        current = current->next;
    }
    return size;
}
 
template <class T>
void List342<T>::DeleteList(){
    Node<T>* current = head_;
    while(current != nullptr){
        Node<T>* temp = current;
        current = current->next;
        delete temp->data;
        delete temp;
    }
    head_ = nullptr;
}

template <class T>
bool List342<T>::Merge(List342& list1){
    //check if merging itself
    if(this == &list1){
        return false;
    }

    Node<T>* current = head_;
    Node<T>* list1_current = list1.head_;
    Node<T>* prev = nullptr;

    while (list1_current != nullptr) {
        //check if current of list1 should be inserted into this list
        if(current == nullptr || *(list1_current->data) < *(current->data)){
            Node<T>* temp = list1_current;
            list1_current = list1_current->next;
            temp->next = current;
            
            //insert at the head
            if(prev == nullptr){
                head_ = temp;
            } else{
                prev->next = temp;
            }
            prev = temp;

        //check for duplicates between list1 and this list
        } else if(*(list1_current->data) == *(current->data)){
            Node<T>* temp = list1_current;
            list1_current = list1_current->next;
            delete temp->data;
            delete temp;

        //no node got inserted simply go to next node in this list
        } else{
            prev = current;
            current = current->next;
        }
    }
    list1.head_ = nullptr;
    return true;
}

template <class T>
List342<T> List342<T>::operator+(const List342<T> &other) const{
    List342<T> this_copy = *this;
    List342<T> other_copy = other;
    this_copy += other_copy;
    return this_copy;
}

template <class T>
List342<T>& List342<T>::operator+=(const List342<T> &other){
    List342<T> other_copy = other;
    Merge(other_copy);
    return *this;
}

template <class T>
ostream& operator<<(ostream& stream, const List342<T>& list){
    Node<T>* current = list.head_;
    while(current != nullptr){
        stream << *(current->data);
        current = current->next;
    }
    return stream;
}

template <class T>
bool List342<T>::operator==(const List342<T>& other) const{    
    if(Size() != other.Size()){
        return false;
    }

    Node<T>* current = head_;
    Node<T>* other_current = other.head_;

    while(current != nullptr && other_current != nullptr){
        if(*(current->data) != *(other_current->data)){
            return false;
        }
        current = current->next;
        other_current = other_current->next;
    }
    return true;
}

template <class T>
bool List342<T>::operator!=(const List342<T>& other) const{
    return !(*this == other);
}

template <class T>
List342<T>& List342<T>::operator=(const List342<T>& list){
    if(this == &list){
        return *this;
    }
    
    //delete everything in the current list
    DeleteList();

    if(list.head_ == nullptr){
        head_ = nullptr;
        return *this;
    }

    //copy all of the nodes in list to this
    Node<T>* current = list.head_;
    while(current != nullptr){
        Insert(current->data);
        current = current->next;
    }
    return *this;
}
#endif