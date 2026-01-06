//
// AsciiArt.cpp : Convert black and white jpeg/bmps to ascii art using '$', 'o', and '''
//

#include "conio.h"
#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include <fstream>

#pragma comment(lib,"gdiplus.lib")

using namespace std;
using namespace Gdiplus;

#define BLOCKSIZE_X		(6)
#define BLOCKSIZE_Y		((BLOCKSIZE_X*3)/2)
#define DARKTHRESH		(BLOCKSIZE_X*BLOCKSIZE_Y*3/4)
#define MEDIUMTHRESH	(BLOCKSIZE_X*BLOCKSIZE_Y*2/4)
#define LIGHTTHRESH		(BLOCKSIZE_X*BLOCKSIZE_Y*1/4)

int main(int argc, char* argv[])
{
	char fileName[256] = "c3_2.jpg";
	WCHAR wczFileName[1024] = L"";

	cout << "Enter the file name: ";
	cin >> fileName;

	cout << "\nConverting " << fileName << "...";

	// convert file name to widechar format
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, fileName, -1, wczFileName, 1024);

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	{
		Bitmap image(wczFileName);
		Color pixelColor;
		int imageHeight = image.GetHeight();
		int imageWidth = image.GetWidth();

		cout << '\n';
		// Loop through the image N x M bits at a time and match
		// and output the ascii character
		for (int y = 0; y < imageHeight / BLOCKSIZE_Y; y++) {
			for (int x = 0; x < imageWidth / BLOCKSIZE_X; x++) {

				int blackPixelCount = 0;

				for (int k = 0; k < BLOCKSIZE_X; k++) {
					for (int l = 0; l < BLOCKSIZE_Y; l++) {

						// Find the average black pixel density.
						image.GetPixel((x * BLOCKSIZE_X) + k, (y * BLOCKSIZE_Y) + l, &pixelColor);
						int pixRColor = pixelColor.GetR();
						if (pixRColor < 128) {
							blackPixelCount++;
						}
					}
				}

				if (blackPixelCount >= DARKTHRESH) {
					cout << '$';
				}
				else if (blackPixelCount >= MEDIUMTHRESH) {
					cout << 'o';
				}
				else if (blackPixelCount >= LIGHTTHRESH) {
					cout << '\'';
				}
				else {
					cout << ' ';
				}
			}
			// Move to next row...
			cout << "\n";
		}
	}

	GdiplusShutdown(gdiplusToken);

	return 0;
}
