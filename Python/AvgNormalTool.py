from fbx import FbxNode

import FBXClass
import time

# replace it with your fbx file path
# path = "E:/Client/GrapicsLab/Clients/URPWater/URPWater/Assets/Prefab/Cube.fbx"
path = "E:/MayaCube.fbx"

def Start(path):
    fbxFile = FBXClass.FBX_Class(path)
    print("Start AvgNormal....")
    time_start = time.time()
    fbxFile.add_vertex_color(fbxFile.scene.GetRootNode())
    fbxFile.avg_mesh_normal()
    fbxFile.save()
    time_end = time.time()
    print("Done，Elapsed time:", time_end - time_start, "s")
    # fbxFile.scene.SetName("")
    # rootNode:FbxNode =
    #print(fbxFile.scene.GetName())

    #print(fbxFile.scene.GetNode(0).GetNode(0).GetName())

Start(path)