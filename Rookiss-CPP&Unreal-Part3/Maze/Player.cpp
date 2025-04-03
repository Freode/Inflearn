﻿#include "pch.h"
#include "Player.h"
#include "Board.h"

void Player::Init(Board* board)
{
	_pos = board->GetEnterPos();
	_board = board;

	// RightHand();
	// Bfs();
	AStar();
}

void Player::Update(uint64 deltaTick)
{
	// 목적지 도착
	if (_pathIndex >= _path.size())
	{
		// 맵 다시 생성
		_board->GenerateMap();
		Init(_board);
		return;
	}

	// 지정된 간격마다 업데이트 진행
	_sumTick += deltaTick;
	if (_sumTick >= MOVE_TICK)
	{
		_sumTick = 0;

		_pos = _path[_pathIndex];
		_pathIndex++;
	}
}

bool Player::CanGo(Pos pos)
{
	TileType tileType = _board->GetTileType(pos);
	return tileType == TileType::EMPTY;
}

void Player::RightHand()
{
	Pos pos = _pos;

	_path.clear();
	_path.push_back(pos);

	// 목적지 도착하기 전에는 계속 실행
	Pos dest = _board->GetExitPos();

	Pos front[4] =
	{
		Pos{-1, 0},		// UP
		Pos{0, -1},		// LEFT
		Pos{1, 0},		// DOWN
		Pos{0, 1},		// RIGHT
	};

	while (pos != dest)
	{
		// 1. 현재 바라보는 방향을 기준으로 오른쪽으로 갈 수 있는지 확인
		int32 newDir = (_dir - 1 + DIR_COUNT) % DIR_COUNT;
		if (CanGo(pos + front[newDir]))
		{
			// 오른쪽 방향으로 90도 회전.
			_dir = newDir;
			// 앞으로 한 보 전진
			pos += front[_dir];

			_path.push_back(pos);
		}
		// 2. 현재 바라보는 방향을 기준으로 전진할 수 있는지 확인.
		else if (CanGo(pos + front[_dir]))
		{
			// 앞으로 한 보 전진
			pos += front[_dir];

			_path.push_back(pos);
		}
		else
		{
			// 왼쪽 방향으로 90도 회전
			_dir = (_dir + 1) % DIR_COUNT;
		}
	}

	// 내가 걸어온 길을 stack으로 관리
	// 다시 되돌아가는 과정 skip
	stack<Pos> s;

	for (int i = 0; i < _path.size() - 1; i++)
	{
		// 되돌아가고 있다고 판단
		if (s.empty() == false && s.top() == _path[i + 1])
		{
			s.pop();
		}
		// 새로운 길이라고 판단
		else
		{
			s.push(_path[i]);
		}
	}

	// 목적지 도착
	if (_path.empty() == false)
	{
		s.push(_path.back());
	}

	// 스택 특성상, 거꾸로 경로가 나오므로 다시 뒤집기
	vector<Pos> path;
	while (s.empty() == false)
	{
		path.push_back(s.top());
		s.pop();
	}

	reverse(path.begin(), path.end());
	_path = path;
}

void Player::Bfs()
{
	Pos pos = _pos;

	// 목적지 도착하기 전에는 계속 실행
	Pos dest = _board->GetExitPos();

	Pos front[4] =
	{
		Pos{-1, 0},		// UP
		Pos{0, -1},		// LEFT
		Pos{1, 0},		// DOWN
		Pos{0, 1},		// RIGHT
	};

	const int32 size = _board->GetSize();
	vector<vector<bool>> discovered(size, vector<bool>(size, false));
	
	// vector<vector<Pos>> parent;

	// parent[A] = B; -> A는 B로 인해 발견함.
	map<Pos, Pos> parent;

	queue<Pos> q;
	q.push(pos);
	discovered[pos.y][pos.x] = true;
	parent[pos] = pos;

	while (q.empty() == false)
	{
		pos = q.front();
		q.pop();

		// 방문
		if (pos == dest)
			break;

		for (int32 dir = 0; dir < 4; dir++)
		{
			Pos nextPos = pos + front[dir];

			// 갈 수 있는 지역인지 확인
			if (CanGo(nextPos) == false)
				continue;

			// 이미 발견한 지역인지 확인
			if (discovered[nextPos.y][nextPos.x])
				continue;

			q.push(nextPos);
			discovered[nextPos.y][nextPos.x] = true;
			parent[nextPos] = pos;
		}

	}

	_path.clear();

	// 찾은 경로를 거꾸로 거슬러 올라가서 추적
	pos = dest;

	while (true)
	{
		_path.push_back(pos);

		// 시작점은 자신이 곧 부모
		if (pos == parent[pos])
			break;

		// 부모 위치로 한 칸 이동
		pos = parent[pos];
	}

	reverse(_path.begin(), _path.end());
}

// priority_queue에 넣을 노드 정보
struct PQNode
{
	bool operator<(const PQNode& other) const
	{
		return f < other.f;
	}
	bool operator>(const PQNode& other) const
	{
		return f > other.f;
	}

	int32	f;		// f = g + h
	int32	g;
	Pos		pos;
};

