//=======================================================================
// db.cc
//-----------------------------------------------------------------------
// This file is part of the package porg
// Copyright (C) 2015 David Ricart
// For more information visit http://porg.sourceforge.net
//=======================================================================

#include "config.h"
#include "opt.h"
#include "db.h"
#include "util.h"
#include <gtkmm/messagedialog.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/image.h>
#include <glibmm/fileutils.h>	// Dir

using std::string;
using namespace Grop;

float 				DB::s_total_size = 0;
std::vector<Pkg*> 	DB::s_pkgs;
bool				DB::s_initialized = false;


DB::DB()
{
	g_assert(s_initialized == false);
	g_assert(Opt::initialized());

	Opt::check_logdir();

	Gtk::MessageDialog dialog("Reading database  (" + Opt::logdir() + ")", false, 
		Gtk::MESSAGE_INFO, Gtk::BUTTONS_NONE, true);
	dialog.set_title("grop :: info");
	
	Gtk::ProgressBar* progressbar(Gtk::manage(new Gtk::ProgressBar()));;
	dialog.get_message_area()->pack_start(*progressbar);
	dialog.set_image(*(Gtk::manage(new Gtk::Image(DATADIR "/pixmaps/grop.png"))));
	dialog.show_all();

	Glib::Dir dir(Opt::logdir());
	int npkgs = std::vector<string>(dir.begin(), dir.end()).size();	
	s_pkgs.reserve(npkgs);
	float cnt = 0;

	for (Glib::DirIterator d = dir.begin(); d != dir.end(); ++d) {

		if (!Glib::file_test(Opt::logdir() + "/" + *d, Glib::FILE_TEST_IS_REGULAR))
			continue;

		dialog.set_secondary_text(*d);
		main_iter();
		
		try 
		{	
			Pkg* pkg = new Pkg(*d);
			pkg->read_log();
			s_pkgs.push_back(pkg);
			s_total_size += pkg->size();
		}
		catch (std::exception const& x) 
		{
			g_warning("%s", x.what()); 
		}
		
		progressbar->set_fraction(cnt++ / npkgs);
		main_iter();
	}

	s_initialized = true;
}


DB::~DB()
{
	for (const_iter p(s_pkgs.begin()); p != s_pkgs.end(); delete *p++) ;
}


void DB::init()
{
	static DB db;
}


void DB::remove_pkg(Pkg* pkg)
{
	g_assert(pkg != 0);

	pkg->unlog();

	for (iter p(s_pkgs.begin()); p != s_pkgs.end(); ++p) {
		if (*p == pkg) {
			s_total_size -= pkg->size();
			s_pkgs.erase(p);
			break;
		}
	}
			
	delete pkg;
}

