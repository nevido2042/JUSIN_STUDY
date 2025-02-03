// CUnitTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "CUnitTool.h"


// CUnitTool 대화 상자

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CUnitTool, pParent)
	, m_strName(_T(""))
	, m_iAttack(0)
	, m_iHp(0)
	, m_strFindName(_T(""))
{

}

CUnitTool::~CUnitTool()
{
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT5, m_strName);
	DDX_Text(pDX, IDC_EDIT3, m_iAttack);
	DDX_Text(pDX, IDC_EDIT4, m_iHp);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[2]);
	DDX_Control(pDX, IDC_CHECK1, m_Check[0]);
	DDX_Control(pDX, IDC_CHECK2, m_Check[1]);
	DDX_Control(pDX, IDC_CHECK3, m_Check[2]);
	DDX_Control(pDX, IDC_BUTTON3, m_Bitmap);
	DDX_Text(pDX, IDC_EDIT6, m_strFindName);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CUnitTool::OnAdd)
	ON_LBN_SELCHANGE(IDC_LIST1, &CUnitTool::OnListBox)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON6, &CUnitTool::OnDelete)
	ON_BN_CLICKED(IDC_BUTTON4, &CUnitTool::OnSave)
	ON_BN_CLICKED(IDC_BUTTON5, &CUnitTool::OnLoad)
	ON_EN_CHANGE(IDC_EDIT6, &CUnitTool::OnFindData)
END_MESSAGE_MAP()






void CUnitTool::OnAdd()
{
	UpdateData(TRUE);

	UNITDATA* pUnit = new UNITDATA;

	pUnit->strName = m_strName;
	pUnit->iAttack = m_iAttack;
	pUnit->iHp = m_iHp;

	for (int i = 0; i < 3; ++i)
	{
		// 버튼 체크 상태를 확인
		if (m_Radio[i].GetCheck())
		{
			pUnit->byJobIndex = i;
			break;
		}
	}

	pUnit->byItem = 0x00;

	if (m_Check[0].GetCheck())
		pUnit->byItem |= RUBY;

	if (m_Check[1].GetCheck())
		pUnit->byItem |= DIAMOND;

	if (m_Check[2].GetCheck())
		pUnit->byItem |= SAPPHIRE;



	// AddString : 리스트 박스에 문자열을 삽입
	m_ListBox.AddString(pUnit->strName);

	m_mapUnitData.insert({pUnit->strName, pUnit});

	UpdateData(FALSE);
}


void CUnitTool::OnListBox()
{
	UpdateData(TRUE);

	CString	strFindName;

	// GetCurSel : 커서가 선택한 셀의 인덱스 값을 반환
	int	iIndex = m_ListBox.GetCurSel();

	m_ListBox.GetText(iIndex, strFindName);

	auto	iter = m_mapUnitData.find(strFindName);

	if (iter == m_mapUnitData.end())
		return;

	m_strName = iter->second->strName;
	m_iHp = iter->second->iHp;
	m_iAttack = iter->second->iAttack;

	for (int i = 0; i < 3; ++i)
	{
		m_Radio[i].SetCheck(FALSE);
		m_Check[i].SetCheck(FALSE);
	}

	m_Radio[iter->second->byJobIndex].SetCheck(TRUE);

	if(iter->second->byItem & RUBY)
		m_Check[0].SetCheck(TRUE);

	if (iter->second->byItem & DIAMOND)
		m_Check[1].SetCheck(TRUE);

	if (iter->second->byItem & SAPPHIRE)
		m_Check[2].SetCheck(TRUE);


	UpdateData(FALSE);
	
}


void CUnitTool::OnDestroy()
{
	CDialog::OnDestroy();

	for_each(m_mapUnitData.begin(), m_mapUnitData.end(), CDeleteMap());
	m_mapUnitData.clear();
}


BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	HBITMAP		hBitmap = (HBITMAP)LoadImage(NULL,
											L"../Texture/JusinLogo1.bmp", 
											IMAGE_BITMAP, 
											100, 
											50, 
											LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	m_Bitmap.SetBitmap(hBitmap);

	return TRUE; 
}


void CUnitTool::OnDelete()
{
	UpdateData(TRUE);

	CString	strFindName;

	// GetCurSel : 커서가 선택한 셀의 인덱스 값을 반환
	int	iIndex = m_ListBox.GetCurSel();

	if (LB_ERR == iIndex)
		return;

	m_ListBox.GetText(iIndex, strFindName);

	auto	iter = m_mapUnitData.find(strFindName);

	if (iter == m_mapUnitData.end())
		return;

	Safe_Delete(iter->second);
	m_mapUnitData.erase(strFindName);

	//DeleteString : 해당 인덱스의 리스트 목록을 제거
	m_ListBox.DeleteString(iIndex);

	UpdateData(FALSE);
}
void CUnitTool::OnFindData()
{
	UpdateData(TRUE);

	auto&	iter = m_mapUnitData.find(m_strFindName);


	if (iter == m_mapUnitData.end())
		return;

	// FindString : 리스트 박스에서 일치하는 문자열을 얻어옴(0번 인덱스 목록부터 탐색)
	int iIndex = m_ListBox.FindString(0, m_strFindName);

	if (LB_ERR == iIndex)
		return;

	m_ListBox.SetCurSel(iIndex);

	m_strName = iter->second->strName;
	m_iHp = iter->second->iHp;
	m_iAttack = iter->second->iAttack;

	for (int i = 0; i < 3; ++i)
	{
		m_Radio[i].SetCheck(FALSE);
		m_Check[i].SetCheck(FALSE);
	}

	m_Radio[iter->second->byJobIndex].SetCheck(TRUE);

	if (iter->second->byItem & RUBY)
		m_Check[0].SetCheck(TRUE);

	if (iter->second->byItem & DIAMOND)
		m_Check[1].SetCheck(TRUE);

	if (iter->second->byItem & SAPPHIRE)
		m_Check[2].SetCheck(TRUE);



	UpdateData(FALSE);

}

