#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <cctype>
#include <cstddef>

struct edge
{
	std::string start, end;
};

std::istream& operator>>(std::istream& in, edge& e)
{
	e.start = e.end = "";
	std::string line;
	getline(in,line);
	
	std::size_t i=0;
	for(;i<line.size() && line[i]!='-';++i) e.start+=line[i];
	for(++i;i<line.size();++i) e.end+=line[i];
	
	return in;
}

int main(int argc, char* argv[])
{
	std::unordered_map<std::string,std::unordered_set<std::string>> edges;
	
	edge e;
	while(std::cin>>e)
	{
		edges[e.start].insert(e.end);
		edges[e.end].insert(e.start);
	}
	
	struct waypoint
	{
		std::string current;
		std::unordered_set<std::string> visited;
	};
	
	std::vector<waypoint> to_visit;
	to_visit.push_back({"start",{}});
	
	int count = 0;
	while(!to_visit.empty())
	{
		const auto next = to_visit.back();
		to_visit.pop_back();
		
		if(next.current=="end")
			++count;
		else
		{
			auto visited = next.visited;
			visited.insert(next.current);
			
			for(const auto& n: edges[next.current])
			{
				if(std::isupper(n.front()) || visited.count(n)<1)
					to_visit.push_back({n,visited});
			}
		}
	}
	
	std::cout<<count;
}
