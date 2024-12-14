#pragma once
#include "Vector2.h"
#include <Windows.h>
#include <string>
#include <memory>

// .bmp 이미지를 쓸텐데 png나 jpg같은 파일을 쓰고 싶다 하면 아무데나 가서 변환하면 안된다. 못읽어옴
// 그림판으로 열고 .bmp로 저장하면 바로 해결된다.
// 상대경로는 .exe 기준으로 잡히니깐 그렇게 알고 사용하면 댐


class ImageHandler
{
public:
    // 단순한 다각형을 회전시키고 렌더링
    static void renderRotatedPolygon(HDC _hdc, Vector2 center, std::vector<Vector2Int> points, float angle);
    // 단순한 사각형을 회전시키고 렌더링
    static void renderRotatedRectangle(HDC hdc, Vector2 center, int width, int height, float angle);

    // .bmp을 불러와서 HBITMAP으로 변환
    static HBITMAP loadImg(std::string filePath);
    // 이미지를 지정한 크기만큼 자르고 반환
    static HBITMAP cropImage(const HBITMAP& hOriginalBitmap, int x, int y, int width, int height);
    // 이미지를 지정한 크기만큼 자르고 반환(vector2 전용)
    static HBITMAP cropImage(const HBITMAP& origin, Vector2Int startPos, Vector2Int imgSize);
    // 이미지를 복사해서 옆에 붙여놓아서 크게 만들어주고 반환
    static HBITMAP duplicateImage(const HBITMAP& origin);
    // 이미지를 복사해서 옆에 여러번 붙여놓고 크기만큼 자른 뒤 반환(tessellation)
    static HBITMAP resizeImage(const HBITMAP& origin, int width, int height);
    // 이미지 디지털 줌 후 반환, 해상도 열화가 있음
    static HBITMAP zoomImage(const HBITMAP& origin, float magnification);
    // 이미지 회전
    static HBITMAP RotateBitmap(HBITMAP hBitmap, float angle);

    // 이미지 렌더
    static void Render(const HBITMAP& _bitMap, HDC& _hdc, int x, int y);
    // 이미지 렌더 - 이미지 전체를 alpha값(0~255, 값이 낮을수록 투명)만큼 투명하게 만듬
    static void TransparentRender(const HBITMAP& hBitmap, HDC& _hdc, int x, int y, BYTE alpha);
    // 이미지 렌더 - 뒷 배경을 투명하게 만듬(배경색 : RGB(255,0,255))
    static void renderWithoutBack(const HBITMAP& _bitMap, HDC& _hdc, int x, int y);
    // 이미지 렌더 - 이미지를 디지털 줌 후 렌더해줌
    static void zoomRender(const HBITMAP& _hBitmap, HDC _hdc, int x, int y, float magnification);
private:
    ImageHandler() {}
    ~ImageHandler() {}

    // 이미지 회전과 다각형 회전에 쓰는 함수
    static void RotatePoint(Vector2Int& point, const Vector2& center, float angle);
};

/*
BOOL BitBlt(
  HDC hdcDest,    // 대상 DC (출력할 곳)
  int nXDest,     // 대상 DC의 X 좌표
  int nYDest,     // 대상 DC의 Y 좌표
  int nWidth,     // 복사할 너비
  int nHeight,    // 복사할 높이
  HDC hdcSrc,     // 원본 DC (복사할 것)
  int nXSrc,      // 원본 DC의 X 좌표
  int nYSrc,      // 원본 DC의 Y 좌표
  DWORD dwRop     // 비트 블렌드 연산
);

StretchBlt(
    _hdc,          // 대상 DC
    x,            // 대상 X 좌표
    y,            // 대상 Y 좌표
    width,        // 대상 너비
    height,       // 대상 높이
    hdcSrc,           // 소스 DC
    0,                // 소스 X 좌표
    0,                // 소스 Y 좌표
    bitmap.bmWidth,         // 소스 너비
    bitmap.bmHeight,        // 소스 높이
    SRCCOPY           // 비트 블로팅 작업
);
*/
void ImageHandler::Render(const HBITMAP& _bitMap, HDC& _hdc, int x, int y)
{
    if (_bitMap == NULL) { return; }
    BITMAP bitmap;
    GetObject(_bitMap, sizeof(BITMAP), &bitmap);

    HDC dcMem = CreateCompatibleDC(_hdc);
    SelectObject(dcMem, _bitMap);

    BitBlt(_hdc, x, y, bitmap.bmWidth, bitmap.bmHeight, dcMem, 0, 0, SRCCOPY);

    DeleteDC(dcMem);
}

