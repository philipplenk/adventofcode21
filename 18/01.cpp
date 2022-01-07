#include <iostream>
#include <memory>
#include <variant>

struct snailfish_number
{
	std::variant<int,std::unique_ptr<snailfish_number>> a,b;
};

std::istream& operator>>(std::istream& in, snailfish_number& s)
{
	char skip;
	in>>skip;
	
	if(in.peek()=='[')
	{
		s.a = std::make_unique<snailfish_number>();
		in>>*std::get<std::unique_ptr<snailfish_number>>(s.a);
	}
	else
	{
		int value;
		in>>value;
		s.a = value;
	} 
	
	in>>skip;
	
	if(in.peek()=='[')
	{
		s.b = std::make_unique<snailfish_number>();
		in>>*std::get<std::unique_ptr<snailfish_number>>(s.b);
	}
	else
	{
		int value;
		in>>value;
		s.b = value;
	} 
	
	in>>skip;
	return in;
}

std::ostream& operator<<(std::ostream& out, const snailfish_number& s)
{
	out<<'[';
	std::visit([&](const auto& v)
	{
		if constexpr(std::is_same_v<std::decay_t<decltype(v)>,int>)
		{
			out<<v;
		}
		else if constexpr(std::is_same_v<std::decay_t<decltype(v)>,std::unique_ptr<snailfish_number>>)
			out<<*v;
	},s.a);
	out<<',';
	std::visit([&](const auto& v)
	{
		if constexpr(std::is_same_v<std::decay_t<decltype(v)>,int>)
		{
			out<<v;
		}
		else if constexpr(std::is_same_v<std::decay_t<decltype(v)>,std::unique_ptr<snailfish_number>>)
			out<<*v;
	},s.b);
	out<<']';
	return out;
}

bool is_number_pair(const snailfish_number& s)
{
	return std::holds_alternative<int>(s.a) && std::holds_alternative<int>(s.b);
}

bool is_number_pair(const std::variant<int,std::unique_ptr<snailfish_number>>& var)
{
	return std::holds_alternative<std::unique_ptr<snailfish_number>>(var) && is_number_pair(*std::get<std::unique_ptr<snailfish_number>>(var));
}

struct reduce_action
{
	int add_left, add_right;
	bool was_reduction;
};

bool try_add_right(snailfish_number& s, int val)
{
	if(std::holds_alternative<int>(s.a))
	{
		std::get<int>(s.a)+=val;
		return true;
	}
	else if(try_add_right(*std::get<std::unique_ptr<snailfish_number>>(s.a),val))
		return true;
		
	if(std::holds_alternative<int>(s.b))
	{
		std::get<int>(s.b)+=val;
		return true;
	}
	else if(try_add_right(*std::get<std::unique_ptr<snailfish_number>>(s.b),val))
		return true;
	
	return false;
}

bool try_add_right(std::variant<int,std::unique_ptr<snailfish_number>>& var, int val)
{
	if(std::holds_alternative<int>(var))
	{
		std::get<int>(var)+=val;
		return true;
	}
	return try_add_right(*std::get<std::unique_ptr<snailfish_number>>(var),val);
}

bool try_add_left(snailfish_number& s, int val)
{
	if(std::holds_alternative<int>(s.b))
	{
		std::get<int>(s.b)+=val;
		return true;
	}
	else if(try_add_left(*std::get<std::unique_ptr<snailfish_number>>(s.b),val))
		return true;
		
	if(std::holds_alternative<int>(s.a))
	{
		std::get<int>(s.a)+=val;
		return true;
	}
	else if(try_add_left(*std::get<std::unique_ptr<snailfish_number>>(s.a),val))
		return true;
	
	return false;
}

bool try_add_left(std::variant<int,std::unique_ptr<snailfish_number>>& var, int val)
{
	if(std::holds_alternative<int>(var))
	{
		std::get<int>(var)+=val;
		return true;
	}
	return try_add_left(*std::get<std::unique_ptr<snailfish_number>>(var),val);
}

