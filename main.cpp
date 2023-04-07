#include <bits/stdc++.h>
//too many headers to keep track of

using namespace std;


//PRNG seeded randomly used for generation of random cubes if required
random_device rd;
linear_congruential_engine<std::uint_fast32_t, 48271, 0, 2147483647> rng;
void init_rng() {
    rng.seed(rd());
}


typedef struct cube {
    std::array<uint8_t, 8> pieces;
    std::array<uint8_t, 8> orientations;

/*
 * Positions for the cubies: (U = top, D = bottom, R = right, L = left, F = front, B = back)
 * eg. UFR refers to upper front right corner = the white red green corner in WCA scramble orientation
 * order in array: <UBL, UBR, UFR, UFL, DFL, DFR, DBR, DBL>
 *
 * pieces => 0, 1, 2, 3, 4, 5, 6, 7
 * 0 = WBO, 1 = WRB, 2 = WGR, 3 = WOG, 4 = YGO, 5 = YRG, 6 = YBR, 7 = YOB
 * 
 * orientations (0, 1, 2) : 
 * compare the sticker on top or bottom with how it has been rotated wrt yellow/white.
 * no rotation = 0
 * clockwise = 1
 * anticlockwise = 2
 *
 * A solved cube would have pieces = {0, 1, 2, 3, 4, 5, 6, 7}, rotations = {0, 0, 0, 0, 0, 0, 0, 0}
 *
 *
 */
} cube;

inline bool operator<(const cube &lhs, const cube &rhs) {
        if (lhs.pieces != rhs.pieces) return lhs.pieces < rhs.pieces;
        else return lhs.orientations < rhs.orientations;
};


inline bool operator==(const cube& lhs, const cube& rhs) {
    return (lhs.pieces == rhs.pieces) && (lhs.orientations == rhs.orientations);
    };

//comparison operators for cubes


const cube solved[] = { //6 * 4 = 24 orientations to keep cube = 24 solved states, maybe generalize later
    { {0, 1, 2, 3, 4, 5, 6, 7}, {0, 0, 0, 0, 0, 0, 0, 0} }, //solved, white on top, green in front;
    { {3, 0, 1, 2, 5, 6, 7, 4}, {0, 0, 0, 0, 0, 0, 0, 0} }, //solved, W top, R front;
    { {2, 3, 0, 1, 6, 7, 4, 5}, {0, 0, 0, 0, 0, 0, 0, 0} }, //W top, B front;
    { {1, 2, 3, 0, 7, 4, 5, 6}, {0, 0, 0, 0, 0, 0, 0, 0} }, //W top, O front;
    { {6, 7, 4, 5, 2, 3, 0, 1}, {0, 0, 0, 0, 0, 0, 0, 0} }, //Y top, G front; 
    { {5, 6, 7, 4, 3, 0, 1, 2}, {0, 0, 0, 0, 0, 0, 0, 0} }, //Y top, O front;
    { {4, 5, 6, 7, 0, 1, 2, 3}, {0, 0, 0, 0, 0, 0, 0, 0} }, //Y top, B front;
    { {7, 4, 5, 6, 1, 2, 3, 0}, {0, 0, 0, 0, 0, 0, 0, 0} }, //Y top, R front;
    { {6, 5, 2, 1, 0, 3, 4, 7}, {2, 1, 2, 1, 2, 1, 2, 1} }, //R top, W front;
    { {1, 6, 5, 2, 3, 4, 7, 0}, {1, 2, 1, 2, 1, 2, 1, 2} }, //R top, G front;
    { {2, 1, 6, 5, 4, 7, 0, 3}, {2, 1, 2, 1, 2, 1, 2, 1} }, //R top, Y front;
    { {5, 2, 1, 6, 7, 0, 3, 4}, {1, 2, 1, 2, 1, 2, 1, 2} }, //R top, B front;
    { {3, 4, 7, 0, 1, 6, 5, 2}, {1, 2, 1, 2, 1, 2, 1, 2} }, //O top, B front;
    { {0, 3, 4, 7, 6, 5, 2, 1}, {2, 1, 2, 1, 2, 1, 2, 1} }, //O top, Y front;
    { {7, 0, 3, 4, 5, 2, 1, 6}, {1, 2, 1, 2, 1, 2, 1, 2} }, //O top, G front;
    { {4, 7, 0, 3, 2, 1, 6, 5}, {2, 1, 2, 1, 2, 1, 2, 1} }, //O top, W front;
    { {1, 0, 7, 6, 5, 4, 3, 2}, {2, 1, 2, 1, 2, 1, 2, 1} }, //B top, Y front;
    { {6, 1, 0, 7, 4, 3, 2, 5}, {1, 2, 1, 2, 1, 2, 1, 2} }, //B top, O front;
    { {7, 6, 1, 0, 3, 2, 5, 4}, {2, 1, 2, 1, 2, 1, 2, 1} }, //B top, W front;
    { {0, 7, 6, 1, 2, 5, 4, 3}, {1, 2, 1, 2, 1, 2, 1, 2} }, //B top, R front;
    { {5, 4, 3, 2, 1, 0, 7, 6}, {2, 1, 2, 1, 2, 1, 2, 1} }, //G top, W front;
    { {4, 3, 2, 5, 6, 1, 0, 7}, {1, 2, 1, 2, 1, 2, 1, 2} }, //G top, R front;
    { {3, 2, 5, 4, 7, 6, 1, 0}, {2, 1, 2, 1, 2, 1, 2, 1} }, //G top, Y front;
    { {2, 5, 4, 3, 0, 7, 6, 1}, {1, 2, 1, 2, 1, 2, 1, 2} } //G top, O front;
};


