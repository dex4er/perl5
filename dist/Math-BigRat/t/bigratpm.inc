#include this file into another test for subclass testing...

is ($class->config()->{lib},$CL);

$setup = '';

while (<DATA>)
  {
  chomp;
  $_ =~ s/#.*$//;	# remove comments
  $_ =~ s/\s+$//;	# trailing spaces
  next if /^$/;		# skip empty lines & comments
  if (s/^&//)
    {
    $f = $_;
    }
  elsif (/^\$/)
    {
    $setup = $_; $setup =~ s/\$/\$${class}::/g;	# round_mode, div_scale
    #print "\$setup== $setup\n";
    }
  else
    {
    if (m|^(.*?):(/.+)$|)
      {
      $ans = $2;
      @args = split(/:/,$1,99);
      }
    else
      {
      @args = split(/:/,$_,99); $ans = pop(@args);
      }
    $try = "\$x = new $class \"$args[0]\";";
    if ($f eq "bnorm")
      {
        $try .= "\$x;";
      } elsif ($f eq "finf") {
        my $a = $args[1] || '';
        $try .= "\$x->binf('$a');";
      } elsif ($f eq "is_inf") {
        $try .= "\$x->is_inf('$args[1]');";
      } elsif ($f eq "fone") {
        $try .= "\$x->bone('$args[1]');";
      } elsif ($f eq "fstr") {
        $try .= "\$x->accuracy($args[1]); \$x->precision($args[2]);";
        $try .= '$x->bstr();';
      } elsif ($f eq "parts") {
        # ->bstr() to see if an object is returned
        $try .= '($a,$b) = $x->parts(); $a = $a->bstr(); $b = $b->bstr();';
        $try .= '"$a $b";';
      } elsif ($f eq "numerator") {
        # ->bstr() to see if an object is returned
        $try .= '$x->numerator()->bstr();';
      } elsif ($f eq "denominator") {
        # ->bstr() to see if an object is returned
        $try .= '$x->denominator()->bstr();';
      } elsif ($f =~ /^(length|numify)$/) {
        $try .= "\$x->$f();";
      # some unary ops (can't test the fxxx form, since no AUTOLOAD in BigRat)
      } elsif ($f =~ /^f(nan|sstr|neg|floor|ceil|abs)$/) {
        $try .= "\$x->b$1();";
      # some is_xxx test function
      } elsif ($f =~ /^is_(zero|one|pos|neg|negative|positive|odd|even|nan|int)\z/) {
        $try .= "\$x->$f();";
      } elsif ($f =~ /^(as_number|as_int)\z/){
        $try .= "\$x->$1();";
      } elsif ($f eq "finc") {
        $try .= '++$x;';
      } elsif ($f eq "fdec") {
        $try .= '--$x;';
      } elsif ($f eq "digit") {
        $try .= "\$x->digit($args[1]);";
      } elsif ($f eq "fround") {
        $try .= "$setup; \$x->bround($args[1]);";
      } elsif ($f eq "ffround") {
        $try .= "$setup; \$x->bfround($args[1]);";
      } elsif ($f eq "fsqrt") {
        $try .= "$setup; \$x->bsqrt();";
      } elsif ($f eq "flog") {
        $try .= "$setup; \$x->blog();";
      } elsif ($f eq "ffac") {
        $try .= "$setup; \$x->bfac();";
      }
    else
      {
      $try .= "\$y = new $class \"$args[1]\";";
      if ($f eq "bcmp") {
        $try .= '$x <=> $y;';
      } elsif ($f eq "bacmp") {
        $try .= '$x->bacmp($y);';
      } elsif ($f eq "bpow") {
        $try .= '$x ** $y;';
      } elsif ($f eq "fpow") {
        $try .= '$x->bpow($y);';
      } elsif ($f eq "badd") {
        $try .= '$x + $y;';
      } elsif ($f eq "bsub") {
        $try .= '$x - $y;';
      } elsif ($f eq "bmul") {
        $try .= '$x * $y;';
      } elsif ($f eq "bdiv") {
        $try .= "$setup; \$x / \$y;";
      } elsif ($f eq "fdiv-list") {
        $try .= "$setup; join(',',\$x->bdiv(\$y));";
      } elsif ($f eq "brsft") {
        $try .= '$x >> $y;';
      } elsif ($f eq "blsft") {
        $try .= '$x << $y;';
      } elsif ($f eq "bmod") {
        $try .= '$x % $y;';
      } elsif( $f eq "bmodinv") {
       $try .= "\$x->bmodinv(\$y);";
      } elsif( $f eq "blog") {
       $try .= "\$x->blog(\$y);";
      } else {
        $try .= "\$z = $class->new(\"$args[2]\");";

        # Functions with three arguments
        if( $f eq "bmodpow") {
          $try .= "\$x->bmodpow(\$y,\$z);";
        } else { warn "Unknown op '$f'"; }
      }
    }
    # print "# Trying: '$try'\n";
    $ans1 = eval $try;
    if ($ans =~ m|^/(.*)$|)
      {
      my $pat = $1;
      like ($ans1, qr/$pat/);
      }
    else
      {
      if ($ans eq "")
        {
        is ($ans1, undef);
        }
      else
        {
	is ($ans1, $ans) or diag("Tried: '$try'");
#        if (ref($ans1) eq "$class")
#	  {
#	  # float numbers are normalized (for now), so mantissa shouldn't have
#	  # trailing zeros
#	  #print $ans1->_trailing_zeros(),"\n";
#          print "# Has trailing zeros after '$try'\n"
#	   if !is ($ans1->{_m}->_trailing_zeros(), 0);
#	  }
        }
      } # end pattern or string
    }
  } # end while

# check whether $class->new( Math::BigInt->new()) destroys it
# ($y == 12 in this case)
$x = Math::BigInt->new(1200); $y = $class->new($x);
is ($y,1200); is ($x,1200);

###############################################################################
# zero,inf,one,nan

$x = $class->new(2); $x->bzero(); is ($x->{_a}, undef); is ($x->{_p}, undef);
$x = $class->new(2); $x->binf();  is ($x->{_a}, undef); is ($x->{_p}, undef);
$x = $class->new(2); $x->bone();  is ($x->{_a}, undef); is ($x->{_p}, undef);
$x = $class->new(2); $x->bnan();  is ($x->{_a}, undef); is ($x->{_p}, undef);

__DATA__
&digit
123:2:1
1234:0:4
1234:1:3
1234:2:2
1234:3:1
1234:-1:1
1234:-2:2
1234:-3:3
1234:-4:4
0:0:0
0:1:0
&bmodinv
# format: number:modulus:result
# bmodinv Data errors
abc:abc:NaN
abc:5:NaN
5:abc:NaN
# bmodinv Expected Results from normal use
1:5:1
3:5:2
3:-5:-3
-2:5:2
8:5033:4404
1234567891:13:6
-1234567891:13:7
324958749843759385732954874325984357439658735983745:2348249874968739:1741662881064902
## bmodinv Error cases / useless use of function
inf:5:NaN
5:inf:NaN
-inf:5:NaN
5:-inf:NaN
&as_number
144/7:20
12/1:12
-12/1:-12
-12/3:-4
NaN:NaN
+inf:inf
-inf:-inf
&as_int
144/7:20
12/1:12
-12/1:-12
-12/3:-4
NaN:NaN
+inf:inf
-inf:-inf
&bmodpow
# format: number:exponent:modulus:result
# bmodpow Data errors
abc:abc:abc:NaN
5:abc:abc:NaN
abc:5:abc:NaN
abc:abc:5:NaN
5:5:abc:NaN
5:abc:5:NaN
abc:5:5:NaN
# bmodpow Expected results
0:0:2:1
1:0:2:1
0:0:1:0
8:7:5032:3840
8:-1:5033:4404
8:8:-5:-4
98436739867439843769485798542749827593285729587325:43698764986460981048259837659386739857456983759328457:6943857329857295827698367:3104744730915914415259518
# bmodpow Error cases
8:-1:16:NaN
inf:5:13:NaN
5:inf:13:NaN
&bmod
NaN:1:NaN
1:NaN:NaN
1:1:0
2:2:0
12:6:0
7/4:4/14:1/28
7/4:4/16:0
-7/4:4/16:0
-7/4:-4/16:0
7/4:-4/16:0
7/4:4/32:0
-7/4:4/32:0
-7/4:-4/32:0
7/4:-4/32:0
7/4:4/28:1/28
-7/4:4/28:-1/28
7/4:-4/28:1/28
-7/4:-4/28:-1/28
&fsqrt
1:1
0:0
NaN:NaN
+inf:inf
-inf:NaN
144:12
# sqrt(144) / sqrt(4) = 12/2 = 6/1
144/4:6
25/16:5/4
-3:NaN
&flog
NaN:NaN
0:NaN
-2:NaN
&blog
NaN:NaN:NaN
0:NaN:NaN
NaN:0:NaN
NaN:1:NaN
1:NaN:NaN
0:2:NaN
0:-2:NaN
3:-2:NaN
&finf
1:+:inf
2:-:-inf
3:abc:inf
&numify
0:0
+1:1
1234:1234
3/4:0.75
5/2:2.5
3/2:1.5
5/4:1.25
NaN:NaN
+inf:inf
-inf:-inf
&fnan
abc:NaN
2:NaN
-2:NaN
0:NaN
&fone
2:+:1
-2:-:-1
-2:+:1
2:-:-1
0::1
-2::1
abc::1
2:abc:1
&fsstr
+inf:inf
-inf:-inf
abcfsstr:NaN
1:1/1
3/1:3/1
0.1:1/10
&bnorm
1:1
-0:0
bnormNaN:NaN
+inf:inf
-inf:-inf
inf/inf:NaN
5/inf:0
5/-inf:0
inf/5:inf
-inf/5:-inf
inf/-5:-inf
-inf/-5:inf
123:123
-123.4567:-1234567/10000
# invalid inputs
1__2:NaN
1E1__2:NaN
11__2E2:NaN
#1.E3:NaN
.2E-3.:NaN
#1e3e4:NaN
.2E2:20
inf:inf
+inf:inf
-inf:-inf
+infinity:NaN
+-inf:NaN
abc:NaN
   1 a:NaN
1bcd2:NaN
11111b:NaN
+1z:NaN
-1z:NaN
0:0
+0:0
+00:0
+0_0_0:0
000000_0000000_00000:0
-0:0
-0000:0
+1:1
+01:1
+001:1
+00000100000:100000
+00000800/00000010:80
-00000800/00000010:-80
+00000800/-00000010:-80
-00000800/-00000010:80
123456789:123456789
-1:-1
-01:-1
-001:-1
-123456789:-123456789
-00000100000:-100000
123.456a:NaN
123.456:15432/125
0.01:1/100
.002:1/500
+.2:1/5
-0.0003:-3/10000
-.0000000004:-1/2500000000
123456E2:12345600
123456E-2:30864/25
-123456E2:-12345600
-123456E-2:-30864/25
1e1:10
2e-11:1/50000000000
12/10:6/5
0.1/0.1:1
100/0.1:1000
0.1/10:1/100
1 / 3:1/3
1/ 3:1/3
1 /3:1/3
&fneg
fnegNaN:NaN
+inf:-inf
-inf:inf
+0:0
+1:-1
-1:1
+123456789:-123456789
-123456789:123456789
+123.456789:-123456789/1000000
-123456.789:123456789/1000
123/7:-123/7
-123/7:123/7
123/-7:123/7
&fabs
fabsNaN:NaN
+inf:inf
-inf:inf
+0:0
+1:1
-1:1
+123456789:123456789
-123456789:123456789
+123.456789:123456789/1000000
-123456.789:123456789/1000
&badd
abc:abc:NaN
abc:+0:NaN
+0:abc:NaN
+inf:-inf:NaN
-inf:+inf:NaN
+inf:+inf:inf
-inf:-inf:-inf
baddNaN:+inf:NaN
baddNaN:+inf:NaN
+inf:baddNaN:NaN
-inf:baddNaN:NaN
+0:+0:0
+1:+0:1
+0:+1:1
+1:+1:2
-1:+0:-1
+0:-1:-1
-1:-1:-2
-1:+1:0
+1:-1:0
+9:+1:10
+99:+1:100
+999:+1:1000
+9999:+1:10000
+99999:+1:100000
+999999:+1:1000000
+9999999:+1:10000000
+99999999:+1:100000000
+999999999:+1:1000000000
+9999999999:+1:10000000000
+99999999999:+1:100000000000
+10:-1:9
+100:-1:99
+1000:-1:999
+10000:-1:9999
+100000:-1:99999
+1000000:-1:999999
+10000000:-1:9999999
+100000000:-1:99999999
+1000000000:-1:999999999
+10000000000:-1:9999999999
+123456789:+987654321:1111111110
-123456789:+987654321:864197532
-123456789:-987654321:-1111111110
+123456789:-987654321:-864197532
1/3:1/3:2/3
2/3:-1/3:1/3
&bsub
abc:abc:NaN
abc:+0:NaN
+0:abc:NaN
+inf:-inf:inf
-inf:+inf:-inf
+inf:+inf:NaN
-inf:-inf:NaN
baddNaN:+inf:NaN
baddNaN:+inf:NaN
+inf:baddNaN:NaN
-inf:baddNaN:NaN
+0:+0:0
+1:+0:1
+0:+1:-1
+1:+1:0
-1:+0:-1
+0:-1:1
-1:-1:0
-1:+1:-2
+1:-1:2
+9:+1:8
+99:+1:98
+999:+1:998
+9999:+1:9998
+99999:+1:99998
+999999:+1:999998
+9999999:+1:9999998
+99999999:+1:99999998
+999999999:+1:999999998
+9999999999:+1:9999999998
+99999999999:+1:99999999998
+10:-1:11
+100:-1:101
+1000:-1:1001
+10000:-1:10001
+100000:-1:100001
+1000000:-1:1000001
+10000000:-1:10000001
+100000000:-1:100000001
+1000000000:-1:1000000001
+10000000000:-1:10000000001
+123456789:+987654321:-864197532
-123456789:+987654321:-1111111110
-123456789:-987654321:864197532
+123456789:-987654321:1111111110
2/3:1/3:1/3
7/27:3/54:11/54
-2/3:+2/3:-4/3
-2/3:-2/3:0
0:-123:123
0:123:-123
&bmul
abc:abc:NaN
abc:+0:NaN
+0:abc:NaN
+inf:NaNmul:NaN
+inf:NaNmul:NaN
NaNmul:+inf:NaN
NaNmul:-inf:NaN
+inf:+inf:inf
+inf:-inf:-inf
+inf:-inf:-inf
+inf:+inf:inf
+inf:123.34:inf
+inf:-123.34:-inf
-inf:123.34:-inf
-inf:-123.34:inf
123.34:+inf:inf
-123.34:+inf:-inf
123.34:-inf:-inf
-123.34:-inf:inf
+0:+0:0
+0:+1:0
+1:+0:0
+0:-1:0
-1:+0:0
+123456789123456789:+0:0
+0:+123456789123456789:0
-1:-1:1
-1:+1:-1
+1:-1:-1
+1:+1:1
+2:+3:6
-2:+3:-6
+2:-3:-6
-2:-3:6
+111:+111:12321
+10101:+10101:102030201
+1001001:+1001001:1002003002001
+100010001:+100010001:10002000300020001
+10000100001:+10000100001:100002000030000200001
+11111111111:+9:99999999999
+22222222222:+9:199999999998
+33333333333:+9:299999999997
+44444444444:+9:399999999996
+55555555555:+9:499999999995
+66666666666:+9:599999999994
+77777777777:+9:699999999993
+88888888888:+9:799999999992
+99999999999:+9:899999999991
6:120:720
10:10000:100000
1/4:1/3:1/12
&bdiv
$div_scale = 40; $round_mode = 'even'
abc:abc:NaN
abc:+1:abc:NaN
+1:abc:NaN
-1:abc:NaN
0:abc:NaN
+0:+0:NaN
+0:+1:0
+1:+0:inf
+3214:+0:inf
+0:-1:0
-1:+0:-inf
-3214:+0:-inf
+1:+1:1
-1:-1:1
+1:-1:-1
-1:+1:-1
+1:+2:1/2
+2:+1:2
123:+inf:0
123:-inf:0
+10:+5:2
+100:+4:25
+1000:+8:125
+10000:+16:625
+10000:-16:-625
+999999999999:+9:111111111111
+999999999999:+99:10101010101
+999999999999:+999:1001001001
+999999999999:+9999:100010001
+999999999999999:+99999:10000100001
+1000000000:+9:1000000000/9
+2000000000:+9:2000000000/9
+3000000000:+9:1000000000/3
+4000000000:+9:4000000000/9
+5000000000:+9:5000000000/9
+6000000000:+9:2000000000/3
+7000000000:+9:7000000000/9
+8000000000:+9:8000000000/9
+9000000000:+9:1000000000
+35500000:+113:35500000/113
+71000000:+226:35500000/113
+106500000:+339:35500000/113
+1000000000:+3:1000000000/3
2:25.024996000799840031993601279744051189762:1000000000000000000000000000000000000000/12512498000399920015996800639872025594881
123456:1:123456
1/4:1/3:3/4
# reset scale for further tests
$div_scale = 40
&is_nan
123:0
abc:1
NaN:1
-123:0
&is_inf
+inf::1
-inf::1
abc::0
1::0
NaN::0
-1::0
+inf:-:0
+inf:+:1
-inf:-:1
-inf:+:0
# it must be exactly /^[+-]inf$/
+infinity::0
-infinity::0
&is_odd
abc:0
0:0
-1:1
-3:1
1:1
3:1
1000001:1
1000002:0
+inf:0
-inf:0
123.45:0
-123.45:0
2:0
&is_int
NaNis_int:0
0:1
1:1
2:1
-2:1
-1:1
-inf:0
+inf:0
123.4567:0
-0.1:0
-0.002:0
1/3:0
3/1:1
&is_even
abc:0
0:1
-1:0
-3:0
1:0
3:0
1000001:0
1000002:1
2:1
+inf:0
-inf:0
123.456:0
-123.456:0
0.01:0
-0.01:0
120:1
1200:1
-1200:1
&is_pos
0:0
1:1
-1:0
-123:0
NaN:0
-inf:0
+inf:1
&is_positive
0:0
1:1
-1:0
-123:0
NaN:0
-inf:0
+inf:1
&is_neg
0:0
1:0
-1:1
-123:1
NaN:0
-inf:1
+inf:0
&is_negative
0:0
1:0
-1:1
-123:1
NaN:0
-inf:1
+inf:0
&parts
0:0 1
1:1 1
123:123 1
-123:-123 1
-1200:-1200 1
5/7:5 7
-5/7:-5 7
NaNparts:NaN NaN
+inf:inf inf
-inf:-inf inf
&length
123:3
-123:3
0:1
1:1
12345678901234567890:20
&is_zero
NaNzero:0
+inf:0
-inf:0
0:1
-1:0
1:0
0/3:1
1/3:0
-0/3:1
5/inf:1
&is_one
NaNone:0
+inf:0
-inf:0
0:0
2:0
1:1
-1:0
-2:0
1/3:0
100/100:1
0.1/0.1:1
5/inf:0
&ffloor
0:0
abc:NaN
+inf:inf
-inf:-inf
1:1
-51:-51
-51.2:-52
12.2:12
3/7:0
6/7:0
7/7:1
8/7:1
13/7:1
14/7:2
15/7:2
-3/7:-1
-6/7:-1
-7/1:-7
-8/7:-2
-13/7:-2
-14/7:-2
-15/7:-3
&fceil
0:0
abc:NaN
+inf:inf
-inf:-inf
1:1
-51:-51
-51.2:-51
12.2:13
3/7:1
6/7:1
8/7:2
13/7:2
14/7:2
15/7:3
-3/7:0
-6/7:0
-8/7:-1
-13/7:-1
-14/7:-2
-15/7:-2
&ffac
NaN:NaN
1:1
-1:NaN
&bpow
# bpow test for overload of **
2:2:4
3:3:27
&bacmp
+0:-0:0
+0:+1:-1
-1:+1:0
+1:-1:0
-1:+2:-1
+2:-1:1
-123456789:+987654321:-1
+123456789:-987654321:-1
+987654321:+123456789:1
-987654321:+123456789:1
-123:+4567889:-1
# NaNs
acmpNaN:123:
123:acmpNaN:
acmpNaN:acmpNaN:
# infinity
+inf:+inf:0
-inf:-inf:0
+inf:-inf:0
-inf:+inf:0
+inf:123:1
-inf:123:1
+inf:-123:1
-inf:-123:1
+inf:1/23:1
-inf:1/23:1
+inf:-1/23:1
-inf:-1/23:1
+inf:12/3:1
-inf:12/3:1
+inf:-12/3:1
-inf:-12/3:1
123:inf:-1
-123:inf:-1
123:-inf:-1
-123:-inf:-1
1/23:inf:-1
-1/23:inf:-1
1/23:-inf:-1
-1/23:-inf:-1
12/3:inf:-1
-12/3:inf:-1
12/3:-inf:-1
-12/3:-inf:-1
# return undef
+inf:NaN:
NaN:inf:
-inf:NaN:
NaN:-inf:
1/3:2/3:-1
2/3:1/3:1
2/3:2/3:0
&fpow
2/1:3/1:8
3/1:3/1:27
5/2:3/1:125/8
-2/1:3/1:-8
-3/1:3/1:-27
-5/2:3/1:-125/8
-2/1:4/1:16
-3/1:4/1:81
-5/2:4/1:625/16
-5/2:-4/1:16/625
1/5:-3:125
-1/5:-3:-125
&numerator
NaN:NaN
inf:inf
-inf:-inf
3/7:3
-3/7:-3
0:0
1:1
5/-3:-5
&denominator
NaN:NaN
inf:1
-inf:1
3/7:7
0:1
1/1:1
-1/1:1
-3/7:7
4/-5:5
&finc
3/2:5/2
-15/6:-3/2
NaN:NaN
-1/3:2/3
-2/7:5/7
&fdec
15/6:3/2
-3/2:-5/2
1/3:-2/3
2/7:-5/7
NaN:NaN
