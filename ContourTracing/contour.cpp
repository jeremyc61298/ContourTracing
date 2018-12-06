// --------------------------------------------
// contour.cpp
// Jeremy Campbell
// Project 11 in applied algorithms. Traces 
// the outline of a set of bits in a text file
// and reports how many full "objects" are 
// found, as well as their sizes.
// --------------------------------------------
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::string;
using std::set;

struct point {
	point() : x(0), y(0) {} 
	point(int x, int y) : x(x), y(y) {}
	string str() {
		return x + " " + y;
	}

	bool operator==(const point& p) { return (this->x == p.x && this->y == p.y); }
	bool operator!=(const point& p) { return (this->x != p.x || this->y != p.y); }

	int x, y;
};

// Holds a "bitmap" which has "objects". Goal of this 
// class is to find all the objects within the bitmap that are of at least 
// 5 pixels. It does this using the Moore boundry tracking algorithm
class ContourTracer {
	int rows, cols;
	vector<string> bitmap;
	vector<int> objectSizes;

	// Points are stored in string form
	set<string> markedPoints;

	// Methods
	void trace(point b0);
	vector<point> initializeNeighbors(point current, point previous);
	void skipPoints(int& i, int& j);

public:
	ContourTracer() = default;
	bool inputBitmap(ifstream& fin);
	void traceContours();
	void printSizes(ofstream& fout);

	// Debug 
	void printBitmap();
};

bool ContourTracer::inputBitmap(ifstream& fin) {
	bool moreCases = false;
	fin >> rows >> cols;

	if (rows != 0 && cols != 0) {
		// Trash the endline
		fin.ignore(1, '\n');

		string line;
		for (int i = 0; i < rows; i++) {
			getline(fin, line);
			bitmap.push_back(line);
		}
		
		// Clean the reusable data structures
		objectSizes.clear();
		markedPoints.clear();
		moreCases = true;
	}
	return moreCases;
}

void ContourTracer::traceContours() {
	// Scan the bitmap for a 1 whose location is not in the markedPoints set
	for (int i = 1; i < rows - 1; i++) {
		for (int j = 1; j < cols - 1; j++) {
			if (bitmap[i][j] == '1') {
				point current(i, j);
				if (markedPoints.find(current.str()) == markedPoints.end()) {
					// A new object has been found
					trace(current);
				}
				else {
					// Already traced this object, needs to be "skipped"
					skipPoints(i, j);
				}
			}
		}
	}
}

void ContourTracer::trace(point b0) {
	point c0(b0.x, b0.y - 1);
	vector<point> neighbors = initializeNeighbors(b0, c0);

}

// Maybe I should use a stack instead?
vector<point> ContourTracer::initializeNeighbors(point cur, point prev) {
	// Find all eight surrounding directions starting with the previous point
	vector<point> neighbors;
	neighbors.emplace_back(cur.x - 1, cur.y);
	neighbors.emplace_back(cur.x - 1, cur.y + 1);
	neighbors.emplace_back(cur.x, cur.y + 1);
	neighbors.emplace_back(cur.x + 1, cur.y + 1);
	neighbors.emplace_back(cur.x + 1, cur.y);
	neighbors.emplace_back(cur.x + 1, cur.y - 1);
	neighbors.emplace_back(cur.x, cur.y - 1);
	neighbors.emplace_back(cur.x - 1, cur.y - 1);

	auto vt = neighbors.begin();
	while (*vt != prev) {
		vt++;
	}
	vector<point> vp(vt, neighbors.end());
	vp.insert(vp.end(), neighbors.begin(), vt);
	return vp;
}

void ContourTracer::skipPoints(int& i, int& j) {
	// Advance i and j until at a location that does not have a 1
	for (; (i < rows - 1) && bitmap[i][j] == 1; i++)
		for (; (j < cols - 1) && bitmap[i][j] == 1; j++)
		{}
}

void ContourTracer::printSizes(ofstream& fout) {
	int numObjectsFound = objectSizes.size();
	if (numObjectsFound == 0) {
		fout << "No objects found" << endl;
	}
	else {
		for (int i = 0; i < numObjectsFound; i++) {
			fout << i;
			if (i != numObjectsFound - 1)
				fout << " ";
		}
	}
}

// Debug 
void ContourTracer::printBitmap() {
	cout << "Rows: " << rows << "Cols: " << cols << endl;
	cout << "----------------------------" << endl;
	for (string row : bitmap) {
		cout << row << endl;
	}
	cout << "----------------------------" << endl;
}

int main() {
	ifstream fin("contour.in");

	if (!fin) {
		cerr << "Cannot find input file." << endl;
		return -1;
	}
	ofstream fout("contour.out");

	ContourTracer ct;
	int caseNumber = 1;
	bool moreCases = ct.inputBitmap(fin);
	
	while (moreCases) {
		ct.traceContours();
		fout << "Case " << caseNumber << endl;
		ct.printSizes(fout);

		moreCases = ct.inputBitmap(fin);
		if (moreCases)
			fout << endl;
	}

}