#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int main(int argc, char* argv[])
{
	std::unordered_map<char,int> points
	{
		{')',3},
		{']',57},
		{'}',1197},
		{'>',25137},
	};
	
	std::string line;
	int score = 0;
	while(std::getline(std::cin,line))
	{
		std::vector<char> stack;
		bool do_break = false;
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
						score+=points[c];
						do_break = true;
						break;
					}
					stack.pop_back();
					break;
				}
			}
			if(do_break) break;
		}
	}
	std::cout<<score;
}
