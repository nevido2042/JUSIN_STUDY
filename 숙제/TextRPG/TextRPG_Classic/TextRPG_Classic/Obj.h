#pragma once
class CObj abstract
{
protected:
	char m_szName[32];

public:
	CObj();
	virtual ~CObj();
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Relase() = 0;


public:
	virtual void Render() = 0;
	void Set_Name(const char* _szName);
};

