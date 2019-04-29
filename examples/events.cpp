#include "msfsm.hpp"
#include <iostream>

using namespace msfsm;
using namespace std;

// Events
class Up {};
class Down {};

class Elevator : public Fsm<Elevator> {
public:
    Elevator() {
        transition(lower);
    }

private:
    friend Fsm;

    class State : public Fsm::State {
        friend Fsm;
        using Fsm::State::State;
        virtual void event(Up)   { cout << "Ignoring Up event" << endl; }
        virtual void event(Down) { cout << "Ignoring Down event" << endl; }
    };

    class Upper : public State {
        friend Fsm;
        using State::State;
        void entry() {
            cout << "Entering upper" << endl;
        }
        void event(Down) override {
            transition(fsm.lower);
        }
    } upper {this};

    class Lower : public State {
        friend Fsm;
        using State::State;
        void entry() {
            cout << "Entering lower" << endl;
        }
        void event(Up) override {
            transition(fsm.upper);
        }
        void exit() override {
            cout << "Exiting lower" << endl;
        }
    } lower {this};
};

int main(int argc, char *argv[])
{
    Elevator f;
    f.handle(Up());
    f.handle(Up());
    f.handle(Down());
    f.handle(Down());
    return 0;
}
