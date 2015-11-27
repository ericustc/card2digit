#include <vector>

template <class T>
class mat: public std::vector<T> {
private:
	int _width;
	int _height;
public:
	mat(int width, int height) : std::vector<T>(width * height) {
		_width = width;
		_height = height;
	}
	T at(int column, int row) {
		return (*this)[row * _width + column];
	}
	int size() {
		return _width * _height;
	}
};

char recognize(mat<bool> &pixel, int l, int r, int t, int b);

/*
 * Otsu's method to determine the optimal threshold
 * https://en.wikipedia.org/wiki/Otsu%27s_method
 */
int otsu(int histogram[], int total);

void write_to_training_file(mat<bool> &pixel, int l, int r, int t, int b);
