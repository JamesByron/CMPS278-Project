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
  printf("Store values\n");
  //allData[currentRow] = vec;
  //currentRovw++;
  if (currentRow == 10) {
    printf("here 10!\n");
    int newLength = currentLength * 10;
    //__m256i **newVec = (__m256i**)calloc(newLength, __m256i);
    __m256i newVec[newLength];
    printf("Pointer %ld\n", sizeof(newVec));
    // sort and merge
    simpleColumnSortHigh(allData, newVec, currentLength);
    __m256i *vecLists[10];
    allData = vecLists;
    allData[0] = newVec;
    int x = (int)_mm256_extract_epi32(allData[0][5],0);
    printf("x is %i\n", x);
    //allData = (__m256i**)newVec;
    //storeValues(newVec);
    currentRow = 1;
    currentLength = newLength;
  }
}

void getDataFromClient() {
  printf("Here in getDataFromClient\n");
  int totalLength = currentLength * 8;
  int list[totalLength];
  printf("\nbegin\n");
  printVec();
  int i;
  for (i = 0; i < totalLength; i++) {
    list[i] = 0;
  }
  printf("made list\n");
  i = 0;
  while (i < totalLength) {
    int newValue = (rand()/2000000)+(rand()/2000000);
    printf("%i, ", newValue);
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
  }printf("\nmiddle\n");
  printVec();
  __m256i *newV;
  newV = (__m256i*)calloc(currentLength, sizeof(__m256i));
  for (i = 0; i < currentLength; i++) {
    int start = i*8;
    printf("Store %ld\n", sizeof(allData));
    (*newV)[i] = _mm256_setr_epi32(list[start],list[start+1],list[start+2],list[start+3],list[start+4],list[start+5],list[start+6],list[start+7]);
    printf("allData %i %p %p %p\n", currentRow, allData, allData[0], allData[1]);
    if (currentRow == 0) allData[currentRow] = newV;
  }
  currentRow++;
  storeValues();
  printf("\nend\n");
  printVec();
}

void createDatabase() {
  printf("here 1\n");
  getDataFromClient();
  printf("here 1.1 %i %i\n", currentLength, currentRow);
  //printf("here 2\n");
}

