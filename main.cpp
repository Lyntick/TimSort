
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

    static void insertSort(std::unique_ptr<T[]> &arr ,size_t left, size_t right)noexcept{//including the right element!
        for (int i = left + 1; i <= right; i++)
        {
            int temp = arr[i];
            int j = i - 1;
            while (j >= left && arr[j] > temp)
            {
                arr[j+1] = arr[j];
                j--;
            }
            arr[j+1] = temp;
        }
    }


    static void merge(std::unique_ptr<T[]> &arr ,std::deque< std::pair<size_t, size_t> > &stack){
//        for(auto iter = stack.rbegin(); iter != stack.rend() - 1; ++iter){
//            merge(arr, *(iter + 1), *iter);
//            //todo there is  a problem with merge
//            auto tempPair = iter;
//            (iter+1)->second += tempPair->second;
//        }
        for(int  iter = stack.size() - 1; iter  > 0; --iter){
            if(iter ==0){
                return;
            }
            merge(arr,stack[iter-1], stack[iter]);
            auto tempPair = stack[iter];
            stack[iter-1].second += tempPair.second;

        }
    }

    static void merge(std::unique_ptr<T[]> &arr ,std::pair<size_t, size_t> &left, std::pair<size_t, size_t> &right){// including right
        //todo maybe problem
        auto minArray = std::make_unique<T[]>(left.second);
        for(size_t i = left.first, j = 0; i <  left.first + left.second; ++i, ++j){
            minArray[j] = arr[i];
        }

        size_t leftIndex = 0, rightIndex = right.first, arrIndex = left.first;

        while(leftIndex < left.second && rightIndex <  right.first+right.second){
            if(minArray[leftIndex] <= arr[rightIndex]){
                arr[arrIndex] = minArray[leftIndex];
                ++leftIndex;
                ++arrIndex;
            }else{
                arr[arrIndex] = arr[rightIndex];
                ++rightIndex;
                ++arrIndex;
            }
        }

        while(leftIndex < left.second){
            arr[arrIndex] = minArray[leftIndex];
            ++leftIndex;
            ++arrIndex;
        }

        while(rightIndex < right.first+right.second){
            arr[arrIndex] = arr[rightIndex];
            ++rightIndex;
            ++arrIndex;
        }
    }

    static void split(std::unique_ptr<T[]> &arr ,size_t size, size_t minRun, std::deque< std::pair<size_t, size_t> > &stack){
        size_t startRun = -1, endRun = -1, lengthOfRun = 0;
        for(size_t ptr = 0; ptr < size; ++ptr){
            startRun = ptr;//first elements of run
            size_t subPtr  = 1;//counting length
            while( ptr < size  && subPtr < minRun){
                ++ptr;
                ++subPtr;
            }
            endRun = ptr;
            lengthOfRun = subPtr;
            if(lengthOfRun != 0)
                insertSort(arr, startRun, endRun);//including lengthOfRun
            stack.emplace_back(std::pair<size_t, size_t>(startRun, lengthOfRun));//add to stack
            checkIsStackCorrect(arr, size, stack);
        }

    }

    static void checkIsStackCorrect(std::unique_ptr<T[]> &arr ,size_t size, std::deque< std::pair<size_t, size_t> > &stack){
        if(stack.size() == 1){
            return;
        }

        for(int  iter = stack.size() - 1; iter  >= 0; --iter){
            if(iter >= 2){//stack has 3 elem or more
                if(stack[iter].second + stack[iter - 1].second >= stack[iter - 2].second || stack[iter].second >= stack[iter - 1].second){
                    if(stack[iter].second <= stack[iter -2].second){//merge stack[iter-1] with stack[iter] there is founded min(run)
                        merge(arr,stack[iter-1], stack[iter]);
                        auto tempPair = stack[iter];
                        stack[iter-1].second += tempPair.second;
                        stack.erase(std::find(stack.begin(), stack.end(), tempPair));
                    }else{ //merge stack[iter-1] with stack[iter -2]
                        merge(arr, stack[iter - 2], stack[iter -1]);
                        auto tempPair = stack[iter-1];
                        stack[iter-2].second += tempPair.second;
                        stack.erase(std::find(stack.begin(), stack.end(), tempPair));
                    }
                }
            }else if(iter == 1){//stack has 2 elem
                if(stack[iter].second >= stack[iter - 1].second){
                    merge(arr, stack[iter - 1], stack[iter]);

                    auto tempPair = stack[iter];
                    stack[iter-1].second += tempPair.second;
                    stack.erase(std::find(stack.begin(), stack.end(), tempPair));
                }
            }
        }

    }

public:
    static std::unique_ptr<T[]> sort(std::unique_ptr<T[]> arr ,size_t sizeOfArr){
        if(sizeOfArr < 1){
            return arr;
        }
        if(sizeOfArr < 64){// using insertSort
            insertSort(arr, 0, sizeOfArr - 1);
            return arr;
        }

        std::deque< std::pair<size_t, size_t> > stack;//store first elements of the Run  and length of the Run

        size_t  minRun  = GetMinrun(sizeOfArr);

        split(arr, sizeOfArr, minRun, stack);

        merge(arr,stack);//merge whole array

        return arr;
    }

};




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

template<typename T>
bool checkIfSorted(const std::unique_ptr<T[]> arr, std::size_t size) {
    for (std::size_t i = 0; i < size-1; ++i) {
        if (arr[i] >  arr[i + 1]) {
            return false;
        }
    }
    return true;
}

int main() {
    std::srand(time(0));


    std::size_t sizeOfElem = 1000000;
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

//    for(size_t i = 0; i < sizeOfElem; ++i){
//        std::cout << sortedArr[i] << '\n';
//    }

    std::cout << "\nDurationStd = " << durationStd << "\nDurationTimSort = " << durationMerge << std::endl;

    std::cout << checkIfSorted(std::move(sortedArr), sizeOfElem) << std::endl;



    return 0;
}
