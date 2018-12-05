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
using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::string;

struct point {
	point() : x(0), y(0) {} 
	point(int x, int y) : x(x), y(y) {}

	int x, y;
};

// Holds a "bitmap" which has "objects". Goal of this 
// class is to find all the objects within the bitmap that are of at least 
// 5 pixels. It does this using the Moore boundry tracking algorithm
class ContourTracer {
	int rows, cols;
	vector<string> bitmap;
	vector<int> objectSizes;

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
		moreCases = true;
	}
	return moreCases;
}

void ContourTracer::traceContours() {

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
		// Find the contours
		// Print out the case
		fout << "Case " << caseNumber << endl;
		ct.printSizes(fout);

		moreCases = ct.inputBitmap(fin);
		if (moreCases)
			fout << endl;
	}

}