
#include "stdafx.h"
#include "hex2bin.h"



		//��hex�ַ�ת��Ϊ�ֽ���
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
	DWORD addr = -1;//�׵�ַ
	DWORD offset = 0;//hex����ƫ��
	DWORD binoffset = 0;
	CString temp;//��ʱ�ַ�����
	DWORD char_num;//���е��ֽڸ���
	*hexaddr = 0;
	while (!hex.IsEmpty())
	{
		offset = hex.Find(_T(":"), 0);//�ҵ���һ��":"���ֵ�λ��
		hex.Delete(0, offset + 1);//ɾ��":"֮ǰ�����ݣ�����":"
		temp = hex.Mid( 6,2);
		if (temp == _T("04"))//�洢λ��
		{
			temp = hex.Left(2);
			if (temp == _T("02"))//��ַ���������ֽ�
			{
				char_num= hex2num(temp);//�����ֽڸ���
				temp = hex.Mid(8, char_num * 2);
				addr= hex2num(temp)<<16;//����ַ
				offset = hex.Find(_T(":"), 0);//�ҵ���һ��":"���ֵ�λ��
				temp = hex.Mid(offset + 3, 4);//��ȡ��ַ��16λ
				addr |= hex2num(temp);//�ҵ��˳����ַ
				if (addr == 0x08002800)
				{
					*hexaddr = addr;
				}
				else if (*hexaddr == 0)
				{

				}
				else if (addr- *hexaddr!= binoffset)//���������ĳ����
				{
					*binsize = binoffset;
					break;//ת�������

				}
			}
			else//��ʽ����
			{
				return 2;
			}
		}
		else if (temp == _T("00"))//�����
		{
			if (*hexaddr == 0x08002800)//ֻ�������ַ��ת��
			{
				temp = hex.Left(2);
				char_num = hex2num(temp);//�����ֽڸ���
				DWORD i = 0;
				for (i = 0; i < char_num;i++)//�洢һ�е�����
				{
					temp = hex.Mid(8+i*2,  2);
					bin[binoffset] = hex2num(temp);
					binoffset++;
				}

			}
		}
		else if (temp == _T("05"))//ʲôҲ����
		{

		}
		else if (temp == _T("01"))//���ݽ���
		{
			temp = hex.Left(2);
			if (temp == _T("00"))//û������
			{
				*binsize = binoffset ;
				//*hexaddr = addr;
				break;//ת�������
			}
			else//��ʽ����
			{
				return 2;
			}

		}
		else //��֧�ֵĲ���
		{
			return 1;
		}
	}

	return 0;
}
