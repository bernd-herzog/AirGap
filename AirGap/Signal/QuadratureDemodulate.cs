using System;
using System.Collections.Generic;
using System.Text;
using AirGap.Queue;

namespace AirGap.Signal
{
    public class QuadratureDemodulate : IModule<Complex<float>, float>
    {
        public void ConnectInput(SyncronizedQueue<Complex<float>> queue)
        {
            throw new NotImplementedException();
        }

        public void ConnectOutput(SyncronizedQueue<float> queue)
        {
            throw new NotImplementedException();
        }
    }
}
