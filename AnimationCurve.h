#pragma once
#include <cmath>
#include "Vector3.h"

#define PI 3.141592f
#define XM_PI 3.14159265357989f
namespace std // stl clamp 임시 대체
{
    template<typename T>
    static const T& clamp(const T& v, const T& lo, const T& hi)
    {
        return (v < lo) ? lo : (hi < v) ? hi : v;
    }
}
struct AnimationCurve
{
    enum class EaseType
    {
        easeLinear,
        easeInSine, easeOutSine, easeInOutSine,
        easeInQuad, easeOutQuad, easeInOutQuad,
        easeInCubic, easeOutCubic, easeInOutCubic,
        easeInQuart, easeOutQuart, easeInOutQuart,
        easeInQuint, easeOutQuint, easeInOutQuint,
        easeInCirc, easeOutCirc, easeInOutCirc,
        easeInExpo, easeOutExpo, easeInOutExpo,
        easeInBack, easeOutBack, easeInOutBack,
        easeInElastic, easeOutElastic, easeInOutElastic,
        easeInBounce, easeOutBounce, easeInOutBounce,
        easeStep,
        easeEnd
    };

    //static _matrix MatrixLerpWithCurve(const _matrix& A, const _matrix& B, float t, float(*easeFunc)(float))
    //{
    //    // t를 이징 처리
    //    float easedT = easeFunc(t);
    //
    //    // 분리
    //    _vector scaleA, rotQuatA, transA;
    //    XMMatrixDecompose(&scaleA, &rotQuatA, &transA, A);
    //
    //    _vector scaleB, rotQuatB, transB;
    //    XMMatrixDecompose(&scaleB, &rotQuatB, &transB, B);
    //
    //    // 각각 보간
    //    _vector scale = XMVectorLerp(scaleA, scaleB, easedT);
    //    _vector trans = XMVectorLerp(transA, transB, easedT);
    //    _vector rot = XMQuaternionSlerp(rotQuatA, rotQuatB, easedT);
    //
    //    // 재합성
    //    return XMMatrixAffineTransformation(scale, XMVectorZero(), rot, trans);
    //}
    //
    //// 색상 보간
    //static float4 LerpColorHSL(const float4& A, const float4& B, float t)
    //{
    //    _vector hslA = XMColorRGBToHSL(XMLoadFloat4(&A));
    //    _vector hslB = XMColorRGBToHSL(XMLoadFloat4(&B));
    //    float hA = XMVectorGetX(hslA), sA = XMVectorGetY(hslA), lA = XMVectorGetZ(hslA);
    //    float hB = XMVectorGetX(hslB), sB = XMVectorGetY(hslB), lB = XMVectorGetZ(hslB);
    //
    //    // Hue 차이를 원형(0~2π)에서 최단 경로로 보정
    //    float dh = hB - hA;
    //    if (dh > XM_PI) dh -= XM_2PI;
    //    else if (dh < -XM_PI) dh += XM_2PI;
    //    float h = hA + dh * t;
    //    if (h < 0)      h += XM_2PI;
    //    else if (h > XM_2PI) h -= XM_2PI;
    //
    //    // S, L은 선형 보간
    //    float s = sA + (sB - sA) * t;
    //    float l = lA + (lB - lA) * t;
    //
    //    // HSL → RGB
    //    _vector hsl = XMVectorSet(h, s, l, 0.f);
    //    _vector rgbV = XMColorHSLToRGB(hsl);
    //
    //    float4 out;
    //    XMStoreFloat4(&out, rgbV);
    //    return out;
    //}
    //
    //static float Saturate(float x) { return x < 0.f ? 0.f : (x > 1.f ? 1.f : x); }
    //
    //inline static float4 Lerp(float4 a, float4 b, float t)
    //{
    //    using namespace DirectX;
    //    _vector va = XMLoadFloat4(&a);
    //    _vector vb = XMLoadFloat4(&b);
    //    _vector vr = XMVectorLerp(va, vb, t);
    //    float4 result;
    //    XMStoreFloat4(&result, vr);
    //    return result;
    //}
    //
    //// float3 특수 오버로드
    //static float3 Lerp(Engine::float3 a, Engine::float3 b, float t)
    //{
    //    return
    //    {
    //        a.x + t * (b.x - a.x),
    //        a.y + t * (b.y - a.y),
    //        a.z + t * (b.z - a.z)
    //    };
    //}
    //
    //static float2 Lerp(const float2& a, const float2& b, float t)
    //{
    //    return {
    //        a.x + (b.x - a.x) * t,
    //        a.y + (b.y - a.y) * t
    //    };
    //}
    //
    //
    //static _vector vecLerp(_vector a, _vector b, float t)
    //{
    //    return XMVectorLerp(a, b, t); // XMVECTOR용 특수화
    //}

