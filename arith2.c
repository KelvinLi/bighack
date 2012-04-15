/* $Id$

Copyright (C) 2000  The PARI group.

This file is part of the PARI/GP package.

PARI/GP is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation. It is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY WHATSOEVER.

Check the License for details. You should have received a copy of it, along
with the package; see the file 'COPYING'. If not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#include <pari/pari.h>

static byteptr
initprimes1(ulong size, long *lenp, long *lastp)
{
  long k;
  byteptr q, r, s, p = (byteptr)pari_calloc(size+2), fin = p + size;

  for (r=q=p,k=1; r<=fin; )
  {
    do { r+=k; k+=2; r+=k; } while (*++q);
    for (s=r; s<=fin; s+=k) *s = 1;
  }
  r = p; *r++ = 2; *r++ = 1; /* 2 and 3 */
  for (s=q=r-1; ; s=q)
  {
    do q++; while (*q);
    if (q > fin) break;
    *r++ = (unsigned char) ((q-s) << 1);
  }
  *r++ = 0;
  *lenp = r - p;
  *lastp = ((s - p) << 1) + 1;
  return (byteptr) pari_realloc(p,r-p);
}

#ifndef SLOW2_IN_ROOTS
#  ifdef i386           /* gcc defines this? */
#    define SLOW2_IN_ROOTS      0.36
#  else
#    define SLOW2_IN_ROOTS      2.6
#  endif
#endif
#ifndef CACHE_ARENA
#  ifdef i386           /* gcc defines this? */
   /* Due to smaller SLOW2_IN_ROOTS, smaller arena is OK; fit L1 cache */
#    define CACHE_ARENA (63 * 1024UL) /* No slowdown even with 64K L1 cache */
#  else
#    define CACHE_ARENA (200 * 1024UL) /* No slowdown even with 256K L2 cache */
#  endif
#endif

#define CACHE_ALPHA     (0.38)          /* Cache performance model parameter */
#define CACHE_CUTOFF    (0.018)         /* Cache performance not smooth here */

static double slow2_in_roots = SLOW2_IN_ROOTS;

typedef struct {
    ulong arena;
    double power;
    double cutoff;
} cache_model_t;

static cache_model_t cache_model = { CACHE_ARENA, CACHE_ALPHA, CACHE_CUTOFF };

static ulong
good_arena_size(ulong slow2_size, ulong total, ulong fixed_to_cache,
                cache_model_t *cache_model, long model_type)
{
  ulong asize, cache_arena = cache_model->arena;
  double Xmin, Xmax, A, B, C1, C2, D, V;
  double alpha = cache_model->power, cut_off = cache_model->cutoff;

  if (model_type != 0)
      pari_err(talker, "unsupported type of cache model"); /* Future expansion */

  /* The simplest case: we fit into cache */
  if (total + fixed_to_cache <= cache_arena)
      return total;
  /* The simple case: fitting into cache doesn't slow us down more than 10% */
  if (cache_arena - fixed_to_cache > 10 * slow2_size) {
      asize = cache_arena - fixed_to_cache;
      if (asize > total) asize = total; /* Automatically false... */
      return asize;
  }
  /* Slowdown of not fitting into cache is significant.  Try to optimize.
     Do not be afraid to spend some time on optimization - in trivial
     cases we do not reach this point; any gain we get should
     compensate the time spent on optimization.  */

  B = (1 - ((double)fixed_to_cache)/cache_arena);
  A = B + ((double)slow2_size)/cache_arena;
  C2 = A*B;
  C1 = (A + B - 1/alpha*(A - B))/2;
  D = C1*C1 - C2;
  if (D > 0)
      V = cut_off*cut_off + 2*C1*cut_off + C2; /* Value at CUT_OFF */
  else
      V = 0;                            /* Peacify the warning */
  Xmin = cut_off;
  Xmax = ((double)total - fixed_to_cache)/cache_arena; /* Two candidates */

  if ( D <= 0 || (V >= 0 && C1 + cut_off >= 0) ) /* slowdown increasing */
      Xmax = cut_off;                   /* Only one candidate */
  else if (V >= 0 &&                    /* slowdown concave down */
           ((Xmax + C1) <= 0 || (Xmax*Xmax + 2*C1*Xmax + C2) <= 0))
      /* DO NOTHING */;                 /* Keep both candidates */
  else if (V <= 0 && (Xmax*Xmax + 2*C1*Xmax + C2) <= 0) /* slowdown decreasing */
      Xmin = cut_off;                   /* Only one candidate */
  else /* Now we know: 2 roots, the largest is in CUT_OFF..Xmax */
      Xmax = sqrt(D) - C1;
  if (Xmax != Xmin) {   /* Xmin == CUT_OFF; Check which one is better */
      double v1 = (cut_off + A)/(cut_off + B);
      double v2 = 2.33 * (Xmax + A)/(Xmax + B) * pow(Xmax, alpha);

      if (1.1 * v2 >= v1) /* Prefer fitting into the cache if slowdown < 10% */
          V = v1;
      else {
          Xmin = Xmax;
          V = v2;
      }
  } else if (B > 0)                     /* We need V */
      V = 2.33 * (Xmin + A)/(Xmin + B) * pow(Xmin, alpha);
  if (B > 0 && 1.1 * V > A/B)  /* Now Xmin is the minumum.  Compare with 0 */
      Xmin = 0;

  asize = (ulong)((1 + Xmin)*cache_arena - fixed_to_cache);
  if (asize > total) asize = total;     /* May happen due to approximations */
  return asize;
}

