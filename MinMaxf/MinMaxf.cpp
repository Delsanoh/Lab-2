#include "pch.h"
#include "MinMaxF.h"
#include <windows.h>
#include <climits>


extern "C" MINMAXF_API void find_min_max(int* arr, unsigned int size, int* min_out, int* max_out) {
    int min_val = INT_MAX;
    int max_val = INT_MIN;

    for (unsigned int i = 0; i < size; i++) {
        if (arr[i] < min_val) min_val = arr[i];
        Sleep(7);

        if (arr[i] > max_val) max_val = arr[i];
        Sleep(7);
    }

    *min_out = min_val;
    *max_out = max_val;
}