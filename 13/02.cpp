#include <advent.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace advent;

struct fold_instruction
{
	int value;
	char axis;
};

int main(int argc, char* argv[])
{
	std::vector<point2d> coordinates;
	std::vector<fold_instruction> instructions;
	
	std::string line;
	while(std::getline(std::cin,line) && !line.empty())
	{
		std::stringstream strm{line};
		point2d p;
		char skip;
		strm>>p.x>>skip>>p.y;
		coordinates.push_back(p);
	}
	
	std::string skip_s;
	char skip_c;
	fold_instruction ins;
	while(std::cin>>skip_s>>skip_s>>ins.axis>>skip_c>>ins.value)
		instructions.push_back(ins);
		
	
	std::unordered_set<point2d> visible_points;
	visible_points.insert(coordinates.begin(),coordinates.end());
	
	int max_x = 0, max_y = 0;
	for(auto ins: instructions)
	{
		max_x = 0, max_y = 0;
		std::unordered_set<point2d> next_step;
		for(auto p: visible_points)
		{
			
			if(ins.axis=='x' && ins.value<p.x) p.x = ins.value-(p.x-ins.value);
			if(ins.axis=='y' && ins.value<p.y) p.y = ins.value-(p.y-ins.value);
			if(ins.axis=='x' && ins.value==p.x) continue;
			if(ins.axis=='y' && ins.value==p.y) continue;
			next_step.insert(p);
			max_x = std::max(max_x,p.x);
			max_y = std::max(max_y,p.y);
		}
		visible_points = std::move(next_step);
	}
	
	std::vector<std::string> grid(max_y+1,std::string(max_x+1,'_'));
	for(auto p: visible_points)
		grid[p.y][p.x]='O';
		
	for(const auto& line: grid)
		std::cout<<line<<'\n';
}
