#include <iostream>
#include <string>
#include <vector>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

auto get_input()
{
	std::vector<int> r;
	std::string ln;
	while(std::getline(std::cin, ln))
		r.emplace_back(sv_to_t<int>(ln));
	return r;
}

int pt1(auto in)
{
	timer t("p1");
	int ip = 0;
	int steps = 0;
	while(ip >= 0 && ip < in.size())
	{
		auto j = in[ip];
		++in[ip];
		ip += j;
		++steps;
	}
	return steps;
}

int pt2(auto in)
{
	timer t("p2");
	int ip = 0;
	int steps = 0;
	while(ip >= 0 && ip < in.size())
	{
		auto j = in[ip];
		if(j > 2)
			--in[ip];
		else
			++in[ip];
		ip += j;
		++steps;
	}
	return steps;
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
