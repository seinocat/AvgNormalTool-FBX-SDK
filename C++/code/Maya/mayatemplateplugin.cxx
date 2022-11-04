/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include "mayaextension.h"
#include "mayafbxmethod.h"

class MayaExtensionTemplatePlugin : public FbxPlugin
{
	FBXSDK_PLUGIN_DECLARE(MayaExtensionTemplatePlugin);

protected:
	explicit MayaExtensionTemplatePlugin(const FbxPluginDef& pDefinition, FbxModule pFbxModule) : FbxPlugin(pDefinition, pFbxModule)
	{
	}

	// Implement kfbxmodules::FbxPlugin
	bool SpecificInitialize() override
	{
		return true;
	}

	bool SpecificTerminate() override
	{
		return true;
	}
};

FBXSDK_PLUGIN_IMPLEMENT(MayaExtensionTemplatePlugin);

// FBX Interface
extern "C"
{
    //The DLL is owner of the plug-in
    static MayaExtensionTemplatePlugin* sPlugin = NULL;

    //This function will be called when an application will request the plug-in
    FBXSDK_DLLEXPORT void FBXPluginRegistration(FbxPluginContainer& pContainer, FbxModule pFbxModule)
    {
        if( sPlugin == NULL )
        {
            //Create the plug-in definition which contains the information about the plug-in
            FbxPluginDef sPluginDef;
            sPluginDef.mName = "MayaExtensionTemplatePlugin";
            sPluginDef.mVersion = "1.0";

            //Create an instance of the plug-in.  The DLL has the ownership of the plug-in
            sPlugin = MayaExtensionTemplatePlugin::Create(sPluginDef, pFbxModule);

            //Register the plug-in
            pContainer.Register(*sPlugin);
        }
    }

	FBXSDK_MAYA_EXTENSION_DECLARE();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Maya Extension Export Functions
bool MayaExt_ExportHandled(MObject& pMayaObject)
{
	//Called for each Maya object that *is going to* be translated into fbx common data.
	//Sometimes, for different reasons, this might be called more than once for the same object.
	//
	//The purpose of this function is to tell the plugin if we are going to handle the creation
	//of this Maya object in this extension plugin or if we let the plugin work as usual.
	//
	//So returning TRUE means the corresponding fbx object for this particuliar Maya object won't
	//be created, and we'll have to create it ourselves, so we say "we handle it". Otherwise,
	//returning FALSE will simply result in the usual behavior, which means the plugin will
	//translate the Maya object into the corresponding fbx object, if it knows how-to, of course.
	return false;
}

void MayaExt_ExportBegin(FbxScene* pFbxScene)
{
	//Called before we export anything into the fbx scene.
	//
	//We could choose to create our custom data now or wait at the end of the export
	//depending if that custom data replaces objects in the fbx scene hierarchy.
}

bool MayaExt_ExportProcess(FbxObject** pOutputFbxObject, MObject& pInputObject, FbxScene* pFbxScene)
{
    //Called during the export process of the following objects:
    //
    //      Transform
    //      LodGroup
    //      Effector
    //      Locator
    //      Joint
    //      NurbsCurve
    //      Cluster
    //      Light
    //      Camera
    //      StereoCamera
    //      LookAt
    //      Material
    //      File Texture
    //
    //Right after the FbxNode is created but before the FbxNodeAttribute.
    //The purpose of this function is to intercept the default creation of the FBX object based on the Maya 
    //object and replace it with this extension plugin version. If this function returns TRUE it will mean that
    //all the required manipulation has been perfomed here and no further processing is required by the exporter. 
    //Inversely, if we return FALSE, we let the exporter process the data as usual.
    //
    //The proper implementation of this function requires to check what is the inputObject type
    //and based on his type, we either need to :
    //
    //   - create the corresponding FbxObject type (this is typically a custom Fbx class) when the content 
    //     of pOutputFbxObject is NULL
    //
    //or
    //
    //   - create and add the appropriate FBX data to the existing pOutputFbxObject.
    //
    //Notes:
    //  1) This callback is never called if the MayaExt_ExportHandled callback returned TRUE
    //  2) In case 2 above, this callback is responsible for maintaining the integrity of the
    //     received FbxObject and all its existing connections.
    //
    return false;
}

void MayaExt_ExportTranslated(FbxObject* pFbxObject, MObject& pMayaObject)
{
	//Called after the Maya scene has been translated into an fbx scene, for each
	//object that was convertable. In other words, objects unknown or unsupported
	//by the FbxSdk won't be called in this function.
	//
	//The purpose of this function is to tell the extension plugin what is the corresponding
	//fbx object for that particular Maya object. This is especially useful when connections
	//need to be preserved between plugin and extension plugin fbx objects.
}



void MayaExt_ExportEnd(FbxScene* pFbxScene)
{
	//Called after the scene has been fully translated into fbx. This is the last function called
	//before the extension plugin is done with the export process. Any clean-up or last minute
	//modification to the scene must be done now.

    /*if (MCommonSystemUtils::getEnv("OpenDll") == "true") {
        pFbxScene->SetName("Maya Env Works!!!!");
    }
    else
    {
        pFbxScene->SetName("Maya Env not Works!!!!");
    }*/


    AvgNormal(pFbxScene);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Maya Extension Import Functions
bool MayaExt_ImportHandled(FbxObject* pFbxObject)
{
	//This is called by the importer everytime it is about to translate the FBX object
	//into a Maya object. If the extension plugin should handle the creation, then
	//it needs to answer TRUE to this call.
	return false;
}

void MayaExt_ImportBegin(FbxScene* pFbxScene)
{
	//This is called the the beginning of the import process,
	//allowing us to act on the FBX scene before its loaded from the file.
}

bool MayaExt_ImportProcess(MObject& pOutputObject, FbxObject* pInputFbxObject, bool pIsAnInstance, bool pMerge)
{
    //This is called by the importer everytime it is about to create one of the following Maya object:
	//
    //  Transform
    //  LodGroup
    //  Locator
    //  Joint
    //  NurbsCurve
    //  Light
    //  Camera
    //  StereoCamera
    //  Material
    //
    //The purpose of this function is to intercept the default creation of the Maya object (from the FBX object)
    //and replace it with this extension plugin version. If this function returns TRUE it will mean that
    //all the required manipulation has been perfomed here and no further processing is required by the importer. 
    //Inversely, if we return FALSE, we let the importer process the data as usual.
    //
    //This function need to make sure that calls to MDagPath::getAPathTo() on the returned object 
    //(pOutputObject) will succeed and return a valid path to it so that the FBX Maya plug-in can reference it
    //correctly.
    return false;
}

void MayaExt_ImportTranslated(FbxObject* pFbxObject, MObject& pMayaObject)
{
	//This is called everytime an FBX object got converted into a Maya object
	//while we are traversing the scene during the translation process.
}

void MayaExt_ImportEnd(FbxScene* pFbxScene)
{
	//This is called at the very end of the import.
}


