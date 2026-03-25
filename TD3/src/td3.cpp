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
void merge_sort_merge(std::vector<int> & vec, size_t const left, size_t const middle, size_t const right)
{
    std::vector<int> left_array {};
    std::vector<int> right_array {};

    for (int i {0}; i < middle-left ; i++)
    {
        left_array[i] = vec[left + i];
    }

    for (int i {0}; i < right-middle ; i++)
    {
        right_array[i] = vec[middle + i];
    }

    
}

void merge_sort(std::vector<int> & vec, size_t const left, size_t const right)
{
    if (left >= right) // cas de base : vec contient un seul élément ou est vide
    {
        return; // comme merge_sort ne renvoie rien
    }
    else // cas récursif
    {
        int middle {(left + (right-left))/2};

        merge_sort(vec, left, middle); // tableau de gauche
        merge_sort(vec, middle, right); // tableau de droite

        merge_sort_merge(vec, left, middle, right);
    }
}

void merge_sort(std::vector<int> & vec) 
{
    merge_sort(vec, 0, vec.size() - 1);
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
    std::vector<int> array {1, 2, 3, 4, 5, 6, 7, 8, 9};

    //BUBBLE SORT - TEST :
    bubble_sort(array);
    print_vec(array);

    return 0;
}