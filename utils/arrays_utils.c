#include "arrays_utils.h"

bool is_array_true(int* arr, int len)
{
    int i;

    for (i = 0; i < len; i++)
    {
        if (arr[i] == 0)
            return false;
    }

    return true;
}