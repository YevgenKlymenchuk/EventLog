#pragma once

#ifdef EVENTLOG_EXPORTS 
/*Enabled as "export" while compiling the dll project*/
#define DLLEXPORT __declspec(dllexport)  
#else
/*Enabled as "import" in the Client side for using already created dll file*/
#define DLLEXPORT __declspec(dllimport)  
#endif

typedef struct _EVENTLOGRECORDEX : EVENTLOGRECORD
{
	LPCTSTR Message;
}
EVENTLOGRECORDEX, *PEVENTLOGRECORDEX;

class DLLEXPORT CEventLog
{
public:
	bool GetIsOpened() const throw();
	long GetRecordCount() const throw();
private:
	HANDLE m_hReadEventLog;
	HANDLE m_hWriteEventLog;
protected:
	LPTSTR m_lpServerName;
	LPTSTR m_lpSourceName;
public:
	CEventLog(LPCTSTR lpSourceName);
	CEventLog(LPCTSTR lpServerName, LPCTSTR lpSourceName);
	virtual ~CEventLog();
public:
	__declspec(property(get = GetIsOpened)) bool IsOpened;
	__declspec(property(get = GetRecordCount)) long RecordCount;
public:
	bool Clear();
	bool Clear(LPCTSTR lpBackupFileName);
	bool Backup(LPCTSTR lpBackupFileName);
};

