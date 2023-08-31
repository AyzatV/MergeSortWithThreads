#pragma once
#include <chrono>
#include <future>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace avhw28
{
	class MergeSorter
	{
	public:
		MergeSorter();
		~MergeSorter();
		std::stringstream fillArrayInfo(int16_t* out_array, uint32_t size);
		std::stringstream mergeSortInfo(int16_t* in_array, int16_t* sorted_array, uint32_t size, bool with_threads = false);

	private:
		bool use_threads = false;
		void merge(int16_t* arr, uint32_t l, uint32_t m, uint32_t r);
		void mergeSortPart(int16_t* arr, uint32_t l, uint32_t r);
		uint32_t unsortedPairs(int16_t* arr, uint32_t size);
	};
}