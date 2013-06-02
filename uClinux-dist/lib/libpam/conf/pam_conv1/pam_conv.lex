
%{
/*
 * $Id: pam_conv.lex,v 1.3 2005/08/16 03:41:36 magicyang Exp $
 *
 * Copyright (c) Andrew G. Morgan 1997 <morgan@parc.power.net>
 *
 * This file is covered by the Linux-PAM License (which should be
 * distributed with this file.)
 */

    const static char lexid[]=
	"$Id: pam_conv.lex,v 1.3 2005/08/16 03:41:36 magicyang Exp $\n"
	"Copyright (c) Andrew G. Morgan 1997 <morgan@parc.power.net>\n";

    extern int current_line;
%}

%%

"#"[^\n]*         ; /* skip comments (sorry) */

"\\\n" {
    ++current_line;
}

([^\n\t ]|[\\][^\n])+ {
    return TOK;
}

[ \t]+      ; /* Ignore */

<<EOF>> {
    return EOFILE;
}

[\n] {
    ++current_line;
    return NL;
}

%%
