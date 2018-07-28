// Samsung Go Tournament Form C (g++-4.8.3)

#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include "Connect6Algo.h"

char info[] = { "TeamName:씨두박년,Department:POSCAT" };

// my code
#include <utility>
#include <queue>
#include <vector>
#include <functional>
using namespace std;

typedef pair<int, int> p;

int INF = 1e9;

int dx[8] = { 1, 1, 0, -1, -1, -1,  0,  1};
int dy[8] = { 0, 1, 1,  1,  0, -1, -1, -1};

bool inb(int y, int x)
{
    return 0 <= y && y < 19 && 0 <= x && x < 19;
}

int s[6][6] =
{
    // 0,  1,  2,  3,   4,   5
    {  0,  0,  0,  0,   0,   0 }, // 0
    {  0,  0,  0,  0,   0,   0 }, // 1
    {  1,  2,  3,  0,   0,   0 }, // 2
    {  3,  4,  5,  6,   0,   0 }, // 3
    {  6,  7,  8,  9, INF,   0 }, // 4
    { 10, 11, 12, 13, INF, INF }  // 5
};

int score(int board[][19])
{
    priority_queue<int> pq1;
    for (int i = 0; i < 19; i++)
    for (int j = 0; j < 19; j++)
    if (board[i][j] == 1 || board[i][j] == 3)
    for (int k = 0; k < 4; k++)
    {
        int len = 1;
        while (1)
        {
            int y = i + len*dy[k], x = j + len*dx[k];
            if (!inb(y, x)) break;
            if (board[y][x] != 1 || board[y][x] != 3) break;
            len++;
        }
        if (len >= 6) return INF;
        pq1.push(len);
        if (pq1.size() > 2) pq1.pop();
    }
    int a = pq1.top(); pq1.pop();
    int b = pq1.top(); pq1.pop();

    int ret = s[b][a];

    priority_queue<int> pq2;
    for (int i = 0; i < 19; i++)
    for (int j = 0; j < 19; j++)
    if (board[i][j] == 2 || board[i][j] == 3)
    for (int k = 0; k < 4; k++)
    {
        int len = 1;
        while (1)
        {
            int y = i + len*dy[k], x = j + len*dx[k];
            if (!inb(y, x)) break;
            if (board[y][x] != 2 || board[y][x] != 3) break;
            len++;
        }
        if (len >= 6) return -INF;
        pq2.push(len);
        if (pq2.size() > 2) pq2.pop();
    }
    int c = pq2.top(); pq2.pop();
    int d = pq2.top(); pq2.pop();

    return ret - s[d][c];
}

bool surr(int board[][19], int y, int x)
{
    for (int k = 0; k < 8; k++) if (inb(y + dy[k], x + dx[x]) && board[y + dy[k]][x + dx[k]]) return true;
    return false;
}

int alphabeta(int board[][19], int depth, int alpha, int beta, bool playermax)
{
    if (depth == 0) return score(board);

    if (playermax)
    {
        for (int y1 = 0; y1 < 18; y1++)
        for (int x1 = 0; x1 < 19; x1++)
        for (int y2 = y1 + 1; y2 < 19; y2++)
        for (int x2 = 0; x2 < 19; x2++)
        {
            if (!isFree(y1, x1) || !surr(board, y1, x1) || !isFree(y2, x2) || !surr(board, y2, x2)) continue;

            board[y1][x1] = board[y2][x2] = 1;
            int v = alphabeta(board, depth - 1, alpha, beta, false);

            board[y1][x1] = board[y2][x2] = 0;

            if (alpha < v) alpha = v;

            if (beta <= alpha) break;
        }

        return alpha;
    }
    else
    {
        for (int y1 = 0; y1 < 18; y1++)
        for (int x1 = 0; x1 < 19; x1++)
        for (int y2 = y1 + 1; y2 < 19; y2++)
        for (int x2 = 0; x2 < 19; x2++)
        {
            if (!isFree(y1, x1) || !surr(board, y1, x1) || !isFree(y2, x2) || !surr(board, y2, x2)) continue;

            board[y1][x1] = board[y2][x2] = 2;
            int v = alphabeta(board, depth - 1, alpha, beta, true);

            board[y1][x1] = board[y2][x2] = 0;

            if (beta > v) beta = v;

            if (beta <= alpha) break;
        }

        return beta;
    }
}

void myturn(int cnt) {

    int x[2], y[2];

    // 이 부분에서 알고리즘 프로그램(AI)을 작성하십시오. 기본 제공된 코드를 수정 또는 삭제하고 본인이 코드를 사용하시면 됩니다.

    if (cnt == 1)
    {
        x[0] = 9; y[0] = 9;
        domymove(x, y, cnt);
        return;
    }

    int myboard[19][19];
    for (int i = 0; i < 19; i++)
    for (int j = 0; j < 19; j++)
    myboard[i][j] = showBoard(i, j);

    int maxscore = -INF;

    for (int y1 = 0; y1 < 18; y1++)
    for (int x1 = 0; x1 < 19; x1++)
    for (int y2 = y1 + 1; y2 < 19; y2++)
    for (int x2 = 0; x2 < 19; x2++)
    {
        if (!isFree(y1, x1) || !surr(myboard, y1, x1) || !isFree(y2, x2) || !surr(myboard, y2, x2)) continue;

        myboard[y1][x1] = myboard[y2][x2] = 1;
        int v = alphabeta(myboard, 3, -INF, INF, true);

        myboard[y1][x1] = myboard[y2][x2] = 0;

        if (maxscore < v)
        {
            maxscore = v;
            x[0] = x1; y[0] = y1; x[1] = x2; y[1] = y2;
        }
    }


    // 이 부분에서 자신이 놓을 돌을 출력하십시오.
    // 필수 함수 : domymove(x배열,y배열,배열크기)
    // 여기서 배열크기(cnt)는 myturn()의 파라미터 cnt를 그대로 넣어야합니다.
    domymove(x, y, cnt);
}
