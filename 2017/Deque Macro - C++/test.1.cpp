/*
 * Compile with -ldl.
 */

#include <dlfcn.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <random>
#include <unistd.h>
#include "Deque.hpp"

//INCLUDE IN DEQUE.HPP
#include <string.h>

// May assume memcpy()-able.
// May assume = operator.

/*
 * Track memory allocation.
 */

// This functions like an assert(), but is careful not to do anything that
// calls malloc(), so can be used before malloc has been initialized.
#define xyzzy_check(e) do {                                                    \
    if (!(e)) {                                                                \
        const char s[] = #e "\n";                                              \
        write(2, s, sizeof s);                                                 \
        abort();                                                               \
    }                                                                          \
} while (0)

size_t alloc_call_count;
size_t total_bytes_allocated;

namespace {
    bool initialized;
    void *(*default_malloc)(size_t);
    void *(*default_realloc)(void *, size_t);
    void *(*default_calloc)(size_t, size_t);
    void
    alloc_init() {
        if (!initialized) {
            // Cast through int to avoid warnings on some versions of g++.
            default_malloc = (void*(*)(size_t)) (uintptr_t) dlsym(RTLD_NEXT, "malloc"); xyzzy_check(default_malloc != nullptr);
            default_realloc = (void*(*)(void*,size_t)) (uintptr_t) dlsym(RTLD_NEXT, "realloc"); xyzzy_check(default_realloc != nullptr);
            default_calloc = (void*(*)(size_t,size_t)) (uintptr_t) dlsym(RTLD_NEXT, "calloc"); xyzzy_check(default_calloc != nullptr);
            initialized = true;
        }
    }
}

void *
malloc(size_t size) noexcept {
    alloc_init();
    void *ptr = default_malloc(size);
    alloc_call_count++;
    total_bytes_allocated += size;
    return ptr;
}

void *
realloc(void *p, size_t size) noexcept {
    alloc_init();
    void *ptr = default_realloc(p, size);
    alloc_call_count++;
    total_bytes_allocated += size;
    return ptr;
}

void *
calloc(size_t num, size_t size) noexcept {
    alloc_init();
    void *ptr = default_calloc(num, size);
    alloc_call_count++;
    total_bytes_allocated += num*size;
    return ptr;
}

/*
 * Test for class MyClass.
 */

struct MyClass {
    int id;
  char name[10];
};

bool
MyClass_less_by_id(const MyClass &o1, const MyClass &o2) {
    return o1.id < o2.id;
}

void
MyClass_print(const MyClass *o) {
    printf("%d\n", o->id);
    printf("%s\n", o->name);
}

//Deque_DEFINE(MyClass)

struct Deque_MyClass;
struct Deque_MyClass_Iterator {
    int current;
    Deque_MyClass * aDeq;
    
    void (*inc)(Deque_MyClass_Iterator * it);
    MyClass & (*deref)(Deque_MyClass_Iterator * it);
    void (*dec)(Deque_MyClass_Iterator * it);
};

struct Deque_MyClass {
    int members;
    int head;
    int sizeMax;
    int tail;
    struct MyClass* arr;
    char type_name[sizeof("Deque_MyClass")];
    struct Deque_MyClass_Iterator * it;
    bool (*compare)(const MyClass&, const MyClass&);
    
    int (*size)(Deque_MyClass * deq);
    bool (*empty)(Deque_MyClass * deq);
    
    void (*push_back)(Deque_MyClass * deq, MyClass temp);
    void (*push_front)(Deque_MyClass * deq, MyClass temp);
    
    MyClass & (*front)(Deque_MyClass * deq);
    MyClass & (*back)(Deque_MyClass * deq);
    MyClass & (*at)(Deque_MyClass * deq, int i);
    void (*pop_front)(Deque_MyClass * deq);
    void (*pop_back)(Deque_MyClass * deq);
    void (*increaseSize)(Deque_MyClass * deq);

