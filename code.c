#include <immintrin.h>
#include <stdio.h>
#include "code.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/un.h>
#include <time.h>

struct sockaddr_un address;
socklen_t address_length;


void initializeSocket() {
  // Initialize server
  socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (socket_fd < 0){
    printf("socket() failed\n");
    exit(1);
  }
  unlink("/tmp/soc");
  memset(&address, 0, sizeof(struct sockaddr_un));
  address.sun_family = AF_UNIX;
  snprintf(address.sun_path, UNIX_PATH_MAX, "/tmp/soc");
  if (bind(socket_fd, (struct sockaddr *) &address, sizeof(struct sockaddr_un)) != 0)
  {
      printf("bind() failed\n");
      exit(1);
  }
  if(listen(socket_fd, 5) != 0)
  {
      printf("listen() failed\n");
      exit(1);
  }
  address_length = sizeof(address);
  connection = (connection_fd = accept(socket_fd, (struct sockaddr *) &address, &address_length));
  printf("successfully received a connection\n");
}

int getOneValue(int *gets) {
  bzero(buffer,128);
  n = read(connection_fd,buffer,127);
  if (buffer[0] == 'g') {
    buffer[0] = ' ';
    *gets = 1;
  }
  long returnedLong = strtol(buffer, &stopstring, 10);
  return (int)returnedLong;
}

void sendOneValue(int value) {
  bzero(buffer,128);
  snprintf(buffer, sizeof buffer, "%i*", value);
  n = write(connection_fd, buffer, strlen(buffer));
}

void insertOneInteger(int *list, int value) {
  int i;
  for (i = (currentLength*8)-1; i >= 0; i--) {
    if (value > list[i]) {
      int temp = list[i];
      list[i] = value;
      value = temp;
    }
  }
}

void storeValues() {
  //printf("Store values\n");
  if (currentRow == 10) {
    struct timeval tv;
    //printf("here 10!\n");
    int newLength = currentLength * 10;
    int *newVec = (int*)calloc(newLength*8, sizeof(int));
    //printf("Pointer %ld\n", sizeof(newVec));
    gettimeofday(&tv,NULL);
    unsigned long startTime = 1000000 * tv.tv_sec + tv.tv_usec;
    // sort and merge
    simpleColumnSortHigh(allData, newVec, currentLength);
    //combineArrays(allData, newVec);
    gettimeofday(&tv,NULL);
    unsigned long endTime = 1000000 * tv.tv_sec + tv.tv_usec;
    printf("SortTime %lu\n", endTime-startTime);
    printf("r %i, c %i, = %i\n", currentRow, currentLength, currentRow*currentLength*8);
    int i;
    for (i = 0; i < 10; i++) {
      free(allData[i]);
    }
    //printf("freed data\n");
    allData[0] = newVec;
    currentRow = 1;
    currentLength = newLength;
  }
}

void getDataFromClient() {
  //printf("Here in getDataFromClient\n");
  int totalLength = currentLength * 8;
  int *list = (int*)calloc(totalLength, sizeof(int));
  int i = 0;
  while (i < totalLength) {
    int newValue = rand();
    //printf("%i, ", newValue);
    int gets = 0;
    //int newValue = getOneValue(&gets);
    if (gets) {
      // this is where we would get a value to return it to the client.
      int found = getValue(list, newValue);
      sendOneValue(found);
    }
    else {
      insertOneInteger(list, newValue);
      i++;
    }
  }
  allData[currentRow] = list;
  currentRow++;
  storeValues();
  //printf("\nend\n");
  //printVec();
}

void createDatabase() {
  //printf("here 1\n");
  getDataFromClient();
  //printf("here 1.1 %i %i\n", currentLength, currentRow);
}

int getValue(int *list, int value) {
  int i;
  for (i = 0; i < currentRow; i++) {
	  int j;
  	for (j = 0; j < currentLength*8; j++) {
	    if (allData[i][j] == value) {
		    return value;
		  }
  		if (allData[i][j] > value) {
  		  // skip to the next row
  		  j = 8*currentLength;
  		}
	  }
  }
  for (i = (currentLength*80)-1; i >= 0; i--) {
    if (list[i] < value) {
      i = -1;
    }
    if (list[i] == value) {
      return list[i];
    }
  }
  return 0;
}

