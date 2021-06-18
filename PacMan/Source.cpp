#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class PacMan : public olc::PixelGameEngine
{
public:
	PacMan()
	{
		sAppName = "PacMan";
	}

	const uint32_t nTileWidth = 14;
	const uint32_t nPelletRadius = 2;
	const uint32_t nPacRadius = 7;
	const float fVelocity = 2.5;

	std::vector<std::string> vRoom;

	olc::Pixel wallColor = olc::BLUE;
	olc::Pixel dotColor = olc::WHITE;
	olc::Pixel pacColor;
	olc::Pixel ghostColor;
	olc::Pixel backgroundColor = olc::BLACK;

	uint32_t roomWidth;
	olc::vd2d vPacLoc;
	uint8_t nPacDir; // 0: left, 1: up, 2: right, 3: down

public:
	void drawPac()
	{
		FillCircle(olc::vi2d(uint32_t(vPacLoc.x * nTileWidth + nTileWidth / 2), uint32_t(vPacLoc.y * nTileWidth + nTileWidth / 2)), nPacRadius, olc::YELLOW);
	}

	void drawRoom()
	{
		uint32_t y = 0;
		for (const auto& section : vRoom)
		{
			for (uint32_t x = 0; x < roomWidth; x++)
			{
				olc::vi2d vPos = olc::vi2d(x * nTileWidth, y * nTileWidth);
				switch (section[x])
				{
				case '/':
					FillRect(vPos, olc::vi2d(nTileWidth, nTileWidth), olc::BLUE);
					break;
				case '.':
					FillCircle(vPos + olc::vi2d(nTileWidth / 2, nTileWidth / 2), nPelletRadius, olc::WHITE);
					break;
				case ' ':
					FillRect(vPos, olc::vi2d(nTileWidth, nTileWidth), olc::BLACK);
					break;
				}
			}
			y++;
		}
	}

	bool OnUserCreate() override
	{
		roomWidth = 28;
		vPacLoc = olc::vd2d(13.5, 23.0);
		nPacDir = 2;

		// Width: 28, Height: 31
		vRoom.push_back("////////////////////////////");
		vRoom.push_back("/............//............/");
		vRoom.push_back("/.////./////.//./////.////./");
		vRoom.push_back("/.////./////.//./////.////./");
		vRoom.push_back("/.////./////.//./////.////./");
		vRoom.push_back("/........................../");
		vRoom.push_back("/.////.//.////////.//.////./");
		vRoom.push_back("/.////.//.////////.//.////./");
		vRoom.push_back("/......//....//....//....../");
		vRoom.push_back("//////.///// // /////.//////");
		vRoom.push_back("//////.///// // /////.//////");
		vRoom.push_back("//////.//          //.//////");
		vRoom.push_back("//////.// //////// //.//////");
		vRoom.push_back("//////.// /      / //.//////");
		vRoom.push_back("      .   /      /   .      ");
		vRoom.push_back("//////.// /      / //.//////");
		vRoom.push_back("//////.// //////// //.//////");
		vRoom.push_back("//////.//          //.//////");
		vRoom.push_back("//////.// //////// //.//////");
		vRoom.push_back("//////.// //////// //.//////");
		vRoom.push_back("/............//............/");
		vRoom.push_back("/.////./////.//./////.////./");
		vRoom.push_back("/.////./////.//./////.////./");
		vRoom.push_back("/ ..//.......  .......//.. /");
		vRoom.push_back("///.//.//.////////.//.//.///");
		vRoom.push_back("///.//.//.////////.//.//.///");
		vRoom.push_back("/......//....//....//....../");
		vRoom.push_back("/.//////////.//.//////////./");
		vRoom.push_back("/.//////////.//.//////////./");
		vRoom.push_back("/........................../");
		vRoom.push_back("////////////////////////////");

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
		Clear(olc::BLACK);
		drawRoom();
		drawPac();

		// movement logic
		double fPacMov = fVelocity * double(fElapsedTime);
		olc::vd2d vPacMov = olc::vd2d(0.0, 0.0);

		switch (nPacDir)
		{
		case 0:
			vPacMov.x = -fPacMov;
			break;
		case 1:
			vPacMov.y = -fPacMov;
			break;
		case 2:
			vPacMov.x = fPacMov;
			break;
		case 3:
			vPacMov.y = fPacMov;
			break;
		}

		vPacLoc += vPacMov;

		// check collisions
		switch (nPacDir)
		{
		case 0:
			if (vRoom.at(int(vPacLoc.y))[int(vPacLoc.x)] == '/')
				vPacLoc.x = double(int(vPacLoc.x) + 1);
			break;
		case 1:
			if (vRoom.at(int(vPacLoc.y))[int(vPacLoc.x)] == '/')
				vPacLoc.y = double(int(vPacLoc.y) + 1);
			break;
		case 2:
			if (vRoom.at(int(vPacLoc.y))[int(vPacLoc.x) + 1] == '/')
			vPacLoc.x = double(int(vPacLoc.x));
			break;
		case 3:
			if (vRoom.at(int(vPacLoc.y) + 1)[int(vPacLoc.x)] == '/')
			vPacLoc.y = double(int(vPacLoc.y));
			break;
		}

		// input logic
		if (GetKey(olc::Key::A).bReleased)
		{
			nPacDir = 0;
		}
		else if (GetKey(olc::Key::W).bReleased)
		{
			nPacDir = 1;
		}
		else if (GetKey(olc::Key::D).bReleased)
		{
			nPacDir = 2;
		}
		else if (GetKey(olc::Key::S).bReleased)
		{
			nPacDir = 3;
		}

		return true;
	}
};


int main()
{
	PacMan demo;
	if (demo.Construct(392, 434, 1, 1))
		demo.Start();

	return 0;
}