#include <algorithm>
#include <iostream>
#include <limits>
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
	
	const auto [min,max] = std::minmax_element(nums.begin(),nums.end());
	std::vector<int> candidates(*max-*min+1);
	std::iota(candidates.begin(),candidates.end(),*min);
	
	std::cout<<std::transform_reduce(candidates.begin(),candidates.end(),std::numeric_limits<int>::max(),
		[](auto lhs, auto rhs){ return std::min(lhs,rhs); },
		[&](const auto& v)
		{
			return std::accumulate(nums.begin(),nums.end(),0,[v](auto lhs, auto rhs)
			{
				const auto diff = std::abs(rhs-v);
				return lhs+(diff*diff+diff)/2;
			});
		}
	);
}
