#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <queue>
#include <chrono>
#include <stdexcept>

using namespace std;
using namespace chrono;

struct Item {
    int value;
    int weight;
    double ratio;
    int index;
};

struct Node {
    int level;
    int weight;
    int value;
    double bound;
    vector<bool> taken;

    bool operator<(const Node& other) const {
        return bound < other.bound;
    }
};

class KnapsackBB {
    vector<Item> items;
    int capacity;
    int max_value;
    vector<bool> best_taken;
    int n;

    double calculateBound(const Node& node) {
        if (node.weight >= capacity) return 0;

        double bound = node.value;
        int total_weight = node.weight;
        int next_level = node.level + 1;

        while (next_level < n && total_weight + items[next_level].weight <= capacity) {
            bound += items[next_level].value;
            total_weight += items[next_level].weight;
            next_level++;
        }

        if (next_level < n) {
            bound += (capacity - total_weight) * items[next_level].ratio;
        }

        return bound;
    }

public:
    KnapsackBB(const vector<int>& values, const vector<int>& weights, int W) {
        n = values.size();
        capacity = W;
        max_value = 0;
        best_taken.resize(n, false);

        for (int i = 0; i < n; ++i) {
            items.push_back({
                values[i],
                weights[i],
                static_cast<double>(values[i])/weights[i],
                i
            });
        }

        sort(items.begin(), items.end(),
            [](const Item& a, const Item& b) { return a.ratio > b.ratio; });
    }

    int solve() {
        priority_queue<Node> pq;

        Node root{-1, 0, 0, 0.0, vector<bool>(n, false)};
        root.bound = calculateBound(root);
        pq.push(root);

        while (!pq.empty()) {
            Node current = pq.top();
            pq.pop();

            if (current.value > max_value) {
                max_value = current.value;
                best_taken = current.taken;
            }

            if (current.bound <= max_value) continue;

            if (current.level == n - 1) continue;

            Node include = current;
            include.level++;
            Item& item = items[include.level];

            if (include.weight + item.weight <= capacity) {
                include.taken[include.level] = true;
                include.weight += item.weight;
                include.value += item.value;
                include.bound = calculateBound(include);

                if (include.bound > max_value) {
                    pq.push(include);
                }
            }

            Node exclude = current;
            exclude.level++;
            exclude.bound = calculateBound(exclude);
            if (exclude.bound > max_value) {
                pq.push(exclude);
            }
        }

        return max_value;
    }

    vector<int> getSelectedIndices() {
        vector<int> indices;
        for (int i = 0; i < n; ++i) {
            if (best_taken[i]) {
                indices.push_back(items[i].index);
            }
        }
        return indices;
    }
};

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
    cin.tie(nullptr);

    cout << "Enter file path: ";
    string file_path;
    getline(cin, file_path);

    vector<int> values, weights;
    int W = processKnapsackFile(values, weights, file_path);

    vector<int> filtered_values, filtered_weights;
    vector<int> original_indices;
    for (size_t i = 0; i < weights.size(); ++i) {
        if (weights[i] <= W) {
            filtered_values.push_back(values[i]);
            filtered_weights.push_back(weights[i]);
            original_indices.push_back(i);
        }
    }

    auto start = high_resolution_clock::now();

    KnapsackBB solver(filtered_values, filtered_weights, W);
    int max_value = solver.solve();
    vector<int> selected = solver.getSelectedIndices();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    vector<int> selected_original;
    for (int idx : selected) {
        selected_original.push_back(original_indices[idx]);
    }

    cout << "\nSelected items: ";
    for (int idx : selected_original) {
        cout << idx + 1 << " ";
    }
    cout << "\nMaximum value: " << max_value;
    cout << "\nExecution time: " << duration.count() << " ms" << endl;

    return 0;
}