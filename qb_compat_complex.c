/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2012 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Chung Leong <cleong@cal.berkeley.edu>                        |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

float32_t cabsf(cfloat32_t n) {
	float32_t res;
	res = sqrtf(n.r * n.r + n.i * n.i);
	return res;
}

float64_t cabs(cfloat64_t n) {
	float64_t res;
	res = sqrt(n.r * n.r + n.i * n.i);
	return res;
}

float32_t cargf(cfloat32_t n) {
	float32_t res;
	res = atan2f(n.i, n.r);
	return res;
}

float64_t carg(cfloat64_t n) {
	float64_t res;
	res = atan2(n.i, n.r);
	return res;
}

cfloat32_t cacosf(cfloat32_t n) {
	static const float32_t one = (float32_t) 1;
	static const float32_t two = (float32_t) 2;
	static const float32_t half = (float32_t) 0.5;
	static const float32_t a_crossover = (float32_t) 10;
	static const float32_t b_crossover = (float32_t) 0.6417;
	static const float32_t s_pi = (float32_t) M_PI;
	static const float32_t half_pi = (float32_t) M_PI_2;
	static const float32_t log_two = (float32_t) M_LN2;
	static const float32_t quarter_pi = (float32_t) M_PI_4;
	
	float32_t x = fabsf(n.r);
	float32_t y = fabsf(n.i);
	cfloat32_t res;
	
	if(zend_isinf(x)) {
		if(zend_isinf(y)) {
			res.r = quarter_pi;
			res.i = x;
		} else if(zend_isnan(y)) {
			res.r = y;
			res.i = -x;
			return res;
		} else {
			res.r = 0;
			res.i = x;
		}
	} else if(zend_isnan(x)) {
		res.r = x;
		res.i = zend_isinf(y) ? -n.i : x;
		return res;
	} else if(zend_isinf(y)) {
		res.r = half_pi;
		res.i = y;
	} else if(zend_isnan(y)) {
		res.r = (x == 0) ? half_pi : y;
		res.i = y;
		return res;
	} else {
		if((y == 0) && (x <= one)) {
			res.r = (x == 0) ? half_pi : acosf(n.r);
			res.i = n.i * -1;
			return res;
		} else {
			float32_t safe_max = FLT_MAX / 8;
			float32_t safe_min = FLT_MIN / 4;
	
			float32_t xp1 = one + x;
			float32_t xm1 = x - one;
	
			if((x < safe_max) && (x > safe_min) && (y < safe_max) && (y > safe_min)) {
				float32_t yy = y * y;
				float32_t r = sqrtf(xp1*xp1 + yy);
				float32_t s = sqrtf(xm1*xm1 + yy);
				float32_t a = half * (r + s);
				float32_t b = x / a;
	
				if(b <= b_crossover) {
					res.r = acosf(b);
				} else {
					float32_t apx = a + x;
					if(x <= one) {
						res.r = atanf(sqrtf(half * apx * (yy /(r + xp1) + (s-xm1)))/x);
					} else {
						res.r = atanf((y * sqrtf(half * (apx/(r + xp1) + apx/(s+xm1))))/x);
					}
				}
	
				if(a <= a_crossover) {
					float32_t am1;
					if(x < one) {
						am1 = half * (yy/(r + xp1) + yy/(s - xm1));
					} else {
						am1 = half * (yy/(r + xp1) + (s + xm1));
					}
					res.i = log1pf(am1 + sqrtf(am1 * (a + one)));
				} else {
					res.i = logf(a + sqrtf(a*a - one));
				}
			} else {
				if(y <= (FLT_EPSILON * fabsf(xm1))) {
					if(x < one) {
						res.r = acosf(x);
						res.i = y / sqrtf(xp1*(one-x));
					} else {
						if((FLT_MAX / xp1) > xm1) {
							res.r = y / sqrtf(xm1*xp1);
							res.i = log1pf(xm1 + sqrtf(xp1*xm1));
						} else {
							res.r = y / x;
							res.i = log_two + logf(x);
						}
					}
				} else if(y <= safe_min) {
					res.r = sqrtf(y);
					res.i = sqrtf(y);
				} else if(FLT_EPSILON * y - one >= x) {
					res.r = half_pi;
					res.i = log_two + logf(y);
				} else if(x > one) {
					float32_t xoy = x/y;
					res.r = atanf(y/x);
					res.i = log_two + logf(y) + half * log1pf(xoy*xoy);
				} else {
					float32_t a = sqrtf(one + y*y);
					res.r = half_pi;
					res.i = half * log1pf(two *y*(y+a));
				}
			}
		}
	
	}
	if(signbit(n.r)) {
		res.r = s_pi - res.r;
	}
	if(!signbit(n.i)) {
		res.i = -1 * res.i;
	}
	return res;
}

