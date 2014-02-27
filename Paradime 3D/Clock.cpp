#include <iostream>
#include <sstream>

#include "Clock.h"
#include "Config.h"
#include "CurrentUpdateState.h"
#include "Timer.h"

using namespace Clock;

	LARGE_INTEGER	ticksPerSecond,
					timeLast,
					timeCurrent;

	unsigned long	currentFrame,
					lastFrame;
	
	double	lastFrameMS,
			elapsedMS;

	Timer	*firstTimer = (Timer*)0,
			*currentTimer,
			*tempTimer;
	
	static unsigned char clockInstances;
	stringstream ss;

void Clock::init()
{
	lastFrameMS = 0;
	lastFrame = 0;
	currentFrame = 0;

	if(QueryPerformanceFrequency(&ticksPerSecond))
	{
		 QueryPerformanceCounter(&timeCurrent);
		 timeLast = timeCurrent;
	}
	else
	{
		throw "QueryPerformanceFrequency has failed";
	}
}

void Clock::Update(double *deltaTime_arg)
{
	UpdateMS();
	UpdateTimers();
	*deltaTime_arg = lastFrameMS / Config::engine::delta_time_divider;
}

void Clock::Update(float *deltaTime_arg)
{
	UpdateMS();
	UpdateTimers();
	*deltaTime_arg = static_cast<float>(lastFrameMS / Config::engine::delta_time_divider);
}

void Clock::Update()
{
	UpdateMS();
	UpdateTimers();
	Current::deltaTime = lastFrameMS / Config::engine::delta_time_divider;
}

void Clock::UpdateMS()
{
	currentFrame++;
	QueryPerformanceCounter(&timeCurrent);
	lastFrameMS = static_cast<double>(timeCurrent.QuadPart - timeLast.QuadPart) / (static_cast<double>(ticksPerSecond.QuadPart)/1000);
	timeLast = timeCurrent;
	elapsedMS += lastFrameMS;
}

double Clock::GetMSdouble()
{
	return lastFrameMS;
}

float Clock::GetMSfloat()
{
	return static_cast<float>(lastFrameMS);
}

int Clock::GetFPS()
{
	return (int)(1000 / lastFrameMS);
}

string Clock::GetFPSstring()
{
	ss.str(std::string());
	ss << "FPS: " << (1000 / lastFrameMS);
	return ss.str();
}

string Clock::GetMSstring()
{
	ss.str(std::string());
	ss << "MS: " << GetMSfloat();
	return ss.str();
}

void Clock::UpdateTimers()
{
	if(firstTimer)
	{
		currentTimer = firstTimer;
		currentTimer->Update(elapsedMS);

		while(currentTimer->m_nextNode)
		{
			if(!currentTimer->m_nextNode->Update(elapsedMS))
			{
				if(currentTimer->m_nextNode->m_nextNode)
				{
					tempTimer = currentTimer->m_nextNode->m_nextNode;
					delete currentTimer->m_nextNode;
					currentTimer->m_nextNode = tempTimer;
					currentTimer = currentTimer->m_nextNode;
				}
				else
				{
					delete currentTimer->m_nextNode;
					currentTimer->m_nextNode = (Timer*)0;
				}
			}
			else
				currentTimer = currentTimer->m_nextNode;
		}
	}
}

void Clock::CreateTimer(void (*ptrToFunc_arg)(), unsigned int ID_arg, bool loop_arg, unsigned int timeToActivationMS_arg)
{
	if(!firstTimer)
	{
		firstTimer = new Timer(ID_arg, timeToActivationMS_arg, loop_arg, ptrToFunc_arg, elapsedMS);
	}
	else
	{
		while(currentTimer)
			currentTimer = currentTimer->m_nextNode;

		currentTimer = new Timer(ID_arg, timeToActivationMS_arg, loop_arg, ptrToFunc_arg, elapsedMS);
	}
	//currentTimer = new Timer(*ID_arg, timeToActivationMS_arg, loop_arg, ptrToFunc_arg, elapsedMS);

	
	////int i = 0;
	//if(currentTimer)
	///{
	//	for(currentTimer = firstTimer; currentTimer->m_nextNode; currentTimer = currentTimer->m_nextNode)
	//	{

	//	}

		//currentTimer->m_nextNode = new Timer(ID_arg, timeToActivationMS_arg, loop_arg, ptrToFunc_arg, elapsedMS);
	//}
	//currentTimer = new Timer(*ID_arg, timeToActivationMS_arg, loop_arg, ptrToFunc_arg, elapsedMS);
}