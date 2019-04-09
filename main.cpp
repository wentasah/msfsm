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

    Semaphore() : Fsm(red) {
        transition(red, 1);
    }

    // Semaphore-specific state base class
    class State : public Fsm::NamedState {
        friend Fsm;
        using Fsm::NamedState::NamedState;
        void entry() { cout << __PRETTY_FUNCTION__ << endl; }
        void exit()  override { cout << __PRETTY_FUNCTION__ << endl; }
        void event(Event) override {}
        virtual void event(Up e) {}
        virtual void event(Down e) {}
    };

private:
    class Red: public State {
        friend Fsm;
        using State::State;
        void entry(int x) {}
        void event(Event e) override { if (e == down) transition(fsm.yellow); }
        void event(Down) override { transition(fsm.yellow); }
        void exit()  override {}
    } red {this};

    class Yellow: public State {
        using State::State;
        void exit()  override {}
        void event(Event e) override { if (e == up) transition(fsm.red, 2); }
        void event(Up) override { transition(fsm.red, 3); }
        void event(Down) override { transition(fsm.green); }
    } yellow {this};

    class Green: public State {
        friend Fsm;
        using State::State;
        void entry() { cout << __PRETTY_FUNCTION__ << endl; }
        void exit()  override { cout << __PRETTY_FUNCTION__ << endl; }
        void event(Event) override { cout << "Green event" << endl; }
        void event(Up) override { transition(fsm.yellow); }
    } green {this};

    void onTransition(Fsm::State &nextState) override {
#if 0
        cout << "Transition: "
             << boost::typeindex::type_id_runtime(*state()).pretty_name() << " -> "
             << boost::typeindex::type_id_runtime(nextState).pretty_name() << endl;
#else
        cout << "Transition: "
             << static_cast<State&>(state()).name() << " -> "
             << static_cast<State&>(nextState).name() << endl;
#endif
    }

};

int main()
{
    Semaphore sem;
    sem.handle(Semaphore::down);
    sem.handle();
    sem.handle(Semaphore::Down());
    sem.handle(Semaphore::Down());
    sem.handle(Semaphore::Down());
    sem.handle(Semaphore::Up());
    sem.handle(Semaphore::Up());
    sem.handle(Semaphore::Up());
    return 0;
}