int getValue(int *list, int value) {
  int i;
  for (i = 0; i < currentRow; i++) {
	  int j;
  	for (j = 0; j < currentLength; j++) {
  	  int temp[8] = {_mm256_extract_epi32(allData[i][j],0),
  	                 _mm256_extract_epi32(allData[i][j],1),
  					 _mm256_extract_epi32(allData[i][j],2),
  					 _mm256_extract_epi32(allData[i][j],3),
  					 _mm256_extract_epi32(allData[i][j],4),
  					 _mm256_extract_epi32(allData[i][j],5),
  					 _mm256_extract_epi32(allData[i][j],6),
  					 _mm256_extract_epi32(allData[i][j],7)};
  	  int k;
  	  for (k = 0; k < 8; k++) {
  	    if (temp[k] == value) {
  		    return value;
  		  }
    		if (temp[k] > value) {
    		  // skip to the next row
    		  k = 8;
    		  j = currentLength;
    		}
	    }
	  }
  }
  for (i = (currentLength*10)-1; i >= 0; i--) {
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
  printf("current %i\n", currentRow);
  for (j = 0; j < currentRow; j++) {
    int x = (int)_mm256_extract_epi32(allData[j][0],0);
    printf("%i ", x);
    x = (int)_mm256_extract_epi32(allData[j][0],1);
    printf("%i ", x);
    x = (int)_mm256_extract_epi32(allData[j][0],2);
    printf("%i ", x);
    x = (int)_mm256_extract_epi32(allData[j][0],3);
    printf("%i ", x);
    x = (int)_mm256_extract_epi32(allData[j][0],4);
    printf("%i ", x);
    x = (int)_mm256_extract_epi32(allData[j][0],5);
    printf("%i ", x);
    x = (int)_mm256_extract_epi32(allData[j][0],6);
    printf("%i ", x);
    x = (int)_mm256_extract_epi32(allData[j][0],7);
    printf("%i\n", x);
  }
}

void simpleColumnSortHigh(__m256i **input, __m256i *output, int length) {
  // input[numSets][length][8]
  //int* f = (int*)&result[0];
  printf("Here in sort\n");
    //__m256i output[length * 10];
  __m256i tempVector;
  int numSets = 10; // Must change for loop too
  int numToMove = (8 * numSets * length)-1;
  int numMoved = 0;
  int insertIndex = 7;
  printVec();
  while (numToMove >= 0) {
    int i = 0;
    for (i = 0; i < length; i++) {
      // Sort 1
      int s;
      for (s = numSets-1; s > 0; s--) {
        tempVector = _mm256_max_epi32(input[s-1][i], input[s][i]);
        input[s][i] = _mm256_min_epi32(input[s-1][i], input[s][i]);
        input[s-1][i] = tempVector;
      }
    }printVec();
    exit(0);
    // Items are now sorted with highest on top
    // Take 1
    int maxValue = _mm256_extract_epi32(input[0][0], 7);
    printf("%i ", maxValue);
    if (insertIndex > 3) {
      if (insertIndex > 5) {
        if (insertIndex == 6) { output[numToMove / 8] = _mm256_insert_epi32(output[numToMove / 8], maxValue, 6); }
        else { output[numToMove / 8] = _mm256_insert_epi32(output[numToMove / 8], maxValue, 7); }
      }
      else {
        if (insertIndex == 4) { output[numToMove / 8] = _mm256_insert_epi32(output[numToMove / 8], maxValue, 4); }
        else { output[numToMove / 8] = _mm256_insert_epi32(output[numToMove / 8], maxValue, 5); }
      }
    }
    else {
      if (insertIndex > 1) {
        if (insertIndex == 2) { output[numToMove / 8] = _mm256_insert_epi32(output[numToMove / 8], maxValue, 2); }
        else { output[numToMove / 8] = _mm256_insert_epi32(output[numToMove / 8], maxValue, 3); }
      }
      else {
        if (insertIndex == 1) { output[numToMove / 8] = _mm256_insert_epi32(output[numToMove / 8], maxValue, 1); }
        else { output[numToMove / 8] = _mm256_insert_epi32(output[numToMove / 8], maxValue, 0); }
      }
    }
    //output[numToMove / 8] = _mm256_insert_epi32(output[numToMove / 8], maxValue, insertIndex);
    numMoved++;
    numToMove--;
    insertIndex--;
    if (insertIndex < 0) {
      insertIndex = 7;
    }
    // shift
    input[0][0] = _mm256_insert_epi32(_mm256_slli_si256(input[0][0], 4), _mm256_extract_epi32(input[0][0], 3), 4);
    // we already did input[0][0]
    for (i = 1; i < length; i++) {
      int extractedValue = _mm256_extract_epi32(input[0][i], 7);
      // insert it into i-1 column 0
      input[0][i-1] = _mm256_insert_epi32(input[0][i-1], extractedValue, 0);
      // now shif input[0][i]
      input[0][i] = _mm256_insert_epi32(_mm256_slli_si256(input[0][i], 4), _mm256_extract_epi32(input[0][i], 3), 4);
    }
    if ((numMoved % (numSets * 8)) == 0) {
      length--;
      // We want to not sort lists when they are empty
      // but we don't do this if we are using the double sided approach
      // Other idea is to resort only when input[0][0][7] < input[0][1][7]
      // but we'd have to sort 2+ times and not take more than recent sorts 
    }
  }
  printf("Done sort\n");
}

int main() {
  srand(time(NULL));   // should only be called once
  //initializeSocket();
  currentRow = 0;
  currentLength = 1;
  __m256i *vecLists[10];
  allData = vecLists;
  /*printf("2\n");
  __m256i newV[1];
  printf("3\n");
  newV[0] = _mm256_setr_epi32(2, 4, 6, 8, 10, 12, 14, 16);*/
  printf("vecList1s %p %p\n", vecLists, vecLists[0]);
  printf("allData %p %p\n", allData, allData[0]);
  //malloc(sizeof(__m256i*)*10);
  //printf("%ld\n", sizeof(__m256i*));
  int i;
  for (i = 0; i < 10; i++) {
    printf("%i\n", i);
    createDatabase();
    /*int j;
    for (j = 0; j < i; j++) {
      printf("current %i\n", currentRow);
      int x = (int)_mm256_extract_epi32(allData[j][0],0);
      printf("%i ", x);
      x = (int)_mm256_extract_epi32(allData[j][0],1);
      printf("%i ", x);
      x = (int)_mm256_extract_epi32(allData[j][0],2);
      printf("%i ", x);
      x = (int)_mm256_extract_epi32(allData[j][0],3);
      printf("%i ", x);
      x = (int)_mm256_extract_epi32(allData[j][0],4);
      printf("%i ", x);
      x = (int)_mm256_extract_epi32(allData[j][0],5);
      printf("%i ", x);
      x = (int)_mm256_extract_epi32(allData[j][0],6);
      printf("%i ", x);
      x = (int)_mm256_extract_epi32(allData[j][0],7);
      printf("%i\n", x);
    }*/
  }
  /*
  bzero(buffer,128);
  snprintf(buffer, sizeof buffer, "2456get");
  if (buffer[0] == 'g') {
    buffer[0] = ' ';
  }
  long returnedLong = strtol(buffer, &stopstring, 10);
  printf("%ld %s\n",returnedLong, stopstring);
  */
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

