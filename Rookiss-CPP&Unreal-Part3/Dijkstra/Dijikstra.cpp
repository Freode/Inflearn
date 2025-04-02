#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <stack>

using namespace std;

// 다익스트라 알고리즘
// cost(거리) 기반


struct Vertex
{
	// int data;
};

vector<Vertex> vertices;
vector<vector<int>> adjacent; // 인접 행렬

void CreateGraph()
{
	vertices.resize(6);
	
	// -1 : 연결되어 있지 않음.
	adjacent = vector<vector<int>>(6, vector<int>(6, -1));

	adjacent[0][1] = 15;
	adjacent[0][3] = 35;
	adjacent[1][0] = 15;
	adjacent[1][2] = 5;
	adjacent[1][3] = 10;
	adjacent[3][4] = 5;
	adjacent[5][4] = 5;
}

void Dijikstra(int here)
{
	struct VertexCost
	{
		int vertex;
		int cost;
	};

	// 발견 목록
	list<VertexCost> discovered;

	// 각 정점별로 지금까지 발견한 최소 거리
	vector<int> best(6, INT32_MAX);
	vector<int> parent(6, -1);

	discovered.push_back(VertexCost{ here, 0 });
	best[here] = 0;
	parent[here] = here;

	// 지금 현재 방식은 성능이 좋지 않음. -> 우선순위 큐로 처리해야 함.

	while (discovered.empty() == false)
	{
		// 제일 좋은 후보를 찾는다
		list<VertexCost>::iterator bestIt;
		int bestCost = INT32_MAX;

		for (auto it = discovered.begin(); it != discovered.end(); ++it)
		{
			const int vertex = it->vertex;
			const int cost = it->cost;

			// 지금까지 찾은 가장 좋은 케이스를 대입.
			if (cost < bestCost)
			{
				bestCost = cost;
				bestIt = it;
			}
		}

		// 현재까지 찾은 애들 중에서 제일 좋은 경우를 지움
		int cost = bestIt->cost;
		here = bestIt->vertex;
		discovered.erase(bestIt);

		// 이미 더 짧은 경로로 찾았다면, 스킵
		if (best[here] < cost)
			continue;

		// 방문
		for (int there = 0; there < 6; there++)
		{
			// 연결되지 않았으면, 스킵
			if (adjacent[here][there] == -1)
				continue;

			int nextCost = best[here] + adjacent[here][there];

			// 더 좋은 경로를 과거에 찾았으면, 생략.
			if (nextCost >= best[there])
				continue;

			best[there] = nextCost;
			parent[there] = here;

			// 같은 노드 정보가 들어갈 수 있으나, 비용이 서로 다름
			// ex) (3, 35), (3, 25)
			discovered.push_back(VertexCost{ there, nextCost });
		}
	}
}

int main()
{
	CreateGraph();
	Dijikstra(0);

	return 0;
}