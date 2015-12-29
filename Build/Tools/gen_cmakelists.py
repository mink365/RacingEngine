#coding: utf-8

import argparse
import os
import sys
from jinja2 import Template

from scan_dir import scan_dir

workspace = ""

def list_files(dir_, remove_dir, base_dir, name=None):
	sources = scan_dir(dir_, ".cpp", [])

	headers = scan_dir(dir_, ".h", [])

	sources = [item.replace(remove_dir+"/", "") for item in sources]
	headers = [item.replace(remove_dir+"/", "") for item in headers]

	if name == None:
		name = os.path.basename(dir_)

	return {"name": name, "name_upper": name.upper(), "dir": base_dir, "sources": sources, "headers": headers}

def list_engine():
	engine_infos = []

	list_ = [
		"Animation",
		"Audio",
		"Base",
		"Effect",
		"FileSystem",
		"Font",
		"Math",
		"Message",
		"Render",
		"Resource",
		"Scene",
		"UI",
		"Util"
	]

	for dir_ in list_:
		info = list_files(os.path.join(workspace, "Source", dir_), os.path.join(workspace, "Source"), "RE_SOURCE_DIR")

		engine_infos.append(info)

	return engine_infos

def list_examples():
	info = list_files(os.path.join(workspace, "Examples/Source"), os.path.join(workspace, "Examples/Source"), "RE_EXAMPLES_DIR", "Examples")

	return [info]

def gen_module(info):

	module_template = \
'''
set({{name_upper}}_SOURCE_FILES \
{% for file in sources %}
        {{'${'~dir~'}'}}/{{file}}\
{% endfor %}
        )

set({{name_upper}}_HEADER_FILES \
{% for file in headers %}
        {{'${'~dir~'}'}}/{{file}} \
{% endfor %}
        )

SOURCE_GROUP("{{name}}\\\Source Files" FILES {{ '${'~name_upper~'_SOURCE_FILES}' }})
SOURCE_GROUP("{{name}}\\\Header Files" FILES {{ '${'~name_upper~'_HEADER_FILES}' }})

'''

	t = Template(module_template)

	return t.render(**info)

'''
	[
		{"name":, "sources":, "headers":}
	]
'''
def gen_cmake(cmake_file, infos, cmake_head, module_type, module_name):
	cmake_template = \
'''
{{ cmake_head }}

{% for result in results %}
	{{ result }} \
{% endfor %}

add_{{ module_type }}({{ module_name }} \
{% for info in infos %}
        {{ '${'~info["name_upper"]~'_SOURCE_FILES}' }}
	    {{ '${'~info["name_upper"]~'_HEADER_FILES}' }} \
{% endfor %}
        )
'''
	t = Template(cmake_template)

	results = []
	for s in infos:
		result = gen_module(s)

		results.append(result)

	name_space = {"infos": infos, "results": results, 
		"cmake_head": cmake_head, "module_name": module_name, "module_type": module_type}

	s = t.render(**name_space)

	make_file = open(cmake_file, 'w')
	make_file.write(s)


def gen_cmakes():
	engine_infos = list_engine()

	examples_infos = list_examples()

	gen_cmake(os.path.join(workspace, "Build/CMake/Core/CMakeLists.txt"), engine_infos, "", "library", "Engine")
	gen_cmake(os.path.join(workspace, "Build/CMake/Examples/CMakeLists.txt"), examples_infos, "", "executable", "Examples")


if __name__ == "__main__":
	parser = argparse.ArgumentParser(description="scan assert directory, get texture information")
	parser.add_argument('-i', '--input', help='input assest dir', dest="input", default=".")

	args = parser.parse_args()

	workspace = args.input

	gen_cmakes()