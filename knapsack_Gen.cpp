#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>
#include <chrono>
#include <stdexcept>

using namespace std;
using namespace chrono;

struct Item {
    int value;
    int weight;
};

class GeneticKnapsack {
    vector<Item> items;
    int capacity;
    int population_size;
    double mutation_rate;
    double crossover_rate;
    int max_generations;
    int penalty_factor;
    mt19937 rng;

    struct Chromosome {
        vector<bool> genes;
        int fitness = 0;
        int total_weight = 0;
    };

    vector<Chromosome> population;

    Chromosome createChromosome() {
        Chromosome ch;
        ch.genes.resize(items.size());
        uniform_int_distribution<int> dist(0, 1);

        do {
            ch.total_weight = 0;
            ch.fitness = 0;
            for (size_t i = 0; i < items.size(); ++i) {
                ch.genes[i] = dist(rng);
                if (ch.genes[i]) {
                    ch.total_weight += items[i].weight;
                    ch.fitness += items[i].value;
                }
            }
        } while (ch.total_weight > capacity);

        return ch;
    }

    void calculateFitness(Chromosome& ch) {
        ch.fitness = 0;
        ch.total_weight = 0;

        for (size_t i = 0; i < items.size(); ++i) {
            if (ch.genes[i]) {
                ch.fitness += items[i].value;
                ch.total_weight += items[i].weight;
            }
        }

        if (ch.total_weight > capacity) {
            ch.fitness -= penalty_factor * (ch.total_weight - capacity);
        }
    }

    Chromosome rankedSelection() {
        partial_sort(population.begin(), population.begin() + 10, population.end(), [](const Chromosome& a, const Chromosome& b) {
            return a.fitness > b.fitness;
        });
        uniform_int_distribution<int> dist(0, 9);
        return population[dist(rng)];
    }

    pair<Chromosome, Chromosome> crossover(const Chromosome& parent1, const Chromosome& parent2) {
        Chromosome child1 = parent1, child2 = parent2;

        if (uniform_real_distribution<double>(0, 1)(rng) < crossover_rate) {
            uniform_int_distribution<int> dist(0, items.size() - 1);
            int point = dist(rng);
            for (int i = point; i < items.size(); ++i) {
                swap(child1.genes[i], child2.genes[i]);
            }
        }

        return {move(child1), move(child2)};
    }

    void mutate(Chromosome& ch) {
        uniform_int_distribution<int> mutation_count(0, max(1, (int)(items.size() * mutation_rate)));
        int mutations = mutation_count(rng);
        uniform_int_distribution<int> gene_dist(0, items.size() - 1);

        for (int i = 0; i < mutations; ++i) {
            int index = gene_dist(rng);
            ch.genes[index] = !ch.genes[index];
        }
    }

public:
    GeneticKnapsack(const vector<Item>& items, int W, int pop_size = 100, double mut_rate = 0.01, double cross_rate = 0.8, int gens = 1000, int penalty = 10)
        : items(items), capacity(W), population_size(pop_size), mutation_rate(mut_rate), crossover_rate(cross_rate), max_generations(gens), penalty_factor(penalty), rng(random_device{}()) {}

    void createChromosome(Chromosome& ch) {
        ch.genes.assign(items.size(), false);
        vector<int> indices(items.size());
        iota(indices.begin(), indices.end(), 0);
        shuffle(indices.begin(), indices.end(), rng);

        ch.fitness = 0;
        ch.total_weight = 0;

        for (int i : indices) {
            if (ch.total_weight + items[i].weight <= capacity) {
                ch.genes[i] = true;
                ch.total_weight += items[i].weight;
                ch.fitness += items[i].value;
            }
        }
    }


    void run() {
        population.resize(population_size);
        for (auto& ch : population) {
            createChromosome(ch);
        }

        for (int gen = 0; gen < max_generations; ++gen) {
            vector<Chromosome> new_population;

            sort(population.begin(), population.end(), [](const Chromosome& a, const Chromosome& b) {
                return a.fitness > b.fitness;
            });

            new_population.push_back(population[0]);
            new_population.push_back(population[1]);

            while (new_population.size() < population_size) {
                auto parent1 = rankedSelection();
                Chromosome parent2;
                int attempts = 0;
                do {
                    parent2 = rankedSelection();
                    attempts++;
                } while (parent1.genes == parent2.genes && attempts < 10);

                auto [child1, child2] = crossover(parent1, parent2);
                mutate(child1);
                mutate(child2);

                calculateFitness(child1);
                calculateFitness(child2);

                new_population.push_back(move(child1));
                if (new_population.size() < population_size) {
                    new_population.push_back(move(child2));
                }
            }

            population = move(new_population);
        }
    }


    Chromosome getBestSolution() {
        return *max_element(population.begin(), population.end(), [](const Chromosome& a, const Chromosome& b) {
            return a.fitness < b.fitness;
        });
    }
};

int processKnapsackFile(vector<Item>& items, const string& file_path) {
    ifstream file(file_path);
    if (!file.is_open()) throw runtime_error("Cannot open file");

    int N, W;
    file >> N >> W;

    items.resize(N);
    for (int i = 0; i < N; ++i) {
        if (!(file >> items[i].value >> items[i].weight)) {
            throw runtime_error("Invalid file format");
        }
    }
    return W;
}

int main() {
    cout << "Enter file path: ";
    string file_path;
    getline(cin, file_path);

    try {
        vector<Item> items;
        int W = processKnapsackFile(items, file_path);

        auto start = high_resolution_clock::now();

        GeneticKnapsack solver(items, W, 150, 0.015, 0.85, 500, 100);
        solver.run();
        auto best = solver.getBestSolution();

        auto end = high_resolution_clock::now();

        cout << "\nBest value: " << best.fitness;
        cout << "\nBest weight: " << best.total_weight << "/" << W;
        cout << "\nTime: " << duration_cast<milliseconds>(end - start).count() << " ms\n";
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}
