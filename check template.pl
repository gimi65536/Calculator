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

$var_regex     = '\w\$'; #A-Za-z0-9_
$varinit_regex = 'A-Za-z_\$';

#$s = "  4a23a23 aaa111_\$+ab+a'23'5 2a ```+=5{-|1|+|1waifu} += 5-=5*=5/=5%=5aaaaaa";
$s = "5=(a += 5";
say $s;
#A00.1
#$s =~ s/[^\w\$\s+\-*\/%^\(\)=]//g;
$s =~ s/[^${var_regex}${baseope_regex}\s]//g;
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
$s =~ s/([${varinit_regex}][^\s]*)(?:\s|$)/ $1  /g;
say $s;
#A00.5
$s =~ s/^\s+|\s+$|([^\s]\s)\s+(?=[^\s])/$1$2/g;
say $s;

say "A01 error" if $s !~ /[^\s]/;
say "A02 error" if $s =~ /^[^$var_regex$plusope_regex$leftparentope_regex]/;
say "A03 error" if $s =~ /[^$var_regex$rightparentope_regex]$/;
say "A04 error" if $s =~ /[$plusope_regex$multiope_regex$expope_regex$leftparentope_regex$assignope_regex]\s[$multiope_regex$expope_regex]/;
say "A05 error" if $s =~ /[^$var_regex$rightparentope_regex]\s[$rightparentope_regex]/;
say "A06 error" if $s =~ /[^$var_regex]\s[$assignope_regex]/;
say "A07 error" if $s =~ /[^$var_regex$leftparentope_regex$assignope_regex\s].*?[$assignope_regex]/;

#@a;
#push @a, [$1, $2] while ($s =~ /(a)(2)/g);
#print $a[0][0];
#while($s =~ /(a)(2)/g){
#	print $1;
#}