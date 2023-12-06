// Practice writing a function to find a max value

#include <cs50.h>
#include <limits.h>
#include <stdio.h>

int max(int array[], int n);

int main(void)
{
    int n;
    do
    {
        n = get_int("Number of elements: ");
    }
    while (n < 1);

    int arr[n];

    for (int i = 0; i < n; i++)
    {
        arr[i] = get_int("Element %i: ", i);
    }

    printf("The max value is %i.\n", max(arr, n));
}

int max(int array[], int n)
{
    int max_value = INT_MIN;

    for (int i = 0; i < n; i++)
    {
        int current_element = array[i];
        if (current_element > max_value)
        {
            max_value = current_element;
        }
    }

    return max_value;
}
