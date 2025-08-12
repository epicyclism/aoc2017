#include <iostream>
#include <string>
#include <vector>

#include <fmt/format.h>

#include "timer.h"
#include "ctre_inc.h"

int pt1(auto in)
{
	timer t("p1");
	std::vector<int> v;
	v.reserve(2048);
	v.emplace_back(0);
	int cp = 0;
	for(int n = 1; n < 2018; ++n)
	{
		cp += in;
		cp %= v.size();
		v.insert(v.begin() + cp + 1, n);
		++cp;
	}
	return v[cp + 1];
}

int pt2(auto in)
{
	timer t("p2");
	int cp = 0;
	int n1 = 0;
	for(int n = 1; n < 50000001; ++n)
	{
		cp += in;
		cp %= n;
		if(cp == 0)
			n1 = n;
		++cp;
	}
	return n1;
}

int main(int ac, char** av)
{
	auto in = 335;
	if(ac > 1)
		in = sv_to_t<int>(av[1]);
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
