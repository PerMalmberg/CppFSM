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
In two easy steps:

  1. Initialze an FSM, either with a logger showing state transitions
  
     ```
    fsm::FSM<YourBaseState> fsm{ std::make_unique<InitialState>(), std::make_shared<Your>() };
    ```

or without a logger

    ```
    fsm::FSM<YourBaseState> fsm{ std::make_unique<InitialState>() };
    ```
    
  2. Next, you call ```fsm.Run()``` which in turn will call fsm.Tick() until a state calls ```GetFSM().Terminate()```
  
That's it. Your states and events will now drive the entire flow.

#Creating your states and events
##The base state
As seen above, the FSM is templated on the common base state for all the states which will be used. As such, you inherit from the class fsm::BaseState and pass that same state type as the template parameter of the ```fsm::BaseState```. This is the state from which all of your substates *must* inherit from.

```
class YourBaseState : public fsm::BaseState<YourBaseState>
{
public:
	YourBaseState( const std::string& name )
			: BaseState( name )
	{}
};
```
##Setting up (chained) Enter/Leave behavior
First, let me explain what this is all about.

<pre>
Current State --------------------------| {destructed, deinit}
                                        |
                                        |---------------


                 |-{created, init}----------------------
                 |
Next State   ____|

Time flow --->
</pre>
For any FSM, anytime a state transition is about to happen, a new state is created (or re-initialized if using static instances). The problem is that when this happens, and it runs its constructor code, the current state is still alive and hasn't yet run its deinitializing code (destructor), as seen in the above code. A very common symptom of this is logging that is written in the wrong order, i.e. initialization before deinitialization. Also, consider the case when the states both allocates resources - you'll have more resources allocated than is needed due to the nature of how states are initialized.

To aleviate these problems this FSM uses a Enter/Leave methology. It works as follows.

<pre>
Current State ----------------{Leave} --| {destructed}
                                        |
                                        |---------------


                  |-{created}----------------{Enter}----
                  |
Next State   _____|

Time flow --->
</pre>
We now correct deinitialization and initialization order - the current state runs it Leave-method *before* the new state runs its Enter-method. We thus achieve a more natural flow which makes things easier for the user.

Here's how you implement this behavor in code:

```
class SubState : public YourBaseState
{
public:
	SubState();

private:
	void Enter();
	void Leave();

	SetupEnterChain( SubState );
	SetupLeaveChain( SubState );
};

SubState::SubState() :
		EnterLeave( d, "SubState" ),
		enterChain( *this ), leaveChain( *this )
{

}

```
The framework provides two macros that eases the job to achieve the goal. You provide it with the name of the class and then add the enterChain() and leaveChain() to the initialization list of your constructor and of course implement the Enter- and Leave-methods.

Take note of the access restrictions of the Enter- and Leave-methods and the fact that they are *not* virtual. They are placed as private to avoid compiler warnings due to method hidning (as base states may also implement these methods, see below).

Once you have done these steps, the FSM will call the Enter- and Leave-methods automatically at the appropiate time during state transition.

###Chained Enter/Leave-methods 
As this FSM supports hierachial states, it must also allow make sure each state in the inheritance chain gets its Enter- and Leave-method called, in the correct order. For example, if class B inherits A, A::Enter() will be called before B::Enter(). Likewise, B::Leave() will be called before A::Leave(). This is obviously the same order constructors and destructors are called, but it all happens at a more appropiate time, not at the time of construction or destructions on the state object. All you have to do is to follow the steps above to implement Leave and Enter, and this is all taken care of.

##Events
To be documented.
