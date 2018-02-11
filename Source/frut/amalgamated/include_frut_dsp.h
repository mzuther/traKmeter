/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2018 Martin Zuther (http://www.mzuther.de/)

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   Thank you for using free software!

---------------------------------------------------------------------------- */

#pragma once


#ifndef FRUT_DSP_USE_FFTW
#define FRUT_DSP_USE_FFTW 0
#endif


namespace frut
{
namespace dsp
{

// special includes
#include <float.h>
#include <math.h>

#if FRUT_DSP_USE_FFTW
#include "fftw/api/fftw3.h"
#endif

// normal includes
#include "../dsp/biquad_filter.h"
#include "../dsp/dither.h"
#include "../dsp/filter_chebyshev_stage.h"
#include "../dsp/iir_filter_box.h"

#if FRUT_DSP_USE_FFTW

#include "../dsp/fftw_runner.h"
#include "../dsp/fir_filter_box.h"
#include "../dsp/rate_converter.h"
#include "../dsp/true_peak_meter.h"

#endif

// post includes
#include "../dsp/filter_chebyshev.h"

}
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
