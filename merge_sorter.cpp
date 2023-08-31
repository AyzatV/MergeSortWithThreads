#include "merge_sorter.h"

namespace avhw28
{
	MergeSorter::MergeSorter() {}
	MergeSorter::~MergeSorter() {}

    std::stringstream MergeSorter::fillArrayInfo(int16_t* out_array, uint32_t size)
    {
        std::stringstream info;
        srand(time(nullptr));       // ������� ��������� ����� ������������� �������� �������
        
        // ������ �������� �������
        auto start = std::chrono::steady_clock::now();
        for (uint32_t i = 0; i < size; i++) { out_array[i] = (rand() % 32000) - 16000; }
        auto stop = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() / 1000.0f;
        info << "�������� � ������ " << size << " �������� �� " << std::fixed << std::setprecision(3) << duration << " ������" << std::endl;
        return info;
    }

    std::stringstream MergeSorter::mergeSortInfo(int16_t* in_array, int16_t* sorted_array, uint32_t size, bool with_threads)
    {
        std::stringstream info;
        use_threads = with_threads;

        // ����������� ������� ��� ����������
        for (uint32_t i = 0; i < size; i++) { sorted_array[i] = in_array[i]; }
        // ���������� �������
        auto start = std::chrono::steady_clock::now();
        mergeSortPart(sorted_array, 0, size - 1);
        auto stop = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() / 1000.0f;
        info << std::endl << "���������� ���������� " << (with_threads ? "� ��������������" : "��� �������������") << " �������:" << std::endl;
        info << "������ �� " << size << " ��������� ������������ �� " << std::fixed << std::setprecision(3) << duration << " ������" << std::endl;
        info << "���������� ����������������� ���: " << unsortedPairs(sorted_array, size) << std::endl;
        return info;
    }

	void MergeSorter::merge(int16_t* arr, uint32_t l, uint32_t m, uint32_t r)
	{
        uint32_t nl = m - l + 1;
        uint32_t nr = r - m;
        uint32_t m1 = m + 1;
        // ������� ��������� �������
        auto left = new int16_t[nl];
        auto right = new int16_t[nr];
        // �������� ������ �� ��������� �������
        uint32_t i = 0;
        uint32_t j = 0;
        for (; i < nl; i++) { left[i] = arr[l + i]; }
        for (; j < nr; j++) { right[j] = arr[m1 + j]; }
        i = 0;
        j = 0;
        int k = l;  // ������ ����� �����
        while (i < nl && j < nr)
        {
            // ���������� ����������� �������� ������� �� ������� ������
            if (left[i] <= right[j])
            {
                arr[k] = left[i];
                i++;
            }
            else
            {
                arr[k] = right[j];
                j++;
            }
            k++;
        }
        // ���������� ���������� �������� ����� �����
        while (i < nl)
        {
            arr[k] = left[i];
            i++;
            k++;
        }
        // ���������� ���������� �������� ������ �����
        while (j < nr)
        {
            arr[k] = right[j];
            j++;
            k++;
        }
        delete[] left;
        delete[] right;
    }

    void MergeSorter::mergeSortPart(int16_t* arr, uint32_t l, uint32_t r)
    {
        if (l >= r) return;     // ����� �� ��������
        uint32_t m = (l + r - 1) / 2;
        
        if (use_threads)
        // ������������� ���������� ������ ��� ������ ����� ��� ������� �������
        {
            bool pro_l = (m - l) > 50000;
            bool pro_r = (r - m) > 50000;
            std::future<void> fl;
            std::future<void> fr;
            if (pro_l) fl = std::async(std::launch::deferred, [&]() { mergeSortPart(arr, l, m); });
            if (pro_r) fr = std::async(std::launch::deferred, [&]() { mergeSortPart(arr, m + 1, r); });
            if (pro_l) fl.wait();
            else mergeSortPart(arr, l, m);
            if (pro_r) fr.wait();
            else mergeSortPart(arr, m + 1, r);
        }
        else
        // ��� ������������� ������ ���������������
        {
            mergeSortPart(arr, l, m);
            mergeSortPart(arr, m + 1, r);
        }
        merge(arr, l, m, r);
    }

    uint32_t MergeSorter::unsortedPairs(int16_t* arr, uint32_t size)
    {
        uint32_t result = 0;
        for (uint32_t i = 1; i < size; i++) { if (arr[i - 1] > arr[i]) result++; }
        return result;
    }
}