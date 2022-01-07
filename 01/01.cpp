#include <iostream>

int main(int argc, char* argv[])
{
	int last, now, count = 0;
	std::cin>>last;
	while(std::cin>>now)
	{
		count+=now>last;
		last = now;
	}
	std::cout<<count;
}
