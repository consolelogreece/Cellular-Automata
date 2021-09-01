#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class CellularAutomata : public olc::PixelGameEngine
{
	public:
		CellularAutomata()
		{
			// Name your application
			sAppName = "Cellular Automata";
		}
	public:
		bool** currentState;
		bool** nextState;
		int w = 1920;
		int h = 1080;

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
					int  r = rand() % (10);
					if (r == 5) currentState[x][y] = true;
					else currentState[x][y] = false;
				}
			}

			return true;
		}
	
		bool OnUserUpdate(float fElapsedTime) override
		{		
			for (int x = 2; x < w; x++)
			{
				for (int y = 2; y < h; y++)
				{
					bool alive = currentState[x][y];

					if (alive) Draw(x, y, olc::WHITE);
					else Draw(x, y, olc::BLACK);

					int count = 0;

					if (currentState[x - 1][y - 1]) count++;
					if (currentState[x][y - 1]) count++;
					if (currentState[x + 1][y - 1]) count++;
					if (currentState[x - 1][y]) count++;
					if (currentState[x + 1][y]) count++;
					if (currentState[x - 1][y + 1]) count++;
					if (currentState[x][y + 1]) count++;
					if (currentState[x + 1][y + 1]) count++;

					if (alive && (count < 2 || count > 3)) nextState[x][y] = false;
					else if (count == 3) nextState[x][y] = true;
					else nextState[x][y] = alive;
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
	if (demo.Construct(1920, 1080, 1, 1, true))
		demo.Start();
	return 0;
}