/* Use as in
    install(set_optimize,lLDG)          \\ Through some M too?
    set_optimize(2,1) \\ disable dependence on limit
    \\ 1: how much cache usable, 2: slowdown of setup, 3: alpha, 4: cutoff
    \\ 2,3,4 are in units of 0.001

    { time_primes_arena(ar,limit) =     \\ ar = arena size in K
        set_optimize(1,floor(ar*1024));
        default(primelimit, 200 000);   \\ 100000 results in *larger* malloc()!
        gettime;
        default(primelimit, floor(limit));
        if(ar >= 1, ar=floor(ar));
        print("arena "ar"K => "gettime"ms");
    }
*/
long
set_optimize(long what, GEN g)
{
  long ret = 0;

  switch (what) {
  case 1:
    ret = (long)cache_model.arena;
    break;
  case 2:
    ret = (long)(slow2_in_roots * 1000);
    break;
  case 3:
    ret = (long)(cache_model.power * 1000);
    break;
  case 4:
    ret = (long)(cache_model.cutoff * 1000);
    break;
  default:
    pari_err(talker, "panic: set_optimize");
    break;
  }
  if (g != NULL) {
    ulong val = itou(g);

    switch (what) {
    case 1: cache_model.arena = val; break;
    case 2: slow2_in_roots     = (double)val / 1000.; break;
    case 3: cache_model.power  = (double)val / 1000.; break;
    case 4: cache_model.cutoff = (double)val / 1000.; break;
    }
  }
  return ret;
}

