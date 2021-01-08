/*  
  FastLZ - lightning-fast lossless compression library

  Copyright (C) 2007 Ariya Hidayat (ariya@kde.org)
  Copyright (C) 2006 Ariya Hidayat (ariya@kde.org)
  Copyright (C) 2005 Ariya Hidayat (ariya@kde.org)

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include <pstdint.h> // Standard Types - Sanguine

#define FASTLZ_EXPECT_CONDITIONAL(c)    (c)
#define FASTLZ_UNEXPECT_CONDITIONAL(c)  (c)

/* Manual Set - Sanguine */
#undef FASTLZ_STRICT_ALIGN


/* prototypes */
int fastlz_compress(const void* input, int length, void* output);
int fastlz_decompress(const void* input, int length, void* output, int maxout);

#define MAX_COPY       32
#define MAX_LEN       264  /* 256 + 8 */
#define MAX_DISTANCE 8192

#define FASTLZ_READU16(p) *((const uint16_t*)(p)) 

#define HASH_LOG  13
#define HASH_SIZE (1<< HASH_LOG)
#define HASH_MASK  (HASH_SIZE-1)
#define HASH_FUNCTION(v,p) { v = FASTLZ_READU16(p); v ^= FASTLZ_READU16(p+1)^(v>>(16-HASH_LOG));v &= HASH_MASK; }

#undef MAX_DISTANCE
#define MAX_DISTANCE 8191
#define MAX_FARDISTANCE (65535+MAX_DISTANCE-1)

int fastlz_compress(const void* input, int length, void* output)
{
  const uint8_t* ip = (const uint8_t*) input;
  const uint8_t* ip_bound = ip + length - 2;
  const uint8_t* ip_limit = ip + length - 12;
  uint8_t* op = (uint8_t*) output;

  const uint8_t* htab[HASH_SIZE];
  const uint8_t** hslot;
  uint32_t hval;

  uint32_t copy;

  /* sanity check */
  if(FASTLZ_UNEXPECT_CONDITIONAL(length < 4))
  {
    if(length)
    {
      /* create literal copy only */
      *op++ = length-1;
      ip_bound++;
      while(ip <= ip_bound)
        *op++ = *ip++;
      return length+1;
    }
    else
      return 0;
  }

  /* initializes hash table */
  for (hslot = htab; hslot < htab + HASH_SIZE; hslot++)
    *hslot = ip;

  /* we start with literal copy */
  copy = 2;
  *op++ = MAX_COPY-1;
  *op++ = *ip++;
  *op++ = *ip++;

  /* main loop */
  while(FASTLZ_EXPECT_CONDITIONAL(ip < ip_limit))
  {
    const uint8_t* ref;
    uint32_t distance;

    /* minimum match length */
    uint32_t len = 3;

    /* comparison starting-point */
    const uint8_t* anchor = ip;

    /* check for a run */
    if(ip[0] == ip[-1] && FASTLZ_READU16(ip-1)==FASTLZ_READU16(ip+1))
    {
      distance = 1;
      ip += 3;
      ref = anchor - 1 + 3;
      goto match;
    }

    /* find potential match */
    HASH_FUNCTION(hval,ip);
    hslot = htab + hval;
    ref = htab[hval];

    /* calculate distance to the match */
    distance = anchor - ref;

    /* update hash table */
    *hslot = anchor;

    /* is this a match? check the first 3 bytes */
    if(distance==0 || 
    (distance >= MAX_FARDISTANCE) ||
    *ref++ != *ip++ || *ref++!=*ip++ || *ref++!=*ip++)
      goto literal;

    /* far, needs at least 5-byte match */
    if(distance >= MAX_DISTANCE)
    {
      if(*ip++ != *ref++ || *ip++!= *ref++) 
        goto literal;
      len += 2;
    }
    
    match:

    /* last matched byte */
    ip = anchor + len;

    /* distance is biased */
    distance--;

    if(!distance)
    {
      /* zero distance means a run */
      uint8_t x = ip[-1];
      while(ip < ip_bound)
        if(*ref++ != x) break; else ip++;
    }
    else
    for(;;)
    {
      /* safe because the outer check against ip limit */
      if(*ref++ != *ip++) break;
      if(*ref++ != *ip++) break;
      if(*ref++ != *ip++) break;
      if(*ref++ != *ip++) break;
      if(*ref++ != *ip++) break;
      if(*ref++ != *ip++) break;
      if(*ref++ != *ip++) break;
      if(*ref++ != *ip++) break;
      while(ip < ip_bound)
        if(*ref++ != *ip++) break;
      break;
    }

    /* if we have copied something, adjust the copy count */
    if(copy)
      /* copy is biased, '0' means 1 byte copy */
      *(op-copy-1) = copy-1;
    else
      /* back, to overwrite the copy count */
      op--;

    /* reset literal counter */
    copy = 0;

    /* length is biased, '1' means a match of 3 bytes */
    ip -= 3;
    len = ip - anchor;

    /* encode the match */
    if(distance < MAX_DISTANCE)
    {
      if(len < 7)
      {
        *op++ = (len << 5) + (distance >> 8);
        *op++ = (distance & 255);
      }
      else
      {
        *op++ = (7 << 5) + (distance >> 8);
        for(len-=7; len >= 255; len-= 255)
          *op++ = 255;
        *op++ = len;
        *op++ = (distance & 255);
      }
    }
    else
    {
      /* far away, but not yet in the another galaxy... */
      if(len < 7)
      {
        distance -= MAX_DISTANCE;
        *op++ = (len << 5) + 31;
        *op++ = 255;
        *op++ = distance >> 8;
        *op++ = distance & 255;
      }
      else
      {
        distance -= MAX_DISTANCE;
        *op++ = (7 << 5) + 31;
        for(len-=7; len >= 255; len-= 255)
          *op++ = 255;
        *op++ = len;
        *op++ = 255;
        *op++ = distance >> 8;
        *op++ = distance & 255;
      }
    }

    /* update the hash at match boundary */
    HASH_FUNCTION(hval,ip);
    htab[hval] = ip++;
    HASH_FUNCTION(hval,ip);
    htab[hval] = ip++;

    /* assuming literal copy */
    *op++ = MAX_COPY-1;

    continue;

    literal:
      *op++ = *anchor++;
      ip = anchor;
      copy++;
      if(FASTLZ_UNEXPECT_CONDITIONAL(copy == MAX_COPY))
      {
        copy = 0;
        *op++ = MAX_COPY-1;
      }
  }

  /* left-over as literal copy */
  ip_bound++;
  while(ip <= ip_bound)
  {
    *op++ = *ip++;
    copy++;
    if(copy == MAX_COPY)
    {
      copy = 0;
      *op++ = MAX_COPY-1;
    }
  }

  /* if we have copied something, adjust the copy length */
  if(copy)
    *(op-copy-1) = copy-1;
  else
    op--;

  /* marker for fastlz2 */
  *(uint8_t*)output |= (1 << 5);
  return op - (uint8_t*)output;
}

