#include "msfsm.hpp"
#include <iostream>

using namespace msfsm;
using namespace std;

class Elevator : protected Fsm<Elevator> {
public:
    Elevator() {
        transition(ground, Request(1));
    }

    void moveTo(int floor) {
        cout << "Moving to floor " << floor << endl;
        handle(Request(floor));
    }

private:
    friend Fsm;

    // Events
    struct Request {
        const int floor;
        Request(int f) : floor(f) {}
    };

    class State : public Fsm::State, public Named<State> {
        friend Fsm;
        using Fsm::State::State;
        void entry(Request r) {
            cout << "Entering " << name() << endl;
            event(r);
        }
        virtual void event(Request) = 0;
    };

    class Roof : public State {
        friend Fsm;
        using State::State;
        void event(Request r) override {
            if (r.floor < 3)
                transition(fsm.middle, r);
        }
    } roof {this};

    class Middle : public State {
        friend Fsm;
        using State::State;
        void event(Request r) override {
            if (r.floor < 2)
                transition(fsm.ground, r);
            if (r.floor > 2)
                transition(fsm.roof, r);
        }
    } middle {this};

    class Ground : public State {
        friend Fsm;
        using State::State;
        void event(Request r) override {
            if (r.floor > 1)
                transition(fsm.middle, r);
        }
    } ground {this};
};

int main(int argc, char *argv[])
{
    Elevator e;
    e.moveTo(3);
    e.moveTo(1);
    e.moveTo(2);
    return 0;
}
