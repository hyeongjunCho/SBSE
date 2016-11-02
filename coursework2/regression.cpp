#include <iostream>
#include <cmath>
#include "caltree.hpp"
#include <fstream>

using namespace std;

int main(int argc, char** argv)
{
	if (argc != 2) {
		cout << "argument error...  ./regression <filename>\n";
		return 0;
	}

	vector<CalTree*> trees;
	
	// TODO: change to random generation
	// current: get expr from stdin
	//string line;
	//getline(cin, line);


	string filename(argv[1]);
	data_read(filename);
	double min_val = 1000000000.0;
	int min_tree = -1;
	int i=0;
	while (1)
	{
		//cout << "tree    " << i << endl;
		double val=0.0;
		CalTree* tree = new CalTree(true);
		for (int j=0; j<num_vector; j++) {
			val += tree->calculate(j);
		}
		if (val != val) {
			cout << "clean  " << i << endl;
			free(tree);
			continue;
		}
		val /= num_vector;
		tree->mse = val;
		trees.push_back(tree);
		if (val < min_val)
		{
			min_val = val;
			min_tree = i;
		}
		cout << val << endl;
		cout << tree->get_postfix() << endl;
		i++;
		if (i == 100) break;
	}
	//tree.make_tree(line);
	
	
	cout << "number of vectors:   " << num_vector << endl;
	/*double val;
	for (int i=0; i<num_vector; i++) {
    	val += tree.calculate(i);
	}
	val /= num_vector;*/
	cout << "MSE  " << min_val << endl;
	string postfix = trees[min_tree]->get_postfix();
	cout << postfix << endl;
	std::ofstream outfile;
	outfile.open("MSE.txt", std::ios_base::app);
	outfile << postfix << endl; 
	return 0;
}
