//gchant.cpp

#include "gchant.h"
#include <gtkmm/stock.h>

GChant::GChant() {
	fichier_annot="";
	set_title("GChant");
	
	//Taille de la fenetre : 2000px*2000px
	set_default_size(2000, 2000);
	
	//Ajout de la Box principale a la fenetre
	add(m_Box); 
	
	/////////////////////////////////////////////////////////////////////////////
	////////////////	 C R E A T I O N	 D U	 M E N U  ///////////////////
	/////////////////////////////////////////////////////////////////////////////

	//Create actions for menus and toolbars:
	m_refActionGroup = Gtk::ActionGroup::create();

	//File|New sub menu:
	m_refActionGroup->add(Gtk::Action::create("Open", Gtk::Stock::OPEN, "Ouvrir", "Ouvrir un fichier .dxf"),
					sigc::mem_fun(*this, &GChant::on_menu_file_open));

	m_refActionGroup->add(Gtk::Action::create("Quit", Gtk::Stock::QUIT, "Quitter"),
					sigc::mem_fun(*this, &GChant::on_menu_file_quit));

	m_refUIManager = Gtk::UIManager::create();
	m_refUIManager->insert_action_group(m_refActionGroup);

	Glib::ustring ui_info = 
				"<ui>"
				"	<toolbar name='ToolBar'>"
				"		<toolitem action='Open'/>"
				"		<toolitem action='Quit'/>"
				"	</toolbar>"
				"</ui>";


	#ifdef GLIBMM_EXCEPTIONS_ENABLED
	try {
		m_refUIManager->add_ui_from_string(ui_info);
	} catch (const Glib::Error& ex) {
		cerr << "Erreur lors de la creation du menu : " << ex.what();
	}
	#else
	auto_ptr<Glib::Error> ex;
	m_refUIManager->add_ui_from_string(ui_info, ex);
	if (ex.get()) {
		cerr << "Erreur lors de la creation du menu : " << ex->what();
	}
	#endif //GLIBMM_EXCEPTIONS_ENABLED

	Gtk::Widget* pToolbar = m_refUIManager->get_widget("/ToolBar") ;
	
	
	if(pToolbar) m_HBox_Menu_Zoom.pack_start(*pToolbar, Gtk::PACK_EXPAND_WIDGET);
	
	
	/////////////////////////////////////////////////////////////////////////////
	///////   C R E A T I O N	   D E S    B O U T O N S  	 Z O O M ////////////
	/////////////////////////////////////////////////////////////////////////////	
	
	m_ButtonZoomM.set_label("Zoom-");
	m_ButtonZoomM.signal_clicked().connect(sigc::mem_fun(*this, &GChant::on_button_ZoomM));
	m_HZoom.pack_start(m_ButtonZoomM, Gtk::PACK_SHRINK);
	
	m_ButtonZoomP.set_label("Zoom+");
	m_ButtonZoomP.signal_clicked().connect(sigc::mem_fun(*this, &GChant::on_button_ZoomP));
	m_HZoom.pack_start(m_ButtonZoomP, Gtk::PACK_SHRINK);
	
	m_HBox_Menu_Zoom.pack_start(m_HZoom, Gtk::PACK_SHRINK);
	m_VBoxArbreAnnotBoutons.pack_start(m_HBox_Menu_Zoom, Gtk::PACK_SHRINK);
	
	
	/////////////////////////////////////////////////////////////////////////////
	//////////////   C R E A T I O N	 D E	L ' A R B R E  	 ////////////////
	/////////////////////////////////////////////////////////////////////////////
	
	//Largeur de la zone de gauche : 300px
	m_VBoxArbreAnnotBoutons.set_size_request(300,-1);

	//Création et Installation du modèle de l'arbre
	m_refTreeModel = Gtk::TreeStore::create(m_Columns);
	
	m_Arbre.set_model(m_refTreeModel);

	//Ajout des colonnes à l'arbre
	m_Arbre.append_column("Calque", m_Columns.m_col_name);
	m_Arbre.append_column_editable("Affichage", m_Columns.m_col_display);
	
	//Connect signal:
	((Gtk::CellRendererToggle*)m_Arbre.get_column_cell_renderer(1))->signal_toggled().connect(sigc::mem_fun(*this, &GChant::on_child_toggled));
	m_Arbre.signal_row_activated().connect(sigc::mem_fun(*this, &GChant::on_treeview_row_activated));

	//Montrer les ascensseurs de la ScrolledWindow quand necessaire :
	m_ScrolledArbre.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	
	//Ajout de l'arbre à sa ScrolledWindow
	m_ScrolledArbre.add(m_Arbre);
	
	//Ajout de la ScrolledWindow à la VBox
	m_VBoxArbreAnnotBoutons.pack_start(m_ScrolledArbre);
	
	/////////////////////////////////////////////////////////////////////////////
	/////////////   C R E A T I O N	   D E S    B O U T O N S  	 ////////////////
	/////////////////////////////////////////////////////////////////////////////
	
	m_ButtonAdd.set_label("Nouvelle");
	m_ButtonAdd.set_sensitive(false);
	m_ButtonAdd.signal_clicked().connect(sigc::mem_fun(*this, &GChant::on_button_add));
	m_HButtonBox.add(m_ButtonAdd);
	
	m_ButtonEdit.set_label("Modifier");
	m_ButtonEdit.set_sensitive(false);
	m_ButtonEdit.signal_clicked().connect(sigc::mem_fun(*this, &GChant::on_button_edit));
	m_HButtonBox.add(m_ButtonEdit);
	
	m_ButtonDelete.set_label("Supprimer");
	m_ButtonDelete.set_sensitive(false);
	m_ButtonDelete.signal_clicked().connect(sigc::mem_fun(*this, &GChant::on_button_delete));
	m_HButtonBox.add(m_ButtonDelete);
	
	//Ajout de la ButtonBox à la VBox
	m_VBoxArbreAnnotBoutons.pack_start(m_HButtonBox, Gtk::PACK_SHRINK);
	
	/////////////////////////////////////////////////////////////////////////////
	//////////   C R E A T I O N    D E     Z O N E  A N N O T  	 ////////////
	/////////////////////////////////////////////////////////////////////////////	
	
	//Initialisation du buffer
	m_buffer = Gtk::TextBuffer::create();
	m_buffer->set_text("\n-------- Aucune annotation sélectionnée --------");
	m_Annot.set_buffer(m_buffer);
	
	//Voir les ascenceurs quand nécessaire
	m_ScrolledAnnot.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	
	//Ajout de l'Annot à sa ScrolledWindow
	m_ScrolledAnnot.add(m_Annot);
	//Ajout de la ScrolledWindow à la VBox
	m_VBoxArbreAnnotBoutons.pack_start(m_ScrolledAnnot, Gtk::PACK_EXPAND_WIDGET);	
	
	//ajout de la VBox ((Arbre)(Annotation)) a la HBox (((Arbre)(Annotation))(DrawingArea))
	m_HBox_VBoxArbreAnnot_HBoxDrawingArea.pack_start(m_VBoxArbreAnnotBoutons, false, true, 0);

	/////////////////////////////////////////////////////////////////////////////
	////////	 C R E A T I O N	 D E	 Z O N E	 D E S S I N	/////////
	/////////////////////////////////////////////////////////////////////////////

	_area = new DXFArea();
	m_ScrolledDrawingArea.add(*_area);
	
	// ajout de la ScrolledBox (DrawingArea) a la VBox (((Arbre)(Annotation))(DrawingArea)).
	m_HBox_VBoxArbreAnnot_HBoxDrawingArea.pack_start(m_ScrolledDrawingArea, Gtk::PACK_EXPAND_WIDGET);
	
	// FINALISATION
	// ajout de la HBox (((Arbre)(Annotation))(DrawingArea))	a	la VBox principale	(m_Box).
	m_Box.pack_start(m_HBox_VBoxArbreAnnot_HBoxDrawingArea, Gtk::PACK_EXPAND_WIDGET);
	
	show_all_children();
}

