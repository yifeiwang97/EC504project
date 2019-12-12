#include <stdio.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <string>
#include <stack>

using namespace std;

#define n 2261426
//#define n 16
//#define n 4546
#define num_sample 3
#define Kmax 10

int num_list = 0;


struct node
{
	double x;
	double y;
	string state;
	string county;
	bool visit;
	int divide;
	struct node *parent;
	struct node *left;
	struct node *right;
};
struct node nnlist[Kmax];
struct node Nodes[n];
void swap(struct node *a, struct node *b)
{
	struct node temp;
	temp.state = a->state;
	temp.county = a->county;
	temp.divide = a->divide;
	temp.x = a->x;
	temp.y = a->y;
	temp.left = a->left;
	temp.right = a->right;
	temp.visit = a->visit;
	temp.parent = a->parent;
	a->state = b->state;
	a->county = b->county;
	a->divide = b->divide;
	a->x = b->x;
	a->y = b->y;
	a->left = b->left;
	a->right = b->right;
	a->visit = b->visit;
	a->parent = b->parent;
	b->state = temp.state;
	b->county = temp.county;
	b->divide = temp.divide;
	b->x = temp.x;
	b->y = temp.y;
	b->left = temp.left;
	b->right = temp.right;
	b->visit = temp.visit;
	b->parent = temp.parent;
}

double dist(struct node *a, struct node *b)
{
	double x = (b->y-a->y)*cos(0.5*(a->x + b->x));
	double y = a->x - b->x;
	double R = 6371;
	double aa = sqrt(pow(x,2)+pow(y,2));
	return sqrt(pow(x,2)+pow(y,2))*R;
}

struct node *partition(struct node*start, struct node *end, int divide)
{
	struct node *pivot = end - 1;
	struct node *cur = start;
	struct node *i;
	struct node *j;
	for(i = start; i < end - 1; i++)
	{
		if(divide == 0)
		{
			if(pivot->x > i->x)
			{
				swap(cur, i);
				cur++;
			}
		}
		else 
		{
			if(pivot->y >i->y)
			{
				swap(cur, i);
				cur++;
			}
		}
	}
	swap(cur, end - 1);
	return cur;
}


void quick(struct node*start, struct node*end, int divide)
{
	stack<struct node*> s;
	if( start < end - 1)
	{
		struct node *boundary = partition(start, end, divide);
		
		if( boundary-1 > start) 
		{
			s.push(start);
			s.push(boundary-1);
		}
		if( boundary+1 < end - 1 )
		{
			s.push(boundary+1);
			s.push(end - 1);
		}
		
		while( !s.empty() )
		{
			struct node *r = s.top();
			s.pop();  
			struct node *l = s.top();
			s.pop();
			
			boundary = partition(l,r, divide);
			if( boundary-1>l )
		    { 
		    	s.push(l);
		    	s.push(boundary-1);
	    	}
		    if( boundary+1<r ) 
	     	{
	 	    	s.push(boundary+1);
	    		s.push(r);
	    	}
		}
	}
}


struct node *median(struct node *start, struct node *end, int divide)
{
	srand(rand());
	struct node *sample[num_sample];
	int len = end - start;
	for(int i = 0; i < num_sample; i++)
	{
		int pivot = rand() % len;
		sample[i] = start + pivot;
	}
	for(int i = 0; i<num_sample; i++)
	{
		for(int j = 0; j<num_sample-1; j++)
		{
			if(divide == 0)
			{
				if(sample[j]->x > sample[j+1]->x)
					swap(sample[j], sample[j+1]);
			}
			else
			{
				if(sample[j]->y > sample[j+1]->y)
					swap(sample[j], sample[j+1]);
			}
		}
	}
	return sample[num_sample/2];

}

