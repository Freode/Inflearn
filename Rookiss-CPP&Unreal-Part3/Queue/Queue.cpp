#include <iostream>
#include <vector>
#include <list>
#include <queue>

using namespace std;

// 벡터에는 pop_front와 같은 기능이 존재하지 않음.
// erase로 첫 번째 값을 지우려면, O(N)이 소모됨.

// list나 deque이 적절함 -> 실제로 큐는 deque을 이용함.
// stack도 마찬가지
template<typename T>
class ListQueue
{
public:

    void push(const T& value)
    {
        _container.push_back(value);
    }

    void pop()
    {
        _container.pop_front();
    }

    T& front()
    {
        return _container.front();
    }

    bool empty() { return _container.empty(); }
    int size() { return _container.size(); }

private:
    list<T> _container;
};

// [front/back][][][][]
// [front][][][back][]
// [][][front][back][]
// 즉, 시작 위치와 끝 위치를 들고 있음. (투 포인터)
// 데이터의 유효 범위를 가지고 있는 것.
template<typename T>
class ArrayQueue
{
public:

    ArrayQueue()
    {
        //_container.resize(100);
    }

    void push(const T& value)
    {
        // TODO : 다 찼는지 체크
        // 사이즈를 통해 모두 사용했는지 확인 -> 증설
        if (_size == _container.size())
        {
            // 증설 작업
            int newSize = max(1, _size * 2);
            // container<T>로 하면, 너무 복잡하니 vector<int>로 진행
            vector<T> newData;
            newData.resize(newSize);

            // 데이터 복사
            // front~back까지만, 복사 진행
            for (int i = 0; i < _size; i++)
            {
                int idx = (_front + i) % _container.size();
                newData[i] = _container[idx];
            }

            // 교체
            _container.swap(newData);
            _front = 0;
            _back = _size;
        }

        _container[_back] = value;
        // 범위를 벗어나는 경우, 제일 앞으로 다시 돌아오기
        _back = (_back + 1) % _container.size();
        _size++;
    }

    void pop()
    {
        _front = (_front + 1) % _container.size();
        _size--;
    }

    T& front()
    {
        return _container[_front];
    }

    bool empty() { return _size == 0; }
    int size() { return _container.size(); }

private:
    vector<T> _container;

    int _front = 0;
    int _back = 0;
    int _size = 0;
};


int main()
{
    ArrayQueue<int> q;

    for (int i = 0; i < 100; i++)
    {
        q.push(i);
    }

    while (q.empty() == false)
    {
        int value = q.front();
        q.pop();

        cout << value << endl;
    }

    int size = q.size();
}