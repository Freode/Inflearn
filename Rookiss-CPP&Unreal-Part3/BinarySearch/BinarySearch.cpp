#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>

using namespace std;

// 이진 탐색(binary search)

// 정렬된 배열을 이용해서 이진 탐색 가능(nubmers[mid])
// - 정렬된 연결 리스트는 불가 -> 임의 접근 X
// - 배열의 단점 -> 중간 삽입/삭제가 느리다. -> 만약에, 중간 삽입/삭제까지 하려면, 트리가 필요
vector<int> numbers;

// O(logN)
void BinarySearch(int N)
{
	int left = 0;
	int right = numbers.size() - 1;

	bool b_found = false;
	while (left <= right)
	{
		cout << "탐색 범위 : " << left << "~" << right << endl;

		int mid = (left + right) / 2;

		if (N < numbers[mid])
		{
			cout << N << " < " << numbers[mid] << endl;
			right = mid - 1;
		}
		else if (N > numbers[mid])
		{
			cout << N << " > " << numbers[mid] << endl;
			left = mid + 1;
		}
		else
		{
			cout << "찾음!" << endl;
			b_found = true;
			break;
		}
	}

	if (b_found == false)
	{
		cout << "못 찾음!" << endl;
	}
}

int main()
{
	numbers = vector<int>{ 1,8,15,23,32,44,56,63,81,91 };
	BinarySearch(82);
}