HBITMAP ImageHandler::loadImg(std::string filePath)
{
    return (HBITMAP)LoadImage(NULL, filePath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

HBITMAP ImageHandler::cropImage(const HBITMAP& origin, int x, int y, int width, int height)
{
    HDC hdc = CreateCompatibleDC(NULL);
    HDC hdcMem = CreateCompatibleDC(NULL);

    SelectObject(hdc, origin);
    HBITMAP hCroppedBitmap = CreateCompatibleBitmap(hdc, width, height);
    SelectObject(hdcMem, hCroppedBitmap);
    BitBlt(hdcMem, 0, 0, width, height, hdc, x, y, SRCCOPY);

    DeleteDC(hdc);
    DeleteDC(hdcMem);

    return hCroppedBitmap;
}

HBITMAP ImageHandler::cropImage(const HBITMAP& origin, Vector2Int startPos, Vector2Int imgSize)
{
    HDC hdc = CreateCompatibleDC(NULL);
    HDC hdcMem = CreateCompatibleDC(NULL);

    SelectObject(hdc, origin);
    HBITMAP hCroppedBitmap = CreateCompatibleBitmap(hdc, imgSize.x, imgSize.y);
    SelectObject(hdcMem, hCroppedBitmap);
    BitBlt(hdcMem, 0, 0, imgSize.x, imgSize.y, hdc, startPos.x, startPos.y, SRCCOPY);

    DeleteDC(hdc);
    DeleteDC(hdcMem);

    return hCroppedBitmap;
}


HBITMAP ImageHandler::duplicateImage(const HBITMAP& origin)
{
    BITMAP bitmap;
    GetObject(origin, sizeof(BITMAP), &bitmap);
    int width = bitmap.bmWidth;
    int height = bitmap.bmHeight;

    HDC hdc = CreateCompatibleDC(NULL);
    HDC hdcMem = CreateCompatibleDC(NULL);

    SelectObject(hdc, origin);
    HBITMAP newBitMap = CreateCompatibleBitmap(hdc, width * 2, height * 2);

    SelectObject(hdcMem, newBitMap);
    BitBlt(hdcMem, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
    BitBlt(hdcMem, width, 0, width, height, hdc, 0, 0, SRCCOPY);
    BitBlt(hdcMem, 0, height, width, height, hdc, 0, 0, SRCCOPY);
    BitBlt(hdcMem, width, height, width, height, hdc, 0, 0, SRCCOPY);

    DeleteDC(hdc);
    DeleteDC(hdcMem);

    return newBitMap;
}

HBITMAP ImageHandler::resizeImage(const HBITMAP& origin, int width, int height)
{
    BITMAP bitmap;
    GetObject(origin, sizeof(BITMAP), &bitmap);
    int imgWidth = bitmap.bmWidth;
    int imgHeight = bitmap.bmHeight;

    if (width < imgWidth && height < imgHeight)
    {
        return cropImage(origin, 0, 0, width, height);
    }
    if (width == imgWidth && height == imgHeight) { return origin; }

    HBITMAP resizedImg = origin;
    BITMAP resizedBitmap;

    while (true)
    {
        GetObject(resizedImg, sizeof(BITMAP), &resizedBitmap);
        int resizedWidth = bitmap.bmWidth;
        int resizedHeight = bitmap.bmHeight;

        if (imgWidth == width && resizedHeight == height)
        {
            return resizedImg;
        }

        if (imgWidth > width && resizedHeight > height)
        {
            break;
        }
        resizedImg = duplicateImage(resizedImg);
    }

    return cropImage(resizedImg, 0, 0, width, height);
}


void ImageHandler::renderWithoutBack(const HBITMAP& _bitMap, HDC& _hdc, int x, int y)
{
    if (_bitMap == NULL) { return; }
    BITMAP bitmap;
    GetObject(_bitMap, sizeof(BITMAP), &bitmap);

    HDC dcMem = CreateCompatibleDC(_hdc);
    SelectObject(dcMem, _bitMap);

    //배경색이 하얀색이여서 하얀색을 지정하면 깨짐 -> 바탕도 하얀색이여서 ==> 거의 안쓰는 색으로 배치해둠
    TransparentBlt(_hdc, x, y, bitmap.bmWidth, bitmap.bmHeight, dcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, RGB(255, 0, 255));

    DeleteDC(dcMem);
}


void ImageHandler::TransparentRender(const HBITMAP& hBitmap, HDC& _hdc, int x, int y, BYTE alpha) //alpha에 0~255 정수 넣기(값 낮을수록 투명)
{
    HDC hdcMem = CreateCompatibleDC(_hdc);
    SelectObject(hdcMem, hBitmap);

    BITMAP bitmap;
    GetObject(hBitmap, sizeof(BITMAP), &bitmap);

    BLENDFUNCTION blendFunc{ AC_SRC_OVER, 0, alpha, AC_SRC_ALPHA }; // 기본 합성 방식/플래그/알파 값 설정/소스 비트맵의 알파 채널 사용

    AlphaBlend(_hdc, x, y, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, blendFunc); //BOOL result = AlphaBlend(대상 DC, X 위치, Y 위치, 비트맵 너비, 비트맵 높이, 원본 DC, 원본 비트맵 X 좌표, 원본 비트맵 Y 좌표, 원본 비트맵 너비, 원본 비트맵 높이, BLENDFUNCTION 구조체);

    DeleteDC(hdcMem);
}

//줌인 줌아웃 다 된다
void ImageHandler::zoomRender(const HBITMAP& _hBitmap, HDC _hdc, int x, int y, float magnification)
{
    if (magnification == 1.0f) { Render(_hBitmap, _hdc, x, y); }

    HDC hdcSrc = CreateCompatibleDC(_hdc);
    SelectObject(hdcSrc, _hBitmap);

    BITMAP bitmap;
    GetObject(_hBitmap, sizeof(BITMAP), &bitmap);
    int width = (int)((float)bitmap.bmWidth * magnification);
    int height = (int)((float)bitmap.bmHeight * magnification);

    StretchBlt(_hdc, x, y, width, height, hdcSrc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

    DeleteDC(hdcSrc);
}

//줌인 줌아웃 다 된다
HBITMAP ImageHandler::zoomImage(const HBITMAP& origin, float magnification)
{
    if (magnification == 1.0f) { return origin; }
    BITMAP bitmap;
    GetObject(origin, sizeof(BITMAP), &bitmap);
    int width = (int)((float)bitmap.bmWidth * magnification);
    int height = (int)((float)bitmap.bmHeight * magnification);

    HDC hdc = CreateCompatibleDC(NULL);
    HDC hdcMem = CreateCompatibleDC(NULL);
    SelectObject(hdc, origin);

    HBITMAP newBitMap = CreateCompatibleBitmap(hdc, width, height);
    SelectObject(hdcMem, newBitMap);

    StretchBlt(hdcMem, 0, 0, width, height, hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

    DeleteDC(hdc);
    DeleteDC(hdcMem);

    return newBitMap;
}


void ImageHandler::RotatePoint(Vector2Int& point, const Vector2& center, float angle)
{
    // 각도를 라디안으로 변환
    float radians = angle * 3.14159265358979f / 180.0f;

    Vector2 normalizePoint = point - center;

    // 회전 행렬 적용
    Vector2Int rotatedPoint{
        static_cast<int>(normalizePoint.x * cos(radians) - normalizePoint.y * sin(radians)),
        static_cast<int>(normalizePoint.x * sin(radians) + normalizePoint.y * cos(radians))
    };

    point = rotatedPoint + center;
}

void ImageHandler::renderRotatedPolygon(HDC _hdc, Vector2 center, std::vector<Vector2Int> points, float angle)
{
    for (Vector2Int& point : points)
    {
        RotatePoint(point, center, angle);
    }

    std::unique_ptr<POINT[]> polygon = std::make_unique<POINT[]>(points.size());

    for (unsigned int i = 0; i < points.size(); i++)
    {
        polygon[i] = { points[i].x, points[i].y };
    }

    Polygon(_hdc, polygon.get(), 4);
}


void ImageHandler::renderRotatedRectangle(HDC hdc, Vector2 center, int width, int height, float angle)
{
    std::vector<Vector2Int> points{
        {(int)center.x - width / 2, (int)center.y - height / 2}, // 좌상단
        {(int)center.x + width / 2, (int)center.y - height / 2}, // 우상단
        {(int)center.x + width / 2, (int)center.y + height / 2}, // 우하단
        {(int)center.x - width / 2, (int)center.y + height / 2}  // 좌하단
    };

    renderRotatedPolygon(hdc, center, points, angle);
}



HBITMAP ImageHandler::RotateBitmap(HBITMAP hBitmap, float angle)
{
    BITMAP bitmap;
    GetObject(hBitmap, sizeof(BITMAP), &bitmap);

    int width = bitmap.bmWidth;
    int height = bitmap.bmHeight;

    // 이미지의 대각선 길이 계산
    int canvasSize = static_cast<int>(sqrt(width * width + height * height)) + 1;

    // 새로운 비트맵 크기를 대각선 길이로 설정
    HDC hdc = CreateCompatibleDC(NULL);
    HBITMAP hNewBitmap = CreateCompatibleBitmap(hdc, canvasSize, canvasSize);
    HDC hdcMem = CreateCompatibleDC(NULL);
    SelectObject(hdcMem, hBitmap);

    HDC hdcRotated = CreateCompatibleDC(NULL);
    SelectObject(hdcRotated, hNewBitmap);

    // 배경색
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 255));
    RECT rect = { 0, 0, canvasSize, canvasSize };
    FillRect(hdcRotated, &rect, hBrush);
    DeleteObject(hBrush);

    Vector2Int newCenter{ canvasSize / 2, canvasSize / 2 };
    Vector2Int center{ width / 2, height / 2 };
    Vector2Int diff{ newCenter - center };

    // 회전된 이미지 그리기
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Vector2Int point{ x,y };
            RotatePoint(point, center, angle);
            point += diff;

            if ((point.x >= 0 && point.x < canvasSize) && (point.y >= 0 && point.y < canvasSize))
            {
                COLORREF color = GetPixel(hdcMem, x, y);
                SetPixel(hdcRotated, point.x, point.y, color);
            }
        }
    }

    // 리소스 해제
    DeleteDC(hdc);
    DeleteDC(hdcMem);
    DeleteDC(hdcRotated);

    return hNewBitmap;
}