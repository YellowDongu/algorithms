using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace algorithms
{
    internal class Base
    {
    }
    interface testInterface  // 인터페이스가 가능한 범위
    {
        public void VirtualTestMethod(int one); // => 자동 상속화, 몸체 없음
        public delegate void DelegateTest(int value); // delegate
        public event DelegateTest eventTest; // event
        public int this[int index] // indexer
        { get { return indexerBody[index]; } set { indexerBody[index] = value; } }
        List<int> indexerBody { get; set; } // for indexer
        int getterSetterTest { get; set; } // property with no body(only getset)
        //int valueTest => X
        // 아래는 Dotnet 8.0 이상
        static int TestMethod(int one) { return one; }// static method
        static int test = 2; // static property
        const int constTest = 1; // const
    }
}
