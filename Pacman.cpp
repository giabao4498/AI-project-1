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
	short state; position pre; bool isNew;
};
struct element2
{
	int x,y,point;
};
const short foodPoint=10;
const pair<short,short> direction[4]={{-1,0},{0,1},{1,0},{0,-1}};
int N,M,gamePoint; vector<vector<element1> > matrix(1); position pacman; vector<position> food; deque<element2> q; deque<position> path;
void inp()
{
	freopen("map.txt","r",stdin);
	scanf("%d%d",&N,&M);
	int i,j;
	for(i=1;i<=N;++i)
	{
		matrix.push_back({{1,{0,0},false}});
		for(j=1;j<=M;++j)
		{
			matrix[i].push_back({0,{0,0},true});
			scanf("%hd",&matrix[i][j].state);
			if (matrix[i][j].state==2) food.push_back({i,j});
			else if (matrix[i][j].state==1 || matrix[i][j].state==3) matrix[i][j].isNew=false;
		}
		matrix[i].push_back({1,{0,0},false});
	}
	matrix.push_back({});
	for(i=M+1;i>=0;--i)
	{
		matrix[0].push_back({1,{0,0},false});
		matrix.back().push_back({1,{0,0},false});
	}
	scanf("%d%d",&pacman.first,&pacman.second);
	fclose(stdin);
	++pacman.first; ++pacman.second;
}
void level1_2()
{
	q.push_back({pacman.first,pacman.second,0});
	if (pacman==food[0]) gamePoint=10;
	else
	{
		gamePoint=0;
		matrix[pacman.first][pacman.second].isNew=false;
		short i;
		do
		{
			for(i=0;i<4;++i)
			if (matrix[q[0].x+direction[i].first][q[0].y+direction[i].second].isNew)
			{
				q.push_back({q[0].x+direction[i].first,q[0].y+direction[i].second,q[0].point-1});
				matrix[q.back().x][q.back().y].pre={q[0].x,q[0].y};
				if (matrix[q.back().x][q.back().y].state==2)
				{
					gamePoint=q.back().point+10;
					goto label1;
				}
				matrix[q.back().x][q.back().y].isNew=false;
			}
			q.pop_front();
		}
		while (!q.empty() && q[0].point>-9);
	}
	label1:
	while (matrix[food[0].first][food[0].second].pre!=position(0,0))
	{
		path.push_front(food[0]);
		food[0]=matrix[food[0].first][food[0].second].pre;
	}
	path.push_front(pacman);
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

	string title[6] = { "-------------------------PACMAN GAME-------------------------","x : PACMAN", "# : WALL", "o : FOOD", "! : MONSTER", ". : EMPTY" };
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
				switch (matrix[i][j].state) {
				case 1:
					SetConsoleTextAttribute(h, 10 | FOREGROUND_INTENSITY);
					cout << "#";
					break;
				case 2:
					if (i == path[k].first&&j == path[k].second) {
						SetConsoleTextAttribute(h, 14 | FOREGROUND_INTENSITY);
						cout << "x";
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
		Sleep(1000);
	}
}
int main()
{
	inp();
	level1_2();
	out();
	draw();
}
