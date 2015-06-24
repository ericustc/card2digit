#include <vector>

class mat: public std::vector<bool> {
private:
	int _width;
	int _height;
public:
	mat(int width, int height);
	bool at(int row, int column);
	int size();
};
