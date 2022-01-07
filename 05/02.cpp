#include <algorithm>
#include <iostream>
#include <iterator>
#include <tuple>

#include <cstddef>

struct point2d
{
	int x, y;
};

std::istream& operator>>(std::istream& in, point2d& p)
{
	char skip;
	return in>>p.x>>skip>>p.y;
}

constexpr bool operator==(const point2d& lhs, const point2d& rhs)
{
	return std::tie(lhs.x,lhs.y) == std::tie(rhs.x,rhs.y);
}

constexpr bool operator!=(const point2d& lhs, const point2d& rhs)
{
	return !(lhs==rhs);
}

struct line
{
	point2d start, end;
};

std::istream& operator>>(std::istream& in, line& l)
{
	char skip;
	return in>>l.start>>skip>>skip>>l.end;
}

namespace std
{
	template <>
	struct hash<point2d>
	{
		std::size_t operator()(const point2d& p) const noexcept
		{
			return p.x^p.y;
		}
	};
}

int main(int argc, char* argv[])
{	
	std::unordered_map<point2d,int> line_points;
	
	std::for_each(std::istream_iterator<line>{std::cin},{},[&](const auto& line)
	{
		const int dx = line.start.x<line.end.x?1:line.start.x==line.end.x?0:-1;
		const int dy = line.start.y<line.end.y?1:line.start.y==line.end.y?0:-1;
		
		for(auto p = line.start; p!=line.end; p.x+=dx, p.y+=dy)
			++line_points[p];
			
		++line_points[line.end];
	});
	
	std::cout<<std::count_if(line_points.begin(),line_points.end(),[](const auto& p)
	{
		return p.second>1;
	});
}
