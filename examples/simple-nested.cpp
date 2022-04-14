#include "msfsm.hpp"
#include <iostream>

using namespace msfsm;
using namespace std;

class SimpleNested : public msfsm::Fsm<SimpleNested>
{
public:
    SimpleNested() { transition(outerState1); }

    // Events
    struct ToggleOuter {};
    struct ToggleInner {};

private:
    friend Fsm;

    // Base class for outer states
    class State : public Fsm::State {
        friend Fsm;
        using Fsm::State::State;
        virtual void event(ToggleOuter) = 0;
        virtual void event(ToggleInner) = 0;
    };

    // This is both a state of SimpleNested FSM and a nested FSM with two inner states
    class OuterState1 : public State, public Fsm<OuterState1> {
        friend Fsm<SimpleNested>;
        friend Fsm<OuterState1>;
    public:
        using SimpleNested::State::State;
    private:
        void entry() { cout << "OuterState1" << endl; Fsm<OuterState1>::transition(innerState11); }
        void event(ToggleOuter) override { fsm.transition(fsm.outerState2); }
        void event(ToggleInner e) override { handle(e); }
        void exit() override { destroy(); } // destroy inner FSM

        // Base class of inner states
        class State : public Fsm<OuterState1>::State {
            friend Fsm;
            using Fsm::State::State;
            virtual void event(ToggleInner e) = 0;
        };

        class InnerState11 : public State {
            friend Fsm;
            using State::State;

            void entry() { cout << "InnerState11" << endl; };
            void event(ToggleInner e) override { transition(fsm.innerState12); }
        } innerState11 {this};

        class InnerState12 : public State {
            friend Fsm;
            using State::State;
            void entry() { cout << "InnerState12" << endl; }
            void event(ToggleInner e) override { transition(fsm.innerState11); }
        } innerState12{this};

    } outerState1 {this};

    class OuterState2 : public State, public Fsm<OuterState2> {
        friend Fsm<SimpleNested>;
        friend Fsm<OuterState2>;
    public:
        using SimpleNested::State::State;
    private:
        void entry() { cout << "OuterState2" << endl; Fsm<OuterState2>::transition(innerState22); }
        void event(ToggleOuter) override { fsm.transition(fsm.outerState1); }
        void event(ToggleInner e) override { this->handle(e); }
        void exit() override { destroy(); } // destroy inner FSM

        // Base class of inner states
        class State : public Fsm<OuterState2>::State {
            friend Fsm;
            using Fsm::State::State;
            virtual void event(ToggleInner e) = 0;
        };

        class InnerState21 : public State {
            friend Fsm;
            using State::State;
            void entry() { cout << "InnerState21" << endl; }
            void event(ToggleInner e) override { transition(fsm.innerState22); }
        } innerState21 {this};

        class InnerState22 : public State {
            friend Fsm;
            using State::State;
            void entry() { cout << "InnerState22" << endl; }
            void event(ToggleInner e) override { transition(fsm.innerState21); }
        } innerState22{this};

    } outerState2 {this};

};

int main(int argc, char *argv[])
{
    SimpleNested fsm;
    fsm.handle(SimpleNested::ToggleInner());
    fsm.handle(SimpleNested::ToggleInner());
    fsm.handle(SimpleNested::ToggleInner());

    fsm.handle(SimpleNested::ToggleOuter());

    fsm.handle(SimpleNested::ToggleInner());
    fsm.handle(SimpleNested::ToggleInner());

    fsm.handle(SimpleNested::ToggleOuter());
    return 0;
}
