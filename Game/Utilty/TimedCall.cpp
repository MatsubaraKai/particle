#include "TimedCall.h"

TimedCall::TimedCall(std::function<void()> p, uint32_t time)
{
	time_ = time;
	pFunc_ = p;
}

void TimedCall::Update()
{
	if (isFinish_) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		isFinish_ = true;
		pFunc_();
	}
}
