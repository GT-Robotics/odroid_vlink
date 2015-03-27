/*
 * mfc codec encoding example application
 * Andrzej Hajda <a.hajda@samsung.com>
 *
 * Input camera device.
 *
 * Copyright 2012 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "common.h"
#include "in_demo.h"
#include "v4l_dev.h"

#define SQUARE_SIZE 5

static struct io_dev_ops in_camera_ops = { .req_bufs = v4l_req_bufs,
					   .enq_buf = v4l_enq_buf,
					   .deq_buf = v4l_deq_buf,
					   .destroy = v4l_destroy
				};

int in_camera_set_autofocus(struct io_dev *dev, int val)
{
	struct v4l2_ext_control ctrl;
	struct v4l2_ext_controls ctrls;
	int ret;

	ctrl.id = V4L2_CID_FOCUS_AUTO;
	ctrl.value = val;

	ctrls.ctrl_class = V4L2_CID_CAMERA_CLASS;
	ctrls.count = 1;
	ctrls.controls = &ctrl;

	ret = ioctl(dev->fd, VIDIOC_S_EXT_CTRLS, &ctrls);
	if (ret < 0)
		err("Cannot set autofocus on %d", dev->fd);

	return ret;
}

struct io_dev *in_camera_create(char const *name)
{
	struct io_dev *dev;

	dev = malloc(sizeof(*dev));
	memzero(*dev);

	dev->fd = open(name, O_RDWR, 0);
	if (dev->fd < 0) {
		free(dev);
		err("Cannot open camera(%s) device", name);
		return NULL;
	}

	dev->io[0].type = IO_NONE;
	dev->io[1].type = IO_USERPTR;

	dev->ops = &in_camera_ops;

	in_camera_set_autofocus(dev, 1);

	return dev;
}
