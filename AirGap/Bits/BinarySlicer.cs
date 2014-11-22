using System;
using System.Collections.Generic;
using System.Text;
using AirGap.Queue;

namespace AirGap.Bits
{
    public class BinarySlicer : IModule<float, bool>
    {
        public void ConnectInput(SyncronizedQueue<float> queue)
        {
            throw new NotImplementedException();
        }

        public void ConnectOutput(SyncronizedQueue<bool> queue)
        {
            throw new NotImplementedException();
        }
    }
}
