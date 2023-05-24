/*
  Given an array of positive numbers return the maximum sum of two
  disjoint segments of the array of lengths K and L.
  If we can't get two such disjoint segments, return -1
*/

#include <iostream>
#include <vector>


bool are_disjoint_segments(const size_t start_i, const size_t K, const size_t start_j, const size_t L)
{
  return (start_i + K <= start_j) || (start_j + L <= start_i);
}

int max_disjoint_segments(const std::vector<uint32_t> &nums, const size_t N, const size_t K, const size_t L)
{
  if (K + L > N)
    {
      return -1;
    }

  // prefix[i] would be sum of elements nums[0]..nums[i - 1]
  std::vector<uint32_t> prefix(N + 1);
  prefix[0] = 0;
  
  for (auto i = 1; i <= N; ++i)
    {
      prefix[i] = nums[i - 1] + prefix[i - 1];
      std::cout << i << " " << prefix[i] << std::endl;
    }

  size_t start_i = 0;
  size_t start_j = start_i + K;
  
  auto max_sum = 0;;

  for (auto i = 0; i < N - K + 1; ++i)
    {
      uint32_t sum_i = prefix[i + K] - prefix[i];	  
      for (auto j = i + K; j < N - L + 1; ++j)
	{
	  uint32_t sum_j = prefix[j + L] - prefix[j];
	  if (are_disjoint_segments(i, K, j, L) && (sum_i + sum_j > max_sum))
	    {
		  max_sum = sum_i + sum_j;
	    }
 
	}
    }

  return max_sum;
	      	  
}
 
int main()
{

  size_t N;
  std::cout << "Enter length of nums array" << std::endl;
  std::cin >> N;

  std::cout << "Enter the nums" << std::endl;
  
  std::vector<uint32_t> nums(N);
  for (auto i = 0; i < N; ++i)
    {
      std::cin >> nums[i];
    }

  size_t K;
  std::cout << "Enter K" << std::endl;
  std::cin >> K;

  size_t L;
  std::cout << "Enter L" << std::endl;
  std::cin >> L;

  std::cout << max_disjoint_segments(nums, N, K, L) << std::endl;
}
