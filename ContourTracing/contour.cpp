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
#include <unordered_set>
#include <algorithm>
#include <sstream>
using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::string;
using std::to_string;
using std::unordered_set;
using std::sort;
using std::stringstream;

struct point {
	point() : x(0), y(0) {} 
	point(int x, int y) : x(x), y(y) {}
	explicit point(string p) {
		stringstream ss(p);
		ss >> x >> y;
	}

	string str() {
		return to_string(x) + " " + to_string(y);
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
	const int SMALLEST_OBJECT = 5;
	vector<string> bitmap;
	vector<int> objectSizes;

	// Points are stored in string form
	unordered_set<string> markedPoints;

	// Methods
	void trace(point b0);
	bool marked(const unordered_set<string>& s, string pointToCheck);
	vector<point> initializeNeighbors(point current, point previous);
	void skipPoints(int& i, int& j);
	void addNewSize(unordered_set<string>& uniquePoints, int objectSize);

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
	bitmap.clear();

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
				if (!marked(markedPoints, current.str())) {
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
	point cN, b1, bN(0, 0);	
	unordered_set<string> uniquePoints;
	vector<point> neighbors = initializeNeighbors(b0, c0);
	bool secondPoint = false, fullTrace = false;
	int i, objectSize = 0;

	for (i = 1; i < neighbors.size() && !fullTrace; i++) {
		// Test each neighboring point to see if it's a 1
		point curr = neighbors[i];
		if (bitmap[curr.x][curr.y] == '1') {
			if (curr != b1 || bN != b0) {
				if (!secondPoint) {
					secondPoint = true;
					b1 = curr;
				}
				bN = curr;
				cN = neighbors[i - 1];
				objectSize++;
				markedPoints.insert(bN.str());
				uniquePoints.insert(bN.str());
				neighbors = initializeNeighbors(bN, cN);
				i = 0 ;
			}
			else {
				// Back to where it started, object has been traced. 
				fullTrace = true;
			}
		}
	}
	
	addNewSize(uniquePoints, objectSize);
}

void ContourTracer::addNewSize(unordered_set<string>& uniquePoints, int objectSize) {
	if (uniquePoints.size() <= SMALLEST_OBJECT) {
		for (string s : uniquePoints) {
			point curr(s);
			point prev(curr.x, curr.y - 1);
			vector<point> neighbors = initializeNeighbors(curr, prev);
			for (int i = 0; i < neighbors.size(); i++) {
				// check to see if there are any other points in the object
				point p = neighbors[i];
				if (bitmap[p.x][p.y] == '1' && !marked(uniquePoints, p.str())) {
					uniquePoints.insert(p.str());
				}
			}
		}
	}
	
	if (uniquePoints.size() >= SMALLEST_OBJECT) {
		objectSizes.push_back(objectSize);
	}
}

bool ContourTracer::marked(const unordered_set<string>& s, string pointToCheck) {
	return s.find(pointToCheck) != s.end();
}

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
	while (i < rows - 1 && bitmap[i][j] == '1') {
		while (j < cols - 1 && bitmap[i][j] == '1') {
			//if (bitmap[i][j] == '1')
				++j;
		}
		if (bitmap[i][j] == '1')
			++i;
	}
}


void ContourTracer::printSizes(ofstream& fout) {
	int numObjectsFound = objectSizes.size();
	if (numObjectsFound == 0) {
		fout << "No objects found";
	}
	else {
		sort(objectSizes.begin(), objectSizes.end());
		for (int i = 0; i < numObjectsFound; i++) {
			fout << objectSizes[i];
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
		caseNumber++;
	}

}