#!/usr/local/bin/python3

import re
import sys
import getopt

#TAG = "_CC_"
TAG = "_"
STAG = '_' + TAG
SYSCALL_PREFIX = "SW_SYSCALL" + TAG
SYSCALL_BASE = 0x1000
SYSCALL_INTERVAL = 4
FUNC_REG = r'[\w*]+\s+(\w+)'
PERMISSION_GROUP = 'RESERVED_GROUP_PERMISSION'

def stripComment(line):
  pattern = r'(/\*.*\*/)'
  regexp = re.compile(pattern)
  m = regexp.search(line)
  if m:
    comment = m.group(1)
    line = line.replace(comment, '')

  return line

def shorten(filename, outfile):
  with open(filename, 'r') as rf:
    with open(outfile, 'w') as wf:
      for line in rf.readlines():
        if not line.split():
          continue
        if line.startswith('//'):
          continue

        line = stripComment(line)
        pl = line.strip()

        if pl.endswith(','):
          wf.write(pl)
        elif pl.endswith('('):
          wf.write(pl)
        else:
          wf.write(line)

def findGroup(line):
  regexp = re.compile(PERMISSION_GROUP)
  m = re.search(regexp, line)
  ret = m.group(0)
  return ret
def findFunc(line):
  regexp = re.compile(FUNC_REG)
  m = re.search(regexp, line)
  ret = m.group(1)
  return ret

def subFunc(line):
  regexp = re.compile(FUNC_REG)
  m = re.search(regexp, line)
  ret = m.group(1)

  line = re.sub(ret, STAG + ret, line, count = 1)
  return line


def genAsmCall(filename):
  with open(filename, 'r') as f:
    lines = f.readlines()
    for line in lines:
      if line.startswith('/'): continue
      line = line.rstrip('\n')
      func_name = findFunc(line)
      print("    .global %s%s" %(STAG, func_name))

    for line in lines:
      if line.startswith('/'): continue
      line = line.rstrip('\n')
      func_name = findFunc(line)

      print("""
%s%s:
    push {fp, lr}
    add fp, sp, #4
    swi %s%s
    sub fp, sp, #4
    pop {fp, lr}
    bx lr""" %(STAG, func_name, SYSCALL_PREFIX,func_name.upper()))

def genIdHeader(filename):
  with open(filename, 'r') as f:
    lines = f.readlines()
    for index,line in enumerate(lines):
      if line.startswith('/'): continue
      line = line.rstrip('\n')
      func_name = findFunc(line)
      print("#define %s%s\t\t0x%x" %(SYSCALL_PREFIX, func_name.upper(), SYSCALL_BASE + index*SYSCALL_INTERVAL))


def genHFile(filename):
  with open(filename, 'r') as f:
    lines = f.readlines()
    for index,line in enumerate(lines):
      if line.startswith('/'): continue
      line = line.rstrip('\n')
      linep = subFunc(line)
      print("extern %s" %(linep))


def isVoid(line):
  regexp = re.compile(r'([\w*]+)\s+(\w+)')
  m = re.search(regexp, line)
  ret = m.group(1).lower()
  if ret == 'void':
    return True
  else: return False

def argNum(line):
  regexp = re.compile(r',')
  m = re.findall(regexp, line)
  num = len(m) + 1

  regexp = re.compile(r'[\w*]+\s+\w+\s*\((.*)\)')
  m = re.search(regexp, line)
  if m:
    arg = m.group(1)
    arg = arg.strip()
  else:
    arg = ''

  if num == 1:
    #may be this arg is void/VOID
    if arg.lower() == 'void':
      num -= 1

  arglist = arg.split(',')
  new_arglist = []
  regexp = r'(.*\b)\w+'
  for item in arglist:
    item = item.strip()
    m = re.search(regexp, item)
    item = m.group(1)
    item = ' '.join(item.split())

    new_arglist.append(item)

  return num, new_arglist



def genArgs(num, arglist):
  s = '('
  for i in range(num):
    if i: s+=', '
    if i < 4:
      s += "(" + arglist[i] + ")regs->r" + str(i)
    else:
      s += "(" + arglist[i] + ")args[" + str(i - 4) + "]"

  s += ');'
  return s

def genCFile(filename):
  with open(filename, 'r') as f:
    lines = f.readlines()
    for index,line in enumerate(lines):
      if line.startswith('/'): continue
      line = line.rstrip('\n')
      func_name = findFunc(line)
      is_void = isVoid(line)
      argnum, arglist = argNum(line)
#      print("argnum={}".format(argnum))
      args = genArgs(argnum, arglist)

      if is_void:
        print("""
        SYSCALL_PERMISSION(%s%s, ullPermissions, %s)
            %s%s
            SYSCALL_END;""" %(SYSCALL_PREFIX, func_name.upper(), PERMISSION_GROUP, func_name, args))
      else:
        print("""
        SYSCALL_PERMISSION(%s%s, ullPermissions, %s)
            uwRet = %s%s
            regs->r0 = uwRet;
            SYSCALL_END;""" %(SYSCALL_PREFIX, func_name.upper(), PERMISSION_GROUP, func_name, args))

def usage():
  print("""
   Usage: syscallgen.py <function_header_file>
""")

def version():
  print("""
   Version: syscallgen.py 0.1
   Author: h00206996
""")

if __name__ == '__main__':

  opts, args = getopt.gnu_getopt(sys.argv[1:], 'hv')

  for o in opts:
    if o[0] == '-h':
      usage()
      sys.exit(0)
    elif o[0] == '-v':
      version()
      sys.exit(0)

  if len(args) <1:
    usage()
    sys.exit(-1)

  funclist_filename = args[0]

  tempfile = 'temp'
  shorten(funclist_filename, tempfile)
  print("\n\nGen sre_syscalls.S:\n")
  genAsmCall(tempfile)

  print("\n\nGen sre_syscalls_id.h:\n")
  genIdHeader(tempfile)

  print("\n\nGen sre_syscallDispatch.c:\n")
  genCFile(tempfile)

  print("\n\nGen sre_syscall.h:\n")
  genHFile(tempfile)