cfloat64_t cacos(cfloat64_t n) {
	static const float64_t one = (float64_t) 1;
	static const float64_t two = (float64_t) 2;
	static const float64_t half = (float64_t) 0.5;
	static const float64_t a_crossover = (float64_t) 10;
	static const float64_t b_crossover = (float64_t) 0.6417;
	static const float64_t s_pi = (float64_t) M_PI;
	static const float64_t half_pi = (float64_t) M_PI_2;
	static const float64_t log_two = (float64_t) M_LN2;
	static const float64_t quarter_pi = (float64_t) M_PI_4;
	
	float64_t x = fabs(n.r);
	float64_t y = fabs(n.i);
	cfloat64_t res;
	
	if(zend_isinf(x)) {
		if(zend_isinf(y)) {
			res.r = quarter_pi;
			res.i = x;
		} else if(zend_isnan(y)) {
			res.r = y;
			res.i = -x;
			return res;
		} else {
			res.r = 0;
			res.i = x;
		}
	} else if(zend_isnan(x)) {
		res.r = x;
		res.i = zend_isinf(y) ? -n.i : x;
		return res;
	} else if(zend_isinf(y)) {
		res.r = half_pi;
		res.i = y;
	} else if(zend_isnan(y)) {
		res.r = (x == 0) ? half_pi : y;
		res.i = y;
		return res;
	} else {
		if((y == 0) && (x <= one)) {
			res.r = (x == 0) ? half_pi : acos(n.r);
			res.i = n.i * -1;
			return res;
		} else {
			float64_t safe_max = DBL_MAX / 8;
			float64_t safe_min = DBL_MIN / 4;
	
			float64_t xp1 = one + x;
			float64_t xm1 = x - one;
	
			if((x < safe_max) && (x > safe_min) && (y < safe_max) && (y > safe_min)) {
				float64_t yy = y * y;
				float64_t r = sqrt(xp1*xp1 + yy);
				float64_t s = sqrt(xm1*xm1 + yy);
				float64_t a = half * (r + s);
				float64_t b = x / a;
	
				if(b <= b_crossover) {
					res.r = acos(b);
				} else {
					float64_t apx = a + x;
					if(x <= one) {
						res.r = atan(sqrt(half * apx * (yy /(r + xp1) + (s-xm1)))/x);
					} else {
						res.r = atan((y * sqrt(half * (apx/(r + xp1) + apx/(s+xm1))))/x);
					}
				}
	
				if(a <= a_crossover) {
					float64_t am1;
					if(x < one) {
						am1 = half * (yy/(r + xp1) + yy/(s - xm1));
					} else {
						am1 = half * (yy/(r + xp1) + (s + xm1));
					}
					res.i = log1p(am1 + sqrt(am1 * (a + one)));
				} else {
					res.i = log(a + sqrt(a*a - one));
				}
			} else {
				if(y <= (DBL_EPSILON * fabs(xm1))) {
					if(x < one) {
						res.r = acos(x);
						res.i = y / sqrt(xp1*(one-x));
					} else {
						if((DBL_MAX / xp1) > xm1) {
							res.r = y / sqrt(xm1*xp1);
							res.i = log1p(xm1 + sqrt(xp1*xm1));
						} else {
							res.r = y / x;
							res.i = log_two + log(x);
						}
					}
				} else if(y <= safe_min) {
					res.r = sqrt(y);
					res.i = sqrt(y);
				} else if(DBL_EPSILON * y - one >= x) {
					res.r = half_pi;
					res.i = log_two + log(y);
				} else if(x > one) {
					float64_t xoy = x/y;
					res.r = atan(y/x);
					res.i = log_two + log(y) + half * log1p(xoy*xoy);
				} else {
					float64_t a = sqrt(one + y*y);
					res.r = half_pi;
					res.i = half * log1p(two *y*(y+a));
				}
			}
		}
	
	}
	if(signbit(n.r)) {
		res.r = s_pi - res.r;
	}
	if(!signbit(n.i)) {
		res.i = -1 * res.i;
	}
	return res;
}

