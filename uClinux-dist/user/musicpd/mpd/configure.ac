dnl AC_INIT(src/main.c)
dnl AM_INIT_AUTOMAKE(mpd, 0.10.0)

AC_PREREQ(2.52)
AC_INIT(mpd-uclinux, 0.11.5, eric@petta-tech.com)
AM_INIT_AUTOMAKE($PACKAGE_NAME, $PACKAGE_VERSION)

dnl MAD wants this stuff
AC_SUBST(CCAS)
AC_SUBST(CCASFLAGS)

AC_SUBST(FLAC_SUBDIR)
AC_SUBST(FLAC_LIB)
AC_SUBST(IVORBIS_SUBDIR)
AC_SUBST(IVORBIS_LIB)
AC_SUBST(MAD_SUBDIR)
AC_SUBST(MAD_LIB)
AC_SUBST(ID3_SUBDIR)
AC_SUBST(ID3_LIB)
AC_SUBST(MP4FF_LIB)
AC_SUBST(MP4FF_SUBDIR)
AC_SUBST(MPD_LIBS)
AC_SUBST(MPD_CFLAGS)

AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_MAKE_SET

AM_CONFIG_HEADER(config.h)

MPD_CFLAGS=""
if test x$CC = xgcc; then
	MPD_CFLAGS="-Wall"
fi
MPD_LIBS=""

AC_ARG_ENABLE(audio,[  --disable-audio    disable support for playing],,audio=none)
dnl AC_ARG_ENABLE(shout,[  --disable-shout    disable support for streaming through shout],,enable_shout=yes)
AC_ARG_ENABLE(iconv,[  --disable-iconv     disable iconv support],,enable_iconv=yes)
AC_ARG_ENABLE(http,[  --disable-http     disable http input support],,enable_http=yes)
AC_ARG_ENABLE(ipv6,[  --disable-ipv6     disable IPv6 support],,enable_ipv6=yes)
AC_ARG_ENABLE(alsa,[  --disable-alsa     disable ALSA Mixer support],,enable_alsa=yes)
AC_ARG_ENABLE(ogg,[  --disable-ogg     disable ogg support],,enable_ogg=yes)
AC_ARG_ENABLE(flac,[  --disable-flac    disable flac support],,enable_flac=yes)
AC_ARG_ENABLE(mp3,[  --disable-mp3     disable mp3 support],,enable_mp3=yes)
AC_ARG_ENABLE(aac,[  --disable-aac     disable AAC support],,enable_aac=yes)
AC_ARG_ENABLE(audiofile,[  --disable-audiofile disable audiofile support, disables wave support],,enable_audiofile=yes)
AC_ARG_ENABLE(mod,[  --disable-mod     disable MOD support],,enable_mod=yes)
AC_ARG_ENABLE(id3,[  --disable-id3     disable id3 support],,enable_id3=yes)
AC_ARG_ENABLE(mpd_mad,[  --enable-mpd-mad     use mpd libmad],use_mpd_mad=yes,)
AC_ARG_ENABLE(mpd_flac,[  --enable-mpd-flac     use mpd flac],use_mpd_flac=yes,)
AC_ARG_ENABLE(mpd_ivorbis,[  --enable-mpd-ivorbis     use mpd tremor/integer vorbis],use_mpd_ivorbis=yes,)
AC_ARG_ENABLE(mpd_id3tag,[  --enable-mpd-id3tag    use mpd libid3tag],use_mpd_id3tag=yes,)
AC_ARG_ENABLE(uclinux,[  --enable-uclinux     use options suitable for uClinux],enable_uclinux=yes,)

AC_ARG_WITH(iconv,[  --with-iconv=PFX   Prefix where iconv is installed (optional)], iconv_prefix="$withval", iconv_prefix="")
AC_ARG_WITH(iconv-libraries,[  --with-iconv-libraries=DIR   Directory where libiconv library is installed (optional)], iconv_libraries="$withval", iconv_libraries="")
AC_ARG_WITH(iconv-includes,[  --with-iconv-includes=DIR   Directory where libiconv header files are installed (optional)], iconv_includes="$withval", iconv_includes="")

