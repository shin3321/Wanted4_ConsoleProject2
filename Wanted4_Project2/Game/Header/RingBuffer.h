#pragma once
#include <vector>

class RingBuffer
{
public:
	RingBuffer() {}
	RingBuffer(size_t capacity);
	
	//데이터를 쓸 수 있는 위치와 크기를 반환
	std::pair<char*, size_t> getWritePtr();

	void onWrite(size_t len);
	//실제로 쓴 만큼 _tail을 이동시킴
	void moveTail(size_t);

	bool empty() const{ return _head == _tail; };
	bool full() const { return (_tail + 1) % _capacity == _head; }

	size_t size()const
	{
		if (_tail >= _head)
		{
			return _tail - _head;
		}
		else
		{
			return _capacity - _head + _tail;
		}
	}

	size_t freeSize() const
	{
		return _capacity - size() - 1;
	}

	char* data()  { return _buffer.data(); }

	size_t capacity() const
	{
		return _buffer.size();
	}

	bool write(const char* data, size_t len);
	bool read(char* dest, size_t len);
	bool peek(char* dest, size_t len);

public:
	std::vector<char> _buffer;
	size_t _head = 0;
	size_t _tail = 0;
	size_t _capacity = 0;
};

