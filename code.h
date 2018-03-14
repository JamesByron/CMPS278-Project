#ifndef CODE_H_   /* Include guard */
#define CODE_H_

struct AVXVec {
  struct AVXVec *child;
  int dataLength; // The number of vectors in each row of data[]
  int currentDataIndex; // the current index in data[] that we can fill, always < 10
  int bottom;
  __m256i **data;
};

void insertOneInteger(int *list, int value);
void storeValues(struct AVXVec *destination, __m256i *vec, int length);
__m256i getDataFromClient(struct AVXVec *head);
void createDatabase(struct AVXVec *head);
int getValue(struct AVXVec *current, int value);
void simpleColumnSortHigh(__m256i **input, __m256i *output, int length);
int main();

#endif // CODE_H_