AC_ARG_WITH(id3tag,[  --with-id3tag=PFX   Prefix where libid3tag is installed (optional)], id3tag_prefix="$withval", id3tag_prefix="")
AC_ARG_WITH(id3tag-libraries,[  --with-id3tag-libraries=DIR   Directory where libid3tag library is installed (optional)], id3tag_libraries="$withval", id3tag_libraries="")
AC_ARG_WITH(id3tag-includes,[  --with-id3tag-includes=DIR   Directory where libid3tag header files are installed (optional)], id3tag_includes="$withval", id3tag_includes="")

AC_ARG_WITH(mad,[  --with-mad=PFX   Prefix where libmad is installed (optional)], mad_prefix="$withval", mad_prefix="")
AC_ARG_WITH(mad-libraries,[  --with-mad-libraries=DIR   Directory where libmad library is installed (optional)], mad_libraries="$withval", mad_libraries="")
AC_ARG_WITH(mad-includes,[  --with-mad-includes=DIR   Directory where mad header files are installed (optional)], mad_includes="$withval", mad_includes="")

AC_ARG_WITH(faad,[  --with-faad=PFX   Prefix where faad2 is installed], faad_prefix="$withval", faad_prefix="")
AC_ARG_WITH(faad-libraries,[  --with-faad-libraries=DIR   Directory where faad2 library is installed (optional)], faad_libraries="$withval", faad_libraries="")
AC_ARG_WITH(faad-includes,[  --with-faad-includes=DIR   Directory where faad2 header files are installed (optional)], faad_includes="$withval", faad_includes="")
AC_ARG_WITH(audio,[  --with-audio=ARG  Audio output format (libao,oss,none)], audio="$withval", audio="libao")

AC_C_BIGENDIAN

AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(long long)

AC_CHECK_HEADER(sys/inttypes.h,AC_DEFINE(HAVE_SYS_INTTYPES_H,1,[Define if sys/inttypes.h present]),)

AC_CHECK_LIB(socket,socket,MPD_LIBS="$MPD_LIBS -lsocket",)
AC_CHECK_LIB(nsl,gethostbyname,MPD_LIBS="$MPD_LIBS -lnsl",)

AC_CHECK_LIB(m,exp,MPD_LIBS="$MPD_LIBS -lm",)

dnl doesn't work for systems that don't have CODESET like OpenBSD
dnl AC_CHECK_HEADER(langinfo.h,[enable_langinfo=yes;AC_DEFINE(HAVE_LANGINFO,1,[Define if nl_langinfo.h is present])],enable_langinfo=no)
if test x$enable_uclinux != xyes ; then
	AM_LANGINFO_CODESET
	AC_CHECK_HEADER(locale.h,[enable_locale=yes;AC_DEFINE(HAVE_LOCALE,1,[Define if locale.h is present])],enable_locale=no)
else
	AC_DEFINE(HAVE_UCLINUX,1,[Define to configure for uClinux])
	AC_DEFINE(NDEBUG,1,[Define to configure without debugging])
	CC="arm-elf-gcc"
	enable_iconv=no
	enable_http=no
	enable_ipv6=no
	audio=oss
	enable_alsa=no
fi

if test x$enable_ipv6 = xyes; then
	AC_MSG_CHECKING(for ipv6)
	AC_EGREP_CPP([AP_maGiC_VALUE],
	[
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#ifdef PF_INET6
#ifdef AF_INET6
AP_maGiC_VALUE
#endif
#endif
	],
	AC_DEFINE(HAVE_IPV6, 1, [Define if IPv6 support present])
	AC_MSG_RESULT([yes]),
	AC_MSG_RESULT([no])
)
fi

