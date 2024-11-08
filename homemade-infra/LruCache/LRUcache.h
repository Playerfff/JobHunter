#include <iostream>
#include <unordered_map>
#include <list>
#include <optional>

// Template for LRU Cache
template<typename KeyType, typename ValueType>
class LRUCache {
public:
    LRUCache(size_t capacity) : m_capacity(capacity) {}

    std::optional<ValueType> get(const KeyType& key) {
        // Check if the key exists in the cache
        auto it = m_cacheMap.find(key);
        if (it == m_cacheMap.end()) {
            return std::nullopt; // Key not found
        }
        // Move the accessed item to the front of the list (most recently used)
        m_cacheList.splice(m_cacheList.begin(), m_cacheList, it->second);
        return it->second->second;
    }

    void put(const KeyType& key, const ValueType& value) {
        // Check if the key already exists in the cache
        auto it = m_cacheMap.find(key);
        if (it != m_cacheMap.end()) {
            // Update the value and move the node to the front
            it->second->second = value;
            m_cacheList.splice(m_cacheList.begin(), m_cacheList, it->second);
            return;
        }
        
        // If the cache is full, remove the least recently used item
        if (m_cacheList.size() == m_capacity) {
            auto lru = m_cacheList.back();
            m_cacheMap.erase(lru.first);
            m_cacheList.pop_back();
        }

        // Insert the new item at the front of the list
        m_cacheList.emplace_front(key, value);
        m_cacheMap[key] = m_cacheList.begin();
    }

    void display() const {
        for (const auto& [key, value] : m_cacheList) {
            std::cout << "(" << key << ", " << value << ") ";
        }
        std::cout << std::endl;
    }

private:
    size_t m_capacity;
    std::list<std::pair<KeyType, ValueType>> m_cacheList;
    std::unordered_map<KeyType, typename std::list<std::pair<KeyType, ValueType>>::iterator> m_cacheMap;
};

