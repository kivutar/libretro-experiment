#include "strl.h"

size_t strlcpy(char *dest, const char *source, size_t size)
{
   size_t src_size = 0;
   size_t n = size;

   if (n) 
      while (--n && (*dest++ = *source++)) src_size++;

   if (!n)
   {   
      if (size) *dest = '\0';
      while (*source++) src_size++;
   }   

   return src_size;
}

size_t strlcat(char *dest, const char *source, size_t size)
{
   size_t len = strlen(dest);
   dest += len;

   if (len > size)
      size = 0;
   else
      size -= len;

   return len + strlcpy(dest, source, size);
}
