#include "../includes/OuterLayer.h"



int crypto_hash_sha512(unsigned char *out,const unsigned char *in,unsigned long long inlen)
{
  SHA512(in,inlen,out);
  return 0;
}

static void Hash_prefix(unsigned char *out,int b,const unsigned char *in,int inlen)
{
    unsigned char *x;
    unsigned char h[64];
    int i;

    x = (unsigned char *)malloc(inlen + 1);
    if (!x) return; //allocation failure handling

    x[0] = b;
    for (i = 0;i < inlen;++i) x[i+1] = in[i];
    crypto_hash_sha512(h,x,inlen+1);
    for (i = 0;i < 32;++i) out[i] = h[i];

    free(x);
}

static void HashConfirm(unsigned char *h,const unsigned char *r,const unsigned char *pk,const unsigned char *cache)
{
  unsigned char x[Hash_bytes*2];
  int i;

  Hash_prefix(x,3,r,Inputs_bytes);
  for (i = 0;i < Hash_bytes;++i) x[Hash_bytes+i] = cache[i];

  Hash_prefix(h,2,x,sizeof x);
}

static void HashSession(unsigned char *k,int b,const unsigned char *y,const unsigned char *z)
{
  unsigned char x[Hash_bytes+Ciphertexts_bytes+Confirm_bytes];
  int i;

  Hash_prefix(x,3,y,Inputs_bytes);
  for (i = 0;i < Ciphertexts_bytes+Confirm_bytes;++i) x[Hash_bytes+i] = z[i];

  Hash_prefix(k,b,x,sizeof x);
}