/*
    Copyright (C) 2012 Carl Hetherington <cth@carlh.net>

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

/** @file  src/asset.cc
 *  @brief Parent class for assets of DCPs.
 */

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include "AS_DCP.h"
#include "KM_util.h"
#include "asset.h"
#include "util.h"
#include "metadata.h"

using namespace std;
using namespace boost;
using namespace libdcp;

Asset::Asset (string directory, string mxf_name, sigc::signal1<void, float>* progress, int fps, int length)
	: _directory (directory)
	, _mxf_name (mxf_name)
	, _progress (progress)
	, _fps (fps)
	, _length (length)
	, _uuid (make_uuid ())
{
	
}

void
Asset::write_to_pkl (ostream& s) const
{
	s << "    <Asset>\n"
	  << "      <Id>urn:uuid:" << _uuid << "</Id>\n"
	  << "      <AnnotationText>" << _mxf_name << "</AnnotationText>\n"
	  << "      <Hash>" << _digest << "</Hash>\n"
	  << "      <Size>" << filesystem::file_size(mxf_path()) << "</Size>\n"
	  << "      <Type>application/mxf</Type>\n"
	  << "    </Asset>\n";
}

void
Asset::write_to_assetmap (ostream& s) const
{
	s << "    <Asset>\n"
	  << "      <Id>urn:uuid:" << _uuid << "</Id>\n"
	  << "      <ChunkList>\n"
	  << "        <Chunk>\n"
	  << "          <Path>" << _mxf_name << "</Path>\n"
	  << "          <VolumeIndex>1</VolumeIndex>\n"
	  << "          <Offset>0</Offset>\n"
	  << "          <Length>" << filesystem::file_size(mxf_path()) << "</Length>\n"
	  << "        </Chunk>\n"
	  << "      </ChunkList>\n"
	  << "    </Asset>\n";
}

void
Asset::fill_writer_info (ASDCP::WriterInfo* writer_info) const
{
	writer_info->ProductVersion = Metadata::instance()->product_version;
	writer_info->CompanyName = Metadata::instance()->company_name;
	writer_info->ProductName = Metadata::instance()->product_name.c_str();

	writer_info->LabelSetType = ASDCP::LS_MXF_SMPTE;
	unsigned int c;
	Kumu::hex2bin (_uuid.c_str(), writer_info->AssetUUID, Kumu::UUID_Length, &c);
	assert (c == Kumu::UUID_Length);
}

filesystem::path
Asset::mxf_path () const
{
	filesystem::path p;
	p /= _directory;
	p /= _mxf_name;
	return p;
}

list<string>
Asset::equals (Asset const & other, EqualityFlags flags) const
{
	list<string> notes;
	
	switch (flags) {
	case LIBDCP_METADATA:
		break;
	case MXF_BITWISE:
		if (filesystem::file_size (mxf_path()) != filesystem::file_size (other.mxf_path())) {
			notes.push_back (mxf_path().string() + " and " + other.mxf_path().string() + " sizes differ");
			return notes;
		}
		
		ifstream a (mxf_path().c_str(), ios::binary);
		ifstream b (other.mxf_path().c_str(), ios::binary);

		int buffer_size = 65536;
		char abuffer[buffer_size];
		char bbuffer[buffer_size];

		int n = filesystem::file_size (mxf_path ());

		while (n) {
			int const t = min (n, buffer_size);
			a.read (abuffer, t);
			b.read (bbuffer, t);

			for (int i = 0; i < t; ++i) {
				if (abuffer[i] != bbuffer[i]) {
					notes.push_back (mxf_path().string() + " and " + other.mxf_path().string() + " content differs");
					return notes;
				}
			}

			n -= t;
		}
	}

	return notes;
}
