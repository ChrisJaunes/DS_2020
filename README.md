# DS_2020

合作文档地址:
https://shimo.im/docs/CGcDQKCrhvkCGvQj

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

但是, 只是添加这个地方是不行的, 你会发现 `未解析的符号` 啥的错误

之后需要在 VS 的`头文件/head file`和`源文件/source file`中`添加现有项`, 把那些需要的文件添加进去, 这样基本就可以了 
















