/*

1. Key type is int type. Provides all the types of operators and constructors
2. Value is Just assignable

*/

#include "Map.hpp"

#include <cassert>

class MyAssignable {
    private:
        int val = 0;

    public:
        MyAssignable() = default;
        MyAssignable(int i) : val(i) { }
        bool operator==(const MyAssignable &other) const {
            return this->val == other.val;
        }
};

int main() {
    cs540::Map<int, MyAssignable> m3{{6, 7}};
    m3[20] = {5}; // move assign
    MyAssignable ma{1};
    m3[10] = ma; //copy assign

    return 0;
}
