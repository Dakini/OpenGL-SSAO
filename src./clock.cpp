#include <Windows.h>
#include "clock.h"

Clock::Clock(): ticks_(0), pticks(0), totalSeconds_(0), totalTicks_(0), seconds_(0){
#ifdef _WIN32
	LARGE_INTEGER tmp;
	QueryPerformanceFrequency(&tmp);
	freq = (inteGERT) tmp.QuadPart;
	QueryPerformanceCounter(&tmp);
	cticks = tmp.QuadPart;

#else
	freq = (inteGERT) CLOCKS_PER_SEC;
	cticks = (inteGERT) clock();
#endif
	rfreq = real(1) / (real)freq;
}

void Clock::update(){
	pticks = cticks;
	//get new value for currentticks
#ifdef _WIN32
	LARGE_INTEGER tmp;
	QueryPerformanceCounter(&tmp);
	cticks = tmp.QuadPart;
#else
	cticks = (clock::inteGERT) clock();
#endif
	ticks_ = cticks-pticks;
	totalTicks_ +=ticks_;
	seconds_ = (Clock::real) ticks_ * rfreq;
	totalSeconds_ +=seconds_;
}

void Clock::reset() {
	ticks_=0;
	totalTicks_ = 0;
	seconds_ = 0;
	totalSeconds_ = 0;
#ifdef _WIN32
	LARGE_INTEGER tmp;
	QueryPerformanceCounter(&tmp);
	cticks = tmp.QuadPart;
#else
	cticks = (clock::inteGERT) clock();
#endif
}