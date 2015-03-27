V4L2 Codec encoding example application
by Andrzej Hajda <a.hajda@samsung.com>

===========
* License *
===========
Copyright 2012 Samsung Electronics Co., Ltd.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=============
* About MFC *
=============

MFC stands for Multi Format Codec. It is a hardware module available in the
Exynos family of SoCs. It is capable of decoding a range of compressed
streams with resolution up to 1080p at 30 fps. This includes H264, H264,
MPEG1, MPEG2, MPEG4 and VC1. It also supports encoding in H264 and H263 format
up to 1080p and MPEG up to D1 resolution.

==============
* About FIMC *
==============

Although this is the example application for MFC it also uses the FIMC camera
module.

=======================
* About the interface *
=======================

The interface used by the MFC 5.1 driver is the Video4Linux2 framework.
It acts as a memory to memory device. It supports up to 16 contexts, each of
them can be setup to do encoding or decoding.

=========================
* About the application *
=========================

The purpose of this applications is to present and explain the usage of hardware
video codecs with the V4L2 interface. Application also presents usage of poll
interface with V4L2 devices as an alternative to threads.

=================================
* Encoding in a few words *
=================================

MFC codec is a memory to memory V4L2 device. Following steps should be performed
to encode video stream:
1. Open device.
2. Set image size and format on OUTPUT. MFC supports V4L2_PIX_FMT_NV12M format
   with bytes per line aligned to 128 and image size aligned to 2048.
3. Set codec and buffer size on CAPTURE. Size should be not greater than 2MB.
   Supported codecs: MPEG4, H263, H264.
4. Optionally set additional parameters (bitrate, fps,....).
5. Request CAPTURE and OUTPUT buffers. Due to hardware limitations of MFC on
   some platforms it is recommended to use V4L2_MEMORY_MMAP buffers.
6. Enqueue CAPTURE buffers.
7. Enqueue OUTPUT buffer with first frame.
8. Start streaming (VIDIOC_STREAMON) on both ends.
9. Simultaneously:
   - enqueue buffers with next frames,
   - dequeue used OUTPUT buffers (blocking operation),
   - dequeue buffers with encoded stream (blocking operation),
   - enqueue free CAPTURE buffers.
10. Last frame should have set flags field to V4L2_BUF_FLAG_LAST to
    inform codec about the end of the stream.
11. Stop dequeuing CAPTURE frames after dequeuing frame with luma plane
    bytesused equal 0.
12. Stop streaming (VIDIOC_STREAMOFF) on both ends.
13. Close device.

Encoded stream is in elementary stream format.

==============================
* Application in a few words *
==============================

The application uses pattern of device chain. Single device has input and output.
Chained devices have connected output of one device with input of the following
device. Devices uses buffers to pass data between them.

Video4Linux2 devices uses this pattern natively. The application uses also this
pattern for pseudo devices:
  in_demo - input device providing stream of NV12M frames with animation of
            white square flying over 'noisy background'.
  out_file - writes incoming data to file.

Application uses following strategy regarding queuing buffers:
1. At the beginning all buffers are enqueued in output queues of devices.
2. Every time buffer is dequeued from out queue it is immediately enqueued
   in the input queue of the next device.
3. Every time buffer is dequeued from input queue it is immediately enqueued
   in the output queue of the previous device.

Application uses poll interface to wait for devices which can be dequeued
without blocking. When device is ready dequeue/enqueue operation is performed.

There are two types of devices:
1. V4L2 devices:
   - mfc,
   - in_camera.
2. Function devices - they use read or write function to pass data:
   - in_demo,
   - out_file.

Implementation of devices: mfc.c, in_camera.c, in.demo.c out_file.c shows that
extending application with new devices should be quite easy.

===========================
* Running the application *
===========================

The application takes a few necessary arguments. The only required argument is
MFC device (-m).

Options:
        -i <device>   - FIMC camera device (e.g. /dev/video1)
                        If not specified demo input device is used
        -m <device>   - (required) MFC device (e.g. /dev/video8)
        -o <file>     - Output file name
        -c <codec>    - The codec of the encoded stream
                        Available codecs: mpeg4, h263, h264
        -d <duration> - Number of frames to encode
        -r <rate>     - Frame rate
        -b <bitrate>  - Bitrate
        -s <size>     - Size of frame in format WxH

To determine which devices to use you can try the following commands.

For MFC:
dmesg | grep -e s5p-mfc.*encoder.*/dev/video
which outputs:
[    2.160683] s5p-mfc s5p-mfc: encoder registered as /dev/video9

For FIMC camera:
dmesg | grep -e fimc...capture.*/dev/video
which outputs:
[    2.484000] s5p-fimc-md: Registered exynos4-fimc.0.capture as /dev/video1
[    2.497174] s5p-fimc-md: Registered exynos4-fimc.1.capture as /dev/video3
[    2.510368] s5p-fimc-md: Registered exynos4-fimc.2.capture as /dev/video5
[    2.523578] s5p-fimc-md: Registered exynos4-fimc.3.capture as /dev/video7

