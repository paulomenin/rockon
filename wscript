VERSION='0.0.1'
APPNAME='rockon'

srcdir = 'src'
blddir = 'build'

def init():
	pass

def set_options(opt):
	pass

def configure(conf):
	conf.sub_config('src')

def build(bld):
	bld.add_subdirs('src')

def shutdown():
	pass

