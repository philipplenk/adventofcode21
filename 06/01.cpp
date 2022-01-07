#include <array>
#include <iostream>
#include <numeric>

int main(int argc, char* argv[])
{
	std::array<int,9> state_counts{};
	int state;
	while(std::cin>>state)
	{
		++state_counts[state];
		char skip;
		std::cin>>skip;
	}
	
	for(int i=0;i<80;++i)
	{
		const auto new_fish = state_counts[0];
		for(int j=0;j<8;++j)
			state_counts[j] = state_counts[j+1];
		
		state_counts[8]=new_fish;
		state_counts[6]+=new_fish;
	}
	
	std::cout<<std::accumulate(state_counts.begin(),state_counts.end(),0);
	
}
