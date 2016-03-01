/*
    Copyright (C) 2012-2014 Carl Hetherington <cth@carlh.net>

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

#include "smpte_load_font_node.h"
#include "util.h"
#include <libcxml/cxml.h>

using std::string;
using boost::shared_ptr;
using namespace dcp;

SMPTELoadFontNode::SMPTELoadFontNode (string id, string urn_)
	: LoadFontNode (id)
	, urn (urn_)
{

}

SMPTELoadFontNode::SMPTELoadFontNode (shared_ptr<const cxml::Node> node)
	: LoadFontNode (node->string_attribute ("ID"))
	, urn (remove_urn_uuid (node->content()))
{

}

bool
dcp::operator== (SMPTELoadFontNode const & a, SMPTELoadFontNode const & b)
{
	return a.id == b.id && a.urn == b.urn;
}

bool
dcp::operator!= (SMPTELoadFontNode const & a, SMPTELoadFontNode const & b)
{
	return !(a == b);
}