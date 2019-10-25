// CandyCrunching.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Author: Kevin Kulp (Thanks to Lewis Milton for helping me learn optimization tricks!)
// Date: 10/24/2019

#include "pch.h"
#include <iostream>
#include <vector>
#include <chrono>

#include "CandyBranch.h"

/*
Welcome to CandyCrunching!
This passion project is an (optional) extension of a homework assignment that simulates an idle game.
The objective is to maximize the passive income of candies per second (CPS) after 1800 seconds of simulation.
Since this iteration of the program functions through brute force, finding a total solution is not feasible. (It would require creating roughly 6^250 nodes)
This iteration can solve for smaller values, like 300 seconds; the next iteration aims to solve this problem heuristically through branch pruning.

In this game, six upgrades can be purchased:
Lollipops:      1 CPS   | 150 base cost
JollyRanchers:  4 CPS   | 400 base cost
Jawbreakers:    12 CPS  | 1000 base cost
Ice Cream:      20 CPS  | 2000 base cost
Cotton Candy:   150 CPS | 10000 base cost
Tootsie Rolls:  175 CPS | 20000 base cost
Each time an upgrade is purchased, the upgrade cost increases by 10%.

NOTE: Run this program in Release mode; it is roughly 40x faster to do so.
*/

int main()
{
	// Try changing me! (Note: >275 ticks takes 5+ minutes in Release mode with a good CPU; recommend range is 25-250)
	const int SECONDS_TO_SIMULATE = 250;

	auto start = std::chrono::high_resolution_clock::now();
	CandyBranch candyTree;
	candyTree.Start(SECONDS_TO_SIMULATE);
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "Solution found in " << std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count() << " seconds." << std::endl;
}

