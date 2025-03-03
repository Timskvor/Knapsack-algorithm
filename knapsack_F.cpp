#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <algorithm>

using namespace std;

int knapsack(const vector<int>& values, int W, const vector<int>& weights, vector<int>& selected_items) {
    vector<int> dp(W + 1, 0);

    for (size_t i = 0; i < weights.size(); ++i) {
        int wgt = weights[i], val = values[i];
        for (int w = W; w >= wgt; --w) {
            if (dp[w - wgt] + val > dp[w]) {
                dp[w] = dp[w - wgt] + val;
            }
        }
    }

    int current_weight = W;
    selected_items.clear();
    for (int i = weights.size() - 1; i >= 0; --i) {
        if (current_weight >= weights[i] && dp[current_weight] == dp[current_weight - weights[i]] + values[i]) {
            selected_items.push_back(i);
            current_weight -= weights[i];
        }
    }
    reverse(selected_items.begin(), selected_items.end());

    return dp[W];
}

int processKnapsackFile(vector<int>& values, vector<int>& weights, const string& file_path) {
    ifstream file(file_path, ios::binary | ios::ate);
    if (!file.is_open())
        throw runtime_error("Error: Unable to open file " + file_path);

    streamsize size = file.tellg();
    file.seekg(0, ios::beg);
    vector<char> buffer(size);
    file.read(buffer.data(), size);

    const char* data = buffer.data();
    int N, W, offset;
    if (sscanf(data, "%d %d%n", &N, &W, &offset) != 2)
        throw runtime_error("Invalid data format for N and W");
    data += offset;

    values.clear(); weights.clear();
    values.reserve(N); weights.reserve(N);

    for (int i = 0; i < N; ++i) {
        int v, w;
        if (sscanf(data, "%d %d%n", &v, &w, &offset) != 2)
            throw runtime_error("Invalid data for item " + to_string(i+1));
        data += offset;
        values.push_back(v);
        weights.push_back(w);
    }
    return W;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string file_path;
    getline(cin, file_path);

    vector<int> values, weights, selected_items;
    int W = processKnapsackFile(values, weights, file_path);

    cout << knapsack(values, W, weights, selected_items) << endl;
    cout << "Selected items: ";
    for (int i : selected_items) cout << i + 1 << " ";
    cout << endl;

    return 0;
}