void Player::AStar()
{
	// 점수 매기는 시스템만 다를 뿐, 다익스트라랑 거의 유사

	// 점수 매기기
	// F = G + H
	// F = 최종 점수 (작을수록 좋음. 경로에 따라 달라짐.)
	// G = 시작점에서 해당 좌표까지 이동하는데 드는 비용 (작을수록 좋음. 경로에 따라 달라짐.)
	// H(Huristic) = 목적지에서 얼마나 가까운지 (작을수록 좋음. 고정된 값)

	Pos start = _pos;

	// 목적지 도착하기 전에는 계속 실행
	Pos dest = _board->GetExitPos();

	enum
	{
		// 대각선 사용 여부를 결정
		DIR_COUNT = 8
	};

	Pos front[] =
	{
		Pos{-1, 0},		// UP
		Pos{0, -1},		// LEFT
		Pos{1, 0},		// DOWN
		Pos{0, 1},		// RIGHT
		Pos{-1, -1},	// UP_LEFT
		Pos{1, -1},		// DOWN_LEFT
		Pos{1, 1},		// DOWN_RIGHT
		Pos{-1, 1},		// UP_RIGHT
	};

	// 이동 비용 배열
	int32 cost[] =
	{
		10,	// UP
		10,	// LEFT
		10,	// DOWN
		10,	// RIGHT
		14,
		14,
		14,
		14
	};

	const int32 size = _board->GetSize();

	// TODO
	// 1) 예약(발견) 시스템 구현
	// - 이미 더 좋은 경로를 찾았다면, 스킵
	// 
	// 2) 뒤늦게 더 좋은 경로가 발견될 수 있음 -> 예외 처리 필수
	// - openList에서 찾아서 제거한다거나.
	// - pq에서 pop한 다음에 무시한다거나.

	// best[y][x] -> 지금까지 (y,x)에 대한 가장 좋은 비용(작을수록 좋음.)
	vector<vector<int32>> best(size, vector<int32>(size, INT32_MAX));

	// 부모 추적 용도
	map<Pos, Pos> parent;

	// ClosedList
	// close[y][x] -> (y,x)에 방문을 했는지 여부
	vector<vector<bool>> closed(size, vector<bool>(size, false));
	// 방문했으면, true로 설정하기

	// OpenList
	// f 기준으로 오름차순 정렬되도록 설정
	// 아직 방문은 하지 않았지만, 발견은 한 상태인 노드들
	priority_queue<PQNode, vector<PQNode>, greater<PQNode>> pq;

	// 초기값 설정
	{
		int32 g = 0;
		// 굳이 대각선의 길이가 아니라 우리가 임의로 지정
		int32 h = 10 * (abs(dest.y - start.y) + abs(dest.x - start.x));
		pq.push(PQNode{ g + h, g, start });
		best[start.y][start.x] = g + h;
		parent[start] = start;
	}

	// c.f) 만약에 맵의 size가 너무 큰 경우
	// 2차원 벡터가 아니라 map으로 변경해서 만들어도 됨.

	while (pq.empty() == false)
	{
		// 제일 좋은 후보를 찾는다.
		PQNode node = pq.top();
		pq.pop();

		// 동일한 좌표를 여러 경로로 찾아서
		// 더 빠른 경로로 인해서 이미 방문(closed)된 경우 스킵

		// [선택] -> closed를 굳이 안써도 best를 통해서만 판단 가능

		// 이미 방문한 경우 -> 이미 더 빠른 경로로 방문 -> 
		// 현재 경로는 느린 것 -> 스킵
		if (closed[node.pos.y][node.pos.x])
			continue;

		// 이미 다른 이동 경로가 더 빠르다고 판단된 경우, 스킵
		if (best[node.pos.y][node.pos.x] < node.f)
			continue;

		// 방문했다고만 설정
		closed[node.pos.y][node.pos.x] = true;

		// 목적지에 도착했으면, 바로 종료
		if (node.pos == dest)
			break;

		// 방향별로 검사
		for (int32 dir = 0; dir < DIR_COUNT; dir++)
		{
			Pos nextPos = node.pos + front[dir];

			// 갈 수 있는 지역은 맞는지 확인
			if (CanGo(nextPos) == false)
				continue;

			// [선택] 이미 방문한 곳이면, 스킵
			if (closed[nextPos.y][nextPos.x])
				continue;

			// 비용 계산
			int32 g = node.g + cost[dir];
			int32 h = 10 * (abs(dest.y - nextPos.y) + abs(dest.x - nextPos.x));

			// 다른 경로에서 더 빠른 길을 찾았으면, 스킵
			if (best[nextPos.y][nextPos.x] <= g + h)
				continue;

			// 예약 진행
			best[nextPos.y][nextPos.x] = g + h;
			pq.push(PQNode{ g + h, g, nextPos });
			parent[nextPos] = node.pos;
		}
	}

	// 찾은 경로를 거꾸로 거슬러 올라가서 추적
	_path.clear();
	_pathIndex = 0;

	Pos pos = dest;

	while (true)
	{
		_path.push_back(pos);

		// 시작점은 자신이 곧 부모
		if (pos == parent[pos])
			break;

		// 부모 위치로 한 칸 이동
		pos = parent[pos];
	}

	reverse(_path.begin(), _path.end());
}
