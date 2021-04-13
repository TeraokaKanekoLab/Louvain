#include <algorithm>
#include <chrono>
#include <climits>
#include <deque>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

void print_vector(vector<int> nums)
{
    cout << "[" << nums[0];
    for (int i = 1; i < nums.size(); ++i) {
        cout << ", " << nums[i];
    }
    cout << "]" << endl;
}

void print_set(unordered_set<int>& nums)
{
    cout << "set:";
    for (auto num : nums) {
        cout << " " << num;
    }
    cout << endl;
}