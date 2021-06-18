#! /usr/bin/perl

open STATS, ">>$ENV{GRAIL}/proj/codestats.dat";
my $codesum = "$ENV{GRAIL}/proj/codesummary";

my $projectLineCount = 0;
my $projectByteCount = 0;
my $projectFileCount = 0;

sub statsForOneDir {
    my ($dir) = @_;
    open CODESUM, "<$codesum";
    my $dirFileCount = 0;
    my $dirLineCount = 0;
    my $dirByteCount = 0;
    while (my $line = <CODESUM>) {
	if ($line =~ /\s*(\d+)\s+(\d+)\s+(.*)/) {
	    my $lineCount = $1;
	    my $byteCount = $2;
	    my $filename = $3;
	    $dirFileCount++;
	    $dirLineCount += $lineCount;
	    $dirByteCount += $byteCount;
	} else {
	    print "Error: $line";
	}
    }
    my $time = time();
    printf ("%12s %20s %5d %6d %8d\n", $time, $dir, $dirFileCount, $dirLineCount, $dirByteCount);
    printf (STATS "%12s %20s %5d %6d %8d\n", $time, $dir, $dirFileCount, $dirLineCount, $dirByteCount);
    $projectFileCount += $dirFileCount;
    $projectLineCount += $dirLineCount;
    $projectByteCount += $dirByteCount;
    #print "Files: $files\n";
    #print "Total Lines of Code: $totalLines\n";
    #print "Total bytes: $totalBytes\n";
}
 


#my $cmd = "find src -name \"*.[ch][ch]\" -exec wc -lc \{\} > codesummary ;";
#print $cmd;
chdir("$ENV{GRAIL}/src");
my @files = <*>;
foreach my $file (@files) {
    if ( -d $file ) {
#	print $file, "\n";
	my $cmd = "find $file -name \"*.[ch]?\" -exec wc -lc \{\} >$codesum \\;";
	#	print($cmd, "\n");
	system($cmd);
	statsForOneDir($file);
    }
}
printf ("%12s %20s %5d %6d %8d\n", time(), "total proj", $projectFileCount, $projectLineCount, $projectByteCount);
printf (STATS "%12s %20s %5d %6d %8d\n", time(), "total proj", $projectFileCount, $projectLineCount, $projectByteCount);
close(STATS);      
