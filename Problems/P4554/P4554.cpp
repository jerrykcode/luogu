#include <cstdio>
#include <cstdlib>
#include <queue>
#include <vector>
using namespace std;

typedef unsigned short uint16_t; //16位无符号类型
typedef uint16_t grid_t; //格子
typedef uint16_t dist_t; //距离(花费)
#define DIST_INF 0xffff

//坐标
struct Coordinate {
	grid_t row, col; //行与列
	Coordinate() {}
	Coordinate(grid_t row, grid_t col) : row(row), col(col) {}
};

//dijkstra中存入优先队列的坐标
struct PriorityCoordinate : public Coordinate {
	dist_t dist; //与源的距离
	PriorityCoordinate(grid_t row, grid_t col, dist_t dist) : Coordinate(row, col), dist(dist) {}
	
	//优先队列用于比较的结构体
	struct cmp {
		bool operator() (PriorityCoordinate& a, PriorityCoordinate& b) {
			return a.dist > b.dist;
		}
	};
};

//迭代器，用于枚举一个格子相邻的格子
struct Iterator {
	//构造函数，传入棋盘与尺寸
	Iterator(char ** board, grid_t n_rows, grid_t n_cols) : board(board), n_rows(n_rows), n_cols(n_cols) {}
	
	//设置格子
	bool setCoordinate(grid_t row, grid_t col) {
		if (row < n_rows && col < n_cols) {
			this->row = row;
			this->col = col;
			direction = UP; //初始向上开始找
			return true;
		}
		return false;
	}
	
	//得到下一个相邻格子的行与列
	bool next(grid_t * p_row, grid_t * p_col) {
		switch (direction) {
		case UP: { //向上找
			direction = LEFT; //下次向左
			if (row - 1 >= 0) { //上面存在格子
				*p_row = row - 1;
				*p_col = col;
				return true;
			}
			else //上面不存在
				return next(p_row, p_col); //递归找
			break;
		}
		case LEFT: { //向左找
			direction = RIGHT; //下次向右
			if (col - 1 >= 0) { //左边存在格子
				*p_row = row;
				*p_col = col - 1;
				return true;
			}
			else 
				return next(p_row, p_col); //递归找
			break;
		}
		case RIGHT: { //向右找
			direction = DOWN; //下次向下
			if (col + 1 < n_cols) { //右边存在格子
				*p_row = row;
				*p_col = col + 1;
				return true;
			}
			else 
				return next(p_row, p_col); //递归
			break;
		}
		case DOWN: { //向下找
			direction = END; //向下为最后可能得到的相邻格子，下次将结束寻找
			if (row + 1 < n_rows) { //下边存在格子
				*p_row = row + 1;
				*p_col = col;
				return true;
			}
			else
				return next(p_row, p_col); //递归，事实上direction更新为END后，递归也只可能返回false
			break;
		}
		case END :{ //无处可找
			return false; //返回false
			break;
		}
		default: return false; break;
		}	
		return false;
	}
private:
	char ** board; //棋盘
	grid_t n_rows, n_cols; //棋盘的尺寸
	grid_t row, col; //当前格子，next函数得到的便是row, col的相邻格子

	//寻找相邻格子的方向
	typedef enum {
		UP,
		LEFT,
		RIGHT,
		DOWN,
		END,
	} Direction;

	Direction direction;
};

//计算最短距离(最小花费)
//与普通dijkstra相同，只是图的顶点是棋盘中的一个格子(坐标)，一个顶点的邻接点为棋盘中与他相邻的格子
dist_t dijkstra(char ** board, grid_t n_rows, grid_t n_cols, Coordinate src, Coordinate des) {
	bool ** collected = new bool *[n_rows];
	dist_t ** dist = new dist_t *[n_rows];
	for (size_t i = 0; i < n_rows; i++) {
		collected[i] = new bool[n_cols];
		fill(collected[i], collected[i] + n_cols, false);
		dist[i] = new dist_t[n_cols];
		fill(dist[i], dist[i] + n_cols, DIST_INF);
	}
	priority_queue<PriorityCoordinate, vector<PriorityCoordinate>, PriorityCoordinate::cmp> pq;
	pq.push(PriorityCoordinate(src.row, src.col, 0));
	dist[src.row][src.col] = 0;
	grid_t top_row, top_col;	
	dist_t top_dist, tmp_dist;
	grid_t adj_row, adj_col;
	Iterator it(board, n_rows, n_cols);
	while (!pq.empty()) {
		top_row = pq.top().row;
		top_col = pq.top().col;
		top_dist = pq.top().dist;
		pq.pop();
		if (collected[top_row][top_col]) continue;
		collected[top_row][top_col] = true;
		if (top_row == des.row && top_col == des.col) {
			//到达终点
			while (!pq.empty()) pq.pop();
			break;
		}
		it.setCoordinate(top_row, top_col); //设置迭代器
		while (it.next(&adj_row, &adj_col)) { //遍历相邻的格子
			if (!collected[adj_row][adj_col]) {
				tmp_dist = top_dist + (board[top_row][top_col] == board[adj_row][adj_col] ? 0 : 1);
				if (tmp_dist < dist[adj_row][adj_col]) {
					//更新
					dist[adj_row][adj_col] = tmp_dist;
					pq.push(PriorityCoordinate(adj_row, adj_col, tmp_dist));
				}
			}
		}
	}
	dist_t result = dist[des.row][des.col];
	//释放内存
	for (size_t i = 0; i < n_rows; i++) {
		free(collected[i]);
		free(dist[i]);
	}
	free(collected);
	free(dist);
	return result;
}

int main() {
	grid_t n, m, x1, y1, x2, y2;
	Coordinate src, des;
	char ** board;
	while (1) {
		scanf("%hd %hd", &n, &m);		
		if (!n) break;
		//输入棋盘
		board = new char *[n];
		for (size_t i = 0; i < n; i++) {
			board[i] = new char[m];			
			for (size_t j = 0; j < m; j++) {		
				scanf(" %c", &board[i][j]);
			}
		}
		//输入起点，终点坐标
		scanf("%hd %hd %hd %hd", &x1, &y1, &x2, &y2);
		src.row = x1;
		src.col = y1;
		des.row = x2;
		des.col = y2;
		printf("%hd\n", dijkstra(board, n, m, src, des));
		for (size_t i = 0; i < n; i++) {
			free(board[i]);
		}
		free(board);
	}
	return 0;
}
