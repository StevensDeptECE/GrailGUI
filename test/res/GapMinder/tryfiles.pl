#!/usr/bin/perl
use strict;
use File::Copy;

chdir("datasets");
my @files = <*.csv>;
chdir("..");

#print @files;

my $start = 0;
my $end = $#files;

#my $mid = ($start + $end) / 2;
my $mid = 212;

my $size = int($mid / 2);
while ($size > 0) {
    system("rm allFiles/*.csv");
    copy("datasets/gdp_per_capita.csv", "allFiles");
    copy("datasets/ddf--datapoints--poisonings_deaths_per_100000_people.csv", "allFiles");
    copy("datasets/ddf--datapoints--vacc_rate--by--country--time.csv", "allFiles");

    for (my $i = 0; $i < $mid; $i++) {
        copy("datasets/$files[$i]", "allFiles");
    }


   my $status = system("testGapMinder");
   if ($status != 0) {
       print "testGapMinder Failed: $status\n";
   }
   system("testWidgetGapMinder");

    print "Files up to $mid are in allFiles, last file = $files[$mid-1]\n";
    print "Did it work?";
        my $input = <STDIN>;
    if ($input =~ /.*[yY].*/) {
        $mid += $size;
    } else {
        $mid -= $size;
    }
    $size = int($size / 2);
}
