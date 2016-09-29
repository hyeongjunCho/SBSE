#ifndef tsp
#define tsp

#include <vector>
using namespace std;

struct vertex
{
	long x;
	long y;
	unsigned short nth;
};

struct edge
{
	unsigned short a;
	unsigned short b;
	float weight;
};

struct graph
{
	struct vertex **v;
	struct edge **e;
};

struct DisjointSets
{
    int *parent, *rnk;
    int n;
	vector<struct edge*> edges;
 
    // Constructor.
    DisjointSets(int n)
    {
        // Allocate memory
        this->n = n;
        parent = new int[n+1];
        rnk = new int[n+1];
 
        // Initially, all vertices are in
        // different sets and have rank 0.
        for (int i = 0; i <= n; i++)
        {
            rnk[i] = 0;
 
            //every element is parent of itself
            parent[i] = i;
        }
    }
 
    // Find the parent of a node 'u'
    // Path Compression
    int find(int u)
    {
        /* Make the parent of the nodes in the path
           from u--> parent[u] point to parent[u] */
        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
    }
 
    // Union by rank
    void merge(int x, int y)
    {
        x = find(x), y = find(y);
 
        /* Make tree with smaller height
           a subtree of the other tree  */
        if (rnk[x] > rnk[y])
            parent[y] = x;
        else // If rnk[x] <= rnk[y]
            parent[x] = y;
 
        if (rnk[x] == rnk[y])
            rnk[y]++;
    }

	void insert_edge(struct edge* e)
	{
		edges.push_back(e);
	}

	int num_edges()
	{
		return edges.size();
	}

	int check()
	{
		int *vertices = new int[n];
		fill_n(vertices, n, 0);
		for (auto temp_edge : edges) 
		{
			vertices[temp_edge->a] = 1;
			vertices[temp_edge->b] = 1;
		}
		int count=0;
		int i=0;
		for (i=0; i<n; i++)
		{
			if (vertices[i] == 0) count++;
		}
		return count;
	}

	int total_weight()
	{
		float ret = 0.0;
		for (auto edge : edges)
		{
			ret += edge->weight;
		}
		return ret;
	}

	vector<struct edge*> extract_edges()
	{
		return edges;
	}
};

struct Euler
{
	int *visited, *d, *f;
	vector<struct edge*> edges;
	int num_vertices;
	int time;

	vector<int> order;
	int *order_array;

	vector<int> *adj;

    Euler(vector<struct edge*> e)
	{
		num_vertices = e.size() + 1;
		cout << 1 << endl;
		visited = new int[num_vertices];
		fill_n(visited, num_vertices, 0);
		cout << 2 << endl;
		d = new int[num_vertices];
		fill_n(d, num_vertices, 0);
		f = new int[num_vertices];
		fill_n(f, num_vertices, 0);

		cout << 3 << endl;
		order_array = new int[num_vertices];

		edges=e;
		cout << 4 << endl;

		adj = new vector<int>[num_vertices];
		cout << 5 << endl;
		for (int i = 0; i < num_vertices-1; i++)
		{
			//cout << "a  " << edges[i]->a << endl;
			adj[(int)edges[i]->b].push_back((int)edges[i]->a);
			adj[(int)edges[i]->a].push_back((int)edges[i]->b);
		}
		cout << 6 << endl;
	}

	void dfs()
	{
		int u;
		time = 0;
		/*for (u = 0; u < num_vertices; u++)
		{
			if (visited[u] == 0) // white
			{
				dfs_visit(u);
			}
		}*/
		dfs_visit(0);
	}

	void dfs_visit(int u)
	{
		visited[u] = 1;
		order.push_back(u);
		time++;
		d[u] = time;
		for (auto v : adj[u])
		{
			if (visited[v] == 0)
			{
				dfs_visit(v);
			}
		}
		visited[u] = 2;
		time++;
		f[u] = time;
	}

	void euler_circuit()
	{
		int i = 0;
		for (auto temp : order)
		{
			order_array[i] = temp;
			i++;
		}
	}

	int *extract_euler()
	{
		return order_array;
	}
};

struct graph* fileread(char* name);
float distance(struct vertex *a, struct vertex *b);
bool edge_cmp (struct edge *a, struct edge *b);
#endif
