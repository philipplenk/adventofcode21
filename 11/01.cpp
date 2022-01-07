#include <advent.hpp>

#include <iostream>
#include <iterator>
#include <string>
#include <unordered_set>
#include <vector>

using namespace advent;

int main(int argc, char* argv[])
{
	std::vector<std::string> grid(std::istream_iterator<std::string>{std::cin},{});
	const int height = grid.size(), width = grid[0].size();
	
	int flashes = 0;
	
	for(int i=0;i<100;++i)
	{
		std::unordered_set<point2d> flashed, new_flashed;
		for(int y=0;y<height;++y)
		{
			for(int x=0;x<width;++x)
			{
				++grid[y][x];
				if(grid[y][x]>'9') flashed.insert({x,y});
			}
		}
		
		while(!flashed.empty())
		{
			flashes+=flashed.size();
			
			for(auto next: flashed)
			{
				for(auto v: {vec2d{-1,0},vec2d{1,0},vec2d{0,-1},vec2d{0,1},vec2d{1,1},vec2d{-1,1},vec2d{1,-1},vec2d{-1,-1}})
				{
					const point2d pos{next.x+v.x,next.y+v.y};
					if(pos.x>=0 && pos.y>=0 && pos.x<width && pos.y<height)
					{
						if(grid[pos.y][pos.x]++=='9')
							new_flashed.insert(pos);
					}
				}
			}
			flashed = new_flashed;
			new_flashed.clear();
		}
		
		for(int y=0;y<height;++y)
		{
			for(int x=0;x<width;++x)
			{
				if(grid[y][x]>'9') grid[y][x]='0';
			}
		}
	}
	
	std::cout<<flashes;
}