cfloat32_t cacoshf(cfloat32_t n) {
	cfloat32_t res;
	return res;
}

cfloat64_t cacosh(cfloat64_t n) {
	cfloat64_t res;
	return res;
}

cfloat32_t casinf(cfloat32_t n) {
	static const float32_t one = (float32_t) 1;
	static const float32_t two = (float32_t) 2;
	static const float32_t half = (float32_t) 0.5;
	static const float32_t a_crossover = (float32_t) 10;
	static const float32_t b_crossover = (float32_t) 0.6417;
	static const float32_t s_pi = (float32_t) M_PI;
	static const float32_t half_pi = (float32_t) M_PI_2;
	static const float32_t log_two = (float32_t) M_LN2;
	static const float32_t quarter_pi = (float32_t) M_PI_4;
	
	float32_t x = fabsf(n.r);
	float32_t y = fabsf(n.i);
	cfloat32_t res;

	if(zend_isnan(x)) {
		if(zend_isinf(y)) {
			res.r = x;
			res.i = y;
		} else {
			res.r = x;
			res.i = x;
			return res;
		}
	} else if(zend_isnan(y)) {
		if(x == 0) {
			res.r = 0;
			res.i = y;
		} else if(zend_isinf(x)) {
			res.r = y;
			res.i = x;
		} else {
			res.r = y;
			res.i = y;
			return res;
		}
	} else if(zend_isinf(x)) {
		if(zend_isinf(y)) {
			res.r = quarter_pi;
		} else {
			res.r = half_pi;
		}
		res.i = x;
	} else if(zend_isinf(y)) {
		res.r = 0;
		res.i = y;
	} else {
		if((y == 0) && (x <= one)) {
			res.r = asinf(n.r);
			res.i = n.i;
			return res;
		} else {
			float32_t safe_max = FLT_MAX / 8;
			float32_t safe_min = FLT_MIN / 4;
	
			float32_t xp1 = one + x;
			float32_t xm1 = x - one;

			if((x < safe_max) && (x > safe_min) && (y < safe_max) && (y > safe_min)) {
				float32_t yy = y * y;
				float32_t r = sqrtf(xp1*xp1 + yy);
				float32_t s = sqrtf(xm1*xm1 + yy);
				float32_t a = half * (r + s);
				float32_t b = x / a;

				if(b <= b_crossover) {
					res.r = asinf(b);
				} else {
					float32_t apx = a + x;
					if(x <= one) {
						res.r = atanf(x/sqrtf(half * apx * (yy /(r + xp1) + (s-xm1))));
					} else {
						res.r = atanf(x/(y * sqrtf(half * (apx/(r + xp1) + apx/(s+xm1)))));
					}
				}

				if(a <= a_crossover) {
					float32_t am1;
					if(x < one) {
						am1 = half * (yy/(r + xp1) + yy/(s - xm1));
					} else {
						am1 = half * (yy/(r + xp1) + (s + xm1));
					}
					res.i = log1pf(am1 + sqrtf(am1 * (a + one)));
				} else {
					res.i = logf(a + sqrtf(a*a - one));
				}
			} else {
				if(y <= (FLT_EPSILON * fabsf(xm1))) {
					if(x < one) {
						res.r = asinf(x);
						res.i = y / sqrtf(-xp1*xm1);
					} else {
						res.r = half_pi;
						if((FLT_MAX / xp1) > xm1) {
							res.i = log1pf(xm1 + sqrtf(xp1*xm1));
						} else {
							res.i = log_two + logf(x);
						}
					}
				} else if(y <= safe_min) {
					res.r = half_pi - sqrtf(y);
					res.i = sqrtf(y);
				} else if(FLT_EPSILON * y - one >= x) {
					res.r = x/y; 
					res.i = log_two + logf(y);
				} else if(x > one) {
					float32_t xoy = x/y;
					res.r = atanf(x/y);
					res.i = log_two + logf(y) + half * log1pf(xoy*xoy);
				} else {
					float32_t a = sqrtf(one + y*y);
					res.r = x/a;
					res.i = half * log1pf(two*y*(y+a));
				}
			}
		}
	}
	if(signbit(n.r)) {
		res.r = -1 * res.r;
	}
	if(signbit(n.i)) {
		res.i = -1 * res.i;
	}
	return res;
}

