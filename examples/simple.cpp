#include "msfsm.hpp"
#include <iostream>

using namespace msfsm;
using namespace std;

class Simple : public Fsm<Simple> {
public:
    Simple() {
        transition(off);
    }

    using Fsm::transition;  // Make transition public

    class Off : public State {
        friend Fsm;
        using State::State;
        void entry() {
            cout << "Entering off" << endl;
        }
        void exit() override {
            cout << "Exiting off" << endl;
        }
    } off {this};

    class On : public State {
        friend Fsm;
        using State::State;
        void entry() {
            cout << "Entering on" << endl;
        }
        void exit() override {
            cout << "Exiting on" << endl;
        }
    } on {this};
};

int main(int argc, char *argv[])
{
    Simple s;
    s.transition(s.on);
    return 0;
}
