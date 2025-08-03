#include <iostream>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "timer.h"

auto get_input()
{
	std::string ln;
	std::getline(std::cin, ln);
	return ln;
}

int pt1(auto in)
{
	timer t("p1");
	in.push_back(in[0]);
	int cnt = 0;
	for(int n = 0; n < in.size() - 1; ++n)
		cnt += (in[n] == in[n + 1]) ? in[n] - '0' : 0;
	return cnt;
}

int pt2(auto const& in)
{
	timer t("p2");
	auto sz  = in.size();
	auto stp = in.size() / 2;
	int cnt = 0;
	for(int n = 0; n < in.size(); ++n)
		cnt += (in[n] == in[(n + stp) % sz]) ? in[n] - '0' : 0;

	return cnt;
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
