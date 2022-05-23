use strict;

open(BIG, ">big.grail") ||die("Can't open big.grail");

print BIG '<grail stylesheet="abc.css" title="big grail file">';


for (my $i = 0; $i < 1000; $i++) {
    my $y = $i * 30;
    print BIG "<drawText x=\"0\" y=\"$y\">Testing</drawText>\n";

}

print BIG "</grail>";
