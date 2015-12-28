#pragma once

class IClientBind
{
public:
	virtual BOOL Init(const TCHAR * lpszConfig) = 0;

	// 对象增删改 (ID+类型)
	virtual BOOL AddObjct(int nID,const char * pInBuffer,size_t nInBufferLength) = 0;
	virtual BOOL DelObject(int nID) = 0;
	virtual BOOL UpdateObject(int nID,const char  * pInBuffer,size_t nInBufferLength) = 0;

	// 系统
};