using System;
using System.Collections.Generic;
using System.Text;
using AirGap.Queue;

namespace AirGap.Signal
{
    public class ComplexToFloat : IModule<Complex<float>, bool>
    {
        public void ConnectInput(SyncronizedQueue<Complex<float>> queue)
        {
            throw new NotImplementedException();
        }

        public void ConnectOutput(SyncronizedQueue<bool> queue)
        {
            throw new NotImplementedException();
        }
    }
}
