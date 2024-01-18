#include "cube.hpp"
#include <algorithm>
#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <random>
#include <set>
#include <utility>
#include <vector>
using namespace std;
// Alphabetically ordered header files :D

const int states = 3674160;
// Number of states

int cube_hash(cube &c) {
    /* Hash is calculated as:
     * L = Lehmer code of permutation of corners
     *      there are 7 such corners, one is fixed (UBL)
     *
     * A = Amount of possible corner orientations for a given permutation of corners
     *      Orientation of UBL is fixed
     *      Orientation of DBL depends on rest of the corners
     *      Each other corner has 3 orientations each
     *      so A = 3^6 = 729
     *
     * D = Decimal equivalent of the orientation of corners expressed in ternary
     *      there are 729 such cases, D lies between 0 and 728
     *
     *  Final Hash = LA + D
     *
     * This hash is 1:1 for all ~3.6 million nodes
     *
     */

    set<int> os = {1, 2, 3, 4, 5, 6, 7};
    vector<int> factorials = {1, 1, 2, 6, 24, 120, 720, 5040, 40320};
    vector<int> pow3 = {1, 3, 9, 27, 81, 243, 729};
    vector<int> factoradic;
    int final_hash = 0;

    for (int i = 1; i < 7; i++) {
        int idx = 0;
        for (auto it: os) {
            if (it == c.pieces[i]) {
                factoradic.push_back(idx);
                os.erase(it);
            }
            idx++;
        }
    }

    for (int i = 0; i < factoradic.size(); i++) final_hash += factoradic[i] * factorials[6 - i]; //L

    final_hash *= 729; // A

    for (int i = 1; i < 7; i++) final_hash += c.orientations[i] * pow3[6 - i]; // D

    return final_hash;
}

cube unhash(int hash) {
    // Generates a cube from the given hash
    // Essentially the reverse of the hash function
    // 1 to 1 correspondence so this is possible


    cube c;
    int or_sum = 0;
    int perm = hash / 729;
    int orie = hash % 729;
    vector<int> factoradic;
    vector<int> factorials = {1, 1, 2, 6, 24, 120, 720, 5040, 40320};
    vector<int> pow3 = {1, 3, 9, 27, 81, 243, 729};
    set<int> os = {1, 2, 3, 4, 5, 6, 7};

    c.orientations[0] = 0;
    for (int i = 1; i <= 6; i++) {
        c.orientations[i] = orie / pow3[6-i];
        or_sum += c.orientations[i];
        orie %= pow3[6-i];
    }
    c.orientations[7] = (3 - (or_sum%3)) % 3;

    c.pieces[0] = 0;    
    for (int i = 1; i <= 6; i++) {
        factoradic.push_back(perm / factorials[7-i]);
        perm %= factorials[7-i];
    }

    for (int i = 1; i < 7; i++) {
        int idx = 0;
        for (auto it: os) {
            if (idx == factoradic[i-1]) {
                c.pieces[i] = it;
                os.erase(it);
            }
            idx++;
        }
    }
    c.pieces[7] = *os.begin();

    return c; 
}

