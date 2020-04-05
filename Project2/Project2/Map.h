//
//  Map.h
//  Project2
//
//  Created by Jack Zhao on 1/26/20.
//  Copyright © 2020 Jack Zhao. All rights reserved.
//

#ifndef Map_h
#define Map_h

#include <iostream>
#include <stdio.h>

using KeyType = std::string;
using ValueType = double;



class Map
{
  public:
    Map();
    ~Map();
    Map(const Map &src);   //Copy constructor
    Map &operator=(const Map &src);     //Assignment operator
    bool empty() const;
    int size() const;
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);


    void dump() const;
    
    
  private:
    struct Node
    {
        KeyType key;
        ValueType value;
        Node* next;
        Node* prev;
    };
    Node* head;

    int m_size;
    
};

bool combine(const Map& m1, const Map& m2, Map& result);
void reassign(const Map& m, Map& result);

#endif /* Map_h */
