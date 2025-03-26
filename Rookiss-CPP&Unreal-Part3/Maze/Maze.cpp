// Maze.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "ConsoleHelper.h"

int main()
{
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

        // === 렌더링 ===

        ConsoleHelper::SetCursorPosition(0, 0);
        // 마우스 커서 깜빡임 제거
        ConsoleHelper::ShowConsoleCursor(false);

        ConsoleHelper::SetCursorColor(ConsoleColor::RED);

        const char* TILE = "■";
        for (int32 y = 0; y < 25; y++)
        {
            for (int32 x = 0; x < 25; x++)
            {
                cout << TILE;
            }
            cout << endl;
        }
    }
}