    Deque_MyClass_Iterator * startVal;    
    Deque_MyClass_Iterator * endVal;
    Deque_MyClass_Iterator (*begin)(Deque_MyClass * deq);
    Deque_MyClass_Iterator (*end)(Deque_MyClass * deq);
    
    void (*clear)(Deque_MyClass * deq);
    void (*dtor)(Deque_MyClass * deq);
};

void inc(Deque_MyClass_Iterator * it) {
    it->current = (it->current+it->aDeq->sizeMax+1)%(it->aDeq->sizeMax);
}

void dec(Deque_MyClass_Iterator * it) {
    it->current = (it->current+it->aDeq->sizeMax-1)%(it->aDeq->sizeMax);
}

MyClass & deref(Deque_MyClass_Iterator * it) {
    return it->aDeq->arr[it->current];
}

int size(Deque_MyClass * deq) {
    return deq->members;
}

bool empty(Deque_MyClass * deq) {
    if (size(deq) == 0) {
        return true;
    } else {
        return false;
    }
}

void increaseSize(Deque_MyClass * deq) {
    int oldSize = deq->sizeMax;
    deq->sizeMax = deq->sizeMax * 2;
    MyClass * tempArr = (MyClass*) malloc(deq->sizeMax * sizeof(struct MyClass));
    int current = deq->head;
    for (int i = 0; i < oldSize; i++) {
        tempArr[i] = deq->arr[current];
        current = (current+oldSize+1)%oldSize;
    }
    free(deq->arr);
    deq->arr = tempArr;
}

void push_back(Deque_MyClass * deq, MyClass temp) {
    if (deq->members == (deq->sizeMax - 1)) {
        increaseSize(deq);
    }
    if (deq->members == 0) {
        //insert at head
        deq->arr[deq->head] = temp;
        //make tail one past head
        deq->tail = (deq->head+deq->sizeMax+1)%(deq->sizeMax);
    } else {
        //head doesn't change, get index before tail
        deq->arr[deq->tail] = temp;
        //add 1 to tail
        deq->tail = (deq->tail+deq->sizeMax+1)%(deq->sizeMax);
    }
    deq->members++;
}

void push_front(Deque_MyClass * deq, MyClass temp) {
    if (deq->members == (deq->sizeMax - 1)) {
        increaseSize(deq);
    }
    if (deq->members == 0) {
        //insert at head
        deq->arr[deq->head] = temp;
        //make tail one past head
        deq->tail = (deq->head+deq->sizeMax+1)%(deq->sizeMax);
    } else {
        //tail doesn't change
        deq->arr[(deq->head+deq->sizeMax-1)%(deq->sizeMax)] = temp;
        //subtract 1 from head
        deq->head = (deq->head+deq->sizeMax-1)%(deq->sizeMax);
    }
    deq->members++;
}

MyClass & front(Deque_MyClass * deq) {
    return deq->arr[deq->head];
}

MyClass & back(Deque_MyClass * deq) {
    return deq->arr[(deq->tail+deq->sizeMax-1)%(deq->sizeMax)];
}

MyClass & at(Deque_MyClass * deq, int i) {
    return deq->arr[(deq->head+deq->sizeMax+i)%(deq->sizeMax)];
}

void pop_front(Deque_MyClass * deq) {
    if (deq->members > 0) {
        deq->head = (deq->head+deq->sizeMax+1)%(deq->sizeMax);
        deq->members--;
    }
}

void pop_back(Deque_MyClass * deq) {
    if (deq->members > 0) {
        deq->tail = (deq->tail+deq->sizeMax-1)%(deq->sizeMax);
        deq->members--;
    }
}

Deque_MyClass_Iterator begin(Deque_MyClass * deq) {
    deq->startVal->aDeq->sizeMax = deq->sizeMax;
    deq->startVal->current = deq->head;
    deq->startVal->aDeq = deq;
    return *deq->startVal;
}

Deque_MyClass_Iterator end(Deque_MyClass * deq) {
    deq->endVal->aDeq->sizeMax = deq->sizeMax;
    deq->endVal->current = deq->tail;
    deq->endVal->aDeq = deq;
    return *deq->endVal;
}

