import FBXClass

fbxFile = FBXClass.FBX_Class(r"E:\Client\GrapicsLab\Clients\URPWater\URPWater\Assets\Prefab\nmxss.fbx")
# 添加顶点色通道
#fbxFile.AddVertColor(fbxFile.scene.GetRootNode())
fbxFile.AvgMeshNormal()
fbxFile.save()


