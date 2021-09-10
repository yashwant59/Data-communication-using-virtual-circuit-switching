// floyd_warshal.h : Defines the entry point for the console application.
//


#include<bits/stdc++.h>
using namespace std;
typedef struct node{	int destination;			//defining graph nodes
						int weight;
						struct node *next;
					}node;

typedef struct {	node *head;			//defining graph adjacency list
					}list;

typedef struct {	/*list *a;
						char *colour;			
						int *distance;
						int *pie;*/
					vector<vector<int>> pie,wtmatrix;
					
						int V,E;
					}graph;
/*
graph* input_graph1(unordered_map<int ,vector<int>>,int,int);	
void print_output(graph*);
void floyd_warshal(graph*);
int min(int,int);
void print_path(graph*,int,int);

void output_shortest_path(graph);*/

void print_path(graph *g,int i,int j,vector<int> &v)
{
	if(i==j){
		//printf(" %d",i);
		v.push_back(i);
		return;
	}

		print_path(g,i,g->pie[i][j],v);
		//printf(" %d",j);
		v.push_back(j);
}


unordered_multimap<int,pair<int,vector<int>>> output_shortest_path(graph *g,unordered_multimap<int,pair<int,vector<int>>> &m)
{
	int i=0,j=0,k=0;

	for(i=0;i<g->V;i++)
	{
		for(j=0;j<g->V;j++)
		{
			if(g->wtmatrix[i][j]>=10000)
				//printf("\nnode %d   to node %d -----distance infinite \n",i,j);
				continue;
			else
			{	//printf("\nnode %d   to node %d -----distance %d  --------predeccessor %d\n",i,j,g->wtmatrix[i][j],g->pie[i][j]);
				vector<int> v;
				
				if(i!=j)
				{	print_path(g,i,j,v);

					m.insert(make_pair(i,make_pair(j,v)));
				}
			}

			
		}
	}

	g->pie.clear();
	//delete &(g->pie);
	//delete &(g->wtmatrix);
	g->wtmatrix.clear();
	delete g;
	return m;

}





graph* input_graph1(unordered_multimap<int,vector<int>> &m,int V,int E,int metric)			//This function is used to take graph as input.
{

	int n=0,e=0,u=0,v=0,wt=0;
	graph *g=(graph*)calloc(1,sizeof(graph));

	
	

	g->V=V;
	g->E=E;
	//g->n=n;
	/*
	g->wtmatrix=(int**)calloc(V+1,sizeof(int));
	g->pie=(int**)calloc(V+1,sizeof(int));
	for(int i=0;i<=V;i++)
	{
		
		g->wtmatrix[i]=(int*)calloc(V+1,sizeof(int));
		g->pie[i]=(int*)calloc(V+1,sizeof(int));
		
	}*/
	
	
	for(int i=0;i<g->V;i++)
	{
		vector<int> v,v1;
		for(int j=0;j<g->V;j++)
		{
			v.push_back(0);
			v1.push_back(0);
		}
		g->pie.push_back(v);
		g->wtmatrix.push_back(v1);

	}

	
	
	
	for(int i=0;i<g->V;i++)
	{
		for(int j=0;j<g->V;j++)
		{
			if(i==j)
				g->wtmatrix[i][j]=0;
			else
				g->wtmatrix[i][j]=10000;

			g->pie[i][j]=i;
		}
	}

	for(auto i=m.begin();i!=m.end();i++)			//for undirected graph only
	{
		u=i->first;
		v=*i->second.begin();
		if(metric==1)
		{
			g->wtmatrix[u][v]=*(i->second.begin()+1);
		g->wtmatrix[v][u]=*(i->second.begin()+1);
		}
		else
		{
			g->wtmatrix[u][v]=1;
			g->wtmatrix[v][u]=1;
		}
		
		
		g->pie[u][v] = u;
		g->pie[v][u]=v;
	}
	return g;
}

int min(int a,int b)
{
	if(a<b)
		return a;
	else
		return b;
}

void floyd_warshal(graph* g)
{
	int i=0,k=0,j=0;
	for( k=0;k<g->V;k++)
	{

		for( i=0;i<g->V;i++)
		{
			for(j=0;j<g->V;j++)
			{
				if(g->wtmatrix[i][j]>g->wtmatrix[i][k]+g->wtmatrix[k][j])
				{	g->pie[i][j]=g->pie[k][j];
				
					g->wtmatrix[i][j]=g->wtmatrix[i][k]+g->wtmatrix[k][j];
				}
			}
		}
	}

}
