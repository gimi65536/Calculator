#include "RatioNumber.h"

#ifndef _RATIOMATH_
#define _RATIOMATH_

numeric fast_sin(const numeric& r, int time, int precis = DEFAULT_endure_precision){
	fast_zero_signal = false;
	numeric sol;
	int plus = 0, tmp = precis, continuous_zero = 0;
	while(tmp >= 2){
		plus ++;
		tmp /= 10;
	}
	fast_start("SIN", sol, precis, precis + plus, 0);
	bnint base = 1, now_n = r.get_numerator(), now_d = r.get_denominator();
	const bnint base_n = now_n ^ 2, base_d = now_d ^ 2;
	for(int i = 0;i < time;i++, now_n *= base_n, now_d *= base_d, base *= (2 * i + 1) * 2 * i){
		if(i % 2 != 0){
			fast_add(-now_n, now_d * base);
		}else{
			fast_add(now_n, now_d * base);
		}
		if(fast_zero_signal){
			fast_zero_signal = false;
			if(continuous_zero >= 1){
				break;
			}
			continuous_zero ++;
		}else{
			continuous_zero = 0;
		}
		if(i == time - 2){
			fast_putin_temp();
		}
	}
	fast_end();
	return sol;
}

numeric fast_cos(const numeric& r, int time, int precis = DEFAULT_endure_precision){
	fast_zero_signal = false;
	numeric sol;
	int plus = 0, tmp = precis, continuous_zero = 0;
	while(tmp >= 2){
		plus ++;
		tmp /= 10;
	}
	fast_start("COS", sol, precis, precis + plus, 0);
	bnint base = 1, now_n = 1, now_d = 1;
	const bnint base_n = r.get_numerator() ^ 2, base_d = r.get_denominator() ^ 2;
	for(int i = 0;i < time;i++, now_n *= base_n, now_d *= base_d, base *= (2 * i - 1) * 2 * i){
		if(i % 2 != 0){
			fast_add(-now_n, now_d * base);
		}else{
			fast_add(now_n, now_d * base);
		}
		if(fast_zero_signal){
			fast_zero_signal = false;
			if(continuous_zero >= 1){
				break;
			}
			continuous_zero ++;
		}else{
			continuous_zero = 0;
		}
		if(i == time - 2){
			fast_putin_temp();
		}
	}
	fast_end();
	return sol;
}

numeric fast_tan(const numeric& r, int time, int precis = DEFAULT_endure_precision){
	numeric sol = fast_sin(r, time, precis + 10), sol1 = fast_cos(r, time, precis + 10);
	sol /= sol1;
	return sol;
}

numeric fast_arcsin(const numeric& r, int time, int precis = DEFAULT_endure_precision){
	fast_zero_signal = false;
	numeric sol;
	int plus = 0, tmp = precis, continuous_zero = 0;
	while(tmp >= 2){
		plus ++;
		tmp /= 10;
	}
	fast_start("ARCSINE", sol, precis, precis + plus, 0);
	bnint now_n = r.get_numerator(), now_d = r.get_denominator();
	const bnint base_n = now_n ^ 2, base_d = now_d ^ 2;
	for(int i = 0, j = 1;i < time;i++, j += 2, now_n *= base_n * (2 * i - 1), now_d *= base_d * (2 * i)){
		fast_add(now_n, now_d * j);
		if(fast_zero_signal){
			fast_zero_signal = false;
			if(continuous_zero >= 1){
				break;
			}
			continuous_zero ++;
		}else{
			continuous_zero = 0;
		}
		if(i % 100 == 0){
			bnint Gcd = gcd(now_n, now_d);
			if(!Gcd.is_zero()){
				now_n /= Gcd;
				now_d /= Gcd;
			}
		}
	}
	fast_end();
	return sol;
}

numeric fast_arctan(const numeric& r, int time, int precis = DEFAULT_endure_precision){
	fast_zero_signal = false;
	numeric sol;
	int plus = 0, tmp = precis, continuous_zero = 0;
	while(tmp >= 2){
		plus ++;
		tmp /= 10;
	}
	fast_start("ARCTANGENT", sol, precis, precis + plus, 0);
	bnint now_n = r.get_numerator(), now_d = r.get_denominator();
	const bnint base_n = now_n ^ 2, base_d = now_d ^ 2;
	for(int i = 0, j = 1;i < time;i++, j += 2, now_n *= base_n, now_d *= base_d){
		if(i % 2 != 0){
			fast_add(-now_n, now_d * j);
		}else{
			fast_add(now_n, now_d * j);
		}
		if(fast_zero_signal){
			fast_zero_signal = false;
			if(continuous_zero >= 1){
				break;
			}
			continuous_zero ++;
		}else{
			continuous_zero = 0;
		}
		if(i == time - 2){
			fast_putin_temp();
		}
	}
	fast_end();
	return sol;
}

numeric fast_arctan2(const numeric& r, int time, int precis = DEFAULT_endure_precision){
	fast_zero_signal = false;
	numeric sol;
	int plus = 0, tmp = precis, continuous_zero = 0;
	while(tmp >= 2){
		plus ++;
		tmp /= 10;
	}
	fast_start("ARCTANGENT", sol, precis, precis + plus, 0);
	numeric one(1, 1);
	numeric one_plus_r_r = (r * r + one);
	bnint now_n = r.get_numerator() * one_plus_r_r.get_denominator(), now_d = r.get_denominator() * one_plus_r_r.get_numerator();
	const bnint base_n = (r.get_numerator() ^ 2) * one_plus_r_r.get_denominator(), base_d = (r.get_denominator() ^ 2) * one_plus_r_r.get_numerator();
	for(int i = 0;i < time;i++, now_n *= base_n * 4 * i * i, now_d *= base_d * 2 * i * (2 * i + 1)){
		fast_add(now_n, now_d);
		if(fast_zero_signal){
			fast_zero_signal = false;
			if(continuous_zero >= 1){
				break;
			}
			continuous_zero ++;
		}else{
			continuous_zero = 0;
		}
		if(i % 100 == 0){
			bnint Gcd = gcd(now_n, now_d);
			if(!Gcd.is_zero()){
				now_n /= Gcd;
				now_d /= Gcd;
			}
		}
	}
	fast_end();
	return sol;
}

numeric fast_exp(const numeric& r, int time, int precis = DEFAULT_endure_precision){
	fast_zero_signal = false;
	numeric sol;
	int plus = 0, tmp = precis, continuous_zero = 0;
	while(tmp >= 2){
		plus ++;
		tmp /= 10;
	}
	fast_start("EXPONENTIAL", sol, precis, precis + plus, 0);
	bnint now_n = 1, now_d = 1;
	const bnint base_n = r.get_numerator(), base_d = r.get_denominator();
	for(int i = 0;i < time;i++, now_n *= base_n, now_d *= base_d * i){
		fast_add(now_n, now_d);
		if(fast_zero_signal){
			fast_zero_signal = false;
			if(continuous_zero >= 1){
				break;
			}
			continuous_zero ++;
		}else{
			continuous_zero = 0;
		}
	}
	fast_end();
	return sol;
}

#endif