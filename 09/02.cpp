#include <advent.hpp>

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace advent;

int main(int argc, char* argv[])
{
	std::vector<std::string> grid(std::istream_iterator<std::string>{std::cin},{});
	const int width = grid[0].size();
	const int height = grid.size();
	
	std::unordered_set<point2d> seen;
	std::vector<int> sizes;
	
	const auto floodfill = [&](point2d p)
	{
		int size = 0;
		
		std::vector<point2d> to_visit;
		to_visit.push_back(p);
		
		while(!to_visit.empty())
		{
			const auto next = to_visit.back();
			to_visit.pop_back();
			
			if(seen.count(next)) continue;
			seen.insert(next);
			++size;
			
			for(auto v: {vec2d{-1,0},vec2d{1,0},vec2d{0,-1},vec2d{0,1}})
			{
				const point2d pos{next.x+v.x,next.y+v.y};
				if(pos.x>=0 && pos.y>=0 && pos.x<width && pos.y<height)
				{
					if(!seen.count(pos) && grid[pos.y][pos.x]<'9')
						to_visit.push_back(pos);
				}
			}
		}
		
		return size;
	};
	
	for(int y=0;y<height;++y)
	{
		for(int x=0;x<width;++x)
		{
			if(grid[y][x]<'9' && !seen.count({x,y}))
				sizes.push_back(floodfill({x,y}));
		}
	}
	
	std::nth_element(sizes.begin(),sizes.begin()+3,sizes.end(),std::greater<>{});
	std::cout<<sizes[0]*sizes[1]*sizes[2];
}
