import fbx
cubeVertices = [ fbx.FbxVector4( -5, -5,  5 ), # 0 - vertex index.
                 fbx.FbxVector4(  5, -5,  5 ), # 1
                 fbx.FbxVector4(  5,  5,  5 ), # 2
                 fbx.FbxVector4( -5,  5,  5 ), # 3
                 fbx.FbxVector4( -5, -5, -5 ), # 4
                 fbx.FbxVector4(  5, -5, -5 ), # 5
                 fbx.FbxVector4(  5,  5, -5 ), # 6
                 fbx.FbxVector4( -5,  5, -5 )] # 7

# The polygons (faces) of the cube.
polygonVertices = [ (  0,  1,  2,  3 ), # Face 1 - composed of the vertex index sequence: 0, 1, 2, and 3.
                    (  4,  5,  6,  7 ), # Face 2
                    (  8,  9, 10, 11 ), # Face 3
                    ( 12, 13, 14, 15 ), # Face 4
                    ( 16, 17, 18, 19 ), # Face 5
                    ( 20, 21, 22, 23 )] # Face 6

saveFilename = 'myFbxSaveFile.fbx'

###############################################################
# Helper Function(s).                                         #
###############################################################

def addCube( pScene ):
    ''' Adds a cubic mesh to the scene. '''

    # Obtain a reference to the scene's root node.
    rootNode = pScene.GetRootNode()

    # Create a new node in the scene.
    newNode = fbx.FbxNode.Create( pScene, 'myNode' )
    rootNode.AddChild( newNode )

    # Create a new mesh node attribute in the scene, and set it as the new node's attribute
    newMesh = fbx.FbxMesh.Create( pScene, 'myMesh' )
    newNode.SetNodeAttribute( newMesh )

    # Define the new mesh's control points. Since we are defining a cubic mesh,
    # there are 4 control points per face, and there are six faces, for a total
    # of 24 control points.
    global cubeVertices
    newMesh.InitControlPoints( 24 )
    # Face 1
    newMesh.SetControlPointAt( cubeVertices[0], 0 )
    newMesh.SetControlPointAt( cubeVertices[1], 1 )
    newMesh.SetControlPointAt( cubeVertices[2], 2 )
    newMesh.SetControlPointAt( cubeVertices[3], 3 )
    # Face 2
    newMesh.SetControlPointAt( cubeVertices[1], 4 )
    newMesh.SetControlPointAt( cubeVertices[5], 5 )
    newMesh.SetControlPointAt( cubeVertices[6], 6 )
    newMesh.SetControlPointAt( cubeVertices[2], 7 )
    # Face 3
    newMesh.SetControlPointAt( cubeVertices[5], 8 )
    newMesh.SetControlPointAt( cubeVertices[4], 9 )
    newMesh.SetControlPointAt( cubeVertices[7], 10 )
    newMesh.SetControlPointAt( cubeVertices[6], 11 )
    # Face 4
    newMesh.SetControlPointAt( cubeVertices[4], 12 )
    newMesh.SetControlPointAt( cubeVertices[0], 13 )
    newMesh.SetControlPointAt( cubeVertices[3], 14 )
    newMesh.SetControlPointAt( cubeVertices[7], 15 )
    # Face 5
    newMesh.SetControlPointAt( cubeVertices[3], 16 )
    newMesh.SetControlPointAt( cubeVertices[2], 17 )
    newMesh.SetControlPointAt( cubeVertices[6], 18 )
    newMesh.SetControlPointAt( cubeVertices[7], 19 )
    # Face 6
    newMesh.SetControlPointAt( cubeVertices[1], 20 )
    newMesh.SetControlPointAt( cubeVertices[0], 21 )
    newMesh.SetControlPointAt( cubeVertices[4], 22 )
    newMesh.SetControlPointAt( cubeVertices[5], 23 )

    # Now that the control points per polygon have been defined, we can create
    # the actual polygons within the mesh.
    global polygonVertices
    for i in range( 0, len( polygonVertices ) ):

        newMesh.BeginPolygon( i )

        for j in range( 0, len( polygonVertices[i] ) ):

            newMesh.AddPolygon( polygonVertices[i][j] )

        newMesh.EndPolygon()

def saveScene( pFilename, pFbxManager, pFbxScene, pAsASCII=False ):
    ''' Save the scene using the Python FBX API '''
    exporter = fbx.FbxExporter.Create( pFbxManager, '' )

    if pAsASCII:
        #DEBUG: Initialize the FbxExporter object to export in ASCII.
        asciiFormatIndex = getASCIIFormatIndex( pFbxManager )
        isInitialized = exporter.Initialize( pFilename, asciiFormatIndex )
    else:
        isInitialized = exporter.Initialize( pFilename )

    if( isInitialized == False ):
        raise Exception( 'Exporter failed to initialize. Error returned: ' + str( exporter.GetStatus().GetErrorString() ) )

    exporter.Export( pFbxScene )

    exporter.Destroy()


def getASCIIFormatIndex( pManager ):
    ''' Obtain the index of the ASCII export format. '''
    # Count the number of formats we can write to.
    numFormats = pManager.GetIOPluginRegistry().GetWriterFormatCount()

    # Set the default format to the native binary format.
    formatIndex = pManager.GetIOPluginRegistry().GetNativeWriterFormat()

    # Get the FBX format index whose corresponding description contains "ascii".
    for i in range( 0, numFormats ):

        # First check if the writer is an FBX writer.
        if pManager.GetIOPluginRegistry().WriterIsFBX( i ):

            # Obtain the description of the FBX writer.
            description = pManager.GetIOPluginRegistry().GetWriterFormatDescription( i )

            # Check if the description contains 'ascii'.
            if 'ascii' in description:
                formatIndex = i
                break

    # Return the file format.
    return formatIndex

###############################################################
# Main.                                                       #
###############################################################
if __name__ == '__main__':

    # Create the required FBX SDK data structures.
    fbxManager = fbx.FbxManager.Create()
    fbxScene = fbx.FbxScene.Create( fbxManager, '' )

    # Add a cube to the scene.
    addCube( fbxScene )

    # Save the scene.
    saveScene( saveFilename, fbxManager, fbxScene, pAsASCII=True )

    ### CLEANUP ###
    #
    # Destroy the fbx manager explicitly, which recursively destroys
    # all the objects that have been created with it.    fbxManager.Destroy()
    #
    # Once the memory has been freed, it is good practice to delete
    # the currently invalid references contained in our variables.
    del fbxManager, fbxScene
    #
    ###############
