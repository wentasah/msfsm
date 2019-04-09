// Copytight (C) 2019 Michal Sojka <ms@2x.cz>
//
// SPDX-License-Identifier: MIT

#ifndef FSM_HPP
#define FSM_HPP

#include <iostream>
#include <stdexcept>
#include <boost/type_index.hpp> // Only needed if StateName is used

namespace msfsm {

class noncopyable
{
protected:
    constexpr noncopyable() = default;
    ~noncopyable() = default;
    noncopyable( const noncopyable& ) = delete;
    noncopyable& operator=( const noncopyable& ) = delete;
};


template <class FSM>
class Fsm : noncopyable
{
protected:
    // Base class for all states
    class State : noncopyable {
    public:
        State(FSM *fsm) : fsm(*fsm) {}
        virtual ~State() {}
    private:
        friend Fsm;
        virtual void exit() {}

    protected:
        FSM &fsm;

        // Convenience method
        template<class S, class ... Types>
        void transition(S &nextState, Types... args) { fsm.Fsm<FSM>::transition(nextState, args...); }
    };

    template<class S, class ... Types>
    void transition(S &nextState, Types... args) {
        if (m_state)
            m_state->exit();
        static_cast<FSM*>(this)->onTransition(nextState);
        m_state = &nextState;
        nextState.entry(args...);
    }

    // Call this from derived class destructor if needed. We cannot
    // make this in our destructor, because the states are already
    // destoyed when it runs.
    void destroy() {
        if (m_state)
            m_state->exit();
        m_state = nullptr;
    }

private:
    State *m_state = nullptr;

    // Prevent compile error if derived class does not have its own onTransition() method
    void onTransition(State &nextState) {}

public:
    State* state() const { return m_state; }

    template <typename EV>
    void handle(EV event) {
        if (!m_state)
            throw std::domain_error(__PRETTY_FUNCTION__);
        static_cast<typename FSM::State*>(m_state)->event(event);
    }
};

// Mix-in class for named states
template <class S>
class Named {
    std::string m_name;

public:
    // Unfortunately, type_id does not correspond to derived class
    // in constructor, so we must retrieve (and cache) the name
    // later.
    const std::string& name() {
        if (m_name.empty())
            m_name = boost::typeindex::type_id_runtime(static_cast<S&>(*this)).pretty_name();
        return m_name;
    }
};

}

#endif // FSM_HPP
