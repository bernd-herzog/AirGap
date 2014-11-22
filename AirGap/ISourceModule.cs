using AirGap.Queue;
using System;
using System.Collections.Generic;
using System.Text;

namespace AirGap
{
    public interface ISourceModule<T>
    {
        void ConnectOutput(SyncronizedQueue<T> queue);
    }
}
