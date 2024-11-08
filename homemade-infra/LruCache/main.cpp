#include "LRUcache.h"

int main() {
    LRUCache<int, std::string> cache(3);

    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");
    cache.display(); // Expected: (3, three) (2, two) (1, one)

    cache.get(2);
    cache.display(); // Expected: (2, two) (3, three) (1, one)

    cache.put(4, "four");
    cache.display(); // Expected: (4, four) (2, two) (3, three) - (1, one) evicted

    return 0;
}
