/*
 * mfc codec encoding example application
 * Andrzej Hajda <a.hajda@samsung.com>
 *
 * Sample input device.
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

#include "common.h"
#include "in_demo.h"
#include "func_dev.h"

#define SQUARE_SIZE 5
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

struct in_demo_priv {
	int width;
	int height;
};

static int in_demo_read(struct io_dev *dev, int nbufs, char **bufs, int *lens)
{
	struct in_demo_priv *p;
	int x, y;
	int i, j;
	int size;
	double rx, ry;
	int t;

	p = dev->priv;

	if (nbufs != 2)
		return -1;

	t = dev->io[DIR_OUT].counter;

	size = p->width * p->height;
	if (size > lens[0] || size > 2 * lens[1]) {
		err("Size=%d len=%d,%d", size, lens[0], lens[1]);
		return -1;
	}

	memset(bufs[0], 0, size);
	memset(bufs[1], 128, size / 2);

	rx = cos(7 * t / 3.14 / 25 * 100 / p->width);
	ry = sin(6 * t / 3.14 / 25 * 100 / p->width);

	x = (rx + 1) / 2 * (p->width - 2 * SQUARE_SIZE) + SQUARE_SIZE;
	y = (ry + 1) / 2 * (p->height - 2 * SQUARE_SIZE) + SQUARE_SIZE;

	for (i = MIN(SQUARE_SIZE, p->width) - 1; i >= 0; --i)
		for (j = MIN(SQUARE_SIZE, p->height) - 1; j >= 0; --j)
			bufs[0][x + i + (y + j) * p->width] = 255;

	return size;
}

static int in_demo_destroy(struct io_dev *dev)
{
	free(dev->priv);

	return func_destroy(dev);
}

static struct io_dev_ops in_demo_ops = { .read = in_demo_read,
					 .req_bufs = func_req_bufs,
					 .enq_buf = func_enq_buf,
					 .deq_buf = func_deq_buf,
					 .destroy = in_demo_destroy
					};

struct io_dev *in_demo_create(int width, int height)
{
	struct io_dev *dev;
	struct in_demo_priv *priv;

	dev = malloc(sizeof(*dev));
	memzero(*dev);

	priv = malloc(sizeof(struct in_demo_priv));
	priv->width = width;
	priv->height = height;

	dev->fd = -1;
	dev->io[DIR_IN].type = IO_NONE;
	dev->io[DIR_OUT].type = IO_FUNC;
	dev->ops = &in_demo_ops;

	dev->priv = priv;

	return dev;
}
