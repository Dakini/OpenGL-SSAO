#ifndef CLOCK_H
#define CLOCK_H

#include "opengl.h"
#include <time.h>
class Clock{
public:
	typedef double real;
	typedef long long inteGERT;

	//clock Constructor
	Clock();

	inteGERT frequency() const; //return ticks/sec
	inteGERT ticks() const; //return num of ticks since previous update call
	inteGERT totalTicks() const; //return total number of ticks elapsed since clock was intialised
	real seconds() const; //return number of seconds elapsed since previous call to update
	real totalSeconds() const; //return total number of seconds since clock was intialised

	void update(); // update clock
	void reset(); //reset clock

private:
	//ticks since previous update
	inteGERT ticks_;
	//ticks returned inprevious two calls (update);
	inteGERT cticks;
	inteGERT pticks;

	//totalticks since intialisation
	inteGERT totalTicks_;
	//ticks/sec
	inteGERT freq;

	//1/freq
	real rfreq;

	/// seconds since previous call to update
	real seconds_;
	//total seconds since clock intialised
	real totalSeconds_;
};

inline Clock::inteGERT Clock::frequency() const{
	return freq;
}
inline Clock::inteGERT Clock::ticks() const{
	return ticks_;
}
inline Clock::inteGERT Clock::totalTicks() const{
	return totalTicks_;
}
inline Clock::real Clock::seconds() const{
	return seconds_;
}
inline Clock::real Clock::totalSeconds() const{
	return totalSeconds_;
}
#endif
