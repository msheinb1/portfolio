/* 
 * Run with
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
     * Test Map with plain int.
     */

    {
        MAP_T<const int, std::string> map;

        // Empty container, should print nothing.
        for (auto &e : map) {
            printf("%d ", e.first);
        }

        auto p1(std::make_pair(4, "444"));
        auto p2(std::make_pair(3, "333"));
        auto p3(std::make_pair(0, "000"));
        auto p4(std::make_pair(2, "222"));
        auto p5(std::make_pair(1, "111"));

        map.insert(p1);
        map.insert(p2);
        map.insert(p3);
        map.insert(p4);
        map.insert(p5);

        // Should print "0 1 2 3 4".
        for (auto it = map.begin(); it != map.end(); ++it) {
            print(*it);
        }
        printf("\n");

        // Insert dupes.
        map.insert(p4);
        map.insert(p1);
        map.insert(p3);
        map.insert(p2);
        map.insert(p5);
        // Should print "0 1 2 3 4".
        for (auto it = map.begin(); it != map.end(); ++it) {
            print(*it);
        }
        printf("\n");

        // Erase the first element.
        map.erase(map.begin());

        // Erase the new first element.
        map.erase(map.begin());

        // Erase the element in the end.
        map.erase(--map.end());
        // Should print "2 3".
        for (auto &e : map) {
            print(e);
        }
        printf("\n");

        // Erase all elements.
        map.erase(map.begin());
        map.erase(map.begin());
        // Should print nothing.
        for (auto &e : map) {
            print(e);
        }
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

    srand48(1234);

    if (correct_output) {
        run_test<test_map>();
    } else {
        run_test<cs540::Map>();
    }
}