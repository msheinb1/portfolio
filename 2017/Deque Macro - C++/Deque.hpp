#include <stdio.h>
#include <string.h>

#ifndef Deque_DEFINE
#define Deque_DEFINE(t)                                                             \
struct Deque_##t;                                                                   \
struct Deque_##t##_Iterator {                                                       \
    int current;                                                                    \
    Deque_##t * aDeq;                                                               \
                                                                                    \
    void (*inc)(Deque_##t##_Iterator * it);                                         \
    t & (*deref)(Deque_##t##_Iterator * it);                                        \
    void (*dec)(Deque_##t##_Iterator * it);                                         \
};                                                                                  \
                                                                                    \
struct Deque_##t {                                                                  \
    int members;                                                                    \
    int head;                                                                       \
    int sizeMax;                                                                    \
    int tail;                                                                       \
    t* arr;                                                                         \
    int sizeFac;                                                                    \
    char type_name[sizeof("Deque_" #t)];                                            \
    struct Deque_##t##_Iterator * it;                                               \
    bool (*compare)(const t&, const t&);                                            \
    int (*intCompare_##t)(const void * o1, const void * o2, void * deq);                \
    void (*sort)(Deque_##t * deq, Deque_##t##_Iterator begin, Deque_##t##_Iterator end);\
                                                                                    \
    int (*size)(Deque_##t * deq);                                                   \
    bool (*empty)(Deque_##t * deq);                                                 \
                                                                                    \
    void (*push_back)(Deque_##t * deq, t temp);                                     \
    void (*push_front)(Deque_##t * deq, t temp);                                    \
                                                                                    \
    t & (*front)(Deque_##t * deq);                                                  \
    t & (*back)(Deque_##t * deq);                                                   \
    t & (*at)(Deque_##t * deq, int i);                                              \
    void (*pop_front)(Deque_##t * deq);                                             \
    void (*pop_back)(Deque_##t * deq);                                              \
    void (*increaseSize)(Deque_##t * deq);                                          \
                                                                                    \
    Deque_##t##_Iterator * startVal;                                                \
    Deque_##t##_Iterator * endVal;                                                  \
    Deque_##t##_Iterator (*begin)(Deque_##t * deq);                                 \
    Deque_##t##_Iterator (*end)(Deque_##t * deq);                                   \
                                                                                    \
    void (*clear)(Deque_##t * deq);                                                 \
    void (*dtor)(Deque_##t * deq);                                                  \
};                                                                                  \
                                                                                    \
void inc(Deque_##t##_Iterator * it) {                                               \
    it->current = (it->current+it->aDeq->sizeMax+1)%(it->aDeq->sizeMax);            \
}                                                                                   \
                                                                                    \
void dec(Deque_##t##_Iterator * it) {                                               \
    it->current = (it->current+it->aDeq->sizeMax-1)%(it->aDeq->sizeMax);            \
}                                                                                   \
                                                                                    \
t & deref(Deque_##t##_Iterator * it) {                                              \
    return it->aDeq->arr[it->current];                                              \
}                                                                                   \
                                                                                    \
int size(Deque_##t * deq) {                                                         \
    return deq->members;                                                            \
}                                                                                   \
                                                                                    \
bool empty(Deque_##t * deq) {                                                       \
    if (size(deq) == 0) {                                                           \
        return true;                                                                \
    } else {                                                                        \
        return false;                                                               \
    }                                                                               \
}                                                                                   \
                                                                                    \
void increaseSize(Deque_##t * deq) {                                                \
    int oldSize = deq->sizeMax;                                                     \
    deq->sizeMax = deq->sizeMax * deq->sizeFac;                                     \
    deq->sizeFac = deq->sizeFac * 2;                                                \
    t * tempArr = (t*) malloc(deq->sizeMax * sizeof(t));                            \
    int current = deq->head;                                                        \
    for (int i = 0; i < oldSize; i++) {                                             \
        tempArr[i] = deq->arr[current];                                             \
        current = (current+oldSize+1)%oldSize;                                      \
    }                                                                               \
    free(deq->arr);                                                                 \
    deq->arr = tempArr;                                                             \
}                                                                                   \
                                                                                    \
void push_back(Deque_##t * deq, t temp) {                                           \
    if (deq->members == (deq->sizeMax - 1)) {                                       \
        increaseSize(deq);                                                          \
    }                                                                               \
    if (deq->members == 0) {                                                        \
        deq->arr[deq->head] = temp;                                                 \
        deq->tail = (deq->head+deq->sizeMax+1)%(deq->sizeMax);                      \
    } else {                                                                        \
        deq->arr[deq->tail] = temp;                                                 \
        deq->tail = (deq->tail+deq->sizeMax+1)%(deq->sizeMax);                      \
    }                                                                               \
    deq->members++;                                                                 \
}                                                                                   \
                                                                                    \
void push_front(Deque_##t * deq, t temp) {                                          \
    if (deq->members == (deq->sizeMax - 1)) {                                       \
        increaseSize(deq);                                                          \
    }                                                                               \
    if (deq->members == 0) {                                                        \
        deq->arr[deq->head] = temp;                                                 \
        deq->tail = (deq->head+deq->sizeMax+1)%(deq->sizeMax);                      \
    } else {                                                                        \
        deq->arr[(deq->head+deq->sizeMax-1)%(deq->sizeMax)] = temp;                 \
        deq->head = (deq->head+deq->sizeMax-1)%(deq->sizeMax);                      \
    }                                                                               \
    deq->members++;                                                                 \
}                                                                                   \
                                                                                    \
t & front(Deque_##t * deq) {                                                        \
    return deq->arr[deq->head];                                                     \
}                                                                                   \
                                                                                    \
t & back(Deque_##t * deq) {                                                         \
    return deq->arr[(deq->tail+deq->sizeMax-1)%(deq->sizeMax)];                     \
}                                                                                   \
                                                                                    \
t & at(Deque_##t * deq, int i) {                                                    \
    return deq->arr[(deq->head+deq->sizeMax+i)%(deq->sizeMax)];                     \
}                                                                                   \
                                                                                    \
void pop_front(Deque_##t * deq) {                                                   \
    if (deq->members > 0) {                                                         \
        deq->head = (deq->head+deq->sizeMax+1)%(deq->sizeMax);                      \
        deq->members--;                                                             \
    }                                                                               \
}                                                                                   \
                                                                                    \
void pop_back(Deque_##t * deq) {                                                    \
    if (deq->members > 0) {                                                         \
        deq->tail = (deq->tail+deq->sizeMax-1)%(deq->sizeMax);                      \
        deq->members--;                                                             \
    }                                                                               \
}                                                                                   \
                                                                                    \
Deque_##t##_Iterator begin(Deque_##t * deq) {                                       \
    deq->startVal->aDeq->sizeMax = deq->sizeMax;                                    \
    deq->startVal->current = deq->head;                                             \
    deq->startVal->aDeq = deq;                                                      \
    return *deq->startVal;                                                          \
}                                                                                   \
                                                                                    \
Deque_##t##_Iterator end(Deque_##t * deq) {                                         \
    deq->endVal->aDeq->sizeMax = deq->sizeMax;                                      \
    deq->endVal->current = deq->tail;                                               \
    deq->endVal->aDeq = deq;                                                        \
    return *deq->endVal;                                                            \
}                                                                                   \
                                                                                    \
void clear(Deque_##t * deq) {                                                       \
    deq->members = 0;                                                               \
    deq->head = 0;                                                                  \
    deq->tail = 0;                                                                  \
    deq->sizeMax = 0;                                                               \
}                                                                                   \
                                                                                    \
void dtor(Deque_##t * deq) {                                                         \
    deq->clear(deq);                                                                \
    free(deq->arr);                                                                 \
}                                                                                   \
                                                                                    \
bool Deque_##t##_Iterator_equal(Deque_##t##_Iterator it, Deque_##t##_Iterator val) {\
    if (it.current == val.current) {                                                \
        return true;                                                                \
    } else {                                                                        \
        return false;                                                               \
    }                                                                               \
}                                                                                   \
                                                                                    \
bool Deque_##t##_equal(Deque_##t deq1, Deque_##t deq2) {                            \
    if (deq1.compare != deq2.compare) {                                             \
        return false;                                                               \
    }                                                                               \
    if (deq1.members != deq2.members) {                                             \
        return false;                                                               \
    }                                                                               \
                                                                                    \
    Deque_##t##_Iterator temp_it1 = deq1.begin(&deq1);                              \
    Deque_##t##_Iterator temp_it2 = deq2.begin(&deq2);                              \
                                                                                    \
    for (int i = 0; i < deq1.members; i++) {                                        \
        if (deq1.compare(deq1.arr[temp_it1.current], deq2.arr[temp_it2.current]) == true) {\
            return false;                                                           \
        } else if (deq1.compare(deq2.arr[temp_it2.current], deq1.arr[temp_it1.current]) == true) {\
            return false;                                                           \
        }                                                                           \
        temp_it1.inc(&temp_it1);                                                    \
        temp_it2.inc(&temp_it2);                                                    \
    }                                                                               \
    return true;                                                                    \
}                                                                                   \
                                                                                    \
int intCompare_##t(const void * o1, const void  * o2, void * deq) {                     \
    bool val1 = ((Deque_##t*)deq)->compare(*(t *)o1, *(t *)o2);                     \
    bool val2 = ((Deque_##t*)deq)->compare(*(t *)o2, *(t *)o1);                     \
                                                                                    \
    if (val1 && !val2) {                                                            \
        return -1;                                                                  \
    } else if (!val1 && !val2) {                                                    \
        return 0;                                                                   \
    } else {                                                                        \
        return 1;                                                                   \
    }                                                                               \
}                                                                                   \
                                                                                    \
void sort(Deque_##t * deq, Deque_##t##_Iterator begin, Deque_##t##_Iterator end) {  \
    qsort_r(deq->arr+begin.current, ((end.current)-(begin.current)), sizeof(t), (deq->intCompare_##t), deq);\
}                                                                                   \
                                                                                    \
void Deque_##t##_ctor(Deque_##t * deq, bool (*f)(const t&, const t&)) {             \
    deq->members = 0;                                                               \
    deq->head = 0;                                                                  \
    deq->tail = 0;                                                                  \
    strcpy(deq->type_name,"Deque_" #t);                                             \
    deq->arr = (t*) malloc(10 * sizeof(t));                                         \
    deq->sizeMax = 10;                                                              \
    deq->sizeFac = 2;                                                               \
                                                                                    \
    deq->compare = f;                                                               \
    deq->intCompare_##t = &intCompare_##t;                                          \
    deq->sort = &sort;                                                              \
                                                                                    \
    deq->size = &size;                                                              \
                                                                                    \
    deq->empty = &empty;                                                            \
                                                                                    \
    deq->push_back = &push_back;                                                    \
    deq->push_front = &push_front;                                                  \
                                                                                    \
    deq->front = &front;                                                            \
    deq->back = &back;                                                              \
    deq->at = &at;                                                                  \
    deq->pop_front = &pop_front;                                                    \
    deq->pop_back = &pop_back;                                                      \
    deq->increaseSize = &increaseSize;                                              \
                                                                                    \
    deq->begin = &begin;                                                            \
    deq->end = &end;                                                                \
    deq->clear = &clear;                                                            \
    deq->dtor = &dtor;                                                              \
                                                                                    \
    deq->it = new Deque_##t##_Iterator;                                             \
    deq->it->current = 0;                                                           \
    deq->it->aDeq = deq;                                                            \
    deq->it->inc = &inc;                                                            \
    deq->it->deref = &deref;                                                        \
    deq->it->dec = &dec;                                                            \
                                                                                    \
    deq->startVal = new Deque_##t##_Iterator;                                       \
    deq->startVal->current = 0;                                                     \
    deq->startVal->aDeq = deq;                                                      \
    deq->startVal->inc = &inc;                                                      \
    deq->startVal->deref = &deref;                                                  \
    deq->startVal->dec = &dec;                                                      \
                                                                                    \
    deq->endVal = new Deque_##t##_Iterator;                                         \
    deq->endVal->current = 0;                                                       \
    deq->endVal->aDeq = deq;                                                        \
    deq->endVal->inc = &inc;                                                        \
    deq->endVal->deref = &deref;                                                    \
    deq->endVal->dec = &dec;                                                        \
}
#endif