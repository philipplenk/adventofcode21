#include <array>
#include <bitset>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <cstddef>

struct board_info
{
	std::array<int,25> data;
	std::array<std::bitset<5>,5> completed_in_row{}, completed_in_column{};
	bool done = false;
};

int main(int argc, char* argv[])
{
	std::string drawn_numbers_str;
	std::getline(std::cin,drawn_numbers_str);
	std::stringstream strm{drawn_numbers_str};
	
	int n;
	char skip;
	std::vector<int> drawn_numbers;
	while(strm>>n)
	{
		drawn_numbers.push_back(n);
		strm>>skip;
	}
	
	std::vector<board_info> boards;
	std::array<int,25> board;
	bool oki = true;
	while(oki)
	{
		oki = true;
		for(auto& b: board) oki&=!!(std::cin>>b);
		if(oki) boards.push_back({board,{},{},false});
	}
	
	std::size_t winners = 0;
	for(auto n: drawn_numbers)
	{
		for(auto& info:boards)
		{
			int sum = 0;
			bool done = false;
			for(std::size_t i=0;i<info.data.size();++i)
			{
				if(info.data[i]==n)
				{
					info.completed_in_row[i/5].set(i%5);
					info.completed_in_column[i%5].set(i/5);
					if(info.completed_in_row[i/5]==std::bitset<5>("11111") || info.completed_in_column[i%5]==std::bitset<5>("11111"))
					{
						done = true;
						if(!info.done) ++winners;
						info.done = true;
					}
				}
				else if(!info.completed_in_row[i/5][i%5])
					sum+=info.data[i];
			}
			if(done && winners == boards.size())
			{
				std::cout<<sum*n;
				return 0;
			}
		}
	}
}
