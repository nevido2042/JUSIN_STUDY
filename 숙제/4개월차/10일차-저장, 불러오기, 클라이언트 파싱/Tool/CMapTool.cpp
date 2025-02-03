// CMapTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "CMapTool.h"
#include "CFileInfo.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "CTerrain.h"

// CMapTool 대화 상자

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CMapTool, pParent), m_iDrawID(0)
{

}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnListBox)
	ON_WM_DROPFILES()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnMapTool)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기


void CMapTool::OnListBox()
{
	UpdateData(TRUE);

	CString	strFindName;

	// GetCurSel : 커서가 선택한 셀의 인덱스 값을 반환
	int	iIndex = m_ListBox.GetCurSel();

	m_ListBox.GetText(iIndex, strFindName);

	auto	iter = m_mapPngImage.find(strFindName);

	if (iter == m_mapPngImage.end())
		return;

	m_Picture.SetBitmap(*(iter->second));

	int i(0);

	for (; i < strFindName.GetLength(); ++i)
	{
		// isdigit : 매개 변수로 넘겨받은 문자가 글자 형태의 문자인지 숫자 형태의 문자인지 
		// 판별하는 함수, 숫자형태의 문자로 판별될 경우 0이 아닌 값을 반환

		if (0 != isdigit(strFindName[i]))
			break;
	}

	// Delete(index, count) : 인덱스 위치로부터 카운트 만큼 문자를 삭제하는 함수
	strFindName.Delete(0, i);

	// _tstoi :  문자열을 정수형으로 변환하는 함수

	m_iDrawID = _tstoi(strFindName);

	UpdateData(FALSE);
}


void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	UpdateData(TRUE);

	CDialog::OnDropFiles(hDropInfo);

	TCHAR	szFilePath[MAX_PATH] = L"";

	TCHAR	szFileName[MAX_STR] = L"";

	// DragQueryFile :  드롭된 파일으 정보를 얻어오는 함수
	// 
	// 두 번째 매개 변수 : 0xffffffff(-1)인 경우 드롭된 파일의 개수를 반환
	int	iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

		CString strRelative = CFileInfo::Convert_RelativePath(szFilePath);

		// PathFindFileName : 경로 중 파일 이름만 추출

		CString strFileName = PathFindFileName(strRelative);

		lstrcpy(szFileName, strFileName.GetString());

		// 확장자 명을 제거하는 함수
		PathRemoveExtension(szFileName);

		strFileName = szFileName;

		auto	iter = m_mapPngImage.find(strFileName);

		if(iter == m_mapPngImage.end())
		{
			CImage* pPngImage = new CImage;
			pPngImage->Load(strRelative);

			m_mapPngImage.insert({ strFileName, pPngImage });
			m_ListBox.AddString(szFileName);

		}
	}

	Horizontal_Scroll();

	UpdateData(FALSE);
}

void CMapTool::Horizontal_Scroll()
{
	CString		strName;

	CSize		Size;

	int		iWidth(0);

	CDC* pDC = m_ListBox.GetDC();

	for(int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strName);

		// GetTextExtent : 매개 변수에 해당하는 문자열의 길이를 픽셀 단위로 변환
		Size = pDC->GetTextExtent(strName);

		if (Size.cx > iWidth)
			iWidth = Size.cx;
	}

	m_ListBox.ReleaseDC(pDC);

	// GetHorizontalExtent : 리스트 박스가 가로로 스크롤 할 수 있는 최대 범위를 얻어오는 함수
	if (iWidth > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iWidth);

}


void CMapTool::OnDestroy()
{
	CDialog::OnDestroy();

	for_each(m_mapPngImage.begin(), m_mapPngImage.end(), [](auto& MyPair)
		{
			MyPair.second->Destroy();
			Safe_Delete(MyPair.second);
		});

	m_mapPngImage.clear();
}


void CMapTool::OnMapTool()
{
	CFileDialog		Dlg(FALSE,		// TRUE(불러오기), FALSE(다른 이름으로 저장) 모드 지정
		L"dat",		// default 확장자명
		L"*.dat",	// 대화 상자에 표시될 최초 파일명
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,	// 읽기 전용 체크 박스 숨김 | 중복된 이름으로 파일 저장 시 경고 메세지 띄움
		L"Data Files(*.dat) | *.dat ||", // 대화 상자에 표시될 파일 형식
		this);	// 부모 윈도우 주소

	TCHAR	szPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szPath);

	PathRemoveFileSpec(szPath);

	lstrcat(szPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
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


		CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(GetParentFrame());
		CToolView* pView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

		CTerrain* pTerrain = pView->m_pTerrain;

		vector<TILE*>& vecTile = pTerrain->Get_VecTile();

		if (vecTile.empty())
			return;

		DWORD dwByte(0);

		for (auto& pTile : vecTile)
		{
			WriteFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}
}
