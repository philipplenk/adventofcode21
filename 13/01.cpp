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
	for(auto p: coordinates)
	{
		const auto ins = instructions.front();
		
		if(ins.axis=='x' && ins.value<p.x) p.x = ins.value-(p.x-ins.value);
		if(ins.axis=='y' && ins.value<p.y) p.y = ins.value-(p.y-ins.value);
		if(ins.axis=='x' && ins.value==p.x) continue;
		if(ins.axis=='y' && ins.value==p.y) continue;
		visible_points.insert(p);
	}
	
	std::cout<<visible_points.size();
}
