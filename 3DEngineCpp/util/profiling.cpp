#include "profiling.h"
#include "time.h"
#include <cassert>
#include <iostream>

void ProfileTimer::startInvocation() {
	m_startTime = Time::getTime();
}

void ProfileTimer::stopInvocation() {
	if (m_startTime = 0) {
		std::cout << "Error: StopInvocation() has been called without calling the StartInvocation() method first." << std::endl;
		assert(m_startTime != 0);
	}

	m_numInvocations++;
	m_totalTime += (Time::getTime() - m_startTime);
	m_startTime = 0;
}

double ProfileTimer::getTimeAndReset(double divisor) {
	divisor = (divisor == 0) ? m_numInvocations : divisor;
	double result = (m_totalTime == 0 && divisor == 0) ? 0 : (1000 * m_totalTime) / ((double) divisor);
	m_totalTime = 0;
	m_numInvocations = 0;

	return result;
}

double ProfileTimer::displayAndReset(const std::string& message, double divisor, int displayedMessageLength) {
	std::string whiteSpace = "";
	for (int i = message.length(); i < displayedMessageLength; i++)
		whiteSpace += "";

	double time = getTimeAndReset(divisor);
	std::cout << message << whiteSpace << time << " ms" << std::endl;
	return time;
}