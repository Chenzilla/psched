#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void merge(int a[], int first, int splitter, int last);
void mergeSort(int a[], int first, int last);
void printArray(int A[], int size);
int lw(int processors, int tasks, int taskA[]);
void reverseArray(int A[], int size);
int largestElement(int A[], int size);
int largestElementLessThan(int A[], int size, int compare);
int bw(int processors, int tasks, int taskA[]);
int arraySum(int A[], int size);
int opt(int processors, int tasks, int taskA[]);
int backtrack(int lower, int upper, int processors, int processorA[], int tasks, int taskA[]);
void pushArray(int A[], int size);

int main(int argc, char *argv[]){
  // Process the arguments
  int i, integerArgument, processors, tasks, flagged, temporaryMax; // flagged is a state of whether or not we've reached a flag argument yet.
  int taskA[argc], sortedTaskA[argc];
  flagged = tasks = 0;

  for (i = 1; i < argc; i++){
    integerArgument = atoi(argv[i]);
    // If the first argument is a positive int (as it should be) we assign it to processors
    if (integerArgument > 0 && i == 1)
      processors = atoi(argv[1]);
    // If the first argument is not a positive int, we throw an error
    else if (i == 1){
      printf("Enter a valid processor number \n");
      return 1;
    }
    // Throw error if any arguments are negative
    else if (integerArgument < 0){
      printf("None of the arguments should be negative \n");
      return 1;
    }
    // If we have a positive int and it's not the first element or after an existing flag argument, we add to our list of tasks
    else if (integerArgument > 0 && !flagged){
      taskA[i - 2] = integerArgument;
      sortedTaskA[i - 2] = integerArgument;
      tasks ++;
    }
    // If argument is 0, we need to distinguish between the int and the flags
    else if (integerArgument == 0){
      if (!strcmp(argv[i], "0") && !flagged){
        taskA[i - 2] = integerArgument;
        sortedTaskA[i - 2] = integerArgument;
        tasks ++;
      }
      // If it's a string, process for flags
      else if (strcmp(argv[i], "0")){
        if (!strcmp(argv[i], "-opt")){
          flagged = 1;
          printf("Backtracking\n");
          opt(processors, tasks, taskA);
        }
        else if (!strcmp(argv[i], "-lw")){
          flagged = 1;
          temporaryMax = lw(processors, tasks, taskA);
          printf("-lw  %d\n", temporaryMax);
        }
        else if (!strcmp(argv[i], "-lwd")){
          flagged = 1;
          mergeSort(sortedTaskA, 0, tasks - 1);
          reverseArray(sortedTaskA, tasks);
          temporaryMax = lw(processors, tasks, sortedTaskA);
          printf("-lwd %d\n", temporaryMax);
        }
        else if (!strcmp(argv[i], "-bw")){
          flagged = 1;
          temporaryMax = bw(processors, tasks, taskA);
          printf("-bw  %d\n", temporaryMax);
        }
        else if (!strcmp(argv[i], "-bwd")){
          flagged = 1;
          mergeSort(sortedTaskA, 0, tasks - 1);
          reverseArray(sortedTaskA, tasks);
          temporaryMax = bw(processors, tasks, sortedTaskA);
          printf("-bwd %d\n", temporaryMax);
        }
        else {
          printf("Flag not recognized\n");
          return 1;
        }
      }
    }
  }

  // -opt means use backtracking
  // -lw means use least-workload heuristic. Assign tasks in the order they appear on the command line, and assign each to a processor that has the least workload at the time of the assignment.
  // -lwd means use the least-workload heuristic, but first sort the tasks in decreasing order of run-time
  // -bw means use the best-workload herustic, i.e. assign tasks in the order that their run times appear and assign each to a processor, UNLESS this assignment would not increase the max current workload. In this case, assign that task to the busiest processor with this property.
  // -bwd means use the best-workload heuristic but sort first in decreasing order of run-time and then assign them in that order

  // Call printf("-FFF %d\n", maxWorkLoad); where FFF is current flag blank-padded to three characters. When multiple flags, print in order that they are specified on command line. Flags may be specified multiple times.

  // int arr[] = {12, 11, 13, 5, 6, 2, 1, 2, 7};
  // int arr_size = sizeof(arr)/sizeof(arr[0]);

  // printf("Given array is \n");
  // printArray(arr, arr_size);

  // mergeSort(arr, 0, arr_size - 1);

  // printf("\nSorted array is \n");
  // printArray(arr, arr_size);
  return 0;
}

