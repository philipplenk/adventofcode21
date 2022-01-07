#include <advent.hpp>

#include <iostream>
#include <string>
#include <vector>

using namespace advent;

int main(int argc, char* argv[])
{
	std::string skip;
	char skip_c;
	
	point2d area_min, area_max;
	std::cin>>skip>>skip>>std::ws>>skip_c>>skip_c>>area_min.x>>skip_c>>skip_c>>area_max.x>>skip_c>>skip_c>>skip_c>>area_min.y>>skip_c>>skip_c>>area_max.y;
		
	std::vector<int> candidate_x;
	for(int dx=1;dx<=area_max.x;++dx)
	{
		int x=0, step_dx=dx;
		bool inside = false;
		while(x<=area_max.x && step_dx)
		{
			if(x>=area_min.x && x<=area_max.x) inside = true;
			x+=step_dx;
			step_dx = std::max(0,step_dx-1);
		}
		if(inside) candidate_x.push_back(dx);
	}
	int count = 0;
	for(int dx: candidate_x)
	{
		for(int dy=-2000;dy<2000;++dy)
		{
			int x = 0, y = 0, max_y_step = 0;
			int step_dx = dx, step_dy = dy;
			bool inside = false, below_y = false;
			while(x<=area_max.x && y>=area_min.y)
			{
				inside|=(x>=area_min.x && x<=area_max.x && y>=area_min.y && y<=area_max.y);
				x+=step_dx;
				y+=step_dy;
				max_y_step = std::max(max_y_step,y);
				step_dx-=1;
				step_dx=std::max(0,step_dx);
				step_dy-=1;
				
				if(step_dx==0 && x<area_min.x) break;
				below_y|=y<=area_max.y;
			}
			count+=inside;
		}
	}
	
	std::cout<<count;
}
