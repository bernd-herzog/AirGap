using AirGap.Queue;

namespace AirGap.Signal
{
    public class FirFilter :IModule<Complex<float>, Complex<float>>
    {
        public void ConnectInput(SyncronizedQueue<Complex<float>> queue)
        {
            throw new System.NotImplementedException();
        }

        public void ConnectOutput(SyncronizedQueue<Complex<float>> queue)
        {
            throw new System.NotImplementedException();
        }
    }
}