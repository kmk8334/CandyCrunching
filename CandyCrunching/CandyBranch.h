#pragma once
#include <string>
#include <array>
#include <optional>

class CandyBranch
{
public:
	// Class enum
	enum upgrade { 
		lollipops = 0, 
		jollyRanchers = 1, 
		jawbreakers= 2, 
		iceCream = 3, 
		cottonCandy = 4, 
		tootsieRolls = 5 
	};
	const static int NUM_OF_UPGRADES = 6;
	static constexpr int basePrices[NUM_OF_UPGRADES] = { 150, 400, 1000, 2000, 10000, 20000 };
	static float memopow(int x)
	{
		static std::array<float, 1000> cache;
		static std::array<bool, 1000> check;
		if (cache.size() < x) throw std::runtime_error{ "cache too small" };
		if (!check[x])
		{
			check[x] = true;
			return cache[x] = std::pow(1.1f, x);
		}
		return cache[x];
	}

	// Constructor, recursive constructor, and deconstructor
	CandyBranch();
	CandyBranch(CandyBranch const &, upgrade);
	CandyBranch(CandyBranch const &) = default;
	CandyBranch(CandyBranch &&) noexcept = default;
	CandyBranch &operator=(CandyBranch &&) noexcept = default;
	CandyBranch &operator=(CandyBranch const &) = default;
	~CandyBranch() noexcept = default;

	// Recursive functions
	void Start(int _secondsLeft);
	CandyBranch FindBestBranch();
	bool SimulateBranch();
	int CalcIncome();
	std::array<std::optional<CandyBranch>, NUM_OF_UPGRADES> Branch();
	CandyBranch OptimalNode(std::array<std::optional<CandyBranch>, NUM_OF_UPGRADES>);
	
	// Console functions
	void PrintLog();
	void PrintLogInProgress();
	void PrintCost();

	// Public variables
	int resources = 0;
	int income = 10;
	int* bestIncome;
	std::array<int, NUM_OF_UPGRADES> purchases = { 0 };
	int secondsLeft;
	
	// Debug log values
	std::string nodeLog = "----------\n";

private:
	// Variables for determining the delay until the next upgrade
	int secondsToUpgrade;
	int price;
	upgrade path;
	
	// Helper methods
	void LogPurchase();
	void LogTerminator();
	std::string EnumToString();
};

