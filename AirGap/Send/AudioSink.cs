using System;
using System.Collections.Generic;
using System.Text;
using AirGap.Queue;

namespace AirGap.Send
{
    public class AudioSink : ISinkModule<float>
    {
        public void ConnectInput(SyncronizedQueue<float> queue)
        {
            throw new NotImplementedException();
        }
    }
}
