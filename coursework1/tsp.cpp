#include <iostream>
#include <fstream>
#include "tsp.hpp"
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <cctype>

using namespace std;
int num_vertices;


int main(int argc, char** argv)
{
	struct graph *G = fileread(argv[1]);
	//printf("\n%d\n", (int)((G->v)[11848])->x);
	sort (G->e, G->e + num_vertices * (num_vertices-1) / 2, edge_cmp);
	printf("1\n");
	//printf("e[0]: %d, %d, %f\n", (int)((G->e)[0]->a), (int)((G->e)[0]->b), ((G->e)[0]->weight));

	struct edge current;
	int mst_edges = num_vertices-1;
	float mst_weight=0.0;

	int a;
	int b;

	int *ret = new int[num_vertices+1];
	int *ret_random_search = new int[num_vertices+1];
	int *ret_after_search = new int[num_vertices+1];

	int i;
	DisjointSets ds(num_vertices);
	// mst
	for (i = 0; i < num_vertices * (num_vertices - 1) / 2; i++)
	{
		if (mst_edges == 0) break;
		a = (int)G->e[i]->a;
		b = (int)G->e[i]->b;
		int set_u = ds.find(a);
		int set_v = ds.find(b);

		if (set_u != set_v)
		{
			mst_weight += G->e[i]->weight;
			ds.merge(set_u, set_v);
			mst_edges--;
			ds.insert_edge(G->e[i]);
		}
	}
	printf("mst_edges: %d, mst_weight: %d\n", num_vertices-1-mst_edges, (int)mst_weight);
	printf("num_edges: %d\n", ds.num_edges());
	printf("count: %d\n", ds.check());
	printf("mst_weight: %d\n", ds.total_weight());
	// matching
	
	// mst + matching

	// euler
	//vector<struct edge*> temp_edges = ds.extract_edges();
	//printf("temp_edges: %d\n", (int)temp_edges.size());
	
	Euler euler(ds.extract_edges());
	euler.dfs();
	euler.euler_circuit();
	ret = euler.extract_euler();
	memcpy(ret_random_search,ret,sizeof(int)*num_vertices);
	memcpy(ret_after_search,ret,sizeof(int)*num_vertices);
	printf("%d\n", ret_after_search[5]);

	float total_w = 0.0;

	for (i = 0; i < num_vertices - 1; i++)
	{
		total_w += distance(G->v[ret[i]], G->v[ret[i+1]]);
	}
	total_w += distance(G->v[ret[num_vertices-1]], G->v[ret[0]]);

	float total_w_random_search;
	float total_w_after_search = total_w;
	for (int iter1 = 0; iter1 < num_vertices; iter1++)
	{
		for (int iter2 = 0; iter2 < iter1; iter2++)
		{
			total_w_random_search = 0.0;
			//ret_random_search = ret;
			memcpy(ret_random_search,ret_after_search,sizeof(int)*num_vertices);
			//memcpy(ret_random_search,ret,sizeof(int)*num_vertices);
			ret_random_search[iter1] = ret[iter2];
			ret_random_search[iter2] = ret[iter1];
			/*
			for (i = 0; i < num_vertices-1; i++)
			{
				total_w_random_search += distance(G->v[ret_random_search[i]],G->v[ret_random_search[i+1]]);
			}
			total_w_random_search += distance(G->v[ret_random_search[num_vertices-1]], G->v[ret_random_search[0]]);
			if (total_w > total_w_random_search)
			{
				total_w = total_w_random_search;
				ret_after_search = ret_random_search;
			}
			*/
			if (iter2 == 0)
			{
				total_w_random_search = total_w - distance(G->v[ret[iter1]],G->v[ret[iter1-1]]) - distance(G->v[ret[iter1]],G->v[ret[iter1+1]]) - distance(G->v[ret[iter2]],G->v[ret[iter2+1]]) - distance(G->v[ret[iter2]],G->v[ret[num_vertices-1]]);
				total_w_random_search += distance(G->v[ret[iter2]],G->v[ret[iter1-1]]) + distance(G->v[ret[iter2]],G->v[ret[iter1+1]]) + distance(G->v[ret[iter1]],G->v[ret[iter2+1]]) + distance(G->v[ret[iter1]],G->v[ret[num_vertices-1]]);				
			}
			else if (iter1 == num_vertices-1)
			{
				total_w_random_search = total_w - distance(G->v[ret[iter1]],G->v[ret[iter1-1]]) - distance(G->v[ret[iter1]],G->v[ret[0]]) - distance(G->v[ret[iter2]],G->v[ret[iter2+1]]) - distance(G->v[ret[iter2]],G->v[ret[iter2-1]]);
				total_w_random_search += distance(G->v[ret[iter2]],G->v[ret[iter1-1]]) + distance(G->v[ret[iter2]],G->v[ret[0]]) + distance(G->v[ret[iter1]],G->v[ret[iter2+1]]) + distance(G->v[ret[iter1]],G->v[ret[iter2-1]]);				
			}
			else if (iter1 == iter2 + 1)
			{
				total_w_random_search = total_w - distance(G->v[ret[iter1]],G->v[ret[iter1+1]]) - distance(G->v[ret[iter2]],G->v[ret[iter2-1]]) + distance(G->v[ret[iter2]],G->v[ret[iter1+1]]) + distance(G->v[ret[iter1]],G->v[ret[iter2-1]]);
			}
			else
			{
				total_w_random_search = total_w - distance(G->v[ret[iter1]],G->v[ret[iter1-1]]) - distance(G->v[ret[iter1]],G->v[ret[iter1+1]]) - distance(G->v[ret[iter2]],G->v[ret[iter2+1]]) - distance(G->v[ret[iter2]],G->v[ret[iter2-1]]);
				total_w_random_search += distance(G->v[ret[iter2]],G->v[ret[iter1-1]]) + distance(G->v[ret[iter2]],G->v[ret[iter1+1]]) + distance(G->v[ret[iter1]],G->v[ret[iter2+1]]) + distance(G->v[ret[iter1]],G->v[ret[iter2-1]]);				
			}
			if (total_w_after_search > total_w_random_search)
			{
				total_w_after_search = total_w_random_search;
				printf("iter1: %d, iter2: %d, total_w: %f\n", iter1, iter2, total_w_after_search);
				//ret_after_search = ret_random_search;
				memcpy(ret_after_search,ret_random_search,sizeof(int)*num_vertices);
				memcpy(ret,ret_random_search,sizeof(int)*num_vertices);
				iter1 = 0;
				iter2 = 0;
			}
		}
	}
	total_w = 0.0;
	for (i = 0; i < num_vertices - 1; i++)
	{
		total_w += distance(G->v[ret_after_search[i]], G->v[ret_after_search[i+1]]);
	}
	total_w += distance(G->v[ret_after_search[num_vertices-1]], G->v[ret_after_search[0]]);
	printf("total_weight: %f\n", total_w);

	// remove repeated vertices

	FILE *outfile = fopen("a.txt", "w");
	for (i = 0; i < num_vertices; i++)
	{
		fprintf(outfile, "%d\n", ret_after_search[i]+1);
	}
	fclose(outfile);

	return 0;
}
	
