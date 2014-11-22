using System;
using System.Collections.Generic;
using System.Text;
using AirGap.Queue;

namespace AirGap.Signal
{
    public class Multiply : IModule<Complex<float>, Complex<float>>
    {
        public void ConnectOutput(SyncronizedQueue<Complex<float>> queue)
        {
            throw new NotImplementedException();
        }

        public void ConnectInput(SyncronizedQueue<Complex<float>> queue)
        {
            throw new NotImplementedException();
        }
    }
}
