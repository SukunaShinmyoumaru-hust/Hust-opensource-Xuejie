#!/usr/bin/perl -w

###########################################################################
#                                                                         #
#                               SIGHAN                                    #
#                      Copyright (c) 2003,2005                            #
#                        All Rights Reserved.                             #
#                                                                         #
#  Permission is hereby granted, free of charge, to use and distribute    #
#  this software and its documentation without restriction, including     #
#  without limitation the rights to use, copy, modify, merge, publish,    #
#  distribute, sublicense, and/or sell copies of this work, and to        #
#  permit persons to whom this work is furnished to do so, subject to     #
#  the following conditions:                                              #
#   1. The code must retain the above copyright notice, this list of      #
#      conditions and the following disclaimer.                           #
#   2. Any modifications must be clearly marked as such.                  #
#   3. Original authors' names are not deleted.                           #
#   4. The authors' names are not used to endorse or promote products     #
#      derived from this software without specific prior written          #
#      permission.                                                        #
#                                                                         #
#  SIGHAN AND THE CONTRIBUTORS TO THIS WORK DISCLAIM ALL WARRANTIES       #
#  WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF      #
#  MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL SIGHAN NOR THE          #
#  CONTRIBUTORS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL      #
#  DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA     #
#  OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER      #
#  TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR       #
#  PERFORMANCE OF THIS SOFTWARE.                                          #
#                                                                         #
###########################################################################
#                                                                         #
# Author: Richard Sproat (rws@uiuc.edu)                                   #
#         Tom Emerson (tree@basistech.com)                                #
#                                                                         #
###########################################################################

## This code depends upon a version of diff (e.g. GNU diffutils 2.7.2)
## that supports the -y flag:
##
## -y     Use the side by side output format.
##
## change the following per your installation:

$diff = "/usr/bin/diff";

$USAGE = "Usage:\t$0 dictionary truth test\n\t";

if (@ARGV != 3) {print "$USAGE\n"; exit;}

$tmp1 = "/tmp/comp01$$";
$tmp2 = "/tmp/comp02$$";

%dict = ();

open (S, $ARGV[0]) or  die "$ARGV[0]: $!\n";

while (<S>) {
    chop;
    s/^\s*//;
    s/\s*$//;
    $dict{$_} = 1;
}

close(S);

open (TRUTH, $ARGV[1]) or die "$ARGV[1]: $!\n";
open (TEST, $ARGV[2]) or die "$ARGV[2]: $!\n";

$Tot = $Del = $Ins = $Subst = $Truecount = $Testcount = 0;
$RawRecall = $RawPrecision = 0;

$linenum = 0;


$IVMISSED = $OOVMISSED = $OOV = $IV = 0;

$file1 = $ARGV[1];
$file2 = $ARGV[2];
$file1 =~ s=^/.*/==;
$file2 =~ s=^/.*/==;

