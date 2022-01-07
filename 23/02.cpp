#include <advent.hpp>

#include <array>
#include <bitset>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include <cctype>

using namespace advent;

int main(int argc, char* argv[])
{
	constexpr auto number_of_types = 4;
	constexpr auto number_of_slots = 4;
	constexpr auto max_width = 14;
	constexpr auto max_height = 8;
	constexpr auto first_free_x = 1;
	constexpr auto last_free_x = 11;
	constexpr auto outside_y = 1;
	constexpr auto slot_start_x = 3;
	constexpr auto slot_distance = 2;
	constexpr auto cost_multiplier = 10;
	
	std::vector<std::string> grid;
	std::string line;
	while(std::getline(std::cin,line))
		grid.push_back(line);
		
	grid.insert(grid.begin()+3,"  #D#C#B#A#");
	grid.insert(grid.begin()+4,"  #D#B#A#C#");
		
	const int width = grid[0].size();
	const int height = grid.size();
	
	std::array<std::array<point2d,number_of_slots>,number_of_types> starting_positions;
	std::array<int,number_of_types> starting_pos_found{};
	for(int y=0;y<height;++y)
	{
		for(int x=0;x<width;++x)
		{
			if(std::isalpha(grid[y][x]))
			{
				const int type_id = grid[y][x]-'A';
				starting_positions[type_id][starting_pos_found[type_id]++] = point2d{x,y};
			}
		}
	}
	
	const auto final_configuration = []()
	{
		std::array<std::array<point2d,number_of_slots>,number_of_types> result;
		
		for(int type=0;type<number_of_types;++type)
			for(int slot=0;slot<number_of_slots;++slot)
				result[type][slot] = point2d{slot_start_x+type*slot_distance,outside_y+1+slot};
				
		return result;
	}();
	
	const auto is_final = [&](const auto pos)
	{
		for(int type=0;type<number_of_types;++type)
		{
			const auto& correct = final_configuration[type];
			if(!std::is_permutation(pos[type].begin(),pos[type].end(),correct.begin())) return false;
		}
		return true;
	};
		
	struct waypoint
	{
		std::array<std::array<point2d,number_of_slots>,number_of_types> positions;
		std::array<bool,number_of_slots*number_of_types> done{};
		long long costs = 0, min_finishing_cost = 0;
		
		bool operator<(const waypoint& other) const
		{
			return other.costs+other.min_finishing_cost<costs+min_finishing_cost;
		}
	};
	
	const auto accumulated_min_finish = [&](const auto& positions)
	{
		int step_cost = 1, sum = 0;
		for(int type = 0;type<number_of_types;++type)
		{
			const int target_x = slot_start_x+type*slot_distance;
			for(auto p: positions[type])
				sum+=step_cost*std::abs(p.x-target_x);
			step_cost*=cost_multiplier;
		}
		return sum;
	};
	
	const auto list_moves = [&](const waypoint& last)
	{
		std::vector<waypoint> result_start;
		int step_cost = 1;
		std::bitset<max_width*max_height> occupied;
		for(const auto& points: last.positions)
		{
			for(const auto& p: points)
				occupied[p.y*14+p.x] = true;
		}
		
		const auto can_escape = [&](point2d p)
		{
			for(int y=p.y-1;y>=outside_y;--y)
				if(occupied[y*max_width+p.x]) return false;
			return true;
		};
		
		for(int type=0;type<number_of_types;++type)
		{
			for(int id=0;id<number_of_slots;++id)
			{
				if(last.done[type*number_of_slots+id]) continue;
				
				auto position_cpy = last.positions;
				const auto p = last.positions[type][id];
				
				const auto create_move = [&](point2d dest) -> waypoint
				{
					const auto movement_costs = (std::abs(dest.x-p.x)+std::abs(dest.y-p.y))*step_cost;
					position_cpy[type][id] = dest;
					
					auto done_cpy = last.done;
					if(dest.y!=outside_y) done_cpy[type*number_of_slots+id] = true;
					return {position_cpy,done_cpy,last.costs+movement_costs,accumulated_min_finish(position_cpy)};	
				};
				
				const auto would_block = [&](int x) { return x>=slot_start_x && x<slot_start_x+number_of_types*slot_distance && (x-slot_start_x)%slot_distance==0; };
				
				if(p.y>outside_y && can_escape(p))
				{
					for(int x=p.x-1; x>=first_free_x && !occupied[outside_y*max_width+x];--x)
					{
						if(!would_block(x))
							result_start.push_back(create_move({x,outside_y}));
					}
					
					for(int x=p.x+1;x<=last_free_x && !occupied[outside_y*max_width+x];++x)
					{
						if(!would_block(x))
							result_start.push_back(create_move({x,outside_y}));
					}
				}
				else if(p.y==outside_y)
				{
					const int target_x = slot_start_x+type*slot_distance;
					const int step = p.x>target_x?-1:1;
					
					bool path_clear = true;
					for(int x = p.x; x!=target_x; x+=step)
						path_clear&= x==p.x || !occupied[1*max_width+x];
										
					path_clear&=!occupied[1*max_width+target_x];
					
					std::bitset<outside_y+number_of_slots+1> y_occupied{};
					for(auto op: last.positions[type]) if(op.x==target_x) y_occupied[op.y] = true;
					
					int max_y = outside_y+number_of_slots;
					while(max_y>1 && y_occupied[max_y]) --max_y;
					for(int y=outside_y;y<=max_y;++y) path_clear&=!occupied[y*max_width+target_x];
					
					if(path_clear)
						return std::vector<waypoint>{create_move({target_x,max_y})};
				}
			}
			step_cost*=cost_multiplier;
		}
		return result_start;
	};
	
	std::priority_queue<waypoint> to_visit;
	to_visit.push({starting_positions,{},0,0});
	
	while(!to_visit.empty())
	{
		const auto next = to_visit.top();
		to_visit.pop();
		
		if(is_final(next.positions))
		{
			std::cout<<next.costs;
			break;
		}
				
		for(auto move: list_moves(next))
			to_visit.push(move);
	}
}
