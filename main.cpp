
#include <iostream>
#include <memory>
#include <stack>
#include <vector>
#include <random>
#include <deque>
#include <map>
#include <algorithm>
#include <ctime>

template<typename T>
class TimSort {

    static size_t GetMinrun(int n) noexcept
    {
        int r = 0;//станет 1 если среди сдвинутых битов будет хотя бы 1 ненулевой
        while (n >= 64) {
            r |= n & 1;
            n >>= 1;
        }
        return n + r;
    }

    static void insertSort(std::unique_ptr<T[]> &arr ,size_t size)noexcept{
        //todo there will be sort algorithm
    }

    static void merge(std::unique_ptr<T[]> &arr ,size_t size, std::deque< std::pair<size_t, size_t> > &stack){
        //todo merge algorithm
    }

    static void split(std::unique_ptr<T[]> &arr ,size_t size, size_t minRun, std::deque< std::pair<size_t, size_t> > &stack){
        // while{
        //todo split algorith to find one minRun
        //add <startIndex, sizeOfSubArray> into stack
        //after that check checkIsStackCorrect(stack);
        //}
        checkIsStackCorrect(stack);
    }

    static void checkIsStackCorrect(std::deque< std::pair<size_t, size_t> > &stack){
        //todo there is condition for checking stack is correct
    }

public:
    static std::unique_ptr<T[]> sort(std::unique_ptr<T[]> arr ,size_t sizeOfArr){
        if(sizeOfArr < 2){
            return arr;
        }
        if(sizeOfArr < 64){// using insertSort
            insertSort(arr, sizeOfArr);
            return arr;
        }

        std::deque< std::pair<size_t, size_t> > stack;

        size_t  minRun  = GetMinrun(sizeOfArr);

        split(arr, sizeOfArr, minRun, stack);

        merge(arr, sizeOfArr, stack);

        return arr;
    }

};



template<typename T>
bool checkIfSorted(const std::unique_ptr<T[]> arr, std::size_t size) {
    for (std::size_t i = 0; i < size-1; ++i) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}



void merge(int array[], int const left, int const mid,
           int const right)
{
    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;

    // Create temp arrays
    auto *leftArray = new int[subArrayOne],
            *rightArray = new int[subArrayTwo];

    // Copy data to temp arrays leftArray[] and rightArray[]
    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = array[left + i];
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = array[mid + 1 + j];

    auto indexOfSubArrayOne
            = 0, // Initial index of first sub-array
    indexOfSubArrayTwo
    = 0; // Initial index of second sub-array
    int indexOfMergedArray
            = left; // Initial index of merged array

    // Merge the temp arrays back into array[left..right]
    while (indexOfSubArrayOne < subArrayOne
           && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne]
            <= rightArray[indexOfSubArrayTwo]) {
            array[indexOfMergedArray]
                    = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else {
            array[indexOfMergedArray]
                    = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // left[], if there are any
    while (indexOfSubArrayOne < subArrayOne) {
        array[indexOfMergedArray]
                = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // right[], if there are any
    while (indexOfSubArrayTwo < subArrayTwo) {
        array[indexOfMergedArray]
                = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
    delete[] leftArray;
    delete[] rightArray;
}

// begin is for left index and end is
// right index of the sub-array
// of arr to be sorted */
void mergeSort(int array[], int const begin, int const end)
{
    if (begin >= end)
        return; // Returns recursively

    auto mid = begin + (end - begin) / 2;
    mergeSort(array, begin, mid);
    mergeSort(array, mid + 1, end);
    merge(array, begin, mid, end);
}

int main() {
    std::srand(time(0));


    std::size_t sizeOfElem = 10000000;
    auto arr = std::make_unique<int[]>(sizeOfElem);
    auto arrStd = new int[sizeOfElem];
    for (size_t i = 0; i < sizeOfElem; ++i) {
        arr[i] = std::rand() % 1000000000;
        arrStd[i] = std::rand() % 1000000000;
    }

    double durationMerge, durationStd;
    std::clock_t startMerge, startStd;

    startMerge = clock();
    auto sortedArr = TimSort<int>::sort(std::move(arr), sizeOfElem);
    durationMerge = ( std::clock() - startMerge ) / (double) CLOCKS_PER_SEC;

    startStd = clock();
    mergeSort(arrStd, 0, sizeOfElem);
    durationStd = ( std::clock() - startStd ) / (double) CLOCKS_PER_SEC;

    std::cout << "DurationStd = " << durationStd << "\nDurationMerge = " << durationMerge << std::endl;

    std::cout << checkIfSorted(std::move(sortedArr), sizeOfElem) << std::endl;



    return 0;
}
