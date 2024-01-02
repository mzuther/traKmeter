# traKmeter

*Loudness meter for correctly setting up tracking and mixing levels
(gain staging).*

![Screenshot](./doc/include/images/trakmeter.png)

## About

Sadly, most digital audio equipment only has peak meters.  This is
readily understandable as you want to avoid digital clippings by all
means.  However, badly chosen meter ranges and scales often render
these meters useless.  And the lack of average meters does not exactly
facilitate gain staging.

When I had realised this, I started coding traKmeter.  It has evolved
with my growing knowledge and recording experience, but the underlying
ideas haven't changed at all.

*Please read the [manual][] for a more extensive introduction.*

## Download

traKmeter can be downloaded in the [releases][] section (you may need
to click on "Assets").

I have dropped LV2 plug-in support for good.  However, you can still
use LV2 - just download version **v2.4.6**.

## Documentation

For documentation, licenses and further information, please see the
[manual][] and the directory [doc][].

## FAQ

### traKmeter crashes when the plug-in is loaded / opened

1. traKmeter requires a processor which supports the SSE2 instruction
   set (if you run at Windows 8 and above, it does).  On Windows, you
   might also have to install the [Visual C++ Redistributable for
   Visual Studio 2017][VC++ Redist].

2. traKmeter comes with a folder called `trakmeter`.  This folder must be
   located in the same folder as the plug-in, so please copy it along
   and try again!

### Where are the LV2 plug-ins?

- The last release with LV2 plug-ins was version **v2.4.6**.  You can
  still download them from the "Releases" page.

### The stand-alone version does not work

- Unfortunately, I cannot do anything about it as I did not code this
  part of the application.  The stand-alone works well for me - try
  using another sound card or deleting the settings file.

## Code of conduct

Please read the [code of conduct][COC] before asking for help, filing
bug reports or contributing to this project.  Thanks!

## License

Copyright (c) 2012-2022 Martin Zuther

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

## VST2 plug-ins

Technically, my VST2 plug-ins are not free software.  However, I
chose to provide them for your convenience.  All other binaries really
are free software in the sense of the Free Software Foundation.

*VST is a trademark of Steinberg Media Technologies GmbH, registered
in Europe and other countries.*


[COC]:            https://github.com/mzuther/traKmeter/tree/master/CODE_OF_CONDUCT.markdown
[doc]:            https://github.com/mzuther/traKmeter/tree/master/doc/
[manual]:         https://github.com/mzuther/traKmeter/raw/master/doc/trakmeter.pdf
[releases]:       https://github.com/mzuther/traKmeter/releases

[VC++ Redist]:    https://www.visualstudio.com/downloads/
