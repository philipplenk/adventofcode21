#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

#include <cstddef>

struct substitution_rule
{
	std::string match, insert;
};

std::istream& operator>>(std::istream& in, substitution_rule& s)
{
	char skip;
	return in>>s.match>>skip>>skip>>s.insert;
}

int main(int argc, char* argv[])
{
	std::string str_normal;
	
	std::cin>>str_normal;
	std::vector<substitution_rule> rules(std::istream_iterator<substitution_rule>{std::cin},{});
	std::unordered_map<std::string,char> rules_map;
	for(const auto& r: rules)
		rules_map[r.match] = r.insert[0];
		
	//order of pairs does not matter, so instead of keeping all of the string, we simply count how often each pair appears
	std::unordered_map<std::string,std::size_t> str;
	for(std::size_t i=0;i<str_normal.size();++i)
		str[str_normal.substr(i,2)]+=1;
	
	for(int i=0;i<40;++i)
	{
		std::unordered_map<std::string,std::size_t> new_str;
		
		for(const auto& [pair,count]: str) 
		{
			if(auto it=rules_map.find(pair);it!=rules_map.end())
			{
				new_str[std::string(1,pair[0])+it->second]+=count;
				new_str[std::string(1,it->second)+pair[1]]+=count;
			}
			else
				new_str[pair]+=count;
		}
		str = std::move(new_str);
	}
	
	std::unordered_map<char,std::size_t> counts;
	for(const auto& [pair,count]:str)
		counts[pair[0]]+=count;
	
	const auto [min_it,max_it] = std::minmax_element(counts.begin(),counts.end(),[](const auto& lhs, const auto &rhs)
	{
		return lhs.second<rhs.second;
	});
	
	std::cout<<max_it->second-min_it->second;
}
