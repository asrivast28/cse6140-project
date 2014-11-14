import os

topDir = os.getcwd()

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
           'boost_system',
           ]


srcFiles = [
            'driver.cpp',
            ]

debug = ARGUMENTS.get('DEBUG', 0)

targetName = 'tspsolver'

buildDir = 'build'

if debug in [0, '0']:
    cxxFlags.append('-O3')
    buildDir = 'release'
else:
    cxxFlags.append('-g')
    buildDir = 'debug'
    targetName += '_debug'

env = Environment(CXX = 'g++', CXXFLAGS = cxxFlags, CPPPATH = cppPaths)

buildDir = os.path.join('builds', buildDir)
env.VariantDir(buildDir, '.', duplicate = 0)

srcFiles = [os.path.join(buildDir, f) for f in srcFiles]

env.Program(target = targetName, source = srcFiles, LIBS = cxxLibs)
