project('msfsm', 'cpp',
	default_options: ['cpp_std=c++11'])

executable('semaphore', 'main.cpp')

gtest_dep = dependency('gtest', main : true, required : true)

executable('semaphore2', 'main.cpp', dependencies : gtest_dep)
