#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <variant>
#include <vector>

#include <cstddef>

struct snailfish_number
{
	using variant = std::variant<int,std::unique_ptr<snailfish_number>>;
	variant a,b;
	
	snailfish_number() = default;
	snailfish_number(const snailfish_number& other)
	{
		const auto copy = [](const auto& to_copy) -> variant
		{
			if(std::holds_alternative<int>(to_copy))
				return std::get<int>(to_copy);
			
			return std::make_unique<snailfish_number>(*std::get<std::unique_ptr<snailfish_number>>(to_copy));
		};
		a = copy(other.a);
		b = copy(other.b);
	}
	
	snailfish_number& operator=(snailfish_number other)
	{
		std::swap(*this,other);
		return *this;	
	}
};

bool is_nested(const snailfish_number::variant& var)
{
	return std::holds_alternative<std::unique_ptr<snailfish_number>>(var);
}

auto& unpack_nested(snailfish_number::variant& var)
{
	return *std::get<std::unique_ptr<snailfish_number>>(var);
}

const auto& unpack_nested(const snailfish_number::variant& var)
{
	return *std::get<std::unique_ptr<snailfish_number>>(var);
}

std::istream& operator>>(std::istream& in, snailfish_number::variant& s);
std::istream& operator>>(std::istream& in, snailfish_number& s)
{
	char skip;
	return in>>skip>>s.a>>skip>>s.b>>skip;
}

std::istream& operator>>(std::istream& in, snailfish_number::variant& s)
{
	if(in.peek()=='[')
	{
		s = std::make_unique<snailfish_number>();
		return in>>unpack_nested(s);
	}
	
	s = 0;
	return in>>std::get<int>(s);
}

std::ostream& operator<<(std::ostream& out, const snailfish_number& s)
{
	const auto part_printer = [&](const auto& v)
	{
		if constexpr(std::is_same_v<std::decay_t<decltype(v)>,int>)
			out<<v;
		else
			out<<*v;
	};
	
	out<<'[';
	std::visit(part_printer,s.a);
	out<<',';
	std::visit(part_printer,s.b);
	out<<']';
	return out;
}

bool is_number_pair(const snailfish_number& s)
{
	return std::holds_alternative<int>(s.a) && std::holds_alternative<int>(s.b);
}

bool is_number_pair(const snailfish_number::variant& var)
{
	return is_nested(var) && is_number_pair(unpack_nested(var));
}

std::pair<int,int> as_number_pair(const snailfish_number::variant& var)
{
	const auto& pair = unpack_nested(var);
	return {std::get<int>(pair.a),std::get<int>(pair.b)};
}

enum class add_direction{ left, right };
template <add_direction dir> bool try_add(snailfish_number::variant& var, int val);
template <add_direction dir> bool try_add(snailfish_number& s, int val)
{
	if constexpr(dir==add_direction::right)
		return try_add<dir>(s.a,val) || try_add<dir>(s.b,val);
	else
		return try_add<dir>(s.b,val) || try_add<dir>(s.a,val);
}

template <add_direction dir> bool try_add(snailfish_number::variant& var, int val)
{
	if(std::holds_alternative<int>(var))
	{
		std::get<int>(var)+=val;
		return true;
	}
	return try_add<dir>(unpack_nested(var),val);
}

struct reduce_action
{
	int add_left, add_right;
	bool was_reduction;
};
reduce_action try_explode(snailfish_number::variant& var, int nesting);
reduce_action try_explode(snailfish_number& s, int nesting)
{
	if(nesting==3 && is_number_pair(s.a))
	{
		const auto [left, right] = as_number_pair(s.a);
		s.a = 0;
		return {left,try_add<add_direction::right>(s.b,right)?0:right,true};
	}
	
	if(nesting==3 && is_number_pair(s.b))
	{
		const auto [left, right] = as_number_pair(s.b);
		s.b = 0;
		return {try_add<add_direction::left>(s.a,left)?0:left,right,true};
	}
	
	if(auto recurse_result = try_explode(s.a,nesting+1); recurse_result.was_reduction)
	{
		if(recurse_result.add_right && try_add<add_direction::right>(s.b,recurse_result.add_right)) recurse_result.add_right = 0;
		return recurse_result;
	}
	
	if(auto recurse_result = try_explode(s.b,nesting+1); recurse_result.was_reduction)
	{
		if(recurse_result.add_left && try_add<add_direction::left>(s.a,recurse_result.add_left)) recurse_result.add_left = 0;
		return recurse_result;
	}
	
	return {0,0,false};
}

reduce_action try_explode(snailfish_number::variant& var, int nesting)
{
	if(is_nested(var))
		return try_explode(unpack_nested(var),nesting);
	
	return {0,0,false};
}

bool try_explode(snailfish_number& s)
{
	return try_explode(s,0).was_reduction;
}

std::unique_ptr<snailfish_number> split(int value)
{
	auto new_pair = std::make_unique<snailfish_number>();
	new_pair->a = value/2;
	new_pair->b = (value+1)/2;
	return new_pair;
}

bool try_split(snailfish_number::variant& var);
bool try_split(snailfish_number& s)
{
	return try_split(s.a) || try_split(s.b);
}

bool try_split(snailfish_number::variant& var)
{
	if(std::holds_alternative<int>(var) && std::get<int>(var)>=10)
	{
		var = split(std::get<int>(var));	
		return true;
	}
	return is_nested(var) && try_split(unpack_nested(var));
}

void reduce(snailfish_number& s)
{
	while(try_explode(s) || try_split(s));
}

snailfish_number operator+(snailfish_number lhs, snailfish_number rhs)
{
	snailfish_number result;
	result.a = std::make_unique<snailfish_number>(std::move(lhs));
	result.b = std::make_unique<snailfish_number>(std::move(rhs));
	reduce(result);
	return result;
}

long magnitude(const snailfish_number& s);
long magnitude(const snailfish_number::variant& s)
{
	if(std::holds_alternative<int>(s)) return std::get<int>(s);
	return magnitude(unpack_nested(s));
}

long magnitude(const snailfish_number& s)
{
	return 3*magnitude(s.a)+2*magnitude(s.b);
}

int main(int argc, char* argv[])
{
	std::vector<snailfish_number> nums(std::istream_iterator<snailfish_number>(std::cin),{});
	
	long max = std::numeric_limits<long>::min();
	for(std::size_t i=0;i<nums.size();++i)
	{
		for(std::size_t j=i+1;j<nums.size();++j)
		{
			max = std::max({max,magnitude(nums[i]+nums[j]),magnitude(nums[j]+nums[i])});
		}
	}
	std::cout<<max;
}
