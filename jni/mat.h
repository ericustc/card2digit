#include <vector>

class mat: public std::vector<bool> {
private:
	int _width;
	int _height;
public:
	mat(int width, int height);
	bool at(int column, int row);
	int size();
};

char recognize(mat &pixel, int l, int r, int t, int b);
float compare(unsigned short glyph[], mat &pixel, int l, int r, int t, int b,
		float ratio);

/*
 * Otsu's method to determine the optimal threshold
 * https://en.wikipedia.org/wiki/Otsu%27s_method
 */
int otsu(int histogram[], int total);
