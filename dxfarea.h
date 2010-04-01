#ifndef DXFAREA_H
#define DXFAREA_H

#include <list>

#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>	//pour le Cairo::Context
#include "dxfhandler.h"
#include "src/objects.h"

class DXFArea : public Gtk::DrawingArea {
	
public:
	
	DXFArea();
	virtual ~DXFArea();
	void reDessiner();
	void masquerCalque(string);
	void afficherCalque(string);
	double getEchelle();
	void setEchelle(double);
	void setHandler(DXFHandler &);
	list <string> getCalques();
	
protected:
	
	static vector<double> traitPointille;
	
	double _echelle;
	DXFHandler *_handler;
	list <string> _calquesMasques;
	
	//Override default signal handler:
	virtual bool on_expose_event(GdkEventExpose* event);
	
	void drawBlocs		(Cairo::RefPtr<Cairo::Context> &, list<Block *> &);
	void drawLignes		(Cairo::RefPtr<Cairo::Context> &, list<Line *> &);
	void drawArcs		(Cairo::RefPtr<Cairo::Context> &, list<Arc *> &);
	void drawPolylines	(Cairo::RefPtr<Cairo::Context> &, list<Polyline *> &);
	void drawPoints		(Cairo::RefPtr<Cairo::Context> &, list<Point *> &);
	void drawCercles	(Cairo::RefPtr<Cairo::Context> &, list<Circle *> &);
	void drawTextes		(Cairo::RefPtr<Cairo::Context> &, list<Text *> &);
	void drawImages		(Cairo::RefPtr<Cairo::Context> &, list<Image *> &);
	bool contains (string);
};

#endif // DXFAREA_H
