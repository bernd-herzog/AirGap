using System;
using System.Collections.Generic;
using System.Text;
using AirGap.Queue;

namespace AirGap.Signal
{
    public class FloatToComplex : IModule<float, Complex<float>>
    {
        //siehe http://code.metager.de/source/xref/gnu/gnuradio/gr-analog/lib/frequency_modulator_fc_impl.cc

        public void ConnectInput(SyncronizedQueue<float> queue)
        {
            throw new NotImplementedException();
        }

        public void ConnectOutput(SyncronizedQueue<Complex<float>> queue)
        {
            throw new NotImplementedException();
        }
    }
}
