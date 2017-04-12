#ifndef PROFILING_INCLUDED_H
#define PROFILING_INCLUDED_H

#include <string>

class ProfileTimer {
public:
	ProfileTimer() : m_numInvocations(0), m_totalTime(0), m_startTime(0) {}

	void startInvocation();
	void stopInvocation();

	double getTimeAndReset(double dividend = 0);
	double displayAndReset(const std::string& message, double dividend = 0, int displayedMessageLength = 40);
private:
	int m_numInvocations;
	double m_totalTime;
	double m_startTime;
};

#endif