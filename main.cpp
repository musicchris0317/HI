#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

int N, K;
vector<long long> p;
vector<long long> pref;     // p[i] 的前綴和
vector<long long> pref_idx; // p[i] * i 的前綴和
long long total_sum = 0;    // 紀錄所有切點 p[m] 的總和

// O(1) 取得區間 [L, R] 的 p[i] 總和
long long get_sum(int L, int R) {
    if (L > R) return 0;
    return pref[R] - pref[L - 1];
}

// O(1) 取得區間 [L, R] 的 p[i] * i 總和
long long get_sum_idx(int L, int R) {
    if (L > R) return 0;
    return pref_idx[R] - pref_idx[L - 1];
}

// 計算給定切點 m 在區間 [s, t] 的力矩絕對差值
long long get_diff(int s, int t, int m) {
    long long left_torque = m * get_sum(s, m - 1) - get_sum_idx(s, m - 1);
    long long right_torque = get_sum_idx(m + 1, t) - m * get_sum(m + 1, t);
    return abs(left_torque - right_torque);
}

// 遞迴切割函數
void solve(int s, int t, int level) {
    // 終止條件：區間長度小於 3，或超過限制層級 K
    if (t - s + 1 < 3 || level > K) return;

    int best_m = s + 1;
    long long min_diff = get_diff(s, t, s + 1);

    // 利用二分搜尋概念找出力矩黃金交叉點
    // 因為左力矩隨 m 遞增，右力矩隨 m 遞減，差值呈 V 字形
    int low = s + 1, high = t - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        long long diff = get_diff(s, t, mid);
        
        if (diff < min_diff) {
            min_diff = diff;
            best_m = mid;
        } else if (diff == min_diff) {
            best_m = min(best_m, mid); // 相同差異時，取編號較小者
        }

        // 判斷該往哪邊逼近
        long long left_torque = mid * get_sum(s, mid - 1) - get_sum_idx(s, mid - 1);
        long long right_torque = get_sum_idx(mid + 1, t) - mid * get_sum(mid + 1, t);
        
        if (left_torque < right_torque) {
            low = mid + 1; // 左邊太輕，切點往右移
        } else {
            high = mid - 1; // 右邊太輕，切點往左移
        }
    }
    
    // 再次確保周圍鄰近點有被檢查到（防二分搜邊界微小誤差）
    for (int i = max(s + 1, best_m - 2); i <= min(t - 1, best_m + 2); ++i) {
        long long diff = get_diff(s, t, i);
        if (diff < min_diff) {
            min_diff = diff;
            best_m = i;
        } else if (diff == min_diff) {
            best_m = min(best_m, i);
        }
    }

    // 累加切點的數值
    total_sum += p[best_m];

    // 繼續遞迴左右子區間
    solve(s, best_m - 1, level + 1);
    solve(best_m + 1, t, level + 1);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N >> K)) return 0;

    p.resize(N + 1);
    pref.resize(N + 1, 0);
    pref_idx.resize(N + 1, 0);

    for (int i = 1; i <= N; ++i) {
        cin >> p[i];
        pref[i] = pref[i - 1] + p[i];
        pref_idx[i] = pref_idx[i - 1] + p[i] * i;
    }

    solve(1, N, 1);

    cout << total_sum << "\n";

    return 0;
}