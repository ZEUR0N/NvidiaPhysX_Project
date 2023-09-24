#pragma once
class Timer {
public:
	Timer() noexcept;
	Timer(const float& maxTime) noexcept;

	~Timer() noexcept;
	
	void changeTimeLimit(const float& limit) noexcept;

	void reset() noexcept;

	bool isOver() const noexcept;
	
	void update(const float& deltaTime) noexcept ;

private:

	float timer, timeLimit;
};

