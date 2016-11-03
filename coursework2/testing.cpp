#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include "caltree.hpp"

using namespace std;

int main(int argc, char** argv)
{
	if (argc != 2) {
		cout << "argument error...  ./regression <filename>\n";
		return 0;
	}

	string line;
	getline(cin, line);


	string filename(argv[1]);
	data_read(filename);
	CalTree* tree = new CalTree(false);
	tree->make_tree(line);
	
	
	cout << "number of vectors:   " << num_vector << endl;
	double val;
	for (int i=0; i<num_vector; i++) {
    	val += tree->calculate(i);
	}
	val /= num_vector;
	tree->expression = tree->get_postfix();
	cout << tree->expression << endl;
	cout << "MSE  " << val << endl;
	delete tree;
}
