//dxfarea.cpp

#include <iostream>

#include "dxfarea.h"

using namespace std;

vector<double> DXFArea::traitPointille;
int niveau = 0;	//pour l'affichage

DXFArea::DXFArea() {
	_handler = new DXFHandler();
	traitPointille.push_back(10.);
	traitPointille.push_back(4.);
}

DXFArea::~DXFArea() {}

bool DXFArea::contains(string nom) {
	bool fin = false;
	list<string>::iterator courant = _calquesMasques.begin();
	while (!fin && courant != _calquesMasques.end()) {
		if (*courant == nom) {
			fin = true;
		} else {
			courant++;
		}
	}
	return fin;
}

void DXFArea::reDessiner() {
	queue_draw();
}

void DXFArea::masquerCalque (string nom) {
	_calquesMasques.push_back(nom);
}

void DXFArea::afficherCalque (string nom) {
	_calquesMasques.remove(nom);
}

double DXFArea::getEchelle() {
	return _echelle;
}

void DXFArea::setEchelle (double echelle) {
	_echelle = echelle;
}

void DXFArea::setHandler (DXFHandler &handler) {
	_handler = &handler;
}

list <string> DXFArea::getCalques() {
	return _handler->getCalques();
}

void DXFArea::drawLignes (Cairo::RefPtr<Cairo::Context> &cr, list<Line *> &lignes) {
	for (list<Line *>::iterator courant = lignes.begin() ; courant != lignes.end() ; courant++) {
		
		if (!contains((*courant)->getCalque().getNom())) {
			for (int i=0;i<niveau;i++) cout << "\t";
			cout << "ligne" << endl;
			
			cr->save();
			
			double *couleur = (double *) &dxfColors[(*courant)->getCouleur()];
			cr->set_source_rgb(couleur[0], couleur[1], couleur[2]);
			
			if ((*courant)->isPointille()) cr->set_dash(traitPointille, .0);
			
			cr->move_to((*courant)->getX(), (*courant)->getY()); //data.x1, data.y1
			cr->line_to((*courant)->getX2(), (*courant)->getY2()); //data.x2, data.y2
			
			cr->stroke();
			cr->unset_dash();
		}
		
	}
}

void DXFArea::drawArcs (Cairo::RefPtr<Cairo::Context> &cr, list<Arc *> &arcs) {
	for (list<Arc *>::iterator courant = arcs.begin() ; courant != arcs.end() ; courant++) {
		
		if (!contains((*courant)->getCalque().getNom())) {
			for (int i=0;i<niveau;i++) cout << "\t";
			cout << "arc" << endl;
			
			cr->save();
			
			double *couleur = (double *) &dxfColors[(*courant)->getCouleur()];
			cr->set_source_rgb(couleur[0], couleur[1], couleur[2]);
			
			if ((*courant)->isPointille()) cr->set_dash(traitPointille, .0);
			
			cr->arc((*courant)->getX(), (*courant)->getY(), (*courant)->getRayon(), (*courant)->getAngle1(), (*courant)->getAngle2());
			
			cr->stroke();
			cr->unset_dash();
		}
	}//Arc
}

void DXFArea::drawPolylines (Cairo::RefPtr<Cairo::Context> &cr, list<Polyline *> &polylines) {
	for (list<Polyline *>::iterator courant = polylines.begin() ; courant != polylines.end() ; courant++) {
		
		if (!contains((*courant)->getCalque().getNom())) {
			for (int i=0;i<niveau;i++) cout << "\t";
			cout << "polyline" << endl;
			
			list<Point *> &polyPoints = (*courant)->getPoints();
			Point *precedent = polyPoints.front();
			
			for (list<Point *>::iterator pcourant = polyPoints.begin() ; pcourant != polyPoints.end() ; pcourant++) {
				
				for (int i=0;i<niveau;i++) cout << "\t";
				cout << "\tpolypoint" << endl;
				
				cr->save();
				
				double *couleur = (double *) &dxfColors[(*pcourant)->getCouleur()];
				cr->set_source_rgb(couleur[0], couleur[1], couleur[2]);
				
				if ((*courant)->isPointille()) cr->set_dash(traitPointille, .0);
				
				cr->move_to(precedent->getX(), precedent->getY());
				cr->line_to((*pcourant)->getX(), (*pcourant)->getY());
				
				cr->stroke();
				cr->unset_dash();
				
				precedent = (*pcourant);
			}
			
			int flags = (*courant)->getFlags();
			
			if (flags == 1) { //Si le flag dit de fermer, on ferme ...
				// Fermeture du polygone avec le premier et dernier vertex
				cr->save();
				
				if ((*courant)->isPointille()) cr->set_dash(traitPointille, .0);
				
				cr->move_to(polyPoints.front()->getX(), polyPoints.front()->getY());
				cr->line_to(polyPoints.back()->getX(), polyPoints.back()->getY());
				
				cr->stroke();
				cr->unset_dash();
			}
		}
	}//Polyline
}

