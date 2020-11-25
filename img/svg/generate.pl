#!/usr/bin/perl
use strict;

my @files = <*.svg>;
my @sizes = (32,64);
for my $size (@sizes) {
    mkdir("../$size");
}

for my $file (@files) {
    my $base = substr($file, 0, length($file)-4);
    for my $size (@sizes) {
	my $cmd = "inkscape -z -e ../$size/$base.png -w $size -h $size $file\n";
	print $cmd, "\n";
	system($cmd);
    }
}
