#include "msfsm.hpp"
#include <iostream>

using namespace msfsm;
using namespace std;

// Events
struct Request {
    const int floor;
    Request(int f) : floor(f) {}
};

class Elevator : public Fsm<Elevator> {
public:
    Elevator() {
        transition(ground);
    }

private:
    friend Fsm;

    class State : public Fsm::State {
        friend Fsm;
        using Fsm::State::State;
        virtual void event(Request) = 0;
    };

    class Roof : public State {
        friend Fsm;
        using State::State;
        void entry() {
            cout << "Entering roof" << endl;
        }
        void event(Request r) override {
            if (r.floor < 3)
                transition(fsm.middle, r);
        }
    } roof {this};

    class Middle : public State {
        friend Fsm;
        using State::State;
        void entry(Request r) {
            cout << "Entering middle" << endl;
            event(r);
        }
        void event(Request r) override {
            if (r.floor < 2)
                transition(fsm.ground);
            if (r.floor > 2)
                transition(fsm.roof);
        }
    } middle {this};

    class Ground : public State {
        friend Fsm;
        using State::State;
        void entry() {
            cout << "Entering ground" << endl;
        }
        void event(Request r) override {
            if (r.floor > 1)
                transition(fsm.middle, r);
        }
    } ground {this};
};

int main(int argc, char *argv[])
{
    Elevator e;
    e.handle(Request(3));
    e.handle(Request(1));
    e.handle(Request(2));
    return 0;
}
