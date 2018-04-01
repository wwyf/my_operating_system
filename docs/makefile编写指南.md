# 一些写makefile的笔记

https://seisman.github.io/how-to-write-makefile/introduction.html


## 变量

变量在声明时需要给予初值，而在使用时，需要给在变量名前加上 $ 符号，但最好用小括号 () 或是大括号 {} 把变量给包括起来。如果你要使用真实的 $ 字符，那么你需要用 $$ 来表示。

## clean
```makefile
.PHONY : clean
clean :
    -rm edit $(objects)
```

rm前的-表示出现了错误也不要理。

>前面说过， .PHONY 表示 clean 是一个“伪目标”。而在 rm 命令前面加了一个小减号的 意思就是，也许某些文件出现问题，但不要管，继续做后面的事。当然， clean 的规则不要放在文件 的开头，不然，这就会变成make的默认目标，相信谁也不愿意这样。不成文的规矩是——“clean从来都是放 在文件的最后”。