    template<typename T>
    static T Lerp(T a, T b, float t)
    {
        return a + t * (b - a);
    }

    template<typename T>
    static const T& Clamp(const T& v, const T& lo, const T& hi)
    {
        return (v < lo) ? lo : (hi < v) ? hi : v;
    }

    static float EaseInSine(float t)
    {
        return 1 - cos((t * XM_PI) / 2);
    }

    static float EaseOutSine(float t)
    {
        return  sin((t * XM_PI) / 2);
    }

    static float EaseInOutSine(float t)
    {
        return -(cos(XM_PI * t) - 1) / 2;
    }

    static float EaseInQuad(float t)
    {
        return t * t;
    }

    inline static float EaseOutQuad(float t)
    {
        return 1 - (1 - t) * (1 - t);
    }

    static float EaseInOutQuad(float t)
    {
        return t < 0.5f ? 2.f * t * t : 1.f - pow(-2.f * t + 2.f, 2.f) * 0.5f;
    }

    static float EaseInCubic(float t)
    {
        return t * t * t;
    }

    static float EaseOutCubic(float t)
    {
        return 1.f - pow(1.f - t, 3.f);
    }

    static float EaseInOutCubic(float t)
    {
        return t < 0.5 ? 4.f * t * t * t : 1.f - pow(-2.f * t + 2.f, 3.f) * 0.5f;
    }

    static float EaseInQuart(float t)
    {
        return t * t * t * t;
    }

    static float EaseOutQuart(float t)
    {
        return 1.f - pow(1.f - t, 4.f);
    }

    static float EaseInOutQuart(float t)
    {
        return t < 0.5 ? 8.f * t * t * t * t : 1.f - pow(-2.f * t + 2.f, 4.f) * 0.5f;
    }

    static float EaseInQuint(float t)
    {
        return t * t * t * t * t;
    }

    static float EaseOutQuint(float t)
    {
        return 1.f - pow(1.f - t, 5.f);
    }

    static float EaseInOutQuint(float t)
    {
        return t < 0.5 ? 16.f * t * t * t * t * t : 1.f - pow(-2.f * t + 2.f, 5.f) * 0.5f;
    }

    static float EaseInExpo(float t)
    {
        return t == 0 ? 0 : pow(2.f, 10.f * t - 10.f);
    }

    static float EaseOutExpo(float t)
    {
        return t == 1.f ? 1.f : 1.f - pow(2.f, -10.f * t);
    }

    static float EaseInOutExpo(float t)
    {
        if (t == 0) return 0;
        if (t == 1) return 1;
        return t < 0.5 ? pow(2.f, 20.f * t - 10.f) / 2.f : (2.f - pow(2.f, -20.f * t + 10.f)) * 0.5f;
    }

    static float EaseInCirc(float t)
    {
        return 1.f - sqrt(1.f - pow(t, 2.f));
    }

    static float EaseOutCirc(float t)
    {
        return  sqrt(1.f - pow(t - 1.f, 2.f));
    }

    static float EaseInOutCirc(float t)
    {
        return t < 0.5f ? (1.f - sqrt(1.f - pow(2.f * t, 2.f))) / 2.f : (sqrt(1.f - pow(-2.f * t + 2.f, 2.f)) + 1.f) * 0.5f;
    }

    static float EaseInBack(float t)
    {
        const float c1 = 1.70158f;
        const float c3 = c1 + 1.f;
        return c3 * t * t * t - c1 * t * t;
    }

