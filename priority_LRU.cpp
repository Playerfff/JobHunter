#include <ios>
#include <iostream>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <list>
#include <map>

int g_Time = 0;
const int MAX_ITEMS = 1000000;

struct CacheItem {
    std::string key;
    std::string value;
    int priority;
    int expireTime;

    CacheItem(const std::string& k, const std::string& v, int p, int e)
        : key(k), value(v), priority(p), expireTime(e) {}
};

class Cache {
private:
    std::unordered_map<std::string, std::list<CacheItem>::iterator> cache; // 键到 CacheItem 迭代器的映射
    std::map<int, std::list<CacheItem>> priorityMap; // 优先级到 CacheItem 列表的映射
    const int maxItems;

public:
    Cache(int maxItems) : maxItems(maxItems) {}

    std::string Get(const std::string& key) {
        auto it = cache.find(key);
        if (it == cache.end()) {
            return ""; // 键不存在
        }

        // 获取 CacheItem（通过迭代器）
        auto& itemIter = it->second;
        CacheItem& item = *itemIter;

        if (item.expireTime <= g_Time) {
            // 条目已过期，移除
            priorityMap[item.priority].erase(itemIter);
            if (priorityMap[item.priority].empty()) {
                priorityMap.erase(item.priority);
            }
            cache.erase(key);
            return "";
        }

        // 更新 LRU：将条目移到优先级列表的头部
        priorityMap[item.priority].splice(priorityMap[item.priority].begin(),
                                         priorityMap[item.priority], itemIter);
        return item.value;
    }

    void Set(const std::string& key, const std::string& value, int priority, int expireTime) {
        auto it = cache.find(key);
        if (it != cache.end()) {
            // 键已存在，更新
            auto& itemIter = it->second;
            CacheItem& item = *itemIter;

            // 从旧的优先级列表中移除
            priorityMap[item.priority].erase(itemIter);
            if (priorityMap[item.priority].empty()) {
                priorityMap.erase(item.priority);
            }

            // 更新条目
            item.value = value;
            item.priority = priority;
            item.expireTime = expireTime;
        } else {
            // 键不存在，插入新条目
            if (cache.size() >= maxItems) {
                Evict();
            }
            // 插入到优先级列表
            priorityMap[priority].push_front(CacheItem(key, value, priority, expireTime));
            // 插入到 cache，存储迭代器
            cache[key] = priorityMap[priority].begin();
            return;
        }

        // 更新 LRU：将条目插入到新优先级列表的头部
        priorityMap[priority].push_front(CacheItem(key, value, priority, expireTime));
        auto newIter = priorityMap[priority].begin();
        cache[key] = newIter; // 更新 cache 中的迭代器
    }

private:
    void Evict() {
        // 步骤 1：优先移除过期的条目
        for (auto& [priority, itemList] : priorityMap) {
            for (auto it = itemList.begin(); it != itemList.end();) {
                if (it->expireTime <= g_Time) {
                    cache.erase(it->key);
                    it = itemList.erase(it);
                    if (cache.size() < maxItems) {
                        return;
                    }
                } else {
                    ++it;
                }
            }
        }

        // 清理空的优先级列表
        for (auto it = priorityMap.begin(); it != priorityMap.end();) {
            if (it->second.empty()) {
                it = priorityMap.erase(it);
            } else {
                ++it;
            }
        }

        // 步骤 2：移除优先级最低的 LRU 条目
        if (!priorityMap.empty()) {
            auto lowestPriorityIt = priorityMap.begin();
            auto& itemList = lowestPriorityIt->second;
            auto itemIter = --itemList.end(); // 最后一个元素（LRU）

            cache.erase(itemIter->key);
            itemList.erase(itemIter);

            if (itemList.empty()) {
                priorityMap.erase(lowestPriorityIt);
            }
        }
    }
};

// 测试代码
int main() {
    Cache cache(2); // 容量为 2

    cache.Set("key1", "value1", 1, g_Time + 10); // g_Time = 0
    g_Time += 1;
    cache.Set("key2", "value2", 2, g_Time + 5); // g_Time = 1
    g_Time += 1;

    std::cout << "key1: " << cache.Get("key1") << std::endl; // 输出 "value1"
    std::cout << "key2: " << cache.Get("key2") << std::endl; // 输出 "value2"

    g_Time += 5; // g_Time = 7
    std::cout << "key2 after expiration: " << cache.Get("key2") << std::endl; // 输出 ""

    return 0;
}