// Sort function
void merge(int a[], int first, int splitter, int last){
  int i, j, k;
  int left_size = splitter - first + 1;
  int right_size = last - splitter;

  // Create temporary arrays
  int left_array[left_size], right_array[right_size];

  // Populate the two temporary arrays
  for (i = 0; i < left_size; i++)
    left_array[i] = a[first + i];
  for (j = 0; j < right_size; j++)
    right_array[j] = a[splitter + 1 + j];

  // Merge temporary arrays together
  i = j = 0;
  k = first;
  while (i < left_size && j < right_size){
    if (left_array[i] <= right_array[j]){
      a[k] = left_array[i];
      i++;
    }
    else {
      a[k] = right_array[j];
      j++;
    }
    k++;
  }

  // Copy remaining elements of left and right array
  while (i < left_size){
    a[k] = left_array[i];
    i++;
    k++;
  }

  while (j < right_size){
    a[k] = right_array[j];
    j++;
    k++;
  }
}

void mergeSort(int a[], int first, int last){
  if (first < last){
    int splitter = first + (last - first)/2;
    mergeSort(a, first, splitter);
    mergeSort(a, splitter + 1, last);
    merge(a, first, splitter, last);
  }
}

// Least Workload Function
int lw(int processors, int tasks, int taskA[]) {
  //   Make an array with elements = # of processors
  int processorA[processors];
  int i, j, k, lowestProcessor, lowestProcessorIndex, currentTask;
  for (i = 0; i < processors; i++)
    processorA[i] = 0;
  //   For each task in the workload array:
  //      Go through the processor array, keeping track of which index has the lowest overall workload
  //      Add the task to the index with the lowest value
  for (j = 0; j < tasks; j++){
    currentTask = taskA[j];
    for (k = 0; k < processors; k++){
      if (!j){
        lowestProcessor = currentTask;
        lowestProcessorIndex = j;
      }
      else if (processorA[k] < lowestProcessor){
        lowestProcessor = processorA[k];
        lowestProcessorIndex = k;
      }
    }
    processorA[lowestProcessorIndex] += currentTask;
    lowestProcessor = processorA[lowestProcessorIndex];
  }
  //   Sort the processor array and return the largest (last) processor value
  mergeSort(processorA, 0, processors-1);
  return processorA[processors - 1];
}

// Best Workload Function
int bw(int processors, int tasks, int taskA[]) {
  //    Make an array with elements = # of processors. Also keep track of current max time.
  int processorA[processors], processorACopy[processors];
  int i, j, k, l, lowestProcessor, lowestProcessorIndex, bestProcessorIndex, currentTask, maxProcessor;
  for (i = 0; i < processors; i++)
    processorA[i] = 0;
  //    For each task in the workload array:
  for (j = 0; j < tasks; j++){
  //        Go through the processor array, keeping track of which index has the lowest overall workload
    currentTask = taskA[j];
    maxProcessor = largestElement(processorA, processors);
    for (k = 0; k < processors; k++){
      if (!j){
        lowestProcessor = currentTask;
        lowestProcessorIndex = 0;
      }
      else if (processorA[k] < lowestProcessor){
        lowestProcessor = processorA[k];
        lowestProcessorIndex = k;
      }
    }
    lowestProcessor = processorA[lowestProcessorIndex];
  //        Make copy of the processors array with current task time added to each processor time
    for (l = 0; l < processors; l++)
      processorACopy[l] = processorA[l] + currentTask;
  //        Add the task to the copy[index] with the lowest value
  //        If the new max time is > the old max time
    if (maxProcessor < lowestProcessor + currentTask){
      processorA[lowestProcessorIndex] += currentTask;
      lowestProcessor = processorA[lowestProcessorIndex];
    }
  //            Add this task to the actual processor array
  //            Continue
  //        Else
  //            Add the current task to all indices and see which index is the largest value while being <= the previous max time
  //            Add the task to the actual processor array
    else {
      bestProcessorIndex = largestElementLessThan(processorACopy, processors, maxProcessor);
      processorA[bestProcessorIndex] += currentTask;
      lowestProcessor = processorA[bestProcessorIndex];
    }
  }
  //    Sort the processor array and return the largest workload
  mergeSort(processorA, 0, processors-1);
  return processorA[processors - 1];
}

