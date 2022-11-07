// #include <stdint.h>
// #include "test_util.h"

// static int m_z = 362436069;
// static int m_w = 521288629;

// int GetUint(){
//   m_z = 36969 * (m_z & 65535) + (m_z >> 16);
//   m_w = 18000 * (m_w & 65535) + (m_w >> 16);
//   return (m_z << 16) + m_w;
// }

// int GetUniform(int max){
//   int64_t u = GetUint();
//   return (u + 1.0) * 2.328306435454494e-10 * max;
// }

// int memcheck(void *start, int value, int size){
//   int *p = (int *) start;
//   int i;

//   for (i = 0; i < size; i++, p++)
//     if (*p != value)
//       return 0;

//   return 1;
// }

// void *memoryset(void *b, int c, int len)
// {
//       unsigned char *p = b;
//       while (len > 0)
//       {
//             *p = c;
//             p++;
//             len--;
//       }
//       return (b);
// }