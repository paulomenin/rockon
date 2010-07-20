# encoding: utf-8

VERSION='0.0.1'
APPNAME='rockon'

srcdir = '.'
blddir = 'build'

def init():
	pass

def set_options(opt):
	pass

def configure(conf):
	conf.sub_config('src')
	conf.sub_config('themes')

def build(bld):
	import Options
	
	bld.add_subdirs('src')
	bld.add_subdirs('themes')

def shutdown():
	pass

