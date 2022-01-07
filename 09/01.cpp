#include <advent.hpp>

#include <iostream>
#include <string>
#include <vector>

using namespace advent;

int main(int argc, char* argv[])
{
	std::vector<std::string> grid(std::istream_iterator<std::string>{std::cin},{});
	const int width = grid[0].size();
	const int height = grid.size();
	
	int sum = 0;
	for(int y=0;y<height;++y)
	{
		for(int x=0;x<width;++x)
		{
			const auto value = grid[y][x];
			bool lowest = true;
			for(auto v: {vec2d{-1,0},vec2d{1,0},vec2d{0,-1},vec2d{0,1}})
			{
				point2d pos{x+v.x,y+v.y};
				if(pos.x>=0 && pos.y>=0 && pos.x<width && pos.y<height)
					lowest&=value<grid[pos.y][pos.x];
			}
			if(lowest)
				sum+=value-'0'+1;
		}
	}
	std::cout<<sum;
}