void draw_cube (cube cube) { //displays the map/net of given cube
// 0 = WBO, 1 = WRB, 2 = WGR, 3 = WOG, 4 = YGO, 5 = YRG, 6 = YBR, 7 = YOB
    array<array<char, 3>, 8> colours = {{
        {'W', 'B', 'O'},
        {'W', 'R', 'B'},
        {'W', 'G', 'R'},
        {'W', 'O', 'G'},
        {'Y', 'G', 'O'},
        {'Y', 'R', 'G'},
        {'Y', 'B', 'R'},
        {'Y', 'O', 'B'}
    }};

    printf("      ┌─────┐\n");
    printf("      │ %c %c │\n", colours[cube.pieces[0]][cube.orientations[0]], colours[cube.pieces[1]][cube.orientations[1]]);
    printf("      │ %c %c │\n", colours[cube.pieces[3]][cube.orientations[3]], colours[cube.pieces[2]][cube.orientations[2]]);
    printf("┌─────┼─────┼─────┬─────┐\n");

    printf("│ %c %c │ %c %c │ %c %c │ %c %c │\n", 
            colours[cube.pieces[0]][(cube.orientations[0] + 2) % 3], 
            colours[cube.pieces[3]][(cube.orientations[3] + 1) % 3], 
            colours[cube.pieces[3]][(cube.orientations[3] + 2) % 3],
            colours[cube.pieces[2]][(cube.orientations[2] + 1) % 3],
            colours[cube.pieces[2]][(cube.orientations[2] + 2) % 3],
            colours[cube.pieces[1]][(cube.orientations[1] + 1) % 3],
            colours[cube.pieces[1]][(cube.orientations[1] + 2) % 3],
            colours[cube.pieces[0]][(cube.orientations[0] + 1) % 3]);

    printf("│ %c %c │ %c %c │ %c %c │ %c %c │\n", 
            colours[cube.pieces[7]][(cube.orientations[7] + 1) % 3], 
            colours[cube.pieces[4]][(cube.orientations[4] + 2) % 3], 
            colours[cube.pieces[4]][(cube.orientations[4] + 1) % 3],
            colours[cube.pieces[5]][(cube.orientations[5] + 2) % 3],
            colours[cube.pieces[5]][(cube.orientations[5] + 1) % 3],
            colours[cube.pieces[6]][(cube.orientations[6] + 2) % 3],
            colours[cube.pieces[6]][(cube.orientations[6] + 1) % 3],
            colours[cube.pieces[7]][(cube.orientations[7] + 2) % 3]);

    printf("└─────┼─────┼─────┴─────┘\n");
    printf("      │ %c %c │\n", colours[cube.pieces[4]][cube.orientations[4]], colours[cube.pieces[5]][cube.orientations[5]]);
    printf("      │ %c %c │\n", colours[cube.pieces[7]][cube.orientations[7]], colours[cube.pieces[6]][cube.orientations[6]]);
    printf("      └─────┘\n");
}

