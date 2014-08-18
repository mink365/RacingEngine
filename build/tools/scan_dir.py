#coding: utf-8

import argparse
import os
import sys

def ensureDir(path):
    if not os.path.exists(path):
        os.makedirs(path)

def scanDir(directory, target_ext, path_list):
    file_list = os.listdir(directory)
    for line in file_list:
        file_path = os.path.join(directory, line)
        if line.startswith("."):
            #print(file_path)
            '''if os.path.isdir(file_path):
                os.removedirs(file_path)
            else:
                os.remove(file_path)'''
        elif os.path.isdir(file_path):
            #out.write("\n")
            if target_ext == "dir":
                #print(file_path);
                path_list.append(file_path);
            scanDir(file_path, target_ext, path_list)
        else:
            ext = os.path.splitext(file_path)[-1]
            #relate_path = file_path[prefix_num:].replace("\\", "/")
            relate_path = file_path

            #print("path: " + file_path + " ext: " + ext)
            if ext == target_ext:
               path_list.append(file_path)

    return path_list

def main(argv):
    parser = argparse.ArgumentParser(description="scan assert directory, get texture information")
    parser.add_argument('-i', '--input', help='input assest dir', dest="input", default=".")
    parser.add_argument('-o', '--output', help='out file', dest="output", default="list.java")
    parser.add_argument('-e', '--ext', help='target file ext', dest="ext", default=".cpp")
    
    args = parser.parse_args()

    assets_dir = args.input
    prefix_num = len(assets_dir)
    out_file = args.output
    target_ext = args.ext
    
    #out = open(out_file, 'w')
   
    cpp_path_list = []
    scanDir(assets_dir, target_ext, cpp_path_list)

    for path in cpp_path_list:
        #print(path + " \\")
        print("cardMap[\"" + path + "\"]" + " = true;")
    

if __name__ == "__main__":
    main(sys.argv[1:])
