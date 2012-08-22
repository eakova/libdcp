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

#include <cmath>
#include <boost/filesystem.hpp>
#include "KM_prng.h"
#include "dcp.h"
#include "util.h"
#include "metadata.h"
#include "types.h"
#include "exceptions.h"
#include "subtitle_asset.h"
#include "picture_asset.h"
#include "sound_asset.h"
#include "reel.h"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE libdcp_test
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace boost;

string
j2c (int)
{
	return "test/data/32x32_red_square.j2c";
}

string
wav (libdcp::Channel)
{
	return "test/data/1s_24-bit_48k_silence.wav";
}
		

BOOST_AUTO_TEST_CASE (dcp_test)
{
	Kumu::libdcp_test = true;
	
	libdcp::Metadata* t = libdcp::Metadata::instance ();
	t->issuer = "OpenDCP 0.0.25";
	t->creator = "OpenDCP 0.0.25";
	t->company_name = "OpenDCP";
	t->product_name = "OpenDCP";
	t->product_version = "0.0.25";
	t->issue_date = "2012-07-17T04:45:18+00:00";
	filesystem::remove_all ("build/test/foo");
	filesystem::create_directories ("build/test/foo");
	libdcp::DCP d ("build/test/foo", "A Test DCP", libdcp::FEATURE, 24, 24);

	shared_ptr<libdcp::MonoPictureAsset> mp (new libdcp::MonoPictureAsset (
							 sigc::ptr_fun (&j2c),
							 "build/test/foo",
							 "video.mxf",
							 &d.Progress,
							 24,
							 24,
							 32,
							 32
							 ));

	shared_ptr<libdcp::SoundAsset> ms (new libdcp::SoundAsset (
						  sigc::ptr_fun (&wav),
						  "build/test/foo",
						  "audio.mxf",
						  &(d.Progress),
						  24,
						  24,
						  2
						  ));

	d.add_reel (shared_ptr<libdcp::Reel> (new libdcp::Reel (mp, ms, shared_ptr<libdcp::SubtitleAsset> ())));

	d.write_xml ();
}

BOOST_AUTO_TEST_CASE (error_test)
{
	libdcp::DCP d ("build/test/bar", "A Test DCP", libdcp::TEST, 24, 24);
	vector<string> p;
	p.push_back ("frobozz");

	BOOST_CHECK_THROW (new libdcp::MonoPictureAsset (p, "build/test/bar", "video.mxf", &d.Progress, 24, 24, 32, 32), libdcp::FileError);
	BOOST_CHECK_THROW (new libdcp::SoundAsset (p, "build/test/bar", "audio.mxf", &d.Progress, 24, 24), libdcp::FileError);
}

BOOST_AUTO_TEST_CASE (read_dcp)
{
	libdcp::DCP d ("test/ref/DCP");

	BOOST_CHECK_EQUAL (d.name(), "A Test DCP");
	BOOST_CHECK_EQUAL (d.content_kind(), libdcp::FEATURE);
	BOOST_CHECK_EQUAL (d.frames_per_second(), 24);
	BOOST_CHECK_EQUAL (d.length(), 24);
}
	
