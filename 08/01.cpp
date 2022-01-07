#include <algorithm>
#include <array>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
	std::unordered_map<int,int> digit_by_unique_count = 
	{
		{2,1},
		{4,4},
		{3,7},
		{7,8},
	};
	
	std::array<std::string,10> digits;
	std::array<std::string,4> output;
	
	int count = 0;
	while(std::cin)
	{
		for(auto& d: digits) std::cin>>d;
		char skip;
		std::cin>>skip;
		
		bool ok = true;
		for(auto& d: output) ok&=!!(std::cin>>d);
		if(!ok) break;
		
		for(auto& d: output) std::sort(d.begin(),d.end()), d.erase(std::unique(d.begin(),d.end()),d.end());
		for(const auto& d: output) count+=digit_by_unique_count[d.size()]>0;
	}
	
	std::cout<<count;
}
