#include <iostream>
#include <vector>
#include <graphics.h>
#include <math.h>
#include <algorithm>
#include <string.h>
#include <stdio.h>

#define pi 3.14
#define x0 getmaxx()/2
#define y0 getmaxy()/2

using namespace std;

class Reader {
   int size;
   vector<vector<pair<int, int>>> mAdjList;
public:
   vector<vector<pair<int, int>>> Read();
   int getSize() {return this->size;};
   void setSize(int n) {this->size = n;};
   vector<vector<pair<int, int>>> getAdjList() {return this->mAdjList;}
};


vector<vector<pair<int, int>>> Reader::Read() {
   int n;
   std::cout << "Enter number of vertices:\n";
   std::cin >> n;
   this->setSize(n);

   vector<vector<pair<int, int>>> gr(n);

   std::cout << "Enter graph's adjacency list:\n";
   for (int i = 0; i < n; ++i) {
       int cnt, to, len;
       std::cout << "Enter count of neighbours of the vertex " << i << ":\n";
       std::cin >> cnt;

       cout << "Enter " << cnt << " neighbours of the vertex " << i << " as pairs of two numbers to and len:\n";
       for (int j = 0; j < cnt; ++j) {
           std::cin >> to >> len;
           gr[i].push_back(make_pair(to, len));
       }
   }

   this->mAdjList = gr;
   return gr;
}

class Graph {
private:
   vector<vector<pair<int, int>>> mAdjList;
   int mVCount;

public:
   Graph(int n, vector<vector<pair<int, int>>> g) {
       mVCount = n;

       mAdjList = vector<vector<pair<int, int>>>(n, vector<pair<int, int>>());
       for (int i = 0; i < g.size(); ++i) {
           mAdjList[i] = vector<pair<int, int>>(g[i].size());
           for (int j = 0; j < g[i].size(); ++j) {
               mAdjList[i][j] = g[i][j];
           }
       }
   }

   vector<vector<pair<int, int>>> getmAdjList() {
       return this->mAdjList;
   };

   int getSize() { return mVCount; }

   vector<pair<int, int>> getNeighbors(int i);
   pair<int, int> get(int i, int j);
};

pair<int, int> Graph::get(int i, int j) {
   if (i < 0 || i >= mVCount) {
       return make_pair(-1, -1);
   } else if (j < 0 || j > mAdjList[i].size()) {
       return make_pair(-1, -1);
   }

   return mAdjList[i][j];
}

vector<pair<int, int>> Graph::getNeighbors(int i) {
   if (i < 0 || i >= mVCount) {
       return vector<pair<int, int>>();
   }
   return mAdjList[i];
}


class PathFinder {
private:
   const int INF = 1000000000;

   Graph *mGraph;

   int start;

   vector<int> dist;
   vector<int> parents;

   vector<bool> used;

   void calcRoutes();
public:
   PathFinder(Graph *g, int s) : mGraph(g), start(s) {
       dist = vector<int>(g->getSize(), INF);
       parents = vector<int>(g->getSize());
       used = vector<bool>(g->getSize());

       dist[start] = 0;

       calcRoutes();
   }

   vector<int> findRouteTo(int to);
};


void PathFinder::calcRoutes() {
   for (int i = 0; i < mGraph->getSize(); ++i) {
       int v = -1;
       for (int j = 0; j < mGraph->getSize(); ++j)
           if (!used[j] && (v == -1 || dist[j] < dist[v]))
               v = j;
       if (dist[v] == INF)
           break;
       used[v] = true;

       for (size_t j=0; j<mGraph->getNeighbors(v).size(); ++j) {
           int to = mGraph->get(v, j).first,
                   len = mGraph->get(v, j).second;
           if (dist[v] + len < dist[to]) {
               dist[to] = dist[v] + len;
               parents[to] = v;
           }
       }
   }
}

vector<int> PathFinder::findRouteTo(int to) {
   vector<int> path;
   for (int v=to; v!=start; v=parents[v])
       path.push_back (v);
   path.push_back (start);
   reverse (path.begin(), path.end());
   return path;
}



vector<pair<int,int>> lenth;
vector<char*> value;

class GetOut {
   Graph *gr;
   PathFinder *p;
   int ev;
   int R, r, ro;

public:
   GetOut (Graph *g,  PathFinder *pf, int ver, int r1, int r2, int r3) : gr(g), p(pf), ev(ver), R(r1), r(r2), ro(r3) {};

   int getCoor1 (int n) {
       return (this->R)*cos(n*2*pi/gr->getSize())+x0;
   }

   int getCoor2 (int n) {
       return (this->R)*sin(n*2*pi/gr->getSize())+y0;
   }

   void printGraph ();
   void connections (int a, int b, char* c);
   void WritePath();
};

