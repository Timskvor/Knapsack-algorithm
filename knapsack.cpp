#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

int knapsack(vector<int> &values, int W, vector<int> &weights) {
    int n = weights.size();
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (weights[i - 1] <= w) {
                dp[i][w] = max(dp[i - 1][w], dp[i - 1][w - weights[i - 1]] + values[i - 1]);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    int res = dp[n][W];
    int w = W;
    vector<int> selected_items;

    for (int i = n; i > 0 && res > 0; i--) {
        if (res == dp[i - 1][w])
            continue;
        else {
            selected_items.push_back(i - 1);
            res -= values[i - 1];
            w -= weights[i - 1];
        }
    }

    cout << "Selected items: ";
    for (int i : selected_items) cout << i + 1 << " ";
    cout << endl;

    return dp[n][W];
}

int processKnapsackFile(vector<int>& values, vector<int>& weights, const string& file_path) {
    ifstream file(file_path);
    if (!file.is_open()) {
        throw runtime_error("Error: Unable to open file " + file_path);
    }

    int N, W;
    file >> N >> W;

    values.clear();
    weights.clear();
    values.reserve(N);
    weights.reserve(N);

    for (int i = 0; i < N; ++i) {
        int v, w;
        if (!(file >> v >> w)) {
            throw runtime_error("Error: Invalid data format in file");
        }
        values.push_back(v);
        weights.push_back(w);
    }

    return W;
}

int main() {
    cout << "Enter file path: " << endl;
    string file_path;
    getline(cin, file_path);

    vector<int> weights;
    vector<int> values;
    int W = processKnapsackFile(values, weights, file_path);
    cout << knapsack(values, W, weights) << endl;

    return 0;
}