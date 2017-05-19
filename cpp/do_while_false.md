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

[上一级](base.md)
[上一篇](conv_string_to_char_pointer.md)
[下一篇](regex.md)
