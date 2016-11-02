#ifndef CALTREE_H
#define CALTREE_H

#include <vector>
#include <string>
#include <stack>

using namespace std;

vector<string> u_opers = {"~", "abs", "sin", "cos", "tan", "asin", "acos", "atan", "sinh", "cosh", "tanh", "exp", "sqrt", "log"};

vector<string> b_opers = {"+", "-", "*", "/", "^"};

class TreeNode
{
	public:
		TreeNode(const string& val);
		string value;
		TreeNode *left, *right;
	private:
		friend class CalTree;
}

class CalTree
{
	public:
		CalTree();
		void clean(TreeNode* root);
		void make_tree(const string expr);
		double calculate();
	private:
		double calculate(TreeNode* root);
}

#endif