cfloat64_t casin(cfloat64_t n) {
	static const float64_t one = (float64_t) 1;
	static const float64_t two = (float64_t) 2;
	static const float64_t half = (float64_t) 0.5;
	static const float64_t a_crossover = (float64_t) 10;
	static const float64_t b_crossover = (float64_t) 0.6417;
	static const float64_t s_pi = (float64_t) M_PI;
	static const float64_t half_pi = (float64_t) M_PI_2;
	static const float64_t log_two = (float64_t) M_LN2;
	static const float64_t quarter_pi = (float64_t) M_PI_4;
	
	float64_t x = fabs(n.r);
	float64_t y = fabs(n.i);
	cfloat64_t res;

	if(zend_isnan(x)) {
		if(zend_isinf(y)) {
			res.r = x;
			res.i = y;
		} else {
			res.r = x;
			res.i = x;
			return res;
		}
	} else if(zend_isnan(y)) {
		if(x == 0) {
			res.r = 0;
			res.i = y;
		} else if(zend_isinf(x)) {
			res.r = y;
			res.i = x;
		} else {
			res.r = y;
			res.i = y;
			return res;
		}
	} else if(zend_isinf(x)) {
		if(zend_isinf(y)) {
			res.r = quarter_pi;
		} else {
			res.r = half_pi;
		}
		res.i = x;
	} else if(zend_isinf(y)) {
		res.r = 0;
		res.i = y;
	} else {
		if((y == 0) && (x <= one)) {
			res.r = asin(n.r);
			res.i = n.i;
			return res;
		} else {
			float64_t safe_max = DBL_MAX / 8;
			float64_t safe_min = DBL_MIN / 4;
	
			float64_t xp1 = one + x;
			float64_t xm1 = x - one;

			if((x < safe_max) && (x > safe_min) && (y < safe_max) && (y > safe_min)) {
				float64_t yy = y * y;
				float64_t r = sqrt(xp1*xp1 + yy);
				float64_t s = sqrt(xm1*xm1 + yy);
				float64_t a = half * (r + s);
				float64_t b = x / a;

				if(b <= b_crossover) {
					res.r = asin(b);
				} else {
					float64_t apx = a + x;
					if(x <= one) {
						res.r = atan(x/sqrt(half * apx * (yy /(r + xp1) + (s-xm1))));
					} else {
						res.r = atan(x/(y * sqrt(half * (apx/(r + xp1) + apx/(s+xm1)))));
					}
				}

				if(a <= a_crossover) {
					float64_t am1;
					if(x < one) {
						am1 = half * (yy/(r + xp1) + yy/(s - xm1));
					} else {
						am1 = half * (yy/(r + xp1) + (s + xm1));
					}
					res.i = log1p(am1 + sqrt(am1 * (a + one)));
				} else {
					res.i = log(a + sqrt(a*a - one));
				}
			} else {
				if(y <= (DBL_EPSILON * fabs(xm1))) {
					if(x < one) {
						res.r = asin(x);
						res.i = y / sqrt(-xp1*xm1);
					} else {
						res.r = half_pi;
						if((DBL_MAX / xp1) > xm1) {
							res.i = log1p(xm1 + sqrt(xp1*xm1));
						} else {
							res.i = log_two + log(x);
						}
					}
				} else if(y <= safe_min) {
					res.r = half_pi - sqrt(y);
					res.i = sqrt(y);
				} else if(DBL_EPSILON * y - one >= x) {
					res.r = x/y; 
					res.i = log_two + log(y);
				} else if(x > one) {
					float64_t xoy = x/y;
					res.r = atan(x/y);
					res.i = log_two + log(y) + half * log1p(xoy*xoy);
				} else {
					float64_t a = sqrt(one + y*y);
					res.r = x/a;
					res.i = half * log1p(two*y*(y+a));
				}
			}
		}
	}
	if(signbit(n.r)) {
		res.r = -1 * res.r;
	}
	if(signbit(n.i)) {
		res.i = -1 * res.i;
	}
	return res;
}

