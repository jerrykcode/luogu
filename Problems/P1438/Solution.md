# P1438 无聊的数列
[原题地址](https://www.luogu.org/problem/P1438)

## 题目背景
无聊的YYB总喜欢搞出一些正常人无法搞出的东西。有一天，无聊的YYB想出了一道无聊的题：无聊的数列。。。（K峰：这题不是傻X题吗）

## 题目描述

维护一个数列{a[i]}，支持两种操作：

1、1 L R K D：给出一个长度等于R-L+1的等差数列，首项为K，公差为D，并将它对应加到a[L]~a[R]的每一个数上。即：令a[L]=a[L]+K，a[L+1]=a[L+1]+K+D，

a[L+2]=a[L+2]+K+2D……a[R]=a[R]+K+(R-L)D。

2、2 P：询问序列的第P个数的值a[P]。

## 输入格式

第一行两个整数数n，m，表示数列长度和操作个数。

第二行n个整数，第i个数表示a[i]（i=1,2,3…,n）。

接下来的m行，表示m个操作，有两种形式：

1 L R K D

2 P 字母意义见描述（L≤R）。

## 输出格式

对于每个询问，输出答案，每个答案占一行。

## 输入输出样例

### 输入 #1
> 5 2  
> 1 2 3 4 5  
> 1 2 4 1 2  
> 2 3  
### 输出 #1
> 6  

## 说明/提示

数据规模：

0≤n，m≤100000

|a[i]|，|K|，|D|≤200

Hint：

有没有巧妙的做法？

## 思路
### 概述
设原数组为a，考虑对a进行差分，对于差分数组d，`a[p] = d[1] + d[2] + ... + d[p]`，而对a[l]~a[r]加上等差数列的值，在数组d上的修改为：d[l] = a[l] - a[l - 1]，a[l]增加k，则`d[l]增加了k`。而d[l + 1] = a[l + 1] - a[l]，a[l + 1]增加k + d，a[l]增加k，即d[l + 1]增加d，同理`对于i属于[l + 1, r]，d[i]都增加了d`。最后d[r + 1] = a[r + 1] - a[r]，a[r + 1]不变而a[r]增加了k + (r - l)d，故`d[r + 1]增加了(l - r)d - k`。

到这里发现，对于a的操作，变换到d上，实际就成了`区间求和/修改`，a的更新操作，等价于d[l]和d[r + 1]更新以及d[l + 1]~d[r]区间更新。而得到a[p]的操作，相当于d[1]到d[p]区间求和。

那么再对d进行差分，就可以用树状数组维护了。  
(对于区间求和/修改，参考了[这里](https://www.cnblogs.com/dilthey/p/9366491.html#c))  

### 数学推导
原数组为a，  
对于差分d:  
d[1] = a[1]  
d[2] = a[2] - a[1]  
...  
d[i] = a[i] - a[i - 1]  
...  

对d再次差分得到数组e:  
e[1] = d[1] = a[1]  
e[2] = d[2] - d[1] = (a[2] - a[1]) - a[1] = a[2] - 2a[1]  
e[3] = d[3] - d[2] = (a[3] - a[2]) - (a[2] - a[1]) = a[3] - 2a[2] + a[1]  
...  
e[i] = d[i] - d[i - 1] = (a[i] - a[i - 1]) - (a[i - 1] - a[i - 2])  
= a[i] - 2a[i - 1] + a[i - 2]
...  
另设数组f，`f[i] = e[i] * i`，这个数组在对d区间求和时会用到。

##### 对于 1 l r k d 操作:

a[l]~a[r]增加了首项为k，公差为d的等差数列的值，  
由之前的概述已知，对应到d为:  
* d[l] 增加 k  
* d[i] 增加 d (i属于[l + 1, r])，即`d[l + 1]~d[r]区间增加d`  
* d[r + 1] 增加 (l - r)\*d - k  

对应到e为:
d[l]增加k导致`e[l]增加k，e[l + 1]增加 -k`，  
d[l + 1]~d[r]区间增加d，导致`e[l + 1]增加d，e[r + 1]增加 -d，而e[l + 2]到e[r]这一段由于抵消而不变化`。  
d[r + 1]增加(l - r)\*d - k导致`e[r + 1]增加(l - r)*d - k，e[r + 2]增加 k - (l - r)*d`  
综上，a的更新操作对应到e为:  
* e[l] 增加 k
* e[l + 1]增加 d- k
* e[r + 1] 增加 (l - r - 1)\*d - k
* e[r + 2] 增加 k - (l - r)\*d

同时对f也需要更新，`若e[i]增加了k，那么f[i]增加 k*i`，例如e[l]增加k则f[l]增加k\*l

对`e的树状数组`和`f的树状数组`进行以上4项更新即可完成`1 l r k d`操作。

##### 对于 2 p 操作

a[p]  
= d[1] + d[2] + ... + d[p]  
= e[1] \* p + e[2] \* (p - 1) + ... + e[p] \* 1  
= e[1] \* (p + 1) + e[2] \* (p + 1) + ... + e[p] \* (p + 1) - e[1] \* 1 - e[2] \* 2 - ... - e[p] \* p  
= (e[1] + e[2] + ... + e[p]) \* (p + 1) - (f[1] + f[2] + ... + f[p])  

而`e[1] + e[2] + ... + e[p]`和`f[1] + f[2] + ... + f[p]`都可以通过`e和f的树状数组求出`。`2 p`操作完成。


## 代码

由于对f的更新类似于对e的更新，故使用update_ef函数对e和f的树状数组同时进行更新。

```c
#include "stdio.h"
#include "stdlib.h"

int n;

int lowbit(int x) {
	return x & -x;
}

void update(int * bit, int x, int k) {
	for (; x <= n; x += lowbit(x)) {
		bit[x] += k;
	}
}

int sum(int * bit, int x) {
	//return sum of [1, x]
	int result = 0;
	for (; x > 0; x -= lowbit(x)) {
		result += bit[x];
	}
	return result;
}

int * e_bit, *f_bit;
void update_ef(int x, int k) {
	update(e_bit, x, k);
	update(f_bit, x, k * x); //fi = ei * i
}

int main() {
	int m;
	scanf("%d %d", &n, &m);
	int * a = (int *)malloc((n + 1) * sizeof(int));
	e_bit = (int *)malloc((n + 1) * sizeof(int));
	f_bit = (int *)malloc((n + 1) * sizeof(int));
	for (int i = 1; i <= n; i++) {
		e_bit[i] = f_bit[i] = 0;
		scanf("%d", a + i);
	}	
	//e1 = d1 = a1
	//f1 = e1 * 1	
	update_ef(1, a[1]);
	//e2 = d2 - d1 = (a2 - a1) - a1 = a2 - 2a1
	//f2 = e2 * 2
	update_ef(2, a[2] - 2 * a[1]);
	for (int i = 3, e; i <= n; i++) {
		//di = ai - a(i - 1)
		//ei = di - d(i - 1) = ai - 2a(i - 1) + a(i - 2)
		//fi = ei * i
		e = a[i] - 2 * a[i - 1] + a[i - 2];
		update_ef(i, e);
	}
	free(a);
	int cmd, l, r, k, d, p;
	for (int i = 0; i < m; i++) {
		scanf("%d", &cmd);
		if (cmd == 1) {
			scanf("%d %d %d %d", &l, &r, &k, &d);		
			update_ef(l, k);
			update_ef(l + 1, d - k);
			update_ef(r + 1, (l - r - 1) * d - k);
			update_ef(r + 2, (r - l) * d + k);	
		}
		else {
			scanf("%d", &p);
			printf("%d\n", sum(e_bit, p) * (p + 1) - sum(f_bit, p));
		}
	}
	free(e_bit);
	free(f_bit);
	return 0;
}
```