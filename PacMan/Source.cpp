#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <cmath>

class PacMan : public olc::PixelGameEngine
{
public:
	PacMan()
	{
		sAppName = "PacMan";
	}

	const float fVelocity = 3.5;

	const uint32_t nTileWidth = 14;
	const uint32_t nPelletRadius = 2;
	const uint32_t nPacRadius = 7;

	uint32_t roomWidth;
	uint32_t roomHeight;
	std::string vRoom [31];

	olc::Pixel wallColor = olc::BLUE;
	olc::Pixel dotColor = olc::WHITE;
	olc::Pixel pacColor = olc::YELLOW;
	olc::Pixel backgroundColor = olc::BLACK;

	olc::Pixel ghostColor;

	olc::vd2d vPacLoc;
	olc::vd2d vPacDir;
	olc::vd2d vNextTurn;
	olc::vd2d vMoveDiff;

	uint32_t score = 0;

public:
	void drawScore()
	{
		DrawString(10, nTileWidth * 31, std::to_string(score), olc::WHITE, 2);
	}

	void drawPac()
	{
		FillCircle(olc::vi2d(uint32_t(vPacLoc.x * nTileWidth + nTileWidth / 2), uint32_t(vPacLoc.y * nTileWidth + nTileWidth / 2)), nPacRadius, pacColor);
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
				}
			}
			y++;
		}
	}

	double findNextTurn()
	{
		if (vPacDir.x < 0)
			for (int i = vPacLoc.x; i > 0; i--)
				if (vRoom[int(vPacLoc.y + vNextTurn.y)][i] != '/')
					return i - vPacLoc.x;
		else if (vPacDir.y < 0)
			for (int i = vPacLoc.y; i > 0; i--)
				if (vRoom[i][vPacLoc.x + vNextTurn.x] != '/')
					return i - vPacLoc.y;
		else if (vPacDir.x > 0)
			for (int i = vPacLoc.x; i < roomWidth; i++)
				if (vRoom[int(vPacLoc.y + vNextTurn.y)][i] != '/')
					return vPacLoc.x - i;
		else if (vPacDir.y > 0)
			for (int i = vPacLoc.y; i < roomHeight; i++)
				if (vRoom[i][vPacLoc.x + vNextTurn.x] != '/')
					return vPacLoc.y - i;

		return -1;
	}

	bool OnUserCreate() override
	{
		roomWidth = 28;
		roomHeight = 31;

		vPacLoc = olc::vd2d(13.5, 23.0);
		vPacDir = olc::vd2d(0.0, 0.0);
		vNextTurn = olc::vd2d(0.0, 0.0);

		// Width: 28, Height: 31
		vRoom[0]  = "////////////////////////////";
		vRoom[1]  = "/............//............/";
		vRoom[2]  = "/.////./////.//./////.////./";
		vRoom[3]  = "/.////./////.//./////.////./";
		vRoom[4]  = "/.////./////.//./////.////./";
		vRoom[5]  = "/........................../";
		vRoom[6]  = "/.////.//.////////.//.////./";
		vRoom[7]  = "/.////.//.////////.//.////./";
		vRoom[8]  = "/......//....//....//....../";
		vRoom[9]  = "//////.///// // /////.//////";
		vRoom[10] = "//////.///// // /////.//////";
		vRoom[11] = "//////.//          //.//////";
		vRoom[12] = "//////.// //////// //.//////";
		vRoom[13] = "//////.// //////// //.//////";
		vRoom[14] = "      .   //    //   .      ";
		vRoom[15] = "//////.// //////// //.//////";
		vRoom[16] = "//////.// //////// //.//////";
		vRoom[17] = "//////.//          //.//////";
		vRoom[18] = "//////.// //////// //.//////";
		vRoom[19] = "//////.// //////// //.//////";
		vRoom[20] = "/............//............/";
		vRoom[21] = "/.////./////.//./////.////./";
		vRoom[22] = "/.////./////.//./////.////./";
		vRoom[23] = "/ ..//.......  .......//.. /";
		vRoom[24] = "///.//.//.////////.//.//.///";
		vRoom[25] = "///.//.//.////////.//.//.///";
		vRoom[26] = "/......//....//....//....../";
		vRoom[27] = "/.//////////.//.//////////./";
		vRoom[28] = "/.//////////.//.//////////./";
		vRoom[29] = "/........................../";
		vRoom[30] = "////////////////////////////";

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
		Clear(olc::BLACK);
		drawRoom();
		drawPac();
		drawScore();

		// movement
		olc::vd2d vPacMov = vPacDir * (fVelocity * double(fElapsedTime));

		olc::vd2d vPacOldLoc = olc::vd2d(vPacLoc.x, vPacLoc.y);

		vPacLoc += vPacMov;

		// check collisions
		if (vPacDir.x < 0 && vRoom[int(vPacLoc.y)][vPacLoc.x] == '/')
			vPacLoc.x = std::ceil(vPacLoc.x);
		else if (vPacDir.y < 0 && vRoom[int(vPacLoc.y)][vPacLoc.x] == '/')
			vPacLoc.y = std::ceil(vPacLoc.y);
		else if (vPacDir.x > 0 && vRoom[int(vPacLoc.y)][std::ceil(vPacLoc.x)] == '/')
			vPacLoc.x = std::floor(vPacLoc.x);
		else if (vPacDir.y > 0 && vRoom[int(std::ceil(vPacLoc.y))][vPacLoc.x] == '/')
			vPacLoc.y = std::floor(vPacLoc.y);

		// dot collision
		int collisionY = vPacLoc.y + 0.5;
		int collisionX = vPacLoc.x + 0.5;
		if (vRoom[collisionY][collisionX] == '.')
		{
			vRoom[collisionY][collisionX] = ' ';
			score += 100;
		}

		// input logic
		if (GetKey(olc::Key::A).bHeld)
			vNextTurn = olc::vd2d(-1, 0);
		else if (GetKey(olc::Key::W).bHeld)
			vNextTurn = olc::vd2d(0, -1);
		else if (GetKey(olc::Key::D).bHeld)
			vNextTurn = olc::vd2d(1, 0);
		else if (GetKey(olc::Key::S).bHeld)
			vNextTurn = olc::vd2d(0, 1);
		else if (GetKey(olc::Key::SPACE).bPressed)
			std::cout << vPacLoc.x << ", " << vPacLoc.y << ", " << vPacDir.x << ", " << vPacDir.y
			<< ", " << vNextTurn.x << ", " << vNextTurn.y << std::endl;

		bool bCanTurn = (vPacLoc.x == std::floor(vPacLoc.x) && vPacLoc.y == std::floor(vPacLoc.y))
			|| (std::floor(vPacLoc.x) != std::floor(vPacOldLoc.x) || std::floor(vPacLoc.y) != std::floor(vPacOldLoc.y));

		// turn logic
		if (vNextTurn.x == vPacDir.x || vNextTurn.y == vPacDir.y)
			vPacDir = vNextTurn;
		else if (bCanTurn && vNextTurn != vPacDir)
		{
			double fMoveDiff = std::abs(vPacLoc.x - std::round(vPacLoc.x) + vPacLoc.y - std::round(vPacLoc.y));

			if		(vPacDir.x < 0) vPacLoc.x = std::ceil(vPacLoc.x);
			else if (vPacDir.y < 0) vPacLoc.y = std::ceil(vPacLoc.y);
			else if (vPacDir.x > 0) vPacLoc.x = std::floor(vPacLoc.x);
			else if (vPacDir.y > 0) vPacLoc.y = std::floor(vPacLoc.y);

			int nPacY = vPacLoc.y;
			
			if ((vPacDir.x < 0																// going left
				&& (vNextTurn.y < 0 && vRoom[nPacY - 1][vPacLoc.x] != '/'					// turning down
				||  vNextTurn.y > 0 && vRoom[nPacY + 1][vPacLoc.x] != '/'))					// turning up
			|| (vPacDir.y < 0																// going up
				&& (vNextTurn.x < 0 && vRoom[nPacY][vPacLoc.x - 1] != '/'					// turning left
				||  vNextTurn.x > 0 && vRoom[nPacY][vPacLoc.x + 1] != '/'))					// turning right
			|| (vPacDir.x > 0																// going right
				&& (vNextTurn.y < 0 && vRoom[nPacY - 1][vPacLoc.x] != '/'					// turning down
				||  vNextTurn.y > 0 && vRoom[nPacY + 1][vPacLoc.x] != '/'))					// turning up
			|| (vPacDir.y > 0																// going down
				&& (vNextTurn.x < 0 && vRoom[nPacY][vPacLoc.x - 1] != '/'					// turning left
				||  vNextTurn.x > 0 && vRoom[nPacY][vPacLoc.x + 1] != '/')))				// turning right
			{
				vPacDir = vNextTurn;
			}
			vPacLoc += vPacDir * fMoveDiff;
		}
		
		return true;
	}
};

int main()
{
	PacMan demo;
	if (demo.Construct(392, 470, 1, 1))
		demo.Start();

	return 0;
}