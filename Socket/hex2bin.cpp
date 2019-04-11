
#include "stdafx.h"
#include "hex2bin.h"



		//将hex字符转化为字节数
DWORD hex2num(CString hex)
{
	DWORD ret = 0;
	while (!hex.IsEmpty())
	{
		ret = ret<<4;
		if ((hex[0] >= '0') && (hex[0] <= '9'))
		{
			ret |= hex[0] - '0';
		}
		else if ((hex[0] >= 'a') && (hex[0] <= 'z'))
		{
			ret |= hex[0] - 'a'+10;

		}
		else if ((hex[0] >= 'A') && (hex[0] <= 'Z'))
		{
			ret |= hex[0] - 'A' + 10;
		}
		else
		{
			return 0;
		}
		hex.Delete(0, 1);
	}
	return ret;
}
















int Hex2Bin::hex2bin(unsigned char *bin, CString hex,DWORD *hexaddr,DWORD *binsize)
{
	DWORD addr = -1;//首地址
	DWORD offset = 0;//hex单次偏移
	DWORD binoffset = 0;
	CString temp;//临时字符变量
	DWORD char_num;//单行的字节个数
	*hexaddr = 0;
	while (!hex.IsEmpty())
	{
		offset = hex.Find(_T(":"), 0);//找到第一个":"出现的位置
		hex.Delete(0, offset + 1);//删除":"之前的数据，包括":"
		temp = hex.Mid( 6,2);
		if (temp == _T("04"))//存储位置
		{
			temp = hex.Left(2);
			if (temp == _T("02"))//地址段是两个字节
			{
				char_num= hex2num(temp);//更新字节个数
				temp = hex.Mid(8, char_num * 2);
				addr= hex2num(temp)<<16;//主地址
				offset = hex.Find(_T(":"), 0);//找到第一个":"出现的位置
				temp = hex.Mid(offset + 3, 4);//获取地址低16位
				addr |= hex2num(temp);//找到了程序地址
				if (addr == 0x08002800)
				{
					*hexaddr = addr;
				}
				else if (*hexaddr == 0)
				{

				}
				else if (addr- *hexaddr!= binoffset)//不是连续的程序段
				{
					*binsize = binoffset;
					break;//转换已完成

				}
			}
			else//格式有误
			{
				return 2;
			}
		}
		else if (temp == _T("00"))//程序段
		{
			if (*hexaddr == 0x08002800)//只有这个地址才转换
			{
				temp = hex.Left(2);
				char_num = hex2num(temp);//更新字节个数
				DWORD i = 0;
				for (i = 0; i < char_num;i++)//存储一行的数据
				{
					temp = hex.Mid(8+i*2,  2);
					bin[binoffset] = hex2num(temp);
					binoffset++;
				}

			}
		}
		else if (temp == _T("05"))//什么也不做
		{

		}
		else if (temp == _T("01"))//数据结束
		{
			temp = hex.Left(2);
			if (temp == _T("00"))//没有数据
			{
				*binsize = binoffset ;
				//*hexaddr = addr;
				break;//转换已完成
			}
			else//格式有误
			{
				return 2;
			}

		}
		else //不支持的参数
		{
			return 1;
		}
	}

	return 0;
}
