#include <iostream>
#include <vector>

using namespace std;

template<typename T>
class Vector
{
public:
    Vector()
    {

    }

    ~Vector()
    {
        if (_data)
            delete[] _data;
    }

    void push_back(const T& value)
    {
        // 용량을 먼저 확인
        if (_size == _capacity)
        {
            // 증설 작업
            int newCapacity = static_cast<int>(_capacity * 1.5);
            // 증설 전과 후가 같을 때
            if (newCapacity == _capacity)
                newCapacity++;

            reserve(newCapacity);
        }

        // 데이터 저장
        _data[_size] = value;

        // 데이터 개수 증가
        _size++;
    }

    // 증설 작업
    void reserve(int capacity)
    {
        // 이미 용량이 요청량보다 클 때, 무시
        if (_capacity >= capacity)
            return;

        _capacity = capacity;

        // 데이터 복사
        T* newData = new T[_capacity];

        for (int i = 0; i < _size; i++)
            newData[i] = _data[i];

        // 기존 데이터 삭제
        if (_data)
            delete[] _data;

        // 교체
        _data = newData;
    }

    T& operator[](const int pos) { return _data[pos]; }

    int size() { return _size; }
    int capacity() { return _capacity; }

    void clear()
    {
        // 값 타입이 아니라 클래스 타입일 경우, 객체별로 소멸자 호출 필요
        if (_data)
        {
            // 실제로는 new/delete 키워드보다 malloc/free를 통해서 구현되어 있음.
            delete[] _data;
            _data = new T[_capacity];
        }

        _size = 0;
    }

private:
    // 갖고 있는 데이터 배열
    T* _data = nullptr;
    int     _size = 0;
    int     _capacity = 0;
};

int main()
{
    vector<int> v;

    // size와 capacity 동시에 증가
    v.resize(10);
    cout << v.size() << " " << v.capacity() << endl;

    // 초반에 복사 작업이 많이 일어나니, 크기를 예측할 수 있으면, 미리 해당 크기만큼 할당하도록 작업하는 것이 효율적
    v.reserve(100);
    cout << v.size() << " " << v.capacity() << endl;

    for (int i = 0; i < 100; i++)
    {
        v.push_back(i);
        cout << v[i] << " " << v.size() << " " << v.capacity() << endl;
    }

    v.clear();
    // capacity는 유지, size만 0
    cout << v.size() << " " << v.capacity() << endl;

}
