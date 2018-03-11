
struct AVXVec {
	AVXVec left;
	AVXVec right;
	_m256 data;
	_m256epsi valid; 
	int pin;
	};