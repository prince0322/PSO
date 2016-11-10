#include <bits/stdc++.h>

#include "MaxRectsBinPack.cpp"

using namespace std;

#define tam_width first
#define test_time second

typedef bitset <60> Particle;
typedef vector <Particle> Population;

// changes in each and every iteration.
Particle pBest;

// best of all pBest particle.
Particle gBest;

// stores total number of core.
int no_of_core;

// constraint tam width which will be used for limiting the tam width
// at the time of calculating the test time using 2D bin packing method.
int input_tam_width;

// stores the tam width and corresponding test-time for the given core
// in sorted (descending) according to area of the rectangle.
vector <pair <int, int> > rectangles[10];

// comparator function for comparing the two rectangles on the basis
// area (product of tam-width and test-time) in descending order.
inline bool cmp_rectangle(
    const pair <int, int> &a,
    const pair <int, int> &b) {
  int area_a = a.tam_width * a.test_time;
  int area_b = b.tam_width * b.test_time;
  return area_a > area_b;
}

// It sorts rectangles of all the cores according to comparator function.
inline void sort_rectangles() {
  for (int i = 0; i < no_of_core; i++) {
    sort(rectangles[i].begin(), rectangles[i].begin() + input_tam_width, cmp_rectangle);
  }
}

// it returns max bit required to store the given number of elements.
inline int bit_required(int x) {
  for (int i = 0; i < 30; i++) {
    int mx = (1 << i);
    if (mx >= x) {
      return i;
    }
  }
  return -1;
}

// it returns non - negative random number lesser than limit value.
inline long long get_random(long long limit) {
  long long x = (rand() << 15) + rand();
  x %= limit;
  if (x < 0) {
    x += limit;
  }
  return x;
}

// it generates one random particles.
inline Particle get_random_particles() {
  Particle a;
  a.set(); a.flip();
  int sz = a.size();
  for (int i = 0; i < sz; i++) {
    if (get_random(2) == 1) {
      a.set(i, 1);
    }
  }
  return a;
}

// Extracts data from the data table and store in the rectangles.
inline void extract_data() {
  FILE *read_csv = fopen("d695.csv", "r");
  fscanf(read_csv, "%d", &no_of_core);
  fscanf(read_csv, "%d", &input_tam_width);
  //input_tam_width = 16;
  for (int w = 1; w <= 64; w++) {
    int temp_tam_width = 0, temp_test_time = 0;
    fscanf(read_csv, "%d", &temp_tam_width);
    for (int core = 0; core < no_of_core; core++) {
      fscanf(read_csv, "%d", &temp_test_time);
      rectangles[core].push_back(make_pair(temp_tam_width, temp_test_time));
    }
    // ignore total test_time of core.
    fscanf(read_csv, "%d", &temp_test_time);
  }
}

// helper function for debugging.
inline void print_rectangles() {
  for (int core = 0; core < no_of_core; core++) {
    printf("Rectangle of core_no %d.\n", core + 1);
    for (int i = 0; i < rectangles[core].size(); i++) {
      pair <int, int> rectangle = rectangles[core][i];
      printf("TAM width : %d Test Time : %d\n", rectangle.tam_width, rectangle.test_time);
    }
    putchar('\n');
  }
}

// get rectangle from bit of particle generation.
inline int get_rectangle_idx(Particle ps, int l, int r) {
  int res = 0;
  for (int i = l; i <= r; i++) {
    if (ps.test(i)) {
      res |= (1 << (i - l));
    }
  }
  return res;
}

// return the limit value of the particle generation.
inline long long get_limit() {
  int total_bit = no_of_core * bit_required(input_tam_width);
  long long x = (1ll << total_bit);
  return x - 1ll;
}

// It returns the fitness value of the given generation.
inline int get_fitness(Particle ps, bool print = false) {
  // This can be changed individually even for each rectangle packed.
  rbp :: MaxRectsBinPack::FreeRectChoiceHeuristic heuristic
      = rbp :: MaxRectsBinPack::RectBestShortSideFit;
  rbp :: MaxRectsBinPack bin;
  bin.Init(input_tam_width, 500894);
  int fitness = 0;
  int required_bit = bit_required(input_tam_width);
  // packing is done individually
  for (int i = 0; i < no_of_core; i++) {
    int start_bit = required_bit * i;
    int rect_idx = get_rectangle_idx(ps, start_bit, start_bit + required_bit - 1);
    pair <int, int> rectangle = rectangles[i][rect_idx];
    rbp :: Rect packedRect
        = bin.Insert(rectangle.tam_width, rectangle.test_time, heuristic);
    // test success or failure.
    if (packedRect.height > 0) {
      if (print) {
        cout << "Core No : " << i + 1 << " ";
        cout << "tam_width = " << rectangle.tam_width << " ";
        cout << "test time = " << rectangle.test_time << " should put at" << endl;
        cout << "y = " << packedRect.x << " x = " << packedRect.y << endl;
      }
      fitness = max(fitness, packedRect.y + packedRect.height);
    } else {
      return -1;
    }
  }
  return fitness;
}

inline Particle better(const Particle &a, const Particle &b) {
  int fitness_a = get_fitness(a);
  int fitness_b = get_fitness(b);
  if (fitness_a <= fitness_b) {
    return a;
  } else {
    return b;
  }
}

// return new particle of cur induced by best particle.
Particle get_induced_particle(Particle cur) {
  int sz = cur.size();
  int l = get_random(sz);
  int r = get_random(sz);
  if (l > r) swap(l, r);
  for (int i = l; i <= r; i++) {
    int z = get_random(2);
    if (z == 1) {
      cur.set(i, pBest.test(i));
    } else {
      cur.set(i, gBest.test(i));
    }
  }
  return cur;
}

// return new particle of cur induced by best particle.
Particle moov_just_opposite(Particle cur) {
  int sz = cur.size();
  int l = get_random(sz);
  int r = get_random(sz);
  if (l > r) swap(l, r);
  for (int i = l; i <= r; i++) {
    if (cur.test(i)) {
      cur.set(i, 0);
    } else {
      cur.set(i, 1);
    }
  }
  return cur;
}


Population next_population(Population cur) {
  Population nxt = cur;
  for (int i = 0; i < cur.size(); i++) {
    if (i == 0) {
      pBest = cur[i];
    } else {
      pBest = better(pBest, cur[i]);
    }
  }
  gBest = better(gBest, pBest);
  for (int i = 0; i < cur.size(); i++) {
    nxt[i] = get_induced_particle(nxt[i]);
    nxt[i] = moov_just_opposite(nxt[i]);
  }
  return nxt;
}


int main() {
  extract_data();
  sort_rectangles();
  //print_rectangles();
  Population cur;
  int iteration = 1500, population_size = 150;
  for (int i = 0; i < population_size; i++) {
    cur.push_back(get_random_particles());
  }
  for (int iter = 1; iter <= iteration; iter++) {
    Population nxt = next_population(cur);
    cout << "After Iteration " << iter << " : ";
    cout << get_fitness(pBest) << " " << get_fitness(gBest) << endl;
    cout << pBest << endl;
    cout << gBest << endl;
    cur = nxt;
  }
  printf("%d\n", get_fitness(gBest, true));
}
