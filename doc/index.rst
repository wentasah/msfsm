.. MSFSM documentation master file, created by
   sphinx-quickstart on Mon Apr 29 17:20:24 2019.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

MSFSM: Simple C++ state machine library
=======================================

..
   .. toctree::
      :maxdepth: 2
      :caption: Contents:

..
   .. contents:: Table of Contents

.. highlight:: c++

.. warning:: Documentation is not finished. Work in progress.


Simple state machine
--------------------

The following listing shows a simple state machine built with MSFSM.

The **state machine** is a class derived from ``Fsm`` class template,
which gets the derived class as a template parameter (so called `CRTP
<https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern>`_).
The **states** are classes nested in the state machine class and derived
from ``Fsm::State``. Since the ``Fsm`` is the base class, the
``Fsm::`` prefix can be omitted as in the example below.

The state classes have to contain a little bit of "bolierplpate" code
-- ``friend Fsm;`` and ``using State::State``. The friend declaration
allows the ``Fsm`` class to call state methods. Alternatively, instead
of declaring a friend, we could make all the state methods public, but
this would mean that anybody can call them and we don't want anybody to
mess up with our states. ``using State::State;`` is there to inherit
the constructor from ``Fsm::State``. If you need to define your own
constructor, you will need to call ``Fsm::State::State`` from it.

Besides defining our state classes, we also immediately instantiate
them as member variables of our state machine class and use C++11
in-class member initializers ``{this}`` to pass ``this`` to their
constructors. This is needed, because, as will be seen in the next
examples, each state class has a reference called ``fsm``, referring
to the state machine object.

In the constructor of the ``Simple`` class, we specify the initial
state by calling the ``transition()`` method. Typically,
``transition()`` is called as a reaction to certain events, but as
this is a simple example, we call it directly and hence we have to
make it public with ``using Fsm::transition;``.

.. literalinclude:: ../examples/simple.cpp
   :end-before: int main(

The ``main()`` function creates the state machine and invokes
transition from the initial *off* state to the *on* state.

.. literalinclude:: ../examples/simple.cpp
   :start-after: int main(
   :prepend: int main(int argc, char *argv[])

The output of the program will be:

.. literalinclude:: ../examples/simple.expect

This demonstrates the basic functionality -- calling of ``entry`` and
``exit`` methods on state transition. You may wonder why you do not
see ``Exiting on`` in the output. That's because, technically, the
*on* state is never left. If you need to leave it, add a destructor to
your state machine class::

  ~Simple {
      destroy();
  }

Reaction to events
------------------

The previous example was simple enough to fit into one screen, but it
lacked an important feature: reaction to events. In MSFSM, events are
defined by types. In the following example, we will use two events:
``Up`` and ``Down``:

.. literalinclude:: ../examples/events.cpp
   :start-after: using namespace std;
   :end-before: class Elevator

Next, we will define the ``Elevator`` class, which reacts to those
events. We don't need to expose anything except the constructor and
members inherited from ``Fsm``, so we declare all the internals as
private. For this to work, we must allow ``Fsm`` to access the private
members so we add ``friend Fsm;``.

Another difference from the simple example above is that we don't
inherit our states directly from ``Fsm::State``, but we define an
intermediate class ``Elevator::State``. This class defines virtual
methods called ``event`` for each event we want our state machine to
react. Here we define a default reaction (printing a message), but we
can make these methods pure virtual if we want.

Then we can define our two states ``Upper`` and ``Lower`` by
inheriting from ``Elevator::State`` and adding ``entry()`` method and
overriding ``event()`` or ``exit()`` methods.

.. literalinclude:: ../examples/events.cpp
   :start-after: class Down {};
   :end-before: int main(

In the ``main()`` function, we create our elevator state machine and
"send" a few events to it.

.. literalinclude:: ../examples/events.cpp
   :start-after: int main(
   :prepend: int main(int argc, char *argv[])

The output of this program looks as follows:

.. literalinclude:: ../examples/events.expect

..
   Indices and tables
   ==================

   * :ref:`genindex`
   * :ref:`modindex`
   * :ref:`search`

..  LocalWords:  initializer initializers destructor
