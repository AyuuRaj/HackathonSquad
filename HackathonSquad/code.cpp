#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using vi = vector<int>;
using vll = vector<ll>;

static int N, M;
static vll skill;
static vector<vi> adj; 
static vi order;

static vector<char> sel;
static vector<char> best;
static vi conflict_cnt;

static auto START = chrono::steady_clock::now();
inline double elapsed() {
    return chrono::duration<double>(chrono::steady_clock::now() - START).count();
}
static constexpr double TIME_LIMIT = 290.0;

inline void add_node(int v, ll& score) {
    sel[v] = 1;
    score += skill[v];
    for (int u : adj[v]) conflict_cnt[u]++;
}

inline void remove_node(int v, ll& score) {
    sel[v] = 0;
    score -= skill[v];
    for (int u : adj[v]) conflict_cnt[u]--;
}

void localSearch(ll& cand_score) {
    bool improved = true;
    while (improved && elapsed() < TIME_LIMIT) {
        improved = false;

        for (int v : order) {
            if (!sel[v] && conflict_cnt[v] == 0) {
                add_node(v, cand_score);
                improved = true;
            }
        }

        if (elapsed() >= TIME_LIMIT) break;

        for (int i = N - 1; i >= 0; i--) {
            int v = order[i];
            if (!sel[v]) continue;

            remove_node(v, cand_score);

            vi to_add;
            for (int nb : adj[v]) {
                if (!sel[nb] && conflict_cnt[nb] == 0) {
                    to_add.push_back(nb);
                }
            }

            sort(to_add.begin(), to_add.end(), [&](int a, int b) {
                return skill[a] > skill[b];
            });

            ll gain = -skill[v];
            vi actually_added;

            for (int u : to_add) {
                if (conflict_cnt[u] == 0) {
                    sel[u] = 1;
                    gain += skill[u];
                    actually_added.push_back(u);
                    for (int w : adj[u]) conflict_cnt[w]++;
                }
            }

            if (gain > 0) {
                cand_score += (gain + skill[v]);
                improved = true;
            } else {
                for (int u : actually_added) {
                    sel[u] = 0;
                    for (int w : adj[u]) conflict_cnt[w]--;
                }
                sel[v] = 1;
                cand_score += skill[v];
                for (int u : adj[v]) conflict_cnt[u]++;
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> N >> M)) return 0;

    skill.resize(N + 1);
    adj.resize(N + 1);
    sel.resize(N + 1, 0);
    best.resize(N + 1, 0);
    conflict_cnt.resize(N + 1, 0);

    for (int i = 1; i <= N; i++) cin >> skill[i];
    for (int i = 0; i < M; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    order.resize(N);
    iota(order.begin(), order.end(), 1);
    sort(order.begin(), order.end(), [&](int a, int b) {
        return skill[a] > skill[b];
    });

    ll cand_score = 0;
    vi greedy_order = order;
    sort(greedy_order.begin(), greedy_order.end(), [&](int a, int b) {
        double ra = (double)skill[a] / (adj[a].size() + 1);
        double rb = (double)skill[b] / (adj[b].size() + 1);
        return ra > rb;
    });

    for (int v : greedy_order) {
        if (conflict_cnt[v] == 0) {
            add_node(v, cand_score);
        }
    }

    localSearch(cand_score);

    best = sel;
    ll best_score = cand_score;

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    while (elapsed() < TIME_LIMIT) {
        vi selected;
        for (int i = 1; i <= N; i++) {
            if (best[i]) selected.push_back(i);
        }

        int remove_count = max(1, (int)selected.size() / 5);
        shuffle(selected.begin(), selected.end(), rng);

        vector<char> excluded(N + 1, 0);
        for (int i = 0; i < remove_count; i++) excluded[selected[i]] = 1;

        fill(sel.begin(), sel.end(), 0);
        fill(conflict_cnt.begin(), conflict_cnt.end(), 0);
        cand_score = 0;

        for (int v : selected) {
            if (!excluded[v]) {
                add_node(v, cand_score);
            }
        }

        vi pool;
        for (int v = 1; v <= N; v++) {
            if (!sel[v] && conflict_cnt[v] == 0 && !excluded[v]) {
                pool.push_back(v);
            }
        }

        sort(pool.begin(), pool.end(), [&](int a, int b) {
            double ra = (double)skill[a] / (adj[a].size() + 1);
            double rb = (double)skill[b] / (adj[b].size() + 1);
            return ra > rb;
        });

        for (int v : pool) {
            if (conflict_cnt[v] == 0) {
                add_node(v, cand_score);
            }
        }

        localSearch(cand_score);

        if (cand_score > best_score) {
            best_score = cand_score;
            best = sel;
        }
    }

    cout << best_score << "\n";
    bool first = true;
    for (int i = 1; i <= N; i++) {
        if (best[i]) {
            if (!first) cout << " ";
            cout << i;
            first = false;
        }
    }
    cout << "\n";

    return 0;
}