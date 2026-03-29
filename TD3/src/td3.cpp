#include <vector>
#include <algorithm>
#include <iostream>

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
    if (right - left <= 1) // cas de base : vec contient un seul élément ou est vide
    {
        return; // comme merge_sort ne renvoie rien
    }
    else // cas récursif
    {
        size_t middle = left + (right - left) / 2;

        merge_sort(vec, left, middle); // tableau de gauche
        merge_sort(vec, middle, right); // tableau de droite

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

int main()
{
    // TESTS : 
    std::vector<int> array {9, 8, 7, 6, 5, 4, 3, 2, 1};

    //BUBBLE SORT - TEST :
    std::cout << "bubble :" << std::endl;
    bubble_sort(array);
    print_vec(array);

    //MERGE SORT - TEST :
    array = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    std::cout << "merge : " << std::endl;
    merge_sort(array);
    print_vec(array);

    return 0;
}