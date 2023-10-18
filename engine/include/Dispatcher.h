
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	Dispatcher.h
// Author: Zac Peel-Yates (s1703955)
// Date Created: 01/01/22
// Last Edited:  01/01/22
// Brief: Observer classes & Dispatcher class definitions and implementations. Used for handling events (see Event.h)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef DISPATCHER_H
#define DISPATCHER_H
#include <list>
#include <map>
#include <typeindex>
#include <typeinfo>
#include "Event.h"

//virtual observer base class
class Observer
{
public:
	//remove unwanted behaviours 
	Observer() = default;
	virtual ~Observer() = default;
	Observer(const Observer&) = delete;
	Observer(const Observer&&) = delete;
	//public execute event
	inline void Exec(Event* e) { Call(e); }
	virtual void* Instance() = 0;
private:
	//private event handling
	virtual void Call(Event* e) = 0;
};


// member function event observer
template<typename T, typename ConcreteEvent>
class MemberObserver : public Observer
{
public:
	typedef void(T::* MemberFunction)(ConcreteEvent*);
	MemberObserver(T* a_instance, MemberFunction a_function) : m_instance(a_instance), m_memberFunction(a_function) {}
	~MemberObserver() { m_instance = nullptr; }
	inline void* Instance() { return (void*)m_instance; }
private:
	inline void Call(Event* e) { (m_instance->*m_memberFunction)(static_cast<ConcreteEvent*>(e)); }
	MemberFunction m_memberFunction;
	T* m_instance;
};

//global function event observer
template<typename ConcreteEvent>
class GlobalObserver : public Observer
{
public:
	typedef void (*Function)(ConcreteEvent*);
	GlobalObserver(Function a_function) : m_function(a_function) {}
	~GlobalObserver() {}
	inline void* Instance() { return nullptr; }
private:
	inline void Call(Event* e) { (*m_function)(static_cast<ConcreteEvent*>(e)); };
	Function m_function;
};

//Event dispatcher, handles event subscriptions and sending event notifications
class Dispatcher
{
public:
	static inline Dispatcher* CreateInstance() { if (m_instance == nullptr) m_instance = new Dispatcher(); return GetInstance(); }
	static inline Dispatcher* GetInstance() { return m_instance; }
	static inline void DestroyInstance() { if (m_instance != nullptr) delete m_instance; m_instance = nullptr; }
	template<typename T, typename ConcreteEvent>
	void Subscribe(T* a_instance, void(T::* memberFunction)(ConcreteEvent*))
	{
		std::list<Observer*>* observers = m_subscribers[typeid(ConcreteEvent)];
		if (observers == nullptr)
		{
			//make new list for this event type
			observers = new std::list<Observer*>();
			m_subscribers[typeid(ConcreteEvent)] = observers;
		}
		observers->push_back(new MemberObserver<T, ConcreteEvent>(a_instance, memberFunction));
	}

	template<typename T, typename ConcreteEvent>
	void Unsubscribe(T* a_instance, void(T::* memberFunction)(ConcreteEvent*))
	{
		std::list<Observer*>* observers = m_subscribers[typeid(ConcreteEvent)];
		if (observers == nullptr) return;
		observers->remove(new MemberObserver<T, ConcreteEvent>(a_instance, memberFunction));

	}

	template<typename ConcreteEvent>
	void Publish(ConcreteEvent* e, bool cleanup = false)
	{
		std::list<Observer*>* observers = m_subscribers[typeid(ConcreteEvent)];
		if (observers == nullptr) return;
		for (auto& handler : *observers)
		{
			handler->Exec(e);
			if (static_cast<Event*>(e)->IsHandled()) break;
			if (cleanup) delete e;
		}
	}

protected:
	Dispatcher() {};
	~Dispatcher()
	{
		//clean up subscribers when destroyed
		for (auto iter = m_subscribers.begin(); iter != m_subscribers.end(); ++iter)
		{
			std::list<Observer*>* observers = iter->second;
			for (auto o = observers->begin(); o != observers->end(); ++o)
			{
				delete* o;
				*o = nullptr;
			}
			delete observers;
		}
	}

private:
	std::map<std::type_index, std::list<Observer*>*>m_subscribers;
	static Dispatcher* m_instance;

};
#endif // DISPATCHER_H
