#include <linux/kernel.h>
#include <linux/string.h>
#define MAX_LENGTH 1000
#define MAX_DIGIT 1000
#define OFFSET 48

typedef struct {
    char decimal[MAX_DIGIT];
    int length;
} bignum;

#define bn_tmp(x) \
    bn_new(&(bignum) { .length = 0 }, x)

bignum *bn_new(bignum *bn, char decimal[]);
void bn_add(bignum *x, bignum *y, bignum *dest);
void bn_subtract(bignum *x, bignum *y, bignum *dest);
void bn_multiply(bignum *x, bignum *y, bignum *dest);