void make_graph(vector<pair<int, array<int, 9>>> &graph) {
    /*
     *  Graph is a vector of pairs
     *  First element of pair -> depth from solved state
     *  Second element of pair -> adjacency list
     *  Adjacency list contains list of hashes of all cubes one turn away from the current cube state
     *  Index of vector = hash of current cube state
     *
     *  initial depth = 20
     *  initial adj list = {-1 x9}
     *
     *
     */

    vector<int> compliment = {2, 1, 0, 5, 4, 3, 8, 7, 6};
    /* Stores the move compliment of a given move, from the cube header file
     *
     * for eg. move 0 = D, move 2 = D'
     * if state 1 becomes state 2 upon performing D, then state 2 becomes 1 upon performing D'
     * i.e compliment of D is D'
     * => compliment of 0 is 2
     * which is what this vector is for
     * helps us fill the graph quicker
     *
     */

    auto start_time = chrono::high_resolution_clock::now();
    if (!filesystem::exists("graph.bin")) {
        ofstream graph_file("graph.bin", ios::binary);
        graph[0].first = 0;
        int ctr = 0;
        queue<int> next;
        next.push(0);
        while (!next.empty()) {
            int cur = next.front();
            next.pop();
            if (ctr % 100000 == 0) cout << ctr << ":" << endl;
            for (int j = 0; j < 9; j++) {
                if(graph[cur].second[j] == -1) {
                    cube c = unhash(cur);
                    c.apply_move(j);
                    int adj = cube_hash(c);
                    graph[cur].second[j] = adj;

                    if(graph[adj].first == 20) {
                        graph[adj].first = graph[cur].first + 1;
                        next.push(adj);
                    }

                    graph[adj].second[compliment[j]] = cur;
                }
            }
            ctr++;
        }


        for (int i = 0; i < states; i++) {
            graph_file.write(reinterpret_cast<const char *>(&graph[i].first), sizeof(int));
            for (int j = 0; j < 9; j++) {
                graph_file.write(reinterpret_cast<const char *>(&graph[i].second[j]), sizeof(int));
            }

        }
    }

    else {
        ifstream graph_file("graph.bin", ios::binary);
        for (int i = 0; i < states; i++) {
            graph_file.read(reinterpret_cast<char *>(&(graph[i].first)), sizeof(int));

            for (int j = 0; j < 9; j++) {
                graph_file.read(reinterpret_cast<char *>(&(graph[i].second[j])), sizeof(int));
            }
        }
    }

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cout << "Graph construction complete in " << duration.count() << " milliseconds\n";
}



void solution(int hash, vector<pair<int, array<int, 9>>> &graph, string &sol_string) {
    /*
     * Uses a bit of a property of the way the graph was generated
     *
     * move 0 = F, so the 0th element of the adjacency list is (old cube + F)
     * similary, 1 = F2 and so on
     *
     * so we find the first element with a depth lower than the current cube, 
     *  and add the corresponding move to the solution recursively.
     *  (at max 12 recursions of O(1) operations so doesn't take long at all)
     *
     * if depth of cube = 0, cube is solved, we are done
     */

    uint8_t depth = graph[hash].first;
    if (depth == 0) return;
    else {
        for (int i = 0; i < 9; i++) { 
            if (graph[graph[hash].second[i]].first == depth - 1) {

                if (i == 0) sol_string = sol_string + (string) "F ";
                else if (i == 1) sol_string  = sol_string + (string) "F2 ";
                else if (i == 2) sol_string = sol_string + (string) "F' ";
                else if (i == 3) sol_string = sol_string + (string) "D ";
                else if (i == 4) sol_string = sol_string + (string) "D2 ";
                else if (i == 5) sol_string = sol_string + (string) "D' " ;
                else if (i == 6) sol_string = sol_string + (string) "R ";
                else if (i == 7) sol_string = sol_string + (string) "R2 ";
                else if (i == 8) sol_string = sol_string + (string) "R' ";

                solution(graph[hash].second[i], graph, sol_string);

                break;
            }
        }
    }
}


int main() {    
    
    // Initializing RNG to generate random cubes if necessary
    random_device rd;
    linear_congruential_engine<std::uint_fast32_t, 48271, 0, 2147483647> rng;
    rng.seed(rd());

    //Making graph
    vector<pair<int, array<int, 9>>> graph(states, { 20, {-1, -1, -1, -1, -1, -1, -1, -1, -1} });
    make_graph(graph);

    //cube test = random_cube(rng);
    //test.draw();
    //cout << cube_hash(test) << endl;
    //test = unhash(cube_hash(test));
    //test.draw();
    //
    // sample code for random cube ^

    while (true) {
        // loop to get solutions of cube given scramble

        cube test = solved[0];
        string scr;
        cout << "input a scramble:\n";
        fflush(stdin);
        getline(cin, scr);
        test.apply_scramble(scr);

        cout << "The scrambled cube is:\n";
        test.draw();
        string sol_string = "";
        int ort = test.find_orientation();
        test.rotate_to_wca();
        solution(cube_hash(test), graph, sol_string);
        str_rotate(sol_string, ort);
        cout << sol_string << endl;
    }

    return 0;
}
