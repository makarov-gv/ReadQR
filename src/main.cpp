#include <iostream>
#include <algorithm>
#include <vector>
#include <zbar.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main()
{
  zbar::ImageScanner scanner;
  scanner.set_config(zbar::ZBAR_QRCODE, zbar::ZBAR_CFG_ENABLE, 1);

  cv::Mat im = cv::imread("QRCode.png");
  cv::Mat imGray;
  cv::cvtColor(im, imGray, cv::COLOR_BGR2GRAY);

  zbar::Image image(im.cols, im.rows, "Y800", (uchar *)imGray.data, im.cols * im.rows);
  scanner.scan(image);

  for(zbar::Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
  {
    std::string data;
    data = symbol->get_data();
    std::cout << std::endl << data << std::endl << std::endl;
  }
  return 0;
}