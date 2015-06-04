/*
    Copyright (C) 2012-2015 Carl Hetherington <cth@carlh.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#include "subtitle_asset.h"
#include <boost/filesystem.hpp>

namespace dcp {

class InteropLoadFontNode;

class InteropSubtitleAsset : public SubtitleAsset
{
public:
	InteropSubtitleAsset (std::string movie_title, std::string language);
	InteropSubtitleAsset (boost::filesystem::path file);

	bool equals (
		boost::shared_ptr<const Asset>,
		EqualityOptions,
		NoteHandler note
		) const;

	std::list<boost::shared_ptr<LoadFontNode> > load_font_nodes () const;

	void add_font (std::string id, std::string uri);
	
	Glib::ustring xml_as_string () const;

private:
	std::string _movie_title;
	std::list<boost::shared_ptr<InteropLoadFontNode> > _load_font_nodes;
};

}