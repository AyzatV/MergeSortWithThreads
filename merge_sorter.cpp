#include "merge_sorter.h"

namespace avhw28
{
	MergeSorter::MergeSorter() {}
	MergeSorter::~MergeSorter() {}

    std::stringstream MergeSorter::fillArrayInfo(int16_t* out_array, uint32_t size)
    {
        std::stringstream info;
        srand(time(nullptr));       // задание начальной точки генерирования значений массива
        
        // запись значений массива
        auto start = std::chrono::steady_clock::now();
        for (uint32_t i = 0; i < size; i++) { out_array[i] = (rand() % 32000) - 16000; }
        auto stop = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() / 1000.0f;
        info << "Записано в массив " << size << " значений за " << std::fixed << std::setprecision(3) << duration << " секунд" << std::endl;
        return info;
    }

    std::stringstream MergeSorter::mergeSortInfo(int16_t* in_array, int16_t* sorted_array, uint32_t size, bool with_threads)
    {
        std::stringstream info;
        use_threads = with_threads;

        // копирование массива для сортировки
        for (uint32_t i = 0; i < size; i++) { sorted_array[i] = in_array[i]; }
        // сортировка массива
        auto start = std::chrono::steady_clock::now();
        mergeSortPart(sorted_array, 0, size - 1);
        auto stop = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() / 1000.0f;
        info << std::endl << "Результаты сортировки " << (with_threads ? "с использованием" : "без использования") << " потоков:" << std::endl;
        info << "Массив из " << size << " элементов отсортирован за " << std::fixed << std::setprecision(3) << duration << " секунд" << std::endl;
        info << "Количество неотсортированных пар: " << unsortedPairs(sorted_array, size) << std::endl;
        return info;
    }

	void MergeSorter::merge(int16_t* arr, uint32_t l, uint32_t m, uint32_t r)
	{
        uint32_t nl = m - l + 1;
        uint32_t nr = r - m;
        uint32_t m1 = m + 1;
        // создаем временные массивы
        auto left = new int16_t[nl];
        auto right = new int16_t[nr];
        // копируем данные во временные массивы
        uint32_t i = 0;
        uint32_t j = 0;
        for (; i < nl; i++) { left[i] = arr[l + i]; }
        for (; j < nr; j++) { right[j] = arr[m1 + j]; }
        i = 0;
        j = 0;
        int k = l;  // начало левой части
        while (i < nl && j < nr)
        {
            // записываем минимальные элементы обратно во входной массив
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
        // записываем оставшиеся элементы левой части
        while (i < nl)
        {
            arr[k] = left[i];
            i++;
            k++;
        }
        // записываем оставшиеся элементы правой части
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
        if (l >= r) return;     // выход из рекурсии
        uint32_t m = (l + r - 1) / 2;
        
        if (use_threads)
        // использование отдельного потока для каждой части при наличии условий
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
        // без использования режима многопоточности
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