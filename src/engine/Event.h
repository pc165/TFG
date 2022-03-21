#ifndef TFG_EVENT_H
#define TFG_EVENT_H

#include <functional>
#include <iostream>


class Event {
    void f(int n1) {
        std::cout << n1 << '\n';
    }

    void g(Event *n1) {
        using namespace std::placeholders;  // for _1, _2, _3...
        int n = 1;
        auto f1 = [this, n]() { return this->f(n); };
        f1();
    }
};


#endif //TFG_EVENT_H
