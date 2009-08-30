#! /usr/bin/env python
# encoding: UTF-8

import TaskGen
from TaskGen import feature

def edje(self):
	self.default_chmod=0664
	bld_rule = '${EDJE} '+'-id '+self.cwd+' -fd '+self.cwd+' ${SRC} ${TGT}'
	TaskGen.declare_chain(name='edje_cc', rule=bld_rule, ext_in='.edc', ext_out='.edj', reentrant=0, install=None)

def detect(conf):
	conf.find_program('edje_cc', var='EDJE', mandatory=True)

feature('edje')(edje)