struct graph* fileread(char* name)
{
	FILE *fp = fopen(name, "r");
	struct graph* G = (struct graph *)malloc(sizeof(struct graph *));

	char line[50];
	char *ptr;

	fgets(line, 50, fp);
	fgets(line, 50, fp);
	fgets(line, 50, fp);
	fgets(line, 50, fp);
	
	char *temp = (char*)malloc(13);
	memcpy(temp, line+12, strlen((char*)line+12));
	num_vertices = atoi(temp);
	printf("%d\n", num_vertices);
	struct vertex **V = (struct vertex **)malloc(sizeof(struct vertex *) * num_vertices);
	struct edge **E = (struct edge **)malloc(sizeof(struct edge *) * num_vertices * (num_vertices - 1));


	fgets(line, 50, fp);
	fgets(line, 50, fp);
	
	int i=0;
	
	char buffer[50];
	sprintf(buffer, "%d", i);
	for (i=0; i<num_vertices; i++)
	{
		fgets(line, 50, fp);
		ptr = strtok(line, " ");
		ptr = strtok(NULL, " ");
		V[i] = (struct vertex *)malloc(sizeof(struct vertex *));
		V[i]->x = (long)atof(ptr);
		ptr = strtok(NULL, " ");
		V[i]->y = (long)atof(ptr);
		V[i]->nth = i;
		//printf("%d.. x: %7d, y: %7d\n", i, (int)V[i]->x, (int)V[i]->y);
	}
	fgets(line,50,fp);
	printf("%d.. x: %7d, y: %7d\n", 20, (int)V[20]->x, (int)V[20]->y);
	printf("E[1]: %f\n", distance(V[1], V[0]));
	int j=0, k=0;
	for (i=0; i<num_vertices; i++)
	{
		for (j=0; j<num_vertices; j++)
		{
			if (i == j) 
			{
				break;
			}
			E[k] = (struct edge *)malloc(sizeof(struct edge *));
			E[k]->a = i;
			E[k]->b = j;
			E[k]->weight = distance(V[i], V[j]); 
			//printf("E[%d]: %f\n", k, E[k]->weight);
			k++;
		}
	}
	printf("E[10000]: %d, %d, %f\n", (int)E[10000]->a, (int)E[10000]->b, E[10000]->weight);

	printf("k: %d\n", k);
	G->v = V;
	G->e = E;

	return G;
}

float distance(struct vertex *a, struct vertex *b)
{
	return sqrt( pow((a->x-b->x),2) + pow((a->y-b->y),2) );
}
bool edge_cmp (struct edge *a, struct edge *b)
{
	//printf("a weight %d, b weigth %d\n", (int)a->weight, (int)b->weight);
	return a->weight < b->weight;
}
