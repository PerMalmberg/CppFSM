# CppFSM
An event driven, templated, hierachial Finite State Machine in simple C++

# Why
There are many implementations of Finite State Machines available, so why another one? I wanted a completely
(or as near as possible) event driven FSM in simple modern C++. I also wanted to hide the complexities from
the user, so that s/he can concentrate on the actualy task at hand. A framework shall help the user, not put 
additional burden in their laps.

# What
  * Hierarchical states, allowing for easy extension of existing state behaviour.
  * Correct call order for Enter and Leave methods, including chained Enter/Leave for the entire state class hierarchy.
      * No overlapping of states - a state is really left before the next is entered. Naturally, constructors and destructors are 
      still being called in the normal order, but the Enter/Leave chains allows for easy and correct sequencial execution relative
      to other states.
  * Fully event driven and type safe architecture.
    * Events are always delivered in order and in a completely type safe way.
    * No stray events between states; events are cleaned up whenever a new state is activated. 
      * To communicate between states, you should construct them with what they need to do their work.
  * Once the FSM is initialized the states drive the entire flow until the FSM is told to terminate, at which point 
  the normal leave procedure of the current state is executed which allows for a graceful termination.
  * No destruction of the current state until it returns control to the FSM. This means that calling SetState() and SendEvent() is
  completely safe - the current state will not be destructed until the FSM reaquires control, which happens when the current state 
  returns from what ever work it was doing.
  
# How
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
As seen above, the FSM is templated on the common base state for all the states. As such, you inherit from the class fsm::BaseState and pass that same state type as the template parameter of the ```fsm::BaseState```. This is the state from which all of your substates *must* inherit from.

```
class YourBaseState : public fsm::BaseState<YourBaseState>
{
public:
	YourBaseState( const std::string& name )
			: BaseState( name )
	{}
};
```

A state may override the default empty implementation of the Tick-method in the fsm::BaseState. The purpose of this method is to provide the state with a place where it can perform some work when there are no events to process.
 
```
class YourBaseState : public fsm::BaseState<YourBaseState>
{
public:
	YourBaseState( const std::string& name )
			: BaseState( name )
	{}

protected:
    void Tick() override { }
};
```

## Setting up Enter/Leave behavior
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
For any FSM, anytime a state transition is about to happen, a new state is created (or re-initialized if using static instances). The problem is that when this happens, and it runs its constructor code, the current state is still alive and hasn't yet run its deinitialization code (destructor), as seen in the above code. A very common symptom of this is logging that is written in the wrong order, i.e. initialization before deinitialization. Also, consider the case when the states both allocates resources - you'll have more resources allocated than is needed due to the nature of how states are initialized.

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
We now get correct deinitialization and initialization order - the current state runs it Leave-method *before* the new state runs its Enter-method. We thus achieve a more natural flow which makes things easier for the user.

Here's how you implement this behavior in code:

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

Once you have done these steps, the FSM will call the Enter- and Leave-methods automatically at the appropriate time during state transition. Both Enter and Leave are optional, so only implement them where they are needed.

### Chained Enter/Leave-methods 
As this FSM supports hierarchical states, it must also make sure each state in the inheritance chain gets its Enter- and Leave-method called, in the correct order. For example, if class B inherits A, A::Enter() must be called before B::Enter(). Likewise, B::Leave() will be called before A::Leave(). This is obviously the same order constructors and destructors are called, but it all happens at a more appropriate time, not at the time of construction or destruction of the state. All you have to do is to follow the steps above to implement Enter and Leave, and this is all taken care of by the FSM.

## Events
Contrary to regular direct calls, states should request a certain task to be executed by sending an event. The benefits are better control of the call stack (it becomes much shorter), it allows for easy reuse of functionality in base states and it helps manage [separations of concerns](https://en.wikipedia.org/wiki/Separation_of_concerns).
 
So, instead of directly calling DoMyWork(), the state sends an event via the FSM; ```GetFSM().EnqueueEvent( std::make_unique<DoWork>( ... ) );```, and the DoWork method is instead implemented as an EventReceiver.
  
```
// Note: Forward declare your events, do not include their header files as that causes circular dependencies.
class EventA;

class YouBaseState
		: public fsm::BaseState<YouBaseState>,
		  public fsm::EventReceiver<DoWork>
{
public:
	YouBaseState( const std::string& name );

	void Event( DoWork& ) override { }
};  
```

When done, the event handler for DoWork sends an event do signal the continuation of the flow. Alternatively, the Tick() method may do something when it sees that DoWork has been handled.

***Note:*** When enqueueing an event it will remain queued until the state returns from whatever work it is performing, so make sure not to create infinite/waiting loops in your states. 

## Changing states
Like events, when telling the FSM to set a new state, it is actually queued until the current state relinquishes control to the FSM. 

 As such, calling ```GetFSM().SetState( std::make_unique<TheNewState>( ... ) ); ``` and then performing additional work is safe, although the program flow becomes clearer when you do not.   
