#include "Screen.h"
#include <Windows.h>
#include "RabbitCommonLog.h"

CScreen::CScreen(QObject *parent) : QObject(parent)
{
}

int CScreen::Width()
{
    return GetSystemMetrics(SM_CXSCREEN);
}

int CScreen::Height()
{
    return GetSystemMetrics(SM_CYSCREEN);
}

int CScreen::VirtualTop()
{
    return GetSystemMetrics(SM_XVIRTUALSCREEN);
}

int CScreen::VirtualLeft()
{
    return GetSystemMetrics(SM_YVIRTUALSCREEN);
}

int CScreen::VirtualWidth()
{
    return GetSystemMetrics(SM_CXVIRTUALSCREEN);
}

int CScreen::VirtualHeight()
{
    return GetSystemMetrics(SM_CYVIRTUALSCREEN);
}

int CScreen::VisibleMonitorCount()
{
    return GetSystemMetrics(SM_CMONITORS);
}

QImage CScreen::GetDesktop()
{
    QImage image;
    
    HWND hwnd = NULL;
    //hwnd = GetDesktopWindow();
    HDC dc = GetDC(hwnd);
    HDC memDc = CreateCompatibleDC(dc);
    HBITMAP bitmap;
    
    do{
        bitmap = CreateCompatibleBitmap(dc, Width(), Height());
        if(NULL == bitmap)
        {
            LOG_MODEL_ERROR("Screen",
                            "CreateCompatibleBitmap fail: %d",
                            GetLastError());
            break;
        }
        HGDIOBJ oldBitmap = SelectObject(memDc, bitmap);
        if(NULL == oldBitmap)
        {
            LOG_MODEL_ERROR("Screen",
                            "SelectObject fail: %d",
                            GetLastError());
            break;
        }
        if(!BitBlt(memDc, 0, 0, Width(), Height(), dc, 0, 0, SRCCOPY))
        {
            LOG_MODEL_ERROR("Screen",
                            "BitBlt fail: %d",
                            GetLastError());
            break;
        }
        
        // Get the bitmap format information
        BITMAPINFO bi;
        memset(&bi, 0, sizeof(bi));
        bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bi.bmiHeader.biBitCount = 0;
        // Get image format
        if (!::GetDIBits(dc, bitmap, 0, 1, NULL, (BITMAPINFO*)&bi, DIB_RGB_COLORS))
        {
            LOG_MODEL_ERROR("Screen",
                            "unable to determine device pixel format: %d",
                            GetLastError());
            break;
        }
        if (!::GetDIBits(dc, bitmap, 0, 1, NULL, (BITMAPINFO*)&bi, DIB_RGB_COLORS))
        {
            LOG_MODEL_ERROR("Screen",
                            "unable to determine pixel shifts/palette: %d",
                            GetLastError());
            break;
        }
        
        if(bi.bmiHeader.biBitCount > 8)
        {
            switch(bi.bmiHeader.biCompression)
            {
            case BI_RGB:
                // Default RGB layout
                switch (bi.bmiHeader.biBitCount) {
                case 16:
                    // RGB 555 - High Colour
                    LOG_MODEL_INFO("Screen", "16-bit High Colour");
                    image = QImage(bi.bmiHeader.biWidth, bi.bmiHeader.biHeight, QImage::Format_RGB555);
                    break;
                case 24:
                    LOG_MODEL_INFO("Screen", "24-bit High Colour");
                    image = QImage(bi.bmiHeader.biWidth, bi.bmiHeader.biHeight, QImage::Format_RGB888);
                    break; 
                case 32:
                    // RGB 888 - True Colour
                    LOG_MODEL_INFO("Screen", "32-bit High Colour");
                    image = QImage(bi.bmiHeader.biWidth, bi.bmiHeader.biHeight, QImage::Format_ARGB32);
                    break; 
                default:
                    LOG_MODEL_ERROR("Screen","bits per pixel %u not supported", bi.bmiHeader.biBitCount);
                    break;
                };
                break;
            case BI_BITFIELDS:
                // Default RGB layout
                switch (bi.bmiHeader.biBitCount) {
                case 16:
                    // RGB 555 - High Colour
                    LOG_MODEL_INFO("Screen", "16-bit High Colour");
                    image = QImage(bi.bmiHeader.biWidth, bi.bmiHeader.biHeight, QImage::Format_RGB555);
                    break;
                case 32:
                    // RGB 888 - True Colour
                    LOG_MODEL_INFO("Screen", "32-bit High Colour");
                    image = QImage(bi.bmiHeader.biWidth, bi.bmiHeader.biHeight, QImage::Format_ARGB32);
                    break; 
                default:
                    LOG_MODEL_ERROR("Screen","bits per pixel %u not supported", bi.bmiHeader.biBitCount);
                    break;
                };
                break;
            }
        }
        
        // Get image
        if (!::GetDIBits(dc, bitmap, 0, bi.bmiHeader.biHeight, image.bits(), (BITMAPINFO*)&bi, DIB_RGB_COLORS))
        {
            LOG_MODEL_ERROR("Screen",
                            "Get image fail: %d",
                            GetLastError());
            break;
        }
        
    } while(0);
    
    DeleteObject(bitmap);
    DeleteDC(memDc);
    ReleaseDC(NULL, dc);
    
    return image;
}

