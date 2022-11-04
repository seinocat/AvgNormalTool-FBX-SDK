/****************************************************************************************
 
   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.
 
   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

//! \file mayaextension.h
#ifndef _FBXSDK_MAYA_EXTENSION_H_
#define _FBXSDK_MAYA_EXTENSION_H_

#include <fbxsdk.h>

//Include Maya SDK headers
#include <maya/MGlobal.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnDagNode.h>
#include <maya/MItDag.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MCommonSystemUtils.h>

class FbxMayaNode;

//Define the FBX Maya Extension interface
#define FBXSDK_MAYA_EXTENSION_DECLARE()\
	FBXSDK_DLLEXPORT bool MayaExt_IsExtension(){return true;}\
	\
	FBXSDK_DLLEXPORT bool MayaExt_ExportHandled(MObject& pMayaObject);\
	FBXSDK_DLLEXPORT void MayaExt_ExportBegin(FbxScene* pFbxScene);\
    FBXSDK_DLLEXPORT bool MayaExt_ExportProcess(FbxObject** pOutputFbxObject, MObject& pInputObject, FbxScene* pFbxScene);\
	FBXSDK_DLLEXPORT void MayaExt_ExportTranslated(FbxObject* pFbxObject, MObject& pMayaObject);\
	FBXSDK_DLLEXPORT void MayaExt_ExportEnd(FbxScene* pFbxScene);\
	\
	FBXSDK_DLLEXPORT bool MayaExt_ImportHandled(FbxObject* pFbxObject);\
	FBXSDK_DLLEXPORT void MayaExt_ImportBegin(FbxScene* pFbxScene);\
    FBXSDK_DLLEXPORT bool MayaExt_ImportProcess(MObject& pOutputObject, FbxObject* pInputFbxObject, bool pIsAnInstance, bool pMerge);\
	FBXSDK_DLLEXPORT void MayaExt_ImportTranslated(FbxObject* pFbxObject, MObject& pMayaObject);\
	FBXSDK_DLLEXPORT void MayaExt_ImportEnd(FbxScene* pFbxScene);\

#endif /* _FBXSDK_MAYA_EXTENSION_H_ */
