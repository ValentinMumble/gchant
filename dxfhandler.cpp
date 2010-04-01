//dxfhandler.cpp

#include <iostream>

#include "dxfhandler.h"


//Default constructor.
DXFHandler::DXFHandler() {
	Layer *calqueRoot = new Layer("__root__", 256, "CONTINUOUS");
	_blocRoot = new Block("__root__", 256, *calqueRoot, "CONTINUOUS", .0, .0);
	_blocCourant = _blocRoot;
}

//Ajout des calques
void DXFHandler::addLayer (const DL_LayerData& data) {
	cout << "LAYER:	" << data.name << ", flags:	" << data.flags << endl;
	printAttributes();
	
	_mapCalques[data.name] = new Layer(data.name, attributes.getColor(), attributes.getLineType());
}

//Ajout des blocs
void DXFHandler::addBlock (const DL_BlockData& data) {
	cout << "BLOCK:	" << data.name << endl;
	cout << "	bpx: " << data.bpx << ", bpy: " << data.bpy << endl;
	cout << "	flags:	" << data.flags << endl;
	printAttributes();
	
	if (_mapBlocs[data.name] == NULL) {		//si le bloc n'a pas ete reference par un INSERT
		Block *bloc = new Block(data.name, attributes.getColor(), *_mapCalques[attributes.getLayer()], attributes.getLineType(), data.bpx, data.bpy);
		_mapBlocs[data.name] = bloc;
		_blocCourant = bloc;
	} else {								//si le bloc a deja ete reference par un INSERT
		_mapBlocs[data.name]->setBasePoints(data.bpx, data.bpy);
		_blocCourant = _mapBlocs[data.name];
	}

}

void DXFHandler::endBlock() {
	cout << "Fin du bloc : " << _blocCourant->getNom() << endl << endl;
	_blocCourant = _blocRoot;
}

//Ajout des insert
void DXFHandler::addInsert (const DL_InsertData& data) {
	cout << "INSERT:	" << data.name << endl;
	cout << "	ipx: " << data.ipx << ", ipy: " << data.ipy << endl;
	cout << "	sx: " << data.sx << ", sy: " << data.sy << ", angle: " << data.angle << endl;
	printAttributes();
	
	Block *bloc;
	if (_mapBlocs[data.name] != NULL) {		//si le bloc concerne est deja reference
		bloc = new Block(*_mapBlocs[data.name]);
		bloc->setInsert(*_mapCalques[attributes.getLayer()], data.ipx, data.ipy, data.sx, data.sy, data.angle);
	} else {								//s'il n'est pas reference
		bloc = new Block(data.name, attributes.getColor(), *_mapCalques[attributes.getLayer()], attributes.getLineType(), data.ipx, data.ipy, data.sx, data.sy, data.angle);
		_mapBlocs[data.name] = bloc;
	}
	_blocCourant->addBloc(*bloc);
}

//Ajout des lignes
void DXFHandler::addLine (const DL_LineData& data) {
	cout << "LINE:		(" << data.x1 << ", " << data.y1 << ") (" << data.x2 << ", " << data.y2 << ")" << endl;
	printAttributes();
		
	Line *ligne = new Line(attributes.getColor(), *_mapCalques[attributes.getLayer()], attributes.getLineType(), data.x1, data.y1, data.x2, data.y2);
	_blocCourant->addLigne(*ligne);
}

//Ajout des arcs
void DXFHandler::addArc (const DL_ArcData& data) {
	cout << "ARC:		(" << data.cx << ", " << data.cy << "), radius: " << data.radius << ", angle1: " << data.angle1 << ", angle2: " << data.angle2 << endl;
	printAttributes();
	
	Arc *arc = new Arc(attributes.getColor(), *_mapCalques[attributes.getLayer()], attributes.getLineType(), data.cx, data.cy, data.radius, data.angle1, data.angle2);
	_blocCourant->addArc(*arc);
}

//Ajout des points
void DXFHandler::addPoint (const DL_PointData& data) {
	cout << "POINT:		(" << data.x << ", " << data.y << ")" << endl;
	printAttributes();
	
	Point *point = new Point(attributes.getColor(), *_mapCalques[attributes.getLayer()], attributes.getLineType(), data.x, data.y);
	_blocCourant->addPoint(*point);
}

