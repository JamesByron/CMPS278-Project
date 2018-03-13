
struct AVXVec {
	AVXVec left;
	AVXVec right;
	_m256 data;
	_m256epsi valid; 
	int pin;
	};

struct AVXVec {
  struct AVXVec *child;
  __m256i *data;
  int dataLength; // The number of vectors in each row of data[]
  int currentDataIndex; // the current index in data[] that we can fill, always < 10
  };