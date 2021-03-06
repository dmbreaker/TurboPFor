/**
    Copyright (C) powturbo 2015-2018
    GPL v2 License

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

    - email    : powturbo [AT] gmail.com
    - github   : https://github.com/powturbo
    - homepage : https://sites.google.com/site/powturbo/
    - twitter  : https://twitter.com/powturbo

    TurboPFor - "Benchmark demo application"
**/      
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#ifdef __APPLE__
#include <sys/malloc.h>
#else
#include <malloc.h>
#endif
  #ifdef _MSC_VER
#include "vs/getopt.h"
  #else
#include <getopt.h> 
#endif
  #if defined(_WIN32)
#include <windows.h>
#define srand48(x)  srand(x)
#define drand48()   ((double)(rand()) / RAND_MAX)
#define __off64_t   _off64_t  
  #endif
#include <math.h> // pow

#include "conf.h"
#include "time_.h"
//--------------------------------------- Zipfian generator --------------------------------------------------------
static double    a = 1.5;
static unsigned xbits[65],verbose;
void stprint(char *s) {
  int m;
  uint64_t t=0; 
  for(m = 0; m <= 64; m++) 
    t += xbits[m];
  printf("\n%s bits histogram:",s); 
  for(m = 0; m <= 64; m++) 
    if(xbits[m]) printf("%d:%.2f%% ", m, (double)xbits[m]*100/t); printf("\n");
}

int dcmp(double *a, double *b) {
  if(*a < *b) return -1;
  if(*a > *b) return  1;
  return 0;
}

void zipu32(unsigned *a, unsigned n, double alpha, unsigned x1, unsigned x2) {
  int      i; 
  unsigned m = x2 - x1 + 1;
  double   prob, cum, *zmap;
  if(!(zmap = malloc(m*sizeof(zmap[0])))) die("mallo error %d\n", m); 

  // generate initial sample (slow)
  srand48(1); 
  if(alpha > 0) {
    for(cum = 0.0,i = 0; i < m; i++) 
      cum += 1.0 / pow(i+1, alpha); 
    cum = 1.0 / cum;    
    for(zmap[0] = prob = cum,i = 1; i < m; i++) zmap[i] = (prob += (cum / pow(i+1, alpha))); 
  } else for(i = 0; i < m; i++) zmap[i] = 1.0 / m;

  // use binary search to speed up zipfgen
  qsort(zmap, m, sizeof(zmap[0]), (int(*)(const void*,const void*))dcmp); 
  for(i = 0; i < n; i++) { 
    double r = drand48(); 
    int    l = 0, h = m-1;  
    while(l < h) { 
      int k = (l + h) >> 1; 
      if(r >= zmap[k]) l = k + 1; 
      else h = k; 
    }
    a[i] = x1 + l; 
  } 
  free(zmap); 
}

void zipu64(uint64_t *a, unsigned n, double alpha, unsigned x1, unsigned x2) {
  int      i; 
  unsigned m = x2 - x1 + 1;
  double   prob, cum, *zmap;
  if(!(zmap = malloc(m*sizeof(zmap[0])))) die("mallo error %d\n", m); 

  // generate initial sample (slow)
  srand48(1); 
  if(alpha > 0) {
    for(cum = 0.0,i = 0; i < m; i++) 
      cum += 1.0 / pow(i+1, alpha); 
    cum = 1.0 / cum;    
    for(zmap[0] = prob = cum,i = 1; i < m; i++) zmap[i] = (prob += (cum / pow(i+1, alpha))); 
  } else for(i = 0; i < m; i++) zmap[i] = 1.0 / m;

  // use binary search to speed up zipfgen
  qsort(zmap, m, sizeof(zmap[0]), (int(*)(const void*,const void*))dcmp); 
  for(i = 0; i < n; i++) { 
    double r = drand48(); 
    int    l = 0, h = m-1;  
    while(l < h) { 
      int k = (l + h) >> 1; 
      if(r >= zmap[k]) l = k + 1; 
      else h = k; 
    }
    a[i] = x1 + l; 
  } 
  free(zmap); 
}

void zipd64(double *a, unsigned n, double alpha, unsigned x1, unsigned x2) {
  int      i; 
  unsigned m = x2 - x1 + 1;
  double   prob, cum, *zmap;
  if(!(zmap = malloc(m*sizeof(zmap[0])))) die("mallo error %d\n", m); 

  // generate initial sample (slow)
  srand48(1); 
  if(alpha > 0) {
    for(cum = 0.0,i = 0; i < m; i++) 
      cum += 1.0 / pow(i+1, alpha); 
    cum = 1.0 / cum;    
    for(zmap[0] = prob = cum,i = 1; i < m; i++) zmap[i] = (prob += (cum / pow(i+1, alpha))); 
  } else for(i = 0; i < m; i++) zmap[i] = 1.0 / m;

  // use binary search to speed up zipfgen
  qsort(zmap, m, sizeof(zmap[0]), (int(*)(const void*,const void*))dcmp); 
  for(i = 0; i < n; i++) { 
    double r = drand48(); 
    int    l = 0, h = m-1;  
    while(l < h) { 
      int k = (l + h) >> 1; 
      if(r >= zmap[k]) l = k + 1; 
      else h = k; 
    }
    a[i] = x1 + l; 
  } 
  free(zmap); 
}

void tms64(double *a, unsigned n, unsigned x1, unsigned x2) {
  double freq = drand48()*0.5 + 0.1, amp = drand48()+ 0.5;
  int i; for(i=0; i < n; i++) a[i] = x1 + sin(i * freq) * amp;
}

#define OVD (10*MB)
static unsigned rm=0,rx=255;
								
unsigned datagen(unsigned char *in, unsigned n, int isize, double be_mindelta) {	
  unsigned char *ip; 													 printf("zipf alpha=%.2f range[%u..%u].n=%u\n ", a, rm, rx, n);
  int mindelta = be_mindelta,i;
  //in = malloc(n*isize+OVD); 												if(!in) die("malloc err=%u", n*isize);
  switch(isize) {
	case 4: zipu32((unsigned *)in, n, a, rm, rx);    								//{ int i; for(i = 0; i < n; i++) in[i] = i; } 	//
	                                                                            for(i = 1; i <= n; i++) xbits[bsr32(ctou32(in+i*4))]++; 
      if(mindelta == 0 || mindelta == 1) {                                                       	
        unsigned *ip = (unsigned *)in, v;										stprint("delta"); 
        for(ip[0]=0,v = 1; v < n; v++) { 
          ip[v] += ip[v-1] + mindelta;                                     if(ip[v]>=(1u<<31)) die("overflow generating sorted array %d\n", ip[v]);  
        }
      } else stprint("");
      break;
	case 8: zipu64((uint64_t *)in, n, a, rm, rx);    								//{ int i; for(i = 0; i < n; i++) in[i] = i; } 	//
	                                                                           for(i = 1; i <= n; i++) xbits[bsr64(ctou64(in+i*8))]++; 
      if(mindelta == 0 || mindelta == 1) {                                                       	
        uint64_t *ip = (uint64_t *)in, v;										stprint("delta"); 
        for(ip[0]=0,v = 1; v < n; v++) { 
          ip[v] += ip[v-1] + mindelta;                                     if(ip[v]>=(1u<<31)) die("overflow generating sorted array %d\n", ip[v]);  
        }
      } else stprint("");
      break;
    case -8: zipd64((double *)in, n, a, rm, rx);    								//{ int i; for(i = 0; i < n; i++) in[i] = i; } 	//
	                                                                        for(i = 1; i <= n; i++) xbits[bsr64(ctou64(in+i*8))]++; 
      /*if(be_mindelta >= 0 && be_mindelta <= 1) {                                                       	
        double *ip = (double *)in, v;										stprint("delta"); 
        for(ip[0]=0,v = 1; v < n; v++) { 
          ip[v] = ip[v]+ip[v-1] + be_mindelta;                                     //if(ip[v]>=(double)(1u<<31)) die("overflow generating sorted array %.2f\n", ip[v]);  
        }
      } else*/ 
	  stprint("");
      break;
  }	  
  return n*abs(isize);
}

//       0       1        2         3         4         5         6         7,       8        9        10      11      12      13      14
enum { T_0, T_UINT8, T_UINT16, T_UINT24, T_UINT32, T_UINT40, T_UINT48, T_UINT56, T_UINT64, T_FLOAT, T_DOUBLE, T_CHAR, T_TXT, T_TIM32, T_TIM64, T_TST };
#define IPUSH(in,n,isize, nmax,u) { if(n >= nmax) { nmax = nmax?(nmax << 1):(1<<20); in = realloc(in, nmax*isize+OVD); if(!in) die("malloc err=%u", nmax); }\
                                    ctou64(in+n*isize) = u; n++;\
								  }
								
int mdelta;
char *keysep;

uint64_t strtots(char *p, char **pq, int type) {
  struct tm tm; 
  uint64_t  u; 
  char     *s=p;
  int       frac = 0,c;
  
  memset(&tm, 0, sizeof(tm)); tm.tm_mday = 1;
  while(!isdigit(*p)) p++;                      
  u = strtoull(p, &p, 10);              
  
  if(     u <= 99) u += 2000; 
  else if(u >= 19710101 && u < 20381212) { 
    tm.tm_year =  u/10000; 
	tm.tm_mon  = (u%10000)/100; if(!tm.tm_mon || tm.tm_mon > 12) goto a; tm.tm_mon--;
	tm.tm_mday = u%10;          if(!tm.tm_mday || tm.tm_mday > 31) goto a; 
	goto h;
  } else if(u < 1971 || u > 2099) goto a; 
  tm.tm_year = u;                             
  c = *p;  
  if(c != '.' && c != '-' && c != '/') goto b; tm.tm_mon    = strtoul(p+1, &p, 10); if(!tm.tm_mon  || tm.tm_mon  > 12) goto a; tm.tm_mon--; 
  if(c != '.' && c != '-' && c != '/') goto b; tm.tm_mday   = strtoul(p+1, &p, 10); if(!tm.tm_mday || tm.tm_mday > 31) goto a;
  if(c != '.' && c != '-' && c != '/') goto b; h:tm.tm_hour = strtoul(p+1, &p, 10);
  if(tm.tm_hour <= 24 && *p == ':') {
	tm.tm_min = strtoul(p+1, &p, 10); if(tm.tm_min > 60) tm.tm_hour = tm.tm_min = 0;
	tm.tm_sec = strtoul(p+1, &p, 10); if(tm.tm_sec > 60) tm.tm_hour = tm.tm_min = tm.tm_sec = 0;  
	if(type > 0 && (*p == '.' || *p == ',')) { frac = strtoul(p+1, &p, 10); if((c=p-(p+1)) > 6) frac /= 1000000;else if(c > 3) frac /= 1000; } 
  } else tm.tm_hour = 0;
  b:u = mktime(&tm);  																		
  u = u * 1000 + frac;  // Milliseconds
  a:*pq = p;                                               //if(verbose >= 9) printf("[%d-%d-%d %.2d:%.2d:%.2d.%d]\n", tm.tm_year, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, frac, u);exit(0);
  return u;
}

