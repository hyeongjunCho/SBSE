#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <random>
#include "caltree.hpp"
#define MAX_DEPTH 25

using namespace std;
int num_vector;
int num_var;
vector<string> u_opers = {"~", "abs", "sin", "cos", "tan", "asin", "acos", "atan", "sinh", "cosh", "tanh", "exp", "sqrt", "log"};
vector<string> b_opers = {"+", "-", "*", "/", "^"};
vector< vector<double> > data_set;
vector<string> xs;

vector<string> split(const string& str, int delimiter(int) = ::isspace)
{
  	vector<string> result;
	auto e=str.end();
  	auto i=str.begin();
  	while(i!=e){
    	i=find_if_not(i,e, delimiter);
    	if(i==e) break;
    	auto j=find_if(i,e, delimiter);
    	result.push_back(string(i,j));
    	i=j;
  	}
  	return result;
}

bool expr_check(vector<string> vals) {
	bool flag;
	for (auto s:vals)
	{
		//cout<<s<<endl;
		flag = false;
		for (auto o:u_opers)
		{
			if (o.compare(s) == 0)
			{
				flag = true;
				break;
			}
		}
		if (flag) continue;
		for (auto o:b_opers)
		{
			if (o.compare(s) == 0)
			{
				flag = true;
				break;
			}
		}
		if (flag) continue;
		if (s.find_first_not_of("0123456789.") == string::npos) continue;
		if (s[0] == 'x' and s.size() > 1) {
			string sx = s.substr(1);
			if (sx.find_first_not_of("0123456789") == string::npos)
			{
				continue;
			}
		}
		return false;
	}
	return true;
}

void data_read(string filename) 
{
	string str;
	ifstream inf(filename.c_str());
	getline(inf, str);
	while (getline(inf, str)) {
		vector<double> v;
		stringstream ss(str);
		float k;
		while (ss >> k) {
			v.push_back(k);
			if (ss.peek() == ',')
				ss.ignore();
		}
		data_set.push_back(v);
	}
	num_vector = data_set.size();
	num_var = data_set[0].size()-1;
	for (int i=1; i<=num_var; i++)
	{
		string s = "x" + to_string(i);
		xs.push_back(s);
	}
}

int random_int(int max_val)
{
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<> dis(0, max_val);
	return dis(mt);
}

float random_real(float max_val)
{
	random_device rd;
	mt19937 mt(rd());
	uniform_real_distribution<float> dis(0.1, max_val);
	return dis(mt);
}

CalTree::~CalTree()
{
	clean(tree_stack.top());
}

TreeNode* CalTree::randomTree(int max_depth)
{
	if (max_depth == 0)
	{
		int type = random_int(3);
		if (type == 0 && exist_x) {
			TreeNode* t;
			double value = random_real(1.0);
			string s = to_string(value);
			//cout << s << endl;
			t = new TreeNode(s);
			t->valuetype = 0;
			//tree_stack.push(t);
			return t;
		} else {
			TreeNode* t;
			int num = random_int(num_var-1)+1;
			string s = "x" + to_string(num);
			//cout << s << endl;
			t = new TreeNode(s);
			t->valuetype = 1;
			//tree_stack.push(t);
			exist_x = true;
			return t;
		}
	}
	else
	{
		int type = random_int(1); //0:unary, 1:binary
		if (type == 0) {
			TreeNode* w;
			int num = random_int(u_opers.size()-1);
			string s = u_opers[num];
			//cout << s << endl;
			w = new TreeNode(s);
			w->valuetype = 2;
			if (max_depth > 2) {
				w->left = randomTree(max_depth - 1 - random_int(max_depth-1-2));
			}
			else {
				w->left = randomTree(max_depth - 1);
			}
			return w;
		} else if (type == 1) {
			TreeNode* w;
			int num = random_int(b_opers.size()-1);
			string s = b_opers[num];
			//cout << s << endl;
			w = new TreeNode(s);
			w->valuetype = 3;
			if (max_depth > 2) {
				w->left = randomTree(max_depth - 1 - random_int(max_depth-1-2));
				w->right = randomTree(max_depth - 1 - random_int(max_depth-1-2));
			}
			else {
				w->left = randomTree(max_depth - 1);
				w->right = randomTree(max_depth - 1);
			}
			return w;
		}
	}
}

