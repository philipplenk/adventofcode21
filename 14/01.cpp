#include <algorithm>
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
	std::string str;
	
	std::cin>>str;
	std::vector<substitution_rule> rules(std::istream_iterator<substitution_rule>{std::cin},{});
	std::unordered_map<std::string,char> rules_map;
	for(const auto& r: rules)
		rules_map[r.match] = r.insert[0];
	
	for(int i=0;i<10;++i)
	{
		std::string new_str;
		
		for(std::size_t j=0;j<str.size();++j)
		{
			new_str+=str[j];
			if(auto it=rules_map.find(str.substr(j,2));it!=rules_map.end())
				new_str+=it->second;
		}
		str = std::move(new_str);
	}
	
	std::unordered_map<char,int> counts;
	for(auto c:str)
		++counts[c];
	
	const auto [min_it,max_it] = std::minmax_element(counts.begin(),counts.end(),[](const auto& lhs, const auto &rhs)
	{
		return lhs.second<rhs.second;
	});
	
	std::cout<<max_it->second-min_it->second;
}