void printVec(){
  int j;
  int i;
  printf("current %i\n", currentRow);
  for (i = 0; i < currentRow; i++) {
    for (j = 0; j < currentLength*8; j++) {
      printf("%i ", allData[i][j]);
    }
    printf("\n");
  }
}

void simpleColumnSortHigh(int **input, int *output, int length) {
  // input[numSets][length][8]
  //int* f = (int*)&result[0];
  //printf("Here in sort\n");
  //__m256i output[length * 10];
  int numSets = 10; // Must change for loop too
  int numToMove = (8 * numSets * length)-1;
  int numMoved = 0;
  int insertIndex = 7;
  //printVec();
  //printf("making litss\n");
  __m256i data[numSets*length];
  int r;
  //printf("made lists %ld\n", sizeof(data));
  int c;
  for (r = 0; r < numSets; r++) {
    //__m256i *tempV = (__m256i*)calloc(length, sizeof(__m256i));
    for (c = 0; c < length; c++) {
      int end = c*8;
      //printf("here %i %i\n", r, c);
      data[r*length+c] = _mm256_setr_epi32(input[r][end],input[r][end+1],input[r][end+2],input[r][end+3],input[r][end+4],input[r][end+5],input[r][end+6],input[r][end+7]);
      //printf("here too %i %i\n", r, c);
      //printf("ok %i %i\n", r, c);
    }
  }
  while (numToMove >= 0) {
    int i;
    for (i = 0; i < length; i++) {
      //sort 1
      int s;
      for (s = numSets-1; s > 0; s--) {
        __m256i tempVector = _mm256_max_epi32(data[(s-1)*length+i], data[s*length+i]);
        data[s*length+i] = _mm256_min_epi32(data[(s-1)*length+i], data[s*length+i]);
        data[(s-1)*length+i] = tempVector;
      }
    }
    // Items are now sorted with highest on top
    // Take 1
    int maxValue = _mm256_extract_epi32(data[length-1], 7);
    //printf("maxValue %i \n", maxValue);
    output[numToMove] = maxValue;
    //output[numToMove / 8] = _mm256_insert_epi32(output[numToMove / 8], maxValue, insertIndex);
    numMoved++;
    numToMove--;
    insertIndex--;
    if (insertIndex < 0) {
      insertIndex = 7;
    }
    // shift
    data[length-1] = _mm256_insert_epi32(_mm256_slli_si256(data[length-1], 4), _mm256_extract_epi32(data[length-1], 3), 4);
    // we already did input[0][0]
    for (i = length-2; i >= 0; i--) {
      int extractedValue = _mm256_extract_epi32(data[i], 7);
      // insert it into i-1 column 0
      data[i+1] = _mm256_insert_epi32(data[i+1], extractedValue, 0);
      // now shift input[0][i]
      data[i] = _mm256_insert_epi32(_mm256_slli_si256(data[i], 4), _mm256_extract_epi32(data[i], 3), 4);
    }
    /*if ((numMoved % (numSets * 8)) == 0) {
      length--;
      // We want to not sort lists when they are empty
      // but we don't do this if we are using the double sided approach
      // Other idea is to resort only when input[0][0][7] < input[0][1][7]
      // but we'd have to sort 2+ times and not take more than recent sorts 
    }*/
  }
  //printf("Done sort\n");
}

