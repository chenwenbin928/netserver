/*
 *
 * Copyright (C) 2009 jack.wgm, microcai.
 * For conditions of distribution and use, see copyright notice 
 * in (http://code.google.com/p/netsever/source/browse/trunk/COPYING)
 *
 * Author: jack.wgm
 * Email:  jack.wgm@gmail.com
 */

#ifndef _HEAP_FILE_DEFS_
#define _HEAP_FILE_DEFS_

#if _MSC_VER
#pragma once
#endif

// ��������ݰ���ʶ.
#define MSG_PACK_HEART                         0x0000              // ������. c -> s.

#define MSG_PACK_LOGON                         0x0001              // �û���½��. c -> s.
#define MSG_PACK_FBLOGON                       0x0002              // �û���½��. s -> c.

#define MSG_PACK_LOGOUT                        0x0003              // �û��ǳ���. c -> s.

#endif //_HEAP_FILE_DEFS_