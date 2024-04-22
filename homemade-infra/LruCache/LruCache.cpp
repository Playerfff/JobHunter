#include <cstdio>
#include <list>
#include <unordered_map>
using namespace std;

class LRUCache {
  list<pair<int, int>> l;
  unordered_map<int, list<pair<int, int>>::iterator> mp;
  unsigned capacity;

public:
  LRUCache(unsigned cap) : capacity(cap) {}

  int get(int key) {
    auto iter = mp.find(key);
    if (iter != mp.end()) {
      l.splice(l.begin(), l, iter->second); // 没有迭代器或指针失效！！！
      return iter->second->second;          // return value
    }
    return -1;
  }

  void put(int key, int value) {
    auto iter = mp.find(key);
    if (iter != mp.end()) {
      iter->second->second = value;
    } else {
      if (capacity) {
        if (mp.size() >= capacity) {
          mp.erase(l.end()->first);
          l.erase(l.end());
        }
        mp[key] = l.insert(l.begin(), {key, value});
      }
    }
  }
};

int main() {
  int capacity, x, y;
  scanf("%d", &capacity);
  LRUCache cache(unsigned(capacity < 0 ? 0 : capacity));
  for (char op[4]; scanf("%s%d", op, &x) != EOF;) {
    if (*op == 'g') {
      printf("%d\n", cache.get(x));
    } else {
      scanf("%d", &y);
      cache.put(x, y);
    }
  }
  return 0;
}