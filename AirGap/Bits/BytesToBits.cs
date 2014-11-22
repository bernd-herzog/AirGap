using System;
using System.Collections.Generic;
using System.Text;
using AirGap.Queue;

namespace AirGap.Bits
{
    public class BytesToBits : IModule<byte, bool>
    {
        public void ConnectInput(SyncronizedQueue<byte> queue)
        {
            throw new NotImplementedException();
        }

        public void ConnectOutput(SyncronizedQueue<bool> queue)
        {
            throw new NotImplementedException();
        }
    }
}
