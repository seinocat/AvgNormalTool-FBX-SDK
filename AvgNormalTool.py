import FBXClass

normals = []

fbxFile = FBXClass.FBX_Class(r"E:\Client\GrapicsLab\Clients\URPWater\URPWater\Assets\Prefab\nmxss.fbx")

fbxFile.AddVertColor(fbxFile.scene.GetRootNode())
fbxFile.AvgMeshNormal()
fbxFile.save()


