# P1027 Car的旅行路线

[原题地址](https://www.luogu.org/problemnew/show/P1027)

## 思路

用Airport结构体存储机场的x,y坐标，用`airports_`数组存储所有机场。顺序为先存储第0个城市的4个机场，然后第1个城市的4个机场。。。最后一个城市的4个机场。所以`airports_[i]`机场所在的城市编号为`i / 4`。而`第i个城市的第j个机场`，在`airports_`中的编号则为`i * 4 +ｊ`。这两个换算定义成了宏。

输入时通过每个城市的前三个机场坐标计算最后一个机场坐标。

用dijkstra计算起点到终点的最短路径。注意：

* dijkstra算法只有一个源，而在这里起点城市的4个机场均可作为起点，故将这4个机场与源的距离设为0。

* 只要收录的机场所在城市为终点城市，则达到目的地。

* 整个程序没有存储具体的图，对于每次收录的机场来讲，任何其他未收录的机场均为邻接点，计算它们的直线距离，此时若它们在同一城市，则它们之间的费用为直线距离乘以该城市的铁路费，否则费用为直线距离乘以航线费。

## 代码

```cpp
#include <iostream>
#include <stdio.h>
#include <math.h>
using namespace std;

#define N_AIRPORTS_PER_CITY 4 //每个城市的机场数
#define GET_CITY_INDEX(airport_idx) ((airport_idx) / N_AIRPORTS_PER_CITY) //得到一个机场所属的城市编号
#define GET_AIRPORT_INDEX(city_idx, i) ((city_idx) * N_AIRPORTS_PER_CITY + (i)) //得到一个城市第i个机场的编号
#define NO_VALUE -1

class CarRoute {
public:
    CarRoute() {}
    ~CarRoute() {}

    void solve(); //主函数

private:
    struct Airport {
        //机场的坐标
        int x; 
        int y;
    };

    int square(int x) { return x * x; }

    /* 返回两个机场间的直线距离 */
    float getAirportDist(Airport& a, Airport& b);

    /* 用dijkstra求最短路 */
    float dijkstra(int src_city, int des_city);

    int nAirports_; //总机场数量
    Airport *airports_; //机场数组，依次存储第0个，第1个。。。城市的4个机场

    int cost_air_; //航线单位里程的价格
    int *cost_train_; //cost_train_[i]为第i个城市中高速铁路的单位里程价格
};

void CarRoute::solve() {
    int nCities, src_city, des_city; //城市数，起点，终点
    scanf("%d %d %d %d", &nCities, &cost_air_, &src_city, &des_city);
    src_city--; //改为从0开始编号
    des_city--;
    nAirports_ = nCities * N_AIRPORTS_PER_CITY;
    airports_ = new Airport[nAirports_];
    cost_train_ = new int[nCities];
    for (int i = 0; i < nCities; i++) {
        int ax, ay, bx, by, cx, cy, dx, dy;
        //输入每个城市前三个机场
        scanf("%d %d %d %d %d %d %d", &ax, &ay, &bx, &by, &cx, &cy, cost_train_ + i);
        airports_[GET_AIRPORT_INDEX(i, 0)].x = ax;
        airports_[GET_AIRPORT_INDEX(i, 0)].y = ay;
        airports_[GET_AIRPORT_INDEX(i, 1)].x = bx;
        airports_[GET_AIRPORT_INDEX(i, 1)].y = by;
        airports_[GET_AIRPORT_INDEX(i, 2)].x = cx;
        airports_[GET_AIRPORT_INDEX(i, 2)].y = cy;
        int abSquare = square(ax - bx) + square(ay - by);
        int acSquare = square(ax - cx) + square(ay - cy);
        int bcSquare = square(bx - cx) + square(by - cy);
        if (abSquare == acSquare + bcSquare) { //c为直角顶点
            dx = ax + bx - cx;
            dy = ay + by - cy;
        }
        else if (acSquare == abSquare + bcSquare) { //b为直角顶点
            dx = ax + cx - bx;
            dy = ay + cy - by;
        }
        else { //a为直角顶点
            dx = bx + cx - ax;
            dy = by + cy - ay;
        }
        //最后一个机场的坐标已确定
        airports_[GET_AIRPORT_INDEX(i, 3)].x = dx;
        airports_[GET_AIRPORT_INDEX(i, 3)].y = dy;
    }
    float minCost = dijkstra(src_city, des_city); //计算最短路
    printf("%.1f\n", minCost);
    free(airports_);
    free(cost_train_);
}

float CarRoute::getAirportDist(Airport & a, Airport & b) {
    return sqrt(square(a.x - b.x) + square(a.y - b.y));
}

float CarRoute::dijkstra(int src_city, int des_city) {
    float *cost = new float[nAirports_]; //cost[i]表示从起点到第i个机场的当前最小费用
    bool *collected = new bool[nAirports_];
    fill(cost, cost + nAirports_, NO_VALUE);
    fill(collected, collected + nAirports_, false);
    for (int i = 0; i < 4; i++) //将起点城市的4个机场的cost设为0
        cost[GET_AIRPORT_INDEX(src_city, i)] = 0;
    while (true) {
        int minV = NO_VALUE;
        float minCost = NO_VALUE;
        //在未收录机场中找到cost最小的
        for (int i = 0; i < nAirports_; i++)
            if (!collected[i] && cost[i] != NO_VALUE && (cost[i] < minCost || minCost == NO_VALUE)) {
                minV = i;
                minCost = cost[i];
            }
        int minCity = GET_CITY_INDEX(minV); //minCity为最小cost机场minV所在城市
        if (minCity == des_city) { //若与终点在同一城市
            free(cost);
            free(collected);
            return minCost;
        }
        if (minV == NO_VALUE) break;
        collected[minV] = true; //收录minV
        for (int i = 0; i < nAirports_; i++) 
            if (!collected[i]) {
                float dist = getAirportDist(airports_[minV], airports_[i]); //dist为minV到i的直线距离
                //若minV与i在同一城市中，则从minV到i的费用为dist乘以该城市的铁路费用，否则为dist乘以航线费用
                float cost_tmp = GET_CITY_INDEX(i) == minCity ? dist * cost_train_[minCity] : dist * cost_air_;
                if (minCost + cost_tmp < cost[i] || cost[i] == NO_VALUE) { //若通过minV使cost[i]变小
                    cost[i] = minCost + cost_tmp; //更新
                }
            }
    } //while
    free(cost);
    free(collected);
    return NO_VALUE;
}


int main() {
    CarRoute carRoute;
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        carRoute.solve();
    }
    return 0;
}
```
