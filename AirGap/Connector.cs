namespace AirGap
{
    public class Connector
    {
        public SemiConnectedConnector<T> Connect<T>(ISourceModule<T> source)
        {
            return new SemiConnectedConnector<T>(source);
        }
    }
}