#include <iostream>
#include "Timer.h"

Timer::Timer()
{

}
Timer::Timer(unsigned int ID, unsigned int timeToActivationMS, bool loop, void (*ptrToFunc)(), double startingMS)
{
	m_ptrToFunc = ptrToFunc;
	m_ID = ID;
	m_loop = loop;
	m_startingMS = startingMS;
	m_activationMS = timeToActivationMS;
	m_nextNode = (Timer*)0;
}

bool Timer::Update(double currentMS)
{
	if(currentMS - m_startingMS >= m_activationMS)
	{
		if(m_loop == true)
		{
			m_ptrToFunc();
			m_startingMS = currentMS;
			return true;
		}
		else
		{
			m_ptrToFunc();
			return false;
		}
	}
	return true;
}
void Timer::SetLoop(bool loop)
{
	m_loop = loop;
}
