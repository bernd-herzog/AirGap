﻿using System;
using System.Collections.Generic;
using System.Text;

namespace AirGap
{
    interface IModule<TIn, TOut> : ISinkModule<TIn>, ISourceModule<TOut>
    {
    }
}
