#include "olcPixelGameEngine.h"

class CellularAutomata : public olc::PixelGameEngine
{
	public:
		bool* currentState;
		bool* nextState;

		CellularAutomata()
		{
			// Name your application
			sAppName = "Example";
		}

		bool OnUserCreate() override
		{
			nextState = new bool[100, 100];
			currentState = new bool[100, 100];

			// seed, ignore outer boundary as they wont be drawn for optimaztion purposes.
			for (int x = 1; x < 99; x++)
			{
				for (int y = 1; y < 99; y++)
				{
					if (rand() % (2) == 10) nextState[x, y] = true;
				}
			}

			return true;
		}

		bool OnUserUpdate(float fElapsedTime) override
		{			
			return true;
		}
};

int main()
{
	CellularAutomata demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}