GChant::~GChant(){}

void GChant::on_child_toggled(Glib::ustring str)
{
	Gtk::TreeModel::Path path(str.c_str());
	Gtk::TreeModel::Row row = *(m_refTreeModel->get_iter(path));
	string nomCalque = row.get_value(m_Columns.m_col_name);
	bool isDisplayed = row.get_value(m_Columns.m_col_display);
	if (isDisplayed) {
		_area->afficherCalque(nomCalque);
	} else {
		_area->masquerCalque(nomCalque);
	}
	_area->reDessiner();
}


void GChant::lireDXF(string file) {
	_handler = new DXFHandler();
	DL_Dxf* dxf = new DL_Dxf();
	if (!dxf->in(file, _handler)) {
		cerr << file << " n'a pas pu etre ouvert." << endl;
		return;
	}
	_area->setHandler(*_handler);
}

void GChant::reDessiner() {
	//Fill the TreeView's model
	construireArbre();
	_area->set_size_request(1000, 1000);	//TODO: la taille du dessin normalement...
	_area->reDessiner();
}

void GChant::construireArbre(){
	m_refTreeModel->clear();
	list<string> noms_calques = _handler->getCalques();
	int i=0;
	for(list<string>::iterator nom_calque=noms_calques.begin() ; nom_calque!=noms_calques.end() ; nom_calque++){
		Gtk::TreeModel::Row row;
		string rang;
		stringstream ss;
		ss << i;
		ss >> rang;
		rang.append(":*");//a voir disdonc
		row = *(m_refTreeModel->append());
		row[m_Columns.m_col_name] = *nom_calque;
		row[m_Columns.m_col_display] = true;
		lireAnnot(rang, row);
		i++;
	}
}





