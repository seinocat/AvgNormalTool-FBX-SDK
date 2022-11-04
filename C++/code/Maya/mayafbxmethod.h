#ifndef MAYA_FBXMETHOD_H
#define MAYA_FBXMETHOD_H

#include <fbxsdk.h>

void Fbx_SetName(FbxScene* pScene);
void Fbx_AnimCurve(FbxScene* pScene);
FbxAnimCurve* GetCurve(FbxNode* node, FbxAnimLayer* animLayer, int type,const char* channel);
void SetCurve(FbxAnimCurve* curve);

void AvgNormal(FbxScene* pScene);
void AddVertColor(FbxNode* node);

#endif
