#include <advent.hpp>

#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include <cstddef>

using namespace advent;

int main(int argc, char* argv[])
{
	std::string lookup_table;
	std::cin>>lookup_table;
	
	std::vector<std::string> grid{std::istream_iterator<std::string>{std::cin},{}};
	
	const auto run_step = [&](int id)
	{
		std::vector<std::string> new_grid;
		for(int y=-5;y<static_cast<int>(grid.size())+3;++y)
		{
			std::string line;
			for(int x=-3;x<static_cast<int>(grid[0].size())+3;++x)
			{
				int idx = 0;
				for(auto v: {vec2d{-1,-1},vec2d{0,-1},vec2d{1,-1},vec2d{-1,0},vec2d{0,0},vec2d{1,0},vec2d{-1,1},vec2d{0,1},vec2d{1,1}})
				{
					const auto p = point2d{x,y}+v;
					const int value = p.y<0 || p.y>=static_cast<int>(grid.size()) || p.x<0 || p.x>=static_cast<int>(grid[0].size()) ? (id%2?1:0): (grid[p.y][p.x]=='#');
					idx = idx*2 + value;
				}
				line.push_back(lookup_table[idx]);
			}
			new_grid.push_back(line);
		}
		grid = new_grid;
	};
	
	for(int i=0;i<50;++i)
		run_step(i);
	
	int count = 0;
	for(std::size_t y=0;y<grid.size();++y)
		for(std::size_t x=0;x<grid[y].size();++x)
			count+=grid[y][x]=='#';
			
	std::cout<<count;
}
