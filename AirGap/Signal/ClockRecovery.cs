using System;
using System.Collections.Generic;
using System.Text;
using AirGap.Queue;

namespace AirGap.Signal
{
    public class ClockRecovery : IModule<float, float>
    {
        public void ConnectOutput(SyncronizedQueue<float> queue)
        {
            throw new NotImplementedException();
        }

        public void ConnectInput(SyncronizedQueue<float> queue)
        {
            throw new NotImplementedException();
        }
    }
}
