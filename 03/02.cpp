#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

template <typename iter_t, typename compare_fun>
auto bit_partition_remainder(iter_t begin, iter_t end, int bit_id, compare_fun compare)
{
	if(std::distance(begin,end)==1) return *begin;
	
	const auto partition_point = std::partition(begin, end,[bit_id](const auto& str)
	{
		return str[bit_id] == '1';
	});
	
	if(compare(std::distance(begin,partition_point),std::distance(partition_point,end)))
		return bit_partition_remainder(begin,partition_point,bit_id+1,compare);
	
	return bit_partition_remainder(partition_point,end,bit_id+1,compare);
}


int main(int argc, char* argv[])
{
	std::vector<std::string> bits(std::istream_iterator<std::string>{std::cin},{});
	
	const auto as_int = [](const auto& str)
	{
		int res = 0;
		for(auto c: str)
			res = res<<1 | (c=='1');
			
		return res;
	};
	
	std::cout<<as_int(bit_partition_remainder(bits.begin(),bits.end(),0,std::greater_equal<>{}))*as_int(bit_partition_remainder(bits.begin(),bits.end(),0,std::less<>{}));
}
