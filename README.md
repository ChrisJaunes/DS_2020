# DS_2020

合作文档地址:
https://shimo.im/docs/CGcDQKCrhvkCGvQj

### parseInfo大致的用时和内存消耗

@Q4n:

可以从这里大致推断各个类别的数据量. 这个地方的数据是用dom解析的, 但是我们现在的解析是sax, 并且这里只是解析为Info结构的时间, 所以`时间`和`内存`仅供参考.

btw, 因为我这的电脑C盘空间不足, 所以无法完成所有的解析...

```
article  273.687s
book   39.078s
incollection  42.297s
inproceedings 294.891s 28G
mastersthesis 36.062s
phdthesis 47.75s
proceedings  41.547s
www 132.031s 16G
```
