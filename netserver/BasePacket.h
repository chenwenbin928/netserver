/*
 *
 * Copyright (C) 2009 jack.wgm, microcai.
 * For conditions of distribution and use, see copyright notice 
 * in (http://code.google.com/p/netsever/source/browse/trunk/COPYING)
 *
 * Author: jack.wgm
 * Email:  jack.wgm@gmail.com
 */

#ifndef BASEPACKET_H__
#define BASEPACKET_H__

#if _MSC_VER
#pragma once
#endif

// ����,�����˸ö��뷽ʽ,1�ֽڶ���.
#pragma pack(push, 1)

// ������.
typedef struct _tagHeart
{
	UINT type;          // �������.
	UINT packsize;      // �����С.

	UINT session;       // �Ự��־.
}* packHeartPtr, packHeart;

#define packHeartSize sizeof(packHeart)


// ��½��.
typedef struct _tagLogon
{
	UINT type;          // �������.
	UINT packsize;      // �����С.

	UINT session;       // �Ự��־.
	CHAR user[20];		// �û���.
	CHAR pass[20];		// ����.
	UINT flag;			// ��־����½���ǳ�.
}* packLogonPtr, packLogon;

#define packLogonSize sizeof(packLogon)

typedef union _tagMsg
{
	struct _tagMsgHead 
	{
		UINT type;										// �������.
		UINT checksum;									// У���,Ϊ��������֮��. e.c type + rand + packsize.
		UINT rand;										// �����.
		UINT packsize;									// �����С.
	}MsgHead;

	packHeart heart;									// ������.
	packLogon logon;									// ��½��.
} packMsg, *packMsgPtr;

#define packHeadSize sizeof(MsgHead)

// ��������˶��뷽ʽ,�����.
#pragma pack(pop)

#endif // BASEPACKET_H__

