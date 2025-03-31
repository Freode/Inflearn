#include <iostream>
#include <list>

using namespace std;

// 주로 vector를 사용

// 노드 기반
template<typename T>
class Node
{
public:

    // 생성자
    Node() : _prev(nullptr), _next(nullptr), _data(T())
    {

    }

    Node(const T& value) : _prev(nullptr), _next(nullptr), _data(value)
    {

    }

public:
    Node*   _prev;
    Node*   _next;
    T       _data;
};

// iterator
template<typename T>
class Iterator
{
public:
    // 생성자
    Iterator() : _node(nullptr)
    {

    }

    Iterator(Node<T>* node) : _node(node)
    {

    }

    // ++it
    Iterator& operator++()
    {
        _node = _node->_next;
        return *this;
    }

    // it++
    Iterator operator++(int)
    {
        Iterator<T> temp = *this;
        _node = _node->_next;
        return temp;
    }

    // --it
    Iterator& operator--()
    {
        _node = _node->_prev;
        return *this;
    }

    // it--
    Iterator operator--(int)
    {
        Iterator<T> temp = *this;
        _node = _node->_prev;
        return temp;
    }

    // 실제로 들고 있는 데이터를 반환
    // *it
    T& operator*()
    {
        return _node->_data;
    }

    // 같은지 체크
    bool operator==(const Iterator& other)
    {
        return _node == other._node;
    }

    // 다른지 체크
    bool operator!=(const Iterator& other)
    {
        // return !(_node == other._node);
        return _node != other._node;
    }

public:
    Node<T>* _node;
};

template<typename T>
class List
{
public:

    // 생성자
    List() : _size(0)
    {
        // 아무것도 없음에도 불구하고 head와 tail은 필요
        // [head] <-> ... <-> [tail]
        _head = new Node<T>();
        _tail = new Node<T>();
        _head->_next = _tail;
        _tail->_prev = _head;
    }

    // 소멸자
    ~List()
    {
        while (_size > 0)
        {
            pop_back();
        }

        delete _head;
        delete _tail;
    }

    // tail 이전에 데이터 밀어넣기
    void push_back(const T& value)
    {
        AddNode(_tail, value);
    }

    // tail 이전에 데이터 꺼내기
    void pop_back()
    {
        RemoveNode(_tail->_prev);
    }

private:

    // before 이전에 노드를 하나 추가하기

    // [head] <-> [1] <-> [prevNode] <-> [before] <-> [tail]
    // [head] <-> [1] <-> [prevNode] <-> [newNode] <-> [before] <-> [tail]
    Node<T>* AddNode(Node<T>* before, const T& value)
    {
        Node<T>* newNode = new Node<T>(value);
        Node<T>* prevNode = before->_prev;

        // 노드 연결
        prevNode->_next = newNode;
        newNode->_prev = prevNode;

        newNode->_next = before;
        before->_prev = newNode;

        _size++;

        return newNode;
    }

    // [head] <-> [1] <-> [prevNode] <-> [node] <-> [nextNode] <-> [tail]
    // [head] <-> [1] <-> [prevNode] <-> [nextNode] <-> [tail]
    Node<T>* RemoveNode(Node<T>* node)
    {
        Node<T>* prevNode = node->_prev;
        Node<T>* nextNode = node->_next;

        prevNode->_next = nextNode;
        nextNode->_prev = prevNode;

        delete node;

        _size--;

        return nextNode;
    }

    int size() { return _size; }

public:

    // using은 template과 호환 가능
    // typedef는 안됨.
    using iterator = Iterator<T>;

    iterator begin() { return iterator(_head->_next); }
    iterator end() { return iterator(_tail); }

    // iterator 바로 앞에 추가
    iterator insert(iterator it, const T& value)
    {
        Node<T>* node = AddNode(it._node, value);
        return iterator(node);
    }

    //
    iterator erase(iterator it)
    {
        Node<T>* node = RemoveNode(it._node);
        return iterator(node);
    }

private:
    Node<T>*    _head;
    Node<T>*    _tail;
    int         _size;
};

int main()
{
    List<int> li;
    
    List<int>::iterator eraseIt;
    // [ ] <-> [ ] <-> [ ]
    for (int i = 0; i < 10; i++)
    {
        if (i == 5)
        {
            // 위치를 iterator로 받음
            eraseIt = li.insert(li.end(), i);
        }
        else
        {
            li.push_back(i);
        }
    }
    
    li.pop_back();
    // 중간 데이터 빠르게 삭제하기
    li.erase(eraseIt);

    // vector
    // - push_back : O -> O(1)
    // - push_front : X -> O(N)

    for (List<int>::iterator it = li.begin(); it != li.end(); ++it)
    {
        cout << (*it) << endl;
    }

    return 0;
}
