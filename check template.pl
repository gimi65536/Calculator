#!/usr/bin/perl

use warnings;
use 5.010;

sub signtoregex{
	my $s = join("", map {
		my $tmp = $_; #copy, or you would modify original data
		$tmp =~ s/([^\w\$])/\\$1/g;
		$tmp
	} @_);
	return $s;
}

#one-char operators
@baseplusope        = qw(+ -);
@basemultiope       = qw(* / %);
@baseexpope         = qw(^);
@baseleftparentope  = qw/(/;
@baserightparentope = qw/)/;
@baseassignope      = qw(=);
@baseope = (@baseplusope, @basemultiope, @baseexpope, @baseleftparentope, @baserightparentope, @baseassignope);

#multi-char operators
%pairplusope        = ();
%pairmultiope       = ();
%pairexpope         = ();
%pairleftparentope  = ();
%pairrightparentope = ();
%pairassignope      = ('+=' => '`', '-=' => '!', '*=' => '@', '/=' => '\\', '%=' => '&');
%pairope = (%pairplusope, %pairmultiope, %pairexpope, %pairparentope, %pairassignope);

#operator grouped
@plusope        = (@baseplusope,        values %pairplusope);
@multiope       = (@basemultiope,       values %pairmultiope);
@expope         = (@baseexpope,         values %pairexpope);
@leftparentope  = (@baseleftparentope,  values %pairleftparentope);
@rightparentope = (@baserightparentope, values %pairrightparentope);
@assignope      = (@baseassignope,      values %pairassignope);
@ope = (@plusope, @multiope, @expope, @leftparentope, @rightparentope, @assignope);

#operator in regex
$baseope_regex        = &signtoregex(@baseope);
$plusope_regex        = &signtoregex(@plusope);
$multiope_regex       = &signtoregex(@multiope);
$expope_regex         = &signtoregex(@expope);
$leftparentope_regex  = &signtoregex(@leftparentope);
$rightparentope_regex = &signtoregex(@rightparentope);
$assignope_regex      = &signtoregex(@assignope);
$ope_regex            = &signtoregex(@ope);
%pairope_regex = map{
	my ($tmp, $value) = ($_, $pairope{$_});
	&signtoregex($_) => &signtoregex($value)
} keys %pairope;

$varcontent_regex  = '\w\$'; #A-Za-z0-9_
$varinit_regex     = 'A-Za-z_\$';
$varinitonly_regex = ''; #varinit - varcontent
$varcontonly_regex = '\d'; #varcontent - varinit
$vartotal_regex    = '\w\$'; #varinit + varcontent

#$varcontent_regex  = '\w'; #A-Za-z0-9_
#$varinit_regex     = '\$';
#$varinitonly_regex = '\$';
#$varcontonly_regex = '\w';
#$vartotal_regex    = '\w\$';

$s = "  4a23a23 aaa111_\$+ab+a'23'5 2a ```+=5{-|1|+|1waifu} ";
#$s = "z=a b(c=5)";
$s = 'a=0b';
say $s;
#A00.1
#$s =~ s/[^\w\$\s+\-*\/%^\(\)=]//g;
$s =~ s/[^\d${varinit_regex}${varcontent_regex}${baseope_regex}\s]//g;
say $s;
#A00.2
foreach $i (keys %pairope){
	my $tmp = &signtoregex($i);
	$s =~ s/$tmp/${pairope{$i}}/g;
}
say $s;
#A00.3
#$s =~ s/([+\-*\/%^=\(\)^`!@\\&])/ $1 /g;
$s =~ s/([${ope_regex}])/ $1 /g;
say $s;
#A00.4
#$s =~ s/([A-Za-z_\$][^\s]*)(?:\s|$)/ $1  /g;
#$s =~ s/([${varinit_regex}][^\s]*)(?:[${varinitonly_regex}\s]|$)/ $1  /g;
$s =~ s/([${varinit_regex}][$varcontent_regex]*)/ $1 /g;
say $s;
#A00.5
say "A00 error" if $s =~ /(?:^|\s)(?=[$varcontonly_regex])(?=[^\d])|(?:^|\s)\d[^\s]*?(?=[^\d])(?=[$varcontonly_regex])/;
#A00.6
$s =~ s/^\s+|\s+$|([^\s]\s)\s+(?=[^\s])/$1$2/g;
say $s;

say "A01 error" if $s !~ /[^\s]/;
say "A02 error" if $s =~ /^[^$vartotal_regex$plusope_regex$leftparentope_regex]/;
say "A03 error" if $s =~ /[^$vartotal_regex$rightparentope_regex]$/;
say "A04 error" if $s =~ /[$plusope_regex$multiope_regex$expope_regex$leftparentope_regex$assignope_regex]\s[$multiope_regex$expope_regex]/;
say "A05 error" if $s =~ /[^$vartotal_regex$rightparentope_regex]\s[$rightparentope_regex]/;
say "A06 error" if $s =~ /[^$vartotal_regex]\s[$assignope_regex]/;
say "A07 error at $-[0] $+[0]" if $s =~ /[^$vartotal_regex$leftparentope_regex$assignope_regex\s][^$leftparentope_regex]*?[$assignope_regex]/;
say "A08 error-1" if $s =~ /(?:^|\s)\d[^$leftparentope_regex]*?[$assignope_regex]/;
say "A08 error-2" if $s =~ /(?:^|[$leftparentope_regex$assignope_regex]\s)[$varinit_regex][$vartotal_regex\s]*?\s[$varinit_regex][$vartotal_regex\s]*?[$assignope_regex]/;
while($s =~ /([$varinit_regex][$varcontent_regex]*?)(?=\s(.)|$)/g){
	#if($s2 is not in baseassignope and $s1 is not defined){say "A09 error";}
}


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#


