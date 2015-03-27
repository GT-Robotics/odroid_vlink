# V4L2 Codec encoding example application
# Andrzej Hajda <a.hajda@samsung.com>
#
# Copyright 2012 Samsung Electronics Co., Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# Toolchain path
TCPATH = arm-linux-gnueabihf-
KERNELHEADERS = /root/kernel/headers/include

CC = ${TCPATH}gcc
AR = "${TCPATH}ar rc"
AR2 = ${TCPATH}ranlib make -j4


INCLUDES = -I$(KERNELHEADERS)

SOURCES = main.c args.c in_demo.c out_file.c mfc.c io_dev.c func_dev.c v4l_dev.c in_camera.c
OBJECTS := $(SOURCES:.c=.o)
EXEC = mfc-encode
CFLAGS = -Wall -g -DS5PC1XX_FIMC
DEP = .dep

all: $(EXEC) $(DEP)

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) $<

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJECTS) -lm -lrt

clean:
	rm -f *.o $(EXEC) $(DEP)

$(DEP): $(SOURCES) Makefile
	$(CC) -c $(CFLAGS) $(INCLUDES) -MM $(SOURCES) > $(DEP)

.PHONY: clean all

-include $(DEP)
