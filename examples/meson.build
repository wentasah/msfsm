inc = include_directories('..')

examples = [ 'simple',
	     'events',
	     'elevator',
	     'elevator2',
             'simple-nested',
	     'motor',
	     'states_out',
	   ]

test_sh = find_program('test.sh')

foreach i : examples
  exe = executable(i, '@0@.cpp'.format(i), include_directories: inc)
  test(i, test_sh, args: [ exe ])
endforeach