unsigned befgen(unsigned char **_in, unsigned n, int fmt, int isize, FILE *fi, int kid, int skiph, int decs, int divs, int mdelta) {	
  unsigned char *in = *_in,*ip; 
  unsigned nmax = 0;           
  #define LSIZE (1024*16)
  char s[LSIZE+1];
  double pre;

  n = 0;																	 
  while(skiph-- > 0) {
	fgets(s, LSIZE, fi);													if(verbose>=5 && n < 100 ||verbose>5) printf("skip first line\n");
  }
  if(decs) {
    pre = decs?pow(10.0f,(float)decs):1;
    pre /= divs;
  } else pre = 1;
  switch(fmt) {
    case T_TXT:	  
    case T_TIM32:		
    case T_TIM64:																if(verbose) printf("reading text lines. pre=%.2f, col=%d, sep=%s\n", pre, kid, keysep?keysep:"");	
      while(fgets(s, LSIZE, fi)) {
		unsigned char *p = s,*q;
		int k = 0, keyid = 1, c;
        s[strlen(s) - 1] = 0; 	
        q = p;
        if(kid > 1)
          do { 		
            p = q;
            if(keysep && strchr(keysep,*q)) keyid++;		                      
            q++;
          } while(*q && keyid != kid);
        if(fmt == T_TIM32 || fmt == T_TIM64) { 
          while(!isdigit(*p)) p++;
		  uint64_t u = strtots(p, &q, fmt == T_TIM64?1:0);
		  if(fmt == T_TIM32) u /= 1000;
		  a: IPUSH(in,n,isize,nmax,u);							c=*q; *q=0; if(verbose>=5 && n < 100 || verbose>=9) printf("\'%s\'->%llu  ", p, u); *q = c;
        } else if(isize > 0) {                 
          while(!isdigit(*p) && *p != '-' && *p != '+') p++;
		  uint64_t u = strtoll(p, &q, 10)*pre - mdelta;
		  if(*q == '.')
		    u = pre>1.0?round(strtod(p, &q)*pre):strtod(p, &q) - mdelta;
		    IPUSH(in,n,isize,nmax,u);								c=*q; 	*q=0; if(verbose>=5 && n < 100 || verbose>=9) printf("\'%s\'->%lld  ", p, u, c); *q = c;
        } else {
          while(*p && !isdigit(*p) && *p != '-' && *p != '.' && *p != '+') { if(keysep && strchr(keysep,*p)) keyid++; p++; }
		  double d = strtod(p, &q) - mdelta;  					 	
		  IPUSH(in,n,-isize,nmax,d);							c=*q; *q=0; if(verbose>=5 && n < 100 || verbose>=9) printf("\'%s\'->%f  ", p, d); *q = c;
        }
      }  
      break;
    case T_CHAR:                                                         	if(verbose) printf("reading char file. pre=%.2f\n", pre);		
      for(;;) {
        char *p = s,*q;
        int c;
        if(isize > 0) {
		  int64_t u;
          while((c = getc(fi)) >= '0' && c <= '9' || c == '-' || c == '+') 
		    if(p - s < LSIZE) *p++ = c;
	      if(c == '.')  { 
		    *p++ = c;
            while((c = getc(fi)) >= '0' && c <= '9' || c == '-' || c == '+' || c == 'e' || c == 'E') 
		      if(p - s < LSIZE) *p++ = c;
            *p = 0;         
		    u = pre>1.0?round(strtod(s, &q)*pre):strtod(s, &q) - mdelta;
		  } else { 
		    *p = 0;         
            u = strtoll(s, &p, 10) - mdelta;
		  }
          IPUSH(in,n,isize,nmax,u);												if(verbose>=5 && n < 100 || verbose>=9) printf("%s->%lld ", s, (int64_t)u);
        } else {
          while((c = getc(fi)) >= '0' && c <= '9' || c == '-')
		    if(p - s < LSIZE) *p++ = c;
	      if((*p++ = c) == '.')
            while((c = getc(fi)) >= '0' && c <= '9')
		      if(p - s < LSIZE) *p++ = c;
          *p = 0;
		  double d = strtod(s, &p) - mdelta;           							if(verbose>=5 && n < 100 || verbose>=9) printf("%f ", d);
          IPUSH(in,n,-isize,nmax,d);		
        }
        if(c == EOF) break;
      }
      break;
	default: die("unknown data format %d\n", fmt);
  }
  if(verbose>=5) printf("\n");
  *_in = in;
  return n*abs(isize);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
static int mcpy=1, cmp=2;

#define CBUF(_n_) (((size_t)(_n_))*5/3+1024)
int memcheck(unsigned char *in, unsigned n, unsigned char *cpy) { 
  int i;
  if(cmp <= 1) return 0;
  for(i = 0; i < n; i++)
    if(in[i] != cpy[i]) {
	  if(cmp > 3) abort(); // crash (AFL) fuzzing
      printf("ERROR at %d:in=%x,%x dec=%x,%x\n", i, in[i], in[i+1], cpy[i],cpy[i+1]);
	  if(cmp > 2) exit(EXIT_FAILURE);  
	  return i+1; 
	} 
  return 0;
}

void libmemcpy(unsigned char *dst, unsigned char *src, int len) {
  void *(*memcpy_ptr)(void *, const void *, size_t) = memcpy;
  if (time(NULL) == 1) 
    memcpy_ptr = NULL;
  memcpy_ptr(dst, src, len);
}

void pr(unsigned l, unsigned n) { double r = (double)l*100.0/n; if(r>0.1) printf("%10u %6.2f%% ", l, r);else printf("%10u %7.3f%%", l, r); fflush(stdout); }
//-------------------------------------
#include "bitutil.h"
#include "bitpack.h"
#define NTURBOPFOR_DAC
#include "vp4.h"
#include "vint.h"
#include "fp.h"
#include "vsimple.h"
#include "transpose.h"
#include "ext/trle.h"  

  #ifdef CODEC1
#define LZ4 
#define BITSHUFFLE 
  #endif

  #ifdef BITSHUFFLE
#include "bitshuffle/src/bitshuffle.h"
    #ifndef LZ4
#include "bitshuffle/lz4/lz4.h"
#define USE_LZ
    #endif
  #endif

  #ifdef LZ4
#include "lz4/lib/lz4.h"
#define USE_LZ
  #endif
  
  #ifdef LZTURBO
#include "../lz/lz8.h"
#include "../lz/lzb.h"
#define USE_LZ
  #endif
  
#define CPYR(in,n,esize,out) memcpy(out+((n)&(~(esize-1))),in+((n)&(~(esize-1))),(n)&(esize-1))  //, out+((n)&(8*esize-1))
//-----------------  vsimple ---------------------------
unsigned char *vszenc8( uint8_t  *in, unsigned n, unsigned char *out, unsigned char *tmp) { bitzenc8( in, n, tmp, 0, 0); return vsenc8( tmp, n, out); }
unsigned char *vszenc16(uint16_t *in, unsigned n, unsigned char *out, unsigned char *tmp) { bitzenc16(in, n, tmp, 0, 0); return vsenc16(tmp, n, out); }
unsigned char *vszenc32(uint32_t *in, unsigned n, unsigned char *out, unsigned char *tmp) { bitzenc32(in, n, tmp, 0, 0); return vsenc32(tmp, n, out); }
unsigned char *vszenc64(uint64_t *in, unsigned n, unsigned char *out, unsigned char *tmp) { bitzenc64(in, n, tmp, 0, 0); return vsenc64(tmp, n, out); }

unsigned char *vszdec8( unsigned char *in, unsigned n, unsigned char *out) { unsigned char *p = vsdec8( in,n,out); bitzdec8( out, n, 0); return p; }
unsigned char *vszdec16(unsigned char *in, unsigned n, unsigned char *out) { unsigned char *p = vsdec16(in,n,out); bitzdec16(out, n, 0); return p; }
unsigned char *vszdec32(unsigned char *in, unsigned n, unsigned char *out) { unsigned char *p = vsdec32(in,n,out); bitzdec32(out, n, 0); return p; }
unsigned char *vszdec64(unsigned char *in, unsigned n, unsigned char *out) { unsigned char *p = vsdec64(in,n,out); bitzdec64(out, n, 0); return p; }

//------------------ RLE -------------------------
#define RLE8  0xdau 
#define RLE16 0xdadau 
#define RLE32 0xdadadadau 
#define RLE64 0xdadadadadadadadaull 
unsigned trlezc( uint8_t      *in, unsigned n, unsigned char *out, unsigned char *tmp) { bitzenc8(in, n, tmp, 0, 0); return trlec(tmp, n, out); }
unsigned trlezd(unsigned char *in, unsigned inlen, uint8_t *out, unsigned n) { trled(in, inlen, out, n); bitzdec8(out, n, 0); return n; }

unsigned srlezc8( uint8_t  *in, unsigned n, unsigned char *out, unsigned char *tmp, uint8_t  e) { bitzenc8( in, n/( 8/8), tmp, 0, 0); return srlec8( tmp, n, out, e); }
unsigned srlezc16(uint16_t *in, unsigned n, unsigned char *out, unsigned char *tmp, uint16_t e) { bitzenc16(in, n/(16/8), tmp, 0, 0); return srlec16(tmp, n, out, e); }
unsigned srlezc32(uint32_t *in, unsigned n, unsigned char *out, unsigned char *tmp, uint32_t e) { bitzenc32(in, n/(32/8), tmp, 0, 0); return srlec32(tmp, n, out, e); }
unsigned srlezc64(uint64_t *in, unsigned n, unsigned char *out, unsigned char *tmp, uint64_t e) { bitzenc64(in, n/(64/8), tmp, 0, 0); return srlec64(tmp, n, out, e); }

unsigned srlezd8( unsigned char *in, unsigned inlen, unsigned char *out, unsigned n, uint8_t  e) { srled8(in,inlen,out,  n, e); bitzdec8( out, n/(8/8),  0); return n; }
unsigned srlezd16(unsigned char *in, unsigned inlen, unsigned char *out, unsigned n, uint16_t e) { srled16(in,inlen,out, n, e); bitzdec16(out, n/(16/8), 0); return n; }
unsigned srlezd32(unsigned char *in, unsigned inlen, unsigned char *out, unsigned n, uint32_t e) { srled32(in,inlen,out, n, e); bitzdec32(out, n/(32/8), 0); return n; }
unsigned srlezd64(unsigned char *in, unsigned inlen, unsigned char *out, unsigned n, uint64_t e) { srled64(in,inlen,out, n, e); bitzdec64(out, n/(64/8), 0); return n; }

//------------------- LZ --------------------------------------------------
#ifdef USE_LZ
unsigned lzcomp(unsigned char *in, unsigned n, unsigned char *out, int lev) { if(!n) return 0;
  unsigned outsize = CBUF(n);
    #ifdef LZTURBO
  struct lzobj lz; lz.srclen = n; lz.src = in; lz.dst = out+1; lz.dstlen = outsize; out[0]=lev;
	   switch(lev) {
         case 1: lz.level = 0; lz.hbits = 16; return lz8c01(&lz); 
         case 2: lz.level = 2; lz.hbits = 16; return lz8c01(&lz); 
	   }
	#elif defined(LZ4) || defined(BITSHUFFLE)
  int rc = !lev?LZ4_compress_fast((char *)in, (char *)(out+0), n,outsize, 4):(lev<9?LZ4_compress_default((char *)in, (char *)(out+0), n, outsize):
      #ifdef LZ4 
    LZ4_compress_HC((char *)in, (char *)(out+0), n, outsize, lev));
	  #else
	LZ4_compress_default((char *)in, (char *)(out+0), n, outsize));
      #endif
  return rc;
	#endif
}