if test x$audio = xlibao; then
	XIPH_PATH_AO(MPD_LIBS="$MPD_LIBS $AO_LIBS" MPD_CFLAGS="$MPD_CFLAGS $AO_CFLAGS",AC_MSG_ERROR(Must have libao installed!!!))
	AC_DEFINE(HAVE_AUDIO, 1, [Define to output audio with libAO])
elif test x$audio = xoss; then 
	AC_CHECK_HEADERS(sys/soundcard.h)
	AC_CHECK_HEADERS(machine/soundcard.h)
	if test "${ac_cv_header_sys_soundcard_h}" = "yes" || test "${ac_cv_header_machine_soundcard_h}" = "yes"; then
		AC_DEFINE(HAVE_AUDIO, 1, [Define to play audio])
		AC_DEFINE(HAVE_OSS, 1, [Define to output audio with OSS])
	fi
fi

dnl if test x$enable_shout = xyes; then
dnl	XIPH_PATH_SHOUT([AC_DEFINE(HAVE_SHOUT, 1, [Define to enable libshout support]) MPD_LIBS="$MPD_LIBS $SHOUT_LIBS" MPD_CFLAGS="$MPD_CFLAGS $SHOUT_CFLAGS"], enable_shout=no)
dnl fi

AC_CHECK_HEADER(sys/soundcard.h,enable_oss=yes,[AC_MSG_WARN(Soundcard headers not found -- disabling OSS mixer);enable_oss=no;AC_DEFINE(NO_OSS_MIXER,1,[Define to disable OSS mixer support])])

if test x$enable_alsa = xyes; then
	AC_MSG_CHECKING(for alsa)
	AC_EGREP_CPP([AP_maGiC_VALUE],
	[
#include <alsa/asoundlib.h>
#if defined(SND_LIB_MAJOR) && defined(SND_LIB_MINOR)
#if SND_LIB_MAJOR>0 || (SND_LIB_MAJOR==0 && SND_LIB_MINOR>=6)
AP_maGiC_VALUE
#endif
#endif
	],
	enable_alsa=yes
	AC_MSG_RESULT([yes]),
	enable_alsa=no
	AC_MSG_RESULT([no])
	)
fi

if test x$enable_alsa = xyes; then
	AC_CHECK_LIB(asound,main,[MPD_LIBS="$MPD_LIBS -lasound";AC_DEFINE(HAVE_ALSA,1,[Define if alsa support is present])],enable_alsa=no)
fi

if test x$enable_http = xyes; then
	AC_DEFINE(HAVE_HTTP_INPUT,1,[Define for HTTP streaming])
fi

if test x$enable_iconv = xyes; then
  	if test "x$iconv_libraries" != "x" ; then
  		ICONV_LIBS="-L$iconv_libraries"
  	elif test "x$iconv_prefix" != "x" ; then
    		ICONV_LIBS="-L$iconv_prefix/lib"
  	elif test "x$prefix" != "xNONE"; then
    		ICONV_LIBS="-L$libdir"
  	fi

  	ICONV_LIBS="$ICONV_LIBS -liconv"

  	if test "x$iconv_includes" != "x" ; then
  		ICONV_CFLAGS="-I$iconv_includes"
  	elif test "x$iconv_prefix" != "x" ; then
    		ICONV_CFLAGS="-I$iconv_prefix/include"
  	elif test "x$prefix" != "xNONE"; then
    		ICONV_CFLAGS="-I$includedir"
  	fi

	oldcflags=$CFLAGS
	oldlibs=$LIBS
	oldcppflags=$CPPFLAGS
	CFLAGS="$CFLAGS $MPD_CFLAGS $ICONV_CFLAGS"
	LIBS="$LIBS $MPD_LIBS $ICONV_LIBS"
	CPPFLAGS=$CFLAGS
	AC_CHECK_HEADER(iconv.h,MPD_CFLAGS="$MPD_CFLAGS $ICONV_CFLAGS",enable_iconv=no)
	if test x$enable_iconv = xyes; then
		AC_CHECK_LIB(iconv,main,MPD_LIBS="$MPD_LIBS $ICONV_LIBS",)
		AC_DEFINE(HAVE_ICONV,1,[Define to use iconv])
	fi
	CFLAGS=$oldcflags
	LIBS=$oldlibs
	CPPFLAGS=$oldcppflags
