#pragma once
class CObj abstract
{
protected:
	char m_szName[32];

protected:
	CObj();
	virtual ~CObj();

public:
	virtual void Render() = 0;
	void Set_Name(const char* _szName);
};

