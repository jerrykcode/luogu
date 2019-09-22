# P4879 ycz的妹子

[原题地址](https://www.luogu.org/problem/P4879)  
[Github题解](https://github.com/jerrykcode/luogu)

## 题目背景

ycz有很多很多的妹子（ycz：瞎说）

## 题目描述

机房神犇ycz有n个青梅竹马，她们分别住在1~n号城市中。小时候的她们美丽可爱，但是由于女大十八变，有些妹子的颜值发生了变化，但是十分重感情的ycz神犇不忍心抛弃她们，于是记录下来了她们颜值变化的值，我们用Cxy表示第x个城市的妹子的颜值下降了y。长大之后的ycz非常有魅力，有许多妹子被ycz迷得神魂颠倒，我们用Ixy表示第x个城市有一个妹子喜欢上了ycz，她的颜值为y（y有可能是负数，但是ycz来者不拒）。但在中途有一些妹子和ycz吵架了，于是就分手了，我们用Dx表示**第x个妹子**和ycz分手了。

最近神犇ycz要去全国各地找他的妹子们，为了方便计算，我们珂以把ycz的妹子所在的城市当作是一条直线，并且挨在一起。神犇ycz由于忙于和他的妹子们联系此时已经很累了，于是交给你一个这样的任务：他想知道他在某个时间去找他的所有妹子们珂以获得多大的愉悦度，这个愉悦度为他找的妹子的颜值数，你要做的就是求出这个愉悦度之和（注意长大后妹子们的颜值可能为负数/滑稽）。

注意：每个城市只允许有一个妹子，也就是说后来喜欢上ycz的妹子会赶走之前这个城市喜欢ycz的妹子~~（一城不容二女）~~。

UPD:

青梅竹马都是喜欢ycz的。

分手的第x个妹子不是第x城市的妹子，是指从前往后数第x个有妹子的城市的妹子

青梅竹马长大后就是妹子

修改的值y不为负数，但是颜值减去之后可能为负数

## 输入格式

第一行两个正整数n和m  (1<=n<=100000)(1<=n<=100000)

第二行为n个整数ai​，表示小时候ycz的青梅竹马的颜值(1<=ai<=10^9)

接下来m行，每行为一条信息，每条信息可能是下面的一种:

Cxy表示第x个城市的妹子的颜值下降了y

Ixy表示在第x个城市有一个颜值为y的妹子迷上了ycz

Dx表示第x个妹子和ycz分手了

Q表示ycz现在想知道如果现在去找他所有的妹子们珂以获得多大的愉悦度

说明：妹子们居住的城市编号最大为5×10^5

## 输出格式

对于每一个QQ输出一个整数

## 输入输出样例

**输入 #1**

5 10  
1 2 3 4 5  
Q  
C 3 2  
Q  
I 6 6  
Q  
D 4  
Q  
C 5 2  
I 7 9  
Q  

**输出 #1**

15  
13  
19  
15  
22  

## 说明/提示

**样例解释：**

妹子颜值变化如下，删除的就没写在下面了。

1 2 1 4 5

1 2 1 4 5 6

1 2 1 5 6

1 2 1 3 6

1 2 1 3 6 9

对于30%的数据1<=n,m<=10

对于70%的数据1<=n,m<=1000

对于100%的数据1<=n,m<=100000,1<=ai​,y<=10^9

## 思路

使用数组a记录每个城市的女生的颜值，数组has记录每个城市是否有女生，若有则为TRUE(1)，没有则为FALSE(0)。
数组bit相当于是has的树状数组，提供两个操作:
* add(int x, int k)，在x位置增加k，那么add(x, 1)就表示原本x城市没有女生，现在有了；而add(x, -1)则表示原来有女生的城市x现在没有了。
* get_sum(int x)，返回[1, x]的和。get_sum(x)的实际意义就是[1, x]中有女生的城市的数量。

最大城市编号不超过500000，`#define maxcity 500000`，数组的大小都开maxcity + 1。用`sum`记录颜值和。

### 初始化:
* 输入时可确定a[1] ~ a[n]的值，此时sum即a[1] ~ a[n]之和。a[n + 1] ~ a[maxcity]为 0。  
* has[1] ~ has[n]为TRUE，has[n + 1] ~ has[maxcity]为FALSE。  
* bit初始全0，对于i属于[1, n]，对bit进行修改: add(i, 1)  

### C xy:   
更新sum和a[x]: `sum -= y;  a[x] -= y; `  
has和bit不变。

### I xy:
更新sum和a[x]:   
`sum += y - a[x];`  
`a[x] = y;`  
若has[x]为FALSE，即I操作之前x城市没有女生，则更新has[x]为TRUE，并且`add(x, 1)`

### D x:
首先需要找到第x个有女生的城市。  
二分 + 树状数组区间查询:  
从[1, maxcity]开始二分。对于二分的mid，若get_sum[mid] < x，则[0, mid]区间内的女生不够x个，则向右继续二分。若get_sum[mid] > x，则[0, mid]区间内的女生多于x个了，继续向左二分。
当get_sum[mid] == x时，[0, mid]区间内的女生正好有x个，注意此时若has[mid]为TRUE，即mid城市有女生，那么mid就是第x个有女生的城市。但若has[mid]为FALSE，
则第x个有女生的城市在mid之前，只是第x个有女生的城市到mid之间没有城市有女生，所以[0, mid]内仍然是x个女生。这种情况下也要向左二分。  
找到正确的mid之后更新sum和a[mid]: `sum -= a[mid]; a[mid] = 0;`，更新has[mid]为FALSE，并`add(mid, -1)`

### Q
输出sum即可。

## 代码

```c
#include "stdio.h"
#include "stdlib.h"

typedef unsigned char bool;
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define maxcity 500000

long long a[maxcity + 1];
int bit[maxcity + 1];
bool has[maxcity + 1];

int lowbit(int x) {
	return x & -x;
}

void add(int x, int k) {
	for (; x <= maxcity; x += lowbit(x)) {
		bit[x] += k;
	}
}

int get_sum(int x) {
	int result = 0;
	for (; x > 0; x -= lowbit(x)) {
		result += bit[x];
	}
	return result;
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	long long sum = 0;
	int i;
	for (i = 1; i < maxcity + 1; i++) bit[i] = 0;
	for (i = 1; i <= n; i++) {
		scanf("%lld", a + i);
		sum += a[i];
		add(i, 1);
		has[i] = TRUE;
	}
	for (; i < maxcity + 1; i++) {
		a[i] = 0;
		has[i] = FALSE;
	}
	char cmd;
	int x, y;
	for (i = 0; i < m; i++) {
		scanf(" %c", &cmd);
		switch (cmd) {
		case 'C': {
			scanf("%d %d", &x, &y);
			sum -= y;
			a[x] -= y;
			break;
		}
		case 'I': {
			scanf("%d %d", &x, &y);
			sum += y - a[x];
			a[x] = y;
			if (!has[x]) {
				has[x] = TRUE;
				add(x, 1);
			}
			break;
		}
		case 'D': {
			scanf("%d", &x);
			int left = 1, right = maxcity, mid, s;
			while (left <= right) {
				mid = (left + right) / 2;
				s = get_sum(mid);
				if (s == x) {
					if (has[mid]) {
						sum -= a[mid];
						a[mid] = 0;
						has[mid] = FALSE;
						add(mid, -1);
						break;
					}
					else {
						right = mid - 1;
					}
				}
				else if (s < x) {
					left = mid + 1;
				}
				else {
					right = mid - 1;
				}
			}
			break;
		}
		case 'Q': {
			printf("%lld\n", sum);
			break;
		}
		default:
			break;
		}
	}
	return 0;
}
```
> Written with [StackEdit](https://stackedit.io/).