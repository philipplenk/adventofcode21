#include <advent.hpp>

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

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

struct cube
{
	cube(int start_x, int end_x, int start_y, int end_y, int start_z, int end_z)
	{
		start = {start_x,start_y,start_z};
		end = {end_x, end_y, end_z};
	}
	
	friend bool overlap(const cube& lhs, const cube& rhs)
	{
		if(lhs.end.x<rhs.start.x || lhs.start.x>rhs.end.x) return false;
		if(lhs.end.y<rhs.start.y || lhs.start.y>rhs.end.y) return false;
		if(lhs.end.z<rhs.start.z || lhs.start.z>rhs.end.z) return false;
		
		return true;
	}
	
	auto cut(const cube& to_erase) const
	{	
		std::vector<cube> parts;
		
		if(start.x<to_erase.start.x && end.x>=to_erase.start.x)
			parts.push_back({start.x,to_erase.start.x-1,start.y,end.y,start.z,end.z});
		if(end.x>to_erase.end.x && start.x<=to_erase.end.x)
			parts.push_back({to_erase.end.x+1,end.x,start.y,end.y,start.z,end.z});

		
		if(start.y<to_erase.start.y && end.y>=to_erase.start.y)
			parts.push_back({std::max(start.x,to_erase.start.x),std::min(end.x,to_erase.end.x),start.y,to_erase.start.y-1,start.z,end.z});
		if(end.y>to_erase.end.y && start.y<=to_erase.end.y)
			parts.push_back({std::max(start.x,to_erase.start.x),std::min(end.x,to_erase.end.x),to_erase.end.y+1,end.y,start.z,end.z});
		
		if(start.z<to_erase.start.z && end.z>=to_erase.start.z)
			parts.push_back({std::max(start.x,to_erase.start.x),std::min(end.x,to_erase.end.x),std::max(start.y,to_erase.start.y),std::min(end.y,to_erase.end.y),start.z,to_erase.start.z-1});
		if(end.z>to_erase.end.z && start.z<=to_erase.end.z)
			parts.push_back({std::max(start.x,to_erase.start.x),std::min(end.x,to_erase.end.x),std::max(start.y,to_erase.start.y),std::min(end.y,to_erase.end.y),to_erase.end.z+1,end.z});
		
		return parts;
	}
	
	friend bool operator<(const cube& lhs, const cube& rhs) { return std::tie(lhs.start,lhs.end)<std::tie(rhs.start,rhs.end); }
	friend bool operator==(const cube& lhs, const cube& rhs) { return std::tie(lhs.start,lhs.end)==std::tie(rhs.start,rhs.end); }
	
	friend std::ostream& operator<<(std::ostream& out, const cube& c)
	{
		return out<<'('<<c.start.x<<','<<c.start.y<<','<<c.start.z<<')'<<'-'<<'('<<c.end.x<<','<<c.end.y<<','<<c.end.x<<')';
	}
	
	point3d start,end;
};


int main(int argc, char* argv[])
{	
	std::vector<cube> cubes;

	std::string type;
	int start_x,end_x,start_y,end_y,start_z,end_z;
	char skip;
	while(std::cin>>type>>skip>>skip>>start_x>>skip>>skip>>end_x>>skip>>skip>>skip>>start_y>>skip>>skip>>end_y>>skip>>skip>>skip>>start_z>>skip>>skip>>end_z)
	{	
		bool set_on = type=="on";
		
		cube new_cube(start_x,end_x,start_y,end_y,start_z,end_z);
		
		if(set_on)
		{
			std::vector<cube> to_add;
			to_add.push_back(new_cube);
			for(auto c: cubes)
			{
				std::vector<cube> new_to_add;
				for(auto new_c: to_add)
				{
					if(!overlap(c,new_c))
						new_to_add.push_back(new_c);
					else
					{
						const auto parts = new_c.cut(c);
						for(auto p: parts)
							new_to_add.push_back(p);
					}
				}
				to_add = std::move(new_to_add);
			}
			for(auto c: to_add)
				cubes.push_back(c);
		}
		else
		{
			std::vector<cube> new_cubes;
			for(auto c: cubes)
			{
				if(!overlap(c,new_cube))
					new_cubes.push_back(c);
				else
				{
					const auto parts = c.cut(new_cube);
					for(auto part: parts)
						new_cubes.push_back(part);
				}
			}
			cubes = std::move(new_cubes);
		}
	}
	
	unsigned long long sum = 0;
	for(auto c: cubes)
		sum+=(std::abs(c.start.x-c.end.x)+1ull)*(std::abs(c.start.y-c.end.y)+1ull)*(std::abs(c.start.z-c.end.z)+1ull);
	
	std::cout<<sum;
}
