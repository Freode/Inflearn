#include <iostream>
#include <vector>
#include <list>
#include <stack>

using namespace std;

// 되돌리기 (Ctrl+Z) -> 스택

// 동적 배열, 연결 리스트 둘 다 가능

// default : vector<T>
template<typename T, typename Container = vector<T>>
class Stack
{
public:

    void push(const T& value)
    {
        _container.push_back(value);
    }

    void pop()
    {
        _container.pop_back();
    }

    T& top()
    {
        return _container.back();
    }

    bool empty() { return _container.empty(); }
    int size() { return _container.size(); }

private:
    // container만 바꿔도 정상 작동
    // 같은 함수 이름을 사용

    //vector<T> _container;
    //list<T> _container;
    Container _container;
};

int main()
{
    // vector<int> 방식
    // Stack<int> s;

    // list<int> 방식
    Stack<int, list<int>> s;

    s.push(1);
    s.push(2);
    s.push(3);
    s.push(4);
    s.push(5);

    while (s.empty() == false)
    {
        int data = s.top();
        s.pop();

        cout << data << endl;
    }

    int size = s.size();
}