BOOST_AUTO_TEST_CASE (subtitles1)
{
	libdcp::SubtitleAsset subs ("test/data", "subs1.xml");

	BOOST_CHECK_EQUAL (subs.language(), "French");

	list<shared_ptr<libdcp::Subtitle> > s = subs.subtitles_at (libdcp::Time (0, 0, 6, 1));
	BOOST_CHECK_EQUAL (s.size(), 1);
	BOOST_CHECK_EQUAL (*(s.front().get()), libdcp::Subtitle (
				   "Arial",
				   false,
				   libdcp::Color (255, 255, 255),
				   39,
				   libdcp::Time (0, 0, 5, 198),
				   libdcp::Time (0, 0, 7, 115),
				   15,
				   libdcp::BOTTOM,
				   "My jacket was Idi Amin's",
				   libdcp::BORDER,
				   libdcp::Color (0, 0, 0),
				   libdcp::Time (0, 0, 0, 1),
				   libdcp::Time (0, 0, 0, 1)
				   ));
							 
	s = subs.subtitles_at (libdcp::Time (0, 0, 7, 190));
	BOOST_CHECK_EQUAL (s.size(), 2);
	BOOST_CHECK_EQUAL (*(s.front().get()), libdcp::Subtitle (
				   "Arial",
				   true,
				   libdcp::Color (255, 255, 255),
				   39,
				   libdcp::Time (0, 0, 7, 177),
				   libdcp::Time (0, 0, 11, 31),
				   21,
				   libdcp::BOTTOM,
				   "My corset was H.M. The Queen's",
				   libdcp::BORDER,
				   libdcp::Color (0, 0, 0),
				   libdcp::Time (0, 0, 0, 1),
				   libdcp::Time (0, 0, 0, 1)
				   ));
	BOOST_CHECK_EQUAL (*(s.back().get()), libdcp::Subtitle (
				   "Arial",
				   false,
				   libdcp::Color (255, 255, 255),
				   39,
				   libdcp::Time (0, 0, 7, 177),
				   libdcp::Time (0, 0, 11, 31),
				   15,
				   libdcp::BOTTOM,
				   "My large wonderbra",
				   libdcp::BORDER,
				   libdcp::Color (0, 0, 0),
				   libdcp::Time (0, 0, 0, 1),
				   libdcp::Time (0, 0, 0, 1)
				   ));

	s = subs.subtitles_at (libdcp::Time (0, 0, 11, 95));
	BOOST_CHECK_EQUAL (s.size(), 1);
	BOOST_CHECK_EQUAL (*(s.back().get()), libdcp::Subtitle (
				   "Arial",
				   false,
				   libdcp::Color (255, 255, 255),
				   39,
				   libdcp::Time (0, 0, 11, 94),
				   libdcp::Time (0, 0, 13, 63),
				   15,
				   libdcp::BOTTOM,
				   "Once belonged to the Shah",
				   libdcp::BORDER,
				   libdcp::Color (0, 0, 0),
				   libdcp::Time (0, 0, 0, 1),
				   libdcp::Time (0, 0, 0, 1)
				   ));

	s = subs.subtitles_at (libdcp::Time (0, 0, 14, 42));
	BOOST_CHECK_EQUAL (s.size(), 1);
	BOOST_CHECK_EQUAL (*(s.back().get()), libdcp::Subtitle (
				   "Arial",
				   false,
				   libdcp::Color (255, 255, 255),
				   39,
				   libdcp::Time (0, 0, 13, 104),
				   libdcp::Time (0, 0, 15, 177),
				   15,
				   libdcp::BOTTOM,
				   "And these are Roy Hattersley's jeans",
				   libdcp::BORDER,
				   libdcp::Color (0, 0, 0),
				   libdcp::Time (0, 0, 0, 1),
				   libdcp::Time (0, 0, 0, 1)
				   ));
}

