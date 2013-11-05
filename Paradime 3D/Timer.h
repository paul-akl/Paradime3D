#ifndef TIMER_CLASS
#define TIMER_CLASS

class Timer
{
public:
	Timer();
	Timer(unsigned int ID, unsigned int timeToActivationMS, bool loop, void (*ptrToFunc)(), double startingMS);
	bool Update(double currentMS);
	void SetLoop(bool loop);
	Timer *m_nextNode;
private:
	void (*m_ptrToFunc)();
	double m_startingMS;
	unsigned int m_ID;
	unsigned int m_activationMS; 
	bool m_loop;
};
#endif