fi

ID3_SUBDIR=""

if test x$enable_id3 = xyes; then
	if test x$use_mpd_id3tag = xyes; then
		ID3_SUBDIR="libid3tag"
	else
  		if test "x$id3tag_libraries" != "x" ; then
  			ID3TAG_LIBS="-L$id3tag_libraries"
  		elif test "x$id3tag_prefix" != "x" ; then
    			ID3TAG_LIBS="-L$id3tag_prefix/lib"
  		elif test "x$prefix" != "xNONE"; then
    			ID3TAG_LIBS="-L$libdir"
  		fi

  		ID3TAG_LIBS="$ID3TAG_LIBS -lid3tag -lz"

  		if test "x$id3tag_includes" != "x" ; then
  			ID3TAG_CFLAGS="-I$id3tag_includes"
  		elif test "x$id3tag_prefix" != "x" ; then
    			ID3TAG_CFLAGS="-I$id3tag_prefix/include"
  		elif test "x$prefix" != "xNONE"; then
    			ID3TAG_CFLAGS="-I$includedir"
  		fi

  		ID3TAG_CFLAGS="$ID3TAG_CFLAGS"

		oldcflags=$CFLAGS
		oldlibs=$LIBS
		oldcppflags=$CPPFLAGS
		CFLAGS="$CFLAGS $MPD_CFLAGS $ID3TAG_CFLAGS"
		LIBS="$LIBS $MPD_LIBS $ID3TAG_LIBS"
		CPPFLAGS=$CFLAGS
		AC_CHECK_HEADERS(id3tag.h,use_libid3tag=yes,
			[use_libid3tag=no;use_mpd_id3tag=yes])
		if test x$use_libid3tag = xyes; then
			AC_CHECK_LIB(id3tag,id3_file_open,
				[ID3_LIB="$ID3TAG_LIBS";
     				MPD_CFLAGS="$MPD_CFLAGS $ID3TAG_CFLAGS";
				use_libid3tag=yes],
				[use_libid3tag=no;use_mpd_id3tag=yes])
		fi
		CFLAGS=$oldcflags
		LIBS=$oldlibs
		CPPFLAGS=$oldcppflags
	fi
	if test x$use_mpd_id3tag = xyes; then
		AC_DEFINE(USE_MPD_ID3TAG,1,[Define to use mpd libid3tag])
		ID3_LIB="libid3tag/libid3tag.la"
		ID3_SUBDIR="libid3tag"
		AC_CONFIG_SUBDIRS(src/libid3tag)
	fi
	AC_DEFINE(HAVE_ID3TAG,1,[Define to use id3tag])
fi

MAD_SUBDIR=""

