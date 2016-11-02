#include <iostream>
#include <cmath>
#include "caltree.hpp"

using namespace std;

int main(int argc, char** argv)
{
	if (argc != 2) {
		cout << "argument error...  ./regression <filename>\n";
		return 0;
	}

	vector<CalTree> trees;
	
	// TODO: change to random generation
	// current: get expr from stdin
	string line;
	getline(cin, line);


	string filename(argv[1]);
	data_read(filename);
	CalTree tree = CalTree(false);
	tree.make_tree(line);
	
	
	cout << "number of vectors:   " << num_vector << endl;
	double val;
	for (int i=0; i<num_vector; i++) {
    	val += tree.calculate(i);
	}
	val /= num_vector;
	cout << "MSE  " << val << endl;
	string postfix = tree.get_postfix();
	cout << postfix << endl;
}
