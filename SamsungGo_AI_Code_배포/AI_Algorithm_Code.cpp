// Samsung Go Tournament Form C (g++-4.8.3)

#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include "Connect6Algo.h"

char info[] = { "TeamName:씨두박년아,Department:POSCAT" };

// my code
#include <queue>
using namespace std;

extern int op_x[2], op_y[2], op_cnt;

int INF = 1e9;

int dx[8] = { 1, 1, 0, -1, -1, -1,  0,  1 };
int dy[8] = { 0, 1, 1,  1,  0, -1, -1, -1 };

bool inb(int x, int y)
{
	return 0 <= x && x < 19 && 0 <= y && y < 19;
}

int s[6][6] =
{
	// 0,  1,  2,  3,   4,   5
	{ 1,  1,  1,  4,   9,  16 }, // 0
{ 1,  1,  2,  6,  12,  20 }, // 1
{ 1,  2,  3,  8,  15,  24 }, // 2
{ 4,  6,  8, 10,  18,  28 }, // 3
{ 9, 12, 15, 18, INF, INF }, // 4
{ 16, 20, 24, 28, INF, INF }  // 5
};

extern int board[20][20];

bool free(int x, int y)
{
	return board[x][y] == 0;
}

int score()
{
	int a = 0, b = 0;

	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
			if (board[i][j] == 1)
				for (int k = 0; k < 4; k++)
				{
					int len = 1;
					while (1)
					{
						int x = i + len * dx[k], y = j + len * dy[k];
						if (!inb(x, y)) break;
						if (board[x][y] != 1) break;
						len++;
					}
					if (len >= 6) return INF;

					if (len > a)
					{
						a = len;
						b = a;
					}
					else if (len > b)
					{
						b = len;
					}
				}

	int c = 0, d = 0;

	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
			if (board[i][j] == 2)
				for (int k = 0; k < 4; k++)
				{
					int len = 1;
					while (1)
					{
						int x = i + len * dx[k], y = j + len * dy[k];
						if (!inb(x, y)) break;
						if (board[x][y] != 2) break;
						len++;
					}
					if (len >= 6) return -INF;

					if (len > c)
					{
						c = len;
						d = a;
					}
					else if (len > d)
					{
						d = len;
					}
				}


	return s[b][a] - s[d][c];
}

bool surr(int x, int y)
{
	for (int k = 0; k < 8; k++)
		if (inb(x + dx[k], y + dy[k]))
			if (board[x + dx[k]][y + dy[k]] != 0) return true;
	return false;
}

int alphabeta(int px0, int py0, int px1, int py1, int depth, int alpha, int beta, bool playermax)
{
	int hscore = score();
	if (depth == 0 || hscore > INF - 30 || hscore < -INF + 30) return hscore;

	if (playermax)
	{
		int v = -INF;

		for (int x0 = px0 - 2; x0 <= px0 + 2; x0++)
			for (int y0 = py0 - 2; y0 <= py0 + 2; y0++)
				for (int x1 = px1 - 2; x1 <= px1 + 2; x1++)
					for (int y1 = py1 - 2; y1 <= py1 + 2; y1++)
					{
						if (!inb(x0, y0) || !inb(x1, y1)) continue;
						if (x0 == x1 && y0 == y1) continue;
						if (!free(x0, y0) || !free(x1, y1)) continue;

						board[x0][y0] = board[x1][y1] = 1;
						v = max(v, alphabeta(x0, y0, x1, y1, depth - 1, alpha, beta, false));
						board[x0][y0] = board[x1][y1] = 0;

						alpha = max(alpha, v);

						if (beta <= alpha) break;
					}

		return v;
	}
	else
	{
		int v = INF;

		for (int x0 = px0 - 2; x0 <= px0 + 2; x0++)
			for (int y0 = py0 - 2; y0 <= py0 + 2; y0++)
				for (int x1 = px1 - 2; x1 <= px1 + 2; x1++)
					for (int y1 = py1 - 2; y1 <= py1 + 2; y1++)
					{
						if (x0 == x1 && y0 == y1) continue;
						if (!isFree(x0, y0) || !isFree(x1, y1)) continue;

						board[x0][y0] = board[x1][y1] = 2;
						v = min(v, alphabeta(x0, y0, x1, y1, depth - 1, alpha, beta, true));
						board[x0][y0] = board[x1][y1] = 0;

						beta = min(beta, v);

						if (beta <= alpha) break;
					}

		return v;
	}
}

void myturn(int cnt) {

	int x[2], y[2];

	// 이 부분에서 알고리즘 프로그램(AI)을 작성하십시오. 기본 제공된 코드를 수정 또는 삭제하고 본인이 코드를 사용하시면 됩니다.

	if (cnt == 1) // first start
	{
		x[0] = 9; y[0] = 9;
		domymove(x, y, cnt);
		return;
	}

	int minscore = INF;

	for (int x0 = 0; x0 < 19; x0++)
		for (int y0 = 0; y0 < 19; y0++)
			for (int x1 = 0; x1 < 19; x1++)
				for (int y1 = 0; y1 < 19; y1++)
				{
					if (x0 == x1 && y0 == y1) continue;
					if (!free(x0, y0) || !free(x1, y1) || !surr(x0, y0) || !surr(x1, y1)) continue;

					board[x0][y0] = board[x1][y1] = 1;

					if (score() > INF - 30)
					{
						x[0] = x0; y[0] = y0; x[1] = x1; y[1] = y1;
						domymove(x, y, cnt);
						return;
					}

					int v = alphabeta(x0, y0, x1, y1, 2, -INF, INF, false);

					board[x0][y0] = board[x1][y1] = 0;

					if (minscore > v)
					{
						minscore = v;
						x[0] = x0; y[0] = y0; x[1] = x1; y[1] = y1;
					}
				}

	// 이 부분에서 자신이 놓을 돌을 출력하십시오.
	// 필수 함수 : domymove(x배열,y배열,배열크기)
	// 여기서 배열크기(cnt)는 myturn()의 파라미터 cnt를 그대로 넣어야합니다.
	domymove(x, y, cnt);
}