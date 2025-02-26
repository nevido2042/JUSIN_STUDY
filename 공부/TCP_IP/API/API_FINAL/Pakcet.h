#ifndef  __PACKET__
#define  __PACKET__

#include<Windows.h>

class CPacket
{
public:
	/*---------------------------------------------------------------
	Packet Enum.

	----------------------------------------------------------------*/
	enum en_PACKET
	{
		eBUFFER_DEFAULT = 1400		// 패킷의 기본 버퍼 사이즈.
	};

	//////////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자.
	//
	// Return:
	//////////////////////////////////////////////////////////////////////////
	CPacket();
	CPacket(int iBufferSize);

	virtual	~CPacket();


	//////////////////////////////////////////////////////////////////////////
	// 패킷 청소.
	//
	// Parameters: 없음.
	// Return: 없음.
	//////////////////////////////////////////////////////////////////////////
	void	Clear(void);


	//////////////////////////////////////////////////////////////////////////
	// 버퍼 사이즈 얻기.
	//
	// Parameters: 없음.
	// Return: (int)패킷 버퍼 사이즈 얻기.
	//////////////////////////////////////////////////////////////////////////
	int	GetBufferSize(void);
	//////////////////////////////////////////////////////////////////////////
	// 현재 사용중인 사이즈 얻기.
	//
	// Parameters: 없음.
	// Return: (int)사용중인 데이타 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int	GetDataSize(void);



	//////////////////////////////////////////////////////////////////////////
	// 버퍼 포인터 얻기.
	//
	// Parameters: 없음.
	// Return: (char *)버퍼 포인터.
	//////////////////////////////////////////////////////////////////////////
	char* GetBufferPtr(void);

	//////////////////////////////////////////////////////////////////////////
	// 버퍼 Pos 이동. (음수이동은 안됨)
	// GetBufferPtr 함수를 이용하여 외부에서 강제로 버퍼 내용을 수정할 경우 사용. 
	//
	// Parameters: (int) 이동 사이즈.
	// Return: (int) 이동된 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		MoveWritePos(int iSize);
	int		MoveReadPos(int iSize);
	/* ============================================================================= */
	// 연산자 오버로딩
	/* ============================================================================= */
	CPacket& operator = (CPacket& clSrcPacket);

	//////////////////////////////////////////////////////////////////////////
	// 넣기.	각 변수 타입마다 모두 만듬.
	//////////////////////////////////////////////////////////////////////////
	CPacket& operator << (unsigned char byValue);
	CPacket& operator << (char chValue)
	{
		Enqueue(&chValue, sizeof(chValue));
		return *this;
	}

	CPacket& operator << (short shValue)
	{
		{
			Enqueue((char*)&shValue, sizeof(shValue));
			return *this;
		}
	}
	CPacket& operator << (unsigned short wValue);

	CPacket& operator << (int iValue)
	{
		Enqueue((char*)&iValue, sizeof(iValue));
		return *this;
	}
	CPacket& operator << (long lValue);
	CPacket& operator << (float fValue);

	CPacket& operator << (__int64 iValue);
	CPacket& operator << (double dValue);


	//////////////////////////////////////////////////////////////////////////
	// 빼기.	각 변수 타입마다 모두 만듬.
	//////////////////////////////////////////////////////////////////////////
	CPacket& operator >> (BYTE& byValue);
	CPacket& operator >> (char& chValue)
	{
		Dequeue(&chValue, sizeof(chValue));
		return *this;
	}

	CPacket& operator >> (short& shValue)
	{
		Dequeue((char*)&shValue, sizeof(shValue));
		return *this;
	}
	CPacket& operator >> (WORD& wValue);

	CPacket& operator >> (int& iValue)
	{
		Dequeue((char*)&iValue, sizeof(iValue));
		return *this;
	}
	CPacket& operator >> (DWORD& dwValue);
	CPacket& operator >> (float& fValue);

	CPacket& operator >> (__int64& iValue);
	CPacket& operator >> (double& dValue);




	//////////////////////////////////////////////////////////////////////////
	// 데이타 얻기.
	//
	// Parameters: (char *)Dest 포인터. (int)Size.
	// Return: (int)복사한 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		GetData(char* chpDest, int iSize);

	//////////////////////////////////////////////////////////////////////////
	// 데이타 삽입.
	//
	// Parameters: (char *)Src 포인터. (int)SrcSize.
	// Return: (int)복사한 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		PutData(char* chpSrc, int iSrcSize);




protected:

	//할당 위치
	char* allocPtr = { nullptr };
	char* allocEndPtr = { nullptr };
	
	char* front = { nullptr };
	char* rear = { nullptr };

	int	m_iBufferSize = { 0 };

	void Enqueue(char* value, int size);
	void Dequeue(char* value, int size);
	//------------------------------------------------------------
	// 현재 버퍼에 사용중인 사이즈.
	//------------------------------------------------------------
	int	m_iDataSize = { 0 };
};

#endif