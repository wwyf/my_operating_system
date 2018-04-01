# 要在汇编中实现的函数。

## 输出

```c
/*
设置光标位置
*/
void set_cursor(int row, int column)

/* 
在屏幕指定位置放一个ASCII字符  
INPUT：字符，0-24行号，0-79列号
效果：在屏幕指定位置输出一个字符,默认样式为黑底亮白字0x0F
*/
void put_char(char one_char_ascii, int row, int column);

/*
在指定位置输出一串字符串，使用自动换行。
*/
void put_string(char * string_address, int row, int column);

/*
在指定位置输出数字，使用自动换行
*/
void put_int(int number, int row, int column);
```

## 输入

```c
/*
从I/O中得到一个字符，在光标处显示并作为返回值返回。
*/
void get_char();


/* 
从键盘中读取一个按键，并返回
*/
```

## 内存控制