static void
sieve_chunk(byteptr known_primes, ulong s, byteptr data, ulong count)
{   /* start must be odd;
       prime differences (starting from (5-3)=2) start at known_primes[2],
       are terminated by a 0 byte;

       Checks cnt odd numbers starting at 'start', setting bytes
       starting at data to 0 or 1 depending on whether the
       corresponding odd number is prime or not */
    ulong p;
    byteptr q;
    register byteptr write_to = data;   /* Better code with gcc 2.8.1 */
    register ulong   cnt      = count;  /* Better code with gcc 2.8.1 */
    register ulong   start    = s;      /* Better code with gcc 2.8.1 */
    register ulong   delta    = 1;      /* Better code with gcc 2.8.1 */

    memset(data, 0, cnt);
    start >>= 1;                        /* (start - 1)/2 */
    start += cnt;                       /* Corresponds to the end */
    cnt -= 1;
    /* data corresponds to start.  q runs over primediffs.  */
    /* Don't care about DIFFPTR_SKIP: false positives provide no problem */
    for (q = known_primes + 1, p = 3; delta; delta = *++q, p += delta) {
        /* first odd number which is >= start > p and divisible by p
           = last odd number which is <= start + 2p - 1 and 0 (mod p)
           = p + the last even number which is <= start + p - 1 and 0 (mod p)
           = p + the last even number which is <= start + p - 2 and 0 (mod p)
           = p + start + p - 2 - (start + p - 2) % 2p
           = start + 2(p - 1 - ((start-1)/2 + (p-1)/2) % p). */
      long off = cnt - ((start+(p>>1)) % p);

      while (off >= 0) {
          write_to[off] = 1;
          off -= p;
      }
    }
}

/* Here's the workhorse.  This is recursive, although normally the first
   recursive call will bottom out and invoke initprimes1() at once.
   (Not static;  might conceivably be useful to someone in library mode) */
byteptr
initprimes0(ulong maxnum, long *lenp, ulong *lastp)
{
  long size, alloced, psize;
  byteptr q, fin, p, p1, fin1, plast, curdiff;
  ulong last, remains, curlow, rootnum, asize;
  ulong prime_above = 3;
  byteptr p_prime_above;

  if (maxnum <= 1ul<<17)        /* Arbitrary. */
    return initprimes1(maxnum>>1, lenp, (long*)lastp); /* Break recursion */

  maxnum |= 1;                  /* make it odd. */

  /* Checked to be enough up to 40e6, attained at 155893 */
  /* Due to multibyte representation of large gaps, this estimate will
     be broken by large enough maxnum.  However, assuming exponential
     distribution of gaps with the average log(n), we are safe up to
     circa exp(-256/log(1/0.09)) = 1.5e46.  OK with LONG_BITS <= 128. ;-) */
  size = (long) (1.09 * maxnum/log((double)maxnum)) + 146;
  p1 = (byteptr) pari_malloc(size);
  rootnum = (ulong) sqrt((double)maxnum); /* cast it back to a long */
  rootnum |= 1;
  {
    byteptr p2 = initprimes0(rootnum, &psize, &last); /* recursive call */
    memcpy(p1, p2, psize); pari_free(p2);
  }
  fin1 = p1 + psize - 1;
  remains = (maxnum - rootnum) >> 1; /* number of odd numbers to check */

  /* Actually, we access primes array of psize too; but we access it
     consequently, thus we do not include it in fixed_to_cache */
  asize = good_arena_size((ulong)(rootnum * slow2_in_roots), remains + 1, 0,
                          &cache_model, 0) - 1;
  /* enough room on the stack ? */
  alloced = (((byteptr)avma) <= ((byteptr)bot) + asize);
  if (alloced)
    p = (byteptr) pari_malloc(asize + 1);
  else
    p = (byteptr) bot;
  fin = p + asize;              /* the 0 sentinel goes at fin. */
  curlow = rootnum + 2; /* First candidate: know primes up to rootnum (odd). */
  curdiff = fin1;

  /* During each iteration p..fin-1 represents a range of odd
     numbers.  plast is a pointer which represents the last prime seen,
     it may point before p..fin-1. */
  plast = p - ((rootnum - last) >> 1) - 1;
  p_prime_above = p1 + 2;
  while (remains)       /* Cycle over arenas.  Performance is not crucial */
  {
    unsigned char was_delta;

    if (asize > remains) {
      asize = remains;
      fin = p + asize;
    }
    /* Fake the upper limit appropriate for the given arena */
    while (prime_above*prime_above <= curlow + (asize << 1) && *p_prime_above)
        prime_above += *p_prime_above++;
    was_delta = *p_prime_above;
    *p_prime_above = 0;                 /* Put a 0 sentinel for sieve_chunk */

    sieve_chunk(p1, curlow, p, asize);

    *p_prime_above = was_delta;         /* Restore */
    p[asize] = 0;                       /* Put a 0 sentinel for ZZZ */
    /* now q runs over addresses corresponding to primes */
    for (q = p; ; plast = q++)
    {
      long d;

      while (*q) q++;                   /* use ZZZ 0-sentinel at end */
      if (q >= fin) break;
      d = (q - plast) << 1;
      while (d >= DIFFPTR_SKIP)
        *curdiff++ = DIFFPTR_SKIP, d -= DIFFPTR_SKIP;
      *curdiff++ = (unsigned char)d;
    }
    plast -= asize;
    remains -= asize;
    curlow += (asize<<1);
  } /* while (remains) */
  last = curlow - ((p - plast) << 1);
  *curdiff++ = 0;               /* sentinel */
  *lenp = curdiff - p1;
  *lastp = last;
  if (alloced) pari_free(p);
  return (byteptr) pari_realloc(p1, *lenp);
}
#if 0 /* not yet... GN */
/* The diffptr table will contain at least 6548 entries (up to and including
   the 6547th prime, 65557, and the terminal null byte), because the isprime/
   small-factor-extraction machinery wants to depend on everything up to 65539
   being in the table, and we might as well go to a multiple of 4 Bytes.--GN */

