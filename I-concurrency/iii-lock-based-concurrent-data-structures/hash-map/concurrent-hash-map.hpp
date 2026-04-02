#include<thread>
#include <iostream>
#ifndef CONCURRENT_HASH_MAP
#define CONCURRENT_HASH_MAP

template<typename K, typename V>
class ConcurrentHashMap {
private:
    struct Node {
        K key;
        V value;
        Node* next;
    };

    struct Bucket {
        std::mutex mtx;
        Node* head;
    };


    std::vector<Bucket> buckets;
    size_t capacity;

    size_t hash(const K& key) const {
        return (std::hash<K>{}(key) + capacity) % capacity;
    }
public:
    ConcurrentHashMap(size_t initial_capacity) {
        this->capacity = initial_capacity;
        this->buckets = std::vector<Bucket>(initial_capacity);
    }

    void put(const K& key, const V& value) {
        size_t idx = this->hash(key);
        Bucket &bucket = this->buckets[idx];

        // we will acquire the lock of this bucket
        std::lock_guard<std::mutex> lock(bucket.mtx);

        Node* curr = bucket.head;

        while (curr) {
            // if key is already present, just override that key
            if (curr->key == key) {
                curr->value = value;
                return;
            }
            curr = curr->next;
        }
        Node* newNode = new Node{key, value, bucket.head};
        bucket.head = newNode;
    }

    V get(const K& key) {
        size_t idx = this->hash(key);
        // handle this in different scope, because lock will automatically be unlocked
        {
            Bucket &bucket = this->buckets[idx];
        
            std::lock_guard<std::mutex> lock(bucket.mtx);
            Node* curr = bucket.head;
            
            while (curr) {
                if (curr->key == key) {
                    return curr->value;
                }
                curr = curr->next;
            }
        }
        
        // if no key found then create new one with default value
        this->put(key, V());
        return V();
    }

    void remove(const K& key) {
        size_t idx = this->hash(key);
        Bucket &bucket = this->buckets[idx];
        
        std::lock_guard<std::mutex> lock(bucket.mtx);
        Node* curr = bucket.head;
        Node* prev = nullptr;
        
        while (curr) {
            if (curr->key == key) {
                if (prev) 
                    prev->next = curr->next;
                else 
                    bucket.head = curr->next;
                delete curr;
                return;
            }
            
            prev = curr;
            curr = curr->next;
        }
    }
    bool contains(const K& key) {
        size_t idx = this->hash(key);
        Bucket &bucket = this->buckets[idx];
        
        std::lock_guard<std::mutex> lock(bucket.mtx);
        Node* curr = bucket.head;
        
        while (curr) {
            if (curr->key == key) {
                return true;
            }
            curr = curr->next;
        }

        return false;   
    }

    void print() {
        for (Bucket &bucket: this->buckets) {
            // acquire the lock
            std::lock_guard<std::mutex> lock(bucket.mtx);
            Node* curr = bucket.head;

            while (curr) {
                std::cout << curr->key << " " << curr->value << "\n";
                curr = curr->next;
            }
        }
    }
};




#endif