void clear(Deque_MyClass * deq) {
    deq->members = 0;
    deq->head = 0;
    deq->tail = 0;
    deq->sizeMax = 0;
}

void dtor(Deque_MyClass * deq) {
    deq->clear(deq);
    free(deq->arr);
}

bool Deque_MyClass_Iterator_equal(Deque_MyClass_Iterator it, Deque_MyClass_Iterator val) {
    if (it.current == val.current) {
        return true;
    } else {
        return false;
    }
}

bool Deque_MyClass_equal(Deque_MyClass deq1, Deque_MyClass deq2) {
    if (deq1.compare != deq2.compare) {
        return false;
    }
    if (deq1.members != deq2.members) {
        return false;
    }
    
    Deque_MyClass_Iterator temp_it1 = deq1.begin(&deq1);
    Deque_MyClass_Iterator temp_it2 = deq2.begin(&deq2);
    
    for (int i = 0; i < deq1.members; i++) {
        if (deq1.compare(deq1.arr[temp_it1.current], deq2.arr[temp_it2.current]) == true) {
            return false;
        } else if (deq1.compare(deq2.arr[temp_it2.current], deq1.arr[temp_it1.current]) == true) {
            return false;
        }
        temp_it1.inc(&temp_it1);
        temp_it2.inc(&temp_it2);
    }
    return true;
}

void Deque_MyClass_ctor(Deque_MyClass * deq, bool (*f)(const MyClass&, const MyClass&)) {
    deq->members = 0;
    deq->head = 0;
    deq->tail = 0;
    strcpy(deq->type_name,"Deque_MyClass");
    //deq->arr = malloc(number_of_elements * sizeof(struct MyClass))
    deq->arr = (MyClass*) malloc(10 * sizeof(struct MyClass));
    deq->sizeMax = 10;

    deq->compare = f;
    
    deq->size = &size;
    
    deq->empty = &empty;
    
    deq->push_back = &push_back;
    deq->push_front = &push_front;
    
    deq->front = &front;
    deq->back = &back;
    deq->at = &at;
    deq->pop_front = &pop_front;
    deq->pop_back = &pop_back;
    deq->increaseSize = &increaseSize;

    deq->begin = &begin;
    deq->end = &end;
    deq->clear = &clear;
    deq->dtor = &dtor;
    
    //Iter init
    deq->it = new Deque_MyClass_Iterator;
    deq->it->current = 0;
    deq->it->aDeq = deq;
    deq->it->inc = &inc;
    deq->it->deref = &deref;
    deq->it->dec = &dec;
    
    deq->startVal = new Deque_MyClass_Iterator;
    deq->startVal->current = 0;
    deq->startVal->aDeq = deq;
    deq->startVal->inc = &inc;
    deq->startVal->deref = &deref;
    deq->startVal->dec = &dec;
    
    deq->endVal = new Deque_MyClass_Iterator;
    deq->endVal->current = 0;
    deq->endVal->aDeq = deq;
    deq->endVal->inc = &inc;
    deq->endVal->deref = &deref;
    deq->endVal->dec = &dec;
}

/*
 * Test for int.
 */

bool
int_less(const int &o1, const int &o2) {
    return o1 < o2;
}


Deque_DEFINE(int)

