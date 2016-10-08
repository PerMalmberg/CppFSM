# CppFSM
An event driven, templated, hierachial Finite State Machine in simple C++

#Why
There are many implementations of Finite State Machines available, so why another one? I wanted a completely
(or as near as possible) event driven FSM in simple modern C++. I also wanted to hide the complexities from
the user, so that s/he can concentrate on the actualy task at hand. A framework shall help the user, not put 
additional burden in their laps.

#What
  * Hierarchial states, allowing for easy extension of existing state behaviour.
  * Correct call order for Enter and Leave methods, including chained Enter/Leave for the entire state class hierarchy.
      * No overlapping of states - a state is really left before the next is entered. Naturally, constructors and destructors are 
      still being called in the normal order, but the Enter/Leave chains allows for easy and correct sequencial execution relative
      to other states.
  * Fully event driven and type safe architecture.
    * Events are always delivered in order and in a completely type safe way.
    * No stray events between states; states are cleaned up whenever a new state is activated.
  * Once the FSM is initialized the states drive the entire flow until the FSM is told to terminate, at which point 
  the normal leave procedure of the current state is executed which allows for a graceful termination.
  * No destruction of the current state until it returns control to the FSM. This means that calling SetState() and SendEvent() is
  completely safe - the current state will not be destructed until the FSM reaquires control, which happens when the current state 
  returns from what ever work it was doing.
  
#How
In X easy steps:

  1. Initialze an FSM, either with a logger showing state transitions
  
     ```
    fsm::FSM<YourBaseState> fsm{ std::make_unique<InitialState>(), std::make_shared<Your>() };
    ```

or without a logger

    ```
    fsm::FSM<YourBaseState> fsm{ std::make_unique<InitialState>() };
    ```
    
  2. Next, you call ```fsm.Run()``` which in turn will call fsm.Tick() until a state calls ```GetFSM().Terminate()```
  
That's it. Now for how to write your own states and events.

