#include "bignum.h"

bignum *bn_new(bignum *bn, char decimal[])
{
    bn->length = strlen(decimal);
    memcpy(bn->decimal, decimal, bn->length);
    return bn;
}

void bn_add(bignum *x, bignum *y, bignum *dest)
{
    if (x->length < y->length)
        return bn_add(y, x, dest);
    int carry = 0;
    int i;
    for (i = 0; i < y->length; i++) {
        dest->decimal[i] = y->decimal[i] + x->decimal[i] + carry - OFFSET;
        if (dest->decimal[i] >= 10 + OFFSET) {
            carry = 1;
            dest->decimal[i] -= 10;
        } else
            carry = 0;
    }
    for (i = y->length; i < x->length; i++) {
        dest->decimal[i] = x->decimal[i] + carry;
        if (dest->decimal[i] >= 10 + OFFSET) {
            carry = 1;
            dest->decimal[i] -= 10;
        } else
            carry = 0;
    }
    dest->length = x->length;
    dest->decimal[dest->length] = carry + OFFSET;
    if (carry)
        dest->length++;
    dest->decimal[dest->length] = '\0';
}

void bn_subtract(bignum *x, bignum *y, bignum *dest)
{
    bignum *x_copy = bn_tmp(x->decimal);
    if (x->length == y->length) {
        int length = x->length;

        while (x->decimal[length - 1] == y->decimal[length - 1]) {
            length--;
            if (length == 1) {
                dest->length = 1;
                dest->decimal[0] = x->decimal[0] - y->decimal[0];
            }
        }
        x_copy->decimal[length - 1] =
            x->decimal[length - 1] - y->decimal[length - 1] + OFFSET;
        x_copy->decimal[length] = '\0';
        x_copy->length = length;
    }

    char nine[MAX_DIGIT];
    int i;
    for (i = 0; i < x->length - 1; i++)
        nine[i] = '9';
    nine[x_copy->length - 1] = '\0';
    bignum *nines = bn_tmp(nine);

    if (!(--x_copy->decimal[x_copy->length - 1]))
        x_copy->length--;

    for (i = 0; i < y->length; i++) {
        nines->decimal[i] -= y->decimal[i];
        nines->decimal[i] += OFFSET;
    }
    bn_add(x_copy, nines, x_copy);
    bn_add(bn_tmp("1"), x_copy, dest);
}

void bn_multiply(bignum *x, bignum *y, bignum *dest)
{
    if (x->length < y->length)
        return bn_multiply(y, x, dest);
    bignum *sum = bn_tmp("0");
    int i,j;
    for (i = 0; i < y->length; i++) {
        bignum *tmp = bn_tmp("0");
        for (j = 0; j < y->decimal[i] - OFFSET; j++)
            bn_add(x, tmp, tmp);
        for (j = tmp->length; j >= 0; j--)
            tmp->decimal[j + i] = tmp->decimal[j];
        for (j = 0; j < i; j++)
            tmp->decimal[j] = '0';
        tmp->length += i;
        bn_add(tmp, sum, sum);
    }
    dest->length = sum->length;
    for (i = 0; i <= sum->length; i++)
        dest->decimal[i] = sum->decimal[i];
}