#one-char operators
@baseplusope        = qw(+ -);
@basemultiope       = qw(* / %);
@baseexpope         = qw();
@baseleftparentope  = qw/(/;
@baserightparentope = qw/)/;
@baseassignope      = qw(=);
@baseope = (@baseplusope, @basemultiope, @baseexpope, @baseleftparentope, @baserightparentope, @baseassignope);

#multi-char operators
%pairplusope        = ();
%pairmultiope       = ();
%pairexpope         = ();#('**' => '^')
%pairleftparentope  = ();
%pairrightparentope = ();
%pairassignope      = ('+=' => '`', '-=' => '!', '*=' => '@', '/=' => '\\', '%=' => '&');
%pairope = (%pairplusope, %pairmultiope, %pairexpope, %pairparentope, %pairassignope);

#operator grouped
@plusope        = (@baseplusope,        values %pairplusope);
@multiope       = (@basemultiope,       values %pairmultiope);
@expope         = (@baseexpope,         values %pairexpope);
@leftparentope  = (@baseleftparentope,  values %pairleftparentope);
@rightparentope = (@baserightparentope, values %pairrightparentope);
@assignope      = (@baseassignope,      values %pairassignope);
@ope = (@plusope, @multiope, @expope, @leftparentope, @rightparentope, @assignope);

#operator in regex
$baseope_regex        = &signtoregex(@baseope);
$plusope_regex        = &signtoregex(@plusope);
$multiope_regex       = &signtoregex(@multiope);
$expope_regex         = &signtoregex(@expope);
$leftparentope_regex  = &signtoregex(@leftparentope);
$rightparentope_regex = &signtoregex(@rightparentope);
$assignope_regex      = &signtoregex(@assignope);
$ope_regex            = &signtoregex(@ope);
%pairope_regex = map{
	my ($tmp, $value) = ($_, $pairope{$_});
	&signtoregex($_) => &signtoregex($value)
} keys %pairope;

@separator = qw(_);#_ in Py

$varcontent_regex  = '\w\$'; #A-Za-z0-9_
$varinit_regex     = 'A-Za-z_\$';
$varinitonly_regex = '';
$varcontonly_regex = '\d';
$vartotal_regex    = '\w\$';
$separator_regex = &signtoregex(@separator);

#$s = "  4a23a23 aaa111_\$+ab+a'23'5 2a ```+=5{-|1|+|1waifu} ";
$s = "z3b+3_2_3_5=a -b+(c=5)";
say $s;
#B00.1
say "B00 error-1" if $s =~ /[^\d${vartotal_regex}${baseope_regex}${separator_regex}\s]/;
#B00.2
foreach $i (keys %pairope){
	my $tmp = &signtoregex($i);
	$s =~ s/$tmp/${pairope{$i}}/g;
}
say $s;
#A00.3
$s =~ s/([${ope_regex}])/ $1 /g;
say $s;
#A00.4
#say "B00 error-2" if $s =~ /(?:^|\s)(?=[$varcontonly_regex])(?=[^\d])|(?:^|\s)\d[^\s]*?(?=[^\d])(?=[$varcontonly_regex])/;
#A00.5
$s =~ s/^\s+|\s+$|([^\s]\s)\s+(?=[^\s])/$1$2/g;
say $s;

say "B01 error-1" if $s =~ /(?:^|\s)(?=[$separator_regex])(?=[^$varinit_regex])/;
say "B01 error-2" if $s =~ /(?:^|\s)[$varinit_regex][^\s]*?(?=[$separator_regex])(?=[^$varcontent_regex])/;
say "B01 error-3" if $s =~ /(?:^|\s)\d[\d$separator_regex]*?(?:\d[$separator_regex](?=[^\d])|[^\d][$separator_regex])/;
say "B02 error" if $s =~ /[^$ope_regex]\s[$leftparentope_regex]/;
say "B03 error" if $s =~ /[$rightparentope_regex]\s[^$ope_regex]/;
say "B04 error" if $s =~ /[^$ope_regex]\s[^$ope_regex]/;
say "B05 error" if $s =~ /[$leftparentope_regex]\s[$rightparentope_regex]|[$rightparentope_regex]\s[$leftparentope_regex]/;
say "B06 error" if scalar @{[$s =~ /[$leftparentope_regex]/g]} != scalar @{[$s =~ /[$rightparentope_regex]/g]};
while($s =~ /[$rightparentope_regex]/g){ #inefficient
	my $tmp = substr $s, 0, $-[0];
	if(scalar @{[$tmp =~ /[$leftparentope_regex]/g]} <= scalar @{[$tmp =~ /[$rightparentope_regex]/g]}){
		say "B07 error";
	}
}
if($s =~ /[$assignope_regex]/){ #Py does not care
	my $tmp = substr $s, $+[0];
	say $tmp;
	while($s =~ /([$varinit_regex][$varcontent_regex]*?)(?:\s|$)/g){
		#if($s1 is not defined){say "B08 error";}
	}
}

#@a;
#push @a, [$1, $2] while ($s =~ /(a)(2)/g);
#say $a[0][0];
#while($s =~ /(a)(2)/g){
#	say $1;
#}
#say $s;