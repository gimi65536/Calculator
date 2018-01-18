sub signtoregex{
	my ($s) = @_;
	$s =~ s/([^\w\$])/\\$1/g;
	return $s;
}
@baseope = qw(+ - * / % ^ ( ) =);
%pairope = ('+=' => '`', '-=' => '!', '*=' => '@', '/=' => '\\', '%=' => '&');
@ope = (@baseope, values %pairope);
#print @ope, "\n";
@opeinregex = map {&signtoregex($_)} @ope;
#print @opeinregex, "\n";
%pairopeinregex = map{
	my ($tmp, $value) = ($_, $pairope{$_});
	&signtoregex($_) => &signtoregex($value)
} keys %pairope;
#print values %pairopeinregex, "\n";

$s = "  4a23a23 aaa111_\$+ab+a'23'5 2a ```+=5{-|1|+|1waifu} += 5-=5*=5/=5%=5aaaaaa";
print $s, "\n";
$s =~ s/[^\w\$\s+\-*\/%^\(\)=]//g;
print $s, "\n";
foreach $i (keys %pairope){
	my $tmp = &signtoregex($i);
	$s =~ s/$tmp/$pairope{$i}/g;
}
print $s, "\n";
$s =~ s/([+\-*\/%^=\(\)^`!@\\&])/ $1 /g;
print $s, "\n";
$s =~ s/([A-Za-z_\$][^\s]*)(?:\s|$)/ $1  /g;
print $s, "\n";
$s =~ s/^\s+|\s+$|([^\s]\s)\s+(?=[^\s])/$1$2/g;
print $s, "\n";