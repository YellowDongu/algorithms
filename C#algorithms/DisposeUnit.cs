using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace algorithms
{
    internal class DisposeUnit : IDisposable
    {//C#에서는 void 매개변수가 안되네?
        public DisposeUnit()
        {
            Console.WriteLine("Constructor called");
        }
        ~DisposeUnit() { Release(); }

        public void Dispose()
        {
            if (Release())
                IsDisposed = true;
        }

        public bool Release()
        {
            if (IsDisposed)
                return false;

            // Release unmanaged resources here
            Console.WriteLine("Release method called");
            return true;
        }


        public bool IsDisposed { get; private set; }


        static public void SampleMethod()
        {
            DisposeUnit testUnit = new DisposeUnit();
            testUnit.Dispose();
        }
    }
}
