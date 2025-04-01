#include <iostream>
#include <vector>

using namespace std;

void CreateGraph_1()
{
    struct Vertex
    {
        vector<Vertex*> edges;
        // int data;
    };

    vector<Vertex> v;
    v.resize(6);

    v[0].edges.push_back(&v[1]);
    v[0].edges.push_back(&v[3]);
    v[1].edges.push_back(&v[0]);
    v[1].edges.push_back(&v[2]);
    v[1].edges.push_back(&v[3]);
    v[3].edges.push_back(&v[4]);
    v[5].edges.push_back(&v[4]);

    // Q) 0번과 3번 정점이 연결되어 있나요?
    bool connected = false;
    for (Vertex* edge : v[0].edges)
    {
        if (edge == &v[3])
        {
            connected = true;
            break;
        }
    }
}

// Graph_1에서 포인터로 노드 정보와 데이터를 분리해서 관리
// 또한, 포인터로 관리되는 연결 정보를 수정
void CreateGraph_2()
{
    struct Vertex
    {
        // int data;
    };

    vector<Vertex> v;
    v.resize(6);

    // 연결된 목록을 따로 관리
    // adjacent[n] -> n번째 정점과 연결된 정점 목록
    vector<vector<int>> adjacent(6);

    adjacent[0] = { 1, 3 };
    adjacent[1] = { 0, 2, 4 };
    adjacent[3] = { 4 };
    adjacent[5] = { 4 };

    // 정점이 100개
    // - 지하철 노선도 -> 서로 드문 드문 연결 (양옆 or 환승역이라면, 조금 더 많음)
    // - 페이스북/인스타 친구 -> 서로 빽빽하게 연결 -> 리스트로 관리하면, searching할 때, 순회하는 비용이 큼 => 행렬로 관리하는 것이 조금 더 용이함.
    
    // Q) 0번과 3번 정점이 연결되어 있나요?
    bool connected = false;
    for (int vertex : adjacent[0])
    {
        if (vertex == 3)
        {
            connected = true;
            break;
        }
    }

    // STL
    vector<int>& adj = adjacent[0];
    bool connected2 = (find(adj.begin(), adj.end(), 3) != adj.end());
}

void CreateGraph_3()
{
    struct Vertex
    {
        // int data;
    };

    vector<Vertex> v;
    v.resize(6);

    // 연결된 목록을 따로 관리
    // [X][O][X][O][X][X]
    // [O][X][O][O][X][X]
    // [X][X][X][X][X][X]
    // [X][X][X][X][O][X]
    // [X][X][X][X][X][X]
    // [X][X][X][X][O][X]

    // 읽는 방법 : adjacent[from][to]
    // 행렬을 이요한 그래프 표현 (2차원 배열)
    // 메모리 소모가 많지만, 빠른 임시 접근이 가능
    // 간선이 많은 경우 -> 이점
    vector<vector<bool>> adjacent(6, vector<bool>(6, false));
    adjacent[0][1] = true;
    adjacent[0][3] = true;
    adjacent[1][0] = true;
    adjacent[1][2] = true;
    adjacent[1][3] = true;
    adjacent[3][4] = true;
    adjacent[5][4] = true;




    // Q) 0번과 3번 정점이 연결되어 있나요?
    bool connected = adjacent[0][3];


    // 가중치 있는 간선 예시
    vector<vector<int>> adjacent2 =
    {
        vector<int> { -1, 15, -1, 35, -1, -1},
        vector<int> { 15, -1, +5, 10, -1, -1},
        vector<int> { -1, 15, -1, 35, -1, -1},
        vector<int> { -1, 15, -1, 35, +5, -1},
        vector<int> { -1, 15, -1, 35, -1, -1},
        vector<int> { -1, 15, -1, 35, +5, -1},
    };

    bool connected2 = adjacent[0][3] >= 0;
}

int main()
{
    CreateGraph_1();
    CreateGraph_2();

    return 0;
}

