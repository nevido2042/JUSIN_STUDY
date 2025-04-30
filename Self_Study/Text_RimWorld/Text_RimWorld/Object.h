#pragma once
class CObject abstract
{
public:
	CObject();
	virtual ~CObject();
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
};

