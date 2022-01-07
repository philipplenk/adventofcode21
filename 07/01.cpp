#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <vector>

int main(int argc, char* argv[])
{
	std::vector<int> nums;
	int n;
	while(std::cin>>n)
	{
		char skip;
		std::cin>>skip;
		nums.push_back(n);
	}
	
	std::sort(nums.begin(),nums.end());
	const auto median = nums[nums.size()/2];
	std::cout<<std::accumulate(nums.begin(),nums.end(),0,[&](auto lhs, auto rhs)
	{
		return lhs+std::abs(rhs-median);
	});
}
