import fbx
import FbxCommon
import time
from fbx import *

class FBX_Class(object):

    def __init__(self, filename):
        """
        FBX Scene Object
        """
        self.filename = filename
        self.scene = None
        self.sdk_manager = None
        self.sdk_manager, self.scene = FbxCommon.InitializeSdkObjects()
        FbxCommon.LoadScene(self.sdk_manager, self.scene, filename)

        self.root_node = self.scene.GetRootNode()
        self.scene_nodes = self.get_scene_nodes()

    def close(self):
        """
        You need to run this to close the FBX scene safely
        """
        # destroy objects created by the sdk
        self.sdk_manager.Destroy()

    def __get_scene_nodes_recursive(self, node):
        """
        Rescursive method to get all scene nodes
        this should be private, called by get_scene_nodes()
        """
        self.scene_nodes.append(node)
        for i in range(node.GetChildCount()):
            self.__get_scene_nodes_recursive(node.GetChild(i))

    def get_scene_nodes(self):
        """
        Get all nodes in the fbx scene
        """
        self.scene_nodes = []
        for i in range(self.root_node.GetChildCount()):
            self.__get_scene_nodes_recursive(self.root_node.GetChild(i))
        return self.scene_nodes

    def get_type_nodes(self, type):
        """
        Get nodes from the scene with the given type
        display_layer_nodes = fbx_file.get_type_nodes( u'DisplayLayer' )
        """
        nodes = []
        num_objects = self.scene.RootProperty.GetSrcObjectCount()
        for i in range(0, num_objects):
            node = self.scene.RootProperty.GetSrcObject(i)
            if node:
                if node.GetTypeName() == type:
                    nodes.append(node)
        return nodes

    def get_class_nodes(self, class_id):
        """
        Get nodes in the scene with the given classid
        geometry_nodes = fbx_file.get_class_nodes( fbx.FbxGeometry.ClassId )
        """
        nodes = []
        num_nodes = self.scene.RootProperty.GetSrcObjectCount(fbx.FbxCriteria.ObjectType(class_id))
        for index in range(0, num_nodes):
            node = self.scene.RootProperty.GetSrcObject(fbx.FbxCriteria.ObjectType(class_id), index)
            if node:
                nodes.append(node)
        return nodes

    def get_property(self, node, property_string):
        """
        Gets a property from an Fbx node
        export_property = fbx_file.get_property(node, 'no_export')
        """
        fbx_property = node.FindProperty(property_string)
        return fbx_property

    def get_property_value(self, node, property_string):
        """
        Gets the property value from an Fbx node
        property_value = fbx_file.get_property_value(node, 'no_export')
        """
        fbx_property = node.FindProperty(property_string)
        if fbx_property.IsValid():
            # cast to correct property type so you can get
            casted_property = self.__cast_property_type(fbx_property)
            if casted_property:
                return casted_property.Get()
        return None

    def get_node_by_name(self, name):
        """
        Get the fbx node by name
        """
        self.get_scene_nodes()
        # right now this is only getting the first one found
        node = [node for node in self.scene_nodes if node.GetName() == name]
        if node:
            return node[0]
        return None

    def remove_namespace(self):
        """
        Remove all namespaces from all nodes
        This is not an ideal method but
        """
        self.get_scene_nodes()
        for node in self.scene_nodes:
            orig_name = node.GetName()
            split_by_colon = orig_name.split(':')
            if len(split_by_colon) > 1:
                new_name = split_by_colon[-1:][0]
                node.SetName(new_name)
        return True

    def remove_node_property(self, node, property_string):
        """
        Remove a property from an Fbx node
        remove_property = fbx_file.remove_property(node, 'UDP3DSMAX')
        """
        node_property = self.get_property(node, property_string)
        if node_property.IsValid():
            node_property.DestroyRecursively()
            return True
        return False

    def remove_nodes_by_names(self, names):
        """
        Remove nodes from the fbx file from a list of names
        names = ['object1','shape2','joint3']
        remove_nodes = fbx_file.remove_nodes_by_names(names)
        """

        if names == None or len(names) == 0:
            return True

        self.get_scene_nodes()
        remove_nodes = [node for node in self.scene_nodes if node.GetName() in names]
        for node in remove_nodes:
            disconnect_node = self.scene.DisconnectSrcObject(node)
            remove_node = self.scene.RemoveNode(node)
        self.get_scene_nodes()
        return True

    def save(self, filename=None):
        """
        Save the current fbx scene as the incoming filename .fbx
        """
        # save as a different filename
        if not filename is None:
            FbxCommon.SaveScene(self.sdk_manager, self.scene, filename)
        else:
            FbxCommon.SaveScene(self.sdk_manager, self.scene, self.filename)
        self.close()

    def AvgMeshNormal(self):
        print("Start AvgNormal....")
        time_start = time.time()

        geoCount = self.scene.GetGeometryCount()
        for geoIndex in range(geoCount):
            geo: FbxGeometry = self.scene.GetGeometry(geoIndex)
            if geo is not None and geo.GetAttributeType() == fbx.FbxNodeAttribute.eMesh:
                nodeCount = geo.GetNodeCount()
                for i in range(nodeCount):
                    # 获取节点
                    node = geo.GetNode(i)
                    # 获取mesh
                    mesh = node.GetMesh()
                    # 获取Layer0
                    layer = mesh.GetLayer(0)

                    # 获取法线，切线，副切线，顶点色
                    vertNormals = layer.GetNormals()
                    vertTangents = layer.GetTangents()
                    vertBinormals = layer.GetBinormals()
                    vertColor = layer.GetVertexColors()

                    if vertTangents is None:
                        print("Error, Tangents is None")
                    if vertBinormals is None:
                        print("Error, Binormals is None")
                    if vertColor is None:
                        print("Error, VertColor is None")

                    # 优化点，缓存数据，减少调用GetXXX()产生的开销
                    polygonVetexCount = mesh.GetPolygonVertexCount()
                    polygonVertices = mesh.GetPolygonVertices()
                    vertNormalArray = vertNormals.GetDirectArray()
                    vertTangentArray = vertTangents.GetDirectArray()
                    vertBinormalArray = vertBinormals.GetDirectArray()
                    vertColorArray = vertColor.GetDirectArray()
                    vertColorIndexArray = vertColor.GetIndexArray()

                    # 控制顶点字典        <int, List<FbxVector4>>  <顶点索引，法线列表>
                    vertCtrlPoint = {}
                    # 控制顶点平均法线字典  <int, FbxVector4>        <顶点索引, 平均法线>
                    vertAvgNormal = {}

                    # 先取到所有控制点的法线
                    for j in range(polygonVetexCount):
                        # print("正在读取", j, "/", polygonVetexCount)
                        rCurVertexIndex = polygonVertices[j]
                        rNormal: FbxVector4 = vertNormalArray[j]
                        if rCurVertexIndex not in vertCtrlPoint:
                            normals = [rNormal]
                            vertCtrlPoint[rCurVertexIndex] = normals
                        else:
                            if rNormal not in vertCtrlPoint[rCurVertexIndex]:
                                vertCtrlPoint[rCurVertexIndex].append(rNormal)
                    # 平均法线值
                    for key, value in vertCtrlPoint.items():
                        weightNormal = FbxVector4(0, 0, 0, 0)
                        for k in range(len(value)):
                            weightNormal += value[k]
                        weightNormal.Normalize()
                        vertAvgNormal[key] = weightNormal

                    for m in range(polygonVetexCount):
                        print("正在写入", m, "/", polygonVetexCount)
                        # 获取对应的顶点索引，顶点色索引，平均法线
                        vertexIndex = polygonVertices[m]
                        vertColorIndex = vertColorIndexArray[m]
                        smoothNormal: FbxVector4 = vertAvgNormal[vertexIndex]

                        # 构建TBN，转换到切线空间下
                        normal = vertNormalArray[m]
                        tangent = vertTangentArray[m]
                        binormal = vertBinormalArray[m]
                        tempVector = FbxVector4(tangent.DotProduct(smoothNormal),
                                                binormal.DotProduct(smoothNormal),
                                                normal.DotProduct(smoothNormal),
                                                0)

                        smoothNormal = tempVector
                        smoothNormal.Normalize()

                        # 映射到顶点色中
                        color = FbxColor()
                        color.mRed = smoothNormal[0] * 0.5 + 0.5
                        color.mGreen = smoothNormal[1] * 0.5 + 0.5
                        color.mBlue = smoothNormal[2] * 0.5 + 0.5
                        color.mAlpha = vertColorArray[vertColorIndex].mAlpha

                        # 写入顶点色
                        vertColorArray.SetAt(vertColorIndex, color)

        time_end = time.time()
        print("Done，Elapsed time:", time_end - time_start, "s")


    def AddVertColor(self, node:FbxNode):
        print("开始添加顶点色通道")
        if node.GetChildCount() > 0:
            for i in range(node.GetChildCount()):
                mesh = node.GetChild(i).GetMesh()
                if mesh is not None :
                    layer = mesh.GetLayer(0)
                    vertexColor = FbxLayerElementVertexColor.Create(mesh, "")
                    vertexColor.SetMappingMode(FbxLayerElement.eByPolygonVertex)
                    vertexColor.SetReferenceMode(FbxLayerElement.eIndexToDirect)
                    vertexColorArray = vertexColor.GetDirectArray()
                    vertexIndexArray = vertexColor.GetIndexArray()
                    for j in range(mesh.GetPolygonVertexCount()):
                        vertexColorArray.Add(FbxColor(1, 1, 1, 1))
                        vertexIndexArray.Add(j)
                    layer.SetVertexColors(vertexColor)
                self.AddVertColor(node.GetChild(i))