void apply_move(cube &c, int move) { //applies a face turn move to the cube
    std::function<void(int, int, int, int)> rot2 = [&c](int p1, int p2, int p3, int p4) {
        int temp = c.pieces[p4];
        c.pieces[p4] = c.pieces[p2];
        c.pieces[p2] = temp;
        temp = c.pieces[p3];
        c.pieces[p3] = c.pieces[p1];
        c.pieces[p1] = temp;
        temp = c.orientations[p4];
        c.orientations[p4] = c.orientations[p2];
        c.orientations[p2] = temp;
        temp = c.orientations[p3];
        c.orientations[p3] = c.orientations[p1];
        c.orientations[p1] = temp;
    };

    std::function<void(int, int, int, int)> rot1 = [&c](int p1, int p2, int p3, int p4) {
        //for selecting pieces, select p1 towards rotating side, p2 against direction of rotation and go on
        int temp = c.pieces[p4];
        c.pieces[p4] = c.pieces[p1];
        c.pieces[p1] = c.pieces[p2];
        c.pieces[p2] = c.pieces[p3];
        c.pieces[p3] = temp;
        temp = c.orientations[p4];
        c.orientations[p4] = (c.orientations[p1] + 2) % 3;
        c.orientations[p1] = (c.orientations[p2] + 1) % 3;
        c.orientations[p2] = (c.orientations[p3] + 2) % 3;
        c.orientations[p3] = (temp + 1) % 3;
    };

    std::function<void(int, int, int, int)> rot_ud = [&c](int p1, int p2, int p3, int p4) {
        int temp = c.pieces[p4];
        c.pieces[p4] = c.pieces[p1];
        c.pieces[p1] = c.pieces[p2];
        c.pieces[p2] = c.pieces[p3];
        c.pieces[p3] = temp;
        temp = c.orientations[p4];
        c.orientations[p4] = c.orientations[p1];
        c.orientations[p1] = c.orientations[p2];
        c.orientations[p2] = c.orientations[p3];
        c.orientations[p3] = temp;
    };

// 0 = F, 1 = F2, 2 = F'   
    if (move == 0) { rot1(2, 3, 4, 5); return; }
    if (move == 1) { rot2(3, 2, 5, 4); return; }
    if (move == 2) { rot1(4, 3, 2, 5); return; }

// 3 = D, 4 = D2, 5 = D'
    if (move == 3) { rot_ud(6, 5, 4, 7); return; }
    if (move == 4) { rot2(4, 5, 6, 7); return; }
    if (move == 5) { rot_ud(4, 5, 6, 7); return; }

// 6 = R, 7 = R2, 8 = R'
    if (move == 6) { rot1(1, 2, 5, 6); return; }
    if (move == 7) { rot2(1, 2, 5, 6); return; }
    if (move == 8) { rot1(5, 2, 1, 6); return;}

// 9 = L, 10 = L2, 11 = L'
    if (move == 9) { rot1(3, 0, 7, 4); return; }
    if (move == 10) { rot2(3, 0, 7, 4); return; }
    if (move == 11) { rot1(7, 0, 3, 4); return; }

// 12 = B, 13 = B2, 14 = B'
    if (move == 12) { rot1(0, 1, 6, 7); return; }
    if (move == 13) { rot2(6, 1, 0, 7); return; }
    if (move == 14) { rot1(6, 1, 0, 7); return; }

// 15 = U, 16 = U2, 17 = U'
    if (move == 15) { rot_ud(2, 1, 0, 3); return; }
    if (move == 16) { rot2(0, 1, 2, 3); return; }
    if (move == 17) { rot_ud(0, 1, 2, 3); return; }

}

