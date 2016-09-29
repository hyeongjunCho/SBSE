#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <list>
#include <algorithm>
#include <random>

struct SSF
{
	unsigned short a;
	unsigned short b;
	float c;
};

int *foo()
{
	int *a = (int*)malloc(sizeof(int*));
	*a = 300;
	return a;
}

int main ()
{
	struct SSF ssf;

	ssf.a=1;
	ssf.b=2;
	ssf.c=1.1;

	std::cout << sizeof(ssf) << std::endl;
	

	int *b = foo();

	printf("%d\n", *b);
	printf("%f\n", sqrt(pow(4,2)+pow(4,2)));

	std::cout << sqrt(pow(4,2)) << std::endl;

	std::cout << "list test" << std::endl;

	std::list<int> alist;
	std::list<int>::iterator it;

	alist.push_back(1);
	alist.push_back(3);
	alist.push_back(4);
	alist.push_back(5);
	alist.push_back(6);
	alist.push_back(7);
	alist.push_back(8);
	alist.push_back(9);
	alist.push_back(10);

	for (auto i : alist)
	{
		std::cout << i << std::endl;
	}
	it = alist.begin();
	std::advance(it,1);
	alist.insert(it, 2);
	for (auto i : alist)
	{
		std::cout << i << std::endl;
	}
	std::vector<int> temp(alist.begin(), alist.end());
	std::random_device rd;
	std::mt19937 g(rd());
	for (auto i : temp)
	{
		std::cout << i << std::endl;
	}

	std::random_shuffle(std::next(temp.begin(),1),std::prev(temp.end(),1));
	std::copy(temp.begin(),temp.end(),alist.begin());
	for (auto i : alist)
	{
		std::cout << i << std::endl;
	}


	return 0;
}
