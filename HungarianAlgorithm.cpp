#include<iostream>
#include<stdio.h>
#include<math.h>
#include <string.h>
#include<algorithm>
#include <fstream>
std::ifstream infile("Data.txt");

using namespace std;

#define N 55 //Defining the maximum number of vertices in one part
#define INF 100000000 //Defining infinity

int weight[N][N]; //Initialising a matrix to manage and analyze weight, Cost matrix
int n, maximum_match; //defining the interger n for number of students and available slots
int lx[N], ly[N]; //Definining labels of X and Y parts
int xy[N]; //initiating an array that'll contain the vertex matched with X, xy[x]
int yx[N]; //initiating an array that'll contain the vertex matched with X, xy[x]
bool S[N], T[N]; //Defining two sets S and T in algorithm
int diff_vect[N]; //
int diff_x[N]; // Defining a diff_x[y] such a vertex, that
 // l(diff_x[y]) + l(y) - w(diff_x[y],y) = diff_vect[y]
int prev[N]; //An array that'll memorize alternate paths

void define_labels()
{
 memset(lx, 0, sizeof(lx));
 memset(ly, 0, sizeof(ly));
 for (int x = 0; x < n; x++)
 for (int y = 0; y < n; y++)
 lx[x] = max(lx[x], weight[x][y]);
}

void update()
{
    int x, y, diff = INF; //Defining an integer variable diff as infinity
    for (y = 0; y < n; y++) //calculation of diff using diff_vect
        if (!T[y])
            diff = min(diff, diff_vect[y]);

    for (x = 0; x < n; x++) //updating the X labels
        if (S[x]) lx[x] -= diff;

    for (y = 0; y < n; y++) //updating the  Y labels
        if (T[y]) ly[y] += diff;

    for (y = 0; y < n; y++) //A step to update diff_vect array
        if (!T[y])
            diff_vect[y] -= diff;
}

void add_to_tree(int x, int prevx)
//x - current vertex,prevx - vertex from X before x in the alternating path,
//so we add edges (prevx, xy[x]), (xy[x], x)
{
 S[x] = true; //adding  x to S
 prev[x] = prevx;
 for (int y = 0; y < n; y++) //updating the  slacks,as new vertex has been added to S
    if (lx[x] + ly[y] - weight[x][y] < diff_vect[y])
    {
        diff_vect[y] = lx[x] + ly[y] - weight[x][y];
        diff_x[y] = x;
    }
}

void augment() //main function of the algorithm
{

 if (maximum_match == n) return; //checking the perfection in the matching

 int x, y, root; //Initialising the counters and root vertex
 int q[N], wr = 0, rd = 0; //q - queue for Breadth first search, wr - write ,rd - read


 memset(S, false, sizeof(S)); //initializing  set S
 memset(T, false, sizeof(T)); //initializing  set T
 memset(prev, -1, sizeof(prev)); //Defining set prev - for the alternating tree
 for (x = 0; x < n; x++) //finding root of the tree
    if (xy[x] == -1)
    {
    q[wr++] = root = x;
    prev[x] = -2;
    S[x] = true;
    break;
    }

 for (y = 0; y < n; y++) //initializing diff_vect array
 {
    diff_vect[y] = lx[root] + ly[y] - weight[root][y];
    diff_x[y] = root;
 }
 //second part of augment() function
 while (true)
 {
    while (rd < wr) //Constructing a tree using Bfs cycle
    {
        x = q[rd++];
        for (y = 0; y < n; y++) //iterate through all edges in equality graph
            if (weight[x][y] == lx[x] + ly[y] && !T[y])
             {
                if (yx[y] == -1) break;
                 //augmenting path exists!
                T[y] = true; //else just add y to T,
                q[wr++] = yx[y]; //add vertex yx[y], which is matched
                //with y, to the queue
                add_to_tree(yx[y], x); //add edges (y,yx[y]) and (x,y) to the tree
             }
    if (y < n) break; //augmenting path found!
    }
    if (y < n) break; //augmenting path found!

    update(); //augmenting path not found, so improving the labeling
    wr = rd = 0;
    for (y = 0; y < n; y++)

 //in this cycle we add edges that were added to the equality graph as a
 //result of improving the labeling, we add edge (diff_x[y], y) to the tree if
 //and only if !T[y] && diff_vect[y] == 0, also with this edge we add another one
 //(y, yx[y]) or augment the matching, if y was exposed
    if (!T[y] && diff_vect[y] == 0)
    {
        if (yx[y] == -1) //exposed vertex in Y found - augmenting path exists!
        {
            x = diff_x[y];
            break;
        }

    else
    {
    T[y] = true; //else just add y to T,
        if (!S[yx[y]])
        {
            q[wr++] = yx[y]; //add vertex yx[y], which is matched with
            //y, to the queue
            add_to_tree(yx[y], diff_x[y]); //and add edges (x,y) and (y,
            //yx[y]) to the tree
        }
    }

    }

    if (y < n) break; //augmenting path found!
 }

 if (y < n) //we found augmenting path!
 {
    maximum_match++; //increment matching
    //inversing the edges along augmenting path in this cycle
    for (int cx = x, cy = y, ty; cx != -2; cx = prev[cx], cy = ty)
    {
        ty = xy[cx];
        yx[cy] = cx;
        xy[cx] = cy;
    }
    augment(); //recall function, go to step 1 of the algorithm
 }
    //end of augment() function
}



int hungarian()
{
 int ret = 0; //weight of the optimal matching
 maximum_match = 0; //number of vertices in current matching
 memset(xy, -1, sizeof(xy));
 memset(yx, -1, sizeof(yx));
 define_labels(); //step 0
 augment(); //steps 1-3
 for (int x = 0; x < n; x++) //forming answer there
 ret += weight[x][xy[x]];

 for(int i=0;i<n;i++)
    cout<<i<<' '<<xy[i]<<' '<<yx[xy[i]]<<endl;


 return ret;
}
int main()
{
    cout<<"Enter number of vertices:";
    cin>>n;
    cout<<endl;
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            infile>>weight[i][j];

    cout<<hungarian();
}
