/*
    Copyright (C) 2012-2021 Carl Hetherington <cth@carlh.net>

    This file is part of libdcp.

    libdcp is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    libdcp is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libdcp.  If not, see <http://www.gnu.org/licenses/>.

    In addition, as a special exception, the copyright holders give
    permission to link the code of portions of this program with the
    OpenSSL library under certain conditions as described in each
    individual source file, and distribute linked combinations
    including the two.

    You must obey the GNU General Public License in all respects
    for all of the code used other than OpenSSL.  If you modify
    file(s) with this exception, you may extend this exception to your
    version of the file(s), but you are not obligated to do so.  If you
    do not wish to do so, delete this exception statement from your
    version.  If you delete this exception statement from all source
    files in the program, then also delete it here.
*/


/** @file  src/reel_closed_caption_asset.h
 *  @brief ReelClosedCaptionAsset class
 */


#ifndef LIBDCP_REEL_CLOSED_CAPTION_ASSET_H
#define LIBDCP_REEL_CLOSED_CAPTION_ASSET_H


#include "language_tag.h"
#include "reel_asset.h"
#include "reel_file_asset.h"
#include "subtitle_asset.h"


struct verify_invalid_language2;


namespace dcp {


/** @class ReelClosedCaptionAsset
 *  @brief Part of a Reel's description which refers to a closed caption XML/MXF file
 */
class ReelClosedCaptionAsset : public ReelFileAsset
{
public:
	ReelClosedCaptionAsset (std::shared_ptr<SubtitleAsset> asset, Fraction edit_rate, int64_t instrinsic_duration, int64_t entry_point);
	explicit ReelClosedCaptionAsset (std::shared_ptr<const cxml::Node>);

	std::shared_ptr<const SubtitleAsset> asset () const {
		return asset_of_type<const SubtitleAsset>();
	}

	std::shared_ptr<SubtitleAsset> asset () {
		return asset_of_type<SubtitleAsset>();
	}

	bool equals (std::shared_ptr<const ReelClosedCaptionAsset>, EqualityOptions, NoteHandler) const;

	void set_language (dcp::LanguageTag l) {
		_language = l.to_string();
	}

	void unset_language () {
		_language = boost::optional<std::string> ();
	}

	boost::optional<std::string> language () const {
		return _language;
	}

protected:
	friend struct ::verify_invalid_language2;

	boost::optional<std::string> _language;
};


}


#endif
