#ifndef _COMMON_HPP_
#define _COMMON_HPP_

using Graph = std::vector<std::vector<double>>;

struct Solution {
    std::vector<int> sequence; // size = n
    std::vector<int> delimiters; // size = m-1
};

// Generate a random solution
Solution random_solution(int n, int m) {
    Solution s = { std::vector<int>(n-1), std::vector<int>(m-1) };

    // Generate random sequence
    for (int i = 0; i < int(s.sequence.size()); ++i)
        s.sequence[i] = i+1;
    std::random_shuffle(s.sequence.begin(), s.sequence.end());

    // Set delimiters randomly
    for (int i = 0; i < int(s.delimiters.size()); ++i)
        s.delimiters[i] = rand() % n;
    std::sort(s.delimiters.begin(), s.delimiters.end());

    return s;
}

// Swap two nodes in the sequence
void mutation_swap(Solution &s) {
    int i = rand() % s.sequence.size();
    int j = i;
    while (j == i)
        j = rand() % s.sequence.size();
    std::swap(s.sequence[i], s.sequence[j]);
}

// Helper
bool move_delimiter_left(Solution &s, int i) {
    if (s.delimiters[i] > 0
            && (i == 0 || s.delimiters[i-1] < s.delimiters[i])) {
        --s.delimiters[i];
        return true;
    }
    return false;
}

// Helper
bool move_delimiter_right(Solution &s, int i) {
    if (s.delimiters[i] < int(s.sequence.size())
            && (i == int(s.delimiters.size())-1 || s.delimiters[i+1] > s.delimiters[i])) {
        ++s.delimiters[i];
        return true;
    }
    return false;
}

// Mutates the solution by moving a random delimiter right or left
void mutation_delimiter(Solution &s) {
    bool flipped = false; // sanity check
    int start = rand() % s.delimiters.size();
    int i = start;
    int direction = rand() % 2 == 0 ? -1 : 1;
    while (true) {
        if (direction == -1) {
            if (move_delimiter_left(s, i)) return;
            if (move_delimiter_right(s, i)) return;
        } else {
            if (move_delimiter_right(s, i)) return;
            if (move_delimiter_left(s, i)) return;
        }
        i += direction;
        if (i < 0 || i >= int(s.delimiters.size())) {
            assert(!flipped); // can not double flip
            i = start;
            direction *= -1;
            flipped = true;
        }
    }
}

// Print solution for debugging
void print_solution(Solution &s) {
    printf("Vehicle 0: ");
    for (int j = 0; j < s.delimiters[0]; ++j)
        printf("%d ", s.sequence[j]);
    printf("\n");

    for (int i = 1; i < int(s.delimiters.size()); ++i) {
        printf("Vehicle %d: ", i);
        for (int j = s.delimiters[i-1]; j < s.delimiters[i]; ++j)
            printf("%d ", s.sequence[j]);
        printf("\n");
    }

    printf("Vehicle %d: ", int(s.delimiters.size()));
    for (int j = s.delimiters.back(); j < int(s.sequence.size()); ++j)
        printf("%d ", s.sequence[j]);
    printf("\n\n");
}

#endif