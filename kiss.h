typedef struct kissstruct {
   unsigned int z;
   unsigned int w;
   unsigned int jsr;
   unsigned int jcong;
   } kissfmt;

#define ZNEW   (kk->z=36969*(kk->z&65535)+(kk->z>>16))
#define WNEW   (kk->w=18000*(kk->w&65535)+(kk->w>>16))
#define MWC    ((ZNEW<<16)+WNEW )
#define SHR3  (kk->jsr^=(kk->jsr<<17), \
kk->jsr^=(kk->jsr>>13), kk->jsr^=(kk->jsr<<5))
#define CONG  (kk->jcong=69069*kk->jcong+1234567)
#define KISS  ((MWC^CONG)+SHR3)

kissfmt *kissinit(void);
unsigned int kiss(kissfmt *kk);
unsigned int kisspwr(kissfmt *kk, int bits);
int kissint(kissfmt *kk, int limit);
double kissunif(kissfmt *kk);
int kissbit(kissfmt *kk);
