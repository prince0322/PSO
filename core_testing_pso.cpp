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
void extract_data() {
  cout << "TODO: extract data quickly" << endl;
}


int main() {
  FILE *input_file = fopen("input.txt", "r");
  int argc, ans_width = 0, ans_height = 0;
  printf("Enter the no. of variable:\n");
  fscanf(input_file, "%d", &argc);
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