unsigned lzdecomp(unsigned char *in, unsigned n, unsigned char *out) { if(!n) return 0;
  unsigned outsize = CBUF(n), lev;
    #ifdef LZTURBO
  struct lzobj lz; lz.dstlen = n; lz.src = in+1; lz.dst = out; lz.level = 0; lev = in[0]; 
  switch(lev) {
    case 1: 
	case 2: return lz8d(&lz); break;
 //   case 2: return lzbd(&lz); break;
//  case 3: return lzhd(&lz); break;
  }
    #else
  LZ4_decompress_fast((char *)(in+0), (char *)out, n/*, CBUF(n)*/);
	#endif
}

  #ifdef USE_SSE
unsigned tp4lzenc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize, unsigned char *tmp, int lev) {
  tp4enc(in, n, tmp, esize);
  return lzcomp(tmp, n, out, lev);
}

unsigned tp4lzdec(unsigned char *in, unsigned n, unsigned char *out, unsigned esize, unsigned char *tmp) {
  lzdecomp(in,n,tmp); 
  tp4dec(tmp, n, (unsigned char *)out, esize);
  return n;
}
//-----------
unsigned tp4lzzenc16(unsigned char *in, unsigned n, unsigned char *out, unsigned char *tmp, int lev) {
  bitzenc16(in, n/2, out, 0, 0); CPYR(in,n,2,out);
  tp4enc(out, n, tmp, 2); 
  return lzcomp(tmp, n, out, lev);
}

unsigned tp4lzzdec16(unsigned char *in, unsigned n, unsigned char *out, unsigned char *tmp) {
  lzdecomp(in,n,tmp); 
  tp4dec(tmp, n, (unsigned char *)out, 2);
  bitzdec16(out, n/2, 0);
  return n;
}

unsigned tp4lzxenc16(unsigned char *in, unsigned n, unsigned char *out, unsigned char *tmp, int lev) {
  bitxenc16(in, n/2, out, 0); CPYR(in,n,2,out);
  tp4enc(out, n, tmp, 2); 
  return lzcomp(tmp, n, out, lev);
}

unsigned tp4lzxdec16(unsigned char *in, unsigned n, unsigned char *out, unsigned char *tmp) {
  lzdecomp(in,n,tmp); 
  tp4dec(tmp, n, (unsigned char *)out, 2);
  bitxdec16(out, n/2, 0);
  return n;
}

//--------------
unsigned tp4lzzenc32(unsigned char *in, unsigned n, unsigned char *out, unsigned char *tmp, int lev) {
  bitzenc32(in, n/4, out, 0, 0); CPYR(in,n,4,out);
  tp4enc(out, n, tmp, 4); 
  return lzcomp(tmp, n, out, lev);
}

unsigned tp4lzzdec32(unsigned char *in, unsigned n, unsigned char *out, unsigned char *tmp) {
  lzdecomp(in,n,tmp); 
  tp4dec(tmp, n, (unsigned char *)out, 4);
  bitzdec32(out, n/4, 0);
  return n;
}

unsigned tp4lzxenc32(unsigned char *in, unsigned n, unsigned char *out, unsigned char *tmp, int lev) {
  bitxenc32(in, n/4, out, 0); CPYR(in,n,4,out);
  tp4enc(out, n, tmp, 4); 
  return lzcomp(tmp, n, out, lev);
}

unsigned tp4lzxdec32(unsigned char *in, unsigned n, unsigned char *out, unsigned char *tmp) {
  lzdecomp(in,n,tmp); 
  tp4dec(tmp, n, (unsigned char *)out, 4);
  bitxdec32(out, n/4, 0);
  return n;
}
//------------------
unsigned tp4lzzenc64(unsigned char *in, unsigned n, unsigned char *out, unsigned char *tmp, int lev) {
  bitzenc64(in, n/8, out, 0, 0); CPYR(in,n,8,out);
  tp4enc(out, n, tmp, 8); 
  return lzcomp(tmp, n, out, lev);
}

unsigned tp4lzzdec64(unsigned char *in, unsigned n, unsigned char *out, unsigned char *tmp) {
  lzdecomp(in,n,tmp); 
  tp4dec(tmp, n, (unsigned char *)out, 8);
  bitzdec64(out, n/8, 0);
  return n;
}

unsigned tp4lzxenc64(unsigned char *in, unsigned n, unsigned char *out, unsigned char *tmp, int lev) {
  bitxenc64(in, n/8, out, 0); CPYR(in,n,8,out);
  tp4enc(out, n, tmp, 8); 
  return lzcomp(tmp, n, out, lev);
}  
unsigned tp4lzxdec64(unsigned char *in, unsigned n, unsigned char *out, unsigned char *tmp) {
  lzdecomp(in,n,tmp); 
  tp4dec(tmp, n, (unsigned char *)out, 8);
  bitxdec64(out, n/8, 0);
  return n;
}
//--------------------------
unsigned tp4lzzenc8(unsigned char *in, unsigned n, unsigned char *out, unsigned char *tmp, int lev) {
  bitzenc8(in, n/1, out, 0, 0); 
  tp4enc(out, n, tmp, 1); 
  return lzcomp(tmp, n, out, lev);
}

unsigned tp4lzzdec8(unsigned char *in, unsigned n, unsigned char *out, unsigned char *tmp) {
  lzdecomp(in,n,tmp); 
  tp4dec(tmp, n, (unsigned char *)out, 1);
  bitzdec8(out, n/1, 0);
  return n;
}

unsigned tp4lzxenc8(unsigned char *in, unsigned n, unsigned char *out, unsigned char *tmp, int lev) {
  bitxenc8(in, n/1, out, 0); 
  tp4enc(out, n, tmp, 1); 
  return lzcomp(tmp, n, out, lev);
}

unsigned tp4lzxdec8(unsigned char *in, unsigned n, unsigned char *out, unsigned char *tmp) {
  lzdecomp(in,n,tmp); 
  tp4dec(tmp, n, (unsigned char *)out, 1);
  bitxdec8(out, n/1, 0);
  return n;
}
  #endif

unsigned tplzenc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize, unsigned char *tmp, int lev) {
  tpenc(in, n, tmp, esize); 
  return lzcomp(tmp, n, out, lev);
}

unsigned tplzdec(unsigned char *in, unsigned n, unsigned char *out, unsigned esize, unsigned char *tmp) {
  lzdecomp(in,n,tmp); 
  tpdec(tmp, n, (unsigned char *)out, esize);
  return n;
}

  #ifdef BITSHUFFLE
#define   BITSHUFFLE(in,n,out,esize) bshuf_bitshuffle(  in, out, (n)/esize, esize, 0); memcpy((char *)out+((n)&(~(8*esize-1))),(char *)in+((n)&(~(8*esize-1))),(n)&(8*esize-1))
#define BITUNSHUFFLE(in,n,out,esize) bshuf_bitunshuffle(in, out, (n)/esize, esize, 0); memcpy((char *)out+((n)&(~(8*esize-1))),(char *)in+((n)&(~(8*esize-1))),(n)&(8*esize-1))

unsigned bslzenc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize, unsigned char *tmp, int lev) {
  BITSHUFFLE(in, n, tmp, esize); 
  return lzcomp(tmp, n, out, lev);
}

unsigned bslzdec(unsigned char *in, unsigned n, unsigned char *out, unsigned esize, unsigned char *tmp) {
  lzdecomp(in,n,tmp); 
  BITUNSHUFFLE(tmp, n, (unsigned char *)out, esize);
  return n;
}
  #endif
