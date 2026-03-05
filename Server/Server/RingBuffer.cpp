#include "pch.h"
#include "RingBuffer.h"

bool RingBuffer::write(const char* data, size_t len)
{
	//길이가 남은 양보다 많으면 실패 반환
	if (len > freeSize())
		return false;

	for (size_t i = 0; i < len; ++i)
	{
		_buffer[_tail] = data[i];
		_tail = (_tail + 1) % _capacity;
	}
}

bool RingBuffer::read(char* dest, size_t len)
{
	//읽을 데이터가 버퍼의 사이즈보다 크면 실패 반환
	if (len < size())
		return false;

}

bool RingBuffer::peek(char* dest, size_t len)
{
}
