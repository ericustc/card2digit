#include <vector>
#include "mat.h"

using namespace std;

mat::mat(int width, int height) : vector<bool>(width * height) {
	_width = width;
	_height = height;
}

bool mat::at(int column, int row) {
	return (*this)[row * _width + column];
}

int mat::size() {
	return _width * _height;
}
