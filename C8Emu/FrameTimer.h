#pragma once
#include <chrono>

class FrameTimer {

public:
	FrameTimer() {
		last = std::chrono::steady_clock::now();
	}

	__int64 Mark() {
		using namespace std::chrono;
		const auto old = time_point_cast<milliseconds>(last);
		last = time_point_cast<milliseconds>(steady_clock::now());

		auto dur = last - old;
		
		return dur.count();
	}

private:
	std::chrono::steady_clock::time_point last;
};