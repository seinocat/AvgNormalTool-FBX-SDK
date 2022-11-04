#include "maxfbxmethod.h"

void Fbx_SetName(FbxScene* pScene)
{
	pScene->SetName("New Plugin Project !!!");
}

void Fbx_SetName(FbxScene* pScene, INode* pMaxNode)
{
	pScene->SetName("3ds Max Plugin Working..........");
}

