#include <array>
#include <iostream>

int main(int argc, char* argv[])
{
	std::array<int,3> window{};
	int count = 0, idx = 0, next;
	
	for(auto& v: window) std::cin>>v;
	
	while(std::cin>>next)
	{
		count+=next>window[idx];
		window[idx++] = next;
		idx%=window.size();
	}
	
	std::cout<<count;
}
