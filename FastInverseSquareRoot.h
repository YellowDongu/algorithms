#pragma once

//퀘이크3에서 사용된 제곱근 공식. 기존 공식보다 훨씬 빠르게 구하면서도 꽤나 답에 근사한 값을 구한다.
// 다만 std::sqrt 함수가 더 정확하다. 정확함을 원한다면 std::sqrt 종류를 사용할 것
// 하드웨어의 발전으로 std::sqrt가 매우 빨라져서 이걸 그냥 써도 되긴 하지만 Tick이나 Update에 사용하기에는 많이 느리므로 이거 참고해라
// 이걸 엔진에서 구현해놓은걸 쓰는게 제일 좋긴 하다. 언리얼엔진은 FMath::InvSqrt와 FGenericPlatformMath::Sqrt로 구현해놨다.
// 유니티는 C#에서 닷넷이 sqrt를 최적화해서 쓰기 때문에 이걸 구현해놓은건 없다. 저기선 이걸 구현할 수도 없는게 C#제약때문에 이걸 만들 수 없다.
float FastInverseSqrt(float x)
{
    if (x <= 0.0f)
        return 0.0f;

    float xhalf = 0.5f * x;
    int i;
    
    // 1. 비트 해킹: float의 비트 구조를 그대로 정수(int) 변수에 복사합니다.
    // 옛날 방식인 *(int*)&x 대신 현대 C++에서는 memcpy나 비트 캐스트를 권장합니다.
    i = *(int*)&x;

    // 2. 마법의 숫자 등장: 지수를 절반으로 줄이는 비트 시프트와 매직 넘버 연산
    // 질문자님이 말씀하신 "지수를 2로 나누는" 과정이 (i >> 1) 부분입니다.
    i = 0x5f3759df - (i >> 1);

    // 3. 다시 정수 비트를 float으로 되돌립니다.
    x = *(float*)&i;

    // 4. 뉴턴-랩슨 보정: 딱 한 번만 수행해서 정밀도를 높입니다.
    // 이 한 줄만으로도 게임용으로는 충분한 99% 이상의 정밀도가 나옵니다.
    x = x * (1.5f - xhalf * x * x);

    return x;
}

float FastInverseSqrt_(float x)
{
    if (x <= 0.0f)
        return 0.0f;

    float xhalf = 0.5f * x;
    int i;
    i = *(int*)&x;
    i = 0x5f3759df - (i >> 1);

    x = *(float*)&i;
    return x * (1.5f - xhalf * x * x);
}
