//Решение задачи RGB game
//Автор Петр Королев
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
struct Claster
{
	int clasterSize = 0;
	char elements[sizex + 1][sizey + 1];
	//самый левый и нижний элемент кластера
	int keyBallX = sizex - 1, keyBallY = sizey - 1;
	char type;


};
struct Game
{
	char elements[sizex + 1][sizey + 1];
};
Vector2 FindkeyBall(Claster claster)
{
	//ищем самый левый и нижний элемент
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
Claster dfs(Game game, Claster curClaster, Claster check, int x, int y)
{
	//дфс для обхода доски
	char current = game.elements[x][y];
	curClaster.elements[x][y] = current;
	check.elements[x][y] = '1';
	if (x + 1 < sizex && game.elements[x + 1][y] == current && check.elements[x + 1][y] == '0')
	{
		curClaster = dfs(game, curClaster, check, x + 1, y);
	}
	if (y + 1 < sizey && game.elements[x][y + 1] == current && check.elements[x][y + 1] == '0')
	{
		curClaster = dfs(game, curClaster, check, x, y + 1);
	}
	if (y - 1 >= 0 && game.elements[x][y - 1] == current && check.elements[x][y - 1] == '0')
	{
		curClaster = dfs(game, curClaster, check, x, y - 1);
	}
	if (x - 1 >= 0 && game.elements[x - 1][y] == current && check.elements[x - 1][y] == '0')
	{
		curClaster = dfs(game, curClaster, check, x - 1, y);
	}
	return curClaster;
}
Claster FindMaxClaster2(Game game)
{
	char current = '0';
	Claster curClaster, maxClaster, check;
	int t = 0;
	int elementsAlive = 0;
	//инициализируем текущий кластер и считаем количество элементов
	for (int i = 0; i < sizey; i++)
	{
		for (int j = 0; j < sizex; j++)
		{
			curClaster.elements[j][i] = '0';
			check.elements[j][i] = '0';
			if (game.elements[j][i] != '0')
				elementsAlive++;
		}
	}
	//ищем все возможные кластеры
	while (t < elementsAlive)
	{
		//собираем кластер
		for (int i = 0; i < sizey; i++)
		{
			for (int j = 0; j < sizex; j++)
			{
				if (game.elements[j][i] != '0'&&current == '0')
				{
					current = game.elements[j][i];
					curClaster.type = current;
					curClaster = dfs(game, curClaster, check, j, i);
				}
			}
		}

		//считаем размер текущего кластера
		int curSize = 0;
		for (int i = 0; i < sizey; i++)
		{
			for (int j = 0; j < sizex; j++)
			{
				if (curClaster.elements[j][i] == game.elements[j][i] && curClaster.elements[j][i] != '0')
				{
					curSize++;
				}
			}
		}
		curClaster.clasterSize = curSize;

		//проверяем максимальность текущего кластера
		Vector2 keyBall = FindkeyBall(curClaster);
		curClaster.keyBallX = keyBall.x;
		curClaster.keyBallY = keyBall.y;
		if (maxClaster.clasterSize < curClaster.clasterSize)
		{
			maxClaster = curClaster;
		}
		if (maxClaster.clasterSize == curClaster.clasterSize)
		{
			if (maxClaster.keyBallX > curClaster.keyBallX)
				maxClaster = curClaster;
			if (maxClaster.keyBallX == curClaster.keyBallX && maxClaster.keyBallY < curClaster.keyBallY)
				maxClaster = curClaster;
		}
		t += curSize;

		//подчищаем переменные
		for (int i = 0; i < sizey; i++)
		{
			for (int j = 0; j < sizex; j++)
			{
				if (curClaster.elements[j][i] == game.elements[j][i] && curClaster.elements[j][i] != '0')
				{
					game.elements[j][i] = '0';


				}
				curClaster.elements[j][i] = '0';
			}
		}
		current = '0';
	}

	return maxClaster;
}
Game ShakeGame(Game game)
{
	//сжимаем столбцы
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
	//сдвигаем столбцы
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
void SolveTheGame(Game game)
{
	Claster maxClaster;
	int elementsAlive = sizex * sizey;
	maxClaster = FindMaxClaster2(game);
	int count = 0;
	int deltaCount = 0;
	int steps = 0;
	while (elementsAlive > 0 && maxClaster.clasterSize > 1)
	{
		//удаляем из текущей доски макс кластер
		elementsAlive -= maxClaster.clasterSize;
		deltaCount = (maxClaster.clasterSize - 2) * (maxClaster.clasterSize - 2);
		count += deltaCount;
		steps++;
		cout << "Move " << steps << " at (" << 10 - maxClaster.keyBallY << "," << maxClaster.keyBallX + 1 << "): ";
		cout << "removed " << maxClaster.clasterSize << " balls of color " << maxClaster.type << ", " << "got " << deltaCount << " points.\n";
		for (int i = 0; i < sizey; i++)
		{
			for (int j = 0; j < sizex; j++)
			{
				if (game.elements[j][i] == maxClaster.elements[j][i])
					game.elements[j][i] = '0';
			}
		}
		game = ShakeGame(game);
		maxClaster = FindMaxClaster2(game);
	}
	if (elementsAlive == 0)
		count += 1000;
	cout << "Final score: " << count << ", with " << elementsAlive << " balls remaining.\n";
}
int main()
{
	int N;
	cin >> N;
	vector <Game> game(N);
	//считываем входные данные
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
	for (int n = 0; n < N; n++)
	{
		cout << "Game " << n + 1 << ":\n";
		SolveTheGame(game[n]);
	}
	return 0;
}


