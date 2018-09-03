#include "Map.hpp"

#include <cassert>

// basically an int wrapper
class MyKeyType {
    private:
        int val;

    public:
        MyKeyType() = default;
        MyKeyType& operator=(const MyKeyType&) = default;
        MyKeyType& operator=(MyKeyType&&) = default;

        // copy constructable and move assignable
        MyKeyType(MyKeyType&&) = default;
        MyKeyType(const MyKeyType&) = default;
        ~MyKeyType() = default;

        MyKeyType(int i) : val(i) { }

        bool operator<(const MyKeyType& other) const { 
            return this->val < other.val;
        }

        bool operator>(const MyKeyType& other) const { 
            return this->val < other.val;
        }

        bool operator==(const MyKeyType &other) const {
            return this->val == other.val;
        }
};


class MyDefaultConstructible {

        friend bool operator<(const MyDefaultConstructible &o1, const MyDefaultConstructible &o2) {
            return o1.val < o2.val;
        }

    private:
        int val = 0;

    public:
        // not copy assignable, not move assignable
        MyDefaultConstructible& operator=(const MyDefaultConstructible&) = delete;
        MyDefaultConstructible& operator=(MyDefaultConstructible&&) = delete;

        // default constructable, copy constructable and move assignable
        MyDefaultConstructible() = default;
        MyDefaultConstructible(MyDefaultConstructible&&) = default;
        MyDefaultConstructible(const MyDefaultConstructible&) = default;
        ~MyDefaultConstructible() = default;

        MyDefaultConstructible(int i) : val(i) { }

        bool operator==(const MyDefaultConstructible &other) const {
            return this->val == other.val;
        }
};

// manual instantiation, instantiates every member function instead of 
// just the ones called
template class cs540::Map<MyKeyType, MyDefaultConstructible>;


int main() {
    cs540::Map<MyKeyType, MyDefaultConstructible> m2{{8, 9}};
    m2[10]; // should default construct these values
    m2[15];

    return 0;
}
