/* 
 * Run with
 *    
 *    -p
 *
 * to print correct output.
 */

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <utility>
#include "Map.hpp"

/*
 * Wrapper class around std::map to handle slight difference in return value and also
 * provide an Iterator nested name.
 */

template <typename K, typename V>
class test_map : public std::map<K, V> {
    private:
        using base_t = std::map<K, V>;
    public:
        using Iterator = typename base_t::iterator;
        std::pair<typename base_t::iterator, bool>insert(const std::pair<const K, V> &p) {
            return this->base_t::insert(p);
        }
};


/*
 * MyClass class.
 */

struct MyClass {
    friend bool operator<(const MyClass &o1, const MyClass &o2) {
        return o1.num < o2.num;
    }
    friend bool operator==(const MyClass &o1, const MyClass &o2) {
        return o1.num == o2.num;
    }
    MyClass(double n) : num(n) {}
    double num;
};

void
print(const std::pair<const int, std::string> &p) {
    printf("%d, %s; ", p.first, p.second.c_str());
}


/*
 * Additional test functions for BST.
 */

template <template <typename, typename> class MAP_T>
void traverse2(int level);

/*
 * The actual test code.  It's a template so that it can be run with the std::map and the
 * assignment Map.
 */

template <template <typename, typename> class MAP_T>
void
run_test() {

    /*
     * Test Map with MyClass.
     */

    {
        MAP_T<const MyClass, std::string> map;

        // Empty container, should print nothing.
        for (auto it = map.begin(); it != map.end(); ++it) {
            abort();
        }

        MyClass m1(0), m2(3), m3(1), m4(2);
        auto m1_it = map.insert(std::make_pair(m1, "mmm1"));
        map.insert(std::make_pair(m2, "mmm2"));
        map.insert(std::make_pair(m3, "mmm3"));
        map.insert(std::make_pair(m4, "mmm4"));

        // Should print 0.0 1.0 2.0 3.0
        for (auto &e : map) {
            printf("%3.1f ", e.first.num);
        }
        printf("\n");

        // Check return value of insert.
        {
            // Already in, so must return equal to m1_it.
            auto it = map.insert(std::make_pair(m1, "mmm1"));
            assert(it.first == m1_it.first);
        }

        // Erase the first element.
        map.erase(map.begin());
        // Should print "1.0 2.0 3.0".
        for (auto &e : map) {
            printf("%3.1f ", e.first.num);
        }
        printf("\n");

        // Erase the new first element.
        map.erase(map.begin());
        // Should print "2.0 3.0".
        for (auto &e : map) {
            printf("%3.1f ", e.first.num);
        }
        printf("\n");

        map.erase(--map.end());
        // Should print "2.0".
        for (auto &e : map) {
            printf("%3.1f ", e.first.num);
        }
        printf("\n");

        // Erase the last element.
        map.erase(map.begin());
        // Should print nothing.
        for (auto &e : map) {
            printf("%3.1f ", e.first.num);
        }
        printf("\n");
    }

}

/*
 * Main.
 */

int
main(int argc, char *argv[]) {

    bool correct_output = false;

    {
        int c;
        while ((c = getopt(argc, argv, "pi:")) != EOF) {
            switch (c) {
                case 'p':
                    correct_output = true;
                    break;
                case '?':
                    fprintf(stderr, "Unrecog.\n");
                    exit(1);
            }
        }
    }

    if (correct_output) {
        run_test<test_map>();
    } else {
        run_test<cs540::Map>();
    }
}