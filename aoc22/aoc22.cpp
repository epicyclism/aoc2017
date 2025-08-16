#include <iostream>
#include <string>
#include <map>

#include <fmt/format.h>

#include "bs_2d.h"
#include "timer.h"

auto get_input()
{
	bs_2d_sym_t<512, 512> bs;
	std::map<std::pair<int, int>, char> ms;
	std::string ln;
	std::getline(std::cin, ln);
	int dim = ln.size();
	int x = -dim / 2;
	int y = dim / 2;
	do
	{
		for(auto c: ln)
		{
			if(c == '#')
			{
				bs.set(x, y);
				ms.emplace(std::make_pair(x, y), 'I');
			}
			++x;
		}
		--y;
		x = -dim / 2;
	} while (std::getline(std::cin, ln));
	
	return std::make_pair(bs, ms);
}

// UP   RIGHT  DOWN    LEFT
// 0, 1  1, 0.  0, -1.  -1, 0
std::pair<int, int> move_xy[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
int left(int d)
{
	--d;
	if(d < 0)
		d = 3;
	return d;
}

int right(int d)
{
	++d;
	if(d > 3)
		d = 0;
	return d;
}

int reverse(int d)
{
	d += 2;
	if( d > 3)
		d -= 4;
	return d;
}

int pt1(auto in)
{
	timer t("p1");
	int cnt = 0;
	int d = 0; // UP	
	int x = 0;
	int y = 0;
	for(int b = 0; b < 10000; ++b)
	{
		if(in.test(x, y)) // infected
		{
			// clean
			in.reset(x, y);
			// turn right
			d = right(d);
		}
		else
		{
			// infect
			in.set(x, y);
			++cnt;
			// turn left
			d = left(d);
		}
		// step
		x += move_xy[d].first;
		y += move_xy[d].second;
	}

	return cnt;
}

int pt2(auto in)
{
	timer t("p2");
	int cnt = 0;
	int d = 0; // UP	
	int x = 0;
	int y = 0;
	for(int b = 0; b < 10000000; ++b)
	{
		if(in.contains({x, y}))
		{
			switch(in[{x, y}])
			{
				case 'W': // weak -> infected
					in[{x, y}] = 'I';
					++cnt;
					break;
				case 'I':  // infected -> flagged
					in[{x, y}] = 'F';
					d = right(d);
					break;
				case 'F':
					in.erase({x, y}); // flagged -> clean
					d = reverse(d);
					break;
			}
		}
		else
		{
			// clear -> weak
			in[{x, y}] = 'W';
			d = left(d);
		}
		// step
		x += move_xy[d].first;
		y += move_xy[d].second;
	}

	return cnt;
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in.first);
	auto p2 = pt2(in.second);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
