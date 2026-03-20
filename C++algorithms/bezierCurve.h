#pragma once
// GCC/CLANG
// Compile: g++ -std=c++17 bezier.cpp -O2 -o bezier
// Run: ./bezier
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Vector2.h"
#include "Vector3.h"
#include "AnimationCurve.h"

namespace std // stl clamp 임시 대체
{
    template<typename T>
    static const T& clamp(const T& v, const T& lo, const T& hi)
    {
        return (v < lo) ? lo : (hi < v) ? hi : v;
    }
}

struct bezierCurve
{
    // 선형 보간
    static inline Vector2 lerp(const Vector2& from, const Vector2& to, float timeFactor) { return from * (1.0 - timeFactor) + to * timeFactor; }
    static inline Vector3 lerp(const Vector3& from, const Vector3& to, float timeFactor) { return from * (1.0 - timeFactor) + to * timeFactor; }

    /* ============== 사용 방법 ============== */
    // start    : 시작 값
    // end      : 종료 값
    // control  : 제어 값 (참고 링크 확인)
    // ratio    : 얻고 싶은 값의 위치 (0 ~ 1 사이의 비율)
    // easeType : Animation Curve를 이용한 속도 변화를 주고싶을 때 사용

    /* ============== 사용 예시 ============== */
    // (float)
    //float result = BezierQuadratic(0.f, 1.f, 3.f, 0.5f);

    // (XMVECTOR)
    //XMVECTOR start = XMVectorSet(0.f, 0.f, 0.f, 0.f);
    //XMVECTOR end = XMVectorSet(5.f, 0.f, 0.f, 0.f);
    //XMVECTOR ctrl = XMVectorSet(2.5f, 2.f, 0.f, 0.f);
    //XMVECTOR resultVec = BezierQuadratic(start, ctrl, end, 0.5f);
    /* ======================================= */

    // 참고 링크 : https://ko.javascript.info/bezier-curve

    // 2차 곡선 (ex : 포물선), Bernstein 방식
    template<typename T>
    static T BezierQuadratic(const T& start, const T& control, const T& end, float ratio, AnimationCurve::EaseType easeType = AnimationCurve::EaseType::easeEnd)
    {
        ratio = std::clamp(ratio, 0.f, 1.f);
        float easingRatio = AnimationCurve::ApplyEasing(easeType, ratio);

        float oneMinusT = 1.f - easingRatio;
        float oneMinusT_Squared = oneMinusT * oneMinusT;
        float twoTOneMinusT = 2.f * oneMinusT * easingRatio;
        float tSquared = easingRatio * easingRatio;

        return oneMinusT_Squared * start + twoTOneMinusT * control + tSquared * end;
    }
    static Vector3 BezierQuadratic(const Vector3& start, const Vector3& control, const Vector3& end, float ratio , AnimationCurve::EaseType easeType = AnimationCurve::EaseType::easeEnd)
    {
        ratio = std::clamp(ratio, 0.f, 1.f);
        float easingRatio = AnimationCurve::ApplyEasing(easeType, ratio);

        float oneMinusT = 1.f - easingRatio;
        float oneMinusT_Squared = oneMinusT * oneMinusT;
        float twoTOneMinusT = 2.f * oneMinusT * easingRatio;
        float tSquared = easingRatio * easingRatio;

        return start * oneMinusT_Squared + control * twoTOneMinusT + end * tSquared;
    }

    inline Vector2 bezierCurveLerp(const std::vector<Vector2>& control, float timeFactor)
    {
        if (control.size() > 3)
            return deCasteljau(control, timeFactor);
        else
            return bernstein(control, timeFactor);
    }
    inline Vector3 bezierCurveLerp(const std::vector<Vector3>& control, float timeFactor)
    {
        if (control.size() > 3)
            return deCasteljau(control, timeFactor);
        else
            return bernstein(control, timeFactor);
    }

