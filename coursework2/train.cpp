#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include "caltree.hpp"
#include <fstream>
#include <set>

using namespace std;

/*bool cmp(CalTree* t, CalTree* t2)
{
	if(t->expression.compare(t2->expression) < 0) return 1;
	return 0;
}*/
int main(int argc, char** argv)
{
	if (argc != 2 && argc != 4) {
		cout << "argument error...  ./regression <filename>\n";
		return 0;
	}

	// TODO: change to random generation
	// current: get expr from stdin
	//string line;
	//getline(cin, line);

	int num_generations=200;
	if (argc == 4) {
		if (strcmp(argv[1],"-g") == 0) {
			if (atoi(argv[2]) < 200) {
				num_generations = atoi(argv[2]);
			}
		}
	}
	vector<CalTree*> trees;
	string filename(argv[argc-1]);
	data_read(filename);
	double min_val = 1000000000.0;
	int min_tree = -1;
	int i=0;
	while (1)
	{
		double val=0.0;
		CalTree* tree = new CalTree(true);
		for (int j=0; j<num_vector; j++) {
			val += tree->calculate(j);
		}
		if (val != val || val > 100 * num_vector || val < -100 * num_vector) {
			free(tree);
			continue;
		}
		val /= num_vector;
		tree->mse = val;
		tree->expression = tree->get_postfix();
		bool f = false;
		for (auto t : trees)
		{
			if (t->expression.compare(tree->expression) == 0)
			{
				free(tree);
				f=true;
				break;
			}
		}
		if (f) continue;
		else {
			trees.push_back(tree);
			if (trees.size() == 200) break;
		}

		//cout << val << endl;
		//cout << tree->get_postfix() << endl;
		//cout << "nt  " << tree->num_nt << endl;
		//cout << "t   " << tree->num_t << endl;
	}
	//vector< vector<CalTree*> > generations;
	//generations.push_back(trees);
	//for (auto t : forest)
	//{
	//	trees.push_back(t);
	//}
	int cur;
	for (cur = 0; cur < num_generations; cur++)
	{
		cout << "generation " << cur+1 << endl;
		vector<CalTree*> new_gen = next_generation(trees);
		cout << "end generation" << endl;
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
			tree->expression = tree->get_postfix();
			bool f = false;
			for (auto t : trees)
			{
				if (t->expression.compare(tree->expression) == 0)
				{
					free(tree);
					f=true;
					break;
				}
			}
			if (f) {
				i++;
				if (i==new_gen.size()) break;
				continue;
			}
			else {
				//cout << tree->get_postfix() << endl;
				trees.push_back(tree);
				i++;
				if (i==new_gen.size()) break;
			}
			/*bool is_same=false;
			for(set<CalTree*>::iterator it=forest.begin(); it!=forest.end(); it++) {
				if((*it)->expression.compare(tree->expression) == 0) {
					is_same = true;
					break;
				}
			}
			if (!is_same) {
				forest.insert(tree);
				trees.push_back(tree);
			}
			else {
				free(tree);
			}
			forest_size = forest.size();
			i++;
			cout << forest_size << "   " << forest.size() << endl;
			if (i==new_gen.size()) break;*/
		}
		min_val = 100000.0;
		min_tree = -1;
		i = 0;
		while (1)
		{
			CalTree* tree = trees[i];
			if (tree->mse < min_val)
			{
				min_val = tree->mse;
				min_tree = i;		
			}
			i++;
			if (i == trees.size()) break;
		}
		cout << "MSE  " << min_val << endl;
		string postfix = trees[min_tree]->get_postfix();
		cout << postfix << endl;
		
		if (cur>4)
		{
			vector<CalTree*> new_trees;
			for (auto t : trees)
			{
				if (t->mse < max(min_val * 5.0,0.5)) new_trees.push_back(t);
			}
			trees.clear();
			trees.assign(new_trees.begin(), new_trees.end());
		}
		cout << "trees size  " << trees.size() << endl;
	}	
	min_val = 1000000000.0;
	min_tree = -1;
	for (int i=0; i<trees.size(); i++)
	{
		CalTree* t = trees[i];
		if (t->mse < min_val)
		{
			min_val = t->mse;
			min_tree = i;
		}
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
	

	cout << "\n-----------------------------------------------------------------" << endl;
	cout << postfix << endl;
	cout << "-----------------------------------------------------------------" << endl;
	//cout << "nt  " << trees[min_tree]->num_nt << endl;
	//cout << "t   " << trees[min_tree]->num_t << endl;
	//std::ofstream outfile;
	//outfile.open("MSE.txt", std::ios_base::app);
	//outfile << postfix << endl; 
	return 0;
}
