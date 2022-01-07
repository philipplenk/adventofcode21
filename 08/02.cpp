#include <array>
#include <iostream>
#include <string>
#include <unordered_map>

struct bit_mapping
{
	int& target;
	
	friend std::istream& operator>>(std::istream& in, bit_mapping bits)
	{
		static std::string s;
		in>>s;
		
		bits.target = 0;
		for(auto c: s)
			bits.target|=(1<<(c-'a'));
		
		return in;
	}
};

int deduce(const std::array<int,10>& digits, const std::array<int,4>& output)
{
	static const std::unordered_map<int,int> digit_by_unique_count = 
	{
		{2,1},
		{4,4},
		{3,7},
		{7,8},
	};
	
	std::array<int,10> known;
	std::unordered_map<int,int> mapping;
	
	for(const auto& d: digits)
	{
		const auto length = __builtin_popcount(d);
		if(auto it = digit_by_unique_count.find(length); it!=digit_by_unique_count.end())
		{
			known[it->second] = d;
			mapping[d] = it->second;
		}
	}
	
	const auto identify = [&](int digit, auto fun)
	{
		for(const auto& d: digits)
		{
			if(!mapping.count(d) && fun(d))
			{
				known[digit] = d;
				mapping[d] = digit;
				break;
			}
		}
	};
	
	identify(6,[&](auto d) { return __builtin_popcount(d)==6 && (known[1]&d)!=known[1]; });
			
	const auto c = known[8]^known[6];
	const auto f = known[1]^c;
		
	identify(2,[&](auto d) { return __builtin_popcount(d)==5 && (d&f)==0; });
	
	identify(5,[&](auto d) { return __builtin_popcount(d)==5 && (d&c)==0; });
	identify(3,[&](auto d) { return __builtin_popcount(d)==5 && (d&c)!=0; });
	
	const auto e = known[6]^known[5];
		
	identify(9,[&](auto d) { return __builtin_popcount(d)==6 && (d&e)==0; });
	identify(0,[&](auto d) { return __builtin_popcount(d)==6 && (d&e)!=0; });
	
	int result = 0;
	for(auto digit: output)
		result = result*10 + mapping[digit];
	
	return result;
};

int main(int argc, char* argv[])
{	
	std::array<int,10> digits{};
	std::array<int,4> output{};
	
	int sum = 0;
	while(std::cin)
	{
		for(auto& d: digits) std::cin>>bit_mapping{d};
		
		std::cin>>std::ws;
		std::cin.ignore();
		
		for(auto& d: output) std::cin>>bit_mapping{d};
		
		if(std::cin) sum+=deduce(digits,output);
	}
	
	std::cout<<sum;
}
