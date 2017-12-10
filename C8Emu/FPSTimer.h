#pragma once
#include <vector>

class FPSTimer {

public:

	FPSTimer() :
		deltaTimes(1000)
	{}

	void Push(float dt) {
		deltaTimes.Push(dt);
	}

	float Average() {
		return deltaTimes.Average();
	}

private:

	template<typename T>
	class CircularQueue {

	public:
		CircularQueue(size_t size) :
			currentIndex(0),
			maxSize(size),
			data(size),
			currentSum(0),
			currentSize(0)
		{}

		void Push(T element) {
			if (currentIndex > maxSize - 1)
				currentIndex = 0;
			currentSum -= data[currentIndex];
			if (data[currentIndex] == 0)
				currentSize++;
			data[currentIndex] = element;
			currentSum += element;
			currentIndex++;
		}

		T Average() {
			return currentSum / currentSize;
		}
	private:
		T currentSum;
		size_t currentIndex;
		size_t currentSize, maxSize;
		std::vector<T> data;
	};
	
	CircularQueue<float> deltaTimes;
};