struct node *part2(struct node *start, struct node *end, int divide, struct node *pivot)
{
	swap(start, pivot);
	struct node *i = start;
	struct node *j = end;
    struct node *key = start;
    while (true)
    {
		if(divide==0)
		{
			while ((++i)->x < key->x)
			{
				if (i == end)
				{
					break;
				}
			}
			while ((--j)->x > key->x)
			{
				if (j == start)
				{
					break;
				}
			}
			if (i >= j) break;
			swap(i,j);
		}
		else
		{
			while ((++i)->y < key->y)
			{
				if (i == end)
				{
					break;
				}
			}
			while ((--j)->y > key->y)
			{
				if (j == start)
				{
					break;
				}
			}
			if (i >= j) break;
			swap(i,j);
		}
	}
	swap(start, j);
	return j;
}

struct node *construct(struct node *a, int len, int divide)
{
	struct node *T;
	struct node *temp;
	if(len==0)
		return NULL;
	if(len > num_sample)
	{
		T = median(a, a + len, divide);
		T = part2(a, a + len, divide, T);
	}
	else
	{
		quick(a, a+len, divide);
		T = a + (len-1)/2;
	}
	T->divide = divide;
	divide = (divide + 1) % 2;
	T->left = construct(a, T - a, divide);
	//if(T->left)
		//T->left->parent = T;
	T->right = construct(T+1, a + len - (T + 1), divide);
	//if(T->right)
		//T->right->parent = T;
	return T;
}

void add_list(struct node *p, struct node *target)
{
	if(num_list == 0)
		nnlist[Kmax-1] = *p;
	else if(num_list == Kmax)
	{
		if(dist(nnlist+Kmax-1, target) > dist(p, target))
			nnlist[Kmax-1] = *p;
		for(int i=0;i<Kmax-1;i++)
		{
			if(dist(nnlist+Kmax-1,target) < dist(nnlist+i,target))
				swap(nnlist+Kmax-1,nnlist+i);
		}
	}
	else
	{
		nnlist[Kmax - 1 - num_list] = *p;
		if(dist(nnlist+Kmax-1, target) < dist(p, target))
		{
			swap(nnlist+Kmax-1, nnlist + Kmax -1-num_list);
		}
	}
	if(num_list < Kmax)
		num_list++;
}

void build_nnlist(struct node *T, struct node *target)
{
	double dtemp;
	if(T == NULL)
		return;
	if(T->divide == 0)
	{
		if(target->x < T->x)
			build_nnlist(T->left, target);
		else
			build_nnlist(T->right,target);
	}
	else if(T->divide == 1)
	{
		if(target->y < T->y)
			build_nnlist(T->left, target);
		else
			build_nnlist(T->right,target);
	}
	if(T->left == NULL && T->right ==NULL)
	{
		T->visit =true;
		add_list(T, target);
	}
	if(T->parent)
		return;
	if(T->visit == true)
		return;
	T->visit = true;
	if(T->divide == 0)
	{
		add_list(T, target);
		if(abs(T->x-target->x) >= dist(nnlist+Kmax-1, target) && num_list < Kmax)
			return;
		else
		{
			if(target->x < T->x)
				build_nnlist(T->right, target);
			else
				build_nnlist(T->left, target);
		}
	}
	else if(T->divide == 1)
	{
		add_list(T, target);
		if(abs(T->y-target->y) >= dist(nnlist+Kmax-1, target) && num_list < Kmax)
			return;
		else
		{
			if(target->y < T->y)
				build_nnlist(T->right, target);
			else
				build_nnlist(T->left, target);
		}
	}
}

void sortlist(struct node *target)
{
	int i,j;
	for(i=0;i<Kmax-1;i++)
	{
		for(j=0;j<Kmax-1;j++)
		{
			if(dist(nnlist+j, target) > dist(nnlist+j+1, target))
				swap(nnlist+j,nnlist+j+1);
		}
	}
}
void printlist(struct node *target, int k)
{
	cout<<endl<<"K("<<k<<")nearest points are:"<<endl;
	for(int i=0;i<k;i++)
		cout<<(nnlist+i)->state<<"  "<<(nnlist+i)->county<<"  distance(km):"<<dist(nnlist+i,target)<<endl;
	cout<<endl<<"nearest point is:"<<endl;
	cout<<nnlist->state<<"  "<<nnlist->county<<"  ("<<nnlist->x<<","<<nnlist->y<<")  distance(km):"<<dist(nnlist,target)<<endl;
}

