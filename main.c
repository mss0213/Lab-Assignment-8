#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* This program was written by Michael Schlosbon on 3/15/2024. The purpose was to develop a merge sort algorithm that would sort an array of integers in ascending order. */

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
  extraMemoryAllocated += sz;
  size_t* ret = malloc(sizeof(size_t) + sz);
  *ret = sz;
  printf("Extra memory allocated, size: %ld\n", sz);
  return &ret[1];
}

void DeAlloc(void* ptr)
{
  size_t* pSz = (size_t*)ptr - 1;
  extraMemoryAllocated -= *pSz;
  printf("Extra memory deallocated, size: %ld\n", *pSz);
  free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
  return ((size_t*)ptr)[-1];
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated

void mergeSort(int pData[], int l, int r) {  
  if (r <= l) {
    return;
  }

  int m = (l + r) / 2; // Finds the midpoint
  int n1 = m - l + 1; // Finds the size of the left array
  int n2 = r - m; // Finds the size of the right array

  mergeSort(pData, l, m); // Recursive function that sorts the left array
  mergeSort(pData, m + 1, r); // Recursive function that sorts the right array (starts one after midpoint because left goes up to midpoint)

  int* L = (int*)Alloc(sizeof(int) * n1);
  int* R = (int*)Alloc(sizeof(int) * n2);

  // Copy data to temp arrays L[] and R[]
  for (int i = 0; i < n1; i++) {
    L[i] = pData[l + i];
  }

  for (int j = 0; j < n2; j++) {
    R[j] = pData[m + 1 + j];
  }

  // Merge the temp arrays back into pData[l..r]
  int i = 0; // Initial index of first subarray
  int j = 0; // Initial index of second subarray
  int k = l; // Initial index of merged subarray

  while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
      pData[k] = L[i]; // Copies the element from the left array to the main array
      i++;
    } else {
      pData[k] = R[j]; // Copies the element from the right array to the main array
      j++;
    }
    k++;
  }

  // Copy the remaining elements of L[], if any
  while (i < n1) {
    pData[k] = L[i];
    i++;
    k++;
  }

  // Copy the remaining elements of R[], if any
  while (j < n2) {
    pData[k] = R[j];
    j++;
    k++;
  }

  DeAlloc(L); // Frees the left array
  DeAlloc(R); // Frese the right array
}


// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
  {
  FILE* inFile = fopen(inputFileName,"r");
  int dataSz = 0;
  int i, n, *data;
  *ppData = NULL;

  if (inFile)
  {
    fscanf(inFile,"%d\n",&dataSz);
    *ppData = (int *)malloc(sizeof(int) * dataSz);
    // Implement parse data block
    if (*ppData == NULL)
    {
      printf("Cannot allocate memory\n");
      exit(-1);
    }
    for (i=0;i<dataSz;++i)
    {
      fscanf(inFile, "%d ",&n);
      data = *ppData + i;
      *data = n;
    }

    fclose(inFile);
  }

  return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
  int i, sz = dataSz - 100;
  printf("\tData:\n\t");
  for (i=0;i<100;++i)
  {
    printf("%d ",pData[i]);
  }
  printf("\n\t");

  for (i=sz;i<dataSz;++i)
  {
    printf("%d ",pData[i]);
  }
  printf("\n\n");
}

int main(void)
{
  clock_t start, end;
  int i;
    double cpu_time_used;
  char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };

  for (i=0;i<4;++i)
  {
    int *pDataSrc, *pDataCopy;
    int dataSz = parseData(fileNames[i], &pDataSrc);

    if (dataSz <= 0)
      continue;

    pDataCopy = (int *)malloc(sizeof(int)*dataSz);

    printf("---------------------------\n");
    printf("Dataset Size : %d\n",dataSz);
    printf("---------------------------\n");

    printf("Merge Sort:\n");
    memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
    extraMemoryAllocated = 0;
    start = clock();
    mergeSort(pDataCopy, 0, dataSz - 1);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
    printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

    free(pDataCopy);
    free(pDataSrc);
  }
}
