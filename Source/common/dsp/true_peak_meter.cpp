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


TruePeakMeter::TruePeakMeter(
    const int numberOfChannels,
    const int originalFftBufferSize,
    const int upsamplingFactor) :

    frut::dsp::RateConverter(numberOfChannels,
                             originalFftBufferSize,
                             upsamplingFactor)
{
}


float TruePeakMeter::getLevel(
    const int channel)
{
    jassert(channel >= 0);
    jassert(channel < numberOfChannels_);

    return truePeakLevels_[channel];
}


void TruePeakMeter::copyFromBuffer(
    frut::audio::RingBuffer<float> &ringBuffer,
    const unsigned int preDelay)
{
    // copy data from ring buffer to sample buffer
    ringBuffer.copyToBuffer(sampleBufferOriginal_, 0,
                            originalFftBufferSize_, preDelay);

    // upsample buffer (overwrites contents of sample buffer)
    upsample();

    // evaluate true peak level
    for (int channel = 0; channel < numberOfChannels_; ++channel)
    {
        float truePeakLevel = fftSampleBuffer_.getMagnitude(
                                  channel, 0, fftBufferSize_);

        truePeakLevels_.set(channel, truePeakLevel);
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
