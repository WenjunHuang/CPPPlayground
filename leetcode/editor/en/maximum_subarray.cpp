// Given an integer array nums, find the contiguous subarray (containing at
// least one number) which has the largest sum and return its sum.
//
//  A subarray is a contiguous part of an array.
//
//
//  Example 1:
//
//
// Input: nums = [-2,1,-3,4,-1,2,1,-5,4]
// Output: 6
// Explanation: [4,-1,2,1] has the largest sum = 6.
//
//
//  Example 2:
//
//
// Input: nums = [1]
// Output: 1
//
//
//  Example 3:
//
//
// Input: nums = [5,4,-1,7,8]
// Output: 23
//
//
//
//  Constraints:
//
//
//  1 <= nums.length <= 10⁵
//  -10⁴ <= nums[i] <= 10⁴
//
//
//
//  Follow up: If you have figured out the O(n) solution, try coding another
// solution using the divide and conquer approach, which is more subtle.
//  👍 16727 👎 782

#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
using namespace std;
// leetcode submit region begin(Prohibit modification and deletion)
class Solution {
 public:
  int maxSubArray(vector<int>& nums) {
    return MaxSubArray(nums, 0, nums.size() - 1);
  }

 private:
  int MaxSubArray(vector<int>& nums, int lower, int upper) {
    if (lower == upper)
      return nums[lower];

    int mid = (upper + lower) / 2;
    auto s1 = MaxSubArray(nums, lower, mid);
    auto s2 = MaxSubArray(nums, mid + 1, upper);
    auto s3 = CrossingSubArray(nums, lower, mid, upper);
    auto m = std::max({s1, s2, s3});
    return m;
  }

  int CrossingSubArray(vector<int>& nums, int lower, int mid, int upper) {
    auto s_left = numeric_limits<int>::lowest();
    auto sum = 0;
    for (int l = mid; l >= lower; l--) {
      sum += nums[l];
      s_left = max(s_left, sum);
    }

    auto s_right = numeric_limits<int>::lowest();
    sum = 0;
    for (int r = mid + 1; r <= upper; r++) {
      sum += nums[r];
      s_right = max(s_right, sum);
    }
    return s_left + s_right;
  }
};
// leetcode submit region end(Prohibit modification and deletion)

int main() {
  Solution s;
  vector<int> nums{-2, 1, -3, 4, -1, 2, 1, -5, 4};
  auto result = s.maxSubArray(nums);
  std::cout << result << std::endl;
}