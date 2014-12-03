#!/usr/bin/perl

use strict;
use CGI;
my $q = new CGI;

my $login = $q->param( 'nusrn' );
my $file = 'Members.csv';

if ($login eq "") {
	print "Content-type: text/html\n\n";
	print 'username field is empty.';
	print "please retry <a href=\"register.html\">here</a>.";
exit;
}

open(SOURCE, "<$file") or die "Error: Cannot open $file";

while(my $line = <SOURCE>) {

chomp($line);
my @data = split(/,/, $line);

#check if username already exist
  if ($login eq $data[1]) {
	print "Content-type: text/html\n\n";
	print '<head><meta HTTP-EQUIV="REFRESH" content= "0; url=error_perl.html"></head>';
	exit;
  }
}
close(SOURCE);

#if it is a new user
my $pw = $q->param( 'npw' );
my $confmpw = $q->param( 'cpw' );


if ($pw eq "" || $confmpw eq "") {
	print "Content-type: text/html\n\n";
	print 'password fields are empty.';
	print "please retry <a href=\"register.html\">here</a>.";
exit;
}

if ($pw ne $confmpw) {
	print "Content-type: text/html\n\n";
	print 'Confirmation password does not match.';
	print "please retry <a href=\"register.html\">here</a>.";
exit;
}

my $name = $q->param( 'fulln' );


if ($name  eq "") {
	print "Content-type: text/html\n\n";
	print 'fullname field is empty.';
	print "please retry <a href=\"register.html\">here</a>.";
exit;
}

my $new = join(',', "$name" , "$login", "$pw");

$new = "$new" . "\n";	

open(my $ADD, ">>$file");
print $ADD "$new";
close(ADD);

	print "Content-type: text/html\n\n";
	print "Thank you! Now you can log in at our <a href=\"home.html\">homepage</a>.";
