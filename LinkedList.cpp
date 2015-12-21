#include <iostream>     // std::ostream
#include <stdexcept>    // std::out_of_range
#include <string>       // std::to_string()
#include <utility>      // std::swap()

#include "LinkedList.h"

using std::swap;  // Чтобы использовать swap() без std::

LinkedList& LinkedList::operator=(LinkedList rhs)
{
    swap(*this, rhs);
    return *this;

}

LinkedList& LinkedList::operator=(LinkedList&& rhs)
{
    clear();
    swap(*this, rhs);
    return *this;
}

bool LinkedList::operator==(const LinkedList& other) const
{
    if (this->size() == other.size())
    {
        Node* current = other.first;
        Node* current1 = this->first;
        bool flag=true;
        while (current)
        {
            if (current1->value != current->value)
            {
                flag=false;
                break;
            }
            current = current->next;
            current1 = current1->next;
        }
        return flag;
    }
    return false;

}

bool LinkedList::operator!=(const LinkedList& other) const
{
    return !(*this == other);
}



void LinkedList:: clear()
{
    while (size_)
        erase (first);
}
void LinkedList::erase(const Node* node)
{
    //лекция 7 стр 8
    if (node->next)
        node->next->previous=node->previous;
    else
        last=node->previous;

    if (node->previous)
        node->previous->next=node->next;
    else
        first=node->next;
    delete node;
    --size_;
}

void LinkedList::pop_back()
{
    erase(last);
}

void LinkedList::pop_front()
{
    erase(first);
}

LinkedList::Node* LinkedList::insert_after(LinkedList::Node* after)
{
    return nullptr;
}


LinkedList::Node* LinkedList::insert_before(LinkedList::Node* before)
{
    return nullptr;
}

void LinkedList::push_back(const Data& value)
{
    // аналогично push_front
    Node* node = new Node;
    node->value = value;
    node->previous = last;
    node->next = nullptr;
    if (last != nullptr)
    {
        last->next = node;
    }
    last = node;
    if (first == nullptr)
    {
        first = node;
    }
    ++size_;
}

void LinkedList::push_front(const Data& value)
{
    // лекция 7 стр 8
    Node* node = new Node;
    node->value = value;
    node->previous = nullptr;
    node->next = first;
    if (first != nullptr)
    {
        first->previous = node;
    }
    first = node;
    if (last == nullptr)
    {
        last = node;
    }
    ++size_;
}


void swap(LinkedList& left, LinkedList& right)
{
    // HINT: Функция std::swap() меняет значения простых типов.
	// вначале прописано using std::swap; Чтобы использовать swap() без std::
	//лекция 8 стр 5
    std::swap(right.first,left.first);
    std::swap(right.last,left.last);
    std::swap(right.size_,left.size_);
}

LinkedList::LinkedList()
    :first { nullptr }, last { nullptr }, size_ { 0 }

{}



LinkedList::LinkedList(const std::initializer_list<Data> values) :
    LinkedList()
{
    // По std::initializer_list<T> возможен диапазонный for.
    for (const Data& value : values)
    {
        push_back(value);
    }
}

LinkedList::LinkedList(const LinkedList& source)
    :LinkedList() ////иницилизация списка пустым, чтобы можно было вызвать push_back()
{
    //лекция 8 стр 3
    Node* current = source.first;
    while(current)
    {
        push_back(current->value);
        current = current->next;
    }
}

LinkedList::LinkedList(LinkedList&& source) //moving
    : LinkedList()
{
    //swap(*this, source);
    first = source.first;
    last = source.last;
    size_ = source.size_;

    source.first = nullptr;
    source.last = nullptr;
    source.size_ = 0;
}

LinkedList::~LinkedList()
{
    clear();
}

size_t LinkedList::size() const
{
    return LinkedList::size_;

}

Data& LinkedList::value_at(size_t index) //добавить в
{
    //поиск узла и добавление
    Node* current = first;
    size_t current_index = 0;
    if (index>=size_) throw std::out_of_range("Invalid index!");
    else
    {
        while (current)
        {
            if (current_index == index)
            {
                return current->value;
            }
            ++current_index;
            current = current->next;
        }
    }
}

void LinkedList::remove_at(size_t index)
{
    Node* node = (*this).node_at(index); //поиск узла
    erase(node); //удаление
}


void LinkedList::insert_before(size_t index, const Data& value) // вставить до
{
    Node* where = (*this).node_at(index); //поиск узла
    //аналогично push_front
    Node* node = new Node;
    node->value = value;
    node->previous = where->previous;
    node->next = where;
    if  ( node->previous!= nullptr )
    {
        node->previous->next = node;
    }
    if (where-> previous == nullptr)
    {
        first=node;
    }

    where->previous = node;
    ++size_;
}


void LinkedList::insert_after(size_t index, const Data& value) // встатвить после
{
    Node* where = (*this).node_at(index); //поиск узла
	//аналогично push_back
    Node* node = new Node;
    node->value = value;
    node->next = where->next;
    node->previous = where;
    if  ( node->next!= nullptr )
    {
        node->next->previous = node;
    }
    if (where-> next == nullptr)
    {
        last=node;
    }

    where->next = node;
    ++size_;

}

LinkedList::Node* LinkedList::node_at(size_t index)
{
    // лекция 7 стр 9
    Node* current = first;
    size_t current_index = 0;
    if (index>=LinkedList::size_) throw std::out_of_range("Invalid index!");
    else
    {
        while (current)
        {
            if (current_index == index)
            {
                return current;
            }
            ++current_index;
            current = current->next;
        }
    }
}

std::ostream& operator<<(std::ostream& output, const LinkedList& xs)
{
    // лекция 7 стр 7
    LinkedList::Node* current = xs.first;
    if (xs.size()==0) output<<"[]";
    else
    {
        output <<'[';
        while (current!=xs.last)
        {
            output<<current->value;
            current = current->next;
            output<<", ";
        }
        output<<xs.last->value;
        output <<']';
    }

    return output;
}
