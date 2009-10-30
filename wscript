#!/usr/bin/env python
# encoding: utf-8

VERSION='0.0.1'
APPNAME='rockon'

srcdir = '.'
blddir = 'build'

def init():
	pass

def set_options(opt):
	opt.add_option('--doc', action='store_true', default=False,
                   help='build the program documentation using doxygen')

def configure(conf):
	conf.sub_config('src')
	conf.sub_config('themes')
	#conf.check_tool('doxygen', tooldir='waf_tools')

def build(bld):
	import Options
	
	bld.add_subdirs('src')
	bld.add_subdirs('themes')

	#if Options.options.doc:
	#	bld.new_task_gen(
	#		features = 'doxygen',
	#		doxyfile = 'Doxyfile',
	#		install_to = None
	#	)

def shutdown():
	pass

