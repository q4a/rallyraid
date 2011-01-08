// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

//#pragma once
#ifndef STDAFX_H
#define STDAFX_H

#include "targetver.h"

#include <stdio.h>
#include <assert.h>
//#include <tchar.h>
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreMeshManager.h>
#include <OgreResourceManager.h>
#include <OgreConfigFile.h>
#include <OgreLight.h>
#include <OgreMath.h>

#define MY_DEBUG_SERROR  0
#define MY_DEBUG_ERROR   1
#define MY_DEBUG_WARNING 2
#define MY_DEBUG_IMPINFO 3
#define MY_DEBUG_INFO    4
#define MY_DEBUG_NOTE    5

#ifdef MY_DEBUG
# ifndef MY_DEBUG_LEVEL
#  define MY_DEBUG_LEVEL MY_DEBUG_NOTE
# endif
# define dprintf(lev, x, ...) if (lev <= MY_DEBUG_LEVEL) printf((x), __VA_ARGS__)
#else
# define dprintf(lev, x, ...)
#endif

// TODO: reference additional headers your program requires here


#endif // STDAXF_H
