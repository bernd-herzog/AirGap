using AirGap.Queue;
using System;
using System.Collections.Generic;
using System.Text;

namespace AirGap
{
    public interface ISinkModule<T>
    {
        void ConnectInput(SyncronizedQueue<T> queue);
    }
}
