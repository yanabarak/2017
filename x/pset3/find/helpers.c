/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    int max = n-1;
    int min = 0;
    
    while (max >= min)
    {
        int midpoint = (min + max)/2;
        if(values[midpoint] <  value)
          min = midpoint + 1;
        else if (values[midpoint] > value )
          max = midpoint - 1;
        else
          return true;
    }
    return false;
}

/**
 * Sorts array of n values.
 */
 // Insertion sorting
void sort(int values[], int n)
{
    int element;
    for(int i = 0; i < n; i++)
    {
        element = values[i];
        int j = i;
        while((j > 0) && (values[j - 1] > element))
        {
           values[j] = values[j - 1];
           j = j - 1; 
        }
        values[j] = element;
    }
    return;
}