#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

int main(int argc, char* argv[])
{
	std::unordered_map<char,unsigned long> points
	{
		{')',1},
		{']',2},
		{'}',3},
		{'>',4},
	};
	
	std::string line;
	std::vector<unsigned long> scores;
	while(std::getline(std::cin,line))
	{
		std::vector<char> stack;
		bool valid = true;
		for(auto c: line)
		{
			switch(c)
			{
				case '(': stack.push_back(')'); break;
				case '{': stack.push_back('}'); break;
				case '[': stack.push_back(']'); break;
				case '<': stack.push_back('>'); break;
				default:
				{
					if(stack.empty() || stack.back()!=c)
					{
						valid = false;
						break;
					}
					stack.pop_back();
					break;
				}
			}
			if(!valid) break;
		}
		
		if(valid)
		{
			const auto sum = std::accumulate(stack.rbegin(),stack.rend(),0ul,[&](const auto& lhs, const auto & rhs)
			{
				return lhs*5ul+points[rhs];
			});
			scores.push_back(sum);
		}
	}
	std::sort(scores.begin(),scores.end());
	std::cout<<scores[scores.size()/2];
}
