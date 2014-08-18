#coding utf-8

import sys
import os

from scan_dir import scanDir

exclude = [
            
          ]

source_dir = "../../../Source/"
file_dirs = [
            [source_dir + "./Animation/", ".cpp"],
            [source_dir + "./Base/", ".cpp"], 
            [".", ".cpp"]
            ]

def excludePath(file_list, exclude):
    path_list = []

    for path in file_list:
        exc = False
        
        for ex in exclude:
            if path.startswith(ex):
                exc = True
                break

        if not exc:
            path_list.append(path)

    return path_list

def writeList(file_list, out_file):
    for item in file_list:
        out_file.write(item + ' \\')
        out_file.write('\n')

def scanSource(code_dir, ext, out_file):
    out_file.write("LOCAL_SRC_FILES += \\" + "\n")

    file_list = []
    file_list = scanDir(code_dir, ext, file_list)

    file_list = excludePath(file_list, exclude)

    writeList(file_list, out_file)

def scanIncludeDir(include_dir, out_file):
    out_file.write("LOCAL_C_INCLUDES +=\\" + "\n")
    
    out_file.write(include_dir[3:] + "\\" + "\n")

    file_list = []
    file_list = scanDir(include_dir, "dir", file_list)

    new_file_list = []
    for item in file_list:
        path = item[3:] + "/"
        new_file_list.append(path)

    writeList(new_file_list, out_file)

def scanFiles(out_file):

    for file_dir in file_dirs:
        scanSource(file_dir[0], file_dir[1], out_file)
        out_file.write("\n\n")

    # for file_dir in file_dirs:
    #     scanIncludeDir(file_dir, out_file)
    #     out_file.write("\n\n")

def gener():
    template_file = open("Android_template.mk", 'r')
    make_file = open("Android2.mk", 'w')

    for s in template_file:
        if s.startswith("LOCAL_SRC_FILES :="):
            scanFiles(make_file)
        else:
            make_file.write(s)
    
if __name__ == "__main__":
    gener()