int fastlz_decompress(const void* input, int length, void* output, int maxout)
{
  const uint8_t* ip = (const uint8_t*) input;
  const uint8_t* ip_limit  = ip + length;
  uint8_t* op = (uint8_t*) output;
  uint8_t* op_limit = op + maxout;
  uint32_t ctrl = (*ip++) & 31;
  int loop = 1;

  do
  {
    const uint8_t* ref = op;
    uint32_t len = ctrl >> 5;
    uint32_t ofs = (ctrl & 31) << 8;

    if(ctrl >= 32)
    {
      uint8_t code;
      len--;
      ref -= ofs;
      if (len == 7-1)
        do
        {
          code = *ip++;
          len += code;
        } while (code==255);
      code = *ip++;
      ref -= code;

      /* match from 16-bit distance */
      if(FASTLZ_UNEXPECT_CONDITIONAL(code==255))
      if(FASTLZ_EXPECT_CONDITIONAL(ofs==(31 << 8)))
      {
        ofs = (*ip++) << 8;
        ofs += *ip++;
        ref = op - ofs - MAX_DISTANCE;
      }
      
      if(FASTLZ_EXPECT_CONDITIONAL(ip < ip_limit))
        ctrl = *ip++;
      else
        loop = 0;

      if(ref == op)
      {
        /* optimize copy for a run */
        uint8_t b = ref[-1];
        *op++ = b;
        *op++ = b;
        *op++ = b;
        for(; len; --len)
          *op++ = b;
      }
      else
      {
        /* copy from reference */
        ref--;
        *op++ = *ref++;
        *op++ = *ref++;
        *op++ = *ref++;

        for(; len; --len)
          *op++ = *ref++;
      }
    }
    else
    {
      ctrl++;
      *op++ = *ip++; 
      for(--ctrl; ctrl; ctrl--)
        *op++ = *ip++;

      loop = FASTLZ_EXPECT_CONDITIONAL(ip < ip_limit);
      if(loop)
        ctrl = *ip++;
    }
  }
  while(FASTLZ_EXPECT_CONDITIONAL(loop));

  return op - (uint8_t*)output;
}