//Ajout des cercles
void DXFHandler::addCircle (const DL_CircleData& data) {
	cout << "CIRCLE:		(" << data.cx << ", " << data.cy << "), radius: " << data.radius << endl;
	printAttributes();
	
	Circle *cercle = new Circle(attributes.getColor(), *_mapCalques[attributes.getLayer()], attributes.getLineType(), data.cx, data.cy, data.radius);
	_blocCourant->addCercle(*cercle);
}

//Ajout des polylines
void DXFHandler::addPolyline (const DL_PolylineData& data) {
	cout << "POLYLINE, 	flags: " << data.flags << endl;
	printAttributes();
	
	Polyline *polyline = new Polyline(*_mapCalques[attributes.getLayer()], attributes.getLineType(), data.flags);
	_blocCourant->addPolyline(*polyline);
}


//Ajout des vertex
void DXFHandler::addVertex (const DL_VertexData& data) {
	cout << "VERTEX:		(" << data.x << ", " << data.y << "), bulge: " << data.bulge << endl;
	printAttributes();
	
	Point *point = new Point(attributes.getColor(), *_mapCalques[attributes.getLayer()], attributes.getLineType(), data.x, data.y);
	_blocCourant->_polylines.back()->addPoint(*point);
}

//Ajout des textes
void DXFHandler::addText (const DL_TextData& data) {
	cout << "TEXT:	" << data.text << endl;
	cout << "	style: " << data.style << ", height: " << data.height << ", xScaleFactor: " << data.xScaleFactor << endl;
	printAttributes();
	
	Text *texte = new Text(attributes.getColor(), *_mapCalques[attributes.getLayer()], attributes.getLineType(), data.ipx, data.ipy, data.text, data.style, data.height, data.xScaleFactor);
	_blocCourant->addTexte(*texte);
}

void DXFHandler::addImage (const DL_ImageData& data) {
	cout << "IMAGE:	" << data.ref << ", ipx : " << data.ipx << ", ipy : " << data.ipy << ", width : " << data.width << ", height : " << data.height << endl;
	printAttributes();
	
	Image *image = new Image(*_mapCalques[attributes.getLayer()], data.ref, data.ipx, data.ipy, data.width, data.height);
	_mapImages[data.ref] = image;
	_blocCourant->addImage(*image);
}

void DXFHandler::linkImage (const DL_ImageDefData& data) {
	cout << "DefImage:	" << data.ref << ", file : " << data.file << endl;
	printAttributes();
	
	_mapImages[data.ref]->setFile(data.file);
}
/*
 void DXFHandler::add3dFace(const DL_3dFaceData& data) {
 printf("3DFACE\n");
 for (int i=0; i<4; i++) {
 printf("	 corner %d: %6.3f %6.3f\n", i, data.x[i], data.y[i]);
 }
 printAttributes();
 }
 */

Block * DXFHandler::getBlocRoot() {
	return _blocRoot;
}

list <string> DXFHandler::getCalques() {
	list <string> liste;
	for (mapCalques::iterator courant = _mapCalques.begin() ; courant != _mapCalques.end() ; courant++) {
		liste.push_back(courant->first);
	}
	return liste;
}

void DXFHandler::printAttributes() {
	cout << "	Attributes: Layer: " << attributes.getLayer() << ", Color: ";
	if (attributes.getColor()==256)	{
		cout << "BYLAYER";
	} else if (attributes.getColor()==0) {
		cout << "BYBLOCK";
	} else {
		cout << attributes.getColor();
	}
	cout << ", Width: ";
	if (attributes.getWidth()==-1) {
		cout << "BYLAYER";
	} else if (attributes.getWidth()==-2) {
		cout << "BYBLOCK";
	} else if (attributes.getWidth()==-3) {
		cout << "DEFAULT";
	} else {
		cout << attributes.getWidth();
	}
	cout << ", lineType: " << attributes.getLineType() << endl << endl;
}

// EOF
