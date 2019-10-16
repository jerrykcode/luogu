## 关于 两个标记mul_tag和add_tag

标记的作用: 在更新树结点t的sum之后，用标记记录更新的情况，然后不再继续递归更新其子树的sum。当以后query或update需要递归到t的子树时，
再根据标记计算出子树的正确的sum，继而可以继续正确的递归。  
在[线段树1](https://www.luogu.org/problem/P3372)中，只有一个update，即将一段范围内的每个数增加一个值，更新树结点t的sum时，tag(标记)加上增加的值，
以后需要递归t的子树时，t的子树的sum加上tag\*子树的长度(乘以长度是因为每个数都需要增加)即可。此时t的子树的sum得到了正确的值，而t的子树的标记又要记录，
因为t的子树的子树并没有更新。  
在[线段树2](https://www.luogu.org/problem/P3373)中，可以将一段范围内的每个数增加，也可以将它们乘以一个值，故需要两个标记mul_tag和add_tag
对于树结点t，mul_tag初始化为1，add_tag初始化为0  
当t的子树需要通过t的标记更新时，  
t的子树的sum = (t的子树的sum) \* mul_tag + add_tag \* (t的子树的长度)  
所以  
* 当树结点t的范围内的每个数增加add时，  
  t的子树的sum本应该增加add \* (t的子树的长度)，  
  即 (t的子树的sum) \* mul_tag + add_tag \* (t的子树的长度)  + add \* (t的子树的长度)  
		=  (t的子树的sum) \* mul_tag + (add_tag + add) \* (t的子树的长度)  
  那么t的add_tag应该更新为add_tag + add，这样以后递归t的子树前就可以通过标记计算出子树的正确sum

* 当树结点t的范围内的每个数乘以mul时，  
   t的子树的sum本应该乘以mul，  
   即 ( (t的子树的sum) \* mul_tag + add_tag \* (t的子树的长度) ) \* mul  
			= (t的子树的sum) \* mul_tag \* mul + add_tag \* mul \* (t的子树的长度)
   那么t的add_tag应该更新为mul_tag \* mul，t的add_tag应该更新为add_tag \* mul，这样以后递归t的子树前就可以通过标记计算出子树的正确sum