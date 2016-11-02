#ifndef CALTREE_H
#define CALTREE_H

#include <vector>
#include <string>
#include <stack>

using namespace std;

//vector<string> u_opers = {"~", "abs", "sin", "cos", "tan", "asin", "acos", "atan", "sinh", "cosh", "tanh", "exp", "sqrt", "log"};

//vector<string> b_opers = {"+", "-", "*", "/", "^"};
extern vector< vector<double> > data_set;
extern int num_vector;
extern int num_var;

class TreeNode
{
	public:
		TreeNode(const string val);
		string value;
		int valuetype;//0: number, 1: variable, 2: u, 3: bi
		TreeNode *left, *right;
	private:
		friend class CalTree;
};

class CalTree
{
	public:
		CalTree(bool random);
		~CalTree();
		void clean(TreeNode* root);
		void make_tree(const string expr);
		void make_tree();
		double calculate(int i);
		string get_postfix();
		TreeNode* randomTree(int max_depth);
		double mse;
	private:
		stack<TreeNode*> tree_stack;
		bool exist_x;
		double calculate(TreeNode* root, int i);
		string get_postfix(TreeNode* root);
};

int random_int(int max_val);
float random_real(float max_val);
void data_read(string filename);

#endif
