#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

auto get_input()
{
	std::vector<std::pair<int, int>> v;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(auto[m, d, r] = ctre::match<"(\\d+): (\\d+)">(ln); m)
			v.emplace_back(d.to_number<int>(), r.to_number<int>());
		else
			fmt::println("parse fail at {}", ln);
	}
	return v;
}

int64_t pt1(auto const& in)
{
	timer t("p1");
	return std::reduce(in.begin(), in.end(), 0, [](auto l, auto &r){ return r.first % (2 * r.second - 2) == 0 ? l + r.first * r.second: l;});
}

int64_t pt2(auto const& in)
{
	timer t("p2");
	int delay = 1;
	while(1)
	{
		if(std::reduce(in.begin(), in.end(), 0, [&](auto l, auto &r){ return (r.first + delay) % (2 * r.second - 2) == 0 ? l + 1: l;}) == 0)
			break;
		++delay;
	}
	return delay;
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
