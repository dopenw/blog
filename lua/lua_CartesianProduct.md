# 求笛卡尔积

```lua
function CartesianProduct(lst)


  ans={}


function dfs(str,curIndex,alist)
  length=#alist;
  if curIndex >length then
    table.insert(ans,str);
    return
  end
  for _,item in pairs(alist[curIndex]) do
    dfs(str..tostring(item),curIndex+1,alist)
    end
  end
  dfs('',1,lst)
  print(ans)
  for _,v in pairs(ans) do
    print(v)
    end
  return ans
end


CartesianProduct({{1,2},{3,4},{5,6}})
```













































[上一级](base.md)
[下一篇](lua_develop.md)
