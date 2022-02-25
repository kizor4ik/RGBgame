//The solution to the RGB-game task for the Huawei scholarship competition
//Author: Petr Korolev

//#include "pch.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

const int sizex = 15, sizey = 10;

struct Vector2
{
	int x, y;
};

struct Cluster
{
	int clusterSize = 0;
	char elements[sizex + 1][sizey + 1];
	//The bottomost and the leftmost ball of the cluster
	int keyBallX = sizex - 1, keyBallY = sizey - 1;
	char type;
};

struct Game
{
	char elements[sizex + 1][sizey + 1];
};

Cluster CheckCluster;

//Find the bottomost and the leftmost key ball of the cluster
Vector2 FindKeyBall(Cluster claster)
{
	Vector2 keyBall;
	keyBall.x = sizex;
	keyBall.y = 0;
	for (int i = 0; i < sizey; i++)
	{
		for (int j = 0; j < sizex; j++)
		{
			if (claster.elements[j][i] != '0')
			{
				if (j < keyBall.x)
					keyBall.x = j;
				if (j == keyBall.x && i > keyBall.y)
					keyBall.y = i;
			}
		}
	}
	return keyBall;
}

Cluster DFS(Game game, Cluster curClaster, int x, int y)
{
	char current = game.elements[x][y];
	curClaster.elements[x][y] = current;
	CheckCluster.elements[x][y] = '1';
	if (x + 1 < sizex && game.elements[x + 1][y] == current && CheckCluster.elements[x + 1][y] == '0')
	{
		curClaster = DFS(game, curClaster, x + 1, y);
	}
	if (y + 1 < sizey && game.elements[x][y + 1] == current && CheckCluster.elements[x][y + 1] == '0')
	{
		curClaster = DFS(game, curClaster, x, y + 1);
	}
	if (y - 1 >= 0 && game.elements[x][y - 1] == current && CheckCluster.elements[x][y - 1] == '0')
	{
		curClaster = DFS(game, curClaster, x, y - 1);
	}
	if (x - 1 >= 0 && game.elements[x - 1][y] == current && CheckCluster.elements[x - 1][y] == '0')
	{
		curClaster = DFS(game, curClaster, x - 1, y);
	}
	return curClaster;
}

Game CompressTheGame(Game game)
{
	//Shift column down
	for (int j = 0; j < sizex; j++)
	{
		for (int i = sizey - 1; i > 0; i--)
		{
			if (game.elements[j][i] == '0')
			{
				int d = 0;
				for (int k = i; k >= 0; k--)
				{
					if (game.elements[j][k] != '0')
					{
						d = i - k;
						break;
					}
				}
				for (int k = i; k >= 0; k--)
				{
					if (d != 0 && k - d >= 0)
					{
						game.elements[j][k] = game.elements[j][k - d];
						game.elements[j][k - d] = '0';
					}
				}
			}
		}
	}
	//Shift column to the left
	for (int j = 0; j < sizex; j++)
	{
		bool isEmpty = true;
		bool isFinished = true;
		for (int i = 0; i < sizey; i++)
		{
			if (game.elements[j][i] != '0')
			{
				isEmpty = false;
				break;
			}
		}
		if (isEmpty)
		{
			for (int l = j; l < sizex; l++)
			{
				for (int k = 0; k < sizey; k++)
				{
					game.elements[l][k] = game.elements[l + 1][k];
					game.elements[l + 1][k] = '0';
					if (game.elements[l][k] != '0')
					{
						isFinished = false;
					}
				}
			}
		}
		if (!isFinished)
			j--;
	}
	return game;
}

