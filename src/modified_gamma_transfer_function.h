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


/** @file  src/modified_gamma_transfer_function.h
 *  @brief ModifiedGammaTransferFunction class
 */


#include "transfer_function.h"


namespace dcp {


/** A transfer function which for an input x gives a linear output y where
 *
 *  y = x / B                      for x <= threshold
 *  y = ((x + A) / (1 + A))^power  for x >  threshold
 *
 *  The reverse transform is
 *
 *  x = y * B                          for y <= threshold / B
 *  x = (1 + A) * y ^ (1 / power) - A  for y >  threshold / B
 */
class ModifiedGammaTransferFunction : public TransferFunction
{
public:
	ModifiedGammaTransferFunction (double power, double threshold, double A, double B);

	double power () const {
		return _power;
	}

	double threshold () const {
		return _threshold;
	}

	double A () const {
		return _A;
	}

	double B () const {
		return _B;
	}

	bool about_equal (std::shared_ptr<const TransferFunction>, double epsilon) const override;

protected:
	double * make_lut (int bit_depth, bool inverse) const override;

private:
	double _power;
	double _threshold;
	double _A;
	double _B;
};


}
