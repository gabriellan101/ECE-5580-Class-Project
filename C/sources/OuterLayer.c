#include "../includes/OuterLayer.h"

/*
Steps in outer layer key generation:
1. Compute key generation, returns h, f, ginv
2. encode h into public key
3. Encode f and ginv into secret key
Steps 2 and 3 represent "encode K as a string K' in the public key space"
and "encode k as a string k' in the secret key space"
The encode functions were not in the scope of the project and were copied from the reference implementation 
for consistency between our implementation and the reference 
*/
static void OuterKeyGen(unsigned char *pk, unsigned char *sk){
Fq h[P];
F3 f[P], ginv[P];

keyGen(h, f, ginv);
encodeRq(pk, h);
encodeR3(sk, f);
sk += Small_bytes; // this line bumps the pointer of the secret key to the starting point of where ginv should be
/*
sk -> [f encoded (191 bytes)][ginv encoded (191 bytes)]
                            ^pointer needs to be here to not overwrite f encoding
*/
encodeR3(sk, ginv);
// sk does not need to be decremented after this since the code calling this function still has the original pointer value
}


/*
This function is a wrapper on OuterKeyGen that adds hash function usage - specifically hash prefix
This function is also the one that gets called for real usage, it is a wrapper on OuterKeyGen
Full secret key layout after this function:
sk[ 0 .. 381  ]  → Small_encode(f)[191] ; Small_encode(ginv) [191]  (OuterKeyGen output)
sk[ 382 .. 1539]  → copy of pk                          (for decap verification)
sk[1540 .. 1571]  → random seed  (rho)                  (implicit rejection)
sk[1572 .. ?  ]  → Hash_prefix(pk)                      (cached hash)
*/
static void KEM_KeyGen(unsigned char *pk, unsigned char *sk) {
  OuterKeyGen(sk, pk);
  sk += SK_bytes; // increment sk pointer to start of next segment
  memcpy(sk, pk, PK_bytes);
  sk += PK_bytes;
  randombytes(sk, Inputs_bytes);
  sk += Inputs_bytes;
  Hash_Prefix(sk,4,pk,PK_bytes);
}

/*
This function is a wrapper on the core encryption function that takes the output of KEM_KeyGen()
and adds encoding and ciphertext generation
*/
static void OuterEncrypt(unsigned char *CT, const F3 *r, const unsigned char *pk){
  Fq h[P], ct[P];
  decodeRq(h, pk);
  Encrypt(ct, r, h);
  encode_rounded(CT, ct);
}

/*
Outer level decryption steps:
1: decode each segment of the secret key
2: decode into a rounded polynomial
3: Core decryption function
*/
static void OuterDecrypt(F3 *r, const unsigned char *CT, const unsigned char *sk){
  F3 f[P], ginv[P];
  Fq ct[P];

  decodeR3(f, sk);
  sk += Small_bytes;
  decodeR3(ginv, sk);
  decode_rounded(ct, CT);
  Decrypt(r, ct, f, ginv);
}

static void Encap(unsigned char *CT, unsigned char *k, const unsigned char *pk){
  F3 r[P];
  unsigned char r_prime[Inputs_bytes];
  unsigned char cache[Hash_bytes];

  Hash_prefix(cache,4,pk,PK_bytes);
  Short_random(r);

  encodeR3(r_prime, r);
  OuterEncrypt(CT,r,pk); 
  CT += Ciphertexts_bytes; // adjust pointer value to next segment
  HashConfirm(CT,r_prime,pk,cache);

  HashSession(k,1,r_prime,CT);
}

/* ----- encoding rounded polynomials */
// These encoding algorithms are from the reference implementation

static void Rounded_encode(unsigned char *s,const Fq *r)
{
  uint16_t R[P],M[P];
  int i;

  for (i = 0;i < P;++i) R[i] = ((r[i]+adj)*10923)>>15;
  for (i = 0;i < P;++i) M[i] = (Q+2)/3;
  Encode(s,R,M,P);
}

static void Rounded_decode(Fq *r,const unsigned char *s)
{
  uint16_t R[P],M[P];
  int i;

  for (i = 0;i < P;++i) M[i] = (Q+2)/3;
  Decode(R,s,M,P);
  for (i = 0;i < P;++i) r[i] = R[i]*3-adj;
}

/*
Decapsulation steps:
1: desegment the public key, rho, and hash prefix from the KEM_KeyGen() output
2: Generate new ciphertext for comparison
3: Decrypt
4: encode decrypted r into r_prime
5: generate a new ciphertext for comparison with hashing
6: check if they match
7: if they don't match, replace r_prime with rho
8: hash the output to produce the session key k
*/
static void Decap(unsigned char *k, const unsigned char *CT, const unsigned char *sk){
  const unsigned char *pk = sk + SK_bytes; // starting pointer to encapsulated public key
  const unsigned char *rho = pk + PK_bytes; // Pointer to random seed used for implicit rejection for confirmation step
  const unsigned char *cache = rho + Inputs_bytes; // pointer to hash prefix section of data

  F3 r[P];
  unsigned char r_prime[Inputs_bytes];
  unsigned char ctnew[Ciphertexts_bytes+Confirm_bytes];

  OuterDecrypt(r, CT, sk);

  encodeR3(r_prime, r);
  OuterEncrypt(ctnew,r,pk); 
  CT += Ciphertexts_bytes; // adjust pointer value to next segment
  HashConfirm(ctnew,r_prime,pk,cache);

  int matching = confirm(CT, ctnew);

  for(int i = 0; i < Inputs_bytes; i++){
    if(matching == -1) r_prime[i] = rho[i];
  }

  HashSession(k, 1+matching, r_prime, CT);
}

static int confirm(const unsigned char *ct, const unsigned char *ct2){
  
  for(int i = 0; i < (Ciphertexts_bytes+Confirm_bytes); i++){
    if (ct[i] != ct2[i]) return -1;
  }
  return 0;
}


/*
Below are the hash functions from the reference implementation. These were kept unchanged to allow
consistency for KAT usage
*/

int crypto_hash_sha512(unsigned char *out,const unsigned char *in,unsigned long long inlen)
{
  SHA512(in,inlen,out);
  return 0;
}

// This function had to be changed for variable initialization, added malloc() and free()
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