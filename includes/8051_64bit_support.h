#ifndef C8051_64BIT_SUPPORT
#define C8051_64BIT_SUPPORT


#define min(x, y) (((x)<(y))?(x):(y))
#define max(x, y) (((x)>(y))?(x):(y))
#define FALSE 0
#define TRUE 1


// Definition of basic type
typedef struct{
	unsigned long lo, hi;
}Longlong;


typedef union {
	unsigned long lng;
	struct{
		unsigned short lo, hi;
	}pr;
}Long;


// function prototypes
//void assign(Longlong x, Longlong* y);	// data assignment
void assign(long x, Longlong* y);

Longlong get_longlong(void);			// I/O functions (hex)
void put_longlong(const Longlong* a);

void increment(Longlong* x);			// increment/decrement
void decrement(Longlong* x);

void complement(Longlong* x);			// complement, negate, abs, sign
void negate(Longlong* x);
void make_abs(Longlong* x);

void shiftleft(Longlong* x, int n);		// shifts
void shiftright(Longlong* x, int n);

int iszero(const Longlong* x);			// tests
int isneg(const Longlong* x);

int equal(const Longlong* x, const Longlong* y);	// comparison operators
int not_equal(const Longlong* x, const Longlong* y);
int less(const Longlong* x, const Longlong* y);
int greater(const Longlong* x, const Longlong* y);
int less_or_equal(const Longlong* x, const Longlong* y);
int greater_or_equal(const Longlong* x, const Longlong* y);

Longlong and(const Longlong* x, const Longlong* y);	// logical operators
Longlong or(const Longlong* x, const Longlong* y);
Longlong xor(const Longlong* x, const Longlong* y);

Longlong add(const Longlong* x, const Longlong* y);	// arithmetic operators
Longlong add(const Longlong* x, const long y);
Longlong subtract(const Longlong* x, const Longlong* y);
Longlong subtract(const Longlong* x, const long y);
Longlong umult(const unsigned long x, const unsigned long y);
Longlong mult(const long x, const long y);
Longlong mult(const Longlong* x, const Longlong* y);
Longlong udiv (const Longlong* u, unsigned short a);
//unsigned short div3(unsigned short* u, Long* v);
Longlong udiv(const Longlong* U, unsigned long v);
Longlong udiv(const Longlong* u, Longlong* a);
Longlong udiv(const Longlong* u, Longlong* a);
Longlong div (const Longlong* u, long a);
#endif
