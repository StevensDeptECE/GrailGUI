#include <fcntl.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/v4l2-common.h>
#include <linux/v4l2-controls.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include "opengl/Errcode.hh"
#include "util/Ex.hh"
using namespace std;

void readWebcam(const char deviceName[], const char outputFile[]) {
  int fd = open(deviceName, O_RDWR);  // 1.  Open the device
  if (fd < 0) {
    throw Ex1(Errcode::FILE_NOT_FOUND);
  }

  // 2. Ask the device if it can capture frames
  v4l2_capability capability;
  if (ioctl(fd, VIDIOC_QUERYCAP, &capability) < 0) {
    throw Ex1(Errcode::FILE_NOT_FOUND);
  }

  // 3. Set Image format
  v4l2_format imageFormat;
  imageFormat.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  imageFormat.fmt.pix.width = 1024;
  imageFormat.fmt.pix.height = 1024;
  imageFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
  imageFormat.fmt.pix.field = V4L2_FIELD_NONE;
  // tell the device you are using this format
  if (ioctl(fd, VIDIOC_S_FMT, &imageFormat) < 0) {
    throw Ex1(Errcode::FILE_NOT_FOUND);
  }

  // 4. Request Buffers from the device
  v4l2_requestbuffers requestBuffer = {0};
  requestBuffer.count = 1;  // one request buffer
  requestBuffer.type =
      V4L2_BUF_TYPE_VIDEO_CAPTURE;  // request buf for capturing frames
  requestBuffer.memory = V4L2_MEMORY_MMAP;

  if (ioctl(fd, VIDIOC_REQBUFS, &requestBuffer) < 0) {
    throw Ex1(Errcode::FILE_NOT_FOUND);
  }

  // 5. Query the buffer to get raw data
  v4l2_buffer queryBuffer = {0};
  queryBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  queryBuffer.memory = V4L2_MEMORY_MMAP;
  queryBuffer.index = 0;
  if (ioctl(fd, VIDIOC_QUERYBUF, &queryBuffer) < 0) {
    throw Ex1(Errcode::FILE_NOT_FOUND);
  }
  // use a pointer to point to the newly created buffer
  // mmap() will map the memory address of the device to
  // an address in memory
  char* buffer = (char*)mmap(NULL, queryBuffer.length, PROT_READ | PROT_WRITE,
                             MAP_SHARED, fd, queryBuffer.m.offset);
  memset(buffer, 0, queryBuffer.length);

  // 6. Get a frame
  // Create a new buffer type so the device knows whichbuffer we are talking
  // about
  v4l2_buffer bufferinfo;
  memset(&bufferinfo, 0, sizeof(bufferinfo));
  bufferinfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  bufferinfo.memory = V4L2_MEMORY_MMAP;
  bufferinfo.index = 0;

  // Activate streaming
  int type = bufferinfo.type;
  if (ioctl(fd, VIDIOC_STREAMON, &type) < 0) {
    throw Ex1(Errcode::FILE_NOT_FOUND);
  }

  // Queue the buffer
  if (ioctl(fd, VIDIOC_QBUF, &bufferinfo) < 0) {
    throw Ex1(Errcode::FILE_NOT_FOUND);
  }

  // Dequeue the buffer
  if (ioctl(fd, VIDIOC_DQBUF, &bufferinfo) <
      0) {  // Frames get written after dequeuing the buffer
    cout << "Buffer size=" << bufferinfo.bytesused << '\n';
  }

  ofstream outFile(outputFile, ios::binary | ios::app);
  outFile.write(buffer, bufferinfo.bytesused);
  outFile.close();
  if (ioctl(fd, VIDIOC_STREAMOFF, &type) < 0) {
    throw Ex1(Errcode::FILE_NOT_FOUND);
  }
  close(fd);
}

int main(int argc, char* argv[]) {
  const char* webcam = argc > 1 ? argv[1] : "/dev/video0";
  readWebcam(webcam, "test.jpg");
  return 0;
}
