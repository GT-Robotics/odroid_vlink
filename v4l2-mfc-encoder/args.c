/*
 * mfc codec encoding example application
 * Andrzej Hajda <a.hajda@samsung.com>
 *
 * Command line helper functions.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/videodev2.h>
#include <stddef.h>

#include "common.h"
#include "args.h"

struct {
	int id;
	char *name;
} ctrls[] = {
	{ V4L2_CID_MPEG_MFC51_VIDEO_FORCE_FRAME_TYPE, "force_frame"},
	{ V4L2_CID_MPEG_MFC51_VIDEO_FRAME_SKIP_MODE, "frame_skip_mode"},
	{ V4L2_CID_MPEG_MFC51_VIDEO_H264_ADAPTIVE_RC_ACTIVITY, "h264_arc_activity"},
	{ V4L2_CID_MPEG_MFC51_VIDEO_H264_ADAPTIVE_RC_DARK, "h264_arc_dark"},
	{ V4L2_CID_MPEG_MFC51_VIDEO_H264_ADAPTIVE_RC_SMOOTH, "h264_arc_smooth"},
	{ V4L2_CID_MPEG_MFC51_VIDEO_H264_ADAPTIVE_RC_STATIC, "h264_arc_static"},
	{ V4L2_CID_MPEG_MFC51_VIDEO_H264_NUM_REF_PIC_FOR_P, "h264_ref_pic"},
	{ V4L2_CID_MPEG_MFC51_VIDEO_PADDING_YUV, "padding_yuv"},
	{ V4L2_CID_MPEG_MFC51_VIDEO_PADDING, "padding"},
	{ V4L2_CID_MPEG_MFC51_VIDEO_RC_FIXED_TARGET_BIT, "rc_fixed_target_bit"},
	{ V4L2_CID_MPEG_MFC51_VIDEO_RC_REACTION_COEFF, "rc_react_coeff"},
	{ V4L2_CID_MPEG_VIDEO_B_FRAMES, "b_frames"},
	{ V4L2_CID_MPEG_VIDEO_BITRATE, "bitrate"},
	{ V4L2_CID_MPEG_VIDEO_CYCLIC_INTRA_REFRESH_MB, "intra_refresh_mb"},
	{ V4L2_CID_MPEG_VIDEO_FRAME_RC_ENABLE, "rc_enable"},
	{ V4L2_CID_MPEG_VIDEO_GOP_CLOSURE, "gop_closure"},
	{ V4L2_CID_MPEG_VIDEO_GOP_SIZE, "gop_size"},
	{ V4L2_CID_MPEG_VIDEO_H263_B_FRAME_QP, "h263_b_qp"},
	{ V4L2_CID_MPEG_VIDEO_H263_I_FRAME_QP, "h263_i_qp"},
	{ V4L2_CID_MPEG_VIDEO_H263_MAX_QP, "h263_max_qp"},
	{ V4L2_CID_MPEG_VIDEO_H263_MIN_QP, "h263_min_qp"},
	{ V4L2_CID_MPEG_VIDEO_H263_P_FRAME_QP, "h263_p_qp"},
	{ V4L2_CID_MPEG_VIDEO_H264_8X8_TRANSFORM, "h264_8x8_transform"},
	{ V4L2_CID_MPEG_VIDEO_H264_B_FRAME_QP, "h264_b_qp"},
	{ V4L2_CID_MPEG_VIDEO_H264_CPB_SIZE, "h264_cpb_size"},
	{ V4L2_CID_MPEG_VIDEO_H264_ENTROPY_MODE, "h264_entropy"},
	{ V4L2_CID_MPEG_VIDEO_H264_I_FRAME_QP, "h264_i_qp"},
	{ V4L2_CID_MPEG_VIDEO_H264_I_PERIOD, "h264_i_period"},
	{ V4L2_CID_MPEG_VIDEO_H264_LEVEL, "h264_level"},
	{ V4L2_CID_MPEG_VIDEO_H264_LOOP_FILTER_ALPHA, "h264_loop_filter_alpha"},
	{ V4L2_CID_MPEG_VIDEO_H264_LOOP_FILTER_BETA, "h264_loop_filter_beta"},
	{ V4L2_CID_MPEG_VIDEO_H264_LOOP_FILTER_MODE, "h264_loop_filter_mode"},
	{ V4L2_CID_MPEG_VIDEO_H264_MAX_QP, "h264_max_qp"},
	{ V4L2_CID_MPEG_VIDEO_H264_MIN_QP, "h264_min_qp"},
	{ V4L2_CID_MPEG_VIDEO_H264_P_FRAME_QP, "h264_p_qp"},
	{ V4L2_CID_MPEG_VIDEO_H264_PROFILE, "h264_profile"},
	{ V4L2_CID_MPEG_VIDEO_H264_VUI_EXT_SAR_HEIGHT, "h264_vui_sar_height"},
	{ V4L2_CID_MPEG_VIDEO_H264_VUI_EXT_SAR_WIDTH, "h264_vui_sar_width"},
	{ V4L2_CID_MPEG_VIDEO_H264_VUI_SAR_ENABLE, "h264_vui_sar"},
	{ V4L2_CID_MPEG_VIDEO_H264_VUI_SAR_IDC, "h264_vui_sar_idc"},
	{ V4L2_CID_MPEG_VIDEO_HEADER_MODE, "header_mode"},
	{ V4L2_CID_MPEG_VIDEO_MB_RC_ENABLE, "mb_rc"},
	{ V4L2_CID_MPEG_VIDEO_MPEG4_B_FRAME_QP, "mpeg4_b_qp"},
	{ V4L2_CID_MPEG_VIDEO_MPEG4_I_FRAME_QP, "mpeg4_i_qp"},
	{ V4L2_CID_MPEG_VIDEO_MPEG4_LEVEL, "mpeg4_level"},
	{ V4L2_CID_MPEG_VIDEO_MPEG4_MAX_QP, "mpeg4_max_qp"},
	{ V4L2_CID_MPEG_VIDEO_MPEG4_MIN_QP, "mpeg4_min_qp"},
	{ V4L2_CID_MPEG_VIDEO_MPEG4_P_FRAME_QP, "mpeg4_p_qp"},
	{ V4L2_CID_MPEG_VIDEO_MPEG4_PROFILE, "mpeg4_profile"},
	{ V4L2_CID_MPEG_VIDEO_MPEG4_QPEL, "mpeg4_qpel"},
	{ V4L2_CID_MPEG_VIDEO_MULTI_SLICE_MAX_BYTES, "slice_max_bytes"},
	{ V4L2_CID_MPEG_VIDEO_MULTI_SLICE_MAX_MB, "slice_max_mb"},
	{ V4L2_CID_MPEG_VIDEO_MULTI_SLICE_MODE, "slice_mode"},
	{ V4L2_CID_MPEG_VIDEO_VBV_SIZE, "vbv_size"},
};

void print_usage(char const *name)
{
	int i;
	printf("Usage: %s [args]\n"
	       "\t-i <device>   - FIMC camera device (e.g. /dev/video1)\n"
	       "\t                If not specified demo input device is used\n"
	       "\t-m <device>   - (required) MFC device (e.g. /dev/video8)\n"
	       "\t-o <file>     - Output file name\n"
	       "\t-c <codec>[,param[=val]]...\n"
	       "\t              - The codec of the encoded stream optionally\n"
	       "\t                followed by comma separated parameters.\n"
	       "\t                Available codecs: mpeg4, h263, h264\n"
	       "\t-d <duration> - Number of frames to encode\n"
	       "\t-r <rate>     - Frame rate\n"
	       "\t-s <size>     - Size of frame in format WxH\n"
	       "Codec parameters:\n"
		, name);

	for (i = 0; i < array_len(ctrls); ++i) {
		printf("\t%s\n", ctrls[i].name);
	}
}

int get_codec(char *str)
{
	if (strncasecmp("mpeg4", str, 5) == 0)
		return V4L2_PIX_FMT_MPEG4;
	else if (strncasecmp("h263", str, 5) == 0)
		return V4L2_PIX_FMT_H263;
	else if (strncasecmp("h264", str, 5) == 0)
		return V4L2_PIX_FMT_H264;

	return 0;
}

void set_options_default(struct options *o)
{
	memset(o, 0, sizeof(*o));
	o->width = 176;
	o->height = 144;
	o->duration = 250;
	o->rate = 25;
	o->out_name = "demo.out";
	o->codec = V4L2_PIX_FMT_H264;
}

int parse_args(struct options *opts, int argc, char **argv)
{
	static const int codecs[] = {
		V4L2_PIX_FMT_MPEG4, V4L2_PIX_FMT_H263, V4L2_PIX_FMT_H264 };
	const int nctrls = array_len(ctrls);
	char *tokens[nctrls + 4];
	char *s, *v;
	int c, i;

	set_options_default(opts);

	for (i = 0; i < nctrls; ++i)
		tokens[i] = ctrls[i].name;
	tokens[i++] = "mpeg4";
	tokens[i++] = "h263";
	tokens[i++] = "h264";
	tokens[i++] = NULL;

	while ((c = getopt(argc, argv, "i:m:o:c:d:r:s:b:")) != -1) {
		switch (c) {
		case 'i':
			opts->in_name = optarg;
			break;
		case 'm':
			opts->mfc_name = optarg;
			break;
		case 'o':
			opts->out_name = optarg;
			break;
		case 'c':
			s = optarg;

			while (*s) {
				c = getsubopt(&s, tokens, &v);
				if (c < 0) {
					err("unknown codec option '%s'", v);
					return -1;
				} else if (c < nctrls) {
					int *ctl = opts->ctrls[opts->nctrls++];
					if (opts->nctrls > MAX_CTRLS) {
						err("Too many codec options");
						return -1;
					}
					ctl[0] = ctrls[c].id;
					ctl[1] = v ? atoi(v) : 1;
					dbg("opt %s=%d", ctrls[c].name, ctl[1]);
				} else {
					dbg("codec: %.04s",
					    (char *)&codecs[c - nctrls]);
					opts->codec = codecs[c - nctrls];
				}
			};
			break;
		case 'd':
			opts->duration = atoi(optarg);
			break;
		case 'r':
			opts->rate = atoi(optarg);
			break;
		case 's': {
			char *sep = NULL;
			opts->width = strtol(optarg, &sep, 10);
			if (!sep || *sep != 'x') {
				err("Bad size, should be like 320x200");
				return -1;
			}
			opts->height = atoi(++sep);
			break;
		}
		default:
			return -1;
		}
	}

	if (opts->mfc_name == NULL) {
		err("Please provide MFC device");
		return -1;
	}

	return 0;
}

