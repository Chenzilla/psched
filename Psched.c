#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

void merge(int a[], int first, int splitter, int last);
void mergeSort(int a[], int first, int last);
void printArray(int A[], int size);

int main(int argc, char *argv[]) {
  // Process the arguments, use isalpha() and isdigit()

  // -opt means use backtracking
  // -lw means use least-workload heuristic. Assign tasks in the order they appear on the command line, and assign each to a processor that has the least workload at the time of the assignment.
  // -lwd means use the least-workload heuristic, but first sort the tasks in decreasing order of run-time
  // -bw means use the best-workload herustic, i.e. assign tasks in the order that their run times appear and assign each to a processor, UNLESS this assignment would not increase the max current workload. In this case, assign that task to the busiest processor with this property.
  // -bwd means use the best-workload heuristic but sort first in decreasing order of run-time and then assign them in that order

  // Call sort on the runtimes if flag requires it

  // Call backtracking, least-workload, or best-workload, depending on flags

  // Call printf("-FFF %d\n", maxWorkLoad); where FFF is current flag blank-padded to three characters. When multiple flags, print in order that they are specified on command line. Flags may be specified multiple times.

  int arr[] = {12, 11, 13, 5, 6, 2, 1, 2, 7};
  int arr_size = sizeof(arr)/sizeof(arr[0]);

  printf("Given array is \n");
  printArray(arr, arr_size);

  mergeSort(arr, 0, arr_size - 1);

  printf("\nSorted array is \n");
  printArray(arr, arr_size);
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
// int[] lw(int processors, int workloads[]) {
//   Make an array with elements = # of processors
//   For each task in the workload array:
//      Go through the processor array, keeping track of which index has the lowest overall workload
//      Add the task to the index with the lowest value
//   Sort the processor array and return the largest workload
// }

// Best Workload Function
// int[] bw(int processors, int workloads[]) {
//    Make an array with elements = # of processors. Also keep track of current max time.
//    For each task in the workload array:
//        Go through the processor array, keeping track of which index has the lowest overall workload
//        Make copy of the processors array
//        Add the task to the copy[index] with the lowest value
//        If the new max time is > the old max time
//            Add this task to the actual processor array
//            Continue
//        Else
//            Add the current task to all indices and see which index is the largest value while being <= the previous max time
//            Add the task to the actual processor array
//    Sort the processor array and return the largest workload
//}

// Backtracking function
// int[] opt(int processors, int workloads[]) {
//    Sort workload by decreasing order
//    Compute lower bound on max workload by adding the run-times of the individual tasks and divide by number of processors (round up to nearest int)
//    Run lw() on the workloads, and set the resulting max workload as the current smallest workload, i.e. anything that returns larger than this is diregarded
//    Run backtrack(lower, upper, int processors[], int workloads[])
//    Return the resulting minimum workload
//}

// int[] backtrack(int lower, int upper, int processors[], int workload[]) {
//    If workload[] is empty
//        Return the max workload for this assignment or upper, whatever is smaller
//    Else
//        For each processor:
//            Assign next task in workload[] to the current processor[index]
//            Set this new processor[index] as the current_processor_time
//            If the current_processor_time == previous_processor_time
//                BREAK (get out of if block)
//            Else
//                backtrack(int lower, int upper, int new_processor[], int new_workload[])
//            Deassign task from the current processor[index]
//            Set the upper to min of backtrack or upper
//    Return upper
//}
//

// Utility Functions
// Function to print an array
void printArray(int A[], int size)
{
  int i;
  for (i=0; i < size; i++)
    printf("%d ", A[i]);
  printf("\n");
}
