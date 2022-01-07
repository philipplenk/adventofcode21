#include <advent.hpp>

#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace advent;

int main(int argc, char* argv[])
{
	std::vector<std::string> grid(std::istream_iterator<std::string>{std::cin},{});
	const int height = grid.size();
	const int width = grid[0].size();
	
	const auto step = [&](char selector, vec2d movement)
	{
		bool any_movement = false;
		auto new_grid = grid;
		for(int y=0;y<height;++y)
		{
			for(int x=0;x<width;++x)
			{
				const auto next = point2d{(x+movement.x)%width,(y+movement.y)%height};
				if(grid[y][x]==selector && grid[next.y][next.x]=='.')
				{
					any_movement = true;
					new_grid[y][x] = '.';
					new_grid[next.y][next.x]=selector;
				}
			}
		}
		std::swap(grid,new_grid);
		return any_movement;
	};
	
	int steps = 1;
	for(;;++steps)
	{
		const auto east = step('>',{1,0});
		const auto south = step('v',{0,1});
		if(!east && !south) break;
	}
	std::cout<<steps;
}