cfloat32_t casinhf(cfloat32_t n) {
	cfloat32_t res;
	return res;
}

cfloat64_t casinh(cfloat64_t n) {
	cfloat64_t res;
	return res;
}

cfloat32_t catanf(cfloat32_t n) {
	cfloat32_t res;
	return res;
}

cfloat64_t catan(cfloat64_t n) {
	cfloat64_t res;
	return res;
}

cfloat32_t catanhf(cfloat32_t n) {
	cfloat32_t res;
	return res;
}

cfloat64_t catanh(cfloat64_t n) {
	cfloat64_t res;
	return res;
}

cfloat32_t ccosf(cfloat32_t n) {
	cfloat32_t res;
	res.r = cosf(n.r) * coshf(n.i);
	res.i = -sinf(n.r) * sinhf(n.i);
	return res;
}

cfloat64_t ccos(cfloat64_t n) {
	cfloat64_t res;
	res.r = cos(n.r) * cosh(n.i);
	res.i = -sin(n.r) * sinh(n.i);
	return res;
}

cfloat32_t ccoshf(cfloat32_t n) {
	cfloat32_t res;
	res.r = coshf(n.r) * cosf(n.i);
	res.i = sinhf(n.r) * sinf(n.i);
	return res;
}

cfloat64_t ccosh(cfloat64_t n) {
	cfloat64_t res;
	res.r = cosh(n.r) * cos(n.i);
	res.i = sinh(n.r) * sin(n.i);
	return res;
}

cfloat32_t cdivf(cfloat32_t n1, cfloat32_t n2) {
	cfloat32_t res;
	float32_t w = n2.r * n2.r + n2.i * n2.i;
	res.r = ((n1.r * n2.r) + (n1.i * n2.i)) / w;
	res.i = ((n1.i * n2.r) - (n1.r * n2.i)) / w;
	return res;
}

cfloat64_t cdiv(cfloat64_t n1, cfloat64_t n2) {
	cfloat64_t res;
	float64_t w = n2.r * n2.r + n2.i * n2.i;
	res.r = ((n1.r * n2.r) + (n1.i * n2.i)) / w;
	res.i = ((n1.i * n2.r) - (n1.r * n2.i)) / w;
	return res;
}

cfloat32_t cexpf(cfloat32_t n) {
	cfloat32_t res;
	float32_t w = expf(n.r);
	res.r = w * cosf(n.i);
	res.i = w * sinf(n.i);
	return res;
}

cfloat64_t cexp(cfloat64_t n) {
	cfloat64_t res;
	float64_t w = exp(n.r);
	res.r = w * cos(n.i);
	res.i = w * sin(n.i);
	return res;
}

cfloat32_t clogf(cfloat32_t n) {
	cfloat32_t res;
	float32_t w = sqrtf(n.r * n.r + n.i * n.i);
	res.r = logf(w);
	res.i = atan2f(n.i, n.r);
	return res;
}

cfloat64_t clog(cfloat64_t n) {
	cfloat64_t res;
	float64_t w = sqrt(n.r * n.r + n.i * n.i);
	res.r = log(w);
	res.i = atan2(n.i, n.r);
	return res;
}

cfloat32_t cmultf(cfloat32_t n1, cfloat32_t n2) {
	cfloat32_t res;
	res.r = (n1.r * n2.r) - (n1.i * n2.i);
	res.i = (n1.r * n2.i) + (n1.i * n2.r);
	return res;
}

cfloat64_t cmult(cfloat64_t n1, cfloat64_t n2) {
	cfloat64_t res;
	res.r = (n1.r * n2.r) - (n1.i * n2.i);
	res.i = (n1.r * n2.i) + (n1.i * n2.r);
	return res;
}

