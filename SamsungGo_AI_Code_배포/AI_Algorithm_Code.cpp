// Samsung Go Tournament Form C (g++-4.8.3)

#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include "Connect6Algo.h"
#include <fstream>
//
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <queue>

using namespace std;

struct Point
{
	int x, y;

	inline Point& operator=(const Point& p)
	{
		x = p.x;
		y = p.y;
		return *this;
	}

	inline bool operator==(const Point& p)
	{
		return x == p.x && y == p.y;
	}

	inline bool operator!=(Point p)
	{
		return x != p.x || y != p.y;
	}

	bool operator<(const Point p)
	{
		return x<p.x;
	}
};

struct Dscore
{
	int x, y;
	int score, count;
	Dscore(int x_, int y_, int s, int c)
	{
		x = x_;
		y = y_;
		score = s;
		count = c;
	}

	
};

bool operator<(const Dscore &d1, const Dscore &d2)
{
	return d1.score == d2.score ? d1.count > d2.count : d1.score < d2.score;
}

bool operator<(const Point p1, const Point p2) {
	if (p1.x == p2.x) return p1.y < p2.y;
	return p1.x < p2.x;
}

typedef pair<Point, Point> interval;

char info[] = { "TeamName:오목하러왔어요, Department:AI부서[C]" };

int dx[8] = { 1, 1, 0, -1, -1, -1,  0,  1 };
int dy[8] = { 0, 1, 1,  1,  0, -1, -1, -1 };

extern int board[20][20];
extern int op_x[2], op_y[2], op_cnt;

bool inb(int x, int y)
{
	return 0 <= x&&x<19 && 0 <= y&&y<19;
}

bool free(int x, int y)
{
	return board[x][y] == 0;
}

int checkboard(Point p)
{
	return board[p.x][p.y];
}




bool find2(int x, int y)
{
	if (board[x][y] != 0)
		return false;
	for (int dir = 0; dir < 4; dir++)
	{
		// case 1 'X'OOX
		if (inb(x + 3 * dx[dir], y + 3 * dy[dir]))
			if (board[x + 1 * dx[dir]][y + 1 * dy[dir]] == 2 && board[x + 2 * dx[dir]][y + 2 * dy[dir]] == 2 && board[x + 3 * dx[dir]][y + 3 * dy[dir]] == 0)
				return true;

		// case 2 'X'OXOX
		if (inb(x + 4 * dx[dir], y + 4 * dy[dir]))
			if (board[x + 1 * dx[dir]][y + 1 * dy[dir]] == 2 && board[x + 2 * dx[dir]][y + 2 * dy[dir]] == 0 && board[x + 3 * dx[dir]][y + 3 * dy[dir]] == 2 && board[x + 4 * dx[dir]][y + 4 * dy[dir]] == 0)
				return true;

		// case 3 XO'X'OX
		if (inb(x - 2 * dx[dir], y - 2 * dy[dir]), inb(x + 2 * dx[dir], y + 2 * dy[dir]))
			if (board[x - 2 * dx[dir]][y - 2 * dy[dir]] == 0 && board[x - 1 * dx[dir]][y - 1 * dy[dir]] == 2 && board[x + 1 * dx[dir]][y + 1 * dy[dir]] == 2 && board[x + 2 * dx[dir]][y + 2 * dy[dir]] == 0)
				return true;
	}

	return false;
}

bool find3(int x, int y)
{
	if (board[x][y] != 0)
		return false;
	for (int dir = 0; dir < 4; dir++)
	{
		// case 1 'X'OOOX
		if (inb(x + 4 * dx[dir], y + 4 * dy[dir]))
			if (board[x + 1 * dx[dir]][y + 1 * dy[dir]] == 2 && board[x + 2 * dx[dir]][y + 2 * dy[dir]] == 2 && board[x + 3 * dx[dir]][y + 3 * dy[dir]] == 2 && board[x + 4 * dx[dir]][y + 4 * dy[dir]] == 0)
				return true;
	}

	return false;
}


