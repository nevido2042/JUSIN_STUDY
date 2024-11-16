#pragma once
#include "Obj.h"
#include "Weapon.h"
#include "Armor.h"

class CPawn :
    public CObj
{
protected:
    int m_iHP; //체력
    int m_iMaxHP;//최대 체력
    int m_iSTR;//공격력

    int m_iMoney;

    CWeapon* m_pWeapon;
    CArmor* m_pArmor;
public:
    CPawn();
    virtual ~CPawn();
public:
    void Initialize() override;
public:
    void Render() override;
    void Release() override;
    void Render_Equipment();
    void Add_MaxHP(int _iMaxHP)
    {
        m_iMaxHP += _iMaxHP;
    }
    void Add_STR(int _iSTR)
    {
        m_iSTR += _iSTR;
    }
    CWeapon* Get_Weapon()
    {
        return m_pWeapon;
    }
    void Set_Weapon(CWeapon* _pWeapon)
    {
        m_pWeapon = _pWeapon;
    }
    CArmor* Get_Armor()
    {
        return m_pArmor;
    }
    void Set_Armor(CArmor* _pArmor)
    {
        m_pArmor = _pArmor;
    }
    int Get_HP()
    {
        return m_iHP;
    }
    void Set_HP(int _iHP);
    int Get_STR()
    {
        return m_iSTR;
    }
    void Set_STR(int _iSTR);
    int Get_MaxHP()
    {
        return m_iMaxHP;
    }
    void Set_MaxHP(int _iMaxHP)
    {
        m_iMaxHP = _iMaxHP;
    }
    void Add_Money(int _iMoney)
    {
        m_iMoney += _iMoney;
    }
    int Get_Money()
    {
        return m_iMoney;
    }
public:
    void Attack(CObj* _pTarget);
    void Full_HP()
    {
        m_iHP = m_iMaxHP;
    }
};

