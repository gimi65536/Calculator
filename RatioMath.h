#include "RatioNumber.h"
#include <stdexcept>

#ifndef _RATIOMATH_
#define _RATIOMATH_

numeric fast_exp(const numeric& r, int time, int precis = DEFAULT_endure_precision);
numeric fast_arctan2(const numeric& r, int time, int precis = DEFAULT_endure_precision);

numeric Euler(int precis){
	static numeric e = "2.7182818284590452353602874713526624977572470936999595749669676277240766303535"
	"47594571382178525166427427466391932003059921817413596629043572900334295260595630"
	"73813232862794349076323382988075319525101901157383418793070215408914993488416750"
	"92447614606680822648001684774118537423454424371075390777449920695517027618386062"
	"61331384583000752044933826560297606737113200709328709127443747047230696977209310"
	"14169283681902551510865746377211125238978442505695369677078544996996794686445490"
	"59879316368892300987931277361782154249992295763514822082698951936680331825288693"
	"98496465105820939239829488793320362509443117301238197068416140397019837679320683"
	"28237646480429531180232878250981945581530175671736133206981125099618188159304169"
	"03515988885193458072738667385894228792284998920868058257492796104841984443634632"
	"44968487560233624827041978623209002160990235304369941849146314093431738143640546"
	"25315209618369088870701676839642437814059271456354906130310720851038375051011574"
	"77041718986106873969655212671546889570350354021234078498193343210681701210056279";
	static int now_precis = 1036;
	if(precis > now_precis){
		now_precis = precis;
		e = fast_exp(1, 1000000, precis);
	}
	return e.approximate(precis);
}

numeric Napier(int precis){ //alia of Euler()
	return Euler(precis);
}

numeric Pi(int precis){
	static numeric pi = "3.141592653589793238462643383279502884197169399375105820974944592307816406286"
	"208998628034825342117067982148086513282306647093844609550582231725359408128481"
	"117450284102701938521105559644622948954930381964428810975665933446128475648233"
	"786783165271201909145648566923460348610454326648213393607260249141273724587006"
	"606315588174881520920962829254091715364367892590360011330530548820466521384146"
	"951941511609433057270365759591953092186117381932611793105118548074462379962749"
	"567351885752724891227938183011949129833673362440656643086021394946395224737190"
	"702179860943702770539217176293176752384674818467669405132000568127145263560827"
	"785771342757789609173637178721468440901224953430146549585371050792279689258923"
	"542019956112129021960864034418159813629774771309960518707211349999998372978049"
	"951059731732816096318595024459455346908302642522308253344685035261931188171010"
	"003137838752886587533208381420617177669147303598253490428755468731159562863882"
	"353787593751957781857780532171226806613001927876611195909216420198938095257201"
	"065485863278865936153381827968230301952035301852968995773622599413891249721775";
	static int now_precis = 1089;
	if(precis > now_precis){
		now_precis = precis;
		pi = fast_arctan2(1, 1000000, precis);
	}
	return pi.approximate(precis);
}

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
	if(!r.is_fraction() || r < -1 || r > 1){
		throw domain_error("Parameter in fast_arcsin should be between -1 and 1.");
	}
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

numeric fast_arctan2(const numeric& r, int time, int precis){
	fast_zero_signal = false;
	numeric sol;
	int plus = 0, tmp = precis, continuous_zero = 0;
	while(tmp >= 2){
		plus ++;
		tmp /= 10;
	}
	fast_start("ARCTANGENT", sol, precis, precis + plus, 0);
	numeric one_plus_r_r = (r * r + 1);
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

numeric fast_exp(const numeric& r, int time, int precis){
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

numeric fast_log(const numeric& r, int time, int precis = DEFAULT_endure_precision){
	if(!r.is_fraction() || !(r > 0)){
		throw domain_error("Parameter in fast_log should be large than 0.");
	}
	fast_zero_signal = false;
	numeric sol;
	int plus = 0, tmp = precis, continuous_zero = 0;
	while(tmp >= 2){
		plus ++;
		tmp /= 10;
	}
	numeric base = (r - 1) / (r + 1);
	fast_start("LOGARITHM", sol, precis, precis + plus, 0);
	bnint now_n = base.get_numerator(), now_d = base.get_denominator();
	const bnint base_n = now_n ^ 2, base_d = now_d ^ 2;
	for(int i = 0, j = 1;i < time;i++, j += 2, now_n *= base_n, now_d *= base_d){
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
		if(i % 10 == 0){
			bnint Gcd = gcd(now_n, now_d);
			if(!Gcd.is_zero()){
				now_n /= Gcd;
				now_d /= Gcd;
			}
		}
	}
	fast_end();
	return sol * 2;
}

numeric fast_sqrt(const numeric& r, int time, int precis = DEFAULT_endure_precision){
	if(!r.is_fraction() || r < 0){
		throw domain_error("Parameter in fast_sqrt should be equal or large to 0.");
	}
	numeric sol, sol1;
	sol = sol1 = r / 2 / (10_b ^ (((r.get_numerator() / r.get_denominator()).digit() - 1) / 2));
	int plus = 0, tmp = precis;
	while(tmp >= 2){
		plus ++;
		tmp /= 10;
	}
	for(int i = 0;i < time;i++){
		fast_start("SQRT", sol, precis, precis + plus, 0);
		fast_putin_temp();

		numeric tmp = r / sol1;
		fast_start("SQRT1", tmp, precis, precis + plus, 0);
		fast_minus(sol1);
		fast_end();

		fast_add(tmp);
		//fast_add(r / sol1 - sol1);
		fast_end();
		if(sol == sol1){
			return sol;
		}
		sol1 = sol;
	}
	return sol;
}

#endif