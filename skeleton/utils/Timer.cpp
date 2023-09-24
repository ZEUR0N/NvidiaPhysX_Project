#include "Timer.h"

Timer::Timer() noexcept : timer(0), timeLimit(1.0f){

}

Timer::Timer(const float& maxTime) noexcept :timer(0), timeLimit(maxTime)  {

}

Timer::~Timer()noexcept {
}

void Timer::changeTimeLimit(const float& limit) noexcept {
	timeLimit = limit;
}

void Timer::reset()noexcept {
	timer = 0.0f;
}

bool Timer::isOver() const noexcept {
	return timer > timeLimit;
}

void Timer::update(const float& deltaTime) noexcept {
	if (timer <= timeLimit) timer += deltaTime;
}