Cluster FindMaxCluster(Game game)
{
	char current = '0';
	Cluster curCluster, maxCluster;
	int t = 0;
	int elementsAlive = 0;
	//Build up empty clusters and count the number of elements in the game
	for (int i = 0; i < sizey; i++)
	{
		for (int j = 0; j < sizex; j++)
		{
			curCluster.elements[j][i] = '0';

			if (game.elements[j][i] != '0')
				elementsAlive++;
		}
	}
	//Check out all the available clusters
	while (t < elementsAlive)
	{
		//Refresh CheckCluster
		for (int i = 0; i < sizey; i++)
		{
			for (int j = 0; j < sizex; j++)
			{
				CheckCluster.elements[j][i] = '0';
			}
		}

		//Build up current cluster
		for (int i = 0; i < sizey; i++)
		{
			for (int j = 0; j < sizex; j++)
			{
				if (game.elements[j][i] != '0' && current == '0')
				{
					current = game.elements[j][i];
					curCluster.type = current;
					curCluster = DFS(game, curCluster, j, i);
				}
			}
		}

		//Count cluster size
		int curSize = 0;
		for (int i = 0; i < sizey; i++)
		{
			for (int j = 0; j < sizex; j++)
			{
				if (curCluster.elements[j][i] == game.elements[j][i] && curCluster.elements[j][i] != '0')
				{
					curSize++;
				}
			}
		}
		curCluster.clusterSize = curSize;

		//Check if the cluster is the maximum one
		Vector2 keyBall = FindKeyBall(curCluster);
		curCluster.keyBallX = keyBall.x;
		curCluster.keyBallY = keyBall.y;
		if (maxCluster.clusterSize < curCluster.clusterSize)
		{
			maxCluster = curCluster;
		}
		if (maxCluster.clusterSize == curCluster.clusterSize)
		{
			if (maxCluster.keyBallX > curCluster.keyBallX)
				maxCluster = curCluster;
			if (maxCluster.keyBallX == curCluster.keyBallX && maxCluster.keyBallY < curCluster.keyBallY)
				maxCluster = curCluster;
		}
		t += curSize;

		//Remove processed cluster
		for (int i = 0; i < sizey; i++)
		{
			for (int j = 0; j < sizex; j++)
			{
				if (curCluster.elements[j][i] == game.elements[j][i] && curCluster.elements[j][i] != '0')
				{
					game.elements[j][i] = '0';
				}
				curCluster.elements[j][i] = '0';
			}
		}
		current = '0';
	}
	return maxCluster;
}

void SolveTheGame(Game game)
{
	Cluster maxClaster;
	int elementsAlive = sizex * sizey;
	int count=0, deltaCount=0, steps = 0;
	maxClaster = FindMaxCluster(game);
	// Step by step removing the max cluster
	while (elementsAlive > 0 && maxClaster.clusterSize > 1)
	{
		steps++;
		//Remove the current max cluster
		elementsAlive -= maxClaster.clusterSize;
		for (int i = 0; i < sizey; i++)
		{
			for (int j = 0; j < sizex; j++)
			{
				if (game.elements[j][i] == maxClaster.elements[j][i])
					game.elements[j][i] = '0';
			}
		}
		//Save the data and print out the step
		deltaCount = (maxClaster.clusterSize - 2) * (maxClaster.clusterSize - 2);
		count += deltaCount;
		cout << "Move " << steps << " at (" << sizey - maxClaster.keyBallY << "," << maxClaster.keyBallX + 1 << "): ";
		cout << "removed " << maxClaster.clusterSize << " balls of color " << maxClaster.type << ", " << "got " << deltaCount << " points.\n";

		game = CompressTheGame(game);

		maxClaster = FindMaxCluster(game);
	}
	//Extra points
	if (elementsAlive == 0)
		count += 1000;
	cout << "Final score: " << count << ", with " << elementsAlive << " balls remaining.\n";
}
int main()
{
	int N;
	cin >> N;
	vector <Game> game(N);
	
	//Input
	for (int n = 0; n < N; n++)
	{
		for (int i = 0; i < sizey + 1; i++)
		{
			for (int j = 0; j < sizex + 1; j++)
			{
				game[n].elements[j][i] = '0';
			}
		}
		for (int i = 0; i < sizey; i++)
		{
			for (int j = 0; j < sizex; j++)
			{
				cin >> game[n].elements[j][i];
			}
		}
	}
	//Solution for each Game
	for (int n = 0; n < N; n++)
	{
		cout << "Game " << n + 1 << ":\n";
		SolveTheGame(game[n]);
	}
	return 0;
}