#endif

//---------------------SPDP -------------------------
#define NMAIN
#include "ext/SPDP_10.c"
#define SPDP_BSIZE (1<<23)
char ibuf[SPDP_BSIZE*2];

unsigned spdpenc(unsigned char *in, size_t n, unsigned char *out, unsigned bsize, int lev) { 
  unsigned char *op = out,*ip;
  for(ip = in, in += n; ip < in;) {
    unsigned iplen = in - ip,l;
    if(iplen > bsize) iplen = bsize; 
    memcpy(ibuf,ip,iplen); // SPDP is overwriting the input, copy to a tmp buffer
    l = compress(lev, iplen, ibuf, op+4); ctou32(op)=l; op+=4+l; //AC(l <= bsize,"Compress Fatal=%d>%d\n", l, bsize);
	ip += iplen;
  }
  return op - out;
}

size_t spdpdec(unsigned char *in, size_t n, unsigned char *out, unsigned bsize, int lev) {\
  unsigned char *ip = in,*op;
  for(op = out,out+=n; op < out;) { unsigned oplen = out - op,l; 
    if(oplen > bsize) oplen = bsize;
    l = ctou32(ip); 
    ip += 4; 
    memcpy(ibuf,ip,l); 
    decompress(lev, l, ibuf, op); ip += l;
	op += oplen;
  } 
  return ip - in;
}
//--------------------------------------------------------------

#define ID_MEMCPY 80

