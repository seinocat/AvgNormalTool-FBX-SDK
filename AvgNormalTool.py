import FBXClass

# replace it with your fbx file path
path = "E:/Client/GrapicsLab/Clients/URPWater/URPWater/Assets/Prefab/CubeTest.fbx"

def Start(path):
    fbxFile = FBXClass.FBX_Class(path)
    fbxFile.AddVertColor(fbxFile.scene.GetRootNode())
    fbxFile.AvgMeshNormal()
    fbxFile.save()
    return 0

Start(path)