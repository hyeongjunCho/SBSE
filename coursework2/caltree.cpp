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
			num_t++;
			t = new TreeNode(s);
			t->valuetype = 0;
			//tree_stack.push(t);
			return t;
		} else {
			TreeNode* t;
			int num = random_int(num_var-1)+1;
			string s = "x" + to_string(num);
			//cout << s << endl;
			num_t++;
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
			num_nt++;
			w = new TreeNode(s);
			w->valuetype = 2;
			if (max_depth > 3) {
				w->left = randomTree(max_depth - 1 - random_int(max_depth-1-3));
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
			num_nt++;
			w = new TreeNode(s);
			w->valuetype = 3;
			if (max_depth > 3) {
				w->left = randomTree(max_depth - 1 - random_int(max_depth-1-3));
				w->right = randomTree(max_depth - 1 - random_int(max_depth-1-3));
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
	num_nt = -1; //except root
	num_t = 0;
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
				num_t++;
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
			num_t++;
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
				num_nt++;
				tree_stack.pop();
				tree_stack.push(w);
			}
			else
			{
				TreeNode* w = new TreeNode(s);
				w->valuetype = 2;
				w->left = tree_stack.top();
				num_nt++;
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
			if (abs(l) < 0.000001) {
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
			if (abs(l) < 0.000001) {
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
			if (abs(l) < 0.000001) {
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
			if (abs(l) < 0.000001) {
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
			if (abs(l) < 0.000001) {
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
			if (abs(l) < 0.000001) {
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
			if (abs(l) < 0.000001) {
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
			if (abs(l) < 0.000001) {
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
			if (abs(l) < 0.000001) {
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
			if (abs(l) < 0.000001) {
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
			if (abs(l) < 0.000001) {
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
			if (abs(l) < 0.000001) {
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
			if (abs(l) < 0.000001) {
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
			if (abs(l) < 0.000001) {
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
			if (abs(l) < 0.000001) {
				return sqrt(-1.0);
			}
			double r = calculate(root->right, i);
			if (r != r) return sqrt(-1.0);
			if (abs(r) < 0.000001) {
				return sqrt(-1.0);
			}
			ret = l+r;
		}
		else if (root->value.compare("-") == 0)
		{
			double l = calculate(root->left, i);
			if (l != l) {
				return sqrt(-1.0);
			}
			if (abs(l) < 0.000001) {
				return sqrt(-1.0);
			}
			double r = calculate(root->right, i);
			if (r != r) return sqrt(-1.0);
			if (abs(r) < 0.000001) {
				return sqrt(-1.0);
			}
			ret = l-r;
		}
		else if (root->value.compare("*") == 0)
		{
			double l = calculate(root->left, i);
			if (l != l) {
				return sqrt(-1.0);
			}
			if (abs(l) < 0.000001) {
				return sqrt(-1.0);
			}
			double r = calculate(root->right, i);
			if (r != r) return sqrt(-1.0);
			if (abs(r) < 0.000001) {
				return sqrt(-1.0);
			}
			ret = l*r;
		}
		else if (root->value.compare("/") == 0)
		{
			double l = calculate(root->left, i);
			if (l != l) {
				return sqrt(-1.0);
			}
			if (abs(l) < 0.000001) {
				return sqrt(-1.0);
			}
			double r = calculate(root->right, i);
			if (r != r) return sqrt(-1.0);
			if (abs(r) < 0.000001) ret = 1;
			else ret = l/r;
		}
		else if (root->value.compare("^") == 0)
		{
			double l = calculate(root->left, i);
			if (l != l) {
				return sqrt(-1.0);
			}
			if (abs(l) < 0.000001) {
				return sqrt(-1.0);
			}
			double r = calculate(root->right, i);
			if (r != r) return sqrt(-1.0);
			if (abs(r) < 0.000001) {
				return sqrt(-1.0);
			}
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

TreeNode* CalTree::tree_top()
{
	return tree_stack.top();
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

vector<CalTree*> next_generation(vector<CalTree*> generation)
{
	double sum_mse = 0.0;
	vector<double> ratios;
	double sum_ratios = 0.0;
	for (auto t : generation) {
		sum_mse += t->mse;
	}
	//cout << "sum_mse  " << sum_mse << endl;
	for (auto t : generation) {
		ratios.push_back(t->mse/sum_mse);
		sum_ratios += ratios.back();
	}
	for (int i=0; i<generation.size(); i++) {
		ratios[i] /= sum_ratios;
	}
	vector< pair<int,int> > pairs;
	pairs = choose_pairs(ratios); //200 pairs -> 400 child
	//cout << "num_pairs  " << pairs.size() << endl;

	vector<CalTree*> new_generation;
	int i = 0;
	while (1) {
		//cout << "ith  " << i << "  first  " << pairs[i].first << "  second  " << pairs[i].second << endl;
		CalTree* tree1 = generation[pairs[i].first];
		CalTree* tree2 = generation[pairs[i].second];
		float expected1 = random_real(1.1)-0.1;
		float expected2 = random_real(1.1)-0.1;
		//cout << tree1->get_postfix() << endl;
		//cout << tree2->get_postfix() << endl;
		auto pair1 = tree1->pick_node(nullptr, tree1->tree_top(), 0, 0, expected1);
		auto pair2 = tree2->pick_node(nullptr, tree2->tree_top(), 0, 0, expected2);
		TreeNode* temp_node1 = pair1.first.second;
		TreeNode* temp_node2 = pair2.first.second;
		
		bool tree1_left = false;
		bool tree2_left = false;
		if (pair1.first.first->left == temp_node1)
		{
			pair1.first.first->left = temp_node2;
			tree1_left = true;
		}
		else
		{
			pair1.first.first->right = temp_node2;
		}
		if (pair2.first.first->left == temp_node2)
		{
			pair2.first.first->left = temp_node1;
			tree2_left = true;
		}
		else
		{
			pair2.first.first->right = temp_node1;
		}
		string expr1 = tree1->get_postfix();
		string expr2 = tree2->get_postfix();

		CalTree* tree3 = new CalTree(false);
		tree3->make_tree(expr1);
		CalTree* tree4 = new CalTree(false);
		tree4->make_tree(expr2);

		if (tree1_left)	{
			pair1.first.first->left = temp_node1;
		} else {
			pair1.first.first->right = temp_node1;
		}
		if (tree2_left) {
			pair2.first.first->left = temp_node2;
		} else {
			pair2.first.first->right = temp_node2;
		}
		double val1 = 0.0;
		double val2 = 0.0;
		for (int j=0; j<num_vector; j++) {
			val1 += tree3->calculate(j);
			val2 += tree4->calculate(j);
		}
		if (val1 != val1 || val1 > 100 * num_vector || val2 != val2 || val2 > 100 * num_vector || val1 < -100 * num_vector || val2 < -100 * num_vector) {
			free(tree3);
			free(tree4);
			pairs[i] = choose_pair(ratios);
			continue;
		}
			
		new_generation.push_back(tree3);
		new_generation.push_back(tree4);
		i++;
		if (i >= pairs.size()) break;
	}
	return new_generation;
}

vector< pair<int,int> > choose_pairs(vector<double> ratios)
{
	vector< pair<int,int> > pairs;
	for (int i=0; i<200; i++) {
		float val1 = random_real(1.1)-0.1;
		float val2 = random_real(1.1)-0.1;
		int v1 = -1;
		int v2 = -1;
		double current_ratio_sum = 0.0;
		for (int j=0; j<ratios.size(); j++) {
			if (v1 < 0 && current_ratio_sum + ratios[j] > (double)val1) {
				v1 = j;
			}
			if (v2 < 0 && current_ratio_sum + ratios[j] > (double)val2) {
				v2 = j;
			}
			if (v1 >= 0 && v2 >= 0) break;
			current_ratio_sum += ratios[j];
		}
		pairs.push_back(make_pair(v1,v2));
	}
	return pairs;
}

pair<int,int> choose_pair(vector<double> ratios)
{
	pair<int,int> g_pair;
	float val1 = random_real(1.1)-0.1;
	float val2 = random_real(1.1)-0.1;
	int v1 = -1;
	int v2 = -1;
	double current_ratio_sum = 0.0;
	for (int j=0; j<ratios.size(); j++) {
		if (v1 < 0 && current_ratio_sum + ratios[j] > (double)val1) {
			v1 = j;
		}
		if (v2 < 0 && current_ratio_sum + ratios[j] > (double)val2) {
			v2 = j;
		}
		if (v1 >= 0 && v2 >= 0) break;
		current_ratio_sum += ratios[j];
	}
	g_pair = make_pair(v1,v2);
	return g_pair;
}

pair< pair<TreeNode*,TreeNode*>, pair<int,int> > CalTree::pick_node(TreeNode* parent, TreeNode* node, int nt, int t, float expected)
{	//pre-traversal
	//itself
	//left
	//right
	//cout << node->value << endl;
	
	if (parent == nullptr) {	//root
		auto a = pick_node(node, node->left, 0, 0, expected);
		if (a.first.second == nullptr)
		{
			if (node->valuetype == 2)
			{
				//cout << "pick_node error" << endl;
				//exit(1);
			}
			auto b = pick_node(node, node->right, a.second.first, a.second.second, expected);
			return b;
		}
		return a;
	}

	if (node->valuetype == 0 || node->valuetype == 1) {
		double sum_prob;
		if (num_nt == 0) {
			sum_prob = (double)(t+1) / (double)num_t;
		} else {
			sum_prob = (double)nt * (double)0.9 / (double)num_nt + (double)(t+1) * (double)0.1 / (double)num_t;
		}
		if (sum_prob >= expected)
		{
			pair<TreeNode*,TreeNode*> node_pair = make_pair(parent, node);
			pair<int,int> int_pair = make_pair(nt,t+1);
			return make_pair(node_pair, int_pair);
		}
		else
		{
			pair<TreeNode*,TreeNode*> node_pair = make_pair(nullptr,nullptr);
			pair<int,int> int_pair = make_pair(nt,t+1);
			return make_pair(node_pair, int_pair);
		}
	}
	else {
		if ((double)t * (double)0.1 / (double)num_t + (double)(nt+1) * (double)0.9 / (double)num_nt >= expected)
		{
			pair<TreeNode*,TreeNode*> node_pair = make_pair(parent, node);
			pair<int,int> int_pair = make_pair(nt+1,t);
			return make_pair(node_pair, int_pair);
		}
		else
		{
			auto a = pick_node(node, node->left, nt+1, t, expected);
			if (a.first.second == nullptr)
			{
				if (node->valuetype == 2)
				{
					return a;
				}
				auto b = pick_node(node, node->right, a.second.first, a.second.second, expected);
				return b;
			}
			return a;
		}
	}
}
