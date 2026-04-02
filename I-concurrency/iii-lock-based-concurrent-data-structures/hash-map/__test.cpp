#include <iostream>
#include "concurrent-hash-map.hpp"

using namespace std;

int main() {
    ConcurrentHashMap<int, int> chmp(10);
    
    chmp.put(2, 3);
    assert(chmp.get(2) == 3);
    
    chmp.put(4, 12);
    assert(chmp.get(4) == 12);
    assert(chmp.get(2) == 3);
    
    chmp.put(-1, 2);
    assert(chmp.get(4) == 12);
    assert(chmp.get(2) == 3);
    assert(chmp.get(-1) == 2);
    chmp.print();
    chmp.put(2, 10);
    assert(chmp.get(2) == 10);
    
    chmp.print();
    assert(chmp.contains(-1));
    assert(!chmp.contains(19));
    
    // let's remove
    chmp.remove(-1);
    assert(!chmp.contains(-1));
    chmp.print();
    
    chmp.remove(2);
    assert(chmp.get(2) == 0);
    assert(chmp.contains(2));
}