/*
struct Deque_int;
struct Deque_int_Iterator {
    int current;
    Deque_int * aDeq;
    
    void (*inc)(Deque_int_Iterator * it);
    int & (*deref)(Deque_int_Iterator * it);
    void (*dec)(Deque_int_Iterator * it);
};

struct Deque_int {
    int members;
    int head;
    int sizeMax;
    int tail;
    int* arr;
    char type_name[sizeof("Deque_int")];
    struct Deque_int_Iterator * it;
    bool (*compare)(const int&, const int&);
    int (*intCompare)(const void * o1, const void * o2, void * deq);
    void (*sort)(Deque_int * deq, Deque_int_Iterator begin, Deque_int_Iterator end);
    
    int (*size)(Deque_int * deq);
    bool (*empty)(Deque_int * deq);
    
    void (*push_back)(Deque_int * deq, int temp);
    void (*push_front)(Deque_int * deq, int temp);
    
    int & (*front)(Deque_int * deq);
    int & (*back)(Deque_int * deq);
    int & (*at)(Deque_int * deq, int i);
    void (*pop_front)(Deque_int * deq);
    void (*pop_back)(Deque_int * deq);
    void (*increaseSize)(Deque_int * deq);

    Deque_int_Iterator * startVal;    
    Deque_int_Iterator * endVal;
    Deque_int_Iterator (*begin)(Deque_int * deq);
    Deque_int_Iterator (*end)(Deque_int * deq);
    
    void (*clear)(Deque_int * deq);
    void (*dtor)(Deque_int * deq);
};

void inc(Deque_int_Iterator * it) {
    it->current = (it->current+it->aDeq->sizeMax+1)%(it->aDeq->sizeMax);
}

void dec(Deque_int_Iterator * it) {
    it->current = (it->current+it->aDeq->sizeMax-1)%(it->aDeq->sizeMax);
}

int & deref(Deque_int_Iterator * it) {
    return it->aDeq->arr[it->current];
}

int size(Deque_int * deq) {
    return deq->members;
}

bool empty(Deque_int * deq) {
    if (size(deq) == 0) {
        return true;
    } else {
        return false;
    }
}

void increaseSize(Deque_int * deq) {
    int oldSize = deq->sizeMax;
    deq->sizeMax = deq->sizeMax * 2;
    int * tempArr = (int*) malloc(deq->sizeMax * sizeof(int));
    int current = deq->head;
    for (int i = 0; i < oldSize; i++) {
        tempArr[i] = deq->arr[current];
        current = (current+oldSize+1)%oldSize;
    }
    free(deq->arr);
    deq->arr = tempArr;
}

void push_back(Deque_int * deq, int temp) {
    if (deq->members == (deq->sizeMax - 1)) {
        increaseSize(deq);
    }
    if (deq->members == 0) {
        //insert at head
        deq->arr[deq->head] = temp;
        //make tail one past head
        deq->tail = (deq->head+deq->sizeMax+1)%(deq->sizeMax);
    } else {
        //head doesn't change, get index before tail
        deq->arr[deq->tail] = temp;
        //add 1 to tail
        deq->tail = (deq->tail+deq->sizeMax+1)%(deq->sizeMax);
    }
    deq->members++;
}

void push_front(Deque_int * deq, int temp) {
    if (deq->members == (deq->sizeMax - 1)) {
        increaseSize(deq);
    }
    if (deq->members == 0) {
        //insert at head
        deq->arr[deq->head] = temp;
        //make tail one past head
        deq->tail = (deq->head+deq->sizeMax+1)%(deq->sizeMax);
    } else {
        //tail doesn't change
        deq->arr[(deq->head+deq->sizeMax-1)%(deq->sizeMax)] = temp;
        //subtract 1 from head
        deq->head = (deq->head+deq->sizeMax-1)%(deq->sizeMax);
    }
    deq->members++;
}

int & front(Deque_int * deq) {
    return deq->arr[deq->head];
}

int & back(Deque_int * deq) {
    return deq->arr[(deq->tail+deq->sizeMax-1)%(deq->sizeMax)];
}

int & at(Deque_int * deq, int i) {
    return deq->arr[(deq->head+deq->sizeMax+i)%(deq->sizeMax)];
}

void pop_front(Deque_int * deq) {
    if (deq->members > 0) {
        deq->head = (deq->head+deq->sizeMax+1)%(deq->sizeMax);
        deq->members--;
    }
}

void pop_back(Deque_int * deq) {
    if (deq->members > 0) {
        deq->tail = (deq->tail+deq->sizeMax-1)%(deq->sizeMax);
        deq->members--;
    }
}

Deque_int_Iterator begin(Deque_int * deq) {
    deq->startVal->aDeq->sizeMax = deq->sizeMax;
    deq->startVal->current = deq->head;
    deq->startVal->aDeq = deq;
    return *deq->startVal;
}

Deque_int_Iterator end(Deque_int * deq) {
    deq->endVal->aDeq->sizeMax = deq->sizeMax;
    deq->endVal->current = deq->tail;
    deq->endVal->aDeq = deq;
    return *deq->endVal;
}

void clear(Deque_int * deq) {
    deq->members = 0;
    deq->head = 0;
    deq->tail = 0;
    deq->sizeMax = 0;
}

void dtor(Deque_int * deq) {
    deq->clear(deq);
    free(deq->arr);
}

bool Deque_int_Iterator_equal(Deque_int_Iterator it, Deque_int_Iterator val) {
    if (it.current == val.current) {
        return true;
    } else {
        return false;
    }
}

bool Deque_int_equal(Deque_int deq1, Deque_int deq2) {
    if (deq1.compare != deq2.compare) {
        return false;
    }
    if (deq1.members != deq2.members) {
        return false;
    }
    
    Deque_int_Iterator temp_it1 = deq1.begin(&deq1);
    Deque_int_Iterator temp_it2 = deq2.begin(&deq2);
    
    for (int i = 0; i < deq1.members; i++) {
        if (deq1.compare(deq1.arr[temp_it1.current], deq2.arr[temp_it2.current]) == true) {
            return false;
        } else if (deq1.compare(deq2.arr[temp_it2.current], deq1.arr[temp_it1.current]) == true) {
            return false;
        }
        temp_it1.inc(&temp_it1);
        temp_it2.inc(&temp_it2);
    }
    return true;
}

int intCompare(const void * o1, const void  * o2, void * deq) {
    bool val1 = ((Deque_int*)deq)->compare(*(int *)o1, *(int *)o2);
    bool val2 = ((Deque_int*)deq)->compare(*(int *)o2, *(int *)o1);
    
    if (val1 && !val2) {
        return -1;
    } else if (!val1 && !val2) {
        return 0;
    } else {
        return 1;
    }
}

void sort(Deque_int * deq, Deque_int_Iterator begin, Deque_int_Iterator end) {
    qsort_r(deq->arr+begin.current, ((end.current)-(begin.current)), sizeof(int), (deq->intCompare), deq);
}

void Deque_int_ctor(Deque_int * deq, bool (*f)(const int&, const int&)) {
    deq->members = 0;
    deq->head = 0;
    deq->tail = 0;
    strcpy(deq->type_name,"Deque_int");
    //deq->arr = malloc(number_of_elements * sizeof(struct int))
    deq->arr = (int*) malloc(10 * sizeof(int));
    deq->sizeMax = 10;

    deq->compare = f;
    deq->intCompare = &intCompare;
    deq->sort = &sort;
    
    deq->size = &size;
    
    deq->empty = &empty;
    
    deq->push_back = &push_back;
    deq->push_front = &push_front;
    
    deq->front = &front;
    deq->back = &back;
    deq->at = &at;
    deq->pop_front = &pop_front;
    deq->pop_back = &pop_back;
    deq->increaseSize = &increaseSize;

    deq->begin = &begin;
    deq->end = &end;
    deq->clear = &clear;
    deq->dtor = &dtor;
    
    //Iter init
    deq->it = new Deque_int_Iterator;
    deq->it->current = 0;
    deq->it->aDeq = deq;
    deq->it->inc = &inc;
    deq->it->deref = &deref;
    deq->it->dec = &dec;
    
    deq->startVal = new Deque_int_Iterator;
    deq->startVal->current = 0;
    deq->startVal->aDeq = deq;
    deq->startVal->inc = &inc;
    deq->startVal->deref = &deref;
    deq->startVal->dec = &dec;
    
    deq->endVal = new Deque_int_Iterator;
    deq->endVal->current = 0;
    deq->endVal->aDeq = deq;
    deq->endVal->inc = &inc;
    deq->endVal->deref = &deref;
    deq->endVal->dec = &dec;
}*/

