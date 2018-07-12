// FuseExporter.h

#pragma once

class MFnDagNode;

namespace Fuse
{
	class CResource;
	class CNode;
};

namespace FuseExporter
{

//
// Fuse Maya exporter.
//

class CFuseExporter : public MPxFileTranslator
{

public :

	// Default constructor.
	CFuseExporter();
	// Destructor.
	virtual ~CFuseExporter();

	static	void*	Creator();

	virtual	bool	canBeOpened () const;

	virtual MString	defaultExtension () const;
	virtual	MString filter () const;

	virtual bool	haveWriteMethod () const;
	virtual bool	haveReadMethod () const;

	virtual MStatus	writer (const MFileObject& file,
							const MString& optionsString,
							MPxFileTranslator::FileAccessMode mode);

private :

	struct CProgressMessage
	{
		string	szMessage;
	};

	CNode*						m_pRootNode;
	vector<CProgressMessage>	m_ProgressLog;
	vector<CResource*>			m_Resources;

	void	ConvertTransform(	MDagPath DagPath, CVector& vPosition, CMatrix3& mRotation, CMatrix4& mGeomTransform );

	bool	IsVisible( MFnDagNode& fnDag );

	CNode*	WalkGraph( CNode* pParent, MDagPath DagPath );

};

}