void GChant::on_button_ZoomP() {
	m_buffer = Gtk::TextBuffer::create();
	m_buffer->set_text("\ntante----");
	m_Annot.set_buffer(m_buffer);
	setEchelle(_area->getEchelle()+.25);
}
void GChant::on_button_ZoomM() {
	setEchelle(_area->getEchelle()-.25);
}
void GChant::setEchelle(double echelle) {
	_area->setEchelle(echelle);
	_area->reDessiner();
}

//////////////////////////////////////////////////////////////
//////////////// M E T H O D E	 M E N U	//////////////////
//////////////////////////////////////////////////////////////
void GChant::on_menu_file_quit(){ hide(); }

void GChant::on_menu_file_open() {
	Gtk::FileChooserDialog dialog("Choisissez un fichier dxf", Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);
	
	//Add response buttons the the dialog:
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

	//Add filters, so that only certain file types can be selected:

	Gtk::FileFilter filter_dxf;
	filter_dxf.set_name("*.dxf");
	filter_dxf.add_pattern("*.dxf");
	filter_dxf.add_pattern("*.DXF");
	dialog.add_filter(filter_dxf);

	Gtk::FileFilter filter_any;
	filter_any.set_name("Tous les fichiers");
	filter_any.add_pattern("*");
	dialog.add_filter(filter_any);

	//Show the dialog and wait for a user response:
	int result = dialog.run();

	//Handle the response:
	switch (result) {
		case (Gtk::RESPONSE_OK): {
			ouvrir(dialog.get_filename());
			break; }
		case (Gtk::RESPONSE_CANCEL): { break; } 
		default: { break; }
	}
}

void GChant::ouvrir(string fichier_dxf) {
	lireDXF(fichier_dxf);
	fichier_annot = fichier_dxf.replace( fichier_dxf.length()-4, fichier_dxf.length() - (fichier_dxf.length()-4), ".ann");
	cout << "le fichier_annot : "<< fichier_annot << endl;
	reDessiner();
}

