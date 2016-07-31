#include <bits/stdc++.h>

#include "MaxRectsBinPack.cpp"

using namespace std;

#define tam_width first
#define test_time second

// stores total number of core.
int no_of_core;

// constraint tam width which will be used for limiting the tam width
// at the time of calculating the test time using 2D bin packing method.
int input_tam_width;

// stores the tam width and corresponding test-time for the given core
// in sorted (descending) according to area of the rectangle.
vector <pair <int, int> > rectangles[28];

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
    sort(rectangles[i].begin(), rectangles[i].end(), cmp_rectangle);
  }
}

// it returns max bit required to store the given number of elements.
inline int bit_required(int x) {
  for (int i = 0; i < 30; i++) {
    int mx = (1 << i) - 1;
    if (mx >= x) {
      return i;
    }
  }
  return -1;
}

// it returns positive random number lesser than limit value.
inline long long get_random(long long limit) {
  long long x = (rand() << 15) + rand();
  x %= limit;
  if (x < 0) {
    x += limit;
  }
  return x;
}

// Extracts data from the data table and store in the rectangles.
inline void extract_data() {
  FILE *read_csv = fopen("d695.csv", "r");
  fscanf(read_csv, "%d", &no_of_core);
  fscanf(read_csv, "%d", &input_tam_width);
  for (int w = 1; w <= input_tam_width; w++) {
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
inline int get_rectangle_idx(long long ps, int l, int r) {
  int res = 0;
  for (int i = l; i <= r; i++) {
    if ((1ll << i) & ps) {
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
inline int get_fitness(long long ps) {
  // This can be changed individually even for each rectangle packed.
  rbp :: MaxRectsBinPack::FreeRectChoiceHeuristic heuristic
      = rbp :: MaxRectsBinPack::RectBestShortSideFit;
  rbp :: MaxRectsBinPack bin;
  bin.Init(input_tam_width, 93700);
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
      fitness = max(fitness, packedRect.y + packedRect.height);
    } else {
      return -1;
    }
  }
  return fitness;
}

long long next_ps(long long current_ps) {
  // do some heuristic.
}

int main() {
  extract_data();
  sort_rectangles();
  //print_rectangles();
  long long initial_ps = get_random(get_limit());
  int fitness = get_fitness(initial_ps);
  printf("%d", fitness);
}

/**
int main() {
  FILE *read_csv = fopen("input.txt", "r");
  int argc, ans_width = 0, ans_height = 0;
  printf("Enter the no. of variable:\n");
  fscanf(read_csv, "%d", &argc);
  int arr[argc + 1];
  for(int i = 1; i <= argc; i++) {
    fscanf(fr, "%d", &arr[i]);
  }
  using namespace rbp;
  MaxRectsBinPack bin;
  int binWidth = arr[1];
  int binHeight = arr[2];
  printf("Initializing Soc Core to size %dx%d.\n", binWidth, binHeight);
  bin.Init(binWidth, binHeight);

  // Pack each rectangle (w_i, h_i)
  int ctr = 0;
  for(int i = 3; i < argc; i += 2) {
    ctr++;
    // Read next rectangle to pack.
    int rectWidth = arr[i];
    int rectHeight = arr[i + 1];
    //printf("Packing rectangle of size %dx%d: ", rectWidth, rectHeight);

    // Perform the packing.
    MaxRectsBinPack::FreeRectChoiceHeuristic heuristic = MaxRectsBinPack::RectBestShortSideFit; // This can be changed individually even for each rectangle packed.
    Rect packedRect = bin.Insert(rectWidth, rectHeight, heuristic);

    // Test success or failure.
    if (packedRect.height > 0) {
      ans_width = max(ans_width, packedRect.x + packedRect.width);
      ans_height = max(ans_height, packedRect.y + packedRect.height);
      //printf("Packed to (x,y)=(%d,%d), (w,h)=(%d,%d). Free space left: %.2f%%\n", packedRect.x, packedRect.y, packedRect.width, packedRect.height, 100.f - bin.Occupancy()*100.f);
      printf(" %d. packed (x, y)=(%d, %d)\n", ctr,  packedRect.y, packedRect.x);
    } else
      printf("NOT packed all rectangle inside the given block:\n");
  }
  printf("%d X %d size of the rectangle required to pack all soc:\n", ans_width, ans_height);
}
*/
