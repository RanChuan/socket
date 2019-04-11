#include "stdafx.h"//���ͷ�ļ�һ��Ҫ���ڵ�һ

#include "crc8_16.h"
#include "My_Config.h"








//������������÷ֺŷֿ���CStringת��������������

int str_to_int(CString buf, int *ints)
{
	int offset = 0;
	CString temp;
	int trant_num = 0;//ת�������ָ���
	while (!buf.IsEmpty())//����ַ�����Ϊ��
	{
		offset = buf.Find(_T(";"), 0);//�ҵ���һ��";"���ֵ�λ��
		if (offset == -1)//û���ҵ��ֺ�,ֻ��һ��������
		{
			ints[trant_num] = _ttoi(buf);
			trant_num++;
			return trant_num;
		}
		else
		{
			if (offset == 0)//�ֺų����ڵ�һλ
			{
				buf.Delete(0, 1);
				continue;
			}
			temp = buf.Left(offset);//��ȡ��ǰ�������
			ints[trant_num] = _ttoi(temp);
			trant_num++;
			buf.Delete(0, offset + 1);


		}
	}
	return trant_num;
}



















//��������ģʽ,�������л������ݳ���
int my_config::my_entercfg(char * buff, int befflen, unsigned short addr)
{
	int ret = 0;
	unsigned char crc[2];
	buff[0] = 0xff;
	buff[1] = 0xff;
	buff[2] = addr >> 8;
	buff[3] = addr;
	buff[4] = 0x06;
	buff[5] = 0x00;
	buff[6] = 0x01;
	buff[7] = 0x01;//��������ģʽ
	Get_Crc16((unsigned char*)buff, buff[6]+7, (u8*)crc);
	buff[8] = crc[0];
	buff[9] = crc[1];
	ret = 10;
	return ret;
}

//�˳�����ģʽ
int my_config::my_esccfg(char * buff, int befflen, unsigned short addr)
{
	unsigned char crc[2];
	int ret = 0;
	buff[0] = 0xff;
	buff[1] = 0xff;
	buff[2] = addr >> 8;
	buff[3] = addr;
	buff[4] = 0x06;
	buff[5] = 0x00;
	buff[6] = 0x01;
	buff[7] = 0x00;//����ģʽ
	Get_Crc16((unsigned char*)buff, buff[6] + 7, (u8*)crc);
	buff[8] = crc[0];
	buff[9] = crc[1];
	ret = 10;
	return ret;
}


int my_config::my_cfgaddr(char * buff, int befflen, unsigned short addr, unsigned short newaddr, unsigned char type, unsigned char kttype)
{
	unsigned char crc[2];
	int ret = 0;
	buff[0] = 0xff;
	buff[1] = 0xff;
	buff[2] = addr >> 8;
	buff[3] = addr;
	buff[4] = 0x07;
	buff[5] = 0x00;
	buff[6] = 0x04;
	buff[7] = newaddr>>8;
	buff[8] = newaddr;
	buff[9] = type;
	buff[10] = kttype;
	Get_Crc16((unsigned char*)buff, buff[6] + 7, (u8*)crc);
	buff[buff[6] + 7] = crc[0];
	buff[buff[6] + 8] = crc[1];
	ret = buff[6] + 8 + 1;
	return ret;
}

int my_config::my_cfgstudy(unsigned short addr,unsigned char type)
{
	unsigned char data[100];
	unsigned char recv[100];
	unsigned char crc[2];
	int ret = 0;
	data[0] = 0xff;
	data[1] = 0xff;
	data[2] = addr >> 8;
	data[3] = addr;
	data[4] = 0x08;
	data[5] = 0x00;
	data[6] = 0x02;
	data[7] = 2;
	data[8] = type;
	Get_Crc16((unsigned char*)data, data[6] + 7, (u8*)crc);
	data[data[6] + 7] = crc[0];
	data[data[6] + 8] = crc[1];
	return ret;
}

