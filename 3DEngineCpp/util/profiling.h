#ifndef PROFILING_INCLUDED_H
#define PROFILING_INCLUDED_H

#include <string>

#define PROFILING_DISABLE_MODEL_RENDERING 0
#define PROFILING_DISABLE_SHADING 0
#define PROFILING_SET_1x1_VIEWPORT 0
#define PROFILING_SET_2x2_TEXTURE 0

class ProfileTimer {
public:
	ProfileTimer() : m_numInvocations(0), m_totalTime(0), m_startTime(0) {}

	void startInvocation();
	void stopInvocation();

	double getTimeAndReset(double divisor = 0);
	double displayAndReset(const std::string& message, double divisor = 0, int displayedMessageLength = 40);
private:
	int m_numInvocations;
	double m_totalTime;
	double m_startTime;
};

#endif