// FuseExporter.cpp

#include "stdafx.h"
#include "FuseExporter.h"
#include "Resource.h"
#include "Node.h"

CFuseExporter::CFuseExporter() :
	m_pRootNode( NULL )
{
}

CFuseExporter::~CFuseExporter()
{
}

void* CFuseExporter::Creator()
{
	return new CFuseExporter();
}

bool CFuseExporter::canBeOpened () const
{
	return true;
}

CVector MayaToFuse( MPoint Vector )
{
	return CVector( (float)Vector.x, (float)Vector.y, (float)Vector.z );
}

void CFuseExporter::ConvertTransform(	MDagPath DagPath, CVector& vPosition, CMatrix3& mRotation, CMatrix4& mGeomTransform )
{
	MStatus Status;
	MFnTransform Transform( DagPath );
	CHECK_ERROR( Status );
	CMatrix4 mFuseMatrix;
	MMatrix MayaMatrix = DagPath.inclusiveMatrix();
	MayaMatrix.get( mFuseMatrix.m_fEntry );

	// In Maya the rotation pivot and the local coordinate space origin don't necessarily correspond

    // So, get the rotation pivot in world space & use this pivot point as the transform on our world matrix
	mFuseMatrix.SetTranslation( MayaToFuse( Transform.rotatePivot( MSpace::kWorld ) ) );

	// Get the location of the rotate, scale & scale translation pivot in local space. 
	// We use this later to transform the geometry on this node.
	CVector vScalePivot				= MayaToFuse( Transform.scalePivot( MSpace::kTransform ) );
	CVector vScalePivotTranslation	= MayaToFuse( Transform.scalePivotTranslation(MSpace::kTransform ) );
	CVector vRotatePivot			= MayaToFuse( Transform.rotatePivot(MSpace::kTransform ) );
	CVector vObjectScale			= mFuseMatrix.GetScale();
	mGeomTransform			= 
		CMatrix4( CMatrix4::eMATRIX_TRANSLATION,	-vScalePivot.fX, -vScalePivot.fY, -vScalePivot.fZ ) *
		CMatrix4( CMatrix4::eMATRIX_DIAGONAL,		vObjectScale.fX, vObjectScale.fY, vObjectScale.fZ ) *
		CMatrix4( CMatrix4::eMATRIX_TRANSLATION,	vScalePivot.fX, vScalePivot.fY, vScalePivot.fZ ) *
		CMatrix4( CMatrix4::eMATRIX_TRANSLATION,	vScalePivotTranslation.fX, vScalePivotTranslation.fY, vScalePivotTranslation.fZ ) *
		CMatrix4( CMatrix4::eMATRIX_TRANSLATION,	-vRotatePivot.fX, -vRotatePivot.fY, -vRotatePivot.fZ );

	MMatrix ShapeLTM = MMatrix( mFuseMatrix.m_fEntry );

	// If this node has a parent then convert our world matrix into a local matrix.
	if ( DagPath.length() > 1 )
	{
		MDagPath ParentPath = DagPath;
		ParentPath.pop();
		CMatrix4 mParentFuseMatrix;
        MFnTransform ParentTransFn( ParentPath );
		MMatrix ParentMatrix = ParentPath.inclusiveMatrix();
		ParentMatrix.get( mParentFuseMatrix.m_fEntry );

		// Get the parent rotation pivot in world space
		// Use the parent pivot point as the transform on our matrix
        mParentFuseMatrix.SetTranslation( MayaToFuse( ParentTransFn.rotatePivot( MSpace::kWorld ) ) );

 		// Calculate local matrix
		MMatrix ShapeMatrix = ShapeLTM * ParentMatrix.inverse();
		ShapeMatrix.get( mParentFuseMatrix.m_fEntry );

	    // Extract rotation and remove the scaling it's set in vObjectScale
		mRotation = mParentFuseMatrix.GetRotation();
		vPosition = mParentFuseMatrix.GetTranslation().ComponentProduct( mParentFuseMatrix.GetScale() );
	}
	else
	{
		mRotation.MakeIdentity();
		vPosition = mFuseMatrix.GetTranslation();
	}

}

MString	CFuseExporter::defaultExtension () const
{
	return "fuse";
}

MString CFuseExporter::filter () const
{
	return "*.fuse";
}

bool CFuseExporter::haveWriteMethod () const
{
	return true;
}

bool CFuseExporter::haveReadMethod () const
{
	return false;
}

bool CFuseExporter::IsVisible( MFnDagNode& fnDag ) 
{
	if( fnDag.isIntermediateObject() )
		return false;

	MStatus Status;
	MPlug VisPlug = fnDag.findPlug("visibility", &Status );
	CHECK_ERROR( Status );
	bool bVisible;
	CHECK_ERROR( VisPlug.getValue( bVisible ) );
	return bVisible;
}

CNode* CFuseExporter::WalkGraph( CNode* pParent, MDagPath DagPath )
{
	// We only process transforms and joints - Shape children become part of the node.
	MStatus Status;
	MFn::Type Type =  DagPath.apiType( &Status );
	if (	DagPath.apiType( &Status ) != MFn::kWorld && DagPath.apiType( &Status ) != MFn::kTransform && 
			DagPath.apiType( &Status ) != MFn::kJoint )
        return NULL;
	CHECK_ERROR( Status );

	MFnDagNode DagNode( DagPath, &Status );
	if ( !IsVisible( DagNode ) )
		return NULL;

	CHECK_ERROR( Status );
	CNode* pNode = new CNode();
	pNode->SetName( DagNode.name( &Status ).asChar() );
	if ( pParent )
		pParent->AttachChild( pNode );

	CVector vPosition;
	CMatrix3 mRotation;
	CMatrix4 mGeomTransform;
	ConvertTransform( DagPath, vPosition, mRotation, mGeomTransform );
	pNode->SetPosition( vPosition );
	pNode->SetRotation( mRotation );

	MFnTransform Transform( DagPath );
	//if ( IsMayaMesh( Transform ) )
	{
	}

	for ( u32 i = 0; i < DagNode.childCount(); i++ )
	{
		MObject Object = DagNode.child( i );
		DagPath.push( Object );	
		WalkGraph( pNode, DagPath );
		DagPath.pop();
	}
	return pNode;
}

MStatus	CFuseExporter::writer (	const MFileObject& file, const MString& optionsString, 
								MPxFileTranslator::FileAccessMode mode )
{
	MStatus Status;
	try
	{
		MItDag itDag( MItDag::kDepthFirst, MFn::kInvalid, &Status );
		CHECK_ERROR( Status );

		MDagPath DagPath;
		CHECK_ERROR( itDag.getPath(DagPath) );
		m_pRootNode = WalkGraph( NULL, DagPath );
		m_pRootNode->DebugDumpSceneGraph();

		Status = MStatus::kSuccess;
	}
	catch ( EMayaError Error )
	{
		Status = Error.GetStatus();
	}
	if ( m_pRootNode )
		m_pRootNode->DeleteSceneGraph();
	for ( u32 i = 0; i < m_Resources.size(); i++ )
		delete m_Resources[i];
	m_Resources.clear();
	m_ProgressLog.clear();
	return Status;
}