unsigned bench8(unsigned char *in, unsigned n, unsigned char *out, unsigned char *cpy, int id, char *inname, int lev) { 
  unsigned l,m = n/(8/8),rc = 0, sorted=1;
  uint8_t *p;
  char *tmp = NULL; 
  if((id == 43 || id>=60 && id <= 79) && !(tmp = (unsigned char*)malloc(CBUF(n)))) die(stderr, "malloc error\n");

  for(p=(uint8_t*)in,l = 1; l < m; l++) if(p[l] < p[l-1]) { sorted = 0; break; }

  memrcpy(cpy,in,n); 
  switch(id) {
    case  1: TMBENCH("",l=p4nenc8(        in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nenc8        ",p4ndec8(          out, m, cpy)   ,n); break;
                                                                                                      
    case  4: TMBENCH("",l=p4ndenc8(       in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4ndenc8       ",p4nddec8(         out, m, cpy)   ,n); break;
                                                                                                       
    case  7: TMBENCH("",l=p4nd1enc8(      in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nd1enc8      ",p4nd1dec8(        out, m, cpy)   ,n); break;

    case 10: TMBENCH("",l=p4nzenc8(       in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nzenc8       ",p4nzdec8(         out, m, cpy)   ,n); break;
  //case 13: TMBENCH("",l=p4nsenc8(       in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nsenc8       ",p4nsdec8(         out, m, cpy)   ,n); break;
                                                                                                       
    case 20: TMBENCH("",l=bitnpack8(      in, m, out)  ,n);	        pr(l,n); TMBENCH2("bitnpack8      ",bitnunpack8(      out, m, cpy)   ,n); break;
                                                                                                       
    case 23: TMBENCH("",l=bitndpack8(     in, m, out)  ,n);	        pr(l,n); TMBENCH2("bitndpack8     ",bitndunpack8(     out, m, cpy)   ,n); break;
                                                                                                        
    case 26: TMBENCH("",l=bitnd1pack8(    in, m, out)  ,n);	        pr(l,n); TMBENCH2("bitnd1pack8    ",bitnd1unpack8(    out, m, cpy)   ,n); break;
 
    case 29: TMBENCH("",l=bitnzpack8(     in, m, out)  ,n);	        pr(l,n); TMBENCH2("bitnzpack8     ",bitnzunpack8(     out, m, cpy)   ,n); break;
      
    case 32: if(!sorted) return 0; TMBENCH("",l=bitnfpack8(     in, m, out)  ,n); pr(l,n); TMBENCH2("bitnfpack8     ",bitnfunpack8(     out, m, cpy)   ,n); break;

    case 41: TMBENCH("",l=vbzenc8(        in, m, out,0)-out,n);     pr(l,n); TMBENCH2("vbzenc8        ",vbzdec8(          out, m, cpy,0) ,n); break; 
    case 42: TMBENCH("",l=vsenc8(         in, m, out)-out,n); 	    pr(l,n); TMBENCH2("vsenc8         ",vsdec8(           out, m, cpy) ,n); break;   // vsimple : variable simple
    case 43: TMBENCH("",l=vszenc8(        in, m, out,tmp)-out,n);   pr(l,n); TMBENCH2("vszenc8        ",vszdec8(          out, m, cpy) ,n); break;   
	   
    case 50: TMBENCH("",l=bvzzenc8(       in, m, out,0),n); 	    pr(l,n); TMBENCH2("bvzzenc8       ",bvzzdec8(         out, m, cpy,0) ,n); break; // bitio	
    case 51: TMBENCH("",l=bvzenc8(        in, m, out,0),n); 	    pr(l,n); TMBENCH2("bvzenc8        ",bvzdec8(          out, m, cpy,0) ,n); break; 
    case 52: TMBENCH("",l=fpgenc8(        in, m, out,0),n); 	    pr(l,n); TMBENCH2("fpgenc8        ",fpgdec8(          out, m, cpy,0) ,n); break;  

    case 55: TMBENCH("",l=fpzzenc8(       in, m, out,0),n); 	    pr(l,n); TMBENCH2("fpzzenc8       ",fpzzdec8(         out, m, cpy,0) ,n); break; //Floating point/Integer
    case 56: TMBENCH("",l=fpfcmenc8(      in, m, out,0),n); 	    pr(l,n); TMBENCH2("fpfcmenc8      ",fpfcmdec8(        out, m, cpy,0) ,n); break;
    case 57: TMBENCH("",l=fpdfcmenc8(     in, m, out,0),n); 	    pr(l,n); TMBENCH2("fpdfcmenc8     ",fpdfcmdec8(       out, m, cpy,0) ,n); break;
    case 58: TMBENCH("",l=fp2dfcmenc8(    in, m, out,0),n); 	    pr(l,n); TMBENCH2("fp2dfcmenc8    ",fp2dfcmdec8(      out, m, cpy,0) ,n); break;

    case 60: TMBENCH("",l=trlec(          in, n,out),n);            pr(l,n); TMBENCH2("trle            ",trled(             out,l,cpy, n),n);      break;  // TurboRLE
    case 61: TMBENCH("",l=trlezc(         in, n,out,tmp),n);        pr(l,n); TMBENCH2("trlez           ",trlezd(            out,l,cpy, n),n);      break;  
    case 62: TMBENCH("",l=srlec8(         in, n,out,RLE8),n);       pr(l,n); TMBENCH2("srle8          ",srled8(           out,l,cpy, n,RLE8),n);break;
    case 63: TMBENCH("",l=srlezc8(        in, n,out,tmp,RLE8),n);   pr(l,n); TMBENCH2("srlez8         ",srlezd8(          out,l,cpy, n,RLE8),n);break;
      #ifdef USE_LZ
    case 69: TMBENCH("",l=spdpenc(in,m*(8/8),out,SPDP_BSIZE,lev),n);pr(l,n);TMBENCH2("SPDP            ",spdpdec(           out, m*(8/8), cpy,SPDP_BSIZE,lev); ,n); break;
    case 70: TMBENCH("",l=tplzenc(    in, n,out,8/8,tmp,lev) ,n);  pr(l,n); TMBENCH2("tpbyte+lz       ",tplzdec(           out,n,cpy,8/8,tmp) ,n); break;
    case 71: TMBENCH("",l=tp4lzenc(   in, n,out,8/8,tmp,lev) ,n);  pr(l,n); TMBENCH2("tpnibble+lz     ",tp4lzdec(          out,n,cpy,8/8,tmp) ,n); break;
    case 72: TMBENCH("",l=tp4lzzenc8(in, n,out,     tmp,lev) ,n);  pr(l,n); TMBENCH2("tpnibbleZ+lz    ",tp4lzzdec8(       out,n,cpy,     tmp) ,n); break;
    case 73: TMBENCH("",l=tp4lzxenc8(in, n,out,     tmp,lev) ,n);  pr(l,n); TMBENCH2("tpnibbleX+lz    ",tp4lzxdec8(       out,n,cpy,     tmp) ,n); break;
    case 74: TMBENCH("",l=lzcomp(      in, n,out,         lev) ,n); pr(l,n); TMBENCH2("lz              ",lzdecomp(          out,n,cpy)          ,n);break;
	    #ifdef BITSHUFFLE
    case 75: TMBENCH("",l=bslzenc(    in,n,out,8/8,tmp,lev), n);  pr(l,n);  TMBENCH2("bitshuffle+lz   ",bslzdec(out,n,cpy,8/8,tmp), n); break;
        #endif
      #endif
    case ID_MEMCPY: if(!mcpy) return l; TMBENCH( "", libmemcpy(  in,out,n) ,n); pr(n,n); TMBENCH2("memcpy         ", libmemcpy( out,cpy,n) ,n); return n;
	default: return l;
  }
  rc = memcheck(in,m*(8/8),cpy);
  if(tmp) free(tmp);
  if(!rc)
    printf("\t%s\n", inname);
  return l;
}

unsigned bench16(unsigned char *in, unsigned n, unsigned char *out, unsigned char *cpy, int id, char *inname, int lev) { 
  unsigned l,m = n/(16/8),rc = 0, sorted=1;
  uint16_t *p;
  char *tmp = NULL; 
  if((id == 43 || id>=60 && id <= 79) && !(tmp = (unsigned char*)malloc(CBUF(n)))) die(stderr, "malloc error\n");

  for(p=(uint16_t*)in,l = 1; l < m; l++) if(p[l] < p[l-1]) { sorted = 0; break; }

  memrcpy(cpy,in,n); 
  switch(id) {
    case  1: TMBENCH("",l=p4nenc16(        in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nenc16        ",p4ndec16(          out, m, cpy)   ,n); break;
    case  2: TMBENCH("",l=p4nenc128v16(    in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nenc128v16    ",p4ndec128v16(      out, m, cpy)   ,n); break;
                                                                                                       
    case  4: TMBENCH("",l=p4ndenc16(       in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4ndenc16       ",p4nddec16(         out, m, cpy)   ,n); break;
    case  5: TMBENCH("",l=p4ndenc128v16(   in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4ndenc128v16   ",p4nddec128v16(     out, m, cpy)   ,n); break;
                                                                                                       
    case  7: TMBENCH("",l=p4nd1enc16(      in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nd1enc16      ",p4nd1dec16(        out, m, cpy)   ,n); break;
    case  8: TMBENCH("",l=p4nd1enc128v16(  in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nd1enc128v16  ",p4nd1dec128v16(    out, m, cpy)   ,n); break;

    case 10: TMBENCH("",l=p4nzenc16(       in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nzenc16       ",p4nzdec16(         out, m, cpy)   ,n); break;
    case 11: TMBENCH("",l=p4nzenc128v16(   in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nzenc128v16   ",p4nzdec128v16(     out, m, cpy)   ,n); break;
  //case 13: TMBENCH("",l=p4nsenc16(       in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nsenc16       ",p4nsdec16(         out, m, cpy)   ,n); break;
                                                                                                       
    case 20: TMBENCH("",l=bitnpack16(      in, m, out)  ,n);	    pr(l,n); TMBENCH2("bitnpack16      ",bitnunpack16(      out, m, cpy)   ,n); break;
    case 21: TMBENCH("",l=bitnpack128v16(  in, m, out)  ,n);	    pr(l,n); TMBENCH2("bitnpack128v16  ",bitnunpack128v16(  out, m, cpy)   ,n); break;
                                                                                                       
    case 23: TMBENCH("",l=bitndpack16(     in, m, out)  ,n);	    pr(l,n); TMBENCH2("bitndpack16     ",bitndunpack16(     out, m, cpy)   ,n); break;
    case 24: TMBENCH("",l=bitndpack128v16( in, m, out)  ,n);	    pr(l,n); TMBENCH2("bitndpack128v16 ",bitndunpack128v16( out, m, cpy)   ,n); break;
                                                                                                        
    case 26: TMBENCH("",l=bitnd1pack16(    in, m, out)  ,n);	    pr(l,n); TMBENCH2("bitnd1pack16    ",bitnd1unpack16(    out, m, cpy)   ,n); break;
    case 27: TMBENCH("",l=bitnd1pack128v16(in, m, out)  ,n);	    pr(l,n); TMBENCH2("bitnd1pack128v16",bitnd1unpack128v16(out, m, cpy)   ,n); break;
 
    case 29: TMBENCH("",l=bitnzpack16(     in, m, out)  ,n);	    pr(l,n); TMBENCH2("bitnzpack16     ",bitnzunpack16(     out, m, cpy)   ,n); break;
    case 30: TMBENCH("",l=bitnzpack128v16( in, m, out)  ,n);	    pr(l,n); TMBENCH2("bitnzpack128v16 ",bitnzunpack128v16( out, m, cpy)   ,n); break;
      
    case 32: if(!sorted) return 0; TMBENCH("",l=bitnfpack16(     in, m, out)  ,n); pr(l,n); TMBENCH2("bitnfpack16     ",bitnfunpack16(     out, m, cpy)   ,n); break;
    case 33: if(!sorted) return 0; TMBENCH("",l=bitnfpack128v16( in, m, out)  ,n); pr(l,n); TMBENCH2("bitnfpack128v16 ",bitnfunpack128v16( out, m, cpy)   ,n); break;

    case 40: TMBENCH("",l=vbenc16(         in, m, out)-out,n);      pr(l,n); TMBENCH2("vbenc16         ",vbdec16(           out, m, cpy) ,n); break; // TurboVbyte : variable byte
    case 41: TMBENCH("",l=vbzenc16(        in, m, out,0)-out,n);    pr(l,n); TMBENCH2("vbzenc16        ",vbzdec16(          out, m, cpy,0) ,n); break; 
    case 42: TMBENCH("",l=vsenc16(         in, m, out)-out,n); 	    pr(l,n); TMBENCH2("vsenc16         ",vsdec16(           out, m, cpy) ,n); break;   // vsimple : variable simple
    case 43: TMBENCH("",l=vszenc16(        in, m, out,tmp)-out,n);  pr(l,n); TMBENCH2("vszenc16        ",vszdec16(          out, m, cpy) ,n); break;   
	   
    case 50: TMBENCH("",l=bvzzenc16(       in, m, out,0),n); 	    pr(l,n); TMBENCH2("bvzzenc16       ",bvzzdec16(         out, m, cpy,0) ,n); break; // bitio	
    case 51: TMBENCH("",l=bvzenc16(        in, m, out,0),n); 	    pr(l,n); TMBENCH2("bvzenc16        ",bvzdec16(          out, m, cpy,0) ,n); break; 
    case 52: TMBENCH("",l=fpgenc16(        in, m, out,0),n); 	    pr(l,n); TMBENCH2("fpgenc16        ",fpgdec16(          out, m, cpy,0) ,n); break;  

    case 55: TMBENCH("",l=fpzzenc16(       in, m, out,0),n); 	    pr(l,n); TMBENCH2("fpzzenc16       ",fpzzdec16(         out, m, cpy,0) ,n); break; //Floating point/Integer
    case 56: TMBENCH("",l=fpfcmenc16(      in, m, out,0),n); 	    pr(l,n); TMBENCH2("fpfcmenc16      ",fpfcmdec16(        out, m, cpy,0) ,n); break;
    case 57: TMBENCH("",l=fpdfcmenc16(     in, m, out,0),n); 	    pr(l,n); TMBENCH2("fpdfcmenc16     ",fpdfcmdec16(       out, m, cpy,0) ,n); break;
    case 58: TMBENCH("",l=fp2dfcmenc16(    in, m, out,0),n); 	    pr(l,n); TMBENCH2("fp2dfcmenc16    ",fp2dfcmdec16(      out, m, cpy,0) ,n); break;

    case 60: TMBENCH("",l=trlec(           in, n,out),n);           pr(l,n); TMBENCH2("trle            ",trled(             out,l,cpy, n),n);      break;  // TurboRLE
    case 61: TMBENCH("",l=trlezc(          in, n,out,tmp),n);       pr(l,n); TMBENCH2("trlez           ",trlezd(            out,l,cpy, n),n);      break;  
    case 62: TMBENCH("",l=srlec16(         in, n,out,RLE16),n);     pr(l,n); TMBENCH2("srle16          ",srled16(           out,l,cpy, n,RLE16),n);break;
    case 63: TMBENCH("",l=srlezc16(        in, n,out,tmp,RLE16),n); pr(l,n); TMBENCH2("srlez16         ",srlezd16(          out,l,cpy, n,RLE16),n);break;
      #ifdef USE_LZ
    case 69: TMBENCH("",l=spdpenc(in,m*(16/8),out,SPDP_BSIZE,lev),n);pr(l,n);TMBENCH2("SPDP            ",spdpdec(           out, m*(16/8), cpy,SPDP_BSIZE,lev); ,n); break;
    case 70: TMBENCH("",l=tplzenc(    in, n,out,16/8,tmp,lev) ,n);  pr(l,n); TMBENCH2("tpbyte+lz       ",tplzdec(           out,n,cpy,16/8,tmp) ,n); break;
    case 71: TMBENCH("",l=tp4lzenc(   in, n,out,16/8,tmp,lev) ,n);  pr(l,n); TMBENCH2("tpnibble+lz     ",tp4lzdec(          out,n,cpy,16/8,tmp) ,n); break;
    case 72: TMBENCH("",l=tp4lzzenc16(in, n,out,     tmp,lev) ,n);  pr(l,n); TMBENCH2("tpnibbleZ+lz    ",tp4lzzdec16(       out,n,cpy,     tmp) ,n); break;
    case 73: TMBENCH("",l=tp4lzxenc16(in, n,out,     tmp,lev) ,n);  pr(l,n); TMBENCH2("tpnibbleX+lz    ",tp4lzxdec16(       out,n,cpy,     tmp) ,n); break;
    case 74: TMBENCH("",l=lzcomp(      in, n,out,         lev) ,n); pr(l,n); TMBENCH2("lz              ",lzdecomp(          out,n,cpy)          ,n);break;
	    #ifdef BITSHUFFLE
    case 75: TMBENCH("",l=bslzenc(    in,n,out,16/8,tmp,lev), n);  pr(l,n);  TMBENCH2("bitshuffle+lz   ",bslzdec(out,n,cpy,16/8,tmp), n); break;
        #endif
      #endif
    case ID_MEMCPY: if(!mcpy) return l; TMBENCH( "", libmemcpy(  in,out,n) ,n); pr(n,n); TMBENCH2("memcpy         ", libmemcpy( out,cpy,n) ,n); return n;
	default: return l;
  }
  rc = memcheck(in,m*(16/8),cpy);
  if(tmp) free(tmp);
  if(!rc)
    printf("\t%s\n", inname);
  return l;
}

unsigned bench32(unsigned char *in, unsigned n, unsigned char *out, unsigned char *cpy, int id, char *inname, int lev) { 
  unsigned l,m = n/(32/8),rc = 0, sorted=1;
  uint32_t *p;
  char *tmp = NULL; 
  if((id == 43 || id>=60 && id <= 79) && !(tmp = (unsigned char*)malloc(CBUF(n)))) die(stderr, "malloc error\n");

  for(p=(uint32_t*)in,l = 1; l < m; l++) if(p[l] < p[l-1]) { sorted = 0; break; }

  memrcpy(cpy,in,n); 
  switch(id) {
    case  1: TMBENCH("",l=p4nenc32(        in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nenc32        ",p4ndec32(          out, m, cpy)   ,n); break;
    case  2: TMBENCH("",l=p4nenc128v32(    in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nenc128v32    ",p4ndec128v32(      out, m, cpy)   ,n); break;
	  #if defined(__AVX2__) && defined(USE_AVX2)
    case  3: TMBENCH("",l=p4nenc256v32(    in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nenc256v32    ",p4ndec256v32(      out, m, cpy)   ,n); break;
      #endif
                                                                                                       
    case  4: TMBENCH("",l=p4ndenc32(       in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4ndenc32       ",p4nddec32(         out, m, cpy)   ,n); break;
    case  5: TMBENCH("",l=p4ndenc128v32(   in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4ndenc128v32   ",p4nddec128v32(     out, m, cpy)   ,n); break;
	  #if defined(__AVX2__) && defined(USE_AVX2)
    case  6: TMBENCH("",l=p4ndenc256v32( in, m, out)  ,n);   	    pr(l,n); TMBENCH2("p4ndenc256v32 ", p4nddec256v32( out, m, cpy)   ,n); break;
      #endif
                                                                                                       
    case  7: TMBENCH("",l=p4nd1enc32(      in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nd1enc32      ",p4nd1dec32(        out, m, cpy)   ,n); break;
    case  8: TMBENCH("",l=p4nd1enc128v32(  in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nd1enc128v32  ",p4nd1dec128v32(    out, m, cpy)   ,n); break;
	  #if defined(__AVX2__) && defined(USE_AVX2)
    case  9: TMBENCH("",l=p4nd1enc256v32(  in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nd1enc256v32  ",p4nd1dec256v32(     out, m, cpy)   ,n); break;
      #endif

    case 10: TMBENCH("",l=p4nzenc32(       in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nzenc32       ",p4nzdec32(         out, m, cpy)   ,n); break;
    case 11: TMBENCH("",l=p4nzenc128v32(   in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nzenc128v32   ",p4nzdec128v32(     out, m, cpy)   ,n); break;
	  #if defined(__AVX2__) && defined(USE_AVX2)
    case 12: TMBENCH("",l=p4nzenc256v32(   in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nzenc256v32   ",p4nzdec256v32(     out, m, cpy)   ,n); break;
      #endif
  //case 13: TMBENCH("",l=p4nsenc32(       in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nsenc32       ",p4nsdec32(         out, m, cpy)   ,n); break;
                                                                                                       
    case 20: TMBENCH("",l=bitnpack32(      in, m, out)  ,n);	    pr(l,n); TMBENCH2("bitnpack32      ",bitnunpack32(      out, m, cpy)   ,n); break;
    case 21: TMBENCH("",l=bitnpack128v32(  in, m, out)  ,n);	    pr(l,n); TMBENCH2("bitnpack128v32  ",bitnunpack128v32(  out, m, cpy)   ,n); break;
	  #if defined(__AVX2__) && defined(USE_AVX2)
    case 22: TMBENCH("",l=bitnpack256v32(  in, m, out)  ,n);   	    pr(l,n); TMBENCH2("bitnpack256v32  ",bitnunpack256v32(  out, m, cpy)   ,n); break;
      #endif
                                                                                                       
    case 23: TMBENCH("",l=bitndpack32(     in, m, out)  ,n);	    pr(l,n); TMBENCH2("bitndpack32     ",bitndunpack32(     out, m, cpy)   ,n); break;
    case 24: TMBENCH("",l=bitndpack128v32( in, m, out)  ,n);	    pr(l,n); TMBENCH2("bitndpack128v32 ",bitndunpack128v32( out, m, cpy)   ,n); break;
	  #if defined(__AVX2__) && defined(USE_AVX2)
    case 25: TMBENCH("",l=bitndpack256v32( in, m, out)  ,n);        pr(l,n); TMBENCH2("bitndpack256v32 ",bitndunpack256v32( out, m, cpy)   ,n); break;
      #endif 
                                                                                                        
    case 26: TMBENCH("",l=bitnd1pack32(    in, m, out)  ,n);	    pr(l,n); TMBENCH2("bitnd1pack32    ",bitnd1unpack32(    out, m, cpy)   ,n); break;
    case 27: TMBENCH("",l=bitnd1pack128v32(in, m, out)  ,n);	    pr(l,n); TMBENCH2("bitnd1pack128v32",bitnd1unpack128v32(out, m, cpy)   ,n); break;
	  #if defined(__AVX2__) && defined(USE_AVX2)
    case 28: TMBENCH("",l=bitnd1pack256v32(in, m, out)  ,n);   	    pr(l,n); TMBENCH2("bitnd1pack256v32",bitnd1unpack256v32(out, m, cpy)   ,n); break;
      #endif
 
    case 29: TMBENCH("",l=bitnzpack32(     in, m, out)  ,n);	    pr(l,n); TMBENCH2("bitnzpack32     ",bitnzunpack32(     out, m, cpy)   ,n); break;
    case 30: TMBENCH("",l=bitnzpack128v32( in, m, out)  ,n);	    pr(l,n); TMBENCH2("bitnzpack128v32 ",bitnzunpack128v32( out, m, cpy)   ,n); break;
	  #if defined(__AVX2__) && defined(USE_AVX2)
    case 31: TMBENCH("",l=bitnzpack256v32( in, m, out)  ,n);   	    pr(l,n); TMBENCH2("bitnzpack256v32 ",bitnzunpack256v32( out, m, cpy)   ,n); break;                                                                                                      
      #endif     
      
    case 32: if(!sorted) return 0; TMBENCH("",l=bitnfpack32(     in, m, out)  ,n); pr(l,n); TMBENCH2("bitnfpack32     ",bitnfunpack32(     out, m, cpy)   ,n); break;
    case 33: if(!sorted) return 0; TMBENCH("",l=bitnfpack128v32( in, m, out)  ,n); pr(l,n); TMBENCH2("bitnfpack128v32 ",bitnfunpack128v32( out, m, cpy)   ,n); break;
	  #if defined(__AVX2__) && defined(USE_AVX2)
    case 34: if(sorted) { TMBENCH("",l=bitnfpack256v32( in, m, out)  ,n); pr(l,n); TMBENCH2("bitnfpack256v32 ",bitnfunpack256v32( out, m, cpy)   ,n); } break;
      #endif 
    case 40: TMBENCH("",l=vbenc32(         in, m, out)-out,n);      pr(l,n); TMBENCH2("vbenc32         ",vbdec32(           out, m, cpy) ,n); break; // TurboVbyte : variable byte
    case 41: TMBENCH("",l=vbzenc32(        in, m, out,0)-out,n);    pr(l,n); TMBENCH2("vbzenc32        ",vbzdec32(          out, m, cpy,0) ,n); break; 
    case 42: TMBENCH("",l=vsenc32(         in, m, out)-out,n); 	    pr(l,n); TMBENCH2("vsenc32         ",vsdec32(           out, m, cpy) ,n); break;   // vsimple : variable simple
    case 43: TMBENCH("",l=vszenc32(        in, m, out,tmp)-out,n);  pr(l,n); TMBENCH2("vszenc32        ",vszdec32(          out, m, cpy) ,n); break;   // vsimple : variable simple
	   
    case 50: TMBENCH("",l=bvzzenc32(       in, m, out,0),n); 	    pr(l,n); TMBENCH2("bvzzenc32       ",bvzzdec32(         out, m, cpy,0) ,n); break; // bitio	
    case 51: TMBENCH("",l=bvzenc32(        in, m, out,0),n); 	    pr(l,n); TMBENCH2("bvzenc32        ",bvzdec32(          out, m, cpy,0) ,n); break; 
    case 52: TMBENCH("",l=fpgenc32(        in, m, out,0),n); 	    pr(l,n); TMBENCH2("fpgenc32        ",fpgdec32(          out, m, cpy,0) ,n); break;  

    case 55: TMBENCH("",l=fpzzenc32(       in, m, out,0),n); 	    pr(l,n); TMBENCH2("fpzzenc32       ",fpzzdec32(         out, m, cpy,0) ,n); break; //Floating point/Integer
    case 56: TMBENCH("",l=fpfcmenc32(      in, m, out,0),n); 	    pr(l,n); TMBENCH2("fpfcmenc32      ",fpfcmdec32(        out, m, cpy,0) ,n); break;
    case 57: TMBENCH("",l=fpdfcmenc32(     in, m, out,0),n); 	    pr(l,n); TMBENCH2("fpdfcmenc32     ",fpdfcmdec32(       out, m, cpy,0) ,n); break;
    case 58: TMBENCH("",l=fp2dfcmenc32(    in, m, out,0),n); 	    pr(l,n); TMBENCH2("fp2dfcmenc32    ",fp2dfcmdec32(      out, m, cpy,0) ,n); break;

    case 60: TMBENCH("",l=trlec(           in, n,out),n);           pr(l,n); TMBENCH2("trle            ",trled(             out,l,cpy, n),n);      break;  // TurboRLE
    case 61: TMBENCH("",l=trlezc(          in, n,out,tmp),n);       pr(l,n); TMBENCH2("trlez           ",trlezd(            out,l,cpy, n),n);      break;  
    case 62: TMBENCH("",l=srlec32(         in, n,out,RLE32),n);     pr(l,n); TMBENCH2("srle32          ",srled32(           out,l,cpy, n,RLE32),n);break;
    case 63: TMBENCH("",l=srlezc32(        in, n,out,tmp,RLE32),n); pr(l,n); TMBENCH2("srlez32         ",srlezd32(          out,l,cpy, n,RLE32),n);break;
      #ifdef USE_LZ
    case 69: TMBENCH("",l=spdpenc(in,m*(32/8),out,SPDP_BSIZE,lev),n);pr(l,n);TMBENCH2("SPDP            ",spdpdec(           out, m*(32/8), cpy,SPDP_BSIZE,lev); ,n); break;
    case 70: TMBENCH("",l=tplzenc(    in, n,out,32/8,tmp,lev) ,n);  pr(l,n); TMBENCH2("tpbyte+lz       ",tplzdec(           out,n,cpy,32/8,tmp) ,n); break;
    case 71: TMBENCH("",l=tp4lzenc(   in, n,out,32/8,tmp,lev) ,n);  pr(l,n); TMBENCH2("tpnibble+lz     ",tp4lzdec(          out,n,cpy,32/8,tmp) ,n); break;
    case 72: TMBENCH("",l=tp4lzzenc32(in, n,out,     tmp,lev) ,n);  pr(l,n); TMBENCH2("tpnibbleZ+lz    ",tp4lzzdec32(       out,n,cpy,     tmp) ,n); break;
    case 73: TMBENCH("",l=tp4lzxenc32(in, n,out,     tmp,lev) ,n);  pr(l,n); TMBENCH2("tpnibbleX+lz    ",tp4lzxdec32(       out,n,cpy,     tmp) ,n); break;
    case 74: TMBENCH("",l=lzcomp(      in, n,out,         lev) ,n); pr(l,n); TMBENCH2("lz              ",lzdecomp(          out,n,cpy)          ,n);break;
	    #ifdef BITSHUFFLE
    case 75: TMBENCH("",l=bslzenc(    in,n,out,32/8,tmp,lev), n);  pr(l,n);  TMBENCH2("bitshuffle+lz   ",bslzdec(out,n,cpy,32/8,tmp), n); break;
        #endif
      #endif
    case ID_MEMCPY: if(!mcpy) return l; TMBENCH( "", libmemcpy(  in,out,n) ,n); pr(n,n); TMBENCH2("memcpy         ", libmemcpy( out,cpy,n) ,n); return n;
	default: return l;
  }
  rc = memcheck(in,m*(32/8),cpy);
  if(tmp) free(tmp);
  if(!rc)
    printf("\t%s\n", inname);
  return l;
}

unsigned bench64(unsigned char *in, unsigned n, unsigned char *out, unsigned char *cpy, int id, char *inname, int lev) { 
  unsigned l,m = n/(64/8),rc = 0, sorted=1;
  uint64_t *p;
  char *tmp = NULL; 
  if((id == 43 || id>=60 && id <= 79) && !(tmp = (unsigned char*)malloc(CBUF(n)))) die(stderr, "malloc error\n");

  for(p=(uint64_t*)in,l = 1; l < m; l++) if(p[l] < p[l-1]) { sorted = 0; break; }

  memrcpy(cpy,in,n); 
  switch(id) {
    case  1: TMBENCH("",l=p4nenc64(        in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nenc64        ",p4ndec64(          out, m, cpy)   ,n); break;
    case  2: TMBENCH("",l=p4nenc128v64(    in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nenc128v64    ",p4ndec128v64(      out, m, cpy)   ,n); break;
                                                                                                       
    case  4: TMBENCH("",l=p4ndenc64(       in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4ndenc64       ",p4nddec64(         out, m, cpy)   ,n); break;
                                                                                                       
    case  7: TMBENCH("",l=p4nd1enc64(      in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nd1enc64      ",p4nd1dec64(        out, m, cpy)   ,n); break;

    case 10: TMBENCH("",l=p4nzenc64(       in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nzenc64       ",p4nzdec64(         out, m, cpy)   ,n); break;
  //case 13: TMBENCH("",l=p4nsenc64(       in, m, out)  ,n); 	    pr(l,n); TMBENCH2("p4nsenc64       ",p4nsdec64(         out, m, cpy)   ,n); break;
                                                                                                       
    case 20: TMBENCH("",l=bitnpack64(      in, m, out)  ,n);	    pr(l,n); TMBENCH2("bitnpack64      ",bitnunpack64(      out, m, cpy)   ,n); break;
    case 21: TMBENCH("",l=bitnpack128v64(  in, m, out)  ,n);	    pr(l,n); TMBENCH2("bitnpack128v64  ",bitnunpack128v64(  out, m, cpy)   ,n); break;
                                                                                                       
    case 23: TMBENCH("",l=bitndpack64(     in, m, out)  ,n);	    pr(l,n); TMBENCH2("bitndpack64     ",bitndunpack64(     out, m, cpy)   ,n); break;
                                                                                                        
    case 26: TMBENCH("",l=bitnd1pack64(    in, m, out)  ,n);	    pr(l,n); TMBENCH2("bitnd1pack64    ",bitnd1unpack64(    out, m, cpy)   ,n); break;
 
    case 29: TMBENCH("",l=bitnzpack64(     in, m, out)  ,n);	    pr(l,n); TMBENCH2("bitnzpack64     ",bitnzunpack64(     out, m, cpy)   ,n); break;
      
    case 32: if(!sorted) return 0; TMBENCH("",l=bitnfpack64(     in, m, out)  ,n); pr(l,n); TMBENCH2("bitnfpack64     ",bitnfunpack64(     out, m, cpy)   ,n); break;

    case 40: TMBENCH("",l=vbenc64(         in, m, out)-out,n);      pr(l,n); TMBENCH2("vbenc64         ",vbdec64(           out, m, cpy) ,n); break; // TurboVbyte : variable byte
    case 41: TMBENCH("",l=vbzenc64(        in, m, out,0)-out,n);    pr(l,n); TMBENCH2("vbzenc64        ",vbzdec64(          out, m, cpy,0) ,n); break; 
    case 42: TMBENCH("",l=vsenc64(         in, m, out)-out,n); 	    pr(l,n); TMBENCH2("vsenc64         ",vsdec64(           out, m, cpy) ,n); break;   // vsimple : variable simple
    case 43: TMBENCH("",l=vszenc64(        in, m, out,tmp)-out,n);  pr(l,n); TMBENCH2("vszenc64        ",vszdec64(          out, m, cpy) ,n); break;   // vsimple : variable simple
	   
    case 50: TMBENCH("",l=bvzzenc64(       in, m, out,0),n); 	    pr(l,n); TMBENCH2("bvzzenc64       ",bvzzdec64(         out, m, cpy,0) ,n); break; // bitio	
    case 51: TMBENCH("",l=bvzenc64(        in, m, out,0),n); 	    pr(l,n); TMBENCH2("bvzenc64        ",bvzdec64(          out, m, cpy,0) ,n); break; 
    case 52: TMBENCH("",l=fpgenc64(        in, m, out,0),n); 	    pr(l,n); TMBENCH2("fpgenc64        ",fpgdec64(          out, m, cpy,0) ,n); break;  

    case 55: TMBENCH("",l=fpzzenc64(       in, m, out,0),n); 	    pr(l,n); TMBENCH2("fpzzenc64       ",fpzzdec64(         out, m, cpy,0) ,n); break; //Floating point/Integer
    case 56: TMBENCH("",l=fpfcmenc64(      in, m, out,0),n); 	    pr(l,n); TMBENCH2("fpfcmenc64      ",fpfcmdec64(        out, m, cpy,0) ,n); break;
    case 57: TMBENCH("",l=fpdfcmenc64(     in, m, out,0),n); 	    pr(l,n); TMBENCH2("fpdfcmenc64     ",fpdfcmdec64(       out, m, cpy,0) ,n); break;
    case 58: TMBENCH("",l=fp2dfcmenc64(    in, m, out,0),n); 	    pr(l,n); TMBENCH2("fp2dfcmenc64    ",fp2dfcmdec64(      out, m, cpy,0) ,n); break;

    case 60: TMBENCH("",l=trlec(           in, n,out),n);           pr(l,n); TMBENCH2("trle            ",trled(             out,l,cpy, n),n);      break;  // TurboRLE
    case 61: TMBENCH("",l=trlezc(          in, n,out,tmp),n);       pr(l,n); TMBENCH2("trlez           ",trlezd(            out,l,cpy, n),n);      break;  
    case 62: TMBENCH("",l=srlec64(         in, n,out,RLE64),n);     pr(l,n); TMBENCH2("srle64          ",srled64(           out,l,cpy, n,RLE64),n);break;
    case 63: TMBENCH("",l=srlezc64(        in, n,out,tmp,RLE64),n); pr(l,n); TMBENCH2("srlez64         ",srlezd64(          out,l,cpy, n,RLE64),n);break;
      #ifdef USE_LZ
    case 69: TMBENCH("",l=spdpenc(in,m*(64/8),out,SPDP_BSIZE,lev),n);pr(l,n);TMBENCH2("SPDP            ",spdpdec(           out, m*(64/8), cpy,SPDP_BSIZE,lev); ,n); break;
    case 70: TMBENCH("",l=tplzenc(    in, n,out,64/8,tmp,lev) ,n);  pr(l,n); TMBENCH2("tpbyte+lz       ",tplzdec(           out,n,cpy,64/8,tmp) ,n); break;
    case 71: TMBENCH("",l=tp4lzenc(   in, n,out,64/8,tmp,lev) ,n);  pr(l,n); TMBENCH2("tpnibble+lz     ",tp4lzdec(          out,n,cpy,64/8,tmp) ,n); break;
    case 72: TMBENCH("",l=tp4lzzenc64(in, n,out,     tmp,lev) ,n);  pr(l,n); TMBENCH2("tpnibbleZ+lz    ",tp4lzzdec64(       out,n,cpy,     tmp) ,n); break;
    case 73: TMBENCH("",l=tp4lzxenc64(in, n,out,     tmp,lev) ,n);  pr(l,n); TMBENCH2("tpnibbleX+lz    ",tp4lzxdec64(       out,n,cpy,     tmp) ,n); break;
    case 74: TMBENCH("",l=lzcomp(      in, n,out,         lev) ,n); pr(l,n); TMBENCH2("lz              ",lzdecomp(          out,n,cpy)          ,n);break;
	    #ifdef BITSHUFFLE
    case 75: TMBENCH("",l=bslzenc(    in,n,out,64/8,tmp,lev), n);  pr(l,n);  TMBENCH2("bitshuffle+lz   ",bslzdec(out,n,cpy,64/8,tmp), n); break;
        #endif
      #endif
    case ID_MEMCPY: if(!mcpy) return l; TMBENCH( "", libmemcpy(  in,out,n) ,n); pr(n,n); TMBENCH2("memcpy         ", libmemcpy( out,cpy,n) ,n); return n;
	default: return l;
  }
  rc = memcheck(in,m*(64/8),cpy);
  if(tmp) free(tmp);
  if(!rc)
    printf("\t%s\n", inname);
  return l;
}

void usage(char *pgm) {
  fprintf(stderr, "\nIcApp Copyright (c) 2013-2018 Powturbo %s\n", __DATE__);
  fprintf(stderr, "Usage: %s [options] [file]\n", pgm);
  fprintf(stderr, " -b#s     # = blocksize (default filesize,). max=1GB\n");
  fprintf(stderr, " -B#s     # = max. benchmark filesize (default 1GB) ex. -B4G\n");
  fprintf(stderr, "          s = modifier s:K,M,G=(1000, 1.000.000, 1.000.000.000) s:k,m,h=(1024,1Mb,1Gb). (default m) ex. 64k or 64K\n");
  fprintf(stderr, "Benchmark:\n");
  fprintf(stderr, " -i#/-j#  # = Minimum  de/compression iterations per run (default=auto)\n");
  fprintf(stderr, " -I#/-J#  # = Number of de/compression runs (default=3)\n");
  fprintf(stderr, "File format:\n");
  fprintf(stderr, " -f[Xx[k][H]][.d][s]\n");
  fprintf(stderr, "    X = file format:\n");
  fprintf(stderr, "        t = text:one integer per line, k=column number in multiple columns line\n");
  fprintf(stderr, "        c = text:integers separated by non digit char\n");
  fprintf(stderr, "    x = entry format\n");
  fprintf(stderr, "        [b=int8], [s=int16], [u=int32(default)], [s=int64], [f:float] [d:double]\n");
  fprintf(stderr, "    .# = decimal digits (default 2). Convert dec. numbers to integers\n");
  fprintf(stderr, "    H  = skip first line(s)\n");  
  fprintf(stderr, " -H = skip first line(s). Only for text files\n");  
  fprintf(stderr, " -K# = #:column number in multiple columns/line. Only for text files\n");  
  fprintf(stderr, " -ks = s:separator(s) (default , ; and TAB) in multiple columns/line. Only for text files\n");  
  fprintf(stderr, " -s# = #:integer size 2,4,8\n");  
  fprintf(stderr, " -f# = #:floating point size 4,8\n");  
  fprintf(stderr, " -t# = #:Timestamp in iso-8601 converted to seconds (32 bits)\n");  
  fprintf(stderr, " -T# = #:Timestamp in iso-8601 converted to milliseconds (64 bits)\n");  
  fprintf(stderr, " -V# = #:divisor. Only for text files\n");  
  fprintf(stderr, " -D# = #:decimals. Only for text files\n");  
  fprintf(stderr, "Output:\n");
  fprintf(stderr, " -v#      # = verbosity 0..5 (default 1). 5=print first values read from text file\n");
  fprintf(stderr, "----- arg. ZIPF specified --------------\n");
  fprintf(stderr, " -aF     F = zipfian distribution alpha ex. -a1.0 uniform -a1.5 skewed\n");
  fprintf(stderr, " -mNs    N = minimum number. s=(k,K, m,M, g,G\n");
  fprintf(stderr, " -MNs    N = maximum number\n");
  fprintf(stderr, " -nNs    N = number of integers to generate\n");
  fprintf(stderr, "Ex. ./icapp -a1.5 -m0 -M255 -n100M ZIPF\n");
  fprintf(stderr, "ex. ./icapp file -e2\n");
  fprintf(stderr, "ex. ./icapp -Ft -D2 ibm.data\n");
  fprintf(stderr, "ex. ./icapp -Ft -f8 usages.dat\n");
  fprintf(stderr, "ex. ./icapp -Ft -f8 -HH tripdata.csv\n");
  exit(0);
} 

int main(int argc, char* argv[]) {
  unsigned cmp=1, b = 1 << 30, lz=0, fno,idmin=1,idmax=-1,m=1000000; int isize=4,dfmt = 0,kid=1,skiph=0,decs=0,divs=1,be_mindelta=0,lev=1;
  unsigned char *in=NULL,*out,*cpy;
  double mdelta=-10;
  int c, digit_optind = 0, this_option_optind = optind ? optind : 1, option_index = 0;
  static struct option long_options[] = { {"blocsize", 	0, 0, 'b'}, {0, 0, 0}  };
  for(;;) {
    if((c = getopt_long(argc, argv, "a:B:cC:e:D:E:f:F:i:j:k:K:HI:J:l:m:M:n:s:v:V:y", long_options, &option_index)) == -1) break;
    switch(c) {
      case  0 : printf("Option %s", long_options[option_index].name); if(optarg) printf (" with arg %s", optarg);  printf ("\n"); break;								
      case 'C': cmp    = atoi(optarg);  break;
      case 'e': idmin  = atoi(optarg); if(idmax == -1) idmax = idmin; break;
      case 'E': idmax  = atoi(optarg);  break;
      case 'D': decs   = atoi(optarg);  break;

      case 'F': { char *s = optarg; 
	         if(*s=='c') { dfmt = T_CHAR; s++;} 
		else if(*s=='t') { dfmt = T_TXT; if(*++s > '0' && *s <= '9') { kid = *s++ - '0'; if(*s > '0' && *s <= '9') kid = kid*10 + (*s++ - '0'); } } 
		else if(*s=='e') { dfmt = T_TST; s++; } 
		else if(*s=='r') { dfmt = 0;     s++; } // default 

		     if(*s=='f') isize = -4, s++; // float : 4 bytes
		else if(*s=='d') isize = -8, s++; // double: 8 bytes
	    else if(*s=='b') isize =  1, s++; // 1 byte 
		else if(*s=='s') isize =  2, s++; // 2 bytes
		else if(*s=='u') isize =  4, s++; // 4 bytes
		else if(*s=='l') isize =  8, s++; // 8 bytes
		else if(*s=='t') isize =  4, s++, dfmt = T_TIM32; // 4 bytes, timestamp
		else if(*s=='T') isize =  8, s++, dfmt = T_TIM64; // 8 bytes, timestamp
		if(*s == '.') { if(*++s >= '0' && *s <= '9') { decs = s[0] - '0'; s++; } } // number of decimals after .
		if(*s == 'v') { divs = strtod(++s, &s); } 
		if(*s == 'H') { skiph++; s++; } // skip first line(s). ex.  HHH : skip 3 first lines
	    switch(*s) { case 's': be_mindelta = 0; break; case 'S': be_mindelta = 1; break; case 'z': be_mindelta = 2; break; }
	  } break;
      
      case 'H': skiph++; 				  	     break;
	  case 'K': { kid = atoi(optarg); if(!keysep) keysep = ",;\t"; } break;
	  case 'k': keysep = optarg; break;
      case 'i': if((tm_rep  = atoi(optarg))<=0) 
		          tm_rep=tm_Rep=1;         	 break;
      case 'I': tm_Rep  = atoi(optarg);      break;
      case 'j': if((tm_rep2  = atoi(optarg))<=0) 
		          tm_rep2=tm_Rep2=1;         break;
      case 'J': tm_Rep2 = atoi(optarg);      break;
      case 's': isize = argtoi(optarg,1); 	 break;
      case 'B': b = argtoi(optarg,1); 	     break;
      case 'c': cmp++; 				  	     break;
      case 'l': lev = atoi(optarg);  	     break;
      case 'y': mcpy=0; 			  	     break;
	  
      case 'a': a = strtod(optarg, NULL);    break;
      case 'd': mdelta  = strtod(optarg, NULL);break;
      case 'n': m       = argtoi(optarg,1);  break;
      case 'm': rm      = argtoi(optarg,1);  break;
      case 'M': rx      = argtoi(optarg,1);  break;
      case 'f': isize   = -argtoi(optarg,1); break;
      case 'v': verbose = atoi(optarg);      break;
      case 'V': divs    = atoi(optarg);  break;
      default: 
        usage(argv[0]);
        exit(0); 
    }
  }
  if(idmax == -1) idmax = ID_MEMCPY;
  if(idmax < idmin) idmax = idmin;
  if(argc - optind < 1) {
    usage(argv[0]);
    exit(0); 
  }																			if(verbose) printf("dfmt=%d,size=%d\n", dfmt, isize);
  for(fno = optind; fno < argc; fno++) {
    char *inname = argv[fno];  							
    int i,n; 
	long long flen;
	FILE *fi=NULL;
	if(!strcmp(inname,"ZIPF") || !strcmp(inname,"TMS")) flen = n = m*abs(isize);
    else {
      fi = fopen(inname, "rb"); 							if(!fi ) { perror(inname); continue; } 							
      if(dfmt) { 
	    n = flen = befgen(&in, 0, dfmt, isize, fi, kid, skiph, decs, divs, be_mindelta);
      } else { 
        fseek(fi, 0, SEEK_END); 
        flen = ftell(fi); 
	    fseek(fi, 0, SEEK_SET);
        if(flen > b) flen = b;
        n = flen; 
      }  
	}  
    if(!in && !(in  = (unsigned char*)malloc(n+64        ))) die("malloc error 'in =%d'\n", n); cpy = in;
    if(!(out =        (unsigned char*)malloc(CBUF(n)+1024))) die("malloc error 'out=%d'\n", n);  
    if(cmp && !(cpy = (unsigned char*)malloc(CBUF(n)+1024))) die("malloc error 'cpy=%d'\n", n); 
	if(fi) {
      if(!dfmt) n = fread(in, 1, n, fi);								 			
      fclose(fi);
	} else if(!strcmp(inname,"TMS") && abs(isize) == 8)
	  tms64(in, m, rm, rx);
	else 
	  datagen(in, m, isize, mdelta);
    if(n <= 0) exit(0); 
    if(fno == optind)
      tm_init(tm_Rep, 1); 
    if(verbose || fno == optind) printf("   E MB/s     size     ratio    D MB/s  function\n");      
    for(i=idmin; i <= idmax; i++) 
      switch(abs(isize)) {
        case 1: bench8( in,n,out,cpy,i,inname,lev); break;  
        case 2: bench16(in,n,out,cpy,i,inname,lev); break;  
        case 4: bench32(in,n,out,cpy,i,inname,lev); break;    
        case 8: bench64(in,n,out,cpy,i,inname,lev); break;    
        default: die("integer size must be 2, 4 or 8\n"); 
      }
    printf("\n"); 
    if(idmax > idmin) printf("\n");     
    free(in); free(out); free(cpy); in = out = cpy = NULL;
  }
}