cfloat32_t cpowf(cfloat32_t n, cfloat32_t e) {
	cfloat32_t res;
	float32_t u = atan2f(n.i, n.r);
	float32_t v = n.r * n.r + n.i * n.i;
	float32_t x = powf(v, 0.5f * e.r);		
	float32_t y = e.r * u;
	if(e.i != 0) {
		float32_t z = 0.5f * e.i * logf(v);
		float32_t w = expf(-e.i * u);
		res.r = x * w * cosf(y + z);
		res.i = x * w * sinf(y + z);
	} else {
		res.r = x * cosf(y);
		res.i = x * sinf(y);
	}
	return res;
}

cfloat64_t cpow(cfloat64_t n, cfloat64_t e) {
	cfloat64_t res;
	float64_t u = atan2(n.i, n.r);
	float64_t v = n.r * n.r + n.i * n.i;
	float64_t x = pow(v, 0.5 * e.r);		
	float64_t y = e.r * u;
	if(e.i != 0) {
		float64_t z = 0.5 * e.i * log(v);
		float64_t w = exp(-e.i * u);
		res.r = x * w * cos(y + z);
		res.i = x * w * sin(y + z);
	} else {
		res.r = x * cos(y);
		res.i = x * sin(y);
	}
	return res;
}

cfloat32_t csinf(cfloat32_t n) {
	cfloat32_t res;
	res.r = sinf(n.r) * coshf(n.i);
	res.i = cosf(n.r) * sinhf(n.i);
	return res;
}

cfloat64_t csin(cfloat64_t n) {
	cfloat64_t res;
	res.r = sin(n.r) * cosh(n.i);
	res.i = cos(n.r) * sinh(n.i);
	return res;
}

cfloat32_t csinhf(cfloat32_t n) {
	cfloat32_t res;
	res.r = sinhf(n.r) * cosf(n.i);
	res.i = coshf(n.r) * sinf(n.i);
	return res;
}

cfloat64_t csinh(cfloat64_t n) {
	cfloat64_t res;
	res.r = sinh(n.r) * cos(n.i);
	res.i = cosh(n.r) * sin(n.i);
	return res;
}

cfloat32_t csqrtf(cfloat32_t n) {
	cfloat32_t res;
	float32_t s = (n.i > 0.0f ? 1.0f : ((n.i < 0 ? -1.0f : 0.0f)));
	float32_t w = sqrtf(n.r * n.r + n.i * n.i);
	res.r = sqrtf(0.5f * (n.r + w));
	res.i = (s) ? s * sqrtf(0.5f * (-n.r + w)) : 0.0f;
	return res;
}

cfloat64_t csqrt(cfloat64_t n) {
	cfloat64_t res;
	float64_t s = (n.i > 0.0 ? 1.0 : ((n.i < 0 ? -1.0 : 0.0)));
	float64_t w = sqrt(n.r * n.r + n.i * n.i);
	res.r = sqrt(0.5 * (n.r + w));
	res.i = (s) ? s * sqrt(0.5 * (-n.r + w)) : 0.0;
	return res;
}

cfloat32_t ctanf(cfloat32_t n) {
	cfloat32_t res;
	float32_t w = 1.0f / (cosf(2.0f * n.r) + coshf(2.0f * n.i));
	res.r = w * sinf(2.0f * n.r);
	res.i = w * sinhf(2.0f * n.i);
	return res;
}

cfloat64_t ctan(cfloat64_t n) {
	cfloat64_t res;
	float64_t w = 1.0 / (cos(2.0 * n.r) + cosh(2.0 * n.i));
	res.r = w * sin(2.0 * n.r);
	res.i = w * sinh(2.0 * n.i);
	return res;
}

cfloat32_t ctanhf(cfloat32_t n) {
	cfloat32_t res;
	float32_t w = 1.0f / (coshf(2.0f * n.r) + cosf(2.0f * n.i));
	res.r = w * sinhf(2.0f * n.r);
	res.i = w * sinf(2.0f * n.i);
	return res;
}

cfloat64_t ctanh(cfloat64_t n) {
	cfloat64_t res;
	float64_t w = 1.0 / (cosh(2.0 * n.r) + cos(2.0 * n.i));
	res.r = w * sinh(2.0 * n.r);
	res.i = w * sin(2.0 * n.i);
	return res;
}