if test x$enable_mp3 = xyes; then
	if test x$use_mpd_mad = xyes; then
		MAD_SUBDIR="libmad"
	else
  		if test "x$mad_libraries" != "x" ; then
  			MAD_LIBS="-L$mad_libraries"
  		elif test "x$mad_prefix" != "x" ; then
    			MAD_LIBS="-L$mad_prefix/lib"
  		elif test "x$prefix" != "xNONE"; then
    			MAD_LIBS="-L$libdir"
  		fi

  		MAD_LIBS="$MAD_LIBS -lmad"

  		if test "x$mad_includes" != "x" ; then
  			MAD_CFLAGS="-I$mad_includes"
  		elif test "x$mad_prefix" != "x" ; then
    			MAD_CFLAGS="-I$mad_prefix/include"
  		elif test "x$prefix" != "xNONE"; then
    			MAD_CFLAGS="-I$includedir"
  		fi

		oldcflags=$CFLAGS
		oldlibs=$LIBS
		oldcppflags=$CPPFLAGS
		CFLAGS="$CFLAGS $MPD_CFLAGS $MAD_CFLAGS"
		LIBS="$LIBS $MPD_LIBS $MAD_LIBS"
		CPPFLAGS=$CFLAGS
		AC_CHECK_HEADERS(mad.h,use_libmad=yes,
			[use_libmad=no;use_mpd_mad=yes])
		if test x$use_libmad = xyes; then
			AC_CHECK_LIB(mad,mad_stream_init,[MAD_LIB="$MAD_LIBS";
				MPD_CFLAGS="$MPD_CFLAGS $MAD_CFLAGS";
     				use_libmad=yes],[use_libmad=no;use_mpd_mad=yes])
		fi
		CFLAGS=$oldcflags
		LIBS=$oldlibs
		CPPFLAGS=$oldcppflags
	fi
	if test x$use_mpd_mad = xyes; then
		AC_DEFINE(USE_MPD_MAD,1,[Define to use mpd libmad])
		MAD_LIB="libmad/libmad.la"
		MAD_SUBDIR="libmad"
		AC_CONFIG_SUBDIRS(src/libmad)
		MPD_CFLAGS="$MPD_CFLAGS -Ilibmad"
	fi
	AC_DEFINE(HAVE_MAD,1,[Define to use libmad])
fi

MP4FF_SUBDIR=""

