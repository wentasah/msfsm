#include "msfsm.hpp"
#include <iostream>

using namespace msfsm;
using namespace std;

// Events
class Up {};
class Down {};

class UpDown : public Fsm<UpDown> {
public:
    UpDown();
    ~UpDown();

private:
    friend Fsm;

    class State;

    // States = pointers to incomplete class definitions
    class Upper; Upper *upper;
    class Lower; Lower *lower;
};

class UpDown::State : public Fsm::State {
    friend Fsm;
    using Fsm::State::State;
    virtual void event(Up)   { cout << "Ignoring Up event" << endl; }
    virtual void event(Down) { cout << "Ignoring Down event" << endl; }
};

class UpDown::Upper : public UpDown::State {
    friend Fsm;
    using State::State;
    void entry() {
        cout << "Entering upper" << endl;
    }
    void event(Down) override {
        transition(*fsm.lower);
    }
};

class UpDown::Lower : public UpDown::State {
    friend Fsm;
    using State::State;
    void entry() {
        cout << "Entering lower" << endl;
    }
    void event(Up) override {
        transition(*fsm.upper);
    }
    void exit() override {
        cout << "Exiting lower" << endl;
    }
};


UpDown::UpDown()
    : upper(new Upper(this))
    , lower(new Lower(this))
{
    transition(*lower);
}

UpDown::~UpDown()
{
    delete(lower);
    delete(upper);
}

int main(int argc, char *argv[])
{
    UpDown f;
    f.handle(Up());
    f.handle(Up());
    f.handle(Down());
    f.handle(Down());
    return 0;
}