void apply_rotation(cube &c, int rotation) { //applies rotation to the cube
    
//z = 0
    if (rotation == 0) {
        int temp = c.pieces[2];
        c.pieces[2] = c.pieces[3];
        c.pieces[3] = c.pieces[4];
        c.pieces[4] = c.pieces[5];
        c.pieces[5] = temp;

        temp = c.orientations[2];
        c.orientations[2] = (c.orientations[3] + 1)%3;
        c.orientations[3] = (c.orientations[4] + 2)%3;
        c.orientations[4] = (c.orientations[5] + 1)%3;
        c.orientations[5] = (temp + 2)%3;

        temp = c.pieces[7];
        c.pieces[7] = c.pieces[6];
        c.pieces[6] = c.pieces[1];
        c.pieces[1] = c.pieces[0];
        c.pieces[0] = temp;

        temp = c.orientations[7];
        c.orientations[7] = (c.orientations[6] + 2)%3;
        c.orientations[6] = (c.orientations[1] + 1)%3;
        c.orientations[1] = (c.orientations[0] + 2)%3;
        c.orientations[0] = (temp + 1)%3;

        return;
    }

// 1 = z2, 2 = z'
    if (rotation == 1) for (int i = 0; i < 2; i++) apply_rotation(c, 0);
    if (rotation == 2) for (int i = 0; i < 3; i++) apply_rotation(c, 0);

// 3 = x
    if (rotation == 3) {
        int temp = c.pieces[7];
        c.pieces[7] = c.pieces[4];
        c.pieces[4] = c.pieces[3];
        c.pieces[3] = c.pieces[0];
        c.pieces[0] = temp;

        temp = c.orientations[7];
        c.orientations[7] = (c.orientations[4] + 1)%3;
        c.orientations[4] = (c.orientations[3] + 2)%3;
        c.orientations[3] = (c.orientations[0] + 1)%3;
        c.orientations[0] = (temp + 2)%3;

        temp = c.pieces[6];
        c.pieces[6] = c.pieces[5];
        c.pieces[5] = c.pieces[2];
        c.pieces[2] = c.pieces[1];
        c.pieces[1] = temp;

        temp = c.orientations[6];
        c.orientations[6] = (c.orientations[5] + 2)%3;
        c.orientations[5] = (c.orientations[2] + 1)%3;
        c.orientations[2] = (c.orientations[1] + 2)%3;
        c.orientations[1] = (temp + 1)%3;

        return;
    }

// 4 = x2, 5 = x'
    if (rotation == 4) for (int i = 0; i < 2; i++) apply_rotation(c, 3);
    if (rotation == 5) for (int i = 0; i < 3; i++) apply_rotation(c, 3);

// 6 = y
    if (rotation == 6) {
        int temp = c.pieces[3];
        c.pieces[3] = c.pieces[2];
        c.pieces[2] = c.pieces[1];
        c.pieces[1] = c.pieces[0];
        c.pieces[0] = temp;

        temp = c.orientations[3];
        c.orientations[3] = c.orientations[2];
        c.orientations[2] = c.orientations[1];
        c.orientations[1] = c.orientations[0];
        c.orientations[0] = temp;

        temp = c.pieces[4];
        c.pieces[4] = c.pieces[5];
        c.pieces[5] = c.pieces[6];
        c.pieces[6] = c.pieces[7];
        c.pieces[7] = temp;

        temp = c.orientations[4];
        c.orientations[4] = c.orientations[5];
        c.orientations[5] = c.orientations[6];
        c.orientations[6] = c.orientations[7];
        c.orientations[7] = temp;

        return;
    }

// 7 = y2, 8 = y'
    if (rotation == 7) for (int i = 0; i < 2; i++) apply_rotation(c, 6);
    if (rotation == 8) for (int i = 0; i < 3; i++) apply_rotation(c, 6);
}



void scrambler(cube &c, string scramble) {// reads scramble and applies moves to the cube
    for (int i = 0; i < scramble.size(); i++) {
        int move = -1;
        int rot = -1;
        if (scramble[i] == 'F') move = 0;
        if (scramble[i] == 'D') move = 3;
        if (scramble[i] == 'R') move = 6;
        if (scramble[i] == 'L') move = 9;
        if (scramble[i] == 'B') move = 12;
        if (scramble[i] == 'U') move = 15;
        if (scramble[i] == 'z') rot = 0;
        if (scramble[i] == 'x') rot = 3;
        if (scramble[i] == 'y') rot = 6;

        if (scramble[i+1] == '2') {
            if (move >= 0) move += 1;
            else rot += 1;
            }
        if (scramble[i+1] == '\'') { 
            if (move >= 0) move += 2;
            else rot += 2; 
            }

        if (move >= 0) apply_move(c, move);
        if (rot >= 0) apply_rotation(c, rot);
    }
    return;
}

