#pragma once
#include "Vector2.h"
#include <Windows.h>
#include <string>
#include <memory>

// .bmp �̹����� ���ٵ� png�� jpg���� ������ ���� �ʹ� �ϸ� �ƹ����� ���� ��ȯ�ϸ� �ȵȴ�. ���о��
// �׸������� ���� .bmp�� �����ϸ� �ٷ� �ذ�ȴ�.
// ����δ� .exe �������� �����ϱ� �׷��� �˰� ����ϸ� ��


class ImageHandler
{
public:
    // �ܼ��� �ٰ����� ȸ����Ű�� ������
    static void renderRotatedPolygon(HDC _hdc, Vector2 center, std::vector<Vector2Int> points, float angle);
    // �ܼ��� �簢���� ȸ����Ű�� ������
    static void renderRotatedRectangle(HDC hdc, Vector2 center, int width, int height, float angle);

    // .bmp�� �ҷ��ͼ� HBITMAP���� ��ȯ
    static HBITMAP loadImg(std::string filePath);
    // �̹����� ������ ũ�⸸ŭ �ڸ��� ��ȯ
    static HBITMAP cropImage(const HBITMAP& hOriginalBitmap, int x, int y, int width, int height);
    // �̹����� ������ ũ�⸸ŭ �ڸ��� ��ȯ(vector2 ����)
    static HBITMAP cropImage(const HBITMAP& origin, Vector2Int startPos, Vector2Int imgSize);
    // �̹����� �����ؼ� ���� �ٿ����Ƽ� ũ�� ������ְ� ��ȯ
    static HBITMAP duplicateImage(const HBITMAP& origin);
    // �̹����� �����ؼ� ���� ������ �ٿ����� ũ�⸸ŭ �ڸ� �� ��ȯ(tessellation)
    static HBITMAP resizeImage(const HBITMAP& origin, int width, int height);
    // �̹��� ������ �� �� ��ȯ, �ػ� ��ȭ�� ����
    static HBITMAP zoomImage(const HBITMAP& origin, float magnification);
    // �̹��� ȸ��
    static HBITMAP RotateBitmap(HBITMAP hBitmap, float angle);

    // �̹��� ����
    static void Render(const HBITMAP& _bitMap, HDC& _hdc, int x, int y);
    // �̹��� ���� - �̹��� ��ü�� alpha��(0~255, ���� �������� ����)��ŭ �����ϰ� ����
    static void TransparentRender(const HBITMAP& hBitmap, HDC& _hdc, int x, int y, BYTE alpha);
    // �̹��� ���� - �� ����� �����ϰ� ����(���� : RGB(255,0,255))
    static void renderWithoutBack(const HBITMAP& _bitMap, HDC& _hdc, int x, int y);
    // �̹��� ���� - �̹����� ������ �� �� ��������
    static void zoomRender(const HBITMAP& _hBitmap, HDC _hdc, int x, int y, float magnification);
private:
    ImageHandler() {}
    ~ImageHandler() {}

    // �̹��� ȸ���� �ٰ��� ȸ���� ���� �Լ�
    static void RotatePoint(Vector2Int& point, const Vector2& center, float angle);
};