while (defined($truth = <TRUTH>) && defined($test = <TEST>)) {
    $truth =~ s/^\s*//;
    $test =~ s/^\s*//;
    $truth =~ s/\s*$//;
    $test =~ s/\s*$//;
    $truth =~ s/(\xe3\x80\x80)|(\xa1\x40)/ /g;
    $test =~ s/(\xe3\x80\x80)|(\xa1\x40)/ /g;
    $truth =~ s///g;
    $test =~ s///g;
    @truthwords = split /\s+/, $truth;
    @testwords = split /\s+/, $test;
    $truecount = scalar(@truthwords);
    $testcount = scalar(@testwords);
    ++$linenum;
    if ($truecount == 0) { 
	if ($testcount > 0) {
	    print STDERR "Warning: training is 0 but test is nonzero, possible misalignment at line $linenum.\n";
	}
	next; 
    }
    if ($testcount == 0) { 
	print STDERR "Warning: No output in test data where there is in training data, line $linenum\n";
    }
    open (T1, ">$tmp1") or die "Can't open $tmp1";
    open (T2, ">$tmp2") or die "Can't open $tmp2";
    foreach my $w (@truthwords) { print T1 "$w\n"; }
    foreach my $w (@testwords) {print T2 "$w\n";}
    close (T1);
    close (T2);
    open (P, "$diff -y $tmp1 $tmp2 |") 
	or die "Can't open pipe.\n";
    print "--$file1-------$file2----$linenum\n";
    my $del = 0;
    my $ins = 0;
    my $subst = 0;
    my $rawrecall = 0;
    my $rawprecision = 0;
    while (<P>) {
	my $err = 0;
	if (/\s\|\s/) {$subst++ ; $err++; }
	elsif (/\s\>\s/) {$ins++ ; $err++; }
	elsif (/\s\<\s/) {$del++ ; $err++; }
	if (/^([^\s]+)\s/) { 
	    my $w = $1;
	    if (!$dict{$w}) {++$OOV;}	    
	    else {++$IV;}
	    if (/^[^\s]+\s.*\s[\|\>\<]\s/) {
		if (!$dict{$w}) {++$OOVMISSED;}
		else {++$IVMISSED;}
		++$rawrecall; 
	    }
	}
	if (/\s[\|\>\<]\s.*[^\s]$/) { ++$rawprecision; }
	print "$_";
    }
    close (P);
    my $tot = $del + $ins + $subst;
    $Tot += $tot;
    $Del += $del;
    $Ins += $ins;
    $Subst += $subst;
    $Truecount += $truecount;
    $Testcount += $testcount;
    $rawrecall = $truecount - $rawrecall;
    $rawprecision = $testcount - $rawprecision;
    $RawRecall += $rawrecall;
    $RawPrecision += $rawprecision;
    $rawrecall = sprintf("%2.3f", $rawrecall/$truecount);
    $rawprecision = sprintf("%2.3f", $rawprecision/$testcount);
    print "INSERTIONS:\t$ins\n";
    print "DELETIONS:\t$del\n";
    print "SUBSTITUTIONS:\t$subst\n";
    print "NCHANGE:\t$tot\n";
    print "NTRUTH:\t$truecount\n";
    print "NTEST:\t$testcount\n";
    print "TRUE WORDS RECALL:\t$rawrecall\n";
    print "TEST WORDS PRECISION:\t$rawprecision\n";
}

close(TRUTH);
close(TEST);
unlink($tmp1);
unlink($tmp2);

print "=== SUMMARY:\n";
print "=== TOTAL INSERTIONS:\t$Ins\n";
print "=== TOTAL DELETIONS:\t$Del\n";
print "=== TOTAL SUBSTITUTIONS:\t$Subst\n";
print "=== TOTAL NCHANGE:\t$Tot\n";
print "=== TOTAL TRUE WORD COUNT:\t$Truecount\n";
print "=== TOTAL TEST WORD COUNT:\t$Testcount\n";
$RawRecall =  $RawRecall/$Truecount;
$RawPrecision = $RawPrecision/$Testcount;
$beta = 1;
$R = $RawRecall;
$P = $RawPrecision;
$F = (1 + $beta)*$P*$R/($beta * $P + $R);
$F = sprintf("%2.3f", $F);
$RawRecall = sprintf("%2.3f", $RawRecall);
$RawPrecision = sprintf("%2.3f", $RawPrecision);
print "=== TOTAL TRUE WORDS RECALL:\t$RawRecall\n";
print "=== TOTAL TEST WORDS PRECISION:\t$RawPrecision\n";
print "=== F MEASURE:\t$F\n";
if ($OOV > 0) {
    $OOVMISSED = sprintf("%2.3f", 1 - $OOVMISSED / $OOV);
}
else {
    $OOVMISSED = "--";
}
$OOV = sprintf("%2.3f", $OOV / $Truecount);
if ($IV > 0) {
    $IVMISSED = sprintf("%2.3f", 1 - $IVMISSED / $IV);
}
else {
    $IVMISSED = "--";
}
print "=== OOV Rate:\t$OOV\n";
print "=== OOV Recall Rate:\t$OOVMISSED\n";
print "=== IV Recall Rate:\t$IVMISSED\n";

print "###\t$file2\t$Ins\t$Del\t$Subst\t$Tot\t$Truecount\t$Testcount\t$RawRecall\t$RawPrecision\t$F\t$OOV\t$OOVMISSED\t$IVMISSED\n";
exit(0);
