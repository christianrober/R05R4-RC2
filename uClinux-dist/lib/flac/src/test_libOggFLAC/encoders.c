/* test_libOggFLAC - Unit tester for libOggFLAC
 * Copyright (C) 2002,2003  Josh Coalson
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "encoders.h"
#include "file_utils.h"
#include "metadata_utils.h"
#include "FLAC/assert.h"
#include "OggFLAC/stream_encoder.h"
#include "share/grabbag.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FLAC__StreamMetadata streaminfo_, padding_, seektable_, application1_, application2_, vorbiscomment_, cuesheet_, unknown_;
static FLAC__StreamMetadata *metadata_sequence_[] = { &padding_, &seektable_, &application1_, &application2_, &vorbiscomment_, &cuesheet_, &unknown_ };
static const unsigned num_metadata_ = sizeof(metadata_sequence_) / sizeof(metadata_sequence_[0]);
static const char *oggflacfilename_ = "metadata.ogg";

static FLAC__bool die_s_(const char *msg, const OggFLAC__StreamEncoder *encoder)
{
	OggFLAC__StreamEncoderState state = OggFLAC__stream_encoder_get_state(encoder);

	if(msg)
		printf("FAILED, %s", msg);
	else
		printf("FAILED");

	printf(", state = %u (%s)\n", (unsigned)state, OggFLAC__StreamEncoderStateString[state]);
	if(state == OggFLAC__STREAM_ENCODER_FLAC_STREAM_ENCODER_ERROR) {
		FLAC__StreamEncoderState state_ = OggFLAC__stream_encoder_get_FLAC_stream_encoder_state(encoder);
		printf("      FLAC stream encoder state = %u (%s)\n", (unsigned)state_, FLAC__StreamEncoderStateString[state_]);
		if(state_ == FLAC__STREAM_ENCODER_VERIFY_DECODER_ERROR) {
			FLAC__StreamDecoderState dstate = OggFLAC__stream_encoder_get_verify_decoder_state(encoder);
			printf("      verify decoder state = %u (%s)\n", (unsigned)dstate, FLAC__StreamDecoderStateString[dstate]);
		}
	}

	return false;
}

static void init_metadata_blocks_()
{
	mutils__init_metadata_blocks(&streaminfo_, &padding_, &seektable_, &application1_, &application2_, &vorbiscomment_, &cuesheet_, &unknown_);
}

static void free_metadata_blocks_()
{
	mutils__free_metadata_blocks(&streaminfo_, &padding_, &seektable_, &application1_, &application2_, &vorbiscomment_, &cuesheet_, &unknown_);
}

static FLAC__StreamEncoderWriteStatus stream_encoder_write_callback_(const OggFLAC__StreamEncoder *encoder, const FLAC__byte buffer[], unsigned bytes, unsigned samples, unsigned current_frame, void *client_data)
{
	(void)encoder, (void)buffer, (void)bytes, (void)samples, (void)current_frame, (void)client_data;
	return FLAC__STREAM_ENCODER_WRITE_STATUS_OK;
}

static FLAC__bool test_stream_encoder()
{
	OggFLAC__StreamEncoder *encoder;
	OggFLAC__StreamEncoderState state;
	FLAC__StreamEncoderState state_;
	FLAC__StreamDecoderState dstate;
	FLAC__int32 samples[1024];
	FLAC__int32 *samples_array[1] = { samples };
	unsigned i;

	printf("\n+++ libOggFLAC unit test: OggFLAC__StreamEncoder\n\n");

	printf("testing OggFLAC__stream_encoder_new()... ");
	encoder = OggFLAC__stream_encoder_new();
	if(0 == encoder) {
		printf("FAILED, returned NULL\n");
		return false;
	}
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_set_serial_number()... ");
	if(!OggFLAC__stream_encoder_set_serial_number(encoder, file_utils__serial_number))
		return die_s_("returned false", encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_set_verify()... ");
	if(!OggFLAC__stream_encoder_set_verify(encoder, true))
		return die_s_("returned false", encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_set_streamable_subset()... ");
	if(!OggFLAC__stream_encoder_set_streamable_subset(encoder, true))
		return die_s_("returned false", encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_set_do_mid_side_stereo()... ");
	if(!OggFLAC__stream_encoder_set_do_mid_side_stereo(encoder, false))
		return die_s_("returned false", encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_set_loose_mid_side_stereo()... ");
	if(!OggFLAC__stream_encoder_set_loose_mid_side_stereo(encoder, false))
		return die_s_("returned false", encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_set_channels()... ");
	if(!OggFLAC__stream_encoder_set_channels(encoder, streaminfo_.data.stream_info.channels))
		return die_s_("returned false", encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_set_bits_per_sample()... ");
	if(!OggFLAC__stream_encoder_set_bits_per_sample(encoder, streaminfo_.data.stream_info.bits_per_sample))
		return die_s_("returned false", encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_set_sample_rate()... ");
	if(!OggFLAC__stream_encoder_set_sample_rate(encoder, streaminfo_.data.stream_info.sample_rate))
		return die_s_("returned false", encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_set_blocksize()... ");
	if(!OggFLAC__stream_encoder_set_blocksize(encoder, streaminfo_.data.stream_info.min_blocksize))
		return die_s_("returned false", encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_set_max_lpc_order()... ");
	if(!OggFLAC__stream_encoder_set_max_lpc_order(encoder, 0))
		return die_s_("returned false", encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_set_qlp_coeff_precision()... ");
	if(!OggFLAC__stream_encoder_set_qlp_coeff_precision(encoder, 0))
		return die_s_("returned false", encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_set_do_qlp_coeff_prec_search()... ");
	if(!OggFLAC__stream_encoder_set_do_qlp_coeff_prec_search(encoder, false))
		return die_s_("returned false", encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_set_do_escape_coding()... ");
	if(!OggFLAC__stream_encoder_set_do_escape_coding(encoder, false))
		return die_s_("returned false", encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_set_do_exhaustive_model_search()... ");
	if(!OggFLAC__stream_encoder_set_do_exhaustive_model_search(encoder, false))
		return die_s_("returned false", encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_set_min_residual_partition_order()... ");
	if(!OggFLAC__stream_encoder_set_min_residual_partition_order(encoder, 0))
		return die_s_("returned false", encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_set_max_residual_partition_order()... ");
	if(!OggFLAC__stream_encoder_set_max_residual_partition_order(encoder, 0))
		return die_s_("returned false", encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_set_rice_parameter_search_dist()... ");
	if(!OggFLAC__stream_encoder_set_rice_parameter_search_dist(encoder, 0))
		return die_s_("returned false", encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_set_total_samples_estimate()... ");
	if(!OggFLAC__stream_encoder_set_total_samples_estimate(encoder, streaminfo_.data.stream_info.total_samples))
		return die_s_("returned false", encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_set_metadata()... ");
	if(!OggFLAC__stream_encoder_set_metadata(encoder, metadata_sequence_, num_metadata_))
		return die_s_("returned false", encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_set_write_callback()... ");
	if(!OggFLAC__stream_encoder_set_write_callback(encoder, stream_encoder_write_callback_))
		return die_s_("returned false", encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_set_client_data()... ");
	if(!OggFLAC__stream_encoder_set_client_data(encoder, 0))
		return die_s_("returned false", encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_init()... ");
	if(OggFLAC__stream_encoder_init(encoder) != FLAC__STREAM_ENCODER_OK)
		return die_s_(0, encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_get_state()... ");
	state = OggFLAC__stream_encoder_get_state(encoder);
	printf("returned state = %u (%s)... OK\n", (unsigned)state, OggFLAC__StreamEncoderStateString[state]);

	printf("testing OggFLAC__stream_encoder_get_FLAC_stream_encoder_state()... ");
	state_ = OggFLAC__stream_encoder_get_FLAC_stream_encoder_state(encoder);
	printf("returned state = %u (%s)... OK\n", (unsigned)state_, FLAC__StreamEncoderStateString[state_]);

	printf("testing OggFLAC__stream_encoder_get_verify_decoder_state()... ");
	dstate = OggFLAC__stream_encoder_get_verify_decoder_state(encoder);
	printf("returned state = %u (%s)... OK\n", (unsigned)dstate, FLAC__StreamDecoderStateString[dstate]);

	{
		FLAC__uint64 absolute_sample;
		unsigned frame_number;
		unsigned channel;
		unsigned sample;
		FLAC__int32 expected;
		FLAC__int32 got;

		printf("testing OggFLAC__stream_encoder_get_verify_decoder_error_stats()... ");
		OggFLAC__stream_encoder_get_verify_decoder_error_stats(encoder, &absolute_sample, &frame_number, &channel, &sample, &expected, &got);
		printf("OK\n");
	}

	printf("testing OggFLAC__stream_encoder_get_verify()... ");
	if(OggFLAC__stream_encoder_get_verify(encoder) != true) {
		printf("FAILED, expected true, got false\n");
		return false;
	}
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_get_streamable_subset()... ");
	if(OggFLAC__stream_encoder_get_streamable_subset(encoder) != true) {
		printf("FAILED, expected true, got false\n");
		return false;
	}
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_get_do_mid_side_stereo()... ");
	if(OggFLAC__stream_encoder_get_do_mid_side_stereo(encoder) != false) {
		printf("FAILED, expected false, got true\n");
		return false;
	}
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_get_loose_mid_side_stereo()... ");
	if(OggFLAC__stream_encoder_get_loose_mid_side_stereo(encoder) != false) {
		printf("FAILED, expected false, got true\n");
		return false;
	}
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_get_channels()... ");
	if(OggFLAC__stream_encoder_get_channels(encoder) != streaminfo_.data.stream_info.channels) {
		printf("FAILED, expected %u, got %u\n", streaminfo_.data.stream_info.channels, OggFLAC__stream_encoder_get_channels(encoder));
		return false;
	}
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_get_bits_per_sample()... ");
	if(OggFLAC__stream_encoder_get_bits_per_sample(encoder) != streaminfo_.data.stream_info.bits_per_sample) {
		printf("FAILED, expected %u, got %u\n", streaminfo_.data.stream_info.bits_per_sample, OggFLAC__stream_encoder_get_bits_per_sample(encoder));
		return false;
	}
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_get_sample_rate()... ");
	if(OggFLAC__stream_encoder_get_sample_rate(encoder) != streaminfo_.data.stream_info.sample_rate) {
		printf("FAILED, expected %u, got %u\n", streaminfo_.data.stream_info.sample_rate, OggFLAC__stream_encoder_get_sample_rate(encoder));
		return false;
	}
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_get_blocksize()... ");
	if(OggFLAC__stream_encoder_get_blocksize(encoder) != streaminfo_.data.stream_info.min_blocksize) {
		printf("FAILED, expected %u, got %u\n", streaminfo_.data.stream_info.min_blocksize, OggFLAC__stream_encoder_get_blocksize(encoder));
		return false;
	}
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_get_max_lpc_order()... ");
	if(OggFLAC__stream_encoder_get_max_lpc_order(encoder) != 0) {
		printf("FAILED, expected %u, got %u\n", 0, OggFLAC__stream_encoder_get_max_lpc_order(encoder));
		return false;
	}
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_get_qlp_coeff_precision()... ");
	(void)OggFLAC__stream_encoder_get_qlp_coeff_precision(encoder);
	/* we asked the encoder to auto select this so we accept anything */
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_get_do_qlp_coeff_prec_search()... ");
	if(OggFLAC__stream_encoder_get_do_qlp_coeff_prec_search(encoder) != false) {
		printf("FAILED, expected false, got true\n");
		return false;
	}
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_get_do_escape_coding()... ");
	if(OggFLAC__stream_encoder_get_do_escape_coding(encoder) != false) {
		printf("FAILED, expected false, got true\n");
		return false;
	}
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_get_do_exhaustive_model_search()... ");
	if(OggFLAC__stream_encoder_get_do_exhaustive_model_search(encoder) != false) {
		printf("FAILED, expected false, got true\n");
		return false;
	}
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_get_min_residual_partition_order()... ");
	if(OggFLAC__stream_encoder_get_min_residual_partition_order(encoder) != 0) {
		printf("FAILED, expected %u, got %u\n", 0, OggFLAC__stream_encoder_get_min_residual_partition_order(encoder));
		return false;
	}
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_get_max_residual_partition_order()... ");
	if(OggFLAC__stream_encoder_get_max_residual_partition_order(encoder) != 0) {
		printf("FAILED, expected %u, got %u\n", 0, OggFLAC__stream_encoder_get_max_residual_partition_order(encoder));
		return false;
	}
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_get_rice_parameter_search_dist()... ");
	if(OggFLAC__stream_encoder_get_rice_parameter_search_dist(encoder) != 0) {
		printf("FAILED, expected %u, got %u\n", 0, OggFLAC__stream_encoder_get_rice_parameter_search_dist(encoder));
		return false;
	}
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_get_total_samples_estimate()... ");
	if(OggFLAC__stream_encoder_get_total_samples_estimate(encoder) != streaminfo_.data.stream_info.total_samples) {
		printf("FAILED, expected %llu, got %llu\n", streaminfo_.data.stream_info.total_samples, OggFLAC__stream_encoder_get_total_samples_estimate(encoder));
		return false;
	}
	printf("OK\n");

	/* init the dummy sample buffer */
	for(i = 0; i < sizeof(samples) / sizeof(FLAC__int32); i++)
		samples[i] = i & 7;

	printf("testing OggFLAC__stream_encoder_process()... ");
	if(!OggFLAC__stream_encoder_process(encoder, (const FLAC__int32 * const *)samples_array, sizeof(samples) / sizeof(FLAC__int32)))
		return die_s_("returned false", encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_process_interleaved()... ");
	if(!OggFLAC__stream_encoder_process_interleaved(encoder, samples, sizeof(samples) / sizeof(FLAC__int32)))
		return die_s_("returned false", encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_finish()... ");
	OggFLAC__stream_encoder_finish(encoder);
	printf("OK\n");

	printf("testing OggFLAC__stream_encoder_delete()... ");
	OggFLAC__stream_encoder_delete(encoder);
	printf("OK\n");

	printf("\nPASSED!\n");

	return true;
}

FLAC__bool test_encoders()
{
	init_metadata_blocks_();

	if(!test_stream_encoder())
		return false;

	(void) grabbag__file_remove_file(oggflacfilename_);
	free_metadata_blocks_();

	return true;
}
