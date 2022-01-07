#include <algorithm>
#include <iostream>
#include <iterator>
#include <tuple>
#include <vector>

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

bool operator==(const point2d& lhs, const point2d& rhs)
{
	return std::tie(lhs.x,lhs.y) == std::tie(rhs.x,rhs.y);
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
	std::vector<line> lines{std::istream_iterator<line>{std::cin},{}};
	
	std::unordered_map<point2d,int> line_points;
	
	for(auto& line: lines)
	{
		if(line.start.x==line.end.x)
		{
			if(line.start.y>line.end.y) std::swap(line.start.y,line.end.y);
			for(int y = line.start.y;y<=line.end.y;++y)
				++line_points[{line.start.x,y}];
		}
		else if(line.start.y==line.end.y)
		{
			if(line.start.x>line.end.x) std::swap(line.start.x,line.end.x);
			for(int x = line.start.x;x<=line.end.x;++x)
				++line_points[{x,line.start.y}];
		}
	}
	
	std::cout<<std::count_if(line_points.begin(),line_points.end(),[](const auto& p)
	{
		return p.second>1;
	});
}
