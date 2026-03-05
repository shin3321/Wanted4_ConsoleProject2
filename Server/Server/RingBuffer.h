#pragma once
class RingBuffer
{
public:
	RingBuffer();
	RingBuffer(size_t capacity);

	bool empty() const 
	{
		return _head == _tail;
	}
	
	bool full() const
	{
		return (_tail + 1) % _capacity == _head;
	}

	size_t size()
	{
		if (_tail >= _head)
			return _tail - _head;
		else
			return _capacity - _head + _tail;
	}

	//버퍼의 남은 양 확인
	size_t freeSize()
	{
		return _capacity - size() - 1;
	}

	char* date() { return _buffer.data(); }
	size_t capacity() const { return _buffer.size(); }

	bool write(const char* data, size_t len);
	bool read(char* dest, size_t len);
	bool peek(char* dest, size_t len);

private:
	std::vector<char> _buffer;
	size_t _capacity;
	size_t _head = 0;
	size_t _tail = 0;
};

