//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_CIRCULARQUEUE_H
#define RISC_V_SIMULATOR_CIRCULARQUEUE_H

#include <cstdio>
#include <iostream>
#include "Utils.h"

template<class T, DataUnit length>
class CircularQueue {
private:
    T data[length]{};
    DataUnit head, tail;
public:
    CircularQueue() : head(0), tail(0) {}

    CircularQueue(const CircularQueue<T, length> &other) {
        for (int i = 0; i < length; i++) data[i] = other.data[i];
        head = other.head, tail = other.tail;
    }

    CircularQueue(const T other[]) {
        for (int i = 0; i < length; i++) data[i] = other[i];
        head = 0, tail = length;
    }

    CircularQueue &operator=(const CircularQueue<T, length> &other) {
        if (this == &other) return (*this);
        for (int i = 0; i < length; i++) data[i] = other.data[i];
        head = other.head, tail = other.tail;
        return (*this);
    }

    DataUnit nex(DataUnit pos) const {
        return (pos + 1) % length;
    }

    bool empty() const {
        return head == tail;
    }

    bool full() const {
        return nex(tail) == head;
    }

    void push(const T &val) {
        data[tail] = val;
        tail = nex(tail);
    }

    void pop() {
        head = nex(head);
    }

    void clear() {
        head = tail = 0;
    }

    T &operator[](DataUnit pos) {
        return data[pos];
    }

    ~CircularQueue() = default;
};

#endif //RISC_V_SIMULATOR_CIRCULARQUEUE_H
