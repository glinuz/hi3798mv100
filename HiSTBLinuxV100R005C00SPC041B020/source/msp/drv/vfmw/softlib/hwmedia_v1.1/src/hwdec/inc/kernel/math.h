
#ifndef _LINUX_MATH_H
#define _LINUX_MATH_H

#ifdef __KERNEL__
#define MATHLOG 1
#define LOGTWO  0.69314718055994531
#define LOGTEN  2.3025850929940457
#ifndef ZERO
#define ZERO 0.5e-100
#endif

#define sin(x) x
#define cos(x) x
#define sinf(x) x
#define sqrt(x) x

#define strtol(x,y,z) simple_strtol(x,y,z)
#define strtod(x,y) simple_strtol(x,y,0)

/*please tell me who did this */
//#define qsort(a,b,c,d) a

static inline int GetLogItemNumber(double a)
{
    int N;

    if (a <= 1)
    {
        N = -1;
    }
    else if (a <= 1.2)
    {
        N = 7;
    }
    else if (a <= 1.5)
    {
        N = 10;
    }
    else if (a <= 2.0)
    {
        N = 15;
    }
    else if (a <= 3.0)
    {
        N = 24;
    }
    else if (a <= 10)
    {
        N = 84;
    }
    else
    {
        N = 84 * (int)(a / 10);
    }

    return N;
}

static inline double LOGTV(double a)
{
    int N = 0, K , NK;
    double x , xx , y;

    x = (a - 1) / (a + 1);
    xx = x * x;
    N = GetLogItemNumber(a);
    NK = N * 2 + 1;
    y = 1.0 / NK;

    for (K = N ; K > 0 ; K--)
    {
        NK -= 2;
        y = 1.0 / NK + xx * y;
    }

    return (2.0 * x * y);
}

static inline double log(double a)
{
    int TYPE = 0, K = 0;
    double x, y, power = 2.0;

    x = a;

    if (x < ZERO)
    {
        return -100;
    }

    if (x < 1)
    {
        x = 1.0 / x;
        TYPE = 1;
    }

    while (power < x)
    {
        power *= 2.0;
        K++;
    }

    y = LOGTV(2.0 * x / power) + K * LOGTWO;

    if (TYPE == 1)
    {
        y = -y;
    }

    return y;
}

static inline double log10(double a)
{
    return log(a) / LOGTEN;
}

static inline double loga(double a, double x)
{
    return log(x) / log(a);
}

#endif

#endif /* _LINUX_MATH_H */
