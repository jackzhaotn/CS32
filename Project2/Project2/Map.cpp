//
//  Map.cpp
//  Project2
//
//  Created by Jack Zhao on 1/26/20.
//  Copyright © 2020 Jack Zhao. All rights reserved.
//

#include "Map.h"
#include <iostream>



Map::Map()
{
    head = nullptr; //sets head pointer as nullptr
    m_size = 0;
    
}

Map::~Map() {
    Node* p = head;
    while (p != nullptr) {
        Node* deleteNode = p;
        p = p->next;
        delete deleteNode;
    }
}

Map::Map(const Map &src) //copy constructer
{
    head = nullptr;
    m_size = 0;
    
    for (int i = 0; i != m_size; i++)
    {
        KeyType k;
        ValueType v;
        src.get(i, k, v);
        insert(k, v);
    }
}
Map& Map::operator=(const Map &src)  //assignment operator
{

    if (&src == this)
        return *this;
    Map temp = src;
    swap(temp);
    return *this;
}

bool Map::empty() const
{
    return (head == nullptr); //if size is zero, then the list is obviously empty

}

int Map::size() const //returns the m_size
{
    return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value) //inserts node at end of list
{
    if (contains(key)) //returns false for duplicate key
        return false;
    
    Node* newNode = new Node;  //initilizes to-be inserted node and its data
    newNode->key = key;
    newNode->value = value;
    
    if(empty())
    {
        newNode->next = head; //if empty list, the next and prev of the node will be nullptr
        newNode->prev = head;
        head = newNode; //head points to the new node
        m_size++;
    }
    else
    {
        Node* p = head; //creates a pointer that will be iterated to the last node
        while (p->next != nullptr)
            p = p->next;

        p->next = newNode; //assigns the next of the last node to point to newNode
        newNode->next = nullptr;
        newNode->prev = p; //newNode prev will point back to the original last node
        m_size++;
    }
    return true;

}

bool Map::update(const KeyType& key, const ValueType& value)
{
    Node* p = head;
    while (p != nullptr) //iterates through nodes
    {
        if (p->key == key) //finds matching key
        {
            p->value = value; //reassigns the value to the parameter value
            return true;
        }
        else
            p = p->next; //continue iterating
    }
    return false;
}

//same thing as last two functions
bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    //implementation is similar to insert and update function so no further comment is neccessary
    Node* newNode = new Node;
    newNode->key = key;
    newNode->value = value;
    
    if(empty())
    {
        newNode->next = head;
        newNode->prev = head;
        head = newNode;
        m_size++;
    }
    else
    {
        Node* p = head;
        while (p != nullptr)
        {
            if (p->key == key)
            {
                p->value = value;
                return true;
            }
            else
                p = p->next;
        }
        p->next = newNode;
        newNode->next = nullptr;
        newNode->prev = p;
        m_size++;
    }
    return true;
}

bool Map::erase(const KeyType& key)
{
    if (empty()) //nothing to erase if empty to return false
        return false;
    if (head->key == key) //if key to be erased is the first node key
    {
        Node* p = head; //reassigns head to the next value
        head = p->next;
        delete p; //then deletes original head
        m_size--; //decrement size
        return true;
    }
    
    Node* p = head;
    while (p != nullptr) //iterates through nodes
    {
        if (p->key == key) //if key matches
        {
            Node* o = p->prev; //temp nodes pointing to the prev and next nodes
            Node* q = p->next;
            o->next = q; //reassigns the next value
            
            if(q == nullptr) //in case the node in question is the end node
            {
                delete p;
                return true;
            }
            q->prev = o;  //reassigns the next node's previous
            delete p; //now safe to delete
            return true;
        }
        else
        {
            p = p->next;
        }
    }
    
    return false;
}

bool Map::contains(const KeyType& key) const
{
    Node* p = head;
    while (p != nullptr) //iterates through nodes
    {
        if (p->key == key) //finds matching key then return true
            return true;
        else
            p = p->next; //continue interating
    }
    return false; //if no key match, then return false
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    Node* p = head;
    while (p != nullptr) //iterates through nodes
    {
        if (p->key == key) //finds matching key
        {
            value = p->value; //assigns reference value to node value
            return true;
        }
        else
            p = p->next; //continues iteration
    }
    return false; //if no key match, then return false
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (i < 0 && i >= m_size) //if i is out of bounds
        return false;
    
    int count = 0;
    
    Node* p = head;
    while (p != nullptr) //iterates through nodes until it hits i position
    {
        if (count == i) //once iteratation reaches i, fill the reference type and value with the node type and value
        {
            key = p->key;
            value = p->value;
            return true;
        }
        else //iterates to the next
        {
            p = p->next;
            count++;
        }
    }
    return false;
}

void Map::swap(Map& other)
{
    //creates a temp size to switch the size values
    int tempSize = m_size;
    m_size = other.m_size;
    other.m_size = tempSize;
    
    //creates a temp node to switch the node heads
    Node* tempHead = head;
    head = other.head;
    other.head = tempHead;
}

bool combine(const Map& m1, const Map& m2, Map& result)
{
    //temp values
    KeyType k;
    ValueType v;
    ValueType dupeV;
    bool returnTrue = true;
    
    //clears results if not empty
    if(!result.empty())
    {
        for (int i = 0; i != result.size(); i++) //erases the first node until there are none
        {
            result.get(0, k, v);
            result.erase(k);
        }
        
    }

    for (int i = 0; i != m1.size(); i++) //inserts m1 nodes to results as is
    {
        m1.get(i, k, v);
        result.insert(k, v);
    }

    
    for (int i = 0; i != m2.size(); i++) //iterates through m2 nodes
    {
        m2.get(i, k, v);
        if (result.contains(k)) //if there is a duplicate key
        {
            result.get(k, dupeV);
            if (v != dupeV) //if dupe key has different value then erase and return false
            {
                result.erase(k);
                returnTrue = false;
            }
        }
        else //if no dupes, just insert as is
        {
            result.insert(k, v);
        }
        
    }
    return returnTrue;
}

void reassign(const Map& m, Map& result)
{
    //temp values
    KeyType k1;
    ValueType v1;
    KeyType k2;
    ValueType v2;

    
    if(!result.empty()) //clears results if it isn't empty
    {
        for (int i = 0; i != result.size(); i++) //erases the first node until there are none
        {
            result.get(0, k1, v1);
            result.erase(k1);
        }
        
    }
    
    if (m.size() == 1) //if theres only one node, then just straight copy to result
    {
        m.get(0, k1, v1);
        result.insert(k1, v1);
    }
    
    for (int i = 0; i != m.size() - 1; i++) //replaces the key values to the value of the next node
    {
        m.get(i, k1, v1);
        m.get(i+1, k2, v2);
        result.insert(k1, v2);
    }
    //update last node with the value of the head and insert to result
    m.get(0, k1, v1);
    m.get(m.size() -1, k2, v2);
    result.insert(k2, v1);
    
}

void Map::dump() const //prints nodes in order
{
    Node* p = head;
    int count = 0;
    while (p != nullptr) {
        std::cerr << "Node " << count + 1 << " -- " << p->key << p->value << std::endl;
        p = p->next;
        count++;
    }

 }