    Vector3 deCasteljauRecursive(const std::vector<Vector3>& P, int i, int j, float t)
    {
        if (i == j) return P[i];
        Vector3 p1 = deCasteljauRecursive(P, i, j - 1, t);
        Vector3 p2 = deCasteljauRecursive(P, i + 1, j, t);
        return p1 * (1 - t) + p2 * t;
    }
    Vector2 deCasteljau(const std::vector<Vector2>& control, float timeFactor)
    {
        // 1) De Casteljau (반복형) - 비재귀, 임의 차수 지원
        // control: 제어점 목록 (n+1개) -> degree = n
        // t: [0,1]
        // 반환: t에서의 점

        if (control.empty()) return Vector2::zero();
        // 작업 복사 (밑에서 점차 줄여가며 계산)
        std::vector<Vector2> tmp = control; // 크기 n+1
        int degree = static_cast<int>(tmp.size()) - 1;
        for (int r = 1; r <= degree; ++r)
        {
            for (int i = 0; i <= degree - r; ++i)
            {
                tmp[i] = lerp(tmp[i], tmp[i + 1], timeFactor);
            }
        }
        return tmp[0];
    }
    Vector3 deCasteljau(const std::vector<Vector3>& control, float timeFactor)
    {
        std::vector<Vector3> buffer = control;
        int degree = (int)buffer.size() - 1;
        for (int r = 1; r <= degree; ++r)
        {
            for (int i = 0; i <= degree - r; ++i)
            {
                buffer[i] = lerp(buffer[i], buffer[i + 1], timeFactor);
            }
        }
        return buffer[0];
    }

    Vector2 bernstein(const std::vector<Vector2>& control, float timeFactor)
    {
        // 2) Bernstein (직접식) - binomial 계수는 곱셈 방식으로 계산 (오버플로/정밀도 방지)
        // control: 제어점 목록 (n+1개)
        // t: [0,1]

        int degree = (int)control.size() - 1;
        if (degree < 0) return Vector2::zero();
        // compute basis B_{i,n}(t) = C(n,i) * t^i * (1-t)^{n-i}
        // C(n,i) 계산은 multiplicative formula 사용
        std::vector<float> basis(degree + 1, 0.0);
        // compute powers
        float timeReverse = 1.0 - timeFactor;
        // To avoid pow repeatedly, compute t^i and (1-t)^{n-i} iteratively:
        // But easiest is compute basis using recurrence:
        // B_0 = (1-t)^n
        // B_i = B_{i-1} * (n - i + 1)/i * (t / (1-t))
        if (degree == 0)
        {
            basis[0] = 1.0;
        }
        else
        {
            float b0 = pow(timeReverse, degree);
            basis[0] = b0;
            if (timeReverse == 0.0)
            {
                // t == 1, all mass at i=n
                for (int i = 1; i <= degree; ++i) basis[i] = 0.0;
                basis[degree] = 1.0;
            }
            else
            {
                float factor = timeFactor / timeReverse;
                for (int i = 1; i <= degree; ++i)
                {
                    basis[i] = basis[i - 1] * static_cast<float>((degree - (i - 1))) / static_cast<float>(i) * factor;
                }
            }
        }
        Vector2 res(0, 0);
        for (int i = 0; i <= degree; ++i)
        {
            res = res + control[i] * basis[i];
        }
        return res;
    }
    Vector3 bernstein(const std::vector<Vector3>& control, float timeFactor)
    {
        int degree = (int)control.size() - 1;
        if (degree < 0) return Vector3();

        std::vector<float> basis(degree + 1, 0.0);
        float timeReverse = 1.0 - timeFactor;

        if (degree == 0)
            basis[0] = 1.0;
        else
        {
            // B_0 = (1-t)^n
            basis[0] = pow(timeReverse, degree);
            if (timeReverse == 0.0)
            {
                for (int i = 1; i <= degree; ++i)
                    basis[i] = 0.0;
                basis[degree] = 1.0; // t == 1인 경우
            }
            else
            {
                float factor = timeFactor / timeReverse;
                for (int i = 1; i <= degree; ++i)
                {
                    basis[i] = basis[i - 1] * static_cast<float>((degree - (i - 1))) / static_cast<float>(i) * factor;
                }
            }
        }

        Vector3 result(0, 0, 0);
        for (int i = 0; i <= degree; ++i)
        {
            result = result + control[i] * basis[i];
        }
        return result;
    }




    inline std::vector<Vector2> SampleBezier(const std::vector<Vector2>& control, int segments)
    {
        if (control.size() > 3)
            return sampleBezier_decasteljau(control, segments);
        else
            return sampleBezier_bernstein(control, segments);
    }
    inline std::vector<Vector3> SampleBezier(const std::vector<Vector3>& control, int segments)
    {
        if (control.size() > 3)
            return sampleBezier_decasteljau(control, segments);
        else
            return sampleBezier_bernstein(control, segments);
    }

