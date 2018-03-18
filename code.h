#ifndef CODE_H_   /* Include guard */
#define CODE_H_

char buffer[128];
int n;
int socket_fd, connection_fd;
int connection;
char *stopstring;
int **allData;
int currentRow;
int currentLength;
int numSets;

void initializeSocket();
void sendOneValue(int value);
int getOneValue();
void insertOneInteger(int *list, int value);
void storeValues();
void getDataFromClient();
void createDatabase();
int getValue(int *list, int value);
void simpleColumnSortHigh(int **input, int *output, int length);
int main();
void printVec();
int comparator(const void* a, const void* b);
void combineArrays(int **input, int *output);

#endif // CODE_H_