void DXFArea::drawPoints (Cairo::RefPtr<Cairo::Context> &cr, list<Point *> &points) {
	for (list<Point *>::iterator courant = points.begin() ; courant != points.end() ; courant++) {
		
		if (!contains((*courant)->getCalque().getNom())) {
			for (int i=0;i<niveau;i++) cout << "\t";
			cout << "point" << endl;
			
			cr->save();
			
			double *couleur = (double *) &dxfColors[(*courant)->getCouleur()];
			cr->set_source_rgb(couleur[0], couleur[1], couleur[2]);
			
			cr->arc((*courant)->getX(), (*courant)->getY(), 1., 0.0, 2.0*M_PI);
			cr->fill_preserve();
			
			cr->stroke();
		}
	}//Point
}

void DXFArea::drawCercles (Cairo::RefPtr<Cairo::Context> &cr, list<Circle *> &cercles) {
	for (list<Circle *>::iterator courant = cercles.begin() ; courant != cercles.end() ; courant++) {
		
		if (!contains((*courant)->getCalque().getNom())) {
			for (int i=0;i<niveau;i++) cout << "\t";
			cout << "cercle" << endl;
			
			cr->save();
			
			double *couleur = (double *) &dxfColors[(*courant)->getCouleur()];
			cr->set_source_rgb(couleur[0], couleur[1], couleur[2]);
			
			if ((*courant)->isPointille()) cr->set_dash(traitPointille, .0);
			
			cr->arc((*courant)->getX(), (*courant)->getY(), (*courant)->getRayon(), 0.0, 2.0*M_PI);
			
			cr->stroke();
			cr->unset_dash();
		}
	}//Circle	
}

