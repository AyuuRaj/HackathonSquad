content = """# 🏆 Hackathon Squad: Maximum Skill Dream Team

A high-performance C++ solution for a heuristic optimization problem based on the **Maximum Weight Independent Set (MWIS)**.

## 📝 Problem Statement
Your university is hosting a hackathon. You need to assemble a dream team from a pool of freshman coders to maximize the **Total Skill Rating**. 
However, due to past drama, certain pairs of coders refuse to work together (conflicts). 
* **Goal:** Maximize the sum of skill ratings of the selected coders.
* **Condition:** The team must be strictly conflict-free (no two selected coders can have an edge between them).

## 🛑 Constraints
* $1 \\le N \\le 200,000$ (Total Coders)
* $0 \\le M \\le \\frac{N(N-1)}{2}$ (Total Conflicts)
* $1 \\le S_i \\le 1,000,000,000$ (Skill Rating)
* **Execution Time Limit:** 5 Minutes (Code strictly cuts off at 290 seconds).

## 🧠 Algorithmic Logic
Finding the absolute perfect team on a general graph of this size is NP-Hard. This solution uses an **Iterated Local Search (ILS)** with a high-speed $1$-opt swap to find a near-optimal solution within the time limit.

### 1. Greedy Initialization
We start by sorting nodes based on a heuristic metric:
$$\\text{Value} = \\frac{\\text{Skill}}{\\text{Number of Rivals} + 1}$$
We greedily add coders to the team following this order to establish a strong baseline score.

### 2. Local Search (Hill Climbing)
The code constantly tries to improve the current team using two passes:
* **Insertion Pass:** Can we add any free coders to the team without breaking the rules?
* **1-Opt Swap Pass:** We tentatively remove one coder from the team. If removing them frees up multiple high-skill rivals (whose combined skill is strictly greater than the removed coder), we permanently make the swap.

### 3. Perturbation (Escaping Local Optima)
Once the local search gets stuck and can't find any more profitable swaps, we "kick" the state:
* Randomly remove $\\sim 20\\%$ of the currently selected coders.
* Lock the remaining $80\\%$ in place.
* Rebuild the rest of the team greedily and restart the Local Search.
We track the absolute best score across all perturbations.

## ⚡ High-Performance Features
To run as many iterations as possible within 290 seconds, the code is hyper-optimized:
* **$O(1)$ Conflict Tracking:** Instead of checking a node's neighbors every time, we maintain a `conflict_cnt` array. A node is free to join the team if and only if `conflict_cnt[i] == 0`. Updates take $O(\\text{degree})$ time.
* **Vectorized State:** Uses `vector<char>` instead of `vector<bool>` to avoid the bit-packing overhead, drastically improving read/write speeds.
* **Cache-Friendly Graph:** Uses `vector<vector<int>>` instead of `unordered_set` for the adjacency list to maximize CPU cache hits during neighbor traversal.
* **Pre-sorted Loops:** Nodes are sorted by skill *once* globally, avoiding redundant $O(N \\log N)$ operations inside the fast inner loops.

## 🚀 How to Run
Compile with high optimization flags (`-O3`) for maximum iteration throughput.