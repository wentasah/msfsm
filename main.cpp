 #include <iostream>
#include "msfsm.hpp"
#include <boost/type_index.hpp>
using namespace std;
using namespace msfsm;

class Semaphore: public Fsm<Semaphore>
{
public:
    // Events
    class Up {};
    class Down {};

    enum Event { up, down };

    Semaphore() {
        transition(red, 1);
    }

private:
    friend Fsm;

    // Semaphore-specific state base class
    class State : public Fsm::State, public Named<State> {
        friend Fsm;
        using Fsm::State::State;
        void entry() { cout << "Entering: " << name() << endl; }
        void exit()  override { cout << __PRETTY_FUNCTION__ << endl; }
        virtual void event(Event) {}
        virtual void event(Up e) {}
        virtual void event(Down e) {}
    };

    class Red : public State {
        friend Fsm;
        using State::State;
        void entry(int x) {}
        void event(Event e) override { if (e == down) transition(fsm.yellow); }
        void event(Down) override { transition(fsm.yellow); }
        void exit()  override {}
    } red {this};

    class Yellow : public State {
        using State::State;
        void exit()  override {}
        void event(Event e) override { if (e == up) transition(fsm.red, 2); }
        void event(Up) override { transition(fsm.red, 3); }
        void event(Down) override { transition(fsm.green); }
    } yellow {this};

    class Green : public State {
        friend Fsm;
        using State::State;
        void entry() { cout << __PRETTY_FUNCTION__ << endl; }
        void exit()  override { cout << __PRETTY_FUNCTION__ << endl; }
        void event(Event) override { cout << "Green event" << endl; }
        void event(Up) override { transition(fsm.yellow); }
    } green {this};

    void onTransition(State &nextState) {
        cout << "Transition: "
             << (state() ? static_cast<State*>(state())->name() : "--") << " -> "
             << nextState.name() << endl;
    }

};

int main()
{
    Semaphore sem;
    sem.handle(Semaphore::down);
    sem.handle(Semaphore::Down());
    sem.handle(Semaphore::Down());
    sem.handle(Semaphore::Down());
    sem.handle(Semaphore::Up());
    sem.handle(Semaphore::Up());
    sem.handle(Semaphore::Up());
    return 0;
}




