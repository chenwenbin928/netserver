﻿/*
 *
 * Copyright (C) 2009 jack.wgm, microcai.
 * For conditions of distribution and use, see copyright notice 
 * in (http://code.google.com/p/netsever/source/browse/trunk/COPYING)
 *
 * Author: jack.wgm
 * Email:  jack.wgm@gmail.com
 */


#ifndef _HEAD_FILE_
#define _HEAD_FILE_

#if _MSC_VER
#pragma once
#endif

// 禁用警告信息.
#pragma warning(disable:4786)
#pragma warning(disable:4089)
#pragma warning(disable:4146)
#pragma warning(disable:4819)
#pragma warning(disable:4996)
#pragma warning(disable:4267)
#pragma warning(disable:4180)
#pragma warning(disable:4503)

// 头文件包含.
#include <string>

#include <stdexcept>
#include <algorithm>
#include <list>
#include <deque>
#include <set>
#include <iostream>
#include <vector>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/regex.hpp>
#include <boost/thread.hpp>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/tokenizer.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/thread/condition.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "config.h"

#endif //_HEAP_FILE_