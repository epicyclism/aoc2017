#include <iostream>
#include <string>
#include <vector>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

auto get_input()
{
	std::vector<std::vector<int>> al;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		bool one = true;
		for(auto v : ctre::search_all<"(\\d+)">(ln))
		{
			if(one)
			{
				one = false;
				al.emplace_back(std::vector<int>());
			}
			else
				al.back().emplace_back(v.to_number<int>());
		}
	}
	return al;
}

void count_connected(std::vector<std::vector<int>> const& al, int n, std::vector<bool>& visited, int& cnt)
{
	for(auto e: al[n])
	{
		if(!visited[e])
		{
			visited[e] = true;
			++cnt;
			count_connected(al, e, visited, cnt);
		}
	}
}

auto pt12(auto const& in)
{
	timer t("p12");
	int p1 = 0;
	int p2 = 1;
	std::vector<bool> visited(in.size());
	count_connected(in, 0, visited, p1);
	int tmp = 0;
	int n = 1;
	while(n != visited.size())
	{
		if(!visited[n])
		{
			++p2;
			count_connected(in, n, visited, tmp);
		}
		++n;
	}
	return std::make_pair(p1, p2);
}

int main()
{
	auto in = get_input();
	auto[p1, p2] = pt12(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}