    static float EaseOutBack(float t)
    {
        const float c1 = 1.70158f;
        const float c3 = c1 + 1.f;
        return 1.f + c3 * pow(t - 1.f, 3.f) + c1 * pow(t - 1.f, 2.f);
    }

    static float EaseInOutBack(float t)
    {
        const float c1 = 1.70158f;
        const float c2 = c1 * 1.525f;
        return t < 0.5f
            ? (pow(2.f * t, 2.f) * ((c2 + 1.f) * 2.f * t - c2)) * 0.5f
            : (pow(2.f * t - 2.f, 2.f) * ((c2 + 1.f) * (t * 2.f - 2.f) + c2) + 2.f) * 0.5f;
    }

    static float EaseInElastic(float t)
    {
        const float c4 = (2.f * XM_PI) / 3.f;
        return t == 0 ? 0 : t == 1.f ? 1.f : -pow(2.f, 10.f * t - 10.f) * sin((t * 10.f - 10.75f) * c4);
    }

    static float EaseOutElastic(float t)
    {
        const float c4 = (2.f * XM_PI) / 3.f;
        return t == 0 ? 0 : t == 1.f ? 1.f : pow(2.f, -10.f * t) * sin((t * 10.f - 0.75f) * c4) + 1.f;
    }

    static float EaseInOutElastic(float t)
    {
        const float c5 = (2 * XM_PI) / 4.5f;
        if (t == 0) return 0;
        if (t == 1.f) return 1.f;
        return t < 0.5f
            ? -(pow(2.f, 20.f * t - 10.f) * sin((20.f * t - 11.125f) * c5)) * 0.5f
            : (pow(2.f, -20.f * t + 10.f) * sin((20.f * t - 11.125f) * c5)) * 0.5f + 1;
    }

    static float EaseInBounce(float t)
    {
        return 1 - EaseOutBounce(1 - t);
    }

    static float EaseOutBounce(float t)
    {
        const float n1 = 7.5625f;
        const float d1 = 2.75f;

        if (t < 1 / d1)
        {
            return n1 * t * t;
        }
        else if (t < 2 / d1)
        {
            t -= 1.5f / d1;
            return n1 * t * t + 0.75f;
        }
        else if (t < 2.5 / d1)
        {
            t -= 2.25f / d1;
            return n1 * t * t + 0.9375f;
        }
        else
        {
            t -= 2.625f / d1;
            return n1 * t * t + 0.984375f;
        }
    }

    static float EaseInOutBounce(float t)
    {
        return t < 0.5f
            ? (1 - EaseOutBounce(1 - 2 * t)) * 0.5f
            : (1 + EaseOutBounce(2 * t - 1)) * 0.5f;
    }

    static float ApplyEasing(EaseType type, float t)
    {
        switch (type)
        {
        case EaseType::easeLinear: return t;
        case EaseType::easeInSine: return EaseInSine(t);
        case EaseType::easeOutSine: return EaseOutSine(t);
        case EaseType::easeInOutSine: return EaseInOutSine(t);
        case EaseType::easeInQuad: return EaseInQuad(t);
        case EaseType::easeOutQuad: return EaseOutQuad(t);
        case EaseType::easeInOutQuad: return EaseInOutQuad(t);
        case EaseType::easeInCubic: return EaseInCubic(t);
        case EaseType::easeOutCubic: return EaseOutCubic(t);
        case EaseType::easeInOutCubic: return EaseInOutCubic(t);
        case EaseType::easeInQuart: return EaseInQuart(t);
        case EaseType::easeOutQuart: return EaseOutQuart(t);
        case EaseType::easeInOutQuart: return EaseInOutQuart(t);
        case EaseType::easeInQuint: return EaseInQuint(t);
        case EaseType::easeOutQuint: return EaseOutQuint(t);
        case EaseType::easeInOutQuint: return EaseInOutQuint(t);
        case EaseType::easeInExpo: return EaseInExpo(t);
        case EaseType::easeOutExpo: return EaseOutExpo(t);
        case EaseType::easeInOutExpo: return EaseInOutExpo(t);
        case EaseType::easeInBack: return EaseInBack(t);
        case EaseType::easeOutBack: return EaseOutBack(t);
        case EaseType::easeInOutBack: return EaseInOutBack(t);
        case EaseType::easeInElastic: return EaseInElastic(t);
        case EaseType::easeOutElastic: return EaseOutElastic(t);
        case EaseType::easeInOutElastic: return EaseInOutElastic(t);
        case EaseType::easeInBounce: return EaseInBounce(t);
        case EaseType::easeOutBounce: return EaseOutBounce(t);
        case EaseType::easeInOutBounce: return EaseInOutBounce(t);
        case EaseType::easeStep: return t < 1.f ? 0.f : 1.f;
        default: return t;
        }
    }
};


