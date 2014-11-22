using System;
using System.Collections.Generic;
using System.Text;
using AirGap.Queue;

namespace AirGap.Bits
{
    public class BitsToBytes : IModule<bool, byte>
    {
        public void ConnectInput(SyncronizedQueue<bool> queue)
        {
            throw new NotImplementedException();
        }

        public void ConnectOutput(SyncronizedQueue<byte> queue)
        {
            throw new NotImplementedException();
        }
    }
}
