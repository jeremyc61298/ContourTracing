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
using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;

int main() {
	ifstream fin("contour.in");

	if (!fin) {
		cerr << "Cannot find input file." << endl;
		return -1;
	}
}