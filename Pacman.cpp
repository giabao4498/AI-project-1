#include<cstdio>
#include<vector>
#include<deque>
#include<iostream>
#include<Windows.h>
#include<stdlib.h>
#include<fstream>
#include<string>
#include<string.h>
#include<cstring>
using namespace std;
typedef pair<int,int> position;
struct element1
{
	short state; position pre;
};
struct element2
{
	int x,y,len;
};
typedef vector<vector<element1> > matrix;
const short foodPoint=10;
const pair<short,short> direction[4]={{-1,0},{0,1},{1,0},{0,-1}};
const int waitTime=1000;
int N,M,gamePoint; matrix original(1); position pacman; deque<position> food,path; deque<element2> q;
void inp()
{
	freopen("map.txt","r",stdin);
	scanf("%d%d",&N,&M);
	int i,j;
	for(i=1;i<=N;++i)
	{
		original.push_back({{1,{0,0}}});
		for(j=1;j<=M;++j)
		{
			original[i].push_back({0,{0,0}});
			scanf("%hd",&original[i][j].state);
		}
		original[i].push_back({1,{0,0}});
	}
	original.push_back({});
	for(i=M+1;i>=0;--i)
	{
		original[0].push_back({1,{0,0}});
		original.back().push_back({1,{0,0}});
	}
	scanf("%d%d",&pacman.first,&pacman.second);
	fclose(stdin);
	++pacman.first; ++pacman.second;
}
void level1_2()
{
	q.push_back({pacman.first,pacman.second,0});
	matrix a=original;
	int i,j;
	for(i=1;i<=N;++i)
	for(j=1;j<=M;++j)
	if (a[i][j].state==2)
	{
		food.push_back({i,j});
		goto label1;
	}
	label1:
	if (pacman==food[0]) gamePoint=foodPoint;
	else
	{
		gamePoint=0; a[pacman.first][pacman.second].state=1;
		do
		{
			for(i=0;i<4;++i)
			if (a[q[0].x+direction[i].first][q[0].y+direction[i].second].state==0 || a[q[0].x+direction[i].first][q[0].y+direction[i].second].state==2)
			{
				q.push_back({q[0].x+direction[i].first,q[0].y+direction[i].second,q[0].len+1});
				a[q.back().x][q.back().y].pre={q[0].x,q[0].y};
				if (a[q.back().x][q.back().y].state==2)
				{
					gamePoint=foodPoint-q.back().len;
					goto label2;
				}
				a[q.back().x][q.back().y].state=1;
			}
			q.pop_front();
		}
		while (!q.empty() && q[0].len<foodPoint-1);
	}
	label2:
	while (a[food[0].first][food[0].second].pre!=position(0,0))
	{
		path.push_front(food[0]);
		food[0]=a[food[0].first][food[0].second].pre;
	}
	path.push_front(pacman);
}
void dfs(const position& cur,matrix& a,int& comeback)
{
	gamePoint-=comeback+1; comeback=0;
	if (a[cur.first][cur.second].state==2) gamePoint+=foodPoint;
	a[cur.first][cur.second].state=1;
	path.insert(path.end(),food.begin(),food.end());
	path.push_back(cur);
	food.clear();
	for(short i=0;i<4;++i)
	if (a[cur.first+direction[i].first][cur.second+direction[i].second].state==0 || a[cur.first+direction[i].first][cur.second+direction[i].second].state==2)
	{
		dfs({cur.first+direction[i].first,cur.second+direction[i].second},a,comeback);
		food.push_back(cur);
	}
	++comeback;
}
void level3()
{
	matrix a=original; int comeback=-1;
	gamePoint=0;
	dfs(pacman,a,comeback);
}
void out()
{
	ofstream f("result.txt");
	f<<"Path finding for Pacman:\n";
	int i;
	for(i=0;i<path.size();++i) f<<"("<<path[i].first-1<<", "<<path[i].second-1<<")\n";
	f<<"Path length: "<<path.size()-1<<"\nGame point: "<<gamePoint;
	f.close();
}
void draw()
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD maxSize = GetLargestConsoleWindowSize(h);
	COORD pos[6];
	int color[6] = { 15,14,10,11,12,15 };

	string title[6] = { "----------------------------------------------------------------------PACMAN GAME----------------------------------------------------------------------", 
		"x : PACMAN", 
		"# : WALL", 
		"o : FOOD", 
		"! : MONSTER", 
		". : EMPTY" };
	for (int i = 0; i < 6; i++) {
		pos[i].X = (maxSize.X - title[i].length()) / 2;
		pos[i].Y = i;
	}

	int pathSize = path.size();

	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

	for (int k = 0; k < pathSize; k++) {
		system("CLS");
		for (int i = 0; i < 6; i++) {
			SetConsoleCursorPosition(h, pos[i]);
			SetConsoleTextAttribute(h, color[i] | FOREGROUND_INTENSITY);
			cout << title[i] << endl;
		}

		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= M; j++)
				switch (original[i][j].state) {
				case 1:
					SetConsoleTextAttribute(h, 10 | FOREGROUND_INTENSITY);
					cout << "#";
					break;
				case 2:
					if (i == path[k].first&&j == path[k].second) {
						SetConsoleTextAttribute(h, 14 | FOREGROUND_INTENSITY);
						cout << "x";
						original[i][j].state=0;
					}
					else {
						SetConsoleTextAttribute(h, 11 | FOREGROUND_INTENSITY);
						cout << "o";
					}
					break;
				case 3:
					SetConsoleTextAttribute(h, 12 | FOREGROUND_INTENSITY);
					cout << "!";
					break;
				default:
					if (i == path[k].first&&j == path[k].second) {
						SetConsoleTextAttribute(h, 14 | FOREGROUND_INTENSITY);
						cout << "x";
					}
					else {
						SetConsoleTextAttribute(h, 15 | FOREGROUND_INTENSITY);
						cout << ".";
					}
				}
			cout << endl;
		}
		SetConsoleTextAttribute(h, 13 | FOREGROUND_INTENSITY);
		cout << "Path: ";
		for (int i = 0; i <= k; i++) {
			cout << "(" << path[i].first - 1 << ", " << path[i].second - 1 << ") ";
		}
		Sleep(waitTime);
	}
}
int main()
{
	inp();
	level3();
	out();
	draw();
}
