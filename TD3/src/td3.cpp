#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include "../include/ScopedTimer.hpp" // for the chronometer structure

std::vector<int> generate_random_vector(size_t const size, int const max = 100) {
    std::vector<int> vec(size);
    std::generate(vec.begin(), vec.end(), [&max]() { return std::rand() % max;} );
    return vec;
}

bool is_sorted(std::vector<int> const& vec) 
{ 
    return std::is_sorted(vec.begin(), vec.end()); 
}

// BUBBLE SORT :
void swap(std::vector<int> &vec, const int &count)
{
    for (int i = 0; i < vec.size()-1-count; i++)
    {
        if (vec[i] > vec[i + 1])
        {
            int buffer = vec[i + 1];
            vec[i + 1] = vec[i];
            vec[i] = buffer;
        }
    }
}

void bubble_sort(std::vector<int> &vec)
{
    int count {0};

    while (!is_sorted(vec))
    {
        swap(vec, count);
        count += 1;
    }
}

// MERGE SORT :
void merge_sort_merge(std::vector<int> &vec, size_t const left, size_t const middle, size_t const right)
{
    size_t left_size {middle - left};
    size_t right_size {right - middle};

    std::vector<int> left_array(left_size);
    std::vector<int> right_array(right_size);

    for (int i {0}; i < left_size ; i++)
    {
        left_array[i] = vec[left + i];
    }

    for (int i {0}; i < right_size ; i++)
    {
        right_array[i] = vec[middle + i];
    }

    size_t i {0}, j {0};
    size_t k {left};

    while (i < left_size && j < right_size)
    {
        if (left_array[i] <= right_array[j])
        {
            vec[k] = left_array[i];
            i++;
        }
        else
        {
            vec[k] = right_array[j];
            j++;
        }
        
        k++;
    }

    while (i < left_size)
    {
        vec[k] = left_array[i];
        k++;
        i++;
    }

    while (j < right_size)
    {
        vec[k] = right_array[j];
        k++;
        j++;
    }
}

void merge_sort(std::vector<int> & vec, size_t const left, size_t const right)
{
    if (right - left <= 1) // base case : vec contains only one element or is empty
    {
        return; // since merge_sort doesn't return anything
    }
    else // recursive case
    {
        size_t middle = left + (right - left) / 2;

        merge_sort(vec, left, middle); // left array
        merge_sort(vec, middle, right); // right array

        merge_sort_merge(vec, left, middle, right);
    }
}

void merge_sort(std::vector<int> &vec) 
{
    merge_sort(vec, 0, vec.size());
}



void print_vec(const std::vector<int> &vec)
{
    for (int value : vec)
    {
        std::cout << value << std::endl;
    }
}

int search(const std::vector<int> &vec, const int v)
{
    /*
        pre-conditions : vec must be sorted (asc)
    */

    int left {0};
    int right {vec.size() - 1};
    int middle {(left + right)/2};

    while (left <= right)
    {
        if (v > vec[middle])
        {
            left = middle +1;
        }
        else
        {
            right = middle - 1;
        }

        for (int i {left} ; i <= right ; i++)
        {
            if (vec[i] == v)
            {
                return i;
            }
        }
        middle = (left + right)/2;
    }

    return -1; // value v not found in the array
}

int main()
{
    std::vector<int> reversed_sorted {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    std::vector<int> smaller_reversed_sorted {5, 4, 3, 2, 1};
    std::vector<int> empty {};
    std::vector<int> one_element {67};
    std::vector<int> duplicates {10, 9, 8, 7, 10, 5, 4, 3, 3, 1};
    std::vector<int> search1 {1, 2, 2, 3, 4, 8, 12};
    std::vector<int> search2 {1, 2, 3, 3, 6, 14, 12, 15};
    std::vector<int> search3 {2, 2, 3, 4, 5, 8, 12, 15, 16};
    std::vector<int> search4 {5, 6, 7, 8, 9, 10, 11, 12, 13};
    std::vector<int> search5 {1, 2, 3, 4, 5, 6, 7, 8, 9};

    // BUBBLE SORT - TEST - reversed sorted
    {
        std::vector<int> array = reversed_sorted;
        //std::cout << "bubble :" << std::endl;
        ScopedTimer timer("bubble sort with reversed sorted array (n=10)");
        bubble_sort(array);
        //print_vec(array);
    }

    // BUBBLE SORT - TEST - empty
    {
        std::vector<int> array = empty;
        //std::cout << "bubble :" << std::endl;
        ScopedTimer timer("bubble sort with empty array (n=0)");
        bubble_sort(array);
        //print_vec(array);
    }

    // BUBBLE SORT - TEST - one element
    {
        std::vector<int> array = one_element;
        //std::cout << "bubble :" << std::endl;
        ScopedTimer timer("bubble sort with one element array (n=1)");
        bubble_sort(array);
        //print_vec(array);
    }

    // BUBBLE SORT - TEST - smaller reversed sorted
    {
        std::vector<int> array = smaller_reversed_sorted;
        //std::cout << "bubble :" << std::endl;
        ScopedTimer timer("bubble sort with smaller reversed sorted array (n=5)");
        bubble_sort(array);
        //print_vec(array);
    }
    
    // MERGE SORT - TEST - reversed sorted
    {
        std::vector<int> array = reversed_sorted;
        //std::cout << "merge : " << std::endl;
        ScopedTimer timer("merge sort with reversed sorted array (n=10)");
        merge_sort(array);
        //print_vec(array);
    }

    // MERGE SORT - TEST - smaller reversed sorted
    {
        std::vector<int> array = smaller_reversed_sorted;
        //std::cout << "merge : " << std::endl;
        ScopedTimer timer("bubble sort with smaller reversed sorted array (n=5)");
        merge_sort(array);
        //print_vec(array);
    }
    
    // STANDARD SORT
    {
        std::vector<int> array = reversed_sorted;
        ScopedTimer timer("standard sort with reversed sorted array (n=10)");
        std::sort(array.begin(), array.end());
    }

    std::cout << "\n" << std::endl;
    std::cout << "dichotomy indexes :" << std::endl;

    // DICHOTOMY SEARCH 1
    {
        std::vector<int> array = search1;
        std::cout << search(array, 8) << std::endl;
    }

    // DICHOTOMY SEARCH 2
    {
        std::vector<int> array = search2;
        std::cout << search(array, 15) << std::endl;
    }

    // DICHOTOMY SEARCH 3
    {
        std::vector<int> array = search3;
        std::cout << search(array, 16) << std::endl;
    }

    // DICHOTOMY SEARCH 4
    {
        std::vector<int> array = search4;
        std::cout << search(array, 6) << std::endl;
    }

    // DICHOTOMY SEARCH 5
    {
        std::vector<int> array = search5;
        std::cout << search(array, 10) << std::endl;
    }

    return 0;
}