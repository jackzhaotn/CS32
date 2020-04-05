//
//  main.cpp
//  Project2
//
//  Created by Jack Zhao on 1/26/20.
//  Copyright © 2020 Jack Zhao. All rights reserved.
//

#include <iostream>
#include "Map.h"

int main() {
    Map m; //creats map
    KeyType key; //creates temp key for later use
    ValueType value; //creates temp value for later use

    m.insert("a", 1); //test insert function
    m.insert("b", 2);
    m.insert("c", 3);
    m.insert("d", 4);
    m.insert("e", 5);

    assert(m.update("a", 11)); //test update function
    assert(m.update("c", 33));
    assert(m.update("e", 55));
    assert(!m.update("f", 11));

    assert(m.contains("a")); //test contains function
    assert(m.contains("c"));
    assert(m.contains("e"));
    assert(!m.contains("f"));

    assert(m.erase("a")); //test erase function
    assert(m.erase("c"));
    assert(m.erase("e"));
    assert(!m.erase("f"));

    assert(m.insert("e", 5));
    assert(m.insert("f", 6));

    
    assert(m.get("b", value)); //test get function
    assert(value == 2);
    assert(m.get("d", value));
    assert(value == 4);
    assert(m.get("f", value));
    assert(value == 6);
    
    m.get(0,key,value); //test other get function
    assert(key == "b" && value == 2);
    m.get("d", value);
    assert(value == 4);

    assert(m.get(0, key, value));
    assert(key == "b" && value == 2);
    assert(m.get(2, key, value));
    assert(key == "e" && value == 5);
    assert(m.get(3, key, value));
    assert(key == "f" && value == 6);

    Map m1; //creates new map
    m1.insert("a", 1);
    m1.insert("b", 22);
    m1.insert("c", 3);

    m.swap(m1); //test swap function
    m.get(0, key, value);
    assert(key == "a" && value == 1);

    Map result;
    result.insert("x", 1);
    result.insert("y", 2);
    result.insert("z", 3);

    assert(!combine(m, m1, result)); //test combine function
    reassign(m1, result); //test reassign function

    result.dump();


    std::cerr << "Program compiled successfully without errors" << std::endl;

}

