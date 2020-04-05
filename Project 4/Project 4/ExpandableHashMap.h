// ExpandableHashMap.h

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.

#ifndef expandableHashMap
#define expandableHashMap

//const int INITNUMBUCKETS = 8;

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
	ExpandableHashMap(double maximumLoadFactor = 0.5);
	~ExpandableHashMap();
	void reset();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);
    

	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
	}
    
    

	  // C++11 syntax for preventing copying and assignment
	ExpandableHashMap(const ExpandableHashMap&) = delete;
	ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
    struct Node
    {
        KeyType key;
        ValueType value;
        Node* next;
    };
    double m_maxLoadFactor;
    Node* *hashTable;
    int m_nBuckets;
    int m_nItems;
};

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor): m_nBuckets(8), m_nItems(0)
{
    if (maximumLoadFactor < 0)
        m_maxLoadFactor = .5;
    else
        m_maxLoadFactor = maximumLoadFactor;

    hashTable = new Node* [m_nBuckets];
    for (int i = 0; i != m_nBuckets; i++)
        hashTable[i]  =  nullptr;
}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
    for (int i = 0; i != m_nBuckets; i++)
    {
        Node* p = hashTable[i];
        while (p != nullptr)
        {
            Node* toBeDeleted = p;
            p =  p->next;
            delete toBeDeleted;
        }
    }
    delete [] hashTable;
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
    for (int i = 0; i != m_nBuckets; i++)
    {
        Node* p = hashTable[i];
        while (p != nullptr)
        {
            Node* toBeDeleted = p;
            p =  p->next;
            delete toBeDeleted;
        }
    }
    delete [] hashTable;
    
    m_nBuckets = 8;
    m_nItems = 0;
    hashTable = new Node*[m_nBuckets];
    for (int i = 0; i != m_nBuckets; i++)
        hashTable[i] = nullptr;
}

template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
    return m_nBuckets;
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    unsigned int hasher(const KeyType& key);
    int bucket = hasher(key) % m_nBuckets; //hashed bucket for specific key
    
    bool exists = false;
    Node* p = hashTable[bucket]; //p points to the node in the hashTable at bucket
    Node* prev = p;
    if (p != nullptr) //if the bucket is not empty
    {
        //check uniqueness of key, if not unique, update to the newest value
        while (p != nullptr)
        {
            if (p->key == key) //
            {
                p->value = value; //updates value
                exists = true;
                break;
            }
            prev = p;
            p =  p->next;
        }
        //inserts
        if (!exists) //at this point, if !exists, that means p = nullptr and prev is right before the nullptr
        {
            Node* newNode = new Node;  //creates new node and assigns key and value
            newNode->key = key;
            newNode->value = value;
            newNode->next = nullptr;
            prev->next = newNode; //last node of the bucket's next points to the new node;
            m_nItems++;
        }
    } else  {
        Node* newNode = new Node;  //creates new node and assigns key and value
        newNode->key = key;
        newNode->value = value;
        newNode->next = nullptr;
        hashTable[bucket] = newNode;
        m_nItems++;
    }
    
    //RESIZE IF LOAD FACTOR IS EXCEEDED
    if ( m_nItems/(1.0 * m_nBuckets) > m_maxLoadFactor)
    {
        //copy old hashTable
        Node* *oldTable = hashTable;
        int oldNumB = m_nBuckets;
        
        //create new hash table with bigger size
        m_nBuckets *= 2;
        hashTable = new Node* [m_nBuckets];
        for (int i = 0; i != m_nBuckets; i++)
            hashTable[i] = nullptr;
        
        //insert all old hash nodes into new hash table
        Node* toBeDeleted;
        for (int i = 0; i != oldNumB; i++) //iterates through every bucket of old bucket
        {
            Node* p = oldTable[i];
            while (p != nullptr)
            {
                Node* hashNode = new Node; //creates new node and assigns key and value
                hashNode->key = p->key;
                hashNode->value = p->value;
                hashNode->next = nullptr;
                int bucket = hasher(hashNode->key) % m_nBuckets;
                Node* q = hashTable[bucket];
                if (q == nullptr)
                    hashTable[bucket] = hashNode; //inserts into new hashtable
                else
                {
                    while (q->next != nullptr)
                    {
                        q = q->next;
                    }
                    q->next = hashNode;  //inserts into new hashtable
                }
                toBeDeleted = p; //dellocates nodes from old hashtable
                p = p->next;
                delete toBeDeleted;
            }
        }
        delete [] oldTable;
    }
    
}


template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
    unsigned int hasher(const KeyType& key);
    int bucket = hasher(key)  % m_nBuckets; //get correlating bucket number
    
    Node* p =  hashTable[bucket]; //gets point to the bucket
    if (p == nullptr)
        return nullptr;
    else if (p->key == key) //if keys match, return value
        return &(p->value);
    else
    {
        while (p != nullptr) //if not, see if it matches a collision
        {
            if (p->key == key)
                return &(p->value);
            p = p->next;
        }
        return nullptr; //if no match
    }
}

#endif
