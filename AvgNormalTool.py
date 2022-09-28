import FBXClass

# replace it with your fbx path
fbxFile = FBXClass.FBX_Class(r"xxxx\Model\nmxs.fbx")
# add vertex color
#fbxFile.AddVertColor(fbxFile.scene.GetRootNode())
fbxFile.AvgMeshNormal()
fbxFile.save()


