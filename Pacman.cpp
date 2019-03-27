#include<vector>
#include<deque>
#include<iostream>
#include<Windows.h>
#include<fstream>
#include<string>
#include<set>
using namespace std;
typedef pair<int,int> position;
struct element1
{
	short state; position pre; short anotherState;
};
struct element2
{
	int x,y,len;
};
typedef vector<vector<element1> > matrix;
typedef pair<short,short> couple;
const short foodPoint=10;
const couple direction[4]={{-1,0},{0,1},{1,0},{0,-1}};
const int waitTime=1000;
vector<bool> pre;
int N,M,gamePoint; matrix original(1); position pacman; deque<position> path; short level; vector<position> monster; set<position> food; vector<vector<position> > way;
void inp()
{
	ifstream f("map.txt");
	f>>N>>M;
	int i,j;
	for(i=1;i<=N;++i)
	{
		original.push_back({{1,{0,0},-1}});
		for(j=1;j<=M;++j)
		{
			original[i].push_back({0,{0,0},-1});
			f>>original[i][j].state;
			switch (original[i][j].state)
			{
				case 2: food.insert({i,j}); break;
				case 3: monster.push_back({i,j}); original[i][j].anotherState=0;
			}
		}
		original[i].push_back({1,{0,0},-1});
	}
	original.push_back({});
	for(i=M+1;i>=0;--i)
	{
		original[0].push_back({1,{0,0},-1});
		original.back().push_back({1,{0,0},-1});
	}
	f>>pacman.first>>pacman.second;
	f.close();
	++pacman.first; ++pacman.second;
	do
	{
		cout<<"Choose level (1-4): ";
		cin>>level;
	}
	while (level<1 || level>4);
}
void level1_2()
{
	deque<element2> q;
	q.push_back({pacman.first,pacman.second,0});
	matrix a=original; position tmp=*food.begin();
	if (pacman==tmp) gamePoint=foodPoint;
	else
	{
		gamePoint=0; a[pacman.first][pacman.second].state=1;
		short i;
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
					goto label;
				}
				a[q.back().x][q.back().y].state=1;
			}
			q.pop_front();
		}
		while (!q.empty() && q[0].len<foodPoint-1);
	}
	label:
	while (a[tmp.first][tmp.second].pre!=position(0,0))
	{
		path.push_front(tmp);
		tmp=a[tmp.first][tmp.second].pre;
	}
	path.push_front(pacman);
}
bool exist(const position& cur,const matrix& a,const short& k)
{
	int i,j;
	switch (k)
	{
		case 0:
			for(i=1;i<cur.first;++i)
			for(j=1;j<=M;++j)
			if (a[i][j].state==2) return true;
			return false;
		case 1:
			for(i=1;i<=N;++i)
			for(j=cur.second+1;j<=M;++j)
			if (a[i][j].state==2) return true;
			return false;
		case 2:
			for(i=cur.first+1;i<=N;++i)
			for(j=1;j<=M;++j)
			if (a[i][j].state==2) return true;
			return false;
		case 3:
			for(i=1;i<=N;++i)
			for(j=1;j<cur.second;++j)
			if (a[i][j].state==2) return true;
			return false;
	}
}
bool dfs(const position& cur,matrix& a,int& comeback)
{
	gamePoint-=comeback+1; comeback=0;
	if (a[cur.first][cur.second].state==2)
	{
		gamePoint+=foodPoint;
		food.erase(cur);
	}
	path.insert(path.end(),monster.begin(),monster.end());
	path.push_back(cur);
	if (food.empty()) return true;
	a[cur.first][cur.second].state=1;
	monster.clear();
	vector<couple> direction_; short i;
	for(i=0;i<4;++i)
	if (exist(cur,a,i)) direction_.push_back(direction[i]);
	else monster.push_back(direction[i]);
	direction_.insert(direction_.end(),monster.begin(),monster.end());
	monster.clear();
	for(i=0;i<4;++i)
	if (a[cur.first+direction_[i].first][cur.second+direction_[i].second].state==0 || a[cur.first+direction_[i].first][cur.second+direction_[i].second].state==2)
	{
		if (dfs({cur.first+direction_[i].first,cur.second+direction_[i].second},a,comeback)) return true;
		monster.push_back(cur);
	}
	++comeback;
	return false;
}
void level3()
{
	matrix a=original; int comeback=-1;
	gamePoint=0;
	monster.clear();
	dfs(pacman,a,comeback);
}
short bfsPacman(const matrix& a)
{
	deque<element2> q; short i; matrix b=a;
	b[pacman.first][pacman.second].state=1;
	for(i=0;i<4;++i)
	switch (b[pacman.first+direction[i].first][pacman.second+direction[i].second].state)
	{
		case 0: q.push_back({pacman.first+direction[i].first,pacman.second+direction[i].second,i}); break;
		case 2: return i;
	}
	while (!q.empty())
	{
		for(i=0;i<4;++i)
		switch (b[q[0].x+direction[i].first][q[0].y+direction[i].second].state)
		{
			case 0: q.push_back({q[0].x+direction[i].first,q[0].y+direction[i].second,q[0].len}); b[q.back().x][q.back().y].state=1; break;
			case 2: return q[0].len;
		}
		q.pop_front();
	}
	return -1;
}
short bfsMonster(const position& m,const matrix& a)
{
	deque<element2> q; short i; matrix b=a;
	b[m.first][m.second].state=1;
	for(i=0;i<4;++i)
	if (b[m.first+direction[i].first][m.second+direction[i].second].state!=1)
	{
		q.push_back({m.first+direction[i].first,m.second+direction[i].second,i});
		if (position(q.back().x,q.back().y)==pacman) return i;
	}
	while (!q.empty())
	{
		for(i=0;i<4;++i)
		if (b[q[0].x+direction[i].first][q[0].y+direction[i].second].state!=1)
		{
			q.push_back({q[0].x+direction[i].first,q[0].y+direction[i].second,q[0].len});
			if (position(q.back().x,q.back().y)==pacman) return q[0].len;
			b[q.back().x][q.back().y].state=1;
		}
		q.pop_front();
	}
	return -1;
}
bool moveMonster(matrix& a)
{
	short j;
	for(int i=0;i<monster.size();++i)
	if ((j=bfsMonster(monster[i],a))>=0)
	{
		monster[i].first+=direction[j].first; monster[i].second+=direction[j].second;
		if (monster[i]==pacman) return false;
		if (a[monster[i].first][monster[i].second].state!=3)
		{
			a[monster[i].first][monster[i].second].anotherState=a[monster[i].first][monster[i].second].state;
			a[monster[i].first][monster[i].second].state=3;
		}
		a[monster[i].first-direction[j].first][monster[i].second-direction[j].second].state=a[monster[i].first-direction[j].first][monster[i].second-direction[j].second].anotherState;
		way[i].push_back(monster[i]);
	}
	return true;
}
void level4()
{
	gamePoint=0;
	path.push_back(pacman);
	int i; matrix a=original;
	for(i=0;i<monster.size();++i) way.push_back({monster[i]});
	while (!food.empty() && (i=bfsPacman(a))>=0)
	{
		a[pacman.first][pacman.second].state=4;
		pacman.first+=direction[i].first; pacman.second+=direction[i].second;
		if (!moveMonster(a)) break;
		--gamePoint;
		if (a[pacman.first][pacman.second].state==2)
		{
			gamePoint+=foodPoint; a[pacman.first][pacman.second].state=0;
			food.erase(pacman);
		}
		path.push_back(pacman);
	}
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
	if (level == 4)
		for (int i = 1; i <= N; i++)
			for (int j = 1; j <= M; j++)
				if (original[i][j].state == 3)
					original[i][j].state = 0;

	int pathSize = path.size();
	int waySize = way.size();
	for (set<position>::iterator i = food.begin(); i != food.end(); i++)
		original[i->first][i->second].state = 2;

	for (int i = 0; i < waySize; i++)
		pre.push_back(false);

	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

	for (int k = 0; k < pathSize; k++) {
		system("CLS");
		original[path[k].first][path[k].second].state = 4;
		if (level == 4)
			for (int t = 0; t < waySize; t++) {
				if (way[t].size() > k) {
					if (original[way[t][k].first][way[t][k].second].state == 2)
						pre[t] = true;
					original[way[t][k].first][way[t][k].second].state = 3;
				}
				else original[way[t][way[t].size() - 1].first][way[t][way[t].size() - 1].second].state = 3;
			}
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
					SetConsoleTextAttribute(h, 11 | FOREGROUND_INTENSITY);
					cout << "o";
					break;
				case 3:
					SetConsoleTextAttribute(h, 12 | FOREGROUND_INTENSITY);
					cout << "!";
					break;
				case 4:
					SetConsoleTextAttribute(h, 14 | FOREGROUND_INTENSITY);
					cout << "x";
					break;
				default:
					SetConsoleTextAttribute(h, 15 | FOREGROUND_INTENSITY);
					cout << ".";
				}
			cout << endl;
		}
		SetConsoleTextAttribute(h, 13 | FOREGROUND_INTENSITY);

		original[path[k].first][path[k].second].state = 0;
		if (level == 4)
			for (int t = 0; t < waySize; t++) {
				if (way[t].size() > k) {
					if (pre[t]) {
						original[way[t][k].first][way[t][k].second].state = 2;
						pre[t] = false;
					}
					else
						original[way[t][k].first][way[t][k].second].state = 0;
				}
		}
		Sleep(waitTime);
	}
}
int main()
{
	inp();
	switch (level)
	{
		case 3: level3(); break;
		case 4: level4(); break;
		default:
			level1_2();
	}
	out();
	draw();
}
