# P3946 ことりのおやつ��弌抵議泣伉��
[圻籾仇峽](https://www.luogu.org/problemnew/show/P3946)

宸頁2017定議喬爺。��嗽欺阻易弼�牴承勅晶�2333��

![錆僣抵](https://db.loveliv.es/png/navi/476/0)
錆頼僣岻朔��ことり融隼�覲垉穉沈横〉敲破�肇阻矢瞳糾。

晩云議喬爺将械和僣。音侑議頁��書爺匆頁宸劔��耽昼嶝僣議搾業氏奐紗q坐致。

拍匐圻慌嗤n倖仇泣��園催貫1欺n。耽倖仇泣壓蝕兵議扮昨議持僣互業葎hi。

嗤m訳褒�魑逝形�俊宸乂仇泣��万断議海業蛍艶葎wi致。

僣湊寄��巷慌住宥狼由厮将唯畏阻��侭參ことり誼恠揃指社。慢恠揃議堀業頁1m/s。

葎阻圭宴仇夕議紙崙��拍匐圻議祇揃号皿聞誼耽訳祇揃冢鯉仇銭俊曾倖音揖議仇泣��旺拝音氏嗤曾訳祇揃銭俊議仇泣�猴�。

耽倖仇泣脅嗤匯倖自�渮�互li��汽了頁坐致��泌惚欺器宸倖仇泣議扮昨��宸戦議僣議互業互噐li夸氏瓜是壓宸倖泣恠音竃肇��涙隈撹孔仇恠欺ことり社。

泣伉糾宸倖仇泣議園催頁s��ことり社議園催頁t。

音深打泣伉糾才ことり社議僣。

ことり�誑�g昼坪指欺社郭泣伉��埆酔埆挫。泌惚壓g昼岻坪��ことり涙隈欺社��賜宀慢瓜是壓揃貧阻��椎担ことり氏委wtnap延撹慢議泣伉( ? 8 ? )

補秘補竃鯉塀
------

補秘鯉塀��

及1佩6倖屁方��腎鯉侯蝕��蛍艶旗燕n��m��s��t��g��q。

參和n佩��耽佩2倖屁方��腎鯉侯蝕��蛍艶燕幣宸倖仇泣議hi才li。

參和m佩��耽佩3倖屁方��腎鯉侯蝕��蛍艶燕幣宸訳揃銭俊議曾倖仇泣u, v才宸訳揃議海業wi。

補竃鯉塀��

補竃1佩1倖屁方��燕幣欺器ことり社議恷玉喘扮。

泌惚wtnap延撹阻ことり議泣伉椎担補竃"wtnap wa kotori no oyatsu desu!"

補竃扮音根哈催。

補秘補竃劔箭
------

補秘劔箭#1:

>2 1 1 2 10 1  
>1 10  
>3 10  
>1 2 6  

補竃劔箭#1:

>6  

補秘劔箭#2:

>5 6 2 5 10 1  
>1 10  
>1 10  
>1 10  
>1 10  
>1 10  
>1 5 9  
>1 3 9  
>2 4 1  
>2 5 9  
>3 4 1  
>3 5 6  

補竃劔箭#2:

>8  

補秘劔箭#3:

>5 6 2 5 10 1  
>1 10  
>1 10  
>10 10  
>1 10  
>1 10  
>1 5 9  
>1 3 9  
>2 4 1  
>2 5 11  
>3 4 1  
>3 5 6  

補竃劔箭#3:

>wtnap wa kotori no oyatsu desu!  

傍苧
--

斤噐0%議方象��嚥劔箭匯庁匯劔��\
斤噐40%議方象��q = 0。\
斤噐貧匯佩嶄50%議方象��侭嗤wi < li。\
斤噐100%議方象��1 ＋ s, t ＋ n; 0 ＋ g, q ＋ 10^9; 0 ＋ wi ＋ li ＋ 10^9。

![](https://cdn.luogu.org/upload/pic/9826.png)




## 房揃

Dijkstra��耽肝起竃dist峙恷弌議競泣朔��飛乎競泣音辛器(dist峙*q > �涕� - 兜兵互業)��夸音制婢凪惣俊泣。


## 旗鷹

```cpp
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

#define NO_VALUE -1

typedef long long LL;
struct AdjNode {
	int adj_v;
	int adj_weight;
	AdjNode(int adj_v, int adj_weight) : adj_v(adj_v), adj_weight(adj_weight) {}
};

class Dijkstra {
public:
	bool dijkstra(vector<AdjNode> *graph, int nv, int src, int des, int q, LL * v_height, LL *p_shortest_dist);
private:
	struct PriorityNode {
		int v;
		LL dist;
		PriorityNode(int v, LL dist) : v(v), dist(dist) {}
	};
	struct cmp {
		bool operator() (PriorityNode& a, PriorityNode& b) {
			return a.dist > b.dist;
		}
	};

	LL * dist_;
	bool * collected_;
	priority_queue<PriorityNode, vector<PriorityNode>, cmp> pq_;
};

bool Dijkstra::dijkstra(vector<AdjNode>* graph, int nv, int src, int des, int q, LL * v_height, LL * p_shortest_dist) {
	dist_ = new LL[nv];
	fill(dist_, dist_ + nv, NO_VALUE);
	collected_ = new bool[nv];
	fill(collected_, collected_ + nv, false);
	dist_[src] = 0;
	pq_.push(PriorityNode(src, 0));
	int top_v, adj_v;
	LL tmp_dist;
	while (!pq_.empty()) {
		top_v = pq_.top().v;
		pq_.pop();
		if (collected_[top_v]) continue;
		collected_[top_v] = true;
		if (top_v == des) {
			while (!pq_.empty()) pq_.pop();
			break;
		}
		if (dist_[top_v] * q > v_height[top_v]) continue;
		for (auto it = graph[top_v].begin(); it != graph[top_v].end(); it++) {
			adj_v = it->adj_v;
			if (!collected_[adj_v]) {
				tmp_dist = dist_[top_v] +  it->adj_weight;
				if (tmp_dist < dist_[adj_v] || dist_[adj_v] == NO_VALUE) {
					dist_[adj_v] = tmp_dist;
					pq_.push(PriorityNode(adj_v, tmp_dist));
				}
			}
		} //for
	} //while
	bool result = collected_[des];
	*p_shortest_dist = dist_[des];
	free(dist_);
	free(collected_);
	return result;
}

int main() {
	int n, m, s, t, g, q;
	scanf("%d %d %d %d %d %d", &n, &m, &s, &t, &g, &q);
	s--; t--;
	LL * height = new LL[n];
	LL hi, li;
	for (size_t i = 0; i < n; i++) {
		scanf("%d %d", &hi, &li);
		height[i] = li - hi;
	}
	vector<AdjNode> * graph = new vector<AdjNode>[n];
	int u, v, wi;
	for (size_t i = 0; i < m; i++) {
		scanf("%d %d %d", &u, &v, &wi);
		u--; v--;
		graph[u].push_back(AdjNode(v, wi));
		graph[v].push_back(AdjNode(u, wi));
	}
	Dijkstra dijkstra;
	LL shortest_dist;
	if (dijkstra.dijkstra(graph, n, s, t, q, height, &shortest_dist) && shortest_dist <= g) {
		printf("%lld", shortest_dist);
	}
	else printf("wtnap wa kotori no oyatsu desu!");
	free(height);
	for (size_t i = 0; i < n; i++) {
		vector<AdjNode>().swap(graph[i]);
	}
	return 0;
}
```