BOOST_AUTO_TEST_CASE (subtitles2)
{
	libdcp::SubtitleAsset subs ("test/data", "subs2.xml");

	list<shared_ptr<libdcp::Subtitle> > s = subs.subtitles_at (libdcp::Time (0, 0, 42, 100));
	BOOST_CHECK_EQUAL (s.size(), 2);
	BOOST_CHECK_EQUAL (*(s.front().get()), libdcp::Subtitle (
				   "Arial",
				   true,
				   libdcp::Color (255, 255, 255),
				   42,
				   libdcp::Time (0, 0, 41, 62),
				   libdcp::Time (0, 0, 43, 52),
				   89,
				   libdcp::TOP,
				   "At afternoon tea with John Peel",
				   libdcp::BORDER,
				   libdcp::Color (0, 0, 0),
				   libdcp::Time (0, 0, 0, 0),
				   libdcp::Time (0, 0, 0, 0)
				   ));
	BOOST_CHECK_EQUAL (*(s.back().get()), libdcp::Subtitle (
				   "Arial",
				   true,
				   libdcp::Color (255, 255, 255),
				   42,
				   libdcp::Time (0, 0, 41, 62),
				   libdcp::Time (0, 0, 43, 52),
				   95,
				   libdcp::TOP,
				   "I enquired if his accent was real",
				   libdcp::BORDER,
				   libdcp::Color (0, 0, 0),
				   libdcp::Time (0, 0, 0, 0),
				   libdcp::Time (0, 0, 0, 0)
				   ));

	s = subs.subtitles_at (libdcp::Time (0, 0, 50, 50));
	BOOST_CHECK_EQUAL (s.size(), 2);
	BOOST_CHECK_EQUAL (*(s.front().get()), libdcp::Subtitle (
				   "Arial",
				   true,
				   libdcp::Color (255, 255, 255),
				   42,
				   libdcp::Time (0, 0, 50, 42),
				   libdcp::Time (0, 0, 52, 21),
				   89,
				   libdcp::TOP,
				   "He said \"out of the house",
				   libdcp::BORDER,
				   libdcp::Color (0, 0, 0),
				   libdcp::Time (0, 0, 0, 0),
				   libdcp::Time (0, 0, 0, 0)
				   ));
	BOOST_CHECK_EQUAL (*(s.back().get()), libdcp::Subtitle (
				   "Arial",
				   true,
				   libdcp::Color (255, 255, 255),
				   42,
				   libdcp::Time (0, 0, 50, 42),
				   libdcp::Time (0, 0, 52, 21),
				   95,
				   libdcp::TOP,
				   "I'm incredibly scouse",
				   libdcp::BORDER,
				   libdcp::Color (0, 0, 0),
				   libdcp::Time (0, 0, 0, 0),
				   libdcp::Time (0, 0, 0, 0)
				   ));

	s = subs.subtitles_at (libdcp::Time (0, 1, 2, 300));
	BOOST_CHECK_EQUAL (s.size(), 2);
	BOOST_CHECK_EQUAL (*(s.front().get()), libdcp::Subtitle (
				   "Arial",
				   true,
				   libdcp::Color (255, 255, 255),
				   42,
				   libdcp::Time (0, 1, 2, 208),
				   libdcp::Time (0, 1, 4, 10),
				   89,
				   libdcp::TOP,
				   "At home it depends how I feel.\"",
				   libdcp::BORDER,
				   libdcp::Color (0, 0, 0),
				   libdcp::Time (0, 0, 0, 0),
				   libdcp::Time (0, 0, 0, 0)
				   ));
	BOOST_CHECK_EQUAL (*(s.back().get()), libdcp::Subtitle (
				   "Arial",
				   true,
				   libdcp::Color (255, 255, 255),
				   42,
				   libdcp::Time (0, 1, 2, 208),
				   libdcp::Time (0, 1, 4, 10),
				   95,
				   libdcp::TOP,
				   "I spent a long weekend in Brighton",
				   libdcp::BORDER,
				   libdcp::Color (0, 0, 0),
				   libdcp::Time (0, 0, 0, 0),
				   libdcp::Time (0, 0, 0, 0)
				   ));

	s = subs.subtitles_at (libdcp::Time (0, 1, 15, 50));
	BOOST_CHECK_EQUAL (s.size(), 2);
	BOOST_CHECK_EQUAL (*(s.front().get()), libdcp::Subtitle (
				   "Arial",
				   true,
				   libdcp::Color (255, 255, 255),
				   42,
				   libdcp::Time (0, 1, 15, 42),
				   libdcp::Time (0, 1, 16, 42),
				   89,
				   libdcp::TOP,
				   "With the legendary Miss Enid Blyton",
				   libdcp::BORDER,
				   libdcp::Color (0, 0, 0),
				   libdcp::Time (0, 0, 0, 0),
				   libdcp::Time (0, 0, 0, 0)
				   ));
	BOOST_CHECK_EQUAL (*(s.back().get()), libdcp::Subtitle (
				   "Arial",
				   true,
				   libdcp::Color (255, 255, 255),
				   42,
				   libdcp::Time (0, 1, 15, 42),
				   libdcp::Time (0, 1, 16, 42),
				   95,
				   libdcp::TOP,
				   "She said \"you be Noddy",
				   libdcp::BORDER,
				   libdcp::Color (0, 0, 0),
				   libdcp::Time (0, 0, 0, 0),
				   libdcp::Time (0, 0, 0, 0)
				   ));

	s = subs.subtitles_at (libdcp::Time (0, 1, 27, 200));
	BOOST_CHECK_EQUAL (s.size(), 2);
	BOOST_CHECK_EQUAL (*(s.front().get()), libdcp::Subtitle (
				   "Arial",
				   true,
				   libdcp::Color (255, 255, 255),
				   42,
				   libdcp::Time (0, 1, 27, 115),
				   libdcp::Time (0, 1, 28, 208),
				   89,
				   libdcp::TOP,
				   "That curious creature the Sphinx",
				   libdcp::BORDER,
				   libdcp::Color (0, 0, 0),
				   libdcp::Time (0, 0, 0, 0),
				   libdcp::Time (0, 0, 0, 0)
				   ));
	BOOST_CHECK_EQUAL (*(s.back().get()), libdcp::Subtitle (
				   "Arial",
				   true,
				   libdcp::Color (255, 255, 255),
				   42,
				   libdcp::Time (0, 1, 27, 115),
				   libdcp::Time (0, 1, 28, 208),
				   95,
				   libdcp::TOP,
				   "Is smarter than anyone thinks",
				   libdcp::BORDER,
				   libdcp::Color (0, 0, 0),
				   libdcp::Time (0, 0, 0, 0),
				   libdcp::Time (0, 0, 0, 0)
				   ));

	s = subs.subtitles_at (libdcp::Time (0, 1, 42, 300));
	BOOST_CHECK_EQUAL (s.size(), 2);
	BOOST_CHECK_EQUAL (*(s.front().get()), libdcp::Subtitle (
				   "Arial",
				   false,
				   libdcp::Color (255, 255, 255),
				   42,
				   libdcp::Time (0, 1, 42, 229),
				   libdcp::Time (0, 1, 45, 62),
				   89,
				   libdcp::TOP,
				   "It sits there and smirks",
				   libdcp::BORDER,
				   libdcp::Color (0, 0, 0),
				   libdcp::Time (0, 0, 0, 0),
				   libdcp::Time (0, 0, 0, 0)
				   ));
	BOOST_CHECK_EQUAL (*(s.back().get()), libdcp::Subtitle (
				   "Arial",
				   false,
				   libdcp::Color (255, 255, 255),
				   42,
				   libdcp::Time (0, 1, 42, 229),
				   libdcp::Time (0, 1, 45, 62),
				   95,
				   libdcp::TOP,
				   "And you don't think it works",
				   libdcp::BORDER,
				   libdcp::Color (0, 0, 0),
				   libdcp::Time (0, 0, 0, 0),
				   libdcp::Time (0, 0, 0, 0)
				   ));

	s = subs.subtitles_at (libdcp::Time (0, 1, 45, 200));
	BOOST_CHECK_EQUAL (s.size(), 2);
	BOOST_CHECK_EQUAL (*(s.front().get()), libdcp::Subtitle (
				   "Arial",
				   false,
				   libdcp::Color (255, 255, 255),
				   42,
				   libdcp::Time (0, 1, 45, 146),
				   libdcp::Time (0, 1, 47, 94),
				   89,
				   libdcp::TOP,
				   "Then when you're not looking, it winks.",
				   libdcp::BORDER,
				   libdcp::Color (0, 0, 0),
				   libdcp::Time (0, 0, 0, 0),
				   libdcp::Time (0, 0, 0, 0)
				   ));
	BOOST_CHECK_EQUAL (*(s.back().get()), libdcp::Subtitle (
				   "Arial",
				   false,
				   libdcp::Color (255, 255, 255),
				   42,
				   libdcp::Time (0, 1, 45, 146),
				   libdcp::Time (0, 1, 47, 94),
				   95,
				   libdcp::TOP,
				   "When it snows you will find Sister Sledge",
				   libdcp::BORDER,
				   libdcp::Color (0, 0, 0),
				   libdcp::Time (0, 0, 0, 0),
				   libdcp::Time (0, 0, 0, 0)
				   ));

	s = subs.subtitles_at (libdcp::Time (0, 1, 47, 249));
	BOOST_CHECK_EQUAL (s.size(), 2);
	BOOST_CHECK_EQUAL (*(s.front().get()), libdcp::Subtitle (
				   "Arial",
				   false,
				   libdcp::Color (255, 255, 255),
				   42,
				   libdcp::Time (0, 1, 47, 146),
				   libdcp::Time (0, 1, 48, 167),
				   89,
				   libdcp::TOP,
				   "Out mooning, at night, on the ledge",
				   libdcp::BORDER,
				   libdcp::Color (0, 0, 0),
				   libdcp::Time (0, 0, 0, 0),
				   libdcp::Time (0, 0, 0, 0)
				   ));
	BOOST_CHECK_EQUAL (*(s.back().get()), libdcp::Subtitle (
				   "Arial",
				   false,
				   libdcp::Color (255, 255, 255),
				   42,
				   libdcp::Time (0, 1, 47, 146),
				   libdcp::Time (0, 1, 48, 167),
				   95,
				   libdcp::TOP,
				   "One storey down",
				   libdcp::BORDER,
				   libdcp::Color (0, 0, 0),
				   libdcp::Time (0, 0, 0, 0),
				   libdcp::Time (0, 0, 0, 0)
				   ));

	s = subs.subtitles_at (libdcp::Time (0, 2, 6, 210));
	BOOST_CHECK_EQUAL (s.size(), 2);
	BOOST_CHECK_EQUAL (*(s.front().get()), libdcp::Subtitle (
				   "Arial",
				   true,
				   libdcp::Color (255, 255, 255),
				   42,
				   libdcp::Time (0, 2, 5, 208),
				   libdcp::Time (0, 2, 7, 31),
				   89,
				   libdcp::TOP,
				   "HELLO",
				   libdcp::BORDER,
				   libdcp::Color (0, 0, 0),
				   libdcp::Time (0, 0, 0, 0),
				   libdcp::Time (0, 0, 0, 0)
				   ));
	BOOST_CHECK_EQUAL (*(s.back().get()), libdcp::Subtitle (
				   "Arial",
				   true,
				   libdcp::Color (255, 255, 255),
				   42,
				   libdcp::Time (0, 2, 5, 208),
				   libdcp::Time (0, 2, 7, 31),
				   95,
				   libdcp::TOP,
				   "WORLD",
				   libdcp::BORDER,
				   libdcp::Color (0, 0, 0),
				   libdcp::Time (0, 0, 0, 0),
				   libdcp::Time (0, 0, 0, 0)
				   ));

	
	
}

