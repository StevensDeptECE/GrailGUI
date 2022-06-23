use strict;

open(BIG, ">big.grail") ||die("Can't open big.grail");

print BIG '<grail stylesheet="abc.css" title="big grail file">';


for (my $i = 0; $i < 100000; $i++) {
    my $y = $i * 30;
    print BIG "<drawText x=\"0\" y=\"$y\">$i</drawText>\n";
    print BIG "<drawText x=\"0\" y=\"$y\">Dov</drawText>\n";
    print BIG "<drawText x=\"0\" y=\"$y\">Kruger</drawText>\n";
    print BIG "<drawText x=\"0\" y=\"$y\">95.5</drawText>\n";
}

print BIG "</grail>";