if test x$enable_aac = xyes; then
  	if test "x$faad_libraries" != "x" ; then
  		FAAD_LIBS="-L$faad_libraries"
  	elif test "x$faad_prefix" != "x" ; then
    		FAAD_LIBS="-L$faad_prefix/lib"
  	elif test "x$prefix" != "xNONE"; then
    		FAAD_LIBS="-L$libdir"
  	fi

 	FAAD_LIBS="$FAAD_LIBS -lfaad"

  	if test "x$faad_includes" != "x" ; then
  		FAAD_CFLAGS="-I$faad_includes"
  	elif test "x$faad_prefix" != "x" ; then
    		FAAD_CFLAGS="-I$faad_prefix/include"
  	elif test "x$prefix" != "xNONE"; then
    		FAAD_CFLAGS="-I$includedir"
  	fi

	oldcflags=$CFLAGS
	oldlibs=$LIBS
	oldcppflags=$CPPFLAGS
	CFLAGS="$CFLAGS $MPD_CFLAGS $FAAD_CFLAGS -I."
	LIBS="$LIBS $MPD_LIBS $FAAD_LIBS"
	CPPFLAGS=$CFLAGS
	AC_CHECK_HEADER(faad.h,,enable_aac=no)
	if test x$enable_aac = xyes; then
		AC_CHECK_DECL(FAAD2_VERSION,,enable_aac=no,[#include <faad.h>])
	fi
	if test x$enable_aac = xyes; then
		AC_CHECK_DECL(faacDecInit2,,enable_aac=no,[#include <faad.h>])
	fi
	if test x$enable_aac = xyes; then
		AC_CHECK_LIB(faad,faacDecInit2,[MPD_LIBS="$MPD_LIBS $FAAD_LIBS";MPD_CFLAGS="$MPD_CFLAGS $FAAD_CFLAGS";MP4FF_SUBDIR="mp4ff";MP4FF_LIB="mp4ff/libmp4ff.la"],enable_aac=no)
	fi
	if test x$enable_aac = xyes; then
  		AC_MSG_CHECKING(that FAAD2 uses buffer and bufferlen)
		AC_COMPILE_IFELSE([
#include <faad.h>

int main() {
	char buffer;
	long bufferlen = 0;
	faacDecHandle decoder;
	faacDecFrameInfo frameInfo;
	faacDecConfigurationPtr config;
	unsigned char channels;
	long sampleRate;
        mp4AudioSpecificConfig mp4ASC;

	decoder = faacDecOpen();
	config = faacDecGetCurrentConfiguration(decoder);
	config->outputFormat = FAAD_FMT_16BIT;
	faacDecSetConfiguration(decoder,config);
	AudioSpecificConfig(&buffer, bufferlen, &mp4ASC);
	faacDecInit(decoder,&buffer,bufferlen,&sampleRate,&channels);
	faacDecInit2(decoder,&buffer,bufferlen,&sampleRate,&channels);
	faacDecDecode(decoder,&frameInfo,&buffer,bufferlen);

	return 0;
}
],[AC_MSG_RESULT(yes);AC_DEFINE(HAVE_FAAD_BUFLEN_FUNCS,1,[Define if FAAD2 uses buflen in function calls])],[AC_MSG_RESULT(no);
		AC_MSG_CHECKING(that FAAD2 can even be used)
		AC_COMPILE_IFELSE([
#include <faad.h>

int main() {
	char buffer;
	faacDecHandle decoder;
	faacDecFrameInfo frameInfo;
	faacDecConfigurationPtr config;
	unsigned char channels;
	long sampleRate;
	long bufferlen = 0;
	unsigned long dummy1_32;
        unsigned char dummy2_8, dummy3_8, dummy4_8, dummy5_8, dummy6_8,
                                dummy7_8, dummy8_8;

	decoder = faacDecOpen();
	config = faacDecGetCurrentConfiguration(decoder);
	config->outputFormat = FAAD_FMT_16BIT;
	faacDecSetConfiguration(decoder,config);
	AudioSpecificConfig(&buffer,&dummy1_32,&dummy2_8,
                                &dummy3_8,&dummy4_8,&dummy5_8,
                                &dummy6_8,&dummy7_8,&dummy8_8);
	faacDecInit(decoder,&buffer,&sampleRate,&channels);
	faacDecInit2(decoder,&buffer,bufferlen,&sampleRate,&channels);
	faacDecDecode(decoder,&frameInfo,&buffer);
	faacDecClose(decoder);

	return 0;
}
],AC_MSG_RESULT(yes),[AC_MSG_RESULT(no);enable_aac=no])
		])
	fi
	if test x$enable_aac = xyes; then
		AC_CHECK_TYPES(mp4AudioSpecificConfig,,,[#include <faad.h>])
		AC_CHECK_MEMBERS([faacDecConfiguration.downMatrix,faacDecConfiguration.dontUpSampleImplicitSBR,faacDecFrameInfo.samplerate],,,[#include <faad.h>])
		AC_DEFINE(HAVE_FAAD,1,[Define to use FAAD2 for AAC decoding])
	else
		AC_MSG_WARN([faad2 lib needed for MP4/AAC support -- disabling MP4/AAC support])
	fi
	CFLAGS=$oldcflags
	LIBS=$oldlibs
	CPPFLAGS=$oldcppflags
fi

IVORBIS_SUBDIR=""

if test x$enable_ogg = xyes; then
	if test x$use_mpd_ivorbis != xyes; then
		XIPH_PATH_OGG(MPD_LIBS="$MPD_LIBS $OGG_LIBS" MPD_CFLAGS="$MPD_CFLAGS $OGG_CFLAGS",use_mpd_ivorbis=yes)
		if test x$use_mpd_ivorbis != xyes; then
			enable_vorbistest=no
			XIPH_PATH_VORBIS(,enable_mpd_ivorbis=yes)

		dnl
		dnl Vorbis Test
		dnl vorbistest in XIPH_PATH_OGG is broken on my debian system
		dnl so here i used my own hacked up version till i get vorbis-dev gets
		dnl upgraded in debian (hopefully when 1.0.1 comes out)
		dnl
				AC_MSG_CHECKING(that Vorbis is usable)

				ac_save_CFLAGS="$CFLAGS"
				ac_save_LIBS="$LIBS"
				CFLAGS="$CFLAGS $VORBIS_CFLAGS $OGG_CFLAGS"
				LIBS="$LIBS $VORBIS_LIBS $VORBISENC_LIBS $OGG_LIBS"

				AC_TRY_RUN([
		#include <stdio.h>
		#include <stdlib.h>
		#include <string.h>
		#include <vorbis/codec.h>
		#include <vorbis/vorbisenc.h>

		int main ()
		{
		    vorbis_block 	vb;
		    vorbis_dsp_state	vd;
		    vorbis_info		vi;

		    vorbis_info_init (&vi);
		    vorbis_encode_init (&vi, 2, 44100, -1, 128, -1);
		    /*vorbis_analysis_init (&vd, &vi);
		    vorbis_block_init (&vd, &vb);*/
		    /* this function was added in 1.0rc3, so this is what we're testing for */
		    /*vorbis_bitrate_addblock (&vb);*/

		    return 0;
		}

		], AC_MSG_RESULT(yes), AC_MSG_RESULT(no) enable_mpd_ivorbis=no,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
				CFLAGS="$ac_save_CFLAGS"
				LIBS="$ac_save_LIBS"
		dnl 
		dnl End of Vorbis Test
		dnl
		fi
	fi
	if test x$use_mpd_ivorbis != xyes; then
		MPD_LIBS="$MPD_LIBS $VORBIS_LIBS $VORBISFILE_LIBS" 
		MPD_CFLAGS="$MPD_CFLAGS $VORBIS_CFLAGS $VORBISFILE_CFLAGS"
	else
		AC_DEFINE(HAVE_MPD_IVORBIS,1,[Define for bundled ogg vorbis support using Tremor])
		IVORBIS_SUBDIR="tremor"
		IVORBIS_LIB="$IVORBIS_SUBDIR/libvorbisidec.la"
		AC_CONFIG_SUBDIRS(src/tremor)
	fi
	AC_DEFINE(HAVE_OGG,1,[Define for ogg vorbis support])
fi

FLAC_SUBDIR=""

if test x$enable_flac = xyes; then
	if test x$use_mpd_flac != xyes; then
		oldmpdcflags="$MPD_CFLAGS"
		oldmpdlibs="$MPD_LIBS"
		AM_PATH_LIBFLAC(MPD_LIBS="$MPD_LIBS $LIBFLAC_LIBS" MPD_CFLAGS="$MPD_CFLAGS $LIBFLAC_CFLAGS",
		  [use_mpd_flac=yes;AC_MSG_WARN(You need FLAC 1.1 -- using bundled flac support)])
		oldcflags="$CFLAGS"
		oldlibs="$LIBS"
		CFLAGS="$CFLAGS $MPD_CFLAGS"
		LIBS="$LIBS $MPD_LIBS"
		AC_CHECK_LIB(FLAC, FLAC__metadata_object_vorbiscomment_find_entry_from,
			,[use_mpd_flac=yes;AC_MSG_WARN(You need FLAC 1.1 -- using bundled flac support)])
		CFLAGS="$oldcflags"
		LIBS="$oldlibs"
		if x$use_mpd_flac = xyes ; then
			MPD_CFLAGS="$oldmpdcflags"
			MPD_LIBS="$oldmpdlibs"
		fi
	fi
	if test x$use_mpd_flac = xyes; then
		AC_DEFINE(USE_MPD_FLAC,1,[Define to use mpd flac])
		FLAC_SUBDIR="flac"
		AC_CONFIG_SUBDIRS(src/flac)
		FLAC_LIB="$FLAC_SUBDIR/src/libFLAC/libFLAC.la"
		MPD_CFLAGS="$MPD_CFLAGS"
	fi
	AC_DEFINE(HAVE_FLAC,1,[Define for FLAC support])
fi	
		
if test x$enable_audiofile = xyes; then
	AM_PATH_AUDIOFILE(0.1.7, MPD_LIBS="$MPD_LIBS $AUDIOFILE_LIBS" MPD_CFLAGS="$MPD_CFLAGS $AUDIOFILE_CFLAGS",
		[enable_audiofile=no;AC_MSG_WARN(You need audiofile  -- disabling audiofile support)])
fi

if test x$enable_audiofile = xyes; then
	AC_DEFINE(HAVE_AUDIOFILE,1,[Define for audiofile support])
fi

if test x$enable_mod = xyes; then
	AM_PATH_LIBMIKMOD(3.1.7, MPD_CFLAGS="$MPD_CFLAGS $LIBMIKMOD_CFLAGS"
		MPD_LIBS="$MPD_LIBS $LIBMIKMOD_LIBS $LIBMIKMOD_LDADD", enable_mod=no)
	if test x$enable_mod = xyes; then
		AC_DEFINE(HAVE_MIKMOD, 1, [Define for mikmod support])
	fi
fi

AC_OUTPUT(src/mp4ff/Makefile doc/Makefile src/Makefile Makefile )

echo ""
echo "########### MPD CONFIGURATION ############"
echo ""
echo "Volume Mixer Support:"
if test x$enable_oss = xyes; then
	echo "OSS mixer support .............enabled"
else
	echo "OSS mixer support .............disabled"
fi

if test x$enable_alsa = xyes; then
	echo "ALSA mixer support ............enabled"
else
	echo "ALSA mixer support ............disabled"
fi

echo ""
echo "i18n Support:"
if test x$enable_iconv = xyes; then
	echo "iconv support .................enabled"
else
	echo "iconv support .................disabled"
fi

if test x$enable_locale = xyes; then
	echo "locale support ................enabled"
else
	echo "locale support ................disabled"
fi

if test x$am_cv_langinfo_codeset = xyes; then
	echo "langinfo support ..............enabled"
else
	echo "langinfo support ..............disabled"
fi

echo ""
echo "Audio Output Format Support:"
if test x$audio = xlibao || test x$audio = xoss; then
	echo "Playing audio .................enabled"
	if test x$audio = xoss; then
		echo "  using native OSS output .....yes"
	else
		echo "  using libAO output ..........yes"
	fi
else
	echo "Playing audio .................disabled"
fi

if test x$enable_shout = xyes; then
	echo "Streaming through shout .......enabled"
else
	echo "Streaming through shout .......disabled"
fi

if test x$enable_id3 = xyes; then
	echo "ID3 tag support ...............enabled"
	if test x$use_mpd_id3tag = xyes; then
		echo "  using MPD's libid3tag........yes"
	else
		echo "  using MPD's libid3tag........no"
	fi
else
	echo "ID3 tag support ...............disabled"
fi

if test x$enable_mp3 = xyes; then
	echo "mp3 support ...................enabled"
	if test x$use_mpd_mad = xyes; then
		echo "  using MPD's libmad...........yes"
	else
		echo "  using MPD's libmad...........no"
	fi
else
	echo "mp3 support ...................disabled"
fi

if test x$enable_ogg = xyes; then
	echo "Ogg Vorbis support ............enabled"
	if test x$use_mpd_ivorbis = xyes; then
		echo "  using MPD's Tremor...........yes"
	else
		echo "  using MPD's Tremor............no"
	fi
else
	echo "Ogg Vorbis support ............disabled"
fi

if test x$enable_flac = xyes; then
	echo "FLAC support ..................enabled"
	if test x$use_mpd_flac = xyes; then
		echo "  using MPD's libFLAC..........yes"
	else
		echo "  using MPD's libFLAC..........no"
	fi
else
	echo "FLAC support ..................disabled"
fi

if test x$enable_audiofile = xyes; then
	echo "Wave file support .............enabled"
else
	echo "Wave file support .............disabled"
fi

if test x$enable_aac = xyes; then
	echo "MP4/AAC support ...............enabled"
else
	echo "MP4/AAC support ...............disabled"
fi

if test x$enable_mod = xyes; then
	echo "MOD support ...................enabled"
else
	echo "MOD support ...................disabled"
fi

if test x$enable_uclinux = xyes; then
	echo "uCLinux-friendly options ......enabled"
else
	echo "uClinux-friendly options ......disabled"
fi

echo ""
echo "##########################################"
echo ""
echo "You are now ready to compile MPD"
