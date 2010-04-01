//objects.cpp

//#include <iostream>	//pour le main()
#include <string>

#include "objects.h"


//Object
Object::Object (int couleur, string lineType) {
	_couleur = couleur;
	_lineType = lineType;
}

int Object::getCouleur() {
	return _couleur;
}

double Object::degresToRadians(double x) {
	return x * M_PI / 180.;
}

bool Object::isPointille() {		//si le lineType n'est pas CONTINUOUS, alors il est pointille
	return (_lineType != "CONTINUOUS");
}


//Table
Table::Table (string nom, int couleur, string lineType):Object (couleur, lineType) {
	_nom = nom;
}

string Table::getNom() {
	return _nom;
}


//Layer
Layer::Layer (string nom, int couleur, string lineType):Table (nom, couleur, lineType) {}


//Block
Block::Block (string nom, int couleur, Layer &calqueParent, string lineType, double bpx, double bpy):Table (nom, couleur, lineType) {
	_calqueParent = &calqueParent;
	_bpx = bpx;
	_bpy = bpy;
	_ipx = .0;
	_ipy = .0;
	_sx = 1.;
	_sy = 1.;
	_angle = .0;
}

Block::Block (string nom, int couleur, Layer &calqueParent, string lineType, double ipx, double ipy, double sx, double sy, double angle):Table (nom, couleur, lineType) {
	_calqueParent = &calqueParent;
	_bpx = .0;
	_bpy = .0;
	_ipx = ipx;
	_ipy = ipy;
	_sx = sx;
	_sy = sy;
	_angle = degresToRadians(angle);
}

void Block::setInsert (Layer &calqueParent, double ipx, double ipy, double sx, double sy, double angle) {
	_calqueParent = &calqueParent;
	_ipx = ipx;
	_ipy = ipy;
	_sx = sx;
	_sy = sy;
	_angle = degresToRadians(angle);
}

void Block::setBasePoints (double bpx, double bpy) {
	_bpx = bpx;
	_bpy = bpy;
}

list<Line *> & Block::getLignes() {
	return _lignes;
}

list<Block *> & Block::getBlocs() {
	return _blocs;
}

list<Arc *> & Block::getArcs() {
	return _arcs;
}

list<Polyline *> & Block::getPolylines() {
	return _polylines;
}

list<Point *> & Block::getPoints() {
	return _points;
}

list<Circle *> & Block::getCercles() {
	return _cercles;
}

list<Text *> & Block::getTextes() {
	return _textes;
}

list<Image *> & Block::getImages() {
	return _images;
}

Layer & Block::getCalque() {
	return *_calqueParent;
}

double Block::getX() {	//WTF?
	return _ipx;
}

double Block::getY() {
	return _ipy;
}

double Block::getBpx() {
	return _bpx;
}

double Block::getBpy() {
	return _bpy;
}

double Block::getSx() {
	return _sx;
}

double Block::getSy() {
	return _sy;
}

double Block::getAngle() {
	return _angle;
}

void Block::addLigne (Line &ligne) {
	_lignes.push_back(&ligne);
}

void Block::addBloc (Block &bloc) {
	_blocs.push_back(&bloc);
}

void Block::addArc (Arc &arc) {
	_arcs.push_back(&arc);
}

void Block::addPolyline (Polyline &polyline) {
	_polylines.push_back(&polyline);
}

void Block::addCercle (Circle &cercle) {
	_cercles.push_back(&cercle);
}

void Block::addPoint (Point &point) {
	_points.push_back(&point);
}

void Block::addTexte (Text &texte) {
	_textes.push_back(&texte);
}

void Block::addImage (Image &image) {
	_images.push_back(&image);
}


//Entity
Entity::Entity (int couleur, Layer &calqueParent, string lineType, double x, double y):Object (couleur, lineType) {
	_x = x;
	_y = y;
	_calqueParent = &calqueParent;
	if (couleur == 256) {
		_couleur = _calqueParent->getCouleur();
	}
}

Layer & Entity::getCalque() {
	return *_calqueParent;
}

double Entity::getX() {
	return _x;
}

double Entity::getY() {
	return _y;
}

bool Entity::isPointille() {
	if (_lineType == "BYLAYER") {
		return _calqueParent->isPointille();
	} else {
		return (_lineType != "CONTINUOUS");
	}
}


//Point
Point::Point (int couleur, Layer & calqueParent, string lineType, double x, double y):Entity (couleur, calqueParent, lineType, x, y) {}


