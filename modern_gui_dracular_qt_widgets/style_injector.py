import xml.etree.ElementTree as ET
import argparse

# 因为qt creator只能读取ui文件的样式，所以这个脚本的作用是当css文件发生变化时，将其内容更新到指定的ui文件
# 这样开发就简便了
if __name__ == '__main__':
    cmd_parser = argparse.ArgumentParser(description="将css文件内容注入qt ui文件")
    cmd_parser.add_argument("style", help="style sheet file")
    cmd_parser.add_argument("ui", help="qt ui file")
    args = cmd_parser.parse_args()

    with open(args.style, "r") as styleFile:
        contents = styleFile.read()

    ui_tree = ET.parse(args.ui)
    rootStyleSheet = ui_tree.find(".//property[@name='styleSheet']")
    if rootStyleSheet is not None:
        # 去掉旧的string元素
        for oldString in rootStyleSheet.iter("string"):
            rootStyleSheet.remove(oldString)

        # 插入新的string元素
        newString = ET.Element("string", notr="true")
        newString.text = contents
        rootStyleSheet.append(newString)

    # 复写ui文件
    ui_tree.write(args.ui)
