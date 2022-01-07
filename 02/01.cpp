#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
	int x = 0, y = 0, amount;
	
	std::string dir;
	while(std::cin>>dir>>amount)
	{
		if(dir=="forward") x+=amount;
		if(dir=="up") y-=amount;
		if(dir=="down") y+=amount;
	}
	
	std::cout<<y*x;
}
