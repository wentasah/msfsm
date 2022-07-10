// This example shows how to define nested FSMs independently of their
// nesting class. This allows to reuse sub-FSMs at multiple places.
// Here, we nest Homing FSM into the Motor FSM but only at one place.

#include "msfsm.hpp"
#include <iostream>

using namespace msfsm;
using namespace std;

// Event commont to all FSMs below
struct Periodic {};

// Homing FSM that is nested into Motor::Homing state (see below)
class FsmHoming : public Fsm<FsmHoming> {
public:
    virtual ~FsmHoming() {}
private:
    friend Fsm<FsmHoming>;

    // Homing base state
    class State : public Fsm::State, public Named<State> {
        friend Fsm;
        using Fsm::State::State;
        virtual void event(Periodic) = 0;
    };

    void onTransition(State& nextState) {
        cout << nextState.name() << endl;
    }

    // Extra interface for the higher FSM (Motor)
    //
    // Methods to be called by Motor
public:
    void start() { transition(moveLeft); }
    // Methods to be implemented by Motor
private:
    virtual void stop() = 0;
    virtual void setSpeed(int speed) = 0;
    virtual int getPosition() = 0;

    class MoveLeft : public State {
        friend Fsm;
        using State::State;
        void entry() {
            fsm.setSpeed(-1);
        }
        void event(Periodic) override {
            if (fsm.getPosition() == 0)
                transition(fsm.moveRight);
        }
        void exit() override {
            fsm.setSpeed(0);
        }
    } moveLeft {this};

    class MoveRight : public State {
        friend Fsm;
        using State::State;
        void entry() {
            fsm.setSpeed(+1);
        }
        void event(Periodic) override {
            if (fsm.getPosition() == 7)
                return fsm.stop();
        }
        void exit() override {
            fsm.setSpeed(0);
        }
    } moveRight {this};
};

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

    class Homing : public State, public FsmHoming {
    public:
        Homing(Motor *fsm) : Motor::State(fsm) {}
        ~Homing() = default;
    private:
        friend Fsm<Motor>;

        void entry() {
            start();
        }
        void event(Periodic p) override {
            handle(p);          // Send the event to sub-FSM
        }
        void exit() override {
            destroy();          // Destroy sub-FSM
        }

        // Called when sub-FSM finishes
        void stop() override { Motor::State::transition(fsm.idle); }

        void setSpeed(int speed) override { fsm.speed = speed; }
        int getPosition() override { return fsm.position; }

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
