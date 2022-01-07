#include <advent.hpp>

#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>

#include <cstddef>

using namespace advent;

namespace std
{
	template <>
	struct hash<advent::point3d>
	{
		std::size_t operator()(const advent::point3d& p) const noexcept
		{
			return p.x*50+p.y*50*50+p.z;
		}
	};
}

int main(int argc, char* argv[])
{	
	std::unordered_map<point3d,bool> state;

	std::string type;
	int start_x,end_x,start_y,end_y,start_z,end_z;
	char skip;
	while(std::cin>>type>>skip>>skip>>start_x>>skip>>skip>>end_x>>skip>>skip>>skip>>start_y>>skip>>skip>>end_y>>skip>>skip>>skip>>start_z>>skip>>skip>>end_z)
	{
		bool out_of_range = false;
		for(auto v: {start_x,end_x,start_y,end_y,start_z,end_z})
			out_of_range|=(v<-50 || v>50);
		
		if(out_of_range) continue;
		
		bool to_set = type=="on";
		for(int x=start_x;x<=end_x;++x)
		{
			for(int y=start_y;y<=end_y;++y)
			{
				for(int z=start_z;z<=end_z;++z)
				{
					state[{x,y,z}] = to_set;
 				}
			}
		}
	}
	
	std::cout<<std::accumulate(state.begin(),state.end(),0ll,[](auto sum, auto pair){ return sum+pair.second; });
}
