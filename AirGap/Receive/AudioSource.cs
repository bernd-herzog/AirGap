using System;
using System.Collections.Generic;
using System.Text;
using AirGap.Queue;

namespace AirGap.Receive
{
    public class AudioSource: ISourceModule<float>
    {
        public void ConnectOutput(SyncronizedQueue<float> queue)
        {
            throw new NotImplementedException();
        }
    }
}