//Line
Line::Line (int couleur, Layer &calqueParent, string lineType, double x, double y, double x2, double y2):Entity (couleur, calqueParent, lineType, x, y) {
	_x2 = x2;
	_y2 = y2;
}

double Line::getX2() {
	return _x2;
}

double Line::getY2() {
	return _y2;
}


//Circle
Circle::Circle (int couleur, Layer &calqueParent, string lineType, double x, double y, double rayon):Entity (couleur, calqueParent, lineType, x, y) {
	_rayon = rayon;
}

double Circle::getRayon() {
	return _rayon;
}


//Arc
Arc::Arc (int couleur, Layer &calqueParent, string lineType, double x, double y, double rayon, double angle1, double angle2):Circle (couleur, calqueParent, lineType, x, y, rayon) {
	_angle1 = degresToRadians(angle1);
	_angle2 = degresToRadians(angle2);
}

double Arc::getAngle1() {
	return _angle1;
}

double Arc::getAngle2() {
	return _angle2;
}


//Polyline
Polyline::Polyline (Layer &calqueParent, string lineType, int flags):Entity (.0, calqueParent, "CONTINUOUS", .0, .0) {
	_flags = flags;
}

int Polyline::getFlags() {
	return _flags;
}

void Polyline::addPoint(Point &point) {
	_points.push_back(&point);
}

list <Point *> &Polyline::getPoints() {
	return _points;
}


//Text
Text::Text (int couleur, Layer &calqueParent, string lineType, double x, double y, string texte, string style, double taille, double xScaleFactor):Entity (couleur, calqueParent, lineType, x, y) {
	_texte = texte;
	_style = style;
	_taille = taille;
	_xScaleFactor = xScaleFactor;
}

string Text::getTexte() {
	return _texte;
}

string Text::getStyle() {
	return _style;
}

double Text::getTaille() {
	return _taille;
}

double Text::getXScaleFactor() {
	return _xScaleFactor;
}


//Image
Image::Image (Layer &calqueParent, string ref, double x, double y, double width, double height):Entity (256, calqueParent, "CONTINUOUS", x, y) {
	_ref = ref;
	_width = width;
	_height = height;
}

void Image::setFile (string file) {
	_file = file;
}

string Image::getRef() {
	return _ref;
}

string Image::getFile() {
	return _file;
}

double Image::getWidth() {
	return _width;
}

double Image::getHeight() {
	return _height;
}


/*
//main
int main() {
	
	string nomCalque = "tante";
	string nomLigne = "touse";
	
	Layer *calque1 = new Layer(nomCalque, 2);
	Line *ligne = new Line(nomLigne, 256, *calque1, 1.4, 2.4, 4.1, 4.2);
	
	Layer &calque2 = ligne->getCalque();		//comment utiliser getCalque()
	
	Layer *calque3 = new Layer(ligne->getCalque());		//autre facon d'utiliser getCalque()
	
	cout << "string nomCalque = \"tante\";" << endl;
	cout << "string nomLigne = \"touse\";" << endl;
	cout << endl;
	
	cout << "Layer *calque1 = new Layer(nomCalque, 2);" << endl;
	cout << "calque1->getNom() : " << calque1->getNom() << endl;
	cout << "calque1->getCouleur() : " << calque1->getCouleur() << endl;
	cout << endl;
	
	cout << "Line *ligne = new Line(nomLigne, 256, *calque1, 1.4, 2.4, 4.1, 4.2);" << endl;
	cout << "ligne->getNom() : " << ligne->getNom() << endl;
	cout << "ligne->getCouleur() : " << ligne->getCouleur() << endl;
	cout << "ligne->getX1() : " << ligne->getX1() <<endl;
	cout << "ligne->getX2() : " << ligne->getX2() <<endl;
	cout << "ligne->getY1() : " << ligne->getY1() <<endl;
	cout << "ligne->getY2() : " << ligne->getY2() <<endl;
	cout << endl;
	
	cout << "Layer &calque2 = ligne->getCalque();" << endl;
	cout << "calque2.getNom() : " << calque2.getNom() << endl;
	cout << "calque2.getCouleur() : " << calque2.getCouleur() << endl;
	cout << endl;
	
	cout << "Layer *calque3 = new Layer(ligne->getCalque());" << endl;
	cout << "calque3->getNom() : " << calque3->getNom() << endl;
	cout << "calque3->getCouleur() : " << calque3->getCouleur() << endl;
	cout << endl;

	return 0;
}
*/
