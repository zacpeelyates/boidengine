//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	Event.h
// Author: Zac Peel-Yates (s1703955)
// Date Created: 01/01/22
// Last Edited:  01/01/22
// Brief: Base event class definitions & override definitions for each event type
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//base event class
#ifndef EVENT_H
#define EVENT_H

class Event
{
public:
	Event() : m_bHandled(false) {};
	virtual ~Event() = default;
	using DescriptorType = const char*;
	virtual DescriptorType Type() const = 0;
	void Handled() { m_bHandled = true; };
	bool IsHandled() const { return m_bHandled; };
private:
	//tell other subscribers they don't need to handle this event
	bool m_bHandled;
};

//event overloads, currently only windowresizeevent but could be extended
class WindowResizeEvent final : public Event
{
public:
	~WindowResizeEvent() override = default;
	WindowResizeEvent(unsigned int a_width, unsigned int a_height) : m_width(a_width), m_height(a_height) {}
	static constexpr DescriptorType descriptor = "WindowResizeEvent";
	virtual DescriptorType Type() const { return descriptor; };
	inline unsigned int GetWidth() const { return m_width; }
	inline unsigned int GetHeight() const { return m_height; }
private:
	unsigned int m_width;
	unsigned int m_height;

};
#endif // EVENT_H
