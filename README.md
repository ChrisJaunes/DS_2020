# DS_2020

合作文档地址:
https://shimo.im/docs/CGcDQKCrhvkCGvQj

### 关于测试环节
@ChrisJaunes

GUI目录下新添加了DS_console、DS_gui、DS_test三个目录, 按照测试进度可以渐渐移入

DS_test用于管理测试文件和测试程序, test.h 和test.cpp可以作为参考

测试程序应该放在DS_test, 为程序编写的测试函数应该导入test.cpp, 然后在test.cpp的tmain里面调用

作为参考程序的对拍程序和数据生成器按照需要选择时候导入项目

导入项目的test文件请利用宏TEST_DEBUG

```cpp
#ifndef TEST_DEBUG
    //测试代码
#endif
```
test.h禁止放到其他.h文件里，容易出现问题

DS_test目录下的readme.md按照实际情况填写

**时间紧迫，大家加油**

### F3: parseInfo大致的用时和内存消耗

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

数据库导出到了database中, 并且提供了F3Solver接口对数据进行导入.

## 一些常见问题的解答

### Spectre的问题

可能出现在 `VS2019`

解决方案: 关闭缓解策略库, `项目 右键 -> c/c++ -> 代码生成 -> Spectre缓解` 设置为禁用

### console 版本项目`全红`

大概是平台工具集没调整到你使用的VS的版本, 可能出现在 `VS2017使用VS2019项目` 时

解决方案: `项目 右键 -> 平台工具集`, 修改工具集, VS2019对应v142, vs2017对应v141

### Qt项目`全红`

因为这里是`Qt项目`和`console的项目`共用**`console项目`**的代码, 所以在使用Qt项目的时候需要修改一些地方. 还有VS-Qt插件在取名的时候每个人的有差异...(捂脸)

解决方案: 

1. 先解决插件取名有差异的问题 :)
`项目 右键 -> Qt Project Settings -> Qt Installation` 这里修改为自己取的名字.
(打开后会自动补全)

2. 解决代码共用的问题
`项目 右键 -> vc++目录 --> 包含目录` 这个项添加console文件的路径

GUI目录下新添加了DS_console、DS_gui、DS_test三个目录, `项目 右键 -> vc++目录 --> 包含目录` 这个项添加这三个目录的路径，建议使用相对路径，相对路径相对于DS_2020_GUI.vcxproj

把工作目录加进去,也就是加入 "."这个文件夹

但是, 只是添加这个地方是不行的, 你会发现 `未解析的符号` 啥的错误

之后需要在 VS 的`头文件/head file`和`源文件/source file`中`添加现有项`, 把那些需要的文件添加进去, 这样基本就可以了 


起始 64 byte
| 偏移(byte) | 大小(byte) |内容 |
|---|---|---|
|0	|	|固定内容，LYCBP，魔数（magic number）
|5	|1	|sizeof(size_t)，每个节点预分配块大小，对应_block_size
|6	|1	|sizoef(size_t), 文件末尾位置
|7	|1	|sizoef(size_t), 根节点的文件偏移值
|8	|4	|sizoef(unsigned int)，B树的树高
|12	|4	|sizeof(unsigned int)，B树的节点数
|16	|4	|sizeof(size_t), B树的叶子链表起始指针
|20	|4	|sizeof(size_t), 文件空的块位置
|











