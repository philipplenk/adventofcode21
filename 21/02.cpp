#include <array>
#include <iostream>
#include <map>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
	std::array<int,2> players{6,1};
	std::array<int,2> scores{};
	
	std::string skip;
	for(auto& p: players)
	{
		std::cin>>skip>>skip>>skip>>skip>>p;
		--p;
	}
	
	std::map<std::tuple<std::array<int,2>,std::array<int,2>,int,int,int>,std::array<long long,2>> cache;
	
	const auto solve_rec = [&](auto players, auto scores, auto current_player, auto dice_sum, auto throw_num, auto rec) -> std::array<long long,2>
	{
		if(scores[0]>=21) return {1,0};
		if(scores[1]>=21) return {0,1};
		if(auto it=cache.find({players,scores,current_player,dice_sum,throw_num});it!=cache.end()) return it->second;
		
		std::array<long long,2> result;
		for(int i=1;i<=3;++i)
		{
			if(throw_num==2)
			{
				auto player_cpy = players;
				auto score_cpy = scores;
				player_cpy[current_player] = (player_cpy[current_player]+dice_sum+i)%10;
				score_cpy[current_player] += player_cpy[current_player]+1;
				const auto res = rec(player_cpy,score_cpy,(current_player+1)%2, 0, 0, rec);
				result[0]+=res[0];
				result[1]+=res[1];
			}
			else
			{
				const auto res = rec(players,scores,current_player, dice_sum+i, throw_num+1, rec);
				result[0]+=res[0];
				result[1]+=res[1];
			}
		}
			
		return cache[{players,scores,current_player,dice_sum,throw_num}] = result;
	};
	
	const auto result = solve_rec(players,scores,0,0,0,solve_rec);
	std::cout<<std::max(result[0],result[1]);
}
