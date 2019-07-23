#include "stdafx.h"
#include "CEventLog.h"

constexpr auto MAX_BUF_SIZE = 256;

bool CEventLog::GetIsOpened() const throw()
{
	return m_hReadEventLog != NULL && m_hWriteEventLog;
}

long CEventLog::GetRecordCount() const throw()
{
	DWORD numberOfRecords1 = 0x00;
	DWORD numberOfRecords2 = 0x00;
	if (GetNumberOfEventLogRecords(m_hReadEventLog, &numberOfRecords1) == TRUE &&
		GetNumberOfEventLogRecords(m_hWriteEventLog, &numberOfRecords2) == TRUE && 
		numberOfRecords1 == numberOfRecords2)
		return numberOfRecords1;
	return -1;
}

CEventLog::CEventLog(LPCTSTR lpSourceName)
{
	m_lpServerName = new TCHAR[MAX_BUF_SIZE];
	ZeroMemory(m_lpServerName, sizeof(TCHAR) * MAX_BUF_SIZE);
	m_lpSourceName = new TCHAR[MAX_BUF_SIZE];
	ZeroMemory(m_lpSourceName, sizeof(TCHAR) * MAX_BUF_SIZE);

	if (lpSourceName == NULL)
		throw _T("Argument \"lpSourceName\" is NULL.");

	_tcscpy_s(m_lpSourceName, _tcslen(lpSourceName), lpSourceName);
}

CEventLog::CEventLog(LPCTSTR lpServerName, LPCTSTR lpSourceName)
	:CEventLog(lpSourceName)
{
	if (lpServerName != NULL)
		_tcscpy_s(m_lpServerName, _tcslen(lpServerName), lpServerName);

	m_hReadEventLog = OpenEventLog(m_lpServerName, m_lpSourceName);
	if (m_hReadEventLog == NULL)
		m_hWriteEventLog = RegisterEventSource(lpServerName, lpSourceName);

	m_hReadEventLog = OpenEventLog(m_lpServerName, m_lpSourceName);
}

CEventLog::~CEventLog()
{
	if (m_hReadEventLog != NULL)
		CloseEventLog(m_hReadEventLog), m_hReadEventLog = NULL;
	if (m_hWriteEventLog != NULL)
		DeregisterEventSource(m_hWriteEventLog), m_hWriteEventLog = NULL;

	if (m_lpServerName != NULL)
		delete[] m_lpServerName, m_lpServerName = NULL;
	if (m_lpSourceName != NULL)
		delete[] m_lpSourceName, m_lpSourceName = NULL;
}

bool CEventLog::Clear()
{
	return Clear(NULL);
}

bool CEventLog::Clear(LPCTSTR lpBackupFileName)
{
	return ClearEventLog(m_hReadEventLog, lpBackupFileName);
}

bool CEventLog::Backup(LPCTSTR lpBackupFileName)
{
	return BackupEventLog(m_hReadEventLog, lpBackupFileName);
}
