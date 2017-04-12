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

double ProfileTimer::getTimeAndReset(double dividend) {
	dividend = (dividend == 0) ? m_numInvocations : dividend;
	double result = (m_totalTime == 0 && dividend == 0) ? 0 : (1000 * m_totalTime) / ((double) dividend);
	m_totalTime = 0;
	m_numInvocations = 0;

	return result;
}

double ProfileTimer::displayAndReset(const std::string& message, double dividend, int displayedMessageLength) {
	std::string whiteSpace = "";
	for (int i = message.length(); i < displayedMessageLength; i++)
		whiteSpace += "";

	double time = getTimeAndReset(dividend);
	std::cout << message << whiteSpace << time << " ms" << std::endl;
	return time;
}