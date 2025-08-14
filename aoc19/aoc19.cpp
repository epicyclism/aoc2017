#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
//#include <numeric>
//#include <ranges>

#include <fmt/format.h>

//#include "ctre_inc.h"
#include "timer.h"

auto get_input()
{
	std::vector<char> v;
	size_t stride;
	std::string ln;
	std::getline(std::cin, ln);
	stride = ln.size();
	v.append_range(ln);
	while(std::getline(std::cin, ln))
		v.append_range(ln);
	
	return std::make_pair(v, stride);
}

enum class dir{ N, E, S, W};

struct cursor
{
	std::vector<char> const& g_;
	size_t stride_;
	dir cur_dir_;
	size_t here_;

	size_t step()
	{
		if(g_[here_] == '+')
		{
			if(cur_dir_ == dir::N || cur_dir_ == dir::S)
			{
				if(g_[here_ + 1] == '-' || ::isalpha(g_[here_ + 1]))
				{
					cur_dir_ = dir::E;
					++here_;
				}
				else
				{
					cur_dir_ = dir::W;
					--here_;
				}
			}
			else
			{
				if(g_[here_ + stride_] == '|' || ::isalpha(g_[here_ + stride_]))
				{
					cur_dir_ = dir::S;
					here_ += stride_;
				}
				else
				{
					cur_dir_ = dir::N;
					here_ -= stride_;
				}
			}
		}
		else
		{
			switch(cur_dir_)
			{
				case dir::N:
					here_ -= stride_;
					break;
				case dir::E:
					++here_;
					break;
				case dir::S:
					here_ += stride_;
					break;
				case dir::W:
					--here_;
					break;
			}
		}
		return g_[here_] == ' ' ? -1 : here_;
	}
};

auto pt12(auto const& in)
{
	timer t("p12");
	size_t start = std::distance(in.first.begin(), std::ranges::find(in.first, '|'));
	cursor c(in.first, in.second, dir::S, start);
	std::string r;
	int cnt = 1;
	while(c.step() != -1)
	{
		++cnt;
		if(std::isalpha(in.first[c.here_]))
			r.push_back(in.first[c.here_]);
	}
	return std::make_pair(r, cnt);
}

int main()
{
	auto in = get_input();
	auto[p1, p2]= pt12(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
