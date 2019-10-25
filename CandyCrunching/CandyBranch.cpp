// Author: Kevin Kulp, help from Lewis Milton
// Date: 9/8/2019
// Objective: Solve the optimal path for Game Balance's idle farmer candy game
#include "pch.h"
#include "CandyBranch.h"

#include <string>
#include <iostream>

#include <stdio.h>
#include <math.h>

using namespace std::string_literals;

// Array index references
// 0: lollipops
// 1: jollyRanchers
// 2: jawbreakers
// 3: iceCream
// 4: cottonCandy
// 5: tootsieRolls

// Root case of the tree; prints the optimal output after calculated
CandyBranch::CandyBranch()
{
}

// Recursive case of the tree; a branch that inherits from a parent branch
CandyBranch::CandyBranch(CandyBranch const &old, upgrade newPath)
	: path{ newPath }
	, resources{ old.resources }
	, purchases{ old.purchases }
	, secondsLeft{ old.secondsLeft }
	, nodeLog{ old.nodeLog }
	, bestIncome{ old.bestIncome }
{
	income = CalcIncome();
}

// CALL TO START
void CandyBranch::Start(int _secondsLeft)
{
	secondsLeft = _secondsLeft;
	bestIncome = new int(0);
	CandyBranch bestBranch = FindBestBranch();
	bestBranch.PrintLog();
	// bestBranch.PrintCost();
	delete bestIncome;
}

// Calculates the next layer of recursion
CandyBranch CandyBranch::FindBestBranch()
{
	// Calculate a list of the next possible purchases and return the branch with the highest value
	return OptimalNode(Branch());
}

// Recalculates the current income, skips forward the necessary number of seconds and buys the upgrade
// Returns true if the branch makes a purchase, otherwise returns false
bool CandyBranch::SimulateBranch()
{
	price = (int)(basePrices[path] * memopow(purchases[path]));
	income = CalcIncome();

	// Skip forward the number of seconds necessary (rounded up)
	// Formula is ceil((price - resources) / income), optimizied for efficiency via integer division truncating
	secondsToUpgrade = (int)((price - resources + income - 1) / income);
	
	// Ends the branch simulation early if it wouldn't be able to afford the desired upgrade (base case)
	if (secondsToUpgrade > secondsLeft)
	{
		resources += secondsLeft * income;
		secondsLeft -= secondsLeft;
		secondsToUpgrade -= secondsLeft;
		LogTerminator();
		return false;
	}
	
	resources += secondsToUpgrade * income;
	secondsLeft -= secondsToUpgrade;
	resources -= price;
	purchases[path]++;
	LogPurchase();

	if (income > *bestIncome)
	{
		*bestIncome = income;
		PrintLogInProgress();
	}

	return true;
}

// Used to calculate the current income rate (does not set the value)
int CandyBranch::CalcIncome()
{
	return 10
		+ (1 * purchases[0])
		+ (4 * purchases[1])
		+ (12 * purchases[2])
		+ (20 * purchases[3])
		+ (150 * purchases[4])
		+ (175 * purchases[5]);
}

// Calculate a list of child nodes; if child nodes have children, recursively calculate them first
auto CandyBranch::Branch()
-> std::array<std::optional<CandyBranch>, NUM_OF_UPGRADES>
{
	std::array<std::optional<CandyBranch>, NUM_OF_UPGRADES> branchList;
	CandyBranch tempBranch;
	for (int i = 0; i < NUM_OF_UPGRADES; i++)
	{
		tempBranch = CandyBranch(std::cref(*this), static_cast<upgrade>(i));
		// Returns true if the branch makes a purchase, otherwise returns false
		if (tempBranch.SimulateBranch())
		{
			// If the branch did made a purchase, return the optimal node of its child nodes
			branchList[i].emplace(tempBranch.FindBestBranch());
		}
		// else, No purchase was made, so no higher income would be generated; parent node in OptimalNode() already generates resources
	}
	return branchList;
}

// Calculates and returns the optimal node in a given list
CandyBranch CandyBranch::OptimalNode(std::array<std::optional<CandyBranch>, NUM_OF_UPGRADES> childNodes)
{
	// If there's any time remaining, add it on to the node's resource generation calculations
	resources += secondsLeft * income;
	secondsLeft -= secondsLeft;
	secondsToUpgrade -= secondsLeft;
	LogTerminator();

	int bestNode = -1;
	for(int i = 0; i < childNodes.size(); ++i)
	{
		std::optional<CandyBranch> &tempNode = childNodes[i];
		if (!tempNode) continue;

		// Code used for determining the optimal node
		if (false) // TODO: Add condition for buying the upgrade that rewards +10% credit
		{
		}
		// If a bestNode is a child node, compare incomes
		else if (bestNode != -1) 
		{
			if (tempNode->income > childNodes[bestNode]->income)
			{
				bestNode = i;
			}
		}
		// If the parent node is still the best node, compare to the parent's income
		else if (tempNode->income > this->income)
		{
			bestNode = i;
		}
	}
	if (bestNode == -1)
	{
		return *this;
	}
	return std::move(*childNodes[bestNode]);
}

// Outputs the optimal node's log to the screen
void CandyBranch::PrintLog()
{
	std::cout << nodeLog << std::endl;
}

// Outputs the current branch status if it's the best branch found so far
void CandyBranch::PrintLogInProgress()
{
	std::cout << std::endl << "New best case found!" << std::endl;
	std::cout << nodeLog << "Income: "s + std::to_string(income) + ", Resources: "s + std::to_string(resources) + ", Seconds left: "s + std::to_string(secondsLeft) + "\n----------\n"s;
}

// Outputs the current cost of all current branches to the console
void CandyBranch::PrintCost()
{
	std::cout << "----------" << std::endl;
	std::cout << "Lollipop (1CPS): " << (int)(basePrices[0] * memopow(purchases[0])) << std::endl;
	std::cout << "Jolly Ranchers (4CPS): " << (int)(basePrices[1] * memopow(purchases[1])) << std::endl;
	std::cout << "Jawbreakers (12CPS): " << (int)(basePrices[2] * memopow(purchases[2])) << std::endl;
	std::cout << "Ice Cream (20CPS): " << (int)(basePrices[3] * memopow(purchases[3])) << std::endl;
	std::cout << "Cotton Candy (150CPS): " << (int)(basePrices[4] * memopow(purchases[4])) << std::endl;
	std::cout << "Tootsie Rolls (175CPS): " << (int)(basePrices[5] * memopow(purchases[5])) << std::endl;
	std::cout << "----------" << std::endl;
}

// Helper method - adds a message to the log for the current purchase
void CandyBranch::LogPurchase()
{
	// NOTE: log purchase message will vary depending on where it's inserted in code
	// (e.g. adding it after purchases[path]++ will have a higher price if outputted}
	nodeLog += "Bought "s + EnumToString() + " at "s + std::to_string(secondsLeft) + " seconds left\n"s;
}

// Helper method - appends the text to end a log when no more purchases can be made
void CandyBranch::LogTerminator()
{
	nodeLog += "FINAL | Income: "s + std::to_string(income) + ", Resources: "s + std::to_string(resources) + "\n----------\n"s;
}

// Helper method - returns the current upgrade path in string form;
std::string CandyBranch::EnumToString()
{
	switch (path)
	{
	case 0:
		return "lollipops";
	case 1:
		return "jollyRanchers";
	case 2:
		return "jawbreakers";
	case 3:
		return "iceCream";
	case 4:
		return "cottonCandy";
	case 5:
		return "tootsieRolls";
	default:
		return "??????";
	}
}