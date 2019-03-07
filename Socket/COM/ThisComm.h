#pragma once
class ThisComm
{
public:
	ThisComm();
	~ThisComm();
	void OpenComm(LPCWSTR lpcomid);
	void SetComm(LPDCB lpcombcd);
	void CloseComm();
	int SendComm(unsigned char * data, unsigned int length);
	int RecvComm(unsigned char * data, unsigned int length);
	bool if_open;
private:
	HANDLE hcom;
	DCB comstate;
};

