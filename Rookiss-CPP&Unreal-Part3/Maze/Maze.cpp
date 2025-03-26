// Maze.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "ConsoleHelper.h"
#include "Board.h"
#include "Player.h"

Board board;
Player player;

int main()
{
    // 랜덤 시드 설정
    ::srand(static_cast<unsigned int>(time(nullptr)));

    board.Init(25, &player);
    player.Init(&board);

    uint64 lastTick = 0;

    while (true)
    {
#pragma region 프레임 관리
        // 현재 프레임 측정
        const uint64 currentTick = ::GetTickCount64();
        const uint64 deltaTick = currentTick - lastTick;
        // 고정 프레임으로 작동하는 원리
        // if(deltaTick > 일정 시간) continue;
        lastTick = currentTick;
#pragma endregion

        // === 입력 ===

        // === 로직 ===
        player.Update(deltaTick);

        // === 렌더링 ===
        board.Render();
    }
}
