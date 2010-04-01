#ifndef DXFHANDLER_H
#define DXFHANDLER_H

#include <map>
#include <string>
#include <list>

#include "dxflib/dl_creationadapter.h"
#include "src/objects.h"

/**
 * This class takes care of the entities read from the file.
 * Usually such a class would probably store the entities.
 * this one just prints some information about them to stdout.
 *
 * @author Une Tante
 */

using namespace std;

typedef map <string, Block *> mapBlocs;
typedef map <string, Layer *> mapCalques;
typedef map <string, Image *> mapImages;

class DXFHandler : public DL_CreationAdapter {
	
protected:

	mapCalques _mapCalques;
	mapBlocs _mapBlocs;
	mapImages _mapImages;
	Block *_blocRoot;
	Block *_blocCourant;
	
public:
	
	DXFHandler();

	virtual void addLayer	(const DL_LayerData& data);
	virtual void addBlock	(const DL_BlockData& data);
	virtual void endBlock	();
	virtual void addInsert	(const DL_InsertData& data);
	virtual void addLine	(const DL_LineData& data);
	virtual void addArc		(const DL_ArcData& data);
	virtual void addPolyline(const DL_PolylineData& data);
	virtual void addVertex	(const DL_VertexData& data);
	virtual void addPoint	(const DL_PointData& data);
	virtual void addCircle	(const DL_CircleData& data);
	virtual void addText	(const DL_TextData& data);
	virtual void addImage	(const DL_ImageData& data);
	virtual void linkImage	(const DL_ImageDefData& data);
	/*
	virtual void add3dFace(const DL_3dFaceData& data);
	*/

	Block *getBlocRoot();
	list <string> getCalques();
	void printAttributes();
};

#endif	// DXFHANDLER_H
