#pragma once
#ifndef __CLIST__
#define __CLIST__

template<typename T>
class CList
{
public:
	struct Node
	{
		T _data;
		Node* _prev;
		Node* _next;
	};

	class iterator
	{
	public:
		Node* _node;
	public:
		iterator(Node* node = nullptr)
		{
			//인자로 들어온 Node포인터를 저장
			_node = node;
		}
		//후위++
		iterator operator++(int)
		{
			//현재 노드를 다음 노드로 이동
			iterator temp = *this;
			_node = _node->_next;
			return temp;
		}
		//전위++
		iterator& operator++()
		{
			_node = _node->_next;
			return *this;
		}
		iterator operator --(int)
		{
			iterator temp = *this;
			_node = _node->_prev;
			return temp;
		}
		iterator& operator --()
		{
			_node = _node->_prev;
			return *this;
		}
		T& operator *()
		{
			//현재 노드의 데이터를 뽑음
			//T data = _node->_data;
			return _node->_data;
		}
		bool operator == (const iterator& other)
		{
			if (this->_node == other._node)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		bool operator != (const iterator& other)
		{
			if (this->_node != other._node)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	};

public:
	CList<T>()
	{
		_size = 0;
		//_head._data = 0;
		_head._next = &_tail;
		_head._prev = nullptr;
		//_tail._data = 0;
		_tail._next = nullptr;
		_tail._prev = &_head;
	}
	~CList<T>()
	{
		Clear();
	}

	iterator Begin()
	{
		//첫번째 데이터 노드를 가리키는 이터레이터 리턴
		iterator iter = _head._next;
		return iter;
	}
	iterator End()
	{
		//Tail 노드를 가리키는 (데이터가 없는 진짜 더미 끝 노드) 이터레이터를 리턴
		//또는 끝으로 인지할 수 있는 이터레이터를 리턴
		iterator iter = &_tail;
		return iter;
	}
	void Push_Front(T data)
	{
		Node* temp = new Node;
		temp->_data = data;
		_head._next->_prev = temp;
		temp->_next = _head._next;
		_head._next = temp;
		temp->_prev = &_head;
		_size++;
	}
	void Push_Back(T data)
	{
		Node* temp = new Node;
		temp->_data = data;
		_tail._prev->_next = temp;
		temp->_prev = _tail._prev;
		_tail._prev = temp;
		temp->_next = &_tail;
		_size++;
	}
	void Pop_Front()
	{
		Node* temp = _head._next;
		_head._next = temp->_next;
		temp->_next->_prev = &_head;
		delete temp;
		_size--;
	}
	void Pop_Back()
	{
		Node* temp = _tail._prev;
		_tail._prev = temp->_prev;
		temp->_prev->_next = &_tail;
		delete temp;
		_size--;
	}
	void Clear()
	{
		for (; _size > 0; _size--)
		{
			Node* temp = _head._next;
			_head._next = temp->_next;
			temp->_next->_prev = &_head;

			delete(temp);
		}
	}
	int Size() { return _size; };
	bool empty()
	{
		if (_size == 0)
		{
			return true;
		}
		else
			return false;
	};

	iterator Erase(iterator iter)
	{
		//이터레이터의 그 노드를 지움
		//그리고 지운 노드의 다음 노드를 가리키는 이터레이터 리턴
		Node* temp = iter._node;
		Node* temp_Next = temp->_next;
		iterator iter_Next(temp_Next);
		temp->_prev->_next = temp->_next;
		temp->_next->_prev = temp->_prev;

		delete temp;
		_size--;

		return iter_Next;
	}

	void Remove(T Data)
	{
		CList<T>::iterator iter;
		for (iter = Begin(); iter != End(); ++iter)
		{
			if (*iter == Data)
				Erase(iter);
		}
	}

private:
	int _size = 0;
	Node _head;
	Node _tail;
};

#endif