void make_graph(vector<tuple<cube, array<int, 9>, uint8_t>> &graph, map<cube, int> &cube_indices) {
/*
 * Makes the cube graph.
 * First entry of tuple -> the cube state
 * Second entry of tuple -> adjacency list (all cube states accessible by a single turn)
 * Third entry of tuple -> depth from solved state
 *
 * The states are generated in order of depth from solved state, 
 * so iterating down the array is equivalent to BFS starting from solved cube.
 *
 * in my experience takes about 9 minutes on a shitty laptop, 2013 i3
 * Takes about 195 MB of memory
 *
 */
    auto start_time = chrono::high_resolution_clock::now();
    cube start = solved[0];
    int counter = 0;
    array<int, 9> initial;
    initial.fill(-1);
    graph.emplace_back(make_tuple(start, initial, 0));
    cube_indices[start] = 0;
    for (int i = 0; i < 3674160; i++) {
        for (int j = 0; j < 9; j++) {
            cube c = get<0>(graph[i]);
            apply_move(c, j);
            if (cube_indices.find(c) == cube_indices.end()) {
                graph.emplace_back(make_tuple(c, initial, get<2>(graph[i]) + 1));
                cube_indices[c] = ++counter;
                if (!(counter%10000)) cout << "Node " << counter << " has been initialized at depth "
                    << +get<2>(graph[counter]) << " \n";
            }
            get<1>(graph[i])[j] = cube_indices[c];
        }
        if (!((i+1)%10000)) cout << "Node " << i+1 << " has been edgified\n";
    }
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(end_time - start_time);
    cout << "Graph construction complete in " << duration.count() << " seconds\n";
    return;
}

void solution(cube &c, string &sol_string, vector<tuple<cube, array<int, 9>, uint8_t>> &graph, map<cube, int> &indices) {
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

    uint8_t depth = get<2>(graph[indices[c]]);
    if (depth == 0) return;
    else {
        for (int i = 0; i < 9; i++) { 
            if (get<2>(graph[get<1>(graph[indices[c]])[i]]) == depth - 1) {
                //draw_cube(get<0>(graph[get<1>(graph[indices[c]])[i]]));
                if (i == 0) sol_string = sol_string + (string) "F ";
                else if (i == 1) sol_string  = sol_string + (string) "F2 ";
                else if (i == 2) sol_string = sol_string + (string) "F' ";
                else if (i == 3) sol_string = sol_string + (string) "D ";
                else if (i == 4) sol_string = sol_string + (string) "D2 ";
                else if (i == 5) sol_string = sol_string + (string) "D' " ;
                else if (i == 6) sol_string = sol_string + (string) "R ";
                else if (i == 7) sol_string = sol_string + (string) "R2 ";
                else if (i == 8) sol_string = sol_string + (string) "R' ";
                solution(get<0>(graph[get<1>(graph[indices[c]])[i]]), sol_string, graph, indices);
                break;
            }
        }
    }
}

int find_orientation(cube &c) {
    int orientation;
    int i;
    for (i = 0; c.pieces[i] != 0; i++);
    orientation = (3 * i) + c.orientations[i];
    return orientation;
}

void rotate_to_pos(cube &c, int cur_orientation) { //rotates the cube so that white top, green front
    switch(cur_orientation) {
        case 0: break;
        case 1: apply_rotation(c, 0); apply_rotation(c, 8); break;
        case 2: apply_rotation(c, 3); apply_rotation(c, 6); break;
        case 3: apply_rotation(c, 8); break;
        case 4: apply_rotation(c, 3); apply_rotation(c, 7); break;
        case 5: apply_rotation(c, 2); break;
        case 6: apply_rotation(c, 7); break;
        case 7: apply_rotation(c, 2); apply_rotation(c, 6); break;
        case 8: apply_rotation(c, 2); apply_rotation(c, 5); break;
        case 9: apply_rotation(c, 6); break;
        case 10: apply_rotation(c, 5); break;
        case 11: apply_rotation(c, 2); apply_rotation(c, 4); break;
        case 12: apply_rotation(c, 4); break;
        case 13: apply_rotation(c, 0); apply_rotation(c, 6); break;
        case 14: apply_rotation(c, 0); apply_rotation(c, 5); break;
        case 15: apply_rotation(c, 1); apply_rotation(c, 6); break;
        case 16: apply_rotation(c, 5); apply_rotation(c, 7); break;
        case 17: apply_rotation(c, 2); apply_rotation(c, 7); break;
        case 18: apply_rotation(c, 1); break;
        case 19: apply_rotation(c, 2); apply_rotation(c, 8); break;
        case 20: apply_rotation(c, 3); apply_rotation(c, 8); break;
        case 21: apply_rotation(c, 4); apply_rotation(c, 6); break;
        case 22: apply_rotation(c, 3); break;
        case 23: apply_rotation(c, 0); break;
    }
}

