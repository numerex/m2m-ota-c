#ifdef C8051
// 64-bit Arithmetic in C++
//
// This file contains C/C++ functions that allow the manipulation of
// 64-bit integers. It represents an alternative to the C++ class
// longlong. In this file, the type Longlong is treated as an ordinary
// C typedef, and the functions perform operations on the type without
// collection into a C++ class. For the sake of efficiency, the C++
// feature of operator overloading is not used. This implementation
// should be appreciated by those who are programming in C, since it is
// more easily translated into pure C.
//
// In this formulation, structs and unions are also used as sparingly
// as possible to maintain simplicity, while avoiding unnecessary
// data copying.
//
//
//#include <iostream.h>
#include "8051_64bit_support.h"

// macro definitions




// Function definitions

// assign signed long to longlong (y = x)
void assign(long x, Longlong* y){
	y->lo = x;
	y->hi = 0;
	if(x < 0)
		--y->hi;
}








// increment a longlong
void increment(Longlong* x){
	++x->lo;
	if(x->lo == 0)
		++ x->hi;
}


// decrement a longlong
void decrement(Longlong* x){
	--x->lo;
	if((long)x->lo == -1)
		-- x->hi;
}


// complement a longlong
void complement(Longlong* x){
	x->hi = ~x->hi;
	x->lo = ~x->lo;
}


// negate a longlong
void negate(Longlong* x){
	complement(x);
	increment(x);
}


// convert a longlong to its absolute value
void make_abs(Longlong* x){
	if(isneg(x))
		negate(x);
}


// shift a longlong left n bits
void shiftleft(Longlong* x, int n){
	x->hi <<= n;
	x->hi |= (x->lo >> (32-n));
	x->lo <<= n;
}


// shift a longlong left n bits
void shiftright(Longlong* x, int n){
	x->lo >>= n;
	x->lo |= (x->hi << (32-n));
	x->hi >>= n;
}


// test a longlong for zero
int iszero(const Longlong* x){
	return ((x->hi | x->lo) == 0);
}


// test a longlong for sign
int isneg(const Longlong* x){
	return ((long)x->hi < 0);
}


/* compare two longlongs
 * return true if x == y
 */
int equal(const Longlong* x, const Longlong* y){
	return ((x->hi == y->hi) & (x->lo == y->lo));
}


/* compare two longlongs
 * return true if x != y
 */
int not_equal(const Longlong* x, const Longlong* y){
	return ((x->hi != y->hi) || (x->lo != y->lo));
}


/* compare two longlongs
 * return true if x < y
 */
int less(const Longlong* x, const Longlong* y){
	if(x->hi == y->hi)
		return (x->lo < y->lo);
	else
		return (x->hi < y->hi);
}


/* compare two longlongs
 * return true if x > y
 */
int greater(const Longlong* x, const Longlong* y){
	if(x->hi == y->hi)
		return (x->lo > y->lo);
	else
		return (x->hi > y->hi);
}


/* compare two longlongs
 * return true if x <= y
 */
int less_or_equal(const Longlong* x, const Longlong* y){
	if(x->hi == y->hi)
		return (x->lo <= y->lo);
	else
		return (x->hi < y->hi);
}


/* compare two longlongs
 * return true if x >= y
 */
int greater_or_equal(const Longlong* x, const Longlong* y){
	if(x->hi == y->hi)
		return (x->lo >= y->lo);
	else
		return (x->hi > y->hi);
}


// logical and two longlongs
Longlong and(const Longlong* x, const Longlong* y){
	Longlong retval;
	retval = *x;
	retval.hi = y->hi;
	retval.lo = y->lo;
	return retval;
}


// logical and two longlongs
Longlong or(const Longlong* x, const Longlong* y){
	Longlong retval;
	retval = *x;
	retval.hi |= y->hi;
	retval.lo |= y->lo;
	return retval;
}


// logical exclusive or two longlongs
Longlong xor(const Longlong* x, const Longlong* y){
	Longlong retval;
	retval = *x;
	retval.hi ^= y->hi;
	retval.lo ^= y->lo;
	return retval;
}



