/*
 *
 * 问题: n(<=1000)个非负整数(<=20000), 任意选择其中一些数按原顺序构成一个数列, 问可以构成多少个不同的等差数列
 * 
 * dp[i][j][0]表示以第i个数结尾, 公差为j的 递增 等差数列的个数
 * dp[i][j][1]表示以第i个数结尾, 公差为j的 递减 等差数列的个数
 * 使用vector数组vec, vec[h]存储所有数值为h的数的位置
 * 
 */



#include <cstdio>
#include <cstdlib>
#include <vector>
using std::vector;

#define inf_n 1000
#define inf_v 20000
#define mod 998244353

unsigned int dp[inf_n + 1][inf_v + 1][2];
vector<short> vec[inf_v + 1];

int main() {
    int n, h, maxh = 0;
    scanf("%d", &n);
    unsigned int ans = n; //初始先算上单个数字构成的等差数列
    for (short i = 1; i <= n; i++) {
        scanf("%d", &h); //输入第i个数
        //考虑以第i个数作为最后一个数, 能构造出多少个等差数列
        if (h > maxh) maxh = h; //记录当前最大值
        //考虑递增数列情况
        for (int j = 0; j <= h; j++) { //枚举公差
            //等差数列的最后一个数为h, 公差为j, 那么数列前面一个数为 h - j
            //而所有数>=0, 所以公差j不能超过h
            for (short t : vec[h - j]) {
                //第t个输入的数为h - j, 可以与第i个数构成等差数列
                //dp[t][j][0]表示以第t个数结尾, j为公差的递增等差数列个数, 
                //这些数列与第i个数一起可以构成以第i个数结尾的等差数列
                //而第t个数单独一个数与第i个数亦可以构成以第i个数结尾的等差数列
                dp[i][j][0] += dp[t][j][0] + 1;
                dp[i][j][0] %= mod;
            }            
            ans += dp[i][j][0]; //统计答案
            ans %= mod;
        }
        //递减数列情况
        for (int j = 1; j <= maxh - h; j++) { //枚举公差
            //递减等差数列, 前面一个值为 h + j, 而当前已输入最大值为maxh, 所以 j <= max - h
            for (short t : vec[h + j]) {
                //第t个输入的数为h + j, 可以与第i个数构成递减的等差数列
                dp[i][j][1] += dp[t][j][1] + 1;
                dp[i][j][1] %= mod;
            }
            ans += dp[i][j][1]; //统计答案
            ans %= mod;
        }
        vec[h].push_back(i); //把第i个数加入到记录数值为h的vector中
    }
    printf("%u", ans);
    for (int i = 0; i <= maxh; i++)
        vector<short>().swap(vec[i]);
    return 0;
}