/*
BOOL BitBlt(
  HDC hdcDest,    // ��� DC (����� ��)
  int nXDest,     // ��� DC�� X ��ǥ
  int nYDest,     // ��� DC�� Y ��ǥ
  int nWidth,     // ������ �ʺ�
  int nHeight,    // ������ ����
  HDC hdcSrc,     // ���� DC (������ ��)
  int nXSrc,      // ���� DC�� X ��ǥ
  int nYSrc,      // ���� DC�� Y ��ǥ
  DWORD dwRop     // ��Ʈ ���� ����
);

StretchBlt(
    _hdc,          // ��� DC
    x,            // ��� X ��ǥ
    y,            // ��� Y ��ǥ
    width,        // ��� �ʺ�
    height,       // ��� ����
    hdcSrc,           // �ҽ� DC
    0,                // �ҽ� X ��ǥ
    0,                // �ҽ� Y ��ǥ
    bitmap.bmWidth,         // �ҽ� �ʺ�
    bitmap.bmHeight,        // �ҽ� ����
    SRCCOPY           // ��Ʈ ����� �۾�
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

    //������ �Ͼ���̿��� �Ͼ���� �����ϸ� ���� -> ������ �Ͼ���̿��� ==> ���� �Ⱦ��� ������ ��ġ�ص�
    TransparentBlt(_hdc, x, y, bitmap.bmWidth, bitmap.bmHeight, dcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, RGB(255, 0, 255));

    DeleteDC(dcMem);
}


void ImageHandler::TransparentRender(const HBITMAP& hBitmap, HDC& _hdc, int x, int y, BYTE alpha) //alpha�� 0~255 ���� �ֱ�(�� �������� ����)
{
    HDC hdcMem = CreateCompatibleDC(_hdc);
    SelectObject(hdcMem, hBitmap);

    BITMAP bitmap;
    GetObject(hBitmap, sizeof(BITMAP), &bitmap);

    BLENDFUNCTION blendFunc{ AC_SRC_OVER, 0, alpha, AC_SRC_ALPHA }; // �⺻ �ռ� ���/�÷���/���� �� ����/�ҽ� ��Ʈ���� ���� ä�� ���

    AlphaBlend(_hdc, x, y, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, blendFunc); //BOOL result = AlphaBlend(��� DC, X ��ġ, Y ��ġ, ��Ʈ�� �ʺ�, ��Ʈ�� ����, ���� DC, ���� ��Ʈ�� X ��ǥ, ���� ��Ʈ�� Y ��ǥ, ���� ��Ʈ�� �ʺ�, ���� ��Ʈ�� ����, BLENDFUNCTION ����ü);

    DeleteDC(hdcMem);
}

//���� �ܾƿ� �� �ȴ�
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

//���� �ܾƿ� �� �ȴ�
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
    // ������ �������� ��ȯ
    float radians = angle * 3.14159265358979f / 180.0f;

    Vector2 normalizePoint = point - center;

    // ȸ�� ��� ����
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
        {(int)center.x - width / 2, (int)center.y - height / 2}, // �»��
        {(int)center.x + width / 2, (int)center.y - height / 2}, // ����
        {(int)center.x + width / 2, (int)center.y + height / 2}, // ���ϴ�
        {(int)center.x - width / 2, (int)center.y + height / 2}  // ���ϴ�
    };

    renderRotatedPolygon(hdc, center, points, angle);
}



HBITMAP ImageHandler::RotateBitmap(HBITMAP hBitmap, float angle)
{
    BITMAP bitmap;
    GetObject(hBitmap, sizeof(BITMAP), &bitmap);

    int width = bitmap.bmWidth;
    int height = bitmap.bmHeight;

    // �̹����� �밢�� ���� ���
    int canvasSize = static_cast<int>(sqrt(width * width + height * height)) + 1;

    // ���ο� ��Ʈ�� ũ�⸦ �밢�� ���̷� ����
    HDC hdc = CreateCompatibleDC(NULL);
    HBITMAP hNewBitmap = CreateCompatibleBitmap(hdc, canvasSize, canvasSize);
    HDC hdcMem = CreateCompatibleDC(NULL);
    SelectObject(hdcMem, hBitmap);

    HDC hdcRotated = CreateCompatibleDC(NULL);
    SelectObject(hdcRotated, hNewBitmap);

    // ����
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 255));
    RECT rect = { 0, 0, canvasSize, canvasSize };
    FillRect(hdcRotated, &rect, hBrush);
    DeleteObject(hBrush);

    Vector2Int newCenter{ canvasSize / 2, canvasSize / 2 };
    Vector2Int center{ width / 2, height / 2 };
    Vector2Int diff{ newCenter - center };

    // ȸ���� �̹��� �׸���
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

    // ���ҽ� ����
    DeleteDC(hdc);
    DeleteDC(hdcMem);
    DeleteDC(hdcRotated);

    return hNewBitmap;
}