#include "RingBuffer.h"

RingBuffer::RingBuffer(size_t capacity) : _buffer(capacity), _capacity(capacity){}

std::pair<char*, size_t> RingBuffer::getWritePtr()
{
	// tail부터 버퍼의 실제 끝까지 남은 '연속된' 공간만 반환
	// recv는 연속된 메모리 공간을 요구하기 때문
	size_t contiguousFree = _capacity - _tail;

	// 만약 head가 tail보다 뒤에 있다면 (한 바퀴 돌아서 쫓아오는 중)
	if (_head > _tail)
		contiguousFree = _head - _tail - 1; // head 직전까지만

	return { &_buffer[_tail], contiguousFree };
}

void RingBuffer::moveTail(size_t len)
{
	_tail = (_tail + len) % _capacity;
}

void RingBuffer::onWrite(size_t len)
{
	_tail = (_tail + len) % _capacity;
}

bool RingBuffer::write(const char* data, size_t len)

{
	if (len > freeSize())
	{
		return false;
		return false;
	}

	for (int i = 0; i < len; ++i)
	{
		_buffer[_tail] = data[i];
		_tail = (_tail + 1) % _capacity;
	}
	return true;
}

bool RingBuffer::read(char* dest, size_t len)
{
	if (len > size())
	return false;

	for (size_t i = 0; i < len; ++i)
	{
		if(dest)
		{
			dest[i] = _buffer[_head];
		}
		_head = (_head + 1) % _capacity;
	}
	return true;
}

bool RingBuffer::peek(char* dest, size_t len)
{
	if (len > size())
		return false;

	size_t temp = _head;
	for (size_t i = 0; i < len; ++i)
	{
		dest[i] = _buffer[temp];
		temp = (temp + 1) % _capacity;
	}
	return true;
}