#include <iostream>
#include <algorithm>
#include <vector>
#include <zbar.h>


#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;


int main()
{

// Read image
Mat im = imread("QRCode.png");

// Find and decode barcodes and QR codes
  // Create zbar scanner
zbar::ImageScanner scanner;

// Configure scanner
scanner.set_config(zbar::ZBAR_QRCODE, zbar::ZBAR_CFG_ENABLE, 1);

// Convert image to grayscale
Mat imGray;
cvtColor(im, imGray, COLOR_BGR2GRAY);

// Wrap image data in a zbar image
zbar::Image image(im.cols, im.rows, "Y800", (uchar *)imGray.data, im.cols * im.rows);

// Scan the image for barcodes and QRCodes
scanner.scan(image);

// Print results
for(zbar::Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
{
  std::string data;

  data = symbol->get_data();

  // Print type and data
  std::cout << std::endl << data << std::endl << std::endl;
}
  return 0;
}