void replaceAll(string& str, const string& from, const string& to) {
// function replaces all instances of a substring to another
// code shamelessly stolen from stackoverflow

    if(from.empty()) return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

void str_rotate(string &sol, int orient) {
/* Replaces the R, F, D moves from the green front white top solution into another 3-gen move set 
 * based on the original orientation of the cube
 *
 * Can't just directly replace since for eg you need RFD -> FRU
 * if you replace R with F first you get 2 identical Fs, and similar case if F is replaced with R first
 * Hence we use the strings 5, 6, 7 as placeholders
 * (originally I used 1, 2 and 3 but that messed with all the double moves like F2)
 * (0 IQ moment)
 *
 */

    replaceAll(sol, "R", "5");
    replaceAll(sol, "F", "6");
    replaceAll(sol, "D", "7");

    function<void(string, string, string)> change = [&sol] (string r, string f, string d) {
        replaceAll(sol, "5", r);
        replaceAll(sol, "6", f);
        replaceAll(sol, "7", d);
    };

    if (orient == 0) change("R", "F", "D");
    else if (orient == 1) change("F", "D", "R");
    else if (orient == 2) change("D", "R", "F");
    else if (orient == 3) change("F", "L", "D");
    else if (orient == 4) change("L", "D", "F");
    else if (orient == 5) change("D", "F", "L");
    else if (orient == 6) change("L", "B", "D");
    else if (orient == 7) change("B", "D", "L");
    else if (orient == 8) change("D", "L", "B");
    else if (orient == 9) change("B", "R", "D");
    else if (orient == 10) change("R", "D", "B");
    else if (orient == 11) change("D", "B", "R");
    else if (orient == 12) change("R", "B", "U");
    else if (orient == 13) change("B", "U", "R");
    else if (orient == 14) change("U", "R", "B");
    else if (orient == 15) change("B", "L", "U");
    else if (orient == 16) change("L", "U", "B");
    else if (orient == 17) change("U", "B", "L");
    else if (orient == 18) change("L", "F", "U");
    else if (orient == 19) change("F", "U", "L");
    else if (orient == 20) change("U", "L", "F");
    else if (orient == 21) change("F", "R", "U");
    else if (orient == 22) change("R", "U", "F");
    else if (orient == 23) change("U", "F", "R");
}

cube random_cube(void) {
// generates a random cube by randomly permuting the 8 cubies, and randomly orienting 7
// The orientation of the last cubie depends on the orientation of the other 7

    cube c;
    c.pieces = {0, 1, 2, 3, 4, 5, 6, 7};
    c.orientations = {0, 0, 0, 0, 0, 0, 0, 0};
    shuffle(c.pieces.begin(), c.pieces.end(), rng);
    int o_sum = 0;
    uniform_int_distribution<uint8_t> int_gen(0, 2);
    for (int i = 0; i < 7; i++) {
        c.orientations[i] = int_gen(rng);
        o_sum += c.orientations[i];
    }
    
    c.orientations[7] = (3 - (o_sum%3)) % 3;
    return c;
}

int main() {    
    cube test = solved[0];
    draw_cube(test);
/*
    init_rng();

    for (int i = 0; i < 50; i++) {
        test = random_cube();
        cout << find_orientation(test) << endl;
        rotate_to_pos(test, find_orientation(test));
        draw_cube(test);
    }
*/
    
    int inpt = 0;
    vector<tuple<cube, array<int, 9>, uint8_t>> graph;
    graph.reserve(3674160);
    map<cube, int> cube_indices;
    make_graph(graph, cube_indices);
    cout << "Size of graph is " << graph.size() *  (37 + sizeof(cube))<< " bytes\n";

    while (true) {
    test = solved[0];
    string scr;
    cout << "input a scramble:\n";
    fflush(stdin);
    getline(cin, scr);
    scrambler(test, scr);

    cout << "The scrambled cube is:\n";
    draw_cube(test);
    string sol_string = "";
    int ort = find_orientation(test);
    rotate_to_pos(test, ort);
    solution(test, sol_string, graph, cube_indices);
    str_rotate(sol_string, ort);
    cout << sol_string << endl;
    }
    
    return 0;
}