void GetOut::connections (int a, int b, char* c)
{
   int r = this->r;
   int ro = this->ro;
   value.push_back(c);
   double dx = this->getCoor1(b) - this->getCoor1(a);
   double dy = this->getCoor2(b) - this->getCoor2(a);
   double d = dx/dy;
   int y1 = (this->getCoor2(a)*(d*d+1) - sqrt(pow(this->getCoor2(a),2)*(d*d+1)*(d*d+1) - (d*d+1)*(pow(this->getCoor2(a),2)*(d*d+1) - dx*dx - dy*dy - (ro+r)*(ro+r) + 2*sqrt(dx*dx + dy*dy)*(ro+r))))/(d*d+1);
   int y2 = (this->getCoor2(a)*(d*d+1) + sqrt(pow(this->getCoor2(a),2)*(d*d+1)*(d*d+1) - (d*d+1)*(pow(this->getCoor2(a),2)*(d*d+1) - dx*dx - dy*dy - (ro+r)*(ro+r) + 2*sqrt(dx*dx + dy*dy)*(ro+r))))/(d*d+1);
   int x1 = this->getCoor1(a) + y1*d - this->getCoor2(a)*d;
   int x2 = this->getCoor1(a) + y2*d - this->getCoor2(a)*d;

   if ((sqrt(pow(this->getCoor1(b) - x1,2) + pow(this->getCoor2(b) - y1,2))) > (sqrt(pow(this->getCoor1(b) - x2,2) + pow(this->getCoor2(b) - y2,2))))
   {
       pair<int,int> w = make_pair(x2 - 2,y2 - 2);
       lenth.push_back(w);
   }
   else {
       pair<int,int> w = make_pair(x1 - 2,y1 - 2);
       lenth.push_back(w);
   }

}

void smallCircle(int ro)
{
   for (int i = 0; i < value.size(); i++)
   {
       circle(get<0>(lenth[i]), get<1>(lenth[i]), ro);
       floodfill(get<0>(lenth[i]), get<1>(lenth[i]), 0);
   }
}


void GetOut::printGraph ()
{
   int gd = DETECT,gm;
   initgraph(&gd,&gm,NULL);
   setbkcolor(11);
   setcolor(0);

   int r = this->r;
   int ro = this->ro;

   char s[50] = "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16";
   char* pw = strtok(s, " ");
   int v = this->gr->getSize();

   for (int k = 0; k < v; k++)
   {
       circle (this->getCoor1(k),this->getCoor2(k), r);
       floodfill(this->getCoor1(k),this->getCoor2(k), 0);
   }

   vector<vector<pair<int, int>>> l = this->gr->getmAdjList();

   for (int i = 0; i < v; i++)
   {
       for (int j = 0; j < l[i].size(); j++)
       {
           if (get<1>(l[i][j]) > 0)
           {
               char* z = new char[1];
               sprintf (z, "%d", get<1>(l[i][j]));
               connections (i, get<0>(l[i][j]), z);
           }
       }
   }

   smallCircle(ro);
   setlinestyle(0,0,3);
   setcolor(0);
   for (int i = 0; i < v; i++)
   {
       for (int j = 0; j < l[i].size(); j++)
       {
           if (get<1>(l[i][j]) > 0)
           {
               line(this->getCoor1(i), this->getCoor2(i), this->getCoor1(get<0>(l[i][j])),
                    this->getCoor2(get<0>(l[i][j])));
           }
       }
   }

   setcolor(4);
   setlinestyle(1,0,2);
   vector<int> k = this->p->findRouteTo(this->ev);

   for (int i = 0; i < k.size() - 1; i++)
   {
       line(getCoor1(k[i]), getCoor2(k[i]), getCoor1(k[i+1]), getCoor2(k[i+1]));
   }

   setcolor(15);

   for (int k = 0; k < v; k++)
   {
       outtextxy(this->getCoor1(k) - 5, this->getCoor2(k) - 5, pw);
       pw = strtok(nullptr," ");
   }
   for (int i = 0; i < value.size(); i++)
       outtextxy(get<0>(lenth[i]), get<1>(lenth[i]), value[i]);

   getch();
   closegraph();
}



void GetOut:: WritePath() {
   auto route = this->p->findRouteTo(this->ev);
   cout << "Your path: " ;
   for(int i = 0; i < route.size(); i++)  {
       std::cout << route[i] << ' ';
   }
   std::cout << '\n';
}



int main()
{
   Reader a;
   vector<vector<pair<int, int>>> pr = a.Read();

   Graph* g = new Graph(a.getSize(), pr);

   int s;
   std::cout << "Enter starting vertex:\n";
   std::cin >> s;

   PathFinder* pf = new PathFinder(g, s);

   int to;
   std::cout << "Enter ending vertex:\n";
   std::cin >> to;

   GetOut p(g, pf, to, 180, 20, 10);
   p.WritePath();
   p.printGraph();

   return 0;
}


