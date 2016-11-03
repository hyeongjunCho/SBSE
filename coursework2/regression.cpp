#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
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

	int num_generations=20;
	if (argc == 4) {
		if (strcmp(argv[1],"-g") == 0) {
			if (atoi(argv[2]) < 20) {
				num_generations = atoi(argv[2]);
			}
		}
	}
	string filename(argv[argc-1]);
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
		if (val != val || val > 100 * num_vector || val < -100 * num_vector) {
			//cout << "clean  " << i << endl;
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
		//cout << val << endl;
		//cout << tree->get_postfix() << endl;
		//cout << "nt  " << tree->num_nt << endl;
		//cout << "t   " << tree->num_t << endl;
		i++;
		if (i == 100) break;
	}
	vector< vector<CalTree*> > generations;
	generations.push_back(trees);
	int cur;
	for (cur = 0; cur < num_generations; cur++)
	{
		cout << "generation " << cur+1 << endl;
		vector<CalTree*> new_gen = next_generation(generations[cur]);
		cout << "end generation" << endl;
		//cout << "end generation" << endl;
		min_val = 1000000000.0;
		min_tree = -1;
		i=0;
		while (1)
		{
			CalTree* tree = new_gen[i];
			double val=0.0;
			for (int j=0; j<num_vector; j++) {
				val += tree->calculate(j);
			}
			val /= num_vector;
			tree->mse = val;
			if (val < min_val)
			{
				min_val = val;
				min_tree = i;		
			}
			i++;
			if (i==new_gen.size()) break;
		}
		generations.push_back(new_gen);
		cout << "MSE  " << min_val << endl;
		string postfix = generations[cur+1][min_tree]->get_postfix();
		cout << postfix << endl;
		
	}	
	//min_val = 1000000000.0;
	//min_tree = -1;
	//i=0;

	/*while (1)
	{
		CalTree* tree = generations.back()[i];
		double val=0.0;
		for (int j=0; j<num_vector; j++) {
			val += tree->calculate(j);
		}
		val /= num_vector;
		tree->mse = val;
		if (val < min_val)
		{
			min_val = val;
			min_tree = i;		
		}
		i++;
		if (i==200) break;
	}*/
	
	
	
	cout << "number of vectors:   " << num_vector << endl;
	cout << "MSE  " << min_val << endl;
	string postfix = trees[min_tree]->get_postfix();
	cout << postfix << endl;
	//cout << "nt  " << trees[min_tree]->num_nt << endl;
	//cout << "t   " << trees[min_tree]->num_t << endl;
	//std::ofstream outfile;
	//outfile.open("MSE.txt", std::ios_base::app);
	//outfile << postfix << endl; 
	return 0;
}
