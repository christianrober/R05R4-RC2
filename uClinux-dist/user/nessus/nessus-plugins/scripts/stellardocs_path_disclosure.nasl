#
# (C) Tenable Network Security
#
# 

if(description)
{
 script_id(11817);
 script_bugtraq_id(8385);

 
 script_version("$Revision: 1.2 $");
 name["english"] = "StellarDocs Path Disclosure";
 script_name(english:name["english"]);
 
 desc["english"] = "
The remote host is running StellarDocs

There is a flaw in this system which may allow an attacker to
obtain the physical path of the remote installation of StellarDocs.

Solution : Upgrade to the latest version of this software
Risk Factor : Low";


 script_description(english:desc["english"]);
 
 summary["english"] = "SQL Injection and more.";
 
 script_summary(english:summary["english"]);
 
 script_category(ACT_GATHER_INFO);
 
 script_copyright(english:"This script is Copyright (C) 2003 Tenable Network Security");
 family["english"] = "CGI abuses";
 script_family(english:family["english"]);
 script_dependencie("find_service.nes", "no404.nasl");
 script_require_ports("Services/www", 80);
 exit(0);
}

# Check starts here

include("http_func.inc");
include("http_keepalive.inc");

function check(dir)
{
  req = http_get(item:dir + "/data/fetch.php?page='", port:port);
  buf = http_keepalive_send_recv(port:port, data:req);
  if(buf == NULL)exit(0);

  if("mysql_num_rows()" >< buf)
  	{
	security_warning(port);
	exit(0);
	}
 return(0);
}

port = get_kb_item("Services/www");
if(!port)port = 80;


foreach dir (make_list("", cgi_dirs()))
{
 check(dir:dir);
}
