#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
	std::vector<int> counts;
	int total_count = 0;
	
	std::string val;
	while(std::cin>>val)
	{
		counts.resize(val.size());
		for(std::size_t i=0;i<val.size();++i)
			counts[i]+=val[i]=='1';
		++total_count;
	}
	
	int gamma = 0, epsilon = 0;
	for(auto c: counts)
	{
		gamma=2*gamma | !!(c>total_count/2);
		epsilon=2*epsilon | !!(c<total_count/2);
	}
	std::cout<<gamma*epsilon;
}
