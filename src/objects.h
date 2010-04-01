#ifndef OBJECTS_H
#define OBJECTS_H
//objects.h

#include <list>
#include <string>

#include "dxflib/dl_codes.h"

using namespace std;


//Object
class Object {		//abstraite
	
protected:
	Object (int, string);
	
	int _couleur;
	string _lineType;
	
public:
	int getCouleur();
	bool isPointille();
	double degresToRadians(double);
};


//Table
class Table : public Object {		//abstraite
	
protected:
	string _nom;
	
public:
	Table (string, int, string);
	string getNom();
	
};


//Layer
class Layer : public Table {
	
protected:
	
	
public:
	Layer (string, int, string);
	
	
};


//Entity
class Entity : public Object {		//abstraite
	
protected:
	Entity (int, Layer &, string, double, double);
	
	double _x, _y;
	Layer *_calqueParent;
	
public:
	Layer &getCalque();
	
	bool isPointille();
	double getX();
	double getY();
};


//Point
class Point : public Entity {
	
public:
	Point (int, Layer &, string, double, double);
};


//Line
class Line : public Entity {
	
protected:
	double _x2, _y2;
	
	
public:
	Line(int, Layer &, string, double, double, double, double);
	
	double getX2();
	double getY2();
};


//Circle
class Circle : public Entity {
	
protected:
	double _rayon;
	
public:
	Circle (int, Layer &, string, double, double, double);
	
	double getRayon();
};


//Arc
class Arc : public Circle {

protected:
	double _angle1, _angle2;
	
public:
	Arc (int, Layer &, string, double, double, double, double, double);
	
	double getAngle1();
	double getAngle2();
};


//Polyline
class Polyline : public Entity {

protected:
	list <Point *> _points;
	int _flags;
	
public:
	Polyline (Layer &, string, int);
	
	int getFlags();
	void addPoint(Point &);
	list <Point *> &getPoints();
};


//Text
class Text : public Entity {
	
protected:
	string _texte;
	string _style;
	double _xScaleFactor;
	double _taille;
	
public:
	Text (int, Layer &, string, double, double, string, string, double, double);
	
	string getTexte();
	string getStyle();
	double getXScaleFactor();
	double getTaille();
};


//Image
class Image : public Entity {

protected:
	string _ref;
	string _file;
	double _width;
	double _height;
	
public:
	Image (Layer &, string, double, double, double, double);
	
	void setFile(string);
	string getRef();
	string getFile();
	double getWidth();
	double getHeight();
};


//Block
class Block : public Table {
	
protected:
	Layer *_calqueParent;
	double _bpx;
	double _bpy;
	double _ipx;
	double _ipy;
	double _sx;
	double _sy;
	double _angle;
	
	list <Block *> _blocs;
	list <Line *> _lignes;
	list <Arc *> _arcs;
	list <Point *> _points;
	list <Circle *> _cercles;
	list <Text *> _textes;
	list <Image *> _images;
	
public:
	Block (string, int, Layer &, string, double, double);							//Constructeur par points de base
	Block (string, int, Layer &, string, double, double, double, double, double);	//Constructeur par points d'INSERT
	
	list <Polyline *> _polylines;
	
	list <Block *> &getBlocs();
	list <Polyline *> &getPolylines();
	list <Line *> &getLignes();
	list <Arc *> &getArcs();
	list <Circle *> &getCercles();
	list <Point *> &getPoints();
	list <Text *> &getTextes();
	list <Image *> &getImages();
	
	void setInsert(Layer &, double, double, double, double, double);	//pour rentrer les donnees de l'INSERT
	void setBasePoints(double, double);
	Layer &getCalque();
	double getX();
	double getY();
	double getBpx();
	double getBpy();
	double getSx();
	double getSy();
	double getAngle();
	void addLigne(Line &);
	void addBloc(Block &);
	void addArc(Arc &);
	void addPolyline(Polyline &);
	void addCercle(Circle &);
	void addPoint(Point &);
	void addTexte(Text &);
	void addImage(Image &);
};

#endif