Dscore scoring(int x, int y)
{
	int my = 0, op = 0;

	for (int dir = 0; dir < 8; dir++)
	{
		if (inb(x + dx[dir], y + dy[dir]))
		{
			int b = board[x + dx[dir]][y + dy[dir]];
			if (b == 1) my++;
			else if(b == 2) op++;
		}
	}

	return Dscore(x, y, my - op, op);
}

void defense(int &cnt, int turnx[], int turny[])
{
	ofstream f("position.txt", ios::app);
	priority_queue<Dscore> pq1, pq2;

	for (int x = 0; x < 19; x++)
		for (int y = 0; y < 19; y++)
		{
			if (find3(x, y))
			{
				pq1.push(scoring(x, y));
			}
			else if (find2(x, y))
			{
				pq2.push(scoring(x, y));
			}
		}

	while (cnt && !pq1.empty())
	{
		Dscore d = pq1.top();
		pq1.pop();
		turnx[--cnt] = d.x;
		turny[cnt] = d.y;

		f << 4 << ' ' << turnx[cnt] << ' ' << turny[cnt] << ' ' << board[turnx[cnt]][turny[cnt]] << '\n';
		board[turnx[cnt]][turny[cnt]] = 1;
	}

	while (cnt && !pq2.empty())
	{
		Dscore d = pq2.top();
		pq2.pop();
		turnx[--cnt] = d.x;
		turny[cnt] = d.y;

		f << 4 << ' ' << turnx[cnt] << ' ' << turny[cnt] << ' ' << board[turnx[cnt]][turny[cnt]] << '\n';
		board[turnx[cnt]][turny[cnt]] = 1;
	}
}



vector<interval> checkinterval(int x, int y, bool player)
{
	vector<interval> v;

	Point a, b;

	bool beforechecked; int n[4];

	if (player)
		for (int dir = 0; dir < 4; dir++)
		{
			beforechecked = false;
			for (int s = 0; s < 6; s++)
				if (inb(x - s * dx[dir], y - s * dy[dir]) && inb(x + (5 - s) * dx[dir], y + (5 - s) * dy[dir]))
				{
					if (beforechecked)
					{
						n[board[x + (6 - s) * dx[dir]][y + (6 - s) * dy[dir]]]--;
						n[board[x - s * dx[dir]][y - s * dy[dir]]]++;
					}
					else
					{
						n[0] = n[1] = n[2] = n[3] = 0;
						for (int k = -s; k <= 5 - s; k++)
						{
							int b = board[x + k * dx[dir]][y + k * dy[dir]];
							n[b]++;
						}
						beforechecked = true;
					}
					if (!n[2] && n[1]+n[3] >= 4)
					{
						a.x = x - s * dx[dir]; a.y = y - s * dy[dir]; b.x = x + (5 - s) * dx[dir]; b.y = y + (5 - s) * dy[dir];
						v.push_back(interval(a, b));
					}
				}
		}
	else
		for (int dir = 0; dir < 4; dir++)
		{
			beforechecked = false;
			for (int s = 0; s < 6; s++)
				if (inb(x - s * dx[dir], y - s * dy[dir]) && inb(x + (5 - s) * dx[dir], y + (5 - s) * dy[dir]))
				{
					if (beforechecked)
					{
						n[board[x + (6 - s) * dx[dir]][y + (6 - s) * dy[dir]]]--;
						n[board[x - s * dx[dir]][y - s * dy[dir]]]++;
					}
					else
					{
						n[0] = n[1] = n[2] = n[3] = 0;
						for (int k = -s; k <= 5 - s; k++)
						{
							int b = board[x + k * dx[dir]][y + k * dy[dir]];
							n[b]++;
						}
					}
					if (!n[1] && n[2]+n[3] >= 4)
					{
						a.x = x - s * dx[dir]; a.y = y - s * dy[dir]; b.x = x + (5 - s) * dx[dir]; b.y = y + (5 - s) * dy[dir];
						v.push_back(interval(a, b));
					}
				}
		}
	return v;
}

