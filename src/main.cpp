#include <iostream>
#include <algorithm>
#include <vector>
#include <zbar.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

int main(int argc, char** argv) {
    // open v4l2 device
    int fd = open("/dev/video0", O_RDWR);
    if (fd == -1) {
        perror("open /dev/video0");
 return 1;
    }

    // create buffer pool
    v4l2_requestbuffers reqbuf;
    reqbuf.count = 1;
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuf.memory = V4L2_MEMORY_MMAP;
    int ret = ioctl(fd, VIDIOC_REQBUFS, &reqbuf);
    if (ret == -1) {
        perror("VIDIOC_REQBUFS");
        return 1;
    }
 
    // query one buffer
    v4l2_buffer querybuf;
    querybuf.index = 0;
    querybuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    querybuf.memory = V4L2_MEMORY_MMAP;
    ret = ioctl(fd, VIDIOC_QUERYBUF, &querybuf);
    if (ret == -1) {
        perror("VIDIOC_QUERYBUF");
        return 1;
    }
 
    // mmap buffer
    void* bufptr = mmap(NULL, querybuf.length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, querybuf.m.offset);
    if (bufptr == MAP_FAILED) {
        perror("mmap"); 
        return 1;
    }
 
    // read one frame
    ret = read(fd, bufptr, querybuf.length);
    if (ret == -1) {
        perror("read"); 
        return 1;
    }
 
    // save as jpeg
    FILE* fp = fopen("test.jpeg", "wb");
    if (fp == NULL) {
        perror("fopen");
 return 1;
    }
    fwrite(bufptr, querybuf.length, 1, fp);
    fclose(fp);


// // Find and decode barcodes and QR codes
//   // Create zbar scanner
// zbar::ImageScanner scanner;

// // Configure scanner
// scanner.set_config(zbar::ZBAR_QRCODE, zbar::ZBAR_CFG_ENABLE, 1);

// // Convert image to grayscale
// Mat imGray;
// cvtColor(buf, imGray, COLOR_BGR2GRAY);

// // Wrap image data in a zbar image
// zbar::Image image(buf.cols, buf.rows, "Y800", (uchar *)imGray.data, buf.length);

// // Scan the image for barcodes and QRCodes
// scanner.scan(image);

// // Print results
// for(zbar::Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
// {
//   std::string data;

//   data = symbol->get_data();

//   // Print type and data
//   std::cout << std::endl << data << std::endl << std::endl;
// }

// // Cleanup
//     munmap(frame, buffer.length);
//     close(fd);

    return 0;
}