#include <windows.h>
#include <iostream>
#include <vector>
#include <climits>

using namespace std;

struct ThreadData {
	int* arr;
	unsigned int size;

	int* min_out;
	int* max_out;
	double* avg_out;
};

typedef void (*FindMinMaxFunc)(int*, unsigned int, int*, int*);


DWORD WINAPI min_max(LPVOID lpParam) {

	ThreadData* data = (ThreadData*)lpParam;
	
	HMODULE hLib = LoadLibraryA("MinMaxf.dll");
	if (!hLib) {
		cerr << "can't load MinMaxF.dll\n";
		return 1;
	}

	FindMinMaxFunc find_min_max = (FindMinMaxFunc)GetProcAddress(hLib, "find_min_max");
	if (!find_min_max) {
		cerr << "can't find find_min_max\n";
		FreeLibrary(hLib);
		return 1;
	}

	find_min_max(data->arr, data->size, data->min_out, data->max_out);

	FreeLibrary(hLib);

	cout << "\nmin found: " <<*(data->min_out) << "\n";
	cout << "max found: " << *(data->max_out) << "\n";

	return 0;
}

DWORD WINAPI avg(LPVOID lpParam) {

	ThreadData* data = (ThreadData*)lpParam;

	double sum = 0;

	for (size_t i = 0; i < data->size; ++i) {

		sum += data->arr[i];

		Sleep(12);
	}
	*(data->avg_out) = sum / data->size;
	cout << "average found: " << *(data->avg_out) << "\n";

	return 0;
}

int main() {

	setlocale(LC_ALL, "Russian");

	int n;
	cout << "enter array size: ";
	cin >> n;

	if (cin.fail()) {
		cin.clear();
		cin.ignore(10000, '\n');
		cerr << "invalid input! enter a number.\n";
		return 1;
	}


	if (n <= 0) {
		cerr << "array size must be a positive number\n";
		return 1;
	}

	vector <int> arr(n);

	cout << "enter " << n << " elements:\n";
	for (int i = 0; i < n; i++) {
		cout << "  arr[" << i << "] = ";
		cin >> arr[i];

		if (cin.fail()) {
			cin.clear();
			cin.ignore(10000, '\n');
			cerr << "invalid input! enter a number.\n";
			return 1;
		}
	}

	int min_i = INT_MAX;
	int max_i = INT_MIN;
	double average = 0;

	ThreadData data;
	data.arr = arr.data();
	data.size = (unsigned int)arr.size();
	data.min_out = &min_i;
	data.max_out = &max_i;
	data.avg_out = &average;


	DWORD IDMinMax, IDAverage;

	HANDLE hMinMax = CreateThread(NULL, 0, min_max, &data, 0, &IDMinMax);
	HANDLE hAverage = CreateThread(NULL, 0, avg, &data, 0, &IDAverage);

	if (!hMinMax || !hAverage) {
		cerr << "can't create some of the two threads";

	}

	WaitForSingleObject(hMinMax, INFINITE);
	WaitForSingleObject(hAverage, INFINITE);
	
	for (size_t i = 0; i < arr.size(); i++) {
		if (arr[i] == min_i || arr[i] == max_i) {
			arr[i] = (int)average;
		}
	}

	cout << "\narray after changes:\n";
	for (size_t i = 0; i < arr.size(); i++) {
		cout << "  arr[" << i << "] = " << arr[i] << "\n";
	}
	
	CloseHandle(hMinMax);
	CloseHandle(hAverage);

	return 0;
}