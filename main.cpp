#include "merge_sorter.h"
#include <Windows.h>
using std::cout;
using std::endl;
using namespace avhw28;

int main()
{
	SetConsoleOutputCP(1251);	// ��� ������ ���������
	const uint32_t A_SIZE = 10000000;
	auto ini_array = new int16_t[A_SIZE];
	auto sorted_array = new int16_t[A_SIZE];

	MergeSorter ms;
	// ������������� ��������� �������
	cout << ms.fillArrayInfo(ini_array, A_SIZE).str() << endl;
	// ���������� �������������� ������� � ������ ������ ������
	cout << ms.mergeSortInfo(ini_array, sorted_array, A_SIZE).str() << endl;
	// ���������� �������������� ������� � ������ ���������������
	cout << ms.mergeSortInfo(ini_array, sorted_array, A_SIZE, true).str() << endl;

	delete[] ini_array;
	delete[] sorted_array;
	system("Pause");
	return 0;
}