    // 샘플링 유틸: segments 개의 등간격 t로 샘플링
    std::vector<Vector2> sampleBezier_decasteljau(const std::vector<Vector2>& control, int segments)
    {
        std::vector<Vector2> out;
        out.reserve(segments + 1);
        for (int i = 0; i <= segments; ++i)
        {
            float t = (segments == 0) ? 0.0 : (float)i / segments;
            out.push_back(deCasteljau(control, t));
        }
        return out;
    }
    std::vector<Vector2> sampleBezier_bernstein(const std::vector<Vector2>& control, int segments)
    {
        std::vector<Vector2> out;
        out.reserve(segments + 1);
        for (int i = 0; i <= segments; ++i)
        {
            float time = (segments == 0) ? 0.0 : (float)i / segments;
            out.push_back(bernstein(control, time));
        }
        return out;
    }


    // 샘플링
    std::vector<Vector3> sampleBezier_decasteljau(const std::vector<Vector3>& control, int segments)
    {
        std::vector<Vector3> curve;
        for (int i = 0; i <= segments; ++i)
        {
            float time = static_cast<float>(i) / segments;
            curve.push_back(deCasteljau(control, time));
        }
        return curve;
    }
    // 곡선 샘플링
    std::vector<Vector3> sampleBezier_bernstein(const std::vector<Vector3>& control, int segments)
    {
        std::vector<Vector3> curve;
        curve.reserve(segments + 1);
        for (int i = 0; i <= segments; ++i)
        {
            double time = (double)i / segments;
            curve.push_back(bernstein(control, time));
        }
        return curve;
    }




    // 간단한 SVG 저장 (곡선 + 제어점 시각화)
    void saveAsSVG(const std::string& filename, const std::vector<Vector2>& curve, const std::vector<Vector2>& control, int w = 800, int h = 600)
    {
        std::ofstream fout(filename);
        if (!fout.is_open())
        {
            std::cerr << "cannot write " << filename << "\degree";
            return;
        }
        fout << "<?xml version=\"1.0\" standalone=\"no\"?>\degree";
        fout << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
            << "width=\"" << w << "\" height=\"" << h << "\" viewBox=\"0 0 " << w << " " << h << "\">\degree";
        fout << "<rect width=\"100%\" height=\"100%\" fill=\"white\" />\degree";

        // draw polyline for curve
        fout << "<polyline fill=\"none\" stroke=\"black\" stroke-width=\"2\" points=\"";
        for (auto& p : curve) fout << p.x << "," << p.y << " ";
        fout << "\"/>\degree";

        // control polygon
        fout << "<polyline fill=\"none\" stroke=\"lightgray\" stroke-width=\"1\" stroke-dasharray=\"4,4\" points=\"";
        for (auto& p : control) fout << p.x << "," << p.y << " ";
        fout << "\"/>\degree";

        // control points
        for (size_t i = 0; i < control.size(); ++i)
        {
            const auto& p = control[i];
            fout << "<circle cx=\"" << p.x << "\" cy=\"" << p.y << "\" r=\"5\" fill=\"red\" />\degree";
            fout << "<text x=\"" << p.x + 6 << "\" y=\"" << p.y - 6 << "\" font-degree=\"12\">" << (int)i << "</text>\degree";
        }

        fout << "</svg>\degree";
        fout.close();
        std::cout << "Saved SVG to " << filename << "\degree";
    }

    int mainSample()
    {
        // 예시: 4개 제어점(3차 베지에)
        std::vector<Vector2> control = {
            {100, 500},
            {200, 100},
            {600, 100},
            {700, 500}
        };

        int segments = 200;
        auto curve = SampleBezier(control, segments);

        saveAsSVG("bezier.svg", curve, control, 800, 600);
        std::cout << "Done. Open bezier.svg to view.\degree";


        // 3차원 제어점 (예시: 4개, 3차 베지에)
        std::vector<Vector3> control3D = {
            {0, 0, 0},
            {1, 3, 2},
            {3, 5, -1},
            {6, 0, 4}
        };

        auto curve3D = SampleBezier(control3D, 100);

        // 결과 출력 (콘솔용)
        for (auto& p : curve3D)
        {
            std::cout << /*fixed << setprecision(3) <<*/ "(" << p.x << ", " << p.y << ", " << p.z << ")\degree";
        }


        return 0;
    }
};