int my_config::my_cfgcenter(unsigned short addr, unsigned short *daddrs, unsigned short nom,unsigned char *dtype)
{
	unsigned char data[100] = { 0 };
	unsigned char recv[100] = { 0 };
	unsigned char crc[2];
	unsigned short data_len = 0;
	int ret = 0;
	unsigned short i = 0;
	data_len = nom * 3 + 2;
	data[0] = 0xff;
	data[1] = 0xff;
	data[2] = addr >> 8;
	data[3] = addr;
	data[4] = 0x08;
	data[5] = data_len>>8;
	data[6] = data_len;
	data[7] = 1;//����1�����ü������е��豸��ַ
	for (i = 0; i < nom; i++)
	{
		data[i*3+8] = *daddrs>>8;
		data[i*3+9] = *daddrs;
		data[i * 3 + 10] = *dtype;
		daddrs+=2;//������int����ǿ��ת��Ϊshort�����Ե�ַ��2
		dtype++;
	}
	Get_Crc16((unsigned char*)data, ((data[5]<<8)|data[6]) + 7, (u8*)crc);
	data[((data[5] << 8) | data[6]) + 7] = crc[0];
	data[((data[5] << 8) | data[6]) + 8] = crc[1];
	return ret;
}

int my_config::my_cfgcenterip(unsigned short addr, CenterIP ip)
{
	unsigned char data[100];
	unsigned char recv[100];
	unsigned char crc[2];
	unsigned short data_len = 0;
	int ret = 0;
	unsigned short i = 0;
	data_len = 16+1;
	data[0] = 0xff;
	data[1] = 0xff;
	data[2] = addr >> 8;
	data[3] = addr;
	data[4] = 0x08;
	data[5] = data_len >> 8;
	data[6] = data_len;
	data[7] = 3;//����3�����ü������е�IP��ַ
	data[8] = ip.my_ip[0];
	data[9] = ip.my_ip[1];
	data[10] = ip.my_ip[2];
	data[11] = ip.my_ip[3];
	data[12] = ip.my_com >> 8;
	data[13] = ip.my_com;
	data[14] = ip.fw_ip[0];
	data[15] = ip.fw_ip[1];
	data[16] = ip.fw_ip[2];
	data[17] = ip.fw_ip[3];
	data[18] = ip.fw_com >> 8;
	data[19] = ip.fw_com;
	data[20] = ip.wg_ip[0];
	data[21] = ip.wg_ip[1];
	data[22] = ip.wg_ip[2];
	data[23] = ip.wg_ip[3];

	Get_Crc16((unsigned char*)data, ((data[5] << 8) | data[6]) + 7, (u8*)crc);
	data[((data[5] << 8) | data[6]) + 7] = crc[0];
	data[((data[5] << 8) | data[6]) + 8] = crc[1];
	return ret;
}

int my_config::my_complitetime(unsigned char *tt)
{
	unsigned char data[100];
	unsigned char recv[100];
	unsigned char crc[2];
	int addr = 0;
	int ret = 0;
	data[0] = 0xff;
	data[1] = 0xff;
	data[2] = addr >> 8;
	data[3] = addr;
	data[4] = 0x01;
	data[5] = 0x00;
	data[6] = 0x02;
	data[7] = 0x00;
	data[8] = 0x32;
	Get_Crc16((unsigned char*)data, data[6] + 7, (u8*)crc);
	data[((data[5] << 8) | data[6]) + 7] = crc[0];
	data[((data[5] << 8) | data[6]) + 8] = crc[1];
	return ret;
}
		
						//���������ݳ���
int my_config::my_enteriap(char *data,unsigned int datasize)
{

	int ret = 0;
	unsigned char crc[2];
	data[0] = 0xff;
	data[1] = 0xff;
	data[2] = 0x00;
	data[3] = 0x00;
	data[4] = 0x05;
	data[5] = 0;
	data[6] = 9;
	data[7] = 0x01;//���³���
	data[8] = datasize >> 24;
	data[9] = datasize>>16;
	data[10] = datasize >> 8;
	data[11] = datasize;
	data[12] = 0x08;
	data[13] = 0x00;
	data[14] = 0x28;
	data[15] = 0x00;
	Get_Crc16((unsigned char*)data, data[6] + 7, (u8*)crc);
	data[data[6] +7] = crc[0];
	data[data[6] +7+1] = crc[1];
	ret = data[6] + 7 + 2;
	return ret;
}