int
main() {
    FILE *devnull = fopen("/dev/null", "w");
    assert(devnull != 0);
    {
        Deque_MyClass deq;
        Deque_MyClass_ctor(&deq, MyClass_less_by_id);
        
        assert(deq.size(&deq) == 0);
        assert(deq.empty(&deq));
        
        //Should print "---- Deque_MyClass, 14".
        printf("---- %s, %d\n", deq.type_name, (int) sizeof(deq.type_name));
        //std::cout << "---- " << deq.type_name << ", " << sizeof(deq.type_name) << std::endl;
        assert(sizeof deq.type_name == 14);

        deq.push_back(&deq, MyClass{1, "Joe"});
        deq.push_back(&deq, MyClass{2, "Mary"});
        deq.push_back(&deq, MyClass{3, "Tom"});
        deq.push_front(&deq, MyClass{0, "Mike"});
        deq.push_front(&deq, MyClass{-1, "Mary"});

        MyClass_print(&deq.front(&deq));
        MyClass_print(&deq.back(&deq));
        assert(deq.front(&deq).id == -1);
        assert(deq.back(&deq).id == 3);
        
        deq.pop_front(&deq);
        deq.pop_back(&deq);
        assert(deq.front(&deq).id == 0);
        assert(deq.back(&deq).id == 2);

        assert(deq.size(&deq) == 3);

        for (Deque_MyClass_Iterator it = deq.begin(&deq);
         !Deque_MyClass_Iterator_equal(it, deq.end(&deq)); it.inc(&it)) {
            MyClass_print(&it.deref(&it));
        }

        // Multiple iterators?
        for (Deque_MyClass_Iterator it1 = deq.begin(&deq);
         !Deque_MyClass_Iterator_equal(it1, deq.end(&deq)); it1.inc(&it1)) {
            MyClass_print(&it1.deref(&it1));
            for (Deque_MyClass_Iterator it2 = deq.begin(&deq);
             !Deque_MyClass_Iterator_equal(it2, deq.end(&deq)); it2.inc(&it2)) {
                MyClass_print(&it2.deref(&it2));
                for (Deque_MyClass_Iterator it3 = deq.begin(&deq);
                 !Deque_MyClass_Iterator_equal(it3, deq.end(&deq)); it3.inc(&it3)) {
                    MyClass_print(&it3.deref(&it3));
                }
            }
        }

        // Test decrement of end.
        {
            auto it = deq.end(&deq);
            it.dec(&it);
            assert(it.deref(&it).id == 2);
        }

        // printf("Using at.\n");

        for (size_t i = 0; i < 3; i++) {
            MyClass_print(&deq.at(&deq, i));
        }

        // Test that front(), back(), at(), and deref() are returning a reference.
        // Change via at().
        assert(deq.at(&deq, 0).id == 0);
        deq.at(&deq, 0).id = 100;
        assert(deq.at(&deq, 0).id == 100);
        // Change via front().
        assert(deq.front(&deq).id == 100);
        deq.front(&deq).id = 0;
        assert(deq.front(&deq).id == 0);
        assert(deq.at(&deq, 0).id == 0); // Verify with at() also.
        {
            auto it = deq.end(&deq);
            it.dec(&it);
            assert(it.deref(&it).id == 2);
            it.deref(&it).id = 200;
            assert(it.deref(&it).id == 200);
            // Change using back().
            assert(deq.back(&deq).id == 200);
            deq.back(&deq).id = 2;
            assert(deq.back(&deq).id == 2);
            assert(it.deref(&it).id == 2); // Verify with iterator also.
        }

        deq.clear(&deq);

        deq.dtor(&deq);

        // Test equality.  Two deques compare equal if they are of the same
        // length and all the elements compare equal.  It is undefined behavior
        // if the two deques were constructed with different comparison
        // functions.
        {
            Deque_MyClass deq1, deq2;
            Deque_MyClass_ctor(&deq1, MyClass_less_by_id);
            Deque_MyClass_ctor(&deq2, MyClass_less_by_id);

            deq1.push_back(&deq1, MyClass{1, "Joe"});
            deq1.push_back(&deq1, MyClass{2, "Jane"});
            deq1.push_back(&deq1, MyClass{3, "Mary"});
            deq2.push_back(&deq2, MyClass{1, "Joe"});
            deq2.push_back(&deq2, MyClass{2, "Jane"});
            deq2.push_back(&deq2, MyClass{3, "Mary"});

            assert(Deque_MyClass_equal(deq1, deq2));

            deq1.pop_back(&deq1);
            assert(!Deque_MyClass_equal(deq1, deq2));
            deq1.push_back(&deq1, MyClass{4, "Mary"});
            assert(!Deque_MyClass_equal(deq1, deq2));

            deq1.dtor(&deq1);
            deq2.dtor(&deq2);
        }
    }

    // Test that it can handle other types.  Tests are the same, more or less.
    {
        Deque_int deq;
        Deque_int_ctor(&deq, int_less);

        assert(deq.size(&deq) == 0);
        assert(deq.empty(&deq));

        // Should print "---- Deque_int, 10".
        printf("---- %s, %d\n", deq.type_name, int(sizeof(deq.type_name)));
        // std::cout << "---- " << deq.type_name << ", " << sizeof(deq.type_name) << std::endl;
        assert(sizeof deq.type_name == 10);

        deq.push_back(&deq, 1);
        deq.push_back(&deq, 2);
        deq.push_back(&deq, 3);
        deq.push_front(&deq, 0);
        deq.push_front(&deq, -1);

        printf("%d\n", deq.front(&deq));
        printf("%d\n", deq.back(&deq));
        assert(deq.front(&deq) == -1);
        assert(deq.back(&deq) == 3);

        deq.pop_front(&deq);
        deq.pop_back(&deq);
        assert(deq.front(&deq) == 0);
        assert(deq.back(&deq) == 2);

        assert(deq.size(&deq) == 3);

        for (Deque_int_Iterator it = deq.begin(&deq);
         !Deque_int_Iterator_equal(it, deq.end(&deq)); it.inc(&it)) {
            printf("%d\n", it.deref(&it));
        }

        // Test decrement.
        {
            auto it = deq.end(&deq);
            it.dec(&it);
            assert(it.deref(&it) == 2);
        }

        printf("Using at.\n");

        for (size_t i = 0; i < 3; i++) {
            printf("%d: %d\n", int(i), deq.at(&deq, i));
        }

        deq.clear(&deq);

        deq.dtor(&deq);
    }

    // Test equality.  It is undefined behavior if the two deques were constructed with different
    // comparison functions.
    {
        Deque_int deq1, deq2;
        Deque_int_ctor(&deq1, int_less);
        Deque_int_ctor(&deq2, int_less);

        deq1.push_back(&deq1, 1);
        deq1.push_back(&deq1, 2);
        deq1.push_back(&deq1, 3);
        deq2.push_back(&deq2, 1);
        deq2.push_back(&deq2, 2);
        deq2.push_back(&deq2, 3);

        assert(Deque_int_equal(deq1, deq2));

        deq1.pop_back(&deq1);
        assert(!Deque_int_equal(deq1, deq2));
        deq1.push_back(&deq1, 4);
        assert(!Deque_int_equal(deq1, deq2));

        deq1.dtor(&deq1);
        deq2.dtor(&deq2);
    }

    // Test performance.
    {
        std::default_random_engine e;
        using rand = std::uniform_int_distribution<int>;
        std::uniform_real_distribution<float> action;
        Deque_int deq;
        Deque_int_ctor(&deq, int_less);

        for (int i = 0; i < 1000000; i++) {
            deq.push_back(&deq, i);
        }

        // In one end, out the other.
        for (int i = 0; i < 20000000; i++) {
            // fprintf(stderr, "iter: %d\n", i);
            deq.push_back(&deq, i);
            deq.pop_front(&deq);
        }
        for (int i = 0; i < 20000000; i++) {
            deq.push_front(&deq, i);
            deq.pop_back(&deq);
        }

        // To do some computation, to prevent compiler from optimizing out.
        size_t sum = 0;
        // Random.
        int pf = 0, pb = 0, pof = 0, pob = 0;
        for (int i = 0; i < 10000000; i++) {
            if (action(e) > .51) {
                if (rand(0, 1)(e) == 0) {
                    deq.push_back(&deq, i);
                    pf++;
                } else {
                    deq.push_front(&deq, i);
                    pb++;
                }
            } else {
                if (deq.size(&deq) > 0) {
                    if (rand(0, 1)(e) == 0) {
                        deq.pop_back(&deq);
                        pob++;
                    } else {
                        deq.pop_front(&deq);
                        pof++;
                    }
                }
            }
            // Randomly access one element.
            sum += deq.at(&deq, rand(0, deq.size(&deq) - 1)(e));
        }
        // Print it out to prevent optimizer from optimizing out the at() calls.
        fprintf(devnull, "%lu", sum);
        printf("%d push_backs, %d push_fronts, %d pop_backs, %d pop_fronts, %d size\n", pb, pf, pob, pof, (int) deq.size(&deq));
        deq.dtor(&deq);
    }

    // Test random access performance
    {
       size_t sum = 0;
       int lo = 0, hi = 10000000;
       Deque_int deq;
       Deque_int_ctor(&deq, int_less);

       for(int i = lo; i < hi; i++) {
          deq.push_back(&deq, i);
       }

       for(int i = lo; i < hi; i++) {
          sum += deq.at(&deq, i);
       }
       printf("Sum of all integers between %d and %d calculated using a deque is %lu.\n", lo, hi, sum);
       deq.dtor(&deq);
    }

    // Test sort.
    // You must be able to work with the correct less-than function.
    {
        Deque_int deq;
        Deque_int_ctor(&deq, int_less);
        std::default_random_engine e;
        using rand = std::uniform_int_distribution<int>;

        for (int i = 0; i < 10000; i++) {
            deq.push_back(&deq, rand(-1000000, 1000000)(e));
        }
        
        deq.sort(&deq, deq.begin(&deq), deq.end(&deq));
 
        deq.dtor(&deq);
    }

    // Sorting Test 2
    {
       Deque_int deq1;
       Deque_int_ctor(&deq1, int_less);

       for (int i=0;i<10000;i++) {
           deq1.push_back(&deq1, i);
       }

       for (int i=20000;i>=10000;i--) {
           deq1.push_back(&deq1,i);
       }

       deq1.push_back(&deq1,20001);

       auto iter1 =  deq1.end(&deq1);
       iter1.dec(&iter1);	
       
       auto iter2 = deq1.begin(&deq1);
      
       for (int i=0;i<10000;i++) {
           iter2.inc(&iter2);
       }

       deq1.sort(&deq1, iter2,iter1);

       Deque_int deq2;
       Deque_int_ctor(&deq2 , int_less);

       for(int i=0;i<=20001;i++) {
           deq2.push_back(&deq2,i);
       }

      deq1.sort(&deq1, iter2,iter1);
      assert(Deque_int_equal(deq1, deq2)); 

      deq1.dtor(&deq1);
      deq2.dtor(&deq2);
    }

    // Performance testing for sorting
    {
      
       Deque_int deq1;
       Deque_int_ctor(&deq1, int_less);

        std::default_random_engine e;
        using rand = std::uniform_int_distribution<int>;

        for (int i = 0; i < 1000000; i++) {
            deq1.push_back(&deq1, rand(-1000000, 1000000)(e));
        }

       auto iter1 = deq1.begin(&deq1);
       auto iter2 = deq1.begin(&deq1);

       for(int i=0;i<10;i++)
           iter1.inc(&iter1);

       for(int i=0;i<20;i++)
           iter2.inc(&iter2);

       for(int i=0;i<1000000;i++)
       	   deq1.sort(&deq1, iter1,iter2);

       deq1.dtor(&deq1);

    }

   // Print allocation info
   printf("%ld allocations totalling %ld bytes\n", alloc_call_count, total_bytes_allocated);
   int rv = fclose(devnull);
   assert(rv == 0);
   
}