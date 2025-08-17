#include <iostream>
#include <string>
#include <vector>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

auto get_input()
{
	std::vector<std::pair<int, int>> v;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(auto[m, l, r] = ctre::match<"(\\d+)/(\\d+)">(ln); m)
			v.emplace_back(l.to_number<int>(), r.to_number<int>());
		else
			fmt::println("parse fail : {}", ln);
	}
	return v;
}

using graph_t = std::vector<std::vector<size_t>>;

void add_edge(graph_t& g, int from, int to )
{
    g[from].emplace_back(to);
}

void add_edge_undirected(graph_t& g, int from, int to )
{
    add_edge(g, from, to);
    add_edge(g, to, from);
}
// each pipe has two vertices, we make a connection between them, and then from each to every other matching
// pipe end. vertex ids for pipe one are 0, 1 etc.
// return graph and list of vertex to value.
//
auto build_graph(auto const& in)
{
	std::vector<int> val(in.size () * 2);
	graph_t g(in.size() * 2); // each element has two vertices...
	for(auto p = 0; p < in.size(); ++p)
	{
		add_edge_undirected(g, 2 * p, 2 * p + 1);
		val[2*p] = in[p].first;
		val[2*p+1] = in[p].second;
		for(auto t = 0; t < in.size(); ++t)
		{
			if(p != t)
			{
				if(in[p].first == in[t].first)
					add_edge(g, p*2, t*2);
				if(in[p].first == in[t].second)
					add_edge(g, p*2, t*2+1);
				if(in[p].second == in[t].first)
					add_edge(g, p*2+1, t*2);
				if(in[p].second == in[t].second)
					add_edge(g, p*2+1, t*2+1);
			}
		}
	}
	return std::make_pair(g, val);
}

int other(int e)
{
	if( e & 1)
		return e - 1;
	return e + 1;
}

void all_paths_wkr(graph_t const& g, std::vector<int> const& val, int n, std::vector<bool>& visited, int strength, int& max_s, int depth, int& max_d, int& max_sd)
{
	int cnt = 0;
	for(auto e: g[n])
	{
		if(!visited[e] && !visited[other(e)])
		{
			++cnt;
			visited[e] = true;
			visited[other(e)] = true;
			all_paths_wkr(g, val, other(e), visited, strength + val[e] + val[other(e)], max_s, depth + 1, max_d, max_sd);
			visited[e] = false;
			visited[other(e)] = false;
		}
	}
	if(cnt == 0)
	{
		if(strength > max_s)
			max_s = strength;
		if(depth == max_d && strength > max_sd)
			max_sd = strength;
		else
		if(depth > max_d)
		{
			max_sd = strength;
			max_d = depth;
		}
	}
}

void all_paths(graph_t const& g, std::vector<int> const& val, int n, int& max_s, int& max_depth, int& max_sd)
{
	std::vector<bool> visited(g.size());
	visited[n] = true;
	visited[other(n)] = true;
	all_paths_wkr(g, val, other(n), visited, val[n] + val[other(n)], max_s, 1, max_depth, max_sd );
}

auto pt12(auto const& in)
{
	timer t("p12");
	auto[g, pt] = build_graph(in);
	int max_strength = 0;
	int max_depth = 0;
	int max_sd = 0;
	for(int s = 0; s < pt.size(); ++s)
	{
		if(pt[s] == 0)
			all_paths(g, pt, s, max_strength, max_depth, max_sd);
	}
	return std::make_pair(max_strength, max_sd);
}

// p2 1854 too high
//.   1849 too high
//

int main()
{
	auto in = get_input();
	auto[p1, p2] = pt12(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
