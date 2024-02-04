#ifndef _ERROR_H_
#define _ERROR_H_

#include<error.h>
#include<stdio.h>
#include<assert.h>
namespace fei
{
    
void static perror_if(bool on, char* str)
{
    if(on) 
    {
        perror(str);
    }
}


} // namespace fei




#endif
