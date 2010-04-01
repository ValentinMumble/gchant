#ifndef GCHANT_H
#define GCHANT_H

#include <gtkmm.h>
#include <iostream>
#include <string>
#include <fstream>

#include "dxflib/dl_dxf.h"
#include "dxfarea.h"
#include "dxfhandler.h"

using namespace std;

class GChant : public Gtk::Window {
	
public:
	
	GChant();
	virtual ~GChant();
	void lireDXF(string);
	void ajoutAnnot(Gtk::TreeModel::Row row);
	void reDessiner();
	void setEchelle(double);
	
	string lireAnnot(string rang, Gtk::TreeModel::Row row);		//lire l'annotation du "rang" dans le fichier "fic". @return: l'annotation 
	//char* lireNomAnnot( char* rang);	
	void ecrire(string fic, string chaine); //ecrire la "chaine" a la suite du fichier "fic". @return: void
																																						//de la chaine "*s". @return: la chaine (l'annotation)
protected:
	
	class ModelColumns : public Gtk::TreeModel::ColumnRecord {
	public:
		Gtk::TreeModelColumn<bool> m_col_display;
		Gtk::TreeModelColumn<string> m_col_name;
		ModelColumns() {
			add(m_col_display);
			add(m_col_name);  
		}
	};

	void on_treeview_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
	void on_menu_file_quit();
	void on_menu_file_open();
	void on_button_add();
	void on_button_edit();
	void on_button_delete();
	void on_child_toggled(Glib::ustring str);
	void construireArbre();
	void on_button_ZoomP();
	void on_button_ZoomM();
	
	//std::ifstream fichier;
	ModelColumns m_Columns;											//le modele de colonnes pour l'arbre
	//string ligne_active;									//le nom du reseau selectionne
	Gtk::TreeView m_Arbre;											 //l'arbre des reseaux/annotations
	Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
	Glib::RefPtr<Gtk::TreeStore> m_refTreeModel; //la reference pour construire l'arbre
	Gtk::TextView m_Annot;											 //l'annotation
	Gtk::ScrolledWindow m_ScrolledAnnot;				 //la zone scrollable de l'annotation
	//Gtk::Viewport *m_Viewport;									//eventuellement, pour la mise en forme de la drawing area
	Gtk::VBox m_Box;														 //la Box principale
	Glib::RefPtr<Gtk::UIManager> m_refUIManager; //la reference pour construire le menu
	Gtk::Button m_ButtonAdd;										 //le bouton d'ajout d'annotations
	Gtk::Button m_ButtonEdit;										//le bouton d'edition d'annotations
	Gtk::Button m_ButtonDelete;									//le bouton de suppression d'annotations
	Gtk::VBox m_VBoxArbreAnnotBoutons;					 //la VBox Menu/Arbre/Boutons/Annot
	Gtk::HButtonBox m_HButtonBox;								//la HBox Boutons
	Gtk::HBox m_HBox_VBoxArbreAnnot_HBoxDrawingArea;//la HBox reunissant la VBoxArbreAnnotBoutons et la ScrolledDrawingArea
	Glib::RefPtr<Gtk::TextBuffer> m_buffer;			//le buffer pour afficher le texte des annotations
	Gtk::ScrolledWindow m_ScrolledDrawingArea;	 //la Scrolled pour la drawing area
	Gtk::ScrolledWindow m_ScrolledArbre;				 //la Scrolled pour l'arbre
	
	Gtk::HBox m_HBox_Menu_Zoom;
	Gtk::HBox m_HZoom;
	Gtk::Button m_ButtonZoomP;
	Gtk::Button m_ButtonZoomM;
	
	string fichier_annot;
	
	DXFArea *_area;
	DXFHandler *_handler;
};

#endif //GCHANT_H
