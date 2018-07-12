//
// Copyright (C) Xi 
// 
// File: pluginMain.cpp
//
// Author: Maya Plug-in Wizard 2.0
//

#include "stdafx.h"
#include <maya/MFnPlugin.h>
#include "FuseExporter.h"

MStatus initializePlugin( MObject obj )
//
//	Description:
//		this method is called when the plug-in is loaded into Maya.  It 
//		registers all of the services that this plug-in provides with 
//		Maya.
//
//	Arguments:
//		obj - a handle to the plug-in object (use MFnPlugin to access it)
//
{ 
	// Register the translator with the system
	MFnPlugin Plugin( obj, "Xi", "6.0", "Any");
	MStatus   Status =  Plugin.registerFileTranslator("Fuse", "", CFuseExporter::Creator, "", "option1=1", true);
	if (!Status) 
	{
		Status.perror( "registerFileTranslator" );
		return Status;
	}
	return Status;
}

MStatus uninitializePlugin( MObject obj )
//
//	Description:
//		this method is called when the plug-in is unloaded from Maya. It 
//		deregisters all of the services that it was providing.
//
//	Arguments:
//		obj - a handle to the plug-in object (use MFnPlugin to access it)
//
{
	MFnPlugin Plugin( obj );
	MStatus	Status =  Plugin.deregisterFileTranslator("X3D");
	if (!Status) 
	{
		Status.perror("deregisterFileTranslator");
		return Status;
	}
	return Status;
}