void CUnitTool::OnSave()
{
	CFileDialog		Dlg(FALSE,		// TRUE(불러오기), FALSE(다른 이름으로 저장) 모드 지정
						L"dat",		// default 확장자명
						L"*.dat",	// 대화 상자에 표시될 최초 파일명
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,	// 읽기 전용 체크 박스 숨김 | 중복된 이름으로 파일 저장 시 경고 메세지 띄움
						L"Data Files(*.dat) | *.dat ||", // 대화 상자에 표시될 파일 형식
						this);	// 부모 윈도우 주소

	// DoModal : 대화 상자를 실행

	TCHAR	szPath[MAX_PATH] = L"";

	// 현재 프로젝트의 경로를 얻어오는 함수(절대 경로)
	GetCurrentDirectory(MAX_PATH, szPath);
	
	// PathRemoveFileSpec : 전체 경로에서 파일 이름만 잘라주는 함수
	// 경로 상에 파일 이름이 없을 경우엔 마지막 폴더명을 잘라낸다.

	PathRemoveFileSpec(szPath);

	lstrcat(szPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		// GetPathName : 선택된 경로를 반환
		// GetString : 원시 문자열의 형태로 반환

		CString	str = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, 
								GENERIC_WRITE, 
								0, 0, 
								CREATE_ALWAYS, 
								FILE_ATTRIBUTE_NORMAL, 
								0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD	dwByte(0), dwStrByte(0);

		for (auto& MyPair : m_mapUnitData)
		{
			// key 값 저장

			dwStrByte = sizeof(TCHAR) * (MyPair.first.GetLength() + 1);

			WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, MyPair.first.GetString(), dwStrByte, &dwByte, nullptr);

			// value 값 저장

			WriteFile(hFile, &(MyPair.second->byItem), sizeof(BYTE), &dwByte, nullptr);
			WriteFile(hFile, &(MyPair.second->byJobIndex), sizeof(BYTE), &dwByte, nullptr);
			WriteFile(hFile, &(MyPair.second->iAttack), sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &(MyPair.second->iHp), sizeof(int), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}


}

void CUnitTool::OnLoad()
{
	UpdateData(TRUE);

	CFileDialog		Dlg(TRUE,		// TRUE(불러오기), FALSE(다른 이름으로 저장) 모드 지정
		L"dat",		// default 확장자명
		L"*.dat",	// 대화 상자에 표시될 최초 파일명
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,	// 읽기 전용 체크 박스 숨김 | 중복된 이름으로 파일 저장 시 경고 메세지 띄움
		L"Data Files(*.dat) | *.dat ||", // 대화 상자에 표시될 파일 형식
		this);	// 부모 윈도우 주소

	// DoModal : 대화 상자를 실행

	TCHAR	szPath[MAX_PATH] = L"";

	// 현재 프로젝트의 경로를 얻어오는 함수(절대 경로)
	GetCurrentDirectory(MAX_PATH, szPath);

	// PathRemoveFileSpec : 전체 경로에서 파일 이름만 잘라주는 함수
	// 경로 상에 파일 이름이 없을 경우엔 마지막 폴더명을 잘라낸다.

	PathRemoveFileSpec(szPath);

	lstrcat(szPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		for (auto& MyPair : m_mapUnitData)
			delete MyPair.second;

		m_mapUnitData.clear();

		// ResetContent : 리스트 박스 목록 초기화 함수
		m_ListBox.ResetContent();


		CString	str = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath,
			GENERIC_READ,
			0, 0,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD	dwByte(0), dwStrByte(0);
		UNITDATA		tData{};

		while (true)
		{
			// key 값 로드

			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

			TCHAR* pName = new TCHAR[dwStrByte];

			ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

			// value 값 로드

			ReadFile(hFile, &(tData.byItem), sizeof(BYTE), &dwByte, nullptr);
			ReadFile(hFile, &(tData.byJobIndex), sizeof(BYTE), &dwByte, nullptr); 
			ReadFile(hFile, &(tData.iAttack), sizeof(int), &dwByte, nullptr); 
			ReadFile(hFile, &(tData.iHp), sizeof(int), &dwByte, nullptr); 

			if (0 == dwByte)
			{
				delete[] pName;
				break;
			}

			UNITDATA* pUnit = new UNITDATA;

			pUnit->strName = pName;

			delete[]pName;

			pUnit->byItem = tData.byItem;
			pUnit->byJobIndex = tData.byJobIndex;
			pUnit->iHp = tData.iHp;
			pUnit->iAttack = tData.iAttack;

			m_mapUnitData.insert({ pUnit->strName, pUnit });

			m_ListBox.AddString(pUnit->strName);
		}

		CloseHandle(hFile);
	}

	UpdateData(FALSE);
}



