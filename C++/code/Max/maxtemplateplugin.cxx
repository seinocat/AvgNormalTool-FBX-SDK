/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include "maxextension.h"
#include "maxfbxmethod.h"

class MaxExtensionTemplatePlugin : public FbxPlugin
{
	FBXSDK_PLUGIN_DECLARE(MaxExtensionTemplatePlugin);

protected:
	explicit MaxExtensionTemplatePlugin(const FbxPluginDef& pDefinition, FbxModule pFbxModule) : FbxPlugin(pDefinition, pFbxModule)
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

FBXSDK_PLUGIN_IMPLEMENT(MaxExtensionTemplatePlugin);

// FBX Interface
extern "C"
{
    //The DLL is owner of the plug-in
    static MaxExtensionTemplatePlugin* sPlugin = NULL;

    //This function will be called when an application will request the plug-in
    FBXSDK_DLLEXPORT void FBXPluginRegistration(FbxPluginContainer& pContainer, FbxModule pFbxModule)
    {
        if( sPlugin == NULL )
        {
            //Create the plug-in definition which contains the information about the plug-in
            FbxPluginDef sPluginDef;
            sPluginDef.mName = "MaxExtensionTemplatePlugin";
            sPluginDef.mVersion = "1.0";

            //Create an instance of the plug-in.  The DLL has the ownership of the plug-in
            sPlugin = MaxExtensionTemplatePlugin::Create(sPluginDef, pFbxModule);

            //Register the plug-in
            pContainer.Register(*sPlugin);
        }
    }

	FBXSDK_MAX_EXTENSION_DECLARE();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Max Extension Export Functions
bool MaxExt_ExportHandled(INode* pMaxObject)
{
	//Called for each Max object that *is going to* be translated into fbx common data.
	//Sometimes, for different reasons, this might be called more than once for the same object.
	//
	//The purpose of this function is to tell the plugin if we are going to handle the creation
	//of this Max object in this extension plugin or if we let the plugin work as usual.
	//
	//So returning TRUE means the corresponding fbx object for this particuliar Max object won't
	//be created, and we'll have to create it ourselves, so we say "we handle it". Otherwise,
	//returning FALSE will simply result in the usual behavior, which means the plugin will
	//translate the Max object into the corresponding fbx object, if it knows how-to, of course.
	return false;
}

void MaxExt_ExportBegin(FbxScene* pFbxScene, INode* pMaxRootNode)
{
	//Called before we export anything into the fbx scene.
	//
	//We could choose to create our custom data now or wait at the end of the export
	//depending if that custom data replaces objects in the fbx scene hierarchy.
}

bool MaxExt_ExportProcess(FbxObject** pOutputFbxObject, ReferenceTarget* pInputObject, FbxScene* pFbxScene)
{
    //Called during the export process of the following objects:
    //
    //Right after the FbxNode is created but before the FbxNodeAttribute.
    //The purpose of this function is to intercept the default creation of the FBX object based on the 3dsMax 
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
    //  1) This callback can also be called with pOutputFbxObject = NULL and pFbxScene = NULL. In this
    //     case, it should simply return true or false depending if it can handle the input object (no
    //     other action is required)
    //  2) In case 2 above, this callback is responsible for maintaining the integrity of the
    //     received FbxObject and all its existing connections.
    //
    return false;
}

void MaxExt_ExportTranslated(FbxObject* pFbxObject, INode* pMaxObject)
{
	//Called after the Max scene has been translated into an fbx scene, for each
	//object that was convertable. In other words, objects unknown or unsupported
	//by the FbxSdk won't be called in this function.
	//
	//The purpose of this function is to tell the extension plugin what is the corresponding
	//fbx object for that particuliar Max object. This is especially useful when connections
	//need to be preserved between plugin and extension plugin fbx objects.
}

void MaxExt_ExportEnd(FbxScene* pFbxScene, INode* pMaxRootNode)
{
	//Called after the scene has been fully translated into fbx. This is the last function called
	//before the extension plugin is done with the export process. Any clean-up or last minute
	//modification to the scene must be done now.

    AvgNormal(pFbxScene);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Max Extension Import Functions
bool MaxExt_ImportHandled(FbxObject* pFbxObject)
{
	//This is called by the importer everytime its about to translate the FBX object
	//into a Max object. If the extension plugin should handle the creation, then
	//it needs to answer TRUE to this call.
	return false;
}

void MaxExt_ImportBegin(FbxScene* pFbxScene, INode* pMaxRootNode)
{
	//This is called the the beginning of the import process,
	//allowing us to act on the FBX scene before its loaded from the file.
}

bool MaxExt_ImportProcess(ReferenceTarget** pOutputObject, FbxObject* pInputFbxObject, bool pIsAnInstance, bool pMerge)
{
    //  This is called by the importer everytime it is about to create one of the following 3dsMax object:
    //
    //
    //  The purpose of this function is to intercept the default creation of the 3dsMax object (from the FBX object)
    //  and replace it with this extension plugin version. If this function returns TRUE it will mean that
    //  all the required manipulation has been perfomed here and no further processing is required by the importer. 
    //  Inversely, if we return FALSE, we let the importer process the data as usual.
    //
    //  Note that if pOutputObject = NULL and pIsAnInstance = false and pMerge = false, this callback should simply
    //  test to see if it can handle the conversionof pInputFbxObject and return true or false accordingly. No other
    //  action is required.
    return false;
}

void MaxExt_ImportTranslated(FbxObject* pFbxObject, INode* pMaxObject)
{
	//This is called everytime an FBX object got converted into a Max object
	//while we are traversing the scene during the translation process.
}

void MaxExt_ImportEnd(FbxScene* pFbxScene, INode* pMaxRootNode)
{
	//This is called at the very end of the import.
}