int my_config::my_updataing(unsigned short length, unsigned short num_now, unsigned short num_all, unsigned char *buff)
{
	unsigned char data[2048] = { 0 };
	unsigned char recv[2048] = { 0 };
	int ret = 0;
	unsigned char crc[2];
	data[0] = 0xff;
	data[1] = 0xff;
	data[2] = 0x00;
	data[3] = 0x00;
	data[4] = 0x05;
	data[5] = (length + 5)>>8;
	data[6] = length+5;
	data[7] = 0x03;//���³���
	data[8] = num_now >> 8;
	data[9] = num_now;
	data[10] = num_all >> 8;
	data[11] = num_all;
	int t = 0;
	for (t = 0; t < length; t++)
	{
		data[12 + t] = buff[t];
	}
	Get_Crc16((unsigned char*)data, (length + 5) + 7, (u8*)crc);
	data[(length + 5) + 7] = crc[0];
	data[(length + 5) + 7 + 1] = crc[1];
	ret = (length + 5) + 7 + 2;
	for (t = 0; t <ret; t++)
	{
		buff[t] = data[t];
	}
	return ret;
}


			//����ָ����ַ���豸
int my_config::my_reset(unsigned short addr)
{
	unsigned char data[100] = { 0 };
	unsigned char recv[100] = { 0 };
	int ret = 0;
	unsigned char crc[2];
	data[0] = 0xff;
	data[1] = 0xff;
	data[2] = addr>>8;
	data[3] = addr;
	data[4] = 0x09;
	data[5] = 0;
	data[6] = 1;
	data[7] = 0x02;//���³���
	Get_Crc16((unsigned char*)data, data[6] + 7, (u8*)crc);
	data[data[6] + 7] = crc[0];
	data[data[6] + 7 + 1] = crc[1];
	return ret;
}



//У���豸����
int my_config::my_checkreturn(char * data, int datalen)
{
	char crc[2] = { 0 };
	if (((unsigned char)data[0] != 0xff) || ((unsigned char)data[1] != 0xff))
		return -2;
	Get_Crc16((unsigned char*)data, data[6] + 7, (u8*)crc);
	if (((data[data[6] + 7]) == crc[0]) && ((data[data[6] + 8]) == crc[1]))
	{
		return ((data[7] << 8) | data[8]);
	}
	else
	{
		return -3;
	}
}


					//��ȡ��������
CString my_config::my_geterrtype(short errtype)
{
	CString err;
	switch (errtype)
	{
	case 0:
		err = _T("�ɹ�");
		break;
	case 1:
		err = _T("δ֪����");
		break;
	case 2:
		err = _T("���ݴ���");
		break;
	case 3:
		err = _T("���ݳ��ȴ���");
		break;
	case 4:
		err = _T("��Ҫ��ȡ�����ݳ��ȴ���");
		break;
	case 5:
		err = _T("��֧�ֵ���������");
		break;
	case 6:
		err = _T("û������");
		break;
	case 7:
		err = _T("�ɼ����ĵ�ַ����");
		break;
	case 8:
		err = _T("���ǿ�����״̬");
		break;
	case 9:
		err = _T("��Ҫ���Ƶ��豸�뱾������");
		break;
	case 10:
		err = _T("��λ��������ʱ");
		break;
	case -1:
		err = _T("û���յ���λ���ķ���");
		break;
	case -2:
		err = _T("ʧ��");
		break;
	case -3:
		err = _T("CRCУ��ûͨ��");
		break;
	case -4:
		err = _T("�յ��˸���");
		break;
	case -5:
		err = _T("û�д򿪴���");
		break;
	case -6:
		err = _T("û��ָ���˿ں�");
		break;
	default:
		err = _T("δ����Ĵ���");
		break;
	}
	return err;
}