void vote()
{
	int state_count[5]={0};
	int county_count[5]={0};
	int i,j,state_max=0,state_pivot,county_max=0,county_pivot;
	for(i=0;i<5;i++)
	{
		for(j=0;j<5;j++)
		{
			if(nnlist[j].state == nnlist[i].state)
				state_count[i]++;
			if(nnlist[j].county == nnlist[i].county)
				county_count[i]++;
		}
	}
	for(i=0; i<5;i++)
	{
		if(state_count[i] > state_max)
		{
			state_max = state_count[i];
			state_pivot = i;
		}
		if(county_count[i] > county_max)
		{
			county_max = county_count[i];
			county_pivot = i;
		}
	}
	cout<<endl<<"majority vote result:"<<endl;
	cout<<nnlist[state_pivot].state<<"  "<<nnlist[county_pivot].county<<endl;
}

void clearnnlist()
{
	num_list = 0;
}

//preorder
void clearkdtree(struct node *T)
{
	if(T)
	{
		T->visit = false;
		clearkdtree(T->left);
		clearkdtree(T->right);
	}
}

int main()
{
	//read file
	clock_t start, end;
	ifstream f;
	char *fpath="NationalFile.txt";
	string state, county, sx, sy,temp;
	double x,y;
	
	
	struct node *T;
	struct node target = {0,0,"","",false,NULL,NULL,NULL};
    f.open(fpath,ios::in);
    if (!f.is_open()) 
	{
	  printf("Did not find input file \n");
	  exit(1);
    }
	cout<<"read txt dataset...start"<<endl;
	start = clock();
	getline(f, temp);
	for(int i=0;true;i++)
	{
		if(!getline(f, state, '\t'))
			break;
		getline(f, county,'\t');
		getline(f, sx, '\t');
		getline(f, sy,'\n');
		
		Nodes[i].county = county;
		Nodes[i].state = state;
		Nodes[i].x = stod(sx);
		Nodes[i].y = stod(sy);
		Nodes[i].visit = false;
		Nodes[i].left, Nodes[i].right, Nodes[i].parent = NULL, NULL, NULL;
		//cout<<state<<" "<<county<<" "<<sx<<" "<<sy<<endl;
	}
	f.close();
	cout<<"load txt dataset...complete"<<endl;
	end = clock();
	cout<<"data loading time(s):"<<(end - start)/double(1000000)<<endl<<endl;


	//construct kd-tree
	cout<<"construct kd-tree...start"<<endl;
	start = clock();
	T = construct(Nodes, n, 0);
	T->parent = T;
	end = clock();
	cout<<"construct kd-tree...complete"<<endl;
	cout<<endl<<"kd-tree construction time(s):"<<(end - start)/double(1000000)<<endl;
  while(true)
  {
    clearkdtree(T);
		clearnnlist();
	//query and search
		//input target coordinates
		cout<<endl<<"input latitude of your searching point:  ";
		cin>>target.x;
		while(target.x<-90 || target.x > 90)
		{
			cout<<"latitude should be -90~90, please correct your input!";
			cout<<endl<<"input latitude of your searching point:  ";
			cin>>target.x;
		}

		cout<<endl<<"input longitude of your searching point:  ";
		cin>>target.y;
		while(target.y<-180 || target.y > 180)
		{
			cout<<"longitude should be -180~180, please correct your input!"<<endl;
			cout<<endl<<"input longitude of your searching point:  ";
			cin>>target.y;
		}
	
		//search knn
		start = clock();
		build_nnlist(T, &target);
		end = clock();
		cout<<endl<<"kd-tree search time(s):"<<(end - start)/double(1000000)<<endl;

		//sort list and print result
		sortlist(&target);
		printlist(&target, Kmax);
		vote();
   }
		return 0;
}