// Backtracking function
int opt(int processors, int tasks, int taskA[]){
  // Sort workload by decreasing order in order to compute the intial upper bound using -lwd
  int i, j, upperBound, lowerBound, sum, temp;
  int processorA[processors];


  for (j = 0; j < processors; j++)
    processorA[j] = 0;

  mergeSort(taskA, 0, tasks - 1);
  reverseArray(taskA, tasks);
  upperBound = lw(processors, tasks, taskA);

  sum = arraySum(taskA, tasks);

  // Compute lower bound on max workload by adding the run-times of the individual tasks and divide by number of processors (round up to nearest int)
  lowerBound = sum/processors + (sum % processors != 0);
  //    Return the resulting minimum workload
  temp = backtrack(lowerBound, upperBound, processors, processorA, tasks, taskA);
  printf("%d is the answer", temp);
  return temp;
}

int backtrack(int lower, int upper, int processors, int processorA[], int tasks, int taskA[]) {
//    If workload[] is empty
//        Return the max workload for this assignment or upper, whatever is smaller
  int largest, i, j, backtrackLower;
  int tempTaskA[tasks];
  if (tasks == 0){
    largest = largestElement(processorA, processors);
    printf("There are no more tasks to assign: we return the smallest of %d and %d\n", upper, largest);
    return (largest < upper) ? largest : upper;
  }
  else {
    // Create a task array with first element pushed off
    for (i = 0; i < tasks; i++)
      tempTaskA[i] = taskA[i];
    pushArray(tempTaskA, tasks);

    for (j = 0; j < processors; j++){
      printf("There are %d tasks left\n", tasks);
      printf("This is what the processors look like before we add %d\n", taskA[0]);
      printArray(processorA, processors);
      printf("We add %d to index %d, which gives the array below\n", taskA[0], j);
      //            Assign next task in workload[] to the current processor[index]
      //            Set this new processor[index] as the current_processor_time
      processorA[j] += taskA[0];
      printArray(processorA, processors);
      //            If the current_processor_time == previous_processor_time, break
      if (j > 0 && (processorA[j] == processorA[j - 1] + taskA[0])){
        printf("continue\n");
        processorA[j] -= taskA[0];
        continue;
      }
      else if (processorA[j] > upper){
        printf("skip\n");
        processorA[j] -= taskA[0];
        continue;
      }
      //            Else
      //                backtrack(int lower, int upper, int new_processor[], int new_workload[])
      else {
        backtrackLower = backtrack(lower, upper, processors, processorA, tasks - 1, tempTaskA);
        if (backtrackLower < upper)
          upper = backtrackLower;
      }
      // Deassign task from the current processor[index]
      processorA[j] -= taskA[0];
    }
  }
  return upper;
}

// Utility Functions
// Function to print an array
void printArray(int A[], int size){
  int i;
  for (i = 0; i < size; i++)
    printf("%d ", A[i]);
  printf("\n");
}

// Functionto reverse an array
void reverseArray(int A[], int size){
  int i, j;
  int tempA[size];
  for (j = 0; j < size; j++)
    tempA[j] = A[j];
  for (i = 0; i < size; i++){
    A[i] = tempA[size - i - 1];
  }
}

// Function that returns largest element of an array
int largestElement(int A[], int size){
  int i, currentMax;
  currentMax = 0;
  for (i = 0; i < size; i++){
    if (A[i] > currentMax)
      currentMax = A[i];
  }
  return currentMax;
}

// Function that returns largest element in an array that is smaller than a given value
int largestElementLessThan(int A[], int size, int compare){
  int i, currentMax, maxIndex;
  currentMax = 0;
  for (i = 0; i < size; i++){
    if (A[i] > currentMax && A[i] <= compare){
      currentMax = A[i];
      maxIndex = i;
    }
  }
  return maxIndex;
}

// Function that returns sum of all elements in an array
int arraySum(int A[], int size){
  int i, sum;
  sum = 0;
  for (i = 0; i < size; i++)
    sum += A[i];
  return sum;
}

// Function that returns array without first element
void pushArray(int A[], int size){
  int i, j;
  int B[size];
  for (i = 0; i < size; i++)
    B[i] = A[i];
  for (j = 0; j < size - 1; j++)
    A[j] = B[j + 1];
}
