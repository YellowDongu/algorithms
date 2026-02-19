using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace algorithms
{
    public class Singleton
    {
        private Singleton()
        {
        }

        //default singleton
        private static Singleton? instance = null; // nullable 경고 없애기 위해 ? 붙임
        public static Singleton GetInstance { get { if (instance == null) { instance = new Singleton(); } return instance; } }

        //thread safe singleton(lock method)
        private static readonly object padlock = new object();
        public static Singleton GetInstance_lock { get { lock (padlock) { if (instance == null) { instance = new Singleton(); } return instance; } } }

        //thread safe singleton(Lazy<T> method, only .net 4.0 upper version)
        private static readonly Lazy<Singleton> instance_lazy = new Lazy<Singleton>(() => new Singleton());
        public static Singleton GetInstance_lazy { get { return instance_lazy.Value; } }


    }
}
