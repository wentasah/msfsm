MSFSM
=====

Simple yet powerful C++ finite state machine library.

Requirements
------------

- State is represented by a pointer
- Concise syntax understandable for average C++11 programmer
- Entry/exit actions
- Allows both per-FSM and per-state variables
- Transitions and guards expressed as C++ code (if/else, function
  calls, ...)
- FSM can react to custom events represented by arbitrary types
- Events can contain arbitrary payload that can be used in the
  handling code
- No memory allocation during state changes (like in [Boost Statechart][])
- No need to increase limits of other libraries (like
  `BOOST_MPL_LIMIT_VECTOR_SIZE` needed by [Boost MSM][])
- No "advanced" template magic. Light template usage (understandable
  to me) is allowed :)
- Allows multiple instances of the same state machine (as opposed to [tinyfsm][])
- Enforce passing of additional data when transitioning to selected
  states (e.g. error code must be specified when entering an error
  state).
- Supports nested state machines

Installation
------------

This is a header-only library. Just copy it to your project and
include it.

Documentation
-------------

Documentation is in preparation. For now, you can look at [several
examples](examples/).

[Boost Statechart]: https://www.boost.org/doc/libs/1_69_0/libs/statechart/doc/index.html
[Boost MSM]: https://www.boost.org/doc/libs/1_69_0/libs/msm/doc/HTML/index.html
[tinyfsm]: https://github.com/digint/tinyfsm
