#include <bits/stdc++.h>

using namespace std;

#define BOARD_SIZE 15
#define EMPTY 0
#define ME 1
#define OTHER 2

#define START "START"
#define PLACE "PLACE"
#define DONE "DONE"
#define TURN "TURN"
#define BEGIN "BEGIN"
#define END "END"

struct Position
{
	int x;
	int y;
};

ostream &operator<<(ostream &out, const Position &pos)
{
	return out << pos.x << " " << pos.y;
}

class AI
{
  private:
	double TIMELIMIT;
#define INF 10000000
#define DDEBUG
#define HUMAN
#define LIMIT 7
	short int a[15][15];
	clock_t St, SSt;
	inline bool OT()
	{
#ifndef DEBUG
		if (clock() - St >= 0.95 * CLOCKS_PER_SEC * TIMELIMIT)
			return 1;
#endif
		return 0;
	}
	int MSF; //1:black -1:white
	struct poi
	{
		int x, y;
		int H;
	};
	inline bool static cmp(const poi &a, const poi &b) { return a.H > b.H; }
	const int ONE = 10, TWO = 100, THREE = 1000, FOUR = 100000, FIVE = 10000000, _ONE = 1, _TWO = 10, _THREE = 100, _FOUR = 1000;
	int st[8][2] = {{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};
	inline bool IN(int x, int y) { return 0 <= x && x < 15 && 0 <= y && y < 15; }
	int pt[5] = {5, 10, 9, 7, 5};
	int pt2[6] = {1000, 10, 10, 10, 2, 1};
	int dp[3][3][3][3][3][3];
#define WIN 1	 //4000
#define LOSE 2	//-4000
#define FLEX4 3   //2000
#define flex4 4   //-2000
#define BLOCK4 5  //1000
#define block4 6  //-1000
#define FLEX3 7   //1000 ?
#define flex3 8   //-1000
#define BLOCK3 9  //400 ?
#define block3 10 //-600 ?
#define FLEX2 11  //400 ?
#define flex2 12  //-600 ?
#define BLOCK2 13 //100 ?
#define block2 14 //-150 ?
#define FLEX1 15  //100 ?
#define flex1 16  //-150 ?
	size_t boardSize;
	const vector<vector<int>> &board;
	int dep;
	int turnID;
	vector<poi> V, G;
	void initdp() //对1的评分
	{
		memset(dp, 0, sizeof(dp));
		dp[1][1][1][1][1][1] = WIN;
		dp[1][1][1][1][1][0] = WIN;
		dp[0][1][1][1][1][1] = WIN;
		dp[1][1][1][1][1][2] = WIN;
		dp[2][1][1][1][1][1] = WIN;

		//白五连 AI负
		dp[2][2][2][2][2][2] = LOSE;
		dp[2][2][2][2][2][0] = LOSE;
		dp[0][2][2][2][2][2] = LOSE;
		dp[2][2][2][2][2][1] = LOSE;
		dp[1][2][2][2][2][2] = LOSE;

		//黑活四
		dp[0][1][1][1][1][0] = FLEX4;

		//白活四
		dp[0][2][2][2][2][0] = flex4;

		//黑活三
		dp[0][1][1][1][0][0] = FLEX3;
		dp[0][1][1][0][1][0] = FLEX3;
		dp[0][1][0][1][1][0] = FLEX3;
		dp[0][0][1][1][1][0] = FLEX3;

		//白活三
		dp[0][2][2][2][0][0] = flex3;
		dp[0][2][2][0][2][0] = flex3;
		dp[0][2][0][2][2][0] = flex3;
		dp[0][0][2][2][2][0] = flex3;

		//黑活二
		dp[0][1][1][0][0][0] = FLEX2;
		dp[0][1][0][1][0][0] = FLEX2;
		dp[0][1][0][0][1][0] = FLEX2;
		dp[0][0][1][1][0][0] = FLEX2;
		dp[0][0][1][0][1][0] = FLEX2;
		dp[0][0][0][1][1][0] = FLEX2;

		//白活二
		dp[0][2][2][0][0][0] = flex2;
		dp[0][2][0][2][0][0] = flex2;
		dp[0][2][0][0][2][0] = flex2;
		dp[0][0][2][2][0][0] = flex2;
		dp[0][0][2][0][2][0] = flex2;
		dp[0][0][0][2][2][0] = flex2;

		//黑活一
		dp[0][1][0][0][0][0] = FLEX1;
		dp[0][0][1][0][0][0] = FLEX1;
		dp[0][0][0][1][0][0] = FLEX1;
		dp[0][0][0][0][1][0] = FLEX1;

		//白活一
		dp[0][2][0][0][0][0] = flex1;
		dp[0][0][2][0][0][0] = flex1;
		dp[0][0][0][2][0][0] = flex1;
		dp[0][0][0][0][2][0] = flex1;

		int p1, p2, p3, p4, p5, p6, x, y, ix, iy;

		for (p1 = 0; p1 < 3; p1++)
		{
			for (p2 = 0; p2 < 3; p2++)
			{
				for (p3 = 0; p3 < 3; p3++)
				{
					for (p4 = 0; p4 < 3; p4++)
					{
						for (p5 = 0; p5 < 3; p5++)
						{
							for (p6 = 0; p6 < 3; p6++)
							{
								x = 0;
								y = 0;
								ix = 0;
								iy = 0;
								if (p1 == 1)
									x++;
								else if (p1 == 2)
									y++;
								if (p2 == 1)
									x++, ix++;
								else if (p2 == 2)
									y++, iy++;
								if (p3 == 1)
									x++, ix++;
								else if (p3 == 2)
									y++, iy++;
								if (p4 == 1)
									x++, ix++;
								else if (p4 == 2)
									y++, iy++;
								if (p5 == 1)
									x++, ix++;
								else if (p5 == 2)
									y++, iy++;
								if (p6 == 1)
									ix++;
								else if (p6 == 2)
									iy++;

								//黑冲四
								if (x == 4 && y == 0 || ix == 4 && iy == 0)
								{
									if (dp[p1][p2][p3][p4][p5][p6] == 0)
									{
										dp[p1][p2][p3][p4][p5][p6] = BLOCK4;
									}
								}

								//白冲四
								else if (x == 0 && y == 4 || ix == 0 && iy == 4)
								{
									if (dp[p1][p2][p3][p4][p5][p6] == 0)
									{
										dp[p1][p2][p3][p4][p5][p6] = block4;
									}
								}

								//黑眠三
								else if (x == 3 && y == 0 || ix == 3 && iy == 0)
								{
									if (dp[p1][p2][p3][p4][p5][p6] == 0)
									{
										dp[p1][p2][p3][p4][p5][p6] = BLOCK3;
									}
								}

								//白眠三
								else if (x == 0 && y == 3 || ix == 0 && iy == 3)
								{
									if (dp[p1][p2][p3][p4][p5][p6] == 0)
									{
										dp[p1][p2][p3][p4][p5][p6] = block3;
									}
								}

								//黑眠二
								else if (x == 2 && y == 0 || ix == 2 && iy == 0)
								{
									if (dp[p1][p2][p3][p4][p5][p6] == 0)
									{
										dp[p1][p2][p3][p4][p5][p6] = BLOCK2;
									}
								}

								//白眠二
								else if (x == 0 && y == 2 || ix == 0 && iy == 2)
								{
									if (dp[p1][p2][p3][p4][p5][p6] == 0)
									{
										dp[p1][p2][p3][p4][p5][p6] = block2;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	inline void INV_B()
	{
		int INV[3] = {0, 2, 1};
		for (int i = 0; i < 15; i++)
			for (int j = 0; j < 15; j++)
				a[i][j] = INV[a[i][j]];
	}
	inline int H(int P) //对P的评分
	{
		if (P == 2)
			INV_B();
		int weight[17] = {0, 4000, -4000, 2000, -2000, 1000, -1000, 1000, -1000, 400, -600, 400, -600, 100, -150, 100, -150};

		int i, j, s;

		int stat[4][17] = {0};

		int STAT[17];

		//棋型统计
		for (i = 0; i < 15; i++)
		{
			for (j = 0; j < 10; j++)
			{
				s = dp[a[i][j]][a[i][j + 1]][a[i][j + 2]][a[i][j + 3]][a[i][j + 4]][a[i][j + 5]];

				stat[1][s]++;
			}
		}

		for (j = 0; j < 15; j++)
		{
			for (i = 0; i < 10; i++)
			{
				s = dp[a[i][j]][a[i + 1][j]][a[i + 2][j]][a[i + 3][j]][a[i + 4][j]][a[i + 5][j]];

				stat[2][s]++;
			}
		}

		for (i = 0; i < 10; i++)
		{
			for (j = 0; j < 10; j++)
			{
				s = dp[a[i][j]][a[i + 1][j + 1]][a[i + 2][j + 2]][a[i + 3][j + 3]][a[i + 4][j + 4]][a[i + 5][j + 5]];

				stat[3][s]++;
			}
		}
		if (a[0][10])
			s = dp[a[0][10]][a[1][11]][a[2][12]][a[3][13]][a[4][14]][3 - a[0][10]], stat[3][s]++;
		if (a[10][0])
			s = dp[a[10][0]][a[11][1]][a[12][2]][a[13][3]][a[14][4]][3 - a[10][0]], stat[3][s]++;
		for (i = 5; i < 15; i++)
		{
			for (j = 0; j < 10; j++)
			{
				s = dp[a[i][j]][a[i - 1][j + 1]][a[i - 2][j + 2]][a[i - 3][j + 3]][a[i - 4][j + 4]][a[i - 5][j + 5]];

				stat[0][s]++;
			}
		}
		if (a[0][4])
			s = dp[a[0][4]][a[1][3]][a[2][2]][a[3][1]][a[4][0]][3 - a[0][4]], stat[0][s]++;
		if (a[10][14])
			s = dp[a[10][14]][a[11][13]][a[12][12]][a[13][11]][a[14][10]][3 - a[10][14]], stat[0][s]++;
		s = 0;

		//初步评分累加
		for (i = 1; i < 17; i++)
		{
			s += (stat[1][i] + stat[2][i] + stat[3][i] + stat[0][i]) * weight[i];
			STAT[i] = (stat[1][i] > 0) + (stat[2][i] > 0) + (stat[3][i] > 0) + (stat[0][i] > 0);
		}
		if (P == 2)
			INV_B();
		int result = 0;

		//胜
		if (STAT[1] > 0)
		{
			s += 100000;
			result = 1;
		}

		//负
		else if (STAT[2] > 0)
		{
			s -= 100000;
			result = 2;
		}

		//对手冲四、活四
		else if (STAT[4] > 0 || STAT[6] > 0)
		{
			s -= 30000;
		}

		//对手无冲四、活四
		else if (STAT[4] == 0 && STAT[6] == 0)
		{
			int k = 0;

			//检验 冲四活三
			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					if (i != j)
					{
						k += stat[i][5] * stat[j][7];
					}
				}
			}

			//活四
			if (STAT[3] > 0)
			{
				s += 20000;
			}

			//双冲四
			else if (STAT[5] >= 2)
			{
				s += 20000;
			}

			//冲四活三
			else if (k > 0)
			{
				s += 20000;
			}

			//对手有活三
			else if (STAT[8] > 0 && STAT[5] == 0)
			{
				s -= 20000;
			}

			//双活三
			else if (STAT[7] >= 2 && STAT[8] == 0)
			{
				s += 10000;
			}
		}
		return s;
	}
	inline int E(int x, int y, int p)
	{
		a[x][y] = p;
		int h = H(p);
		a[x][y] = 0;
		return h;
	}
	inline void Seek(int P, vector<poi> &V)
	{
		int i, j, k;
		int worth[15][15];
		int B[15][15] = {0};
		for (i = 0; i < 15; i++)
		{
			for (j = 0; j < 15; j++)
			{
				if (a[i][j] != 0)
				{
					for (k = -3; k <= 3; k++)
					{
						if (i + k >= 0 && i + k < 15)
						{
							B[i + k][j] = 1;

							if (j + k >= 0 && j + k < 15)
							{
								B[i + k][j + k] = 1;
							}
							if (j - k >= 0 && j - k < 15)
							{
								B[i + k][j - k] = 1;
							}
						}
						if (j + k >= 0 && j + k < 15)
						{
							B[i][j + k] = 1;
						}
					}
				}
			}
		}
		//对于棋盘A上的空点，评估在该处落子后的局势
		for (i = 0; i < 15; i++)
		{
			for (j = 0; j < 15; j++)
			{
				worth[i][j] = -10000000;
				if (a[i][j] == 0 && B[i][j] == 1)
				{
					worth[i][j] = E(i, j, MSF) * 7 + E(i, j, 3 - MSF) * 3;
				}
			}
		}

		int w;

		//筛选最佳的N个点
		if (MSF == P)
			for (k = 0; k < LIMIT; k++)
			{
				w = -10000000;
				int X, Y;
				for (i = 0; i < 15; i++)
				{
					for (j = 0; j < 15; j++)
					{
						if (worth[i][j] > w)
						{
							w = worth[i][j];
							X = i;
							Y = j;
						}
					}
				}
				if ((k > 0) && ((V[0].H - w) > 30000))
					break;
				if (w == -10000000)
					break;
				V.push_back((poi){X, Y, w});
				worth[X][Y] = -10000000;
			}
		else
		{
			for (k = 0; k < LIMIT; k++)
			{
				w = 10000000;
				int X, Y;
				for (i = 0; i < 15; i++)
				{
					for (j = 0; j < 15; j++)
					{
						if (worth[i][j] < w)
						{
							w = worth[i][j];
							X = i;
							Y = j;
						}
					}
				}
				if ((k > 0) && ((w - V[0].H) > 30000))
					break;
				if (w == 10000000)
					break;
				V.push_back((poi){X, Y, w});
				worth[X][Y] = 10000000;
			}
		}
	}
	inline int dfs(int P, int dep, int alpha, int beta)
	{
		if (OT())
			return 0;
		vector<poi> V;
		V.clear();
		Seek(P, V);
		if (!dep)
			return V[0].H;
		if (P == MSF)
		{
			for (int i = 0; i < V.size(); i++)
			{
				a[V[i].x][V[i].y] = P;
				alpha = max(alpha, dfs(3 - P, dep - 1, alpha, beta));
				a[V[i].x][V[i].y] = 0;
				if (beta <= alpha)
					break;
			}
			return alpha;
		}
		else
		{
			for (int i = 0; i < V.size(); i++)
			{
				a[V[i].x][V[i].y] = P;
				beta = min(beta, dfs(3 - P, dep - 1, alpha, beta));
				a[V[i].x][V[i].y] = 0;
				if (beta <= alpha)
					break;
			}
			return beta;
		}
	}
	inline poi Perform(int P)
	{
		int cnt = 0;
		for (int i = 0; i < 15; i++)
			for (int j = 0; j < 15; j++)
			{
				if (a[i][j])
					cnt++;
				a[i][j] = board[i][j];
			}
		if (cnt < 45)
			TIMELIMIT = 4;
		else
			TIMELIMIT = 1;
		MSF = P;
		St = clock();

		V.clear();
		Seek(MSF, V);
		//	for (int i = 0; i < V.size(); i++)printf("%d %d %d\n", V[i].x, V[i].y, V[i].H);
		for (dep = 1; !OT(); dep += 2)
		{
			for (int i = 0; i < V.size(); i++)
			{
				a[V[i].x][V[i].y] = MSF;
				V[i].H = dfs(3 - MSF, dep, -INF, INF);
				a[V[i].x][V[i].y] = 0;
			}
			if (!OT())
				G = V;
			sort(V.begin(), V.end(), AI::cmp);
		}
		sort(G.begin(), G.end(), AI::cmp);
		if (!G.size())
		{
			if(!a[7][7])
				G.push_back((poi){7, 7, 0});
			else
			{
				for (int i = 0;i<15;i++)
					for (int j = 0; j < 15;j++)
						if(!a[i][j])
							G.push_back((poi){i, j, 0});
			}
		}
		return G[0];
	}

  public:
	explicit AI(const vector<vector<int>> &board);

	void init();

	Position begin();

	Position turn(const Position &other);
};

AI::AI(const vector<vector<int>> &board) : board(board)
{
	this->boardSize = board.size();
}

class REPL
{
  private:
	vector<vector<int>> board;
	AI ai;

  public:
	REPL();

	~REPL() = default;

	void loop();
};

REPL::REPL() : ai(board)
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		board.emplace_back(move(vector<int>(BOARD_SIZE, 0)));
	}
}

void REPL::loop()
{
	while (true)
	{
		string buffer;
		getline(cin, buffer);
		istringstream ss(buffer);

		string command;
		ss >> command;
		if (command == START)
		{
			ai.init();
		}
		else if (command == PLACE)
		{
			int x, y, z;
			ss >> x >> y >> z;
			board[x][y] = z;
		}
		else if (command == DONE)
		{
			cout << "OK" << endl;
		}
		else if (command == BEGIN)
		{
			Position pos = ai.begin();
			board[pos.x][pos.y] = ME;
			cout << pos << endl;
		}
		else if (command == TURN)
		{
			int x, y;
			ss >> x >> y;
			board[x][y] = OTHER;
			Position pos = ai.turn({x, y});
			board[pos.x][pos.y] = ME;
			cout << pos << endl;
		}
		else if (command == END)
		{
			break;
		}
	}
}

/*
 * YOUR CODE BEGIN
 * 你的代码开始
 */

/*
 * You should init your AI here
 * 在这里初始化你的AI
 */
void AI::init()
{
	initdp();
}

/*
 * Game Start, you will put the first chess.
 * Warning: This method will only be called when after the initialize of the map,
 * it is your turn to put the chess, or this method will not be called.
 * You should return a valid Position variable.
 * 棋局开始，首先由你来落子
 * 请注意：只有在当棋局初始化后，轮到你落子时才会触发这个函数。如果在棋局初始化完毕后，轮到对手落子，则这个函数不会被触发。详见项目要求。
 * 你需要返回一个结构体Position，在x属性和y属性填上你想要落子的位置。
 */
Position AI::begin()
{
	poi w = Perform(1);
	return (Position){w.x, w.y};
}

Position AI::turn(const Position &other)
{
	poi w = Perform(1);
	return (Position){w.x, w.y};
}

/*
 * YOUR CODE END
 * 你的代码结束 
 */

int main(int argc, char *argv[])
{
	REPL repl;
	repl.loop();
	return 0;
}