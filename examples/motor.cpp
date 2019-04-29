#include "msfsm.hpp"
#include <iostream>

using namespace msfsm;
using namespace std;

class Motor : protected msfsm::Fsm<Motor>
{
public:
    Motor() {
        transition(homing);
    }
    void home() { handle(Home()); }
    void moveTo(int t) { handle(Goto(t)); }
    void periodic() {
        handle(Periodic());
        position += speed;
        cout << position << endl;
    }

    bool isMoving() const { return state() != &idle; }

private:
    friend Fsm;

    // Events
    struct Periodic {};
    struct Home {};
    struct Goto {
        const int position;
        Goto(int pos) : position(pos) {}
    };

    int position = 3;
    int speed = 0;

    class State : public Fsm::State, public Named<State> {
        friend Fsm;
        using Fsm::State::State;
        virtual void event(Periodic) = 0;
        virtual void event(Goto g) { /* ignore when not explicitly handled */ };
        virtual void event(Home)  { /* ignore when not explicitly handled */ };
    };

    void onTransition(State& nextState) {
        cout << nextState.name() << endl;
    }

    class Homing : public State, public Fsm<Homing> {
        friend Fsm<Motor>;
        friend Fsm<Homing>;
    public:
        using Motor::State::State;
    private:
        void entry() {
            Fsm<Homing>::transition(moveLeft);
        }
        void event(Periodic p) override {
            handle(p);
        }
        void exit() override {
            destroy();
        }

        // Homing base state
        class State : public Fsm<Homing>::State, public Named<State> {
            friend Fsm;
            using Fsm::State::State;
            virtual void event(Periodic) = 0;
        };

        void onTransition(State& nextState) {
            cout << nextState.name() << endl;
        }

        class MoveLeft : public State {
            friend Fsm;
            using State::State;
            void entry() {
                fsm.fsm.speed = -1;
            }
            void event(Periodic) override {
                if (fsm.fsm.position == 0)
                    transition(fsm.moveRight);
            }
            void exit() override {
                fsm.fsm.speed = 0;
            }
        } moveLeft {this};

        class MoveRight : public State {
            friend Fsm;
            using State::State;
            void entry() {
                fsm.fsm.speed = +1;
            }
            void event(Periodic) override {
                if (fsm.fsm.position == 7)
                    return fsm.fsm.transition(fsm.fsm.idle);
            }
            void exit() override {
                fsm.fsm.speed = 0;
            }
        } moveRight {this};
    } homing {this};

    class Idle : public State {
        friend Fsm;
        using State::State;
        void entry() {}
        void event(Goto g) override {
            return transition(fsm.moving, g);
        }
        void event(Home) override {
            return transition(fsm.homing);
        }
        void event(Periodic) override {}
    } idle {this};

    class Moving : public State {
        friend Fsm;
        using State::State;
        int target;
        void entry(Goto g) {
            target = g.position;
            if (target < fsm.position)
                fsm.speed = -1;
            if (target > fsm.position)
                fsm.speed = +1;
        }
        void event(Goto g) override {
            return transition(fsm.moving, g);
        }
        void event(Periodic) override {
            if (fsm.position == target)
                return transition(fsm.idle);
        }
        void exit() override {
            fsm.speed = 0;
        }

    } moving {this};
};

int main(int argc, char *argv[])
{
    Motor m;
    while (m.isMoving())
        m.periodic();
    m.moveTo(4);
    while (m.isMoving())
        m.periodic();
    return 0;
}