TreeNode::TreeNode(const string val)
{
	value = val;
	left = NULL;
	right = NULL;
}

CalTree::CalTree(bool random)
{
	exist_x = false;
	if (random) 
	{
		make_tree();
	}
}


void CalTree::clean(TreeNode* root)
{
	if(root->value.size() != 0)
	{
		clean(root->left);
		clean(root->right);
	}
	else
	{
		delete root;
	}
}


void CalTree::make_tree(const string expr)
{
	vector<string> vals = split(expr);
	if (!expr_check(vals))
	{
		cout << "wrong operator" << endl;
	}

	for (auto s:vals)
	{
		if (s[0] == 'x' and s.size() > 1) {
			string sx = s.substr(1);
			if (sx.find_first_not_of("0123456789") == string::npos)
			{
				TreeNode* t = new TreeNode(s);
				t->valuetype = 1;
				tree_stack.push(t);
			}
			else {
				cout << "variable error\n";
				exit(0);
			}
		}
		else if (s.find_first_not_of("0123456789.") == string::npos)
		{
			TreeNode* t = new TreeNode(s);
			t->valuetype = 0;
			tree_stack.push(t);
		}
		else
		{
			bool bi = false;
			for (auto o:b_opers)
			{
				if (o.compare(s) == 0)
				{
					bi = true;
					break;
				}
			}
			if (bi)
			{
				TreeNode* w = new TreeNode(s);
				w->valuetype = 3;
				w->right = tree_stack.top();
				tree_stack.pop();
				w->left = tree_stack.top();
				tree_stack.pop();
				tree_stack.push(w);
			}
			else
			{
				TreeNode* w = new TreeNode(s);
				w->valuetype = 2;
				w->left = tree_stack.top();
				tree_stack.pop();
				tree_stack.push(w);
			}
		}
	}
}

void CalTree::make_tree()
{
	this->tree_stack.push(randomTree(MAX_DEPTH));
}

double CalTree::calculate(int i)
{
	return pow(calculate(tree_stack.top(),i) - data_set[i].back(), 2);
	//double v = pow(calculate(tree_stack.top(), i)-data_set[i].back(), 2);
	//cout << v << endl;
	//return v;
}