//////////////////////////////////////////////////////////////
//////////////// M E T H O D E	 A R B R E	//////////////////
//////////////////////////////////////////////////////////////

void GChant::on_treeview_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* /* column */) {
	string rang = path.to_string();
	if (rang.find(':')!=string::npos ) {
		m_ButtonAdd.set_sensitive(false);
		m_ButtonEdit.set_sensitive(true);
		m_ButtonDelete.set_sensitive(true);
		Gtk::TreeModel::Row row;
		m_buffer->set_text(lireAnnot(rang, row));
		m_Annot.set_buffer(m_buffer);
	} else {
		m_ButtonAdd.set_sensitive(true);
		m_ButtonEdit.set_sensitive(false);
		m_ButtonDelete.set_sensitive(false);
	}
}

//////////////////////////////////////////////////////////////
//////////////// M E T H O D E	 Z O N E	//////////////////
//////////////////////////////////////////////////////////////

string GChant::lireAnnot(string rang, Gtk::TreeModel::Row row) { //lire l'annotation du rang "rale fichier "fic"
	string annotation = "pas d'annotation rentree !";
	if (fichier_annot!=""){
		ifstream fichier(fichier_annot.c_str(), ios::in);
		if (fichier) {
			bool drap=false;
			string str;
			while (getline(fichier, str) && !drap){	 // met dans "str" la ligne
				string ligne = str.c_str();
				int tailleRang = rang.length();
				int i=0;
				if (rang==ligne.substr(0,tailleRang)) { //si on a le rang dans la ligne
					drap=true;                          //rang trouvé, il y en a plus dautre !! (cf. while)
					getline(fichier, str);
					annotation=str.c_str();
				} else if (rang.find('*')!=string::npos) {
					string rangS = rang.substr(0, tailleRang-1);
					std::cout << "le rang" << rang << std::endl;
					if(ligne.find(rangS)!=string::npos){//si le rang etoile est trouve
						Gtk::TreeModel::Row childrow = *(m_refTreeModel->append(row.children()));
						childrow[m_Columns.m_col_name] = ligne.substr(tailleRang+1,ligne.length());
						getline(fichier,str);
						annotation="";//ce nest pas une annotation...
					}
				}
			}
			fichier.close();
		} else {			
		cerr << "Impossible d'ouvrir le fichier d'annotations!" << endl;			
		}
	} else annotation = "pas de fichier d'annotation!";
	return annotation;
}

void GChant::ecrire(string fic, string chaine)
{
	ofstream fichier(fichier_annot.c_str(), ios::out | ios::app);	// ouverture en écriture avec effacement du fichier ouvert
	if(fichier)
	{
		fichier << "0:0Tatante la grosse tantouze" << endl;
		fichier.close();
	}
	else cerr << "Impossible d'ouvrir le fichier d'annotations!" << endl;
}



void GChant::on_button_add() {
	// Unfinished in the original: ecrire() only appended a placeholder line
}

void GChant::on_button_edit() {
	
}

void GChant::on_button_delete() {
	
}

void GChant::ajoutAnnot(Gtk::TreeModel::Row row)
{
 //row.get_path().to_string()
 // fichier("annotations.ann", ios::in);
 string rang = "0:*";
 //if(fichier){
 string nom_annot = lireAnnot(rang, row);
 //while(nom_annot != NULL){
 //nom_annot = lireAnnot("fichier", rang, true, row); //eh oui //rang de type *:*
 //Gtk::TreeModel::Row childrow = *(m_refTreeModel->append(row.children()));
 //childrow[m_Columns.m_col_name] = nom_annot;
 //}
 //}
 //else cerr << "Impossible d'ouvrir le fichier d'annotations!" << endl;
 //fichier.close();
}