// Easing 함수는 시간 흐름에 따른 매개변수의 변화율을 지정합니다.
// 대부분의 실제 사물들은 일정한 속도로 이동하지 않고, 즉시 시작하거나 즉시 멈추지도 않습니다.
// 서랍을 예로 들자면, 처음에는 빠르게 열다가 거의 다 열었을 때쯤에는 천천히 엽니다.
// 사물을 바닥에 떨어트렸을 때는 사물이 아래로 가속하다 사물이 바닥을 쳤을 때 튕겨 올라옵니다.
//
// 1. easeIn 계열 함수들은 점점 가속되는 효과를 제공합니다.
// 2. easeOut 계열 함수들은 반대로 점점 감속되는 효과를 제공합니다.
// 3. easeInOut 계열 함수들은 가속과 감속이 모두 적용되어 양쪽에서 부드럽게 전환됩니다.
// 이 함수들은 t가 0에서 1까지 진행되며, 각각의 곡선을 정의합니다. 필요에 맞게 사용하시면 됩니다.
//	참고 링크 : https://easings.net/ko

// 사용법
// a를 b까지 움직이는데 걸리는 시간(duration)이 있다고합니다.
// POINT StartPosition = a; // 시작할 좌표
// POINT EndPosition   = b; // 도착할 좌표
//
// float mCurrentTime = 0.f;
// float mDurationTime = 2.f; // a를 b까지 움직이는데 2초가 걸린다.
// void MoveAtoB()
//{
// 	 mCurrentTime += mDeltaTime;             // 프레임당 걸리는 시간 더해주기
//   float t = mCurrentTime / mDurationTime;
//   
//   a = Lerp(StartPosition, EndPosition, EaseInSine(t));
//}

struct AnimationCurve2
{
    template<typename T>
    static T Lerp(T a, T b, float t)
    {
        return a + t * (b - a);
    }

    static float EaseInSine(float t)
    {
        return 1 - std::cos((t * PI) / 2);
    }

    static float EaseOutSine(float t)
    {
        return std::sin((t * PI) / 2);
    }

    static float EaseInOutSine(float t)
    {
        return -(std::cos(PI * t) - 1) / 2;
    }

    static float EaseInQuad(float t)
    {
        return t * t;
    }

    static float EaseOutQuad(float t)
    {
        return 1 - (1 - t) * (1 - t);
    }

    static float EaseInOutQuad(float t)
    {
        return t < 0.5f ? 2.f * t * t : 1.f - std::pow(-2.f * t + 2.f, 2.f) * 0.5f;
    }

    static float EaseInCubic(float t)
    {
        return t * t * t;
    }

    static float EaseOutCubic(float t)
    {
        return 1.f - std::pow(1.f - t, 3.f);
    }

    static float EaseInOutCubic(float t)
    {
        return t < 0.5 ? 4.f * t * t * t : 1.f - std::pow(-2.f * t + 2.f, 3.f) * 0.5f;
    }

    static float EaseInQuart(float t)
    {
        return t * t * t * t;
    }

    static float EaseOutQuart(float t)
    {
        return 1.f - std::pow(1.f - t, 4.f);
    }

    static float EaseInOutQuart(float t)
    {
        return t < 0.5 ? 8.f * t * t * t * t : 1.f - std::pow(-2.f * t + 2.f, 4.f) * 0.5f;
    }

    static float EaseInQuint(float t)
    {
        return t * t * t * t * t;
    }

    static float EaseOutQuint(float t)
    {
        return 1.f - std::pow(1.f - t, 5.f);
    }

    static float EaseInOutQuint(float t)
    {
        return t < 0.5 ? 16.f * t * t * t * t * t : 1.f - std::pow(-2.f * t + 2.f, 5.f) * 0.5f;
    }

