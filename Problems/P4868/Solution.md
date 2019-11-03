# P4868 Preprefix sum
[原题地址](https://www.luogu.org/problem/P4868)

## 题目描述

前缀和(prefix sum)Si = a1 + a2 + ... + ak。

前前缀和(preprefix sum) 则把Si作为原序列再进行前缀和。记再次求得前缀和第i个是SSi 

给一个长度n的序列a1，a2，... an，有两种操作：

Modify i x：把ai改成x；
Query i：查询SSi

## 输入格式

第一行给出两个整数N，M。分别表示序列长度和操作个数  
接下来一行有N个数，即给定的序列a1,a2,....an  
接下来M行，每行对应一个操作，格式见题目描述  

## 输出格式
对于每个询问操作，输出一行，表示所询问的SSi的值。

## 输入输出样例

### 输入 #1
 > 5 3  
1 2 3 4 5  
Query 5  
Modify 3 2  
Query 5  

### 输出 #1
> 35  
32  

## 说明/提示
1<=N,M<=100000,且在任意时刻0<=Ai<=100000

## 思路

对于原数组a的前缀和s以及s的前缀和ss，  
由于ss[i] = s[1] + s[2] + ... + s[i]  
而更新a[i]则s[i]，s[i + 1]，...，s[n]都要更新。 
那么问题就转化为对`前缀和s的区间修改/求和操作。`  
区间修改/求和用树状数组做，就要求差分，而s的差分其实就是:  
s[i] - s[i - 1]  
= (s[i - 1] + a[i]) - s[i - 1]  
= a[i]  
到这里就太开心了，  
要记录的差分就是a数组本身 
用树状数组记录a，以及另一个数组b，b[i] = a[i]\*i  
对Modify i x操作，直接更新a和b的树状数组即可。  
而 
ss[i]  
= s[1] + s[2] + ... + s[i]  
= a[1] + a[1] + a[2] + ... + a[1] + a[2] + ... + a[i]  
= i \* a[1] + (i - 1) \* a[2] + ... + 1 \* a[i]  
= (i + 1) \* a[1] - 1 \* a[1] + (i + 1) \* a[2] - 2 \* a[2] + ... + (i + 1) \* a[i] - i \* a[i]  
= (i + 1) \* (a[1] + a[2] + ... + a[i]) - (1 \* a[1] + 2 \* a[2] + ... + i \* a[i])  
= (i + 1) \* (a[1] + a[2] + ... + a[i]) - (b[1] + b[2] + ... + b[i])  
那么查询ss[i]只需对a和b进行区间求和，  
通过a和b的树状数组也可以轻松实现。  

## Code

```cc
#include "stdio.h"
#include "stdlib.h"
#include <iostream>
#include <string>
using namespace std;

typedef long long LL;

int lowbit(int x) {
	return x & -x;
}

int n;

void update(LL * bit, int idx, LL k) {
	for (; idx <= n; idx += lowbit(idx)) {
		bit[idx] += k;
	}
}

LL sum(LL * bit, int idx) {
	LL result = 0;
	for (; idx > 0; idx -= lowbit(idx)) {
		result += bit[idx];
	}
	return result;
}

int main() {
	int m;
	scanf("%d %d", &n, &m);
	LL * bit_a = (LL *)malloc((n + 1) * sizeof(LL));
	LL * bit_b = (LL *)malloc((n + 1) * sizeof(LL));
	for (int i = 1; i < n + 1; i++) {
		bit_a[i] = bit_b[i] = 0;
	}
	LL *a = (LL *)malloc((n + 1) * sizeof(LL));
	for (int i = 1; i < n + 1; i++) {
		scanf("%lld", a + i);
		update(bit_a, i, a[i]);
		update(bit_b, i, a[i]*i);
	}
	//char cmd[6];
	string cmd;
	int idx;
	LL x;
	for (int i = 0; i < m; i++) {
		//scanf("%s", cmd);
		cin >> cmd;
		if (cmd[0] == 'M') {
			scanf("%d %lld", &idx, &x);
			update(bit_a, idx, x - a[idx]);
			update(bit_b, idx, (x - a[idx]) * idx);
			a[idx] = x;		
		}
		else {
			scanf("%d", &idx);			
			printf("%lld\n", sum(bit_a, idx) * (idx + 1) - sum(bit_b, idx));
		}		
	}
	free(bit_a);
	free(bit_b);
	free(a);
	return 0;
}
```