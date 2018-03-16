#ifndef CODE_H_   /* Include guard */
#define CODE_H_

char buffer[128];
int n;
int socket_fd, connection_fd;
int connection;
char *stopstring;
__m256i **allData;
int currentRow;
int currentLength;

void initializeSocket();
void sendOneValue(int value);
int getOneValue();
void insertOneInteger(int *list, int value);
void storeValues();
void getDataFromClient();
void createDatabase();
int getValue(int *list, int value);
void simpleColumnSortHigh(__m256i **input, __m256i *output, int length);
int main();
void printVec();

#endif // CODE_H_