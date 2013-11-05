#ifndef CLOCK_CLASS
#define CLOCK_CLASS

#include <sstream>
#include <string>
#include <vector>
#include <Windows.h>
#include "Timer.h"

using namespace std;

namespace Clock
{	
	void init();

	__int64 GetCurrentTimeMS();
	unsigned long GetCurrentFrame();
	void Update(double *deltaTime_arg);
	void Update(float *deltaTime_arg);
	void Update();
	double GetMSdouble();
	float GetMSfloat();
	int GetFPS();

	string GetFPSstring();
	string GetMSstring();

	void CreateTimer(void (*ptrToFunc_arg)(), unsigned int ID_arg, bool loop_arg, unsigned int timeToActivationMS_arg);

	void UpdateMS();
	void UpdateTimers();
}
#endif