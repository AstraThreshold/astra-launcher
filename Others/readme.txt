1. 罗列好需要的汉字
2. 用网站将中文转成unicode 注意不能有回车和空格 https://tool.chinaz.com/tools/unicode.aspx
3. 将unicode字符串复制到str.map中 并将\u替换成,$ 去掉第一个逗号
4. 在str.map的第一行输入32-128,
5. 新建终端，在other目录下运行下面指令
wine bdfconv/bdfconv.exe -v -b 0 -f 1 -M str.map -n u8g2_font_myfont -o u8g2_font_myfont.c wenquanyi_9pt.bdf
