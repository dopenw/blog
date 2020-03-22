# 翻译小工具

## 运行要求

### linux
```sh
  sudo dnf install python3-tkinter
  sudo dnf install xclip xsel
  pip install googletrans --user
  pip install pyperclip --user
```

## 源码
```python
#! /usr/bin/python3

from tkinter import *
from tkinter.ttk import Style,Label,Combobox
from googletrans import Translator
import pyperclip


def translate_window(arg):
    translate_text = arg
    root=Tk()
    root.title("googletrans tools")

    def _swap_function():
        box1_value=box1.current()
        box2_value=box2.current()
        if box1_value!=box2_value:
            box1.current(box2_value)
            box2.current(box1_value)

    def _sync_function():
        str1=text1.get("1.0",END)
        translations=translator.translate(str1,dest=combobox_item[box2.current()])
        text2.delete("1.0",END)
        text2.insert("1.0",translations.text)

    label1=Label(root,text="source_text")
    label1.grid(row=0,column=0)
    button1=Button(root,text="swap",command=_swap_function)
    button1.grid(row=1,column=1)
    label2=Label(root,text="dest_text")
    label2.grid(row=0,column=2)

    combobox_item=('en','zh-CN')
    box1=Combobox(root,values=combobox_item)
    box1.grid(row=1,column=0)
    box1.current(0)
    button2=Button(root,height=1, width=2,text="sync",command=_sync_function)
    button2.grid(row=2,column=1)
    box2=Combobox(root,values=combobox_item)
    box2.grid(row=1,column=2)
    box2.current(1)

    text1=Text(root, font=("Comic Sans MS", 12),width=50, height=20)
    text1.grid(row=2,column=0)
    text1.insert(INSERT,translate_text)
    text2=Text(root, font=("Comic Sans MS", 12),width=50, height=20)
    text2.grid(row=2,column=2)

    translator = Translator()
    translations=translator.translate(translate_text,dest='zh-CN')
    text2.insert("1.0",translations.text)

    root.mainloop()

def show_image():

    def center(main_window):
        main_window.update_idletasks()
        #获取屏幕的高度和宽度
        w = main_window.winfo_screenwidth()
        h = main_window.winfo_screenheight()
        size = tuple(int(_) for _ in main_window.geometry().split('+')[0].split('x'))
        x = w/2  + size[0]/2
        print(x)
        y =  size[1]/2
        print(y)
        print("size:",size)
        #resize
        size=(50,50)
        main_window.geometry("%dx%d+%d+%d" % (size + (x, y)))

    def key(event):
        print ("pressed", repr(event.char))

    def callback(event):
        print ("clicked at", event.x, event.y)
        translate_window(pyperclip.paste())


    def dragwin(event):
        x = root.winfo_pointerx() - root._offsetx
        y = root.winfo_pointery() - root._offsety
        root.geometry('+{x}+{y}'.format(x=x,y=y))

    def clickwin(event):
        root._offsetx = event.x
        root._offsety = event.y

    root = Tk()
    #hide窗口标题栏
    root.overrideredirect(1)
    center(root)
    canvas= Canvas(root, width=50, height=50)
    # mouse 左键
    canvas.bind("<Button-1>", callback)
    # mouse 右键，移动窗口
    canvas.bind('<Button-3>',clickwin)
    canvas.bind('<B3-Motion>',dragwin)


    photo = PhotoImage(file = './google_translate.png')
    canvas.create_image(25,25,image=photo)
    canvas.pack()

    root.after(3000,lambda:root.destroy())

    root.mainloop()



if __name__=="__main__":
    pyperclip_source_value=''
    while True:
        pyperclip_current_value=pyperclip.paste()
        if pyperclip_current_value!=pyperclip_source_value:
            show_image()
            pyperclip_source_value=pyperclip_current_value

```
[上一级](README.md)
[上一篇](python_care.md)