void
init_tinyprimes_tridiv(byteptr p);      /* in ifactor2.c */
#endif

static ulong _maxprime = 0;

ulong
maxprime(void) { return _maxprime; }

void
maxprime_check(ulong c)
{
  if (_maxprime < c) pari_err(primer1, c);
}

/* assume ptr is the address of a diffptr containing the succesive
 * differences between primes, and p = current prime (up to *ptr excluded)
 * return smallest prime >= a, update ptr */
ulong
init_primepointer(ulong a, ulong p, byteptr *ptr)
{
  byteptr diff = *ptr;
  if (a <= 0) a = 2;
  maxprime_check((ulong)a);
  while (a > p) NEXT_PRIME_VIADIFF(p,diff);
  *ptr = diff; return p;
}

byteptr
initprimes(ulong maxnum)
{
  long len;
  ulong last;
  byteptr p;
  /* The algorithm must see the next prime beyond maxnum, whence the +512. */
  ulong maxnum1 = ((maxnum<65302?65302:maxnum)+512ul);

  if ((maxnum>>1) > LONG_MAX - 1024)
      pari_err(talker, "Too large primelimit");
  p = initprimes0(maxnum1, &len, &last);
#if 0 /* not yet... GN */
  static int build_the_tables = 1;
  if (build_the_tables) { init_tinyprimes_tridiv(p); build_the_tables=0; }
#endif
  _maxprime = last; return p;
}

GEN
boundfact(GEN n, ulong lim)
{
  switch(typ(n))
  {
    case t_INT: return Z_factor_limit(n,lim);
    case t_FRAC: {
      pari_sp av = avma;
      GEN a = Z_factor_limit(gel(n,1),lim);
      GEN b = Z_factor_limit(gel(n,2),lim);
      gel(b,2) = ZC_neg(gel(b,2));
      return gerepilecopy(av, merge_factor_i(a,b));
    }
  }
  pari_err(arither1);
  return NULL; /* not reached */
}

/* NOT memory clean */
GEN
Z_smoothen(GEN N, GEN L, GEN *pP, GEN *pe)
{
  long i, j, l = lg(L);
  GEN e = new_chunk(l), P = new_chunk(l);
  for (i = j = 1; i < l; i++)
  {
    ulong p = (ulong)L[i];
    long v = Z_lvalrem(N, p, &N);
    if (v) { P[j] = p; e[j] = v; j++; if (is_pm1(N)) { N = NULL; break; } }
  }
  P[0] = evaltyp(t_VECSMALL) | evallg(j); *pP = P;
  e[0] = evaltyp(t_VECSMALL) | evallg(j); *pe = e; return N;
}