// subtract two longlongs
Longlong subtract(const Longlong* x, const Longlong* y){
	Longlong retval;
	retval = *x;
	retval.lo -= y->lo;
	retval.hi -= y->hi;
	if(retval.lo > x->lo)
		-- retval.hi;
	return retval;
}


// add a (signed) long to a longlong
Longlong add(const Longlong* x, const long y){
	Longlong retval;
	retval = *x;
	retval.lo += y;
	if(y > 0){
		if(retval.lo < x->lo)
			++ retval.hi;
	}
	else{
		if(retval.lo > x->lo)
			-- retval.hi;
	}
	return retval;
}



// Multiply two (unsigned) longs to yield a longlong
Longlong umult(const unsigned long x, const unsigned long y){
	Longlong retval;
	Long X, Y;
	Long mid;	// middle two words
	unsigned long ad, bc;

	// pointer to address individual words
	unsigned short *p = (unsigned short *)(&retval);

	 // copy is as quick as pointer assignment
	X.lng = x;
	Y.lng = y;

	// generate partial products
	retval.hi = (unsigned long)X.pr.hi * Y.pr.hi;	// ac
	ad    = (unsigned long)X.pr.hi * Y.pr.lo;
	bc    = (unsigned long)X.pr.lo * Y.pr.hi;
	retval.lo = (unsigned long)X.pr.lo * Y.pr.lo;	// bd

	mid.lng = ad + p[1];		// ad, high half of bd
	if(mid.lng < ad)      	// if carry,
		++p[3];       	// bump high word
	mid.lng += bc;       	// all of bc
	if(mid.lng < bc)      	// if carry,
		++p[3];       	// bump high word

	// results into place
	retval.hi += mid.pr.hi;		// mid.hi to hi.lo
	p[1] = mid.pr.lo;			// mid.lo to lo.hi
	return retval;
}


/* Multiply two (signed) longs to yield a longlong
 * Uses Booth's algorithm
 */
Longlong mult(const long x, const long y){
	Longlong retval;
	retval = umult((unsigned long)x, (unsigned long)y);
	if(x < 0)
		retval.hi -= y;
	if(y < 0)
		retval.hi -= x;
	return retval;
}


// divide a longlong by an unsigned short
Longlong udiv (const Longlong* u, unsigned short a){
	int i;
	Longlong retval;
	Long rem;
	unsigned long temp;
	unsigned short *up;
	unsigned short *qp;

	up = (u->hi);
	qp = retval.hi;
	// test for divide by zero
	if(a == 0){
		retval.lo = 0xffffffffL;
		retval.hi = 0x7fffffffL;
		if((long)u->hi < 0)
			retval.hi |= 0x80000000L;
		return retval;
	}

	// do first division (result may be long)

	retval.hi = u->hi / a;
	 rem.lng = u->hi - retval.hi * a;

	 // loop for next two digits
	 for(i=0; i<2; i++){
		--up;
		--qp;
		rem.pr.hi = rem.pr.lo;
		rem.pr.lo = *up;
		temp = rem.lng / a;
		*qp = (unsigned short)temp;
		rem.lng -= *qp * a;
	}
	return retval;
}


/* divide three digits by two
 * (used by multiword division)
 * inputs: dividend u (leftmost word = 0)
 *     divisor v,
 * return value: quotient word
 * 		 u <- remainder
 */
unsigned short div3(unsigned short u[3], Long v){
	unsigned long q;
	unsigned short temp;
	Long low;
	unsigned long *p1 = (unsigned long *)(&u[1]);
	unsigned long *p0 = (unsigned long *)(&u[0]);

	/* get initial guess of q
	 * using high digit of v
	 */
	q = *p1 / v.pr.hi;
	q = min(q, 0xffff);

	// compute first remainder
	*p1 -= q * v.pr.hi;

	// then second remainder
	low.lng = q * v.pr.lo;
	temp = u[0];
	u[0] -= low.pr.lo;
	if(u[0] > temp)
		--(*p1);
	*p1 -= low.pr.hi;

	// refine as needed
	while((long)(*p1) < 0){
		--q;
		*p0 += v.lng;
		if(*p0 < v.lng)
			++u[2];
	}
	return (unsigned short)q;
}
#endif
