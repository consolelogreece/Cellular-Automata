#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <map>

// rulesets, maps a name to born rules[0], survive rules[1]
std::map<std::string, std::vector<std::string>> rulesets = {
	{"Conway's", { "3", "23"}},
	{"Replicator", {"1357", "1357"}},
	{"Seeds", {"2", ""}},
	{"B25/S4", {"25", "4"}},
	{"Life without death", {"3", "012345678"}},
	{"34 Life", {"34", "34"}},
	{"Diamoeba", {"35678", "5678"}},
	{"2x2", {"36", "125"}},
	{"Day & Night", {"3678", "34678"}},
	{"Morley", {"368", "245"}},
	{"Anneal", {"4678", "35678"}}
};

class CellularAutomata : public olc::PixelGameEngine
{
	public:
		CellularAutomata()
		{
			// Name your application
			sAppName = "Cellular Automata";
		}
	public:
		int w = 576;
		int h = 324;
		int seedWeight = 20;
		std::vector<int> born;
		std::vector<int> survive;
		bool** currentState;
		bool** nextState;
		float timeSinceUpdate;
		float timeBeforeUpdate = 0.03f;

		void GenerateRules()
		{
			std::vector<std::string> ruleset = rulesets["Life without death"];

			std::string bornString = ruleset[0];

			std::string surviveString = ruleset[1];

			for (int i = 0; i < bornString.length(); i++)
			{
				born.insert(born.end(), bornString[i] - '0');
			}

			for (int i = 0; i < surviveString.length(); i++)
			{
				survive.insert(survive.end(), surviveString[i] - '0');
			}
		}

		bool OnUserCreate() override
		{
			nextState = new bool*[w + 2];
			currentState = new bool* [w + 2];

			for (int i = 0; i < w + 2; i++) {
				nextState[i] = new bool[h + 2]{ true };
				currentState[i] = new bool[h + 2]{ true };
			}

			// seed, ignore outer boundary as they wont be drawn for optimaztion purposes.
			for (int x = 1; x < w + 2; x++)
			{
				for (int y = 1; y < h + 2; y++)
				{
					int r = rand() % seedWeight;
					if (r == 0) currentState[x][y] = true;
					else currentState[x][y] = false;
				}
			}

			GenerateRules();

			return true;
		}
	
		bool OnUserUpdate(float fElapsedTime) override
		{		
			// ensure updates do not run too frequently.
			timeSinceUpdate += fElapsedTime;
			if (timeSinceUpdate < timeBeforeUpdate)
			{
				return true;
			}
			else
			{
				timeSinceUpdate = 0;
			}

			for (int x = 2; x < w; x++)
			{
				for (int y = 2; y < h; y++)
				{
					bool alive = currentState[x][y];

					if (alive) Draw(x, y, olc::WHITE);
					else Draw(x, y, olc::BLACK);

					int count = 0;

					count += currentState[x - 1][y - 1]; // above left
					count += currentState[x][y - 1]; // above middle
					count += currentState[x + 1][y - 1]; // above right
					count += currentState[x - 1][y]; // left
					count += currentState[x + 1][y]; // right
					count += currentState[x - 1][y + 1]; // below left
					count += currentState[x][y + 1]; // below
					count += currentState[x + 1][y + 1]; // below right

					bool shouldLive = false;

					if (alive)
					{
						for (int n : survive)
						{
							if (count == n)
							{
								shouldLive = true;
								break;
							}
						}
					}
					else
					{
						for (int n : born)
						{
							if (count == n)
							{
								shouldLive = true;
								break;
							}
						}
					}

					nextState[x][y] = shouldLive;
				}
			}

			bool** plchldr = currentState;
			currentState = nextState;
			nextState = plchldr;

			return true;
		}
};

int main()
{
	CellularAutomata demo;
	if (demo.Construct(576, 324, 2, 2))
		demo.Start();
	return 0;
}