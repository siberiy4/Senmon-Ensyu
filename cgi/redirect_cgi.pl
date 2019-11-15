#!/usr/bin/perl

use CGI;
use CGI::Carp qw/ fatalsToBrowser /;

$q = new CGI;
require './init.cgi';
print << "EOL";
Content-type: text/html

<html>
	<head>
		<META HTTP-EQUIV="Refresh" CONTENT="0; URL=リダイレクト先を入れる" />
	</head>
</html>

EOL
