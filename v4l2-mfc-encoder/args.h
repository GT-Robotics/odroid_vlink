/*
 * mfc codec encoding example application
 * Andrzej Hajda <a.hajda@samsung.com>
 *
 * Argument parser header file.
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

#ifndef ARGS_H
#define ARGS_H

#include "common.h"

#define MAX_CTRLS 100

struct options {
	char *in_name;
	char *mfc_name;
	char *out_name;
	int codec;
	int width;
	int height;
	int duration;
	int rate;
	int nctrls;
	int ctrls[MAX_CTRLS][2];
};

void print_usage(char const *name);
int parse_args(struct options *opts, int argc, char **argv);

#endif
