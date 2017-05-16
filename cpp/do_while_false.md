# do_while_false的功用

```cpp
if( A==true )  
{  
    if( B==true )  
    {  
        if( C==true )  
        {  
            if( D==true )  
            {  
                //code
            }  
        }  
    }  
}  
```
等价于
```cpp
do  
{  
    if( A==false )  
        break;  
    if( B==false )  
        break;  
    if( C==false )  
        break;  
    if( D==false )  
        break;  
    //code
}while(false);
```
