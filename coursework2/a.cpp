#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
#include <sstream>
#include <fstream>

using namespace std;

vector<string> split(const string& str, int delimiter(int) = ::isspace){
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

int main(void)
{
	
	string line;
	getline(cin, line);
	cout << line << endl;
	cout << line.size() << endl;

	vector<string> result = split(line);

	for (auto const s: result)
	{
		cout<<s<<endl;
	}
	string str = "1,2,3,4,5,6";
	vector<int> vect;

	stringstream ss(str);

	int i;

	while(ss >> i)
	{
		vect.push_back(i);

		if (ss.peek() == ',')
			ss.ignore();
	}
	for (i=0; i<vect.size(); i++)
	{
		cout << vect.at(i) << endl;
	}
	double a = sqrt(-1.0);
	if (a != a) cout << a << endl;
}


