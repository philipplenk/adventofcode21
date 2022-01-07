#include <advent.hpp>

#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include <cstddef>

using namespace advent;

int main(int argc, char* argv[])
{
	std::vector<std::string> grid(std::istream_iterator<std::string>{std::cin},{});
	const int height = grid.size();
	const int width = grid[0].size();
			
	struct waypoint
	{
		point2d p;
		int cost = 0;
		bool operator<(const waypoint& w) const
		{
			return cost>w.cost;
		}
	};
	
	std::priority_queue<waypoint> waypoints; 
	std::unordered_map<point2d,int> lowest;
	waypoints.push({{0,0},0});
	
	while(!waypoints.empty())
	{
		const auto next = waypoints.top();
		waypoints.pop();
		if(auto it = lowest.find(next.p);it!=lowest.end() && it->second<=next.cost) continue;
		lowest[next.p] = next.cost;
		
		if(next.p==point2d{5*width-1,5*height-1})
		{
			std::cout<<next.cost;
			break;
		}
		
		for(auto d: {vec2d{1,0},vec2d{-1,0},vec2d{0,1},vec2d{0,-1}})
		{
			const auto dest = next.p+d;
			if(dest.x>=0 && dest.y>=0 && dest.x<5*width && dest.y<5*height)
			{
				auto value = (grid[dest.y%height][dest.x%width]-'0'+dest.y/height+dest.x/width);
				if(value>9) value=1+value%10;
				waypoints.push({dest,next.cost+value});
			}
		}
	}
}
