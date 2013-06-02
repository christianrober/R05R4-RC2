/*
 * configfile.h
 *
 * Function to read pptpd config file.
 *
 * $Id: configfile.h,v 1.2 2005/07/25 05:00:43 magicyang Exp $
 */

#ifndef _PPTPD_CONFIGFILE_H
#define _PPTPD_CONFIGFILE_H

int read_config_file(char *filename, char *keyword, char *value);

#endif	/* !_PPTPD_CONFIGFILE_H */
