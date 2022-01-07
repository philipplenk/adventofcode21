#include <array>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
	std::array<int,2> players{6,1};
	std::array<int,2> scores{0,0};
	
	std::string skip;
	for(auto& p: players)
	{
		std::cin>>skip>>skip>>skip>>skip>>p;
		--p;
	}
	
	int current = 0, turns = 0, dice = 1;
	while(scores[0]<1000 && scores[1]<1000)
	{
		int value = dice;
		dice = dice+1<=100?dice+1:1;
		value+=dice;
		dice = dice+1<=100?dice+1:1;
		value+=dice;
		dice = dice+1<=100?dice+1:1;
		
		players[current]+=value;
		players[current]%=10;
		scores[current]+=players[current]+1;
		current = (current+1)%2;
		turns+=3;
	}
	std::cout<<turns*std::min(scores[0],scores[1]);
}