reduce_action reduce(snailfish_number& s, int nesting)
{
	if(nesting==3 && is_number_pair(s.a))
	{
		const auto& num = *std::get<std::unique_ptr<snailfish_number>>(s.a);
		
		auto to_add_right = std::get<int>(num.b);
		reduce_action ret_val{std::get<int>(num.a),try_add_right(s.b,to_add_right)?0:to_add_right,true};
		s.a = 0;
		return ret_val;
	}
	
	if(nesting==3 && is_number_pair(s.b))
	{
		const auto& num = *std::get<std::unique_ptr<snailfish_number>>(s.b);
		auto to_add_left = std::get<int>(num.a);
		reduce_action ret_val{try_add_left(s.a,to_add_left)?0:to_add_left,std::get<int>(num.b),true};
		s.b = 0;
		return ret_val;
	}
	
	if(std::holds_alternative<std::unique_ptr<snailfish_number>>(s.a))
	{
		auto recurse_result = reduce(*std::get<std::unique_ptr<snailfish_number>>(s.a),nesting+1);
		if(recurse_result.was_reduction)
		{
			if(recurse_result.add_right)
			{
				if(std::holds_alternative<int>(s.b))
				{
					std::get<int>(s.b)+=recurse_result.add_right;
					recurse_result.add_right = 0;
				}
				else if(try_add_right(*std::get<std::unique_ptr<snailfish_number>>(s.b),recurse_result.add_right))
				{
					recurse_result.add_right = 0;
				}
			}
			
			return recurse_result;
		}
	}
	
	if(std::holds_alternative<std::unique_ptr<snailfish_number>>(s.b))
	{
		auto recurse_result = reduce(*std::get<std::unique_ptr<snailfish_number>>(s.b),nesting+1);
		if(recurse_result.was_reduction)
		{
			if(recurse_result.add_left)
			{
				if(std::holds_alternative<int>(s.a))
				{
					std::get<int>(s.a)+=recurse_result.add_left;
					recurse_result.add_left = 0;
				}
				else if(try_add_left(*std::get<std::unique_ptr<snailfish_number>>(s.a),recurse_result.add_left))
				{
					recurse_result.add_left = 0;
				}
			}
			
			return recurse_result;
		}
	}
	
	return {0,0,false};
}

bool try_explode(snailfish_number& s)
{
	return reduce(s,0).was_reduction;
}

bool try_split(snailfish_number& s)
{
	if(std::holds_alternative<int>(s.a) && std::get<int>(s.a)>=10)
	{
		const auto old_value = std::get<int>(s.a);
		auto new_pair = std::make_unique<snailfish_number>();
		new_pair->a = old_value/2;
		new_pair->b = (old_value+1)/2;
		s.a = std::move(new_pair);
		
		return true;
	}
	else if(std::holds_alternative<std::unique_ptr<snailfish_number>>(s.a) && try_split(*std::get<std::unique_ptr<snailfish_number>>(s.a)))
		return true;
	else if(std::holds_alternative<int>(s.b) && std::get<int>(s.b)>=10)
	{
		const auto old_value = std::get<int>(s.b);
		auto new_pair = std::make_unique<snailfish_number>();
		new_pair->a = old_value/2;
		new_pair->b = (old_value+1)/2;
		s.b = std::move(new_pair);
		
		return true;
	}
	else if(std::holds_alternative<std::unique_ptr<snailfish_number>>(s.b) && try_split(*std::get<std::unique_ptr<snailfish_number>>(s.b)))
		return true;
	
	return false;
}

void reduce(snailfish_number& s)
{
	bool done_reduction = false;
	do
	{
		done_reduction = try_explode(s) || try_split(s);
	}
	while(done_reduction);
}

long magnitude(const snailfish_number& s);
long magnitude(const std::variant<int,std::unique_ptr<snailfish_number>>& s)
{
	if(std::holds_alternative<int>(s)) return std::get<int>(s);
	return magnitude(*std::get<std::unique_ptr<snailfish_number>>(s));
}

long magnitude(const snailfish_number& s)
{
	return 3*magnitude(s.a)+2*magnitude(s.b);
}

int main(int argc, char* argv[])
{
	snailfish_number lhs, rhs;
	std::cin>>lhs;
	
	while(std::cin>>rhs)
	{
		snailfish_number next;
		next.a = std::make_unique<snailfish_number>(std::move(lhs));
		next.b = std::make_unique<snailfish_number>(std::move(rhs));
		std::cerr<<next<<" reduced to ";
		reduce(next);
		std::cerr<<next<<'\n';
		lhs = std::move(next);
	}
	
	std::cerr<<lhs<<'\n';
	std::cout<<magnitude(lhs);
}
