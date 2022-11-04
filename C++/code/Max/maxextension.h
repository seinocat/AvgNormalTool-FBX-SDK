/****************************************************************************************
 
   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.
 
   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

//! \file maxextension.h
#ifndef _FBXSDK_MAX_EXTENSION_H_
#define _FBXSDK_MAX_EXTENSION_H_

#include <fbxsdk.h>

//Include 3dsMax SDK headers
#include <max.h>

//Define the FBX 3dsMax Extension interface
#define FBXSDK_MAX_EXTENSION_DECLARE()\
	FBXSDK_DLLEXPORT bool MaxExt_IsExtension(){return true;}\
	\
	FBXSDK_DLLEXPORT bool MaxExt_ExportHandled(INode* pMaxObject);\
	FBXSDK_DLLEXPORT void MaxExt_ExportBegin(FbxScene* pFbxScene, INode* pMaxRootNode);\
    FBXSDK_DLLEXPORT bool MaxExt_ExportProcess(FbxObject** pOutputFbxObject, ReferenceTarget* pMaxObject, FbxScene* pFbxScene);\
	FBXSDK_DLLEXPORT void MaxExt_ExportTranslated(FbxObject* pFbxObject, INode* pMaxObject);\
	FBXSDK_DLLEXPORT void MaxExt_ExportEnd(FbxScene* pFbxScene, INode* pMaxRootNode);\
	\
	FBXSDK_DLLEXPORT bool MaxExt_ImportHandled(FbxObject* pFbxObject);\
	FBXSDK_DLLEXPORT void MaxExt_ImportBegin(FbxScene* pFbxScene, INode* pMaxRootNode);\
    FBXSDK_DLLEXPORT bool MaxExt_ImportProcess(ReferenceTarget** pOutputObject, FbxObject* pInputFbxObject, bool pIsAnInstance, bool pMerge);\
	FBXSDK_DLLEXPORT void MaxExt_ImportTranslated(FbxObject* pFbxObject, INode* pMaxObject);\
	FBXSDK_DLLEXPORT void MaxExt_ImportEnd(FbxScene* pFbxScene, INode* pMaxRootNode);\

#endif /* _FBXSDK_MAX_EXTENSION_H_ */
