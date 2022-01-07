#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
	int x = 0, y = 0, aim = 0, amount;
	
	std::string dir;
	while(std::cin>>dir>>amount)
	{
		if(dir=="forward") x+=amount,y+=aim*amount;
		if(dir=="up") aim-=amount;
		if(dir=="down") aim+=amount;
	}
	
	std::cout<<y*x;
}