BOOST_AUTO_TEST_CASE (dcp_time)
{
	libdcp::Time t (977143, 24);

	BOOST_CHECK_EQUAL (t.t, 73);
	BOOST_CHECK_EQUAL (t.s, 34);
	BOOST_CHECK_EQUAL (t.m, 18);
	BOOST_CHECK_EQUAL (t.h, 11);

	libdcp::Time a (3, 2, 3, 4);
	libdcp::Time b (2, 3, 4, 5);

	libdcp::Time r = a - b;
	BOOST_CHECK_EQUAL (r.h, 0);
	BOOST_CHECK_EQUAL (r.m, 58);
	BOOST_CHECK_EQUAL (r.s, 58);
	BOOST_CHECK_EQUAL (r.t, 249);

	a = libdcp::Time (1, 58, 56, 240);
	b = libdcp::Time (1, 7, 12, 120);
	r = a + b;
	BOOST_CHECK_EQUAL (r.h, 3);
	BOOST_CHECK_EQUAL (r.m, 6);
	BOOST_CHECK_EQUAL (r.s, 9);
	BOOST_CHECK_EQUAL (r.t, 110);

	a = libdcp::Time (24, 12, 6, 3);
	b = libdcp::Time (16, 8, 4, 2);
	BOOST_CHECK_CLOSE (a / b, 1.5, 1e-5);
}

BOOST_AUTO_TEST_CASE (color)
{
	libdcp::Color c ("FFFF0000");

	BOOST_CHECK_EQUAL (c.r, 255);
	BOOST_CHECK_EQUAL (c.g, 0);
	BOOST_CHECK_EQUAL (c.b, 0);

	c = libdcp::Color ("FF00FF00");

	BOOST_CHECK_EQUAL (c.r, 0);
	BOOST_CHECK_EQUAL (c.g, 255);
	BOOST_CHECK_EQUAL (c.b, 0);

	c = libdcp::Color ("FF0000FF");

	BOOST_CHECK_EQUAL (c.r, 0);
	BOOST_CHECK_EQUAL (c.g, 0);
	BOOST_CHECK_EQUAL (c.b, 255);
}
