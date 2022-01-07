#include <advent.hpp>

#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_set>
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
			return p.x^p.y^p.z;
		}
	};
}

constexpr vec3d operator-(const point3d& lhs, const point3d& rhs) noexcept
{
	return {lhs.x-rhs.x,lhs.y-rhs.y,lhs.z-rhs.z};
}

struct scanner_data
{
	point3d position{0,0,0};
	std::unordered_set<point3d> beacons{};
};

std::istream& operator>>(std::istream& in, scanner_data& s)
{
	std::string line;
	if(!std::getline(in,line)) return in;

	char skip;
	s.beacons.clear();
	while(std::getline(in,line) && !line.empty())
	{
		std::stringstream strm{line};
		point3d pos;
		strm>>pos.x>>skip>>pos.y>>skip>>pos.z;
		s.beacons.insert(pos);
	}
	
	in.clear(in.rdstate() & ~std::ios::failbit);

	return in;
}

point3d rotate(point3d p, int rotation)
{
	switch(rotation)
	{
		case 0: return point3d{-p.z,-p.x,p.y};
		case 1: return point3d{p.y,p.x,-p.z};
		case 2: return point3d{p.z,-p.x,-p.y};
		case 3: return point3d{p.z,p.x,p.y};
		case 4: return point3d{-p.z,p.x,-p.y};
		case 5: return point3d{p.y,p.z,p.x};
		case 6: return point3d{p.y,-p.x,p.z};
		case 7: return point3d{p.z,-p.y,p.x};
		case 8: return point3d{-p.y,-p.x,-p.z};
		case 9: return point3d{p.x,p.y,p.z};
		case 10: return point3d{-p.x,-p.z,-p.y};
		case 11: return point3d{-p.y,p.x,p.z};
		case 12: return point3d{p.x,-p.z,p.y};
		case 13: return point3d{-p.y,-p.z,p.x};
		case 14: return point3d{p.x,-p.y,-p.z};
		case 15: return point3d{p.z,p.y,-p.x};
		case 16: return point3d{-p.x,p.y,-p.z};
		case 17: return point3d{-p.z,p.y,p.x};
		case 18: return point3d{p.x,p.z,-p.y};
		case 19: return point3d{p.y,-p.z,-p.x};
		case 20: return point3d{-p.z,-p.y,-p.x};
		case 21: return point3d{-p.x,-p.y,p.z};
		case 22: return point3d{-p.y,p.z,-p.x};
		case 23: return point3d{-p.x,p.z,p.y};
		
	}
	return p;
}

int main(int argc, char* argv[])
{
	std::vector<scanner_data> scanners{std::istream_iterator<scanner_data>{std::cin},{}};
	std::vector<scanner_data> known_scanners;
	
	known_scanners.push_back(std::move(scanners.back()));
	scanners.pop_back();
	
	const auto determine_overlap = [](const scanner_data& known, scanner_data& to_position) -> bool
	{
		for(int relative_rotation = 0; relative_rotation<24; ++relative_rotation)
		{
			for(auto offset_p: known.beacons)
			{
				for(auto other_p: to_position.beacons)
				{
					const auto shift =  offset_p-rotate(other_p,relative_rotation);
					int overlap_count = 0;
					for(auto p: to_position.beacons)
						overlap_count+=known.beacons.count(rotate(p,relative_rotation)+shift);
					
					if(overlap_count>=12)
					{
						std::unordered_set<point3d> rotated;
						for(auto p: to_position.beacons)
							rotated.insert(rotate(p,relative_rotation));
						
						to_position.position = known.position+vec3d{-shift.x,-shift.y,-shift.z};
						to_position.beacons = std::move(rotated);
						return true;
					}
				}
			}
		}
		return false;
	};

	while(!scanners.empty())
	{
		std::size_t finished = 0;
		for(std::size_t i=0;i<scanners.size();++i)
		{
			for(std::size_t j=finished;j<known_scanners.size();++j)
			{
				const auto& known = known_scanners[j];
				if(determine_overlap(known,scanners[i]))
				{
					std::swap(scanners.back(),scanners[i]);
					known_scanners.push_back(std::move(scanners.back()));
					scanners.pop_back();
					--i;
					finished = j+1;
					break;
				}
			}
		}
	}
	
	int max_distance = 0;
	for(std::size_t i=0;i<known_scanners.size();++i)
	{
		for(std::size_t j=i+1;j<known_scanners.size();++j)
		{
			const auto p0 = known_scanners[i].position;
			const auto p1 = known_scanners[j].position;
			max_distance = std::max(max_distance,std::abs(p0.x-p1.x)+std::abs(p0.y-p1.y)+std::abs(p0.z-p1.z));
		}
	} 
	
	std::cout<<max_distance;
}