void DXFArea::drawTextes (Cairo::RefPtr<Cairo::Context> &cr, list<Text *> &textes) {
	for (list<Text *>::iterator courant = textes.begin() ; courant != textes.end() ; courant++) {
		
		if (!contains((*courant)->getCalque().getNom())) {
			for (int i=0;i<niveau;i++) cout << "\t";
			cout << "texte" << endl;
			
			cr->save();
			
			double *couleur = (double *) &dxfColors[(*courant)->getCouleur()];
			cr->set_source_rgb(couleur[0], couleur[1], couleur[2]);
			
			cr->select_font_face((*courant)->getStyle(), Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
			cr->set_font_size((*courant)->getTaille()*(*courant)->getXScaleFactor());
			
			if ((*courant)->isPointille()) cr->set_dash(traitPointille, .0);
			
			cr->move_to((*courant)->getX(), (*courant)->getY());
			cr->transform(Cairo::Matrix(1,0,0,-1,0,0));
			cr->show_text((*courant)->getTexte());
			cr->transform(Cairo::Matrix(1,0,0,-1,0,0));
			cr->fill_preserve();
			
			cr->stroke();
			cr->unset_dash();
		}
	}//Text
}

void DXFArea::drawImages (Cairo::RefPtr<Cairo::Context> &cr, list<Image *> &images) {
	for (list<Image *>::iterator courant = images.begin() ; courant != images.end() ; courant++) {
		
		if (!contains((*courant)->getCalque().getNom())) {
			for (int i=0;i<niveau;i++) cout << "\t";
			cout << "image" << endl;
			
			cr->save();
						
			try {
				Glib::RefPtr<Gdk::Pixbuf> image = Gdk::Pixbuf::create_from_file((*courant)->getFile());
				image->render_to_drawable(get_window(), get_style()->get_black_gc(), 0, 0, (*courant)->getX(), (*courant)->getY(), image->get_width(), image->get_height(), Gdk::RGB_DITHER_NONE, 0, 0);
			} catch (Glib::Error) {
				cr->move_to((*courant)->getX(), (*courant)->getY());
				cr->set_font_size (15.0);
				cr->show_text((*courant)->getRef());
			}
			
			cr->stroke();
		}
	}//Image
}

void DXFArea::drawBlocs (Cairo::RefPtr<Cairo::Context> &cr, list<Block *> &blocs) {
	for (list <Block *>::iterator courant = blocs.begin() ; courant != blocs.end() ; courant++) {		//pour tous les blocs de premier niveau		
		
		if (!contains((*courant)->getCalque().getNom())) {
			for (int i=0;i<niveau;i++) cout << "\t";
			cout << "bloc : " << (*courant)->getNom() << endl;
			
			double x = (*courant)->getX();
			double y = (*courant)->getY();
			double sx = (*courant)->getSx();
			double sy = (*courant)->getSy();
			double angle = (*courant)->getAngle();
			
			list<Block *> &sblocs = (*courant)->getBlocs();	//sous-blocs
			list<Line *> &lignes = (*courant)->getLignes();
			list<Arc *> &arcs = (*courant)->getArcs();
			list<Polyline *> &polylines = (*courant)->getPolylines();
			list<Point *> &points = (*courant)->getPoints();
			list<Circle *> &cercles = (*courant)->getCercles();
			list<Text *> &textes = (*courant)->getTextes();
			list<Image *> &images = (*courant)->getImages();
			
			for (int i=0;i<niveau;i++) cout << "\t";
			cout << "translation a x : " << x << ", y : " << y << endl;
			
			cr->translate(x, y);
			//cr->translate(-(*courant)->getBpx(), -(*courant)->getBpy());
			cr->rotate(angle);
			//cr->scale(sx, sy);
			
			niveau++;
			drawLignes(cr, lignes);
			drawArcs(cr, arcs);
			drawPolylines(cr, polylines);
			drawPoints(cr, points);
			drawCercles(cr, cercles);
			//drawTextes(cr, textes);
			drawImages(cr, images);
			drawBlocs(cr, sblocs);
			niveau--;
			
			for (int i=0;i<niveau;i++) cout << "\t";
			cout << "translation a x : " << -x << ", y : " << -y << endl;
			
			//cr->scale(1/sx, 1/sy);
			cr->rotate(-angle);
			//cr->translate((*courant)->getBpx(), (*courant)->getBpy());
			cr->translate(-x, -y);
			
			for (int i=0;i<niveau;i++) cout << "\t";
			cout << "fin du bloc : " << (*courant)->getNom() << endl << endl;
		}
	}
}

bool DXFArea::on_expose_event (GdkEventExpose* event) {
	// This is where we draw on the window
	Glib::RefPtr<Gdk::Window> window = get_window();
	
	if (window) {
		
		_echelle = 1.;		//TODO: caler l'echelle en fonction de la taille du dxf
		//set_size_request(taille du dessin);
		
		Gtk::Allocation allocation = get_allocation();
		const int width = allocation.get_width();
		const int height = allocation.get_height();
		
		// coordinates for the center of the window
		int xc, yc;
		xc = width / 2;
		yc = height / 2;
		
		Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
		
		// clip to the area indicated by the expose event so that we only redraw
		// the portion of the window that needs to be redrawn
		cr->rectangle(event->area.x, event->area.y, event->area.width, event->area.height);
		cr->clip();
		
		cr->set_line_width(1./_echelle);					//Conserver l'epaisseur des traits a 1
		cr->translate(0, height);							//Translation en bas a gauche
		cr->transform(Cairo::Matrix(1, 0, 0, -1, 0, 0));	//Inverser le plan
		cr->scale(_echelle, _echelle);
		cr->save();
		
		list<Block *> root;
		root.push_back(_handler->getBlocRoot());
				
		drawBlocs(cr, root);
		
	}//if
	
	return true;
}