    static float EaseInExpo(float t)
    {
        return t == 0 ? 0 : std::pow(2.f, 10.f * t - 10.f);
    }

    static float EaseOutExpo(float t)
    {
        return t == 1.f ? 1.f : 1.f - std::pow(2.f, -10.f * t);
    }

    static float EaseInOutExpo(float t)
    {
        if (t == 0) return 0;
        if (t == 1) return 1;
        return t < 0.5 ? std::pow(2.f, 20.f * t - 10.f) / 2.f : (2.f - std::pow(2.f, -20.f * t + 10.f)) * 0.5f;
    }

    static float EaseInCirc(float t)
    {
        return 1.f - std::sqrt(1.f - std::pow(t, 2.f));
    }

    static float EaseOutCirc(float t)
    {
        return std::sqrt(1.f - std::pow(t - 1.f, 2.f));
    }

    static float EaseInOutCirc(float t)
    {
        return t < 0.5f ? (1.f - std::sqrt(1.f - std::pow(2.f * t, 2.f))) / 2.f : (std::sqrt(1.f - std::pow(-2.f * t + 2.f, 2.f)) + 1.f) * 0.5f;
    }

    static float EaseInBack(float t)
    {
        const float c1 = 1.70158f;
        const float c3 = c1 + 1.f;
        return c3 * t * t * t - c1 * t * t;
    }

    static float EaseOutBack(float t)
    {
        const float c1 = 1.70158f;
        const float c3 = c1 + 1.f;
        return 1.f + c3 * std::pow(t - 1.f, 3.f) + c1 * std::pow(t - 1.f, 2.f);
    }

    static float EaseInOutBack(float t)
    {
        const float c1 = 1.70158f;
        const float c2 = c1 * 1.525f;
        return t < 0.5f
            ? (std::pow(2.f * t, 2.f) * ((c2 + 1.f) * 2.f * t - c2)) * 0.5f
            : (std::pow(2.f * t - 2.f, 2.f) * ((c2 + 1.f) * (t * 2.f - 2.f) + c2) + 2.f) * 0.5f;
    }

    static float EaseInElastic(float t)
    {
        const float c4 = (2.f * PI) / 3.f;
        return t == 0 ? 0 : t == 1.f ? 1.f : -std::pow(2.f, 10.f * t - 10.f) * std::sin((t * 10.f - 10.75f) * c4);
    }

    static float EaseOutElastic(float t)
    {
        const float c4 = (2.f * PI) / 3.f;
        return t == 0 ? 0 : t == 1.f ? 1.f : std::pow(2.f, -10.f * t) * std::sin((t * 10.f - 0.75f) * c4) + 1.f;
    }

    static float EaseInOutElastic(float t)
    {
        const float c5 = (2 * PI) / 4.5f;
        if (t == 0) return 0;
        if (t == 1.f) return 1.f;
        return t < 0.5f
            ? -(std::pow(2.f, 20.f * t - 10.f) * std::sin((20.f * t - 11.125f) * c5)) * 0.5f
            : (std::pow(2.f, -20.f * t + 10.f) * std::sin((20.f * t - 11.125f) * c5)) * 0.5f + 1;
    }

    static float EaseInBounce(float t)
    {
        return 1 - EaseOutBounce(1 - t);
    }

    static float EaseOutBounce(float t)
    {
        const float n1 = 7.5625f;
        const float d1 = 2.75f;

        if (t < 1 / d1)
        {
            return n1 * t * t;
        }
        else if (t < 2 / d1)
        {
            t -= 1.5f / d1;
            return n1 * t * t + 0.75f;
        }
        else if (t < 2.5 / d1)
        {
            t -= 2.25f / d1;
            return n1 * t * t + 0.9375f;
        }
        else
        {
            t -= 2.625f / d1;
            return n1 * t * t + 0.984375f;
        }
    }

    static float EaseInOutBounce(float t)
    {
        return t < 0.5f
            ? (1 - EaseOutBounce(1 - 2 * t)) * 0.5f
            : (1 + EaseOutBounce(2 * t - 1)) * 0.5f;
    }
};