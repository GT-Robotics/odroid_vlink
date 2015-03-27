/*
 * mfc codec encoding example application
 * Andrzej Hajda <a.hajda@samsung.com>
 *
 * Output file device.
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
#include <unistd.h>
#include <fcntl.h>

#include "common.h"
#include "out_file.h"
#include "func_dev.h"

int out_file_write(struct io_dev *dev, int nbufs, char **bufs, int *lens)
{
	return write(dev->fd, bufs[0], lens[0]);
}

static struct io_dev_ops out_file_ops = { .write = out_file_write,
					 .req_bufs = func_req_bufs,
					 .enq_buf = func_enq_buf,
					 .deq_buf = func_deq_buf,
					 .destroy = func_destroy
					};

struct io_dev *out_file_create(char const *name)
{
	struct io_dev *dev;

	dev = malloc(sizeof(*dev));
	memzero(*dev);

	dev->fd = open(name, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	dev->io[DIR_IN].type = IO_FUNC;
	dev->io[DIR_OUT].type = IO_NONE;
	dev->ops = &out_file_ops;

	return dev;
}
