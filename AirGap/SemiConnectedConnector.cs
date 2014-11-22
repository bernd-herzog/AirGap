using System.Runtime.InteropServices.ComTypes;

namespace AirGap
{
    public class SemiConnectedConnector<TIn>
    {
        public ISourceModule<TIn> Source { get; set; }

        public SemiConnectedConnector(ISourceModule<TIn> source)
        {
            Source = source;
        }

        public void To<T>(ISinkModule<T> sink)
        {
            
        }

        public SemiConnectedConnector<TOut> To<TOut>(IModule<TIn, TOut> sink)
        {
            return new SemiConnectedConnector<TOut>(sink);
        }
    }
}