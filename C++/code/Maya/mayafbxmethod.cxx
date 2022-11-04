#include "mayafbxmethod.h"
#include <string>
#include <map>

void AvgNormal(FbxScene* pScene)
{
	/*AddVertColor(pScene->GetRootNode());*/
	int geoCount = pScene->GetGeometryCount();
	for (int geoIndex = 0; geoIndex < geoCount; geoIndex++)
	{
		FbxGeometry* geo = pScene->GetGeometry(geoIndex);
		if (geo != nullptr && geo->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			int nodeCount = geo->GetNodeCount();
			for(int i = 0; i < nodeCount; i++)
			{
				FbxNode* node = geo->GetNode(i);
				FbxMesh* mesh = node->GetMesh();
				FbxLayer* layer = mesh->GetLayer(0);

				FbxLayerElementNormal* vertNormals = layer->GetNormals();
				FbxLayerElementTangent* vertTangents = layer->GetTangents();
				FbxLayerElementBinormal* vertBinormals = layer->GetBinormals();
				FbxLayerElementVertexColor* vertColor = layer->GetVertexColors();

				int polygonVertexCount = mesh->GetPolygonVertexCount();
				auto polygonVertices = mesh->GetPolygonVertices();
				/*auto vertNormalArray = vertNormals->GetDirectArray();
				auto vertTangentArray = vertTangents->GetDirectArray();
				auto vertBinormalArray = vertBinormals->GetDirectArray();
				auto vertColorArray = vertColor->GetDirectArray();
				auto vertColorIndexArray = vertColor->GetIndexArray();*/

				std::map<int, FbxArray<FbxVector4>> vertCtrlPoint;
				std::map<int, FbxVector4> vertAvgNormal;
				for(int j = 0; j < polygonVertexCount; j++)
				{
					int rCurVertexIndex = polygonVertices[j];
					auto rNormal = vertNormals->GetDirectArray()[j];

					vertCtrlPoint[rCurVertexIndex].AddUnique(rNormal);
				}

				for(auto it = vertCtrlPoint.begin(); it != vertCtrlPoint.end(); it)
				{
					FbxVector4 tempNormal = FbxVector4(0, 0, 0, 0);
					for(int k = 0; k < it->second.GetCount(); k++)
					{
						tempNormal += it->second[k];
						tempNormal.Normalize();
					}
					vertAvgNormal[it->first] = tempNormal;

					for(int m = 0; m < polygonVertexCount; m++)
					{
						int vertexIndex = polygonVertices[m];
						int vertColorIndex = vertColor->GetIndexArray()[m];
						FbxVector4 smoothNormal = vertAvgNormal[vertexIndex];
						auto normal = vertNormals->GetDirectArray()[m];
						auto tangent = vertTangents->GetDirectArray()[m];
						auto binormal = vertBinormals->GetDirectArray()[m];
						auto tempVector = FbxVector4(tangent.DotProduct(smoothNormal),
							binormal.DotProduct(smoothNormal), normal.DotProduct(smoothNormal), 0);

						smoothNormal = tempVector;
						smoothNormal.Normalize();

						FbxColor color = FbxColor();
						color.mRed = smoothNormal[0] * 0.5 + 0.5;
						color.mGreen = smoothNormal[1] * 0.5 + 0.5;
						color.mBlue = smoothNormal[2] * 0.5 + 0.5;
						color.mRed = vertColor->GetDirectArray()[vertColorIndex].mAlpha;

						vertColor->GetDirectArray().SetAt(vertColorIndex, color);
					}
				}
			}
		}
	}
}


void AddVertColor(FbxNode* node)
{
	if (node->GetChildCount() > 0)
	{
		for(int i = 0; i < node->GetChildCount(); i ++)
		{
			FbxMesh* mesh = node->GetChild(i)->GetMesh();
			if (mesh != nullptr)
			{
				FbxLayer* layer = mesh->GetLayer(0);

				auto getVertexColor = layer->GetVertexColors();
				if (getVertexColor != nullptr)
					continue;

				auto vertexColor = FbxLayerElementVertexColor::Create(mesh, "vertex color");
				vertexColor->SetMappingMode(FbxLayerElement::eByPolygonVertex);
				vertexColor->SetReferenceMode(FbxLayerElement::eIndexToDirect);

				for (int j = 0; j < mesh->GetPolygonVertexCount(); j++)
				{
					vertexColor->GetDirectArray().Add(FbxColor(1, 1, 1, 1));
					vertexColor->GetIndexArray().Add(j);
				}
				layer->SetVertexColors(vertexColor);
			}
				
			/*AddVertColor(node->GetChild(i));*/
		}
	}
}
