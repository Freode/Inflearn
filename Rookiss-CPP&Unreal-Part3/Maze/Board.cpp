﻿#include "pch.h"
#include "Board.h"
#include "ConsoleHelper.h"
#include "Player.h"

const char* TILE = "■";

Board::Board()
{
}

Board::~Board()
{
}

void Board::Init(int32 size, Player* player)
{
    _size = size;
    _player = player;

    GenerateMap();
}

void Board::Render()
{
    ConsoleHelper::SetCursorPosition(0, 0);
    // 마우스 커서 깜빡임 제거
    ConsoleHelper::ShowConsoleCursor(false);

    ConsoleHelper::SetCursorColor(ConsoleColor::RED);

    for (int32 y = 0; y < 25; y++)
    {
        for (int32 x = 0; x < 25; x++)
        {
            // 타일 색상 정하기
            ConsoleColor color = GetTileColor(Pos{ y,x });
            ConsoleHelper::SetCursorColor(color);

            cout << TILE;
        }
        cout << endl;
    }
}

// Binary Tree 미로 생성 알고리즘
// - Mazes For Programmers에서 나옴.

void Board::GenerateMap()
{
    // 한 칸씩 띄어서 빈칸을 뚫는 작업
    for (int32 y = 0; y < _size; y++)
    {
        for (int32 x = 0; x < _size; x++)
        {
            if (x % 2 == 0 || y %  2 == 0)
            {
                _tile[y][x] = TileType::WALL;
            }
            else
            {
                _tile[y][x] = TileType::EMPTY;
            }
        }
    }

    // 랜덤으로 우측 혹은 아래로 긿을 뚫는 작업
    for (int32 y = 0; y < _size; y++)
    {
        for (int32 x = 0; x < _size; x++)
        {
            // 이미 벽을 뚫은 곳
            if (x % 2 == 0 || y % 2 == 0) 
                continue;

            // 마지막 이동 막기
            if (y == _size - 2 && x == _size - 2) 
                continue;

            // 최하단 : 항상 오른쪽으로 작동
            if (y == _size - 2)
            {
                _tile[y][x + 1] = TileType::EMPTY;
                continue;
            }

            // 최우측 : 항상 아래로 작동
            if (x == _size - 2)
            {
                _tile[y + 1][x] = TileType::EMPTY;
                continue;
            }

            // 0 : 우측, 1 : 아래
            const int32 randValue = ::rand() % 2;
            if(randValue == 0)
            {
                _tile[y][x + 1] = TileType::EMPTY;
            }
            else
            {
                _tile[y + 1][x] = TileType::EMPTY;
            }
        }
    }
}

TileType Board::GetTileType(Pos pos)
{
    if (pos.x < 0 || pos.x >= _size)
        return TileType::NONE;

    if (pos.y < 0 || pos.y >= _size)
        return TileType::NONE;

    return _tile[pos.y][pos.x];
}

ConsoleColor Board::GetTileColor(Pos pos)
{
    if (_player && _player->GetPos() == pos)
        return ConsoleColor::YELLOW;

    if (GetExitPos() == pos)
        return ConsoleColor::BLUE;

    TileType tileType = GetTileType(pos);

    switch (tileType)
    {
    case TileType::EMPTY:
        return ConsoleColor::GREEN;

    case TileType::WALL:
        return ConsoleColor::RED;
    }

    return ConsoleColor::WHITE;
}
