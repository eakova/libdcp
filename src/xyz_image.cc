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

/** @file  src/xyz_image.cc
 *  @brief XYZImage class.
 */

#include "xyz_image.h"
#include "dcp_assert.h"
#include <stdexcept>

using namespace dcp;

/** Construct an XYZImage, taking ownership of the opj_image_t */
XYZImage::XYZImage (opj_image_t* image)
	: _opj_image (image)
{
	DCP_ASSERT (_opj_image->numcomps == 3);
}

/** Construct a new XYZImage with undefined contents.
 *  @param size Size for the frame in pixels.
 */
XYZImage::XYZImage (Size size)
{
	opj_image_cmptparm_t cmptparm[3];
	
	for (int i = 0; i < 3; ++i) {
		cmptparm[i].dx = 1;
		cmptparm[i].dy = 1;
		cmptparm[i].w = size.width;
		cmptparm[i].h = size.height;
		cmptparm[i].x0 = 0;
		cmptparm[i].y0 = 0;
		cmptparm[i].prec = 12;
		cmptparm[i].bpp = 12;
		cmptparm[i].sgnd = 0;
	}

	/* XXX: is this _SRGB right? */
	_opj_image = opj_image_create (3, &cmptparm[0], CLRSPC_SRGB);
	if (_opj_image == 0) {
		throw std::runtime_error ("could not create libopenjpeg image");
	}

	_opj_image->x0 = 0;
	_opj_image->y0 = 0;
	_opj_image->x1 = size.width;
	_opj_image->y1 = size.height;
}

/** XYZImage destructor */
XYZImage::~XYZImage ()
{
	opj_image_destroy (_opj_image);
}

/** @param c Component index (0, 1 or 2)
 *  @return Pointer to the data for component c; 12-bit values from 0-4095.
 */
int *
XYZImage::data (int c) const
{
	DCP_ASSERT (c >= 0 && c < 3);
	return _opj_image->comps[c].data;
}

/** @return Size of the image in pixels */
dcp::Size
XYZImage::size () const
{
	/* XXX: this may not be right; x0 and y0 can presumably be non-zero */
	return dcp::Size (_opj_image->x1, _opj_image->y1);
}