// Derrived from http://en.cppreference.com/w/c/algorithm/qsort
int comparator(const void* a, const void* b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

void combineArrays(int **input, int *output) {
  int numSets = 10;
  int totalLength = currentLength*8;
  int i;
  int j;
  for (i = 0; i < numSets; i++) {
    for (j = 0; j < totalLength; j++) {
      output[i*totalLength+j] = input[i][j];
    }
  }
  totalLength = numSets*totalLength;
  qsort(output, totalLength, sizeof(int), comparator);
}

int main() {
  unsigned long st = time(NULL);
  srand(time(NULL));   // should only be called once
  //initializeSocket();
  currentRow = 0;
  currentLength = 1;
  int *vecLists[10];
  allData = vecLists;
  /*newV[0] = _mm256_setr_epi32(2, 4, 6, 8, 10, 12, 14, 16);*/
  //printf("vecList1s %p %p\n", vecLists, vecLists[0]);
  //printf("allData %p %p\n", allData, allData[0]);
  //malloc(sizeof(__m256i*)*10);
  //printf("%ld\n", sizeof(__m256i*));
  int i;
  for (i = 0; i < 50; i++) {
    createDatabase();
    unsigned long et = time(NULL);
    printf("%i Total time: %lu seconds\n", i, et-st);
    //printf("r %i, c %i, = %i\n", currentRow, currentLength, currentRow*currentLength*8);
  }
  printf("\n\n");
  if (0) {
    __m256i v0 = _mm256_setr_epi32(2, 4, 6, 8, 10, 12, 14, 16);
    __m256i v1 = _mm256_setr_epi32(1, 3, 5, 7, 9, 11, 13, 15);
    __m256i v2 = _mm256_setr_epi32(20,26,30,33,34,38,60,65);
    __m256i v3 = _mm256_setr_epi32(15,19,28,31,64,66,67,68);
    __m256i v4 = _mm256_setr_epi32(1,3,4,7,21,22,25,30);
    char t = v0[0] > v1[0];
    //__m256i *bar = malloc(1 * sizeof __m256i);
    int length = 1;
    int vecs = 5;
    int newLength = length * 10;
    __m256i allTen[5][length];
    allTen[0][0] = v0;
    allTen[1][0] = v1;
    allTen[2][0] = v2;
    allTen[3][0] = v3;
    allTen[4][0] = v4;
    /* Compute the difference between the two vectors */
    //__m256i result = _mm256_sub_epi32(evens, odds);
     __m256i result = _mm256_max_epi32(v1,v3);
    __m256i result2 = _mm256_max_epi32(v1,v3);
    int* f = (int*)&result;
    printf("%i %i %i %i %i %i %i %i\n",
      f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
    //int shift = _mm256_extract_epi32(result, 3);
    //int storeMe = _mm256_extract_epi32(result, 7);
    result = _mm256_insert_epi32(_mm256_slli_si256(result, 4), 100, 4);
    //int* f = (int*)&result;
    //f = (int*)&result;
    //int* h = (int*)g;
    //printf("mod %i\n",239%8);
    //printf("v0[7] %i\n",((int*)&v0)[7]);
    //struct AVXVec *newStruct = malloc(sizeof(struct AVXVec));
    printf("size __m256i %lu\n",sizeof(__m256i[10][10]));
    //printf("size _AVXVec %lu\n",sizeof(*newStruct));
    printf("size v4 %lu\n",sizeof(v4));
    //printf("size allTen %lu\n",sizeof(allTen));
    printf("%i %i %i %i %i %i %i %i\n",f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
  }
  return 0;
}
// int
// __m256i _mm256_min_epi32 (__m256i a, __m256i b)
// _mm256_max_epi32 get maximum values
// _mm256_min_epi32 get min
//  _mm256_set_epi32
//__m256i _mm256_mask_set1_epi32 (__m256i src, __mmask8 k, int a)

// sort2, pop, shift, sort1, pop, sort2

//_mm256_movemask_epi8 : get an int with bits set by the first of each 32 bit value
//_mm256_cmpgt_epi32 : compare values, setting result to 1 if true 
//_mm256_blendv_epi8
//https://software.intel.com/sites/landingpage/IntrinsicsGuide/#techs=AVX2&expand=408,742,750,3316,3610,5113,5154

//__int32 _mm256_extract_epi32 (__m256i a, const int index)
//__m256i _mm256_insert_epi32 (__m256i a, __int32 i, const int index)

// _mm256_slli_epi32 shift left 32 bit IntrinsicsGuide
// _mm256_srli_epi32 shift right, zeros

//__m256i _mm256_i32gather_epi32 (int const* base_addr, __m256i vindex, const int scale)
//__m256i _mm256_load_si256 (__m256i const * mem_addr)