void checkWin(int& cnt, int x, int y, int turnx[], int turny[]) {
	if (!cnt) return;
	ofstream f("position.txt", ios::app);
	vector<interval> result = checkinterval(x, y, true);

	if (result.size() != 0) {
		interval I = result[0];

		int dirx = 0, diry = 0;
		if (I.first.x != I.second.x) dirx = I.second.x - I.first.x > 0 ? 1 : -1;
		if (I.first.y != I.second.y) diry = I.second.y - I.first.y > 0 ? 1 : -1;

		Point end = { I.second.x + dirx, I.second.y + diry };
		for (Point here = I.first; here != end; here.x += dirx, here.y += diry) {
			if (checkboard(here) == 0) {
				turnx[--cnt] = here.x;
				turny[cnt] = here.y;

				f << 1 << ' ' << turnx[cnt] << ' ' << turny[cnt] << '\n';

				board[turnx[cnt]][turny[cnt]] = 1;
				
			}
		}
	}
}

void checkLose(int& cnt, int x, int y, int turnx[], int turny[]) {
	ofstream f("position.txt", ios::app);
	while (cnt) {
		vector<interval> result = checkinterval(x, y, false);
		if (result.size() == 0) break;

		map<Point, int> m;
		for (auto I : result) {
			int dirx = 0, diry = 0;
			if (I.first.x != I.second.x) dirx = I.second.x - I.first.x > 0 ? 1 : -1;
			if (I.first.y != I.second.y) diry = I.second.y - I.first.y > 0 ? 1 : -1;

			f << I.first.x << ' ' << I.first.y << ' ' << I.second.x << ' ' << I.second.y << '\n';
			Point end = { I.second.x + dirx, I.second.y + diry };
			for (Point here = I.first; here != end; here.x += dirx, here.y += diry) {
				if (checkboard(here) == 0) {
					m[here]++;
				}
			}
			f << '\n';
		}

		int mx = 0;
		Point dangerous;
		for (auto it : m) {
			if (mx < it.second) {
				mx = it.second;
				dangerous = it.first;
			}
		}

		turnx[--cnt] = dangerous.x;
		turny[cnt] = dangerous.y;
		f << 2 << ' ' << turnx[cnt] << ' ' << turny[cnt] << '\n';
		f << board[turnx[cnt]][turny[cnt]] << '\n';
		board[turnx[cnt]][turny[cnt]] = 1;
		f << board[turnx[cnt]][turny[cnt]] << "\n\n";
		m.clear();
	}
}


void myturn(int cnt) {
	ofstream f("position.txt", ios::app);

	int x[2], y[2];

	// 이 부분에서 알고리즘 프로그램(AI)을 작성하십시오. 기본 제공된 코드를 수정 또는 삭제하고 본인이 코드를 사용하시면 됩니다.
	// 현재 Sample code의 AI는 Random으로 돌을 놓는 Algorithm이 작성되어 있습니다.

	if (cnt == 1)
	{
		bool check = false;
		for (int i = 7; i <= 10; i++)
		{
			for (int j = 7; j <= 10; j++)
			if (!board[i][j])
			{
				x[0] = i; y[0] = j;
				check = true;
			}
			if (check) break;
		}
		domymove(x, y, cnt);
		return;
	}

	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
			checkWin(cnt, i, j, x, y);

	checkLose(cnt, op_x[0], op_y[0], x, y);
	checkLose(cnt, op_x[1], op_y[1], x, y);
	if(cnt) defense(cnt, x, y);

	while (cnt) {
		int tx = rand() % 19;
		int ty = rand() % 19;

		if (board[tx][ty] == 0) {
			x[--cnt] = tx;
			y[cnt] = ty;
			board[tx][ty] = 1;
			f << 3 << ' ' << x[cnt] << ' ' << y[cnt] << '\n';
		}
	}

	f << '\n';

	// 이 부분에서 자신이 놓을 돌을 출력하십시오.
	// 필수 함수 : domymove(x배열,y배열,배열크기)
	// 여기서 배열크기(cnt)는 myturn()의 파라미터 cnt를 그대로 넣어야합니다.
	domymove(x, y, cnt);
}