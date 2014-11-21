import os
import platform

topDir = os.getcwd()

cxx = 'g++'

cppPaths = [
            topDir,
            ]

cxxFlags = [
            '-Wall',
            '-std=c++0x',
            ]

cxxLibs = [
           'boost_filesystem',
           'boost_program_options',
           'boost_timer',
           'boost_chrono',
           'boost_system',
           ]

srcFiles = [
            'ProgramOptions.cpp',
            'TSPInstance.cpp',
            'MSTApproximation.cpp',
            'TreeNode.cpp',
            'BranchAndBound.cpp',
            'GreedyHeuristic.cpp',
            'driver.cpp',
            ]

cppDefs = [
           ]

libPaths = [
            ]

debug = ARGUMENTS.get('DEBUG', 0)

targetName = 'tspsolver'

buildDir = 'build'

otherTools = None


if platform.system() == 'Windows':
    cxx = 'C:\\MinGW\\bin\\g++.exe'
    cppPaths.append("C:\\Boost\\include\\boost-1_57")
    libPaths.append("C:\\Boost\\lib")
    cxxLibs = [lib + '-mgw48-mt-1_57' for lib in cxxLibs]
    otherTools = ['mingw']

if debug in [0, '0']:
    cxxFlags.append('-O3')
    cppDefs.append('NDEBUG')
    buildDir = 'release'
else:
    cxxFlags.append('-g')
    buildDir = 'debug'
    targetName += '_debug'

env = Environment(CXX = cxx, CXXFLAGS = cxxFlags, CPPPATH = cppPaths, CPPDEFINES = cppDefs, LIBPATH = libPaths, tools = otherTools)

buildDir = os.path.join('builds', buildDir)
env.VariantDir(buildDir, '.', duplicate = 0)

srcFiles = [os.path.join(buildDir, f) for f in srcFiles]

env.Program(target = targetName, source = srcFiles, LIBS = cxxLibs)