double CalTree::calculate(TreeNode* root, int i)
{
	//cout << "root->value:  " << root->value << endl;
	if (root->left == NULL && root->right == NULL)
	{
		if ((root->value)[0] == 'x') {
			string sub_str = (root->value).substr(1);
			//cout << "start with x   " << data_set[i][stoi(sub_str)-1] << endl;
			return data_set[i][stoi(sub_str)-1];
		}
		//cout << root->value << endl;
		return stod(root->value);
	}
	else
	{
		double ret=0.0;
		if (root->value.compare("~") == 0)
		{
			double l = calculate(root->left, i);
			if (l != l) {
				return sqrt(-1.0);
			}
			ret = -l;
		}
		else if (root->value.compare("abs") == 0)
		{
			double l = calculate(root->left, i);
			if (l != l) {
				return sqrt(-1.0);
			}
			ret = abs(l);
		}
		else if (root->value.compare("sin") == 0)
		{
			double l = calculate(root->left, i);
			if (l != l) {
				return sqrt(-1.0);
			}
			ret = sin(l);
		}
		else if (root->value.compare("cos") == 0)
		{
			double l = calculate(root->left, i);
			if (l != l) {
				return sqrt(-1.0);
			}
			ret = cos(l);
		}
		else if (root->value.compare("tan") == 0)
		{
			double l = calculate(root->left, i);
			if (l != l) {
				return sqrt(-1.0);
			}
			ret = tan(l);
		}
		else if (root->value.compare("asin") == 0)
		{
			double l = calculate(root->left, i);
			if (l != l) {
				return sqrt(-1.0);
			}
			if (l <= 1 && l >= -1) ret = asin(l);
			else ret = sqrt(-1.0);
		}
		else if (root->value.compare("acos") == 0)
		{
			double l = calculate(root->left, i);
			if (l != l) {
				return sqrt(-1.0);
			}
			if (l <= 1 && l >= -1) ret = acos(l);
			else ret = sqrt(-1.0);
		}
		else if (root->value.compare("atan") == 0)
		{
			double l = calculate(root->left, i);
			if (l != l) {
				return sqrt(-1.0);
			}
			ret = atan(l);
		}
		else if (root->value.compare("sinh") == 0)
		{
			double l = calculate(root->left, i);
			if (l != l) {
				return sqrt(-1.0);
			}
			if (l > 20 || l < -20) ret = sqrt(-1.0);
			else ret = sinh(l);
		}
		else if (root->value.compare("cosh") == 0)
		{
			double l = calculate(root->left, i);
			if (l != l) {
				return sqrt(-1.0);
			}
			if (l > 20 || l < -20) ret = sqrt(-1.0);
			else ret = cosh(l);
		}
		else if (root->value.compare("tanh") == 0)
		{
			double l = calculate(root->left, i);
			if (l != l) {
				return sqrt(-1.0);
			}
			ret = tanh(l);
		}
		else if (root->value.compare("exp") == 0)
		{
			double l = calculate(root->left, i);
			if (l != l) {
				return sqrt(-1.0);
			}
			if (l > 13) ret = sqrt(-1.0);
			else ret = exp(l);
		}
		else if (root->value.compare("sqrt") == 0)
		{
			double l = calculate(root->left, i);
			if (l != l) {
				return sqrt(-1.0);
			}
			ret = sqrt(l);
		}
		else if (root->value.compare("log") == 0)
		{
			double l = calculate(root->left, i);
			if (l != l) {
				return sqrt(-1.0);
			}
			if (l <= 0) ret = sqrt(-1.0);
			else ret = log(l);
		}
		//double r = calculate(root->right, i);
		//if (r != r) return sqrt(-1.0);
		else if (root->value.compare("+") == 0)
		{
			double l = calculate(root->left, i);
			if (l != l) {
				return sqrt(-1.0);
			}
			double r = calculate(root->right, i);
			if (r != r) return sqrt(-1.0);
			ret = l+r;
		}
		else if (root->value.compare("-") == 0)
		{
			double l = calculate(root->left, i);
			if (l != l) {
				return sqrt(-1.0);
			}
			double r = calculate(root->right, i);
			if (r != r) return sqrt(-1.0);
			ret = l-r;
		}
		else if (root->value.compare("*") == 0)
		{
			double l = calculate(root->left, i);
			if (l != l) {
				return sqrt(-1.0);
			}
			double r = calculate(root->right, i);
			if (r != r) return sqrt(-1.0);
			ret = l*r;
		}
		else if (root->value.compare("/") == 0)
		{
			double l = calculate(root->left, i);
			if (l != l) {
				return sqrt(-1.0);
			}
			double r = calculate(root->right, i);
			if (r != r) return sqrt(-1.0);
			if (abs(r) < 0.0001) ret = 1;
			else ret = l/r;
		}
		else if (root->value.compare("^") == 0)
		{
			double l = calculate(root->left, i);
			if (l != l) {
				return sqrt(-1.0);
			}
			double r = calculate(root->right, i);
			if (r != r) return sqrt(-1.0);
			if (l > 1.5 && r > 34) ret = sqrt(-1.0);
			else ret = pow(l,r);
		}
		else
		{
			cout << "calculate:  wrong variable...  " << root->value << endl;
		}
		return ret;
	}
}

string CalTree::get_postfix()
{
	return get_postfix(tree_stack.top());
}

string CalTree::get_postfix(TreeNode* root)
{
	if (root == nullptr) return "";
	string a;
	string b;
	if (root->left) {
		a = get_postfix(root->left) + " ";
	}
	else {
		a = "";
	}
	if (root->right) {
		b = get_postfix(root->right) + " ";
	}
	else {
		b = "";
	}
	return a + b + root->value;
}
