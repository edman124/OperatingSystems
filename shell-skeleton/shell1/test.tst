python3 shell_test.py
Failed test trivial command, exit status 0
Test input:
  /bin/true
  exit
Actual stdout:
  > 
Actual stderr:
  ASAN:DEADLYSIGNAL
  =================================================================
  ==10509==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x563dc75ab78b bp 0x7fff26861960 sp 0x7fff26860f60 T0)
  ==10509==The signal is caused by a READ memory access.
  ==10509==Hint: address points to the zero page.
  [plus 9 more lines, not shown]
Expected stdout regular expression pattern:
  > .*/bin/true.*[Ee]xit status: 0.*
  > 
Expected stderr regular expression pattern:
  <empty>
Errors:
  in stdout: could not find a match for pattern [> .*/bin/true.*[Ee]xit status: 0.*] in line [> ]
  in stdout: could not find match for pattern [> .*/bin/true.*[Ee]xit status: 0.*] in ['> ']
  in stderr: unexpected extra output [['ASAN:DEADLYSIGNAL', '=================================================================', '==10509==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x563dc75ab78b bp 0x7fff26861960 sp 0x7fff26860f60 T0)', '==10509==The signal is caused by a READ memory access.', '==10509==Hint: address points to the zero page.', '    #0 0x563dc75ab78a in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/student/shell-skeleton/shell/main.cc:188', '    #1 0x563dc75a69b5 in main /home/student/shell-skeleton/shell/main.cc:322', '    #2 0x7f85d4e95b96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)', '    #3 0x563dc75a6cc9 in _start (/home/student/shell-skeleton/shell/msh+0x2cc9)', '', 'AddressSanitizer can not provide additional info.', 'SUMMARY: AddressSanitizer: SEGV /home/student/shell-skeleton/shell/main.cc:188 in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)', '==10509==ABORTING']]
Failed test trivial command, exit status 0 (do not check for command name)
Test input:
  /bin/true
  exit
Actual stdout:
  > 
Actual stderr:
  ASAN:DEADLYSIGNAL
  =================================================================
  ==10510==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x55f74020178b bp 0x7ffcc4dffbb0 sp 0x7ffcc4dff1b0 T0)
  ==10510==The signal is caused by a READ memory access.
  ==10510==Hint: address points to the zero page.
  [plus 9 more lines, not shown]
Expected stdout regular expression pattern:
  > .*[Ee]xit status: 0.*
  > 
Expected stderr regular expression pattern:
  <empty>
Errors:
  in stdout: could not find a match for pattern [> .*[Ee]xit status: 0.*] in line [> ]
  in stdout: could not find match for pattern [> .*[Ee]xit status: 0.*] in ['> ']
  in stderr: unexpected extra output [['ASAN:DEADLYSIGNAL', '=================================================================', '==10510==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x55f74020178b bp 0x7ffcc4dffbb0 sp 0x7ffcc4dff1b0 T0)', '==10510==The signal is caused by a READ memory access.', '==10510==Hint: address points to the zero page.', '    #0 0x55f74020178a in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/student/shell-skeleton/shell/main.cc:188', '    #1 0x55f7401fc9b5 in main /home/student/shell-skeleton/shell/main.cc:322', '    #2 0x7f8f64fefb96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)', '    #3 0x55f7401fccc9 in _start (/home/student/shell-skeleton/shell/msh+0x2cc9)', '', 'AddressSanitizer can not provide additional info.', 'SUMMARY: AddressSanitizer: SEGV /home/student/shell-skeleton/shell/main.cc:188 in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)', '==10510==ABORTING']]
Failed test trivial command with extra space, exit status 0
Test input:
   /bin/true
  exit
Actual stdout:
  > 
Actual stderr:
  ASAN:DEADLYSIGNAL
  =================================================================
  ==10511==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x556796c7a78b bp 0x7fffb2a0d910 sp 0x7fffb2a0cf10 T0)
  ==10511==The signal is caused by a READ memory access.
  ==10511==Hint: address points to the zero page.
  [plus 9 more lines, not shown]
Expected stdout regular expression pattern:
  > .*/bin/true.*[Ee]xit status: 0.*
  > 
Expected stderr regular expression pattern:
  <empty>
Errors:
  in stdout: could not find a match for pattern [> .*/bin/true.*[Ee]xit status: 0.*] in line [> ]
  in stdout: could not find match for pattern [> .*/bin/true.*[Ee]xit status: 0.*] in ['> ']
  in stderr: unexpected extra output [['ASAN:DEADLYSIGNAL', '=================================================================', '==10511==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x556796c7a78b bp 0x7fffb2a0d910 sp 0x7fffb2a0cf10 T0)', '==10511==The signal is caused by a READ memory access.', '==10511==Hint: address points to the zero page.', '    #0 0x556796c7a78a in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/student/shell-skeleton/shell/main.cc:188', '    #1 0x556796c759b5 in main /home/student/shell-skeleton/shell/main.cc:322', '    #2 0x7f877852bb96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)', '    #3 0x556796c75cc9 in _start (/home/student/shell-skeleton/shell/msh+0x2cc9)', '', 'AddressSanitizer can not provide additional info.', 'SUMMARY: AddressSanitizer: SEGV /home/student/shell-skeleton/shell/main.cc:188 in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)', '==10511==ABORTING']]
Failed test trivial command with extra tab, exit status 0
Test input:
  	/bin/true
  exit
Actual stdout:
  > 
Actual stderr:
  ASAN:DEADLYSIGNAL
  =================================================================
  ==10512==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x557d944cc78b bp 0x7ffc82972680 sp 0x7ffc82971c80 T0)
  ==10512==The signal is caused by a READ memory access.
  ==10512==Hint: address points to the zero page.
  [plus 9 more lines, not shown]
Expected stdout regular expression pattern:
  > .*[Ee]xit status: 0.*
  > 
Expected stderr regular expression pattern:
  <empty>
Errors:
  in stdout: could not find a match for pattern [> .*[Ee]xit status: 0.*] in line [> ]
  in stdout: could not find match for pattern [> .*[Ee]xit status: 0.*] in ['> ']
  in stderr: unexpected extra output [['ASAN:DEADLYSIGNAL', '=================================================================', '==10512==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x557d944cc78b bp 0x7ffc82972680 sp 0x7ffc82971c80 T0)', '==10512==The signal is caused by a READ memory access.', '==10512==Hint: address points to the zero page.', '    #0 0x557d944cc78a in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/student/shell-skeleton/shell/main.cc:188', '    #1 0x557d944c79b5 in main /home/student/shell-skeleton/shell/main.cc:322', '    #2 0x7fbfeb6b7b96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)', '    #3 0x557d944c7cc9 in _start (/home/student/shell-skeleton/shell/msh+0x2cc9)', '', 'AddressSanitizer can not provide additional info.', 'SUMMARY: AddressSanitizer: SEGV /home/student/shell-skeleton/shell/main.cc:188 in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)', '==10512==ABORTING']]
Failed test trivial command with extra vertical tab, exit status 0
Test input:
  /bin/true
  exit
Actual stdout:
  > 
Actual stderr:
  ASAN:DEADLYSIGNAL
  =================================================================
  ==10513==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x55b423b3a78b bp 0x7fff9dfb7100 sp 0x7fff9dfb6700 T0)
  ==10513==The signal is caused by a READ memory access.
  ==10513==Hint: address points to the zero page.
  [plus 9 more lines, not shown]
Expected stdout regular expression pattern:
  > .*[Ee]xit status: 0.*
  > 
Expected stderr regular expression pattern:
  <empty>
Errors:
  in stdout: could not find a match for pattern [> .*[Ee]xit status: 0.*] in line [> ]
  in stdout: could not find match for pattern [> .*[Ee]xit status: 0.*] in ['> ']
  in stderr: unexpected extra output [['ASAN:DEADLYSIGNAL', '=================================================================', '==10513==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x55b423b3a78b bp 0x7fff9dfb7100 sp 0x7fff9dfb6700 T0)', '==10513==The signal is caused by a READ memory access.', '==10513==Hint: address points to the zero page.', '    #0 0x55b423b3a78a in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/student/shell-skeleton/shell/main.cc:188', '    #1 0x55b423b359b5 in main /home/student/shell-skeleton/shell/main.cc:322', '    #2 0x7f476938db96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)', '    #3 0x55b423b35cc9 in _start (/home/student/shell-skeleton/shell/msh+0x2cc9)', '', 'AddressSanitizer can not provide additional info.', 'SUMMARY: AddressSanitizer: SEGV /home/student/shell-skeleton/shell/main.cc:188 in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)', '==10513==ABORTING']]
Failed test trivial command, exit status 1
Test input:
  /bin/false
  exit
Actual stdout:
  > 
Actual stderr:
  ASAN:DEADLYSIGNAL
  =================================================================
  ==10514==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x5558707a678b bp 0x7ffd26fbb820 sp 0x7ffd26fbae20 T0)
  ==10514==The signal is caused by a READ memory access.
  ==10514==Hint: address points to the zero page.
  [plus 9 more lines, not shown]
Expected stdout regular expression pattern:
  > /bin/false.*[Ee]xit status: 1.*
  > 
Expected stderr regular expression pattern:
  <empty>
Errors:
  in stdout: could not find a match for pattern [> /bin/false.*[Ee]xit status: 1.*] in line [> ]
  in stdout: could not find match for pattern [> /bin/false.*[Ee]xit status: 1.*] in ['> ']
  in stderr: unexpected extra output [['ASAN:DEADLYSIGNAL', '=================================================================', '==10514==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x5558707a678b bp 0x7ffd26fbb820 sp 0x7ffd26fbae20 T0)', '==10514==The signal is caused by a READ memory access.', '==10514==Hint: address points to the zero page.', '    #0 0x5558707a678a in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/student/shell-skeleton/shell/main.cc:188', '    #1 0x5558707a19b5 in main /home/student/shell-skeleton/shell/main.cc:322', '    #2 0x7faa8b824b96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)', '    #3 0x5558707a1cc9 in _start (/home/student/shell-skeleton/shell/msh+0x2cc9)', '', 'AddressSanitizer can not provide additional info.', 'SUMMARY: AddressSanitizer: SEGV /home/student/shell-skeleton/shell/main.cc:188 in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)', '==10514==ABORTING']]
Failed test trivial command, exit status 1 (do not check for command name)
Test input:
  /bin/false
  exit
Actual stdout:
  > 
Actual stderr:
  ASAN:DEADLYSIGNAL
  =================================================================
  ==10515==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x55c07e5f378b bp 0x7ffce6fb7960 sp 0x7ffce6fb6f60 T0)
  ==10515==The signal is caused by a READ memory access.
  ==10515==Hint: address points to the zero page.
  [plus 9 more lines, not shown]
Expected stdout regular expression pattern:
  > .*[Ee]xit status: 1.*
  > 
Expected stderr regular expression pattern:
  <empty>
Errors:
  in stdout: could not find a match for pattern [> .*[Ee]xit status: 1.*] in line [> ]
  in stdout: could not find match for pattern [> .*[Ee]xit status: 1.*] in ['> ']
  in stderr: unexpected extra output [['ASAN:DEADLYSIGNAL', '=================================================================', '==10515==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x55c07e5f378b bp 0x7ffce6fb7960 sp 0x7ffce6fb6f60 T0)', '==10515==The signal is caused by a READ memory access.', '==10515==Hint: address points to the zero page.', '    #0 0x55c07e5f378a in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/student/shell-skeleton/shell/main.cc:188', '    #1 0x55c07e5ee9b5 in main /home/student/shell-skeleton/shell/main.cc:322', '    #2 0x7f37a819eb96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)', '    #3 0x55c07e5eecc9 in _start (/home/student/shell-skeleton/shell/msh+0x2cc9)', '', 'AddressSanitizer can not provide additional info.', 'SUMMARY: AddressSanitizer: SEGV /home/student/shell-skeleton/shell/main.cc:188 in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)', '==10515==ABORTING']]
Failed test trivial command, exit status 2 (check for command name)
Test input:
  test/exit2.sh
  exit
Actual stdout:
  > 
Actual stderr:
  ASAN:DEADLYSIGNAL
  =================================================================
  ==10516==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x55ba09ff078b bp 0x7ffe4fa258c0 sp 0x7ffe4fa24ec0 T0)
  ==10516==The signal is caused by a READ memory access.
  ==10516==Hint: address points to the zero page.
  [plus 9 more lines, not shown]
Expected stdout regular expression pattern:
  > test/exit2.sh.*exit status: 2.*
  > 
Expected stderr regular expression pattern:
  <empty>
Errors:
  in stdout: could not find a match for pattern [> test/exit2.sh.*exit status: 2.*] in line [> ]
  in stdout: could not find match for pattern [> test/exit2.sh.*exit status: 2.*] in ['> ']
  in stderr: unexpected extra output [['ASAN:DEADLYSIGNAL', '=================================================================', '==10516==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x55ba09ff078b bp 0x7ffe4fa258c0 sp 0x7ffe4fa24ec0 T0)', '==10516==The signal is caused by a READ memory access.', '==10516==Hint: address points to the zero page.', '    #0 0x55ba09ff078a in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/student/shell-skeleton/shell/main.cc:188', '    #1 0x55ba09feb9b5 in main /home/student/shell-skeleton/shell/main.cc:322', '    #2 0x7fedcb596b96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)', '    #3 0x55ba09febcc9 in _start (/home/student/shell-skeleton/shell/msh+0x2cc9)', '', 'AddressSanitizer can not provide additional info.', 'SUMMARY: AddressSanitizer: SEGV /home/student/shell-skeleton/shell/main.cc:188 in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)', '==10516==ABORTING']]
Failed test trivial command, exit status 2 (do not check for command name)
Test input:
  test/exit2.sh
  exit
Actual stdout:
  > 
Actual stderr:
  ASAN:DEADLYSIGNAL
  =================================================================
  ==10517==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x55d500d5c78b bp 0x7ffe845f2250 sp 0x7ffe845f1850 T0)
  ==10517==The signal is caused by a READ memory access.
  ==10517==Hint: address points to the zero page.
  [plus 9 more lines, not shown]
Expected stdout regular expression pattern:
  > .*exit status: 2.*
  > 
Expected stderr regular expression pattern:
  <empty>
Errors:
  in stdout: could not find a match for pattern [> .*exit status: 2.*] in line [> ]
  in stdout: could not find match for pattern [> .*exit status: 2.*] in ['> ']
  in stderr: unexpected extra output [['ASAN:DEADLYSIGNAL', '=================================================================', '==10517==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x55d500d5c78b bp 0x7ffe845f2250 sp 0x7ffe845f1850 T0)', '==10517==The signal is caused by a READ memory access.', '==10517==Hint: address points to the zero page.', '    #0 0x55d500d5c78a in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/student/shell-skeleton/shell/main.cc:188', '    #1 0x55d500d579b5 in main /home/student/shell-skeleton/shell/main.cc:322', '    #2 0x7f898940bb96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)', '    #3 0x55d500d57cc9 in _start (/home/student/shell-skeleton/shell/msh+0x2cc9)', '', 'AddressSanitizer can not provide additional info.', 'SUMMARY: AddressSanitizer: SEGV /home/student/shell-skeleton/shell/main.cc:188 in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)', '==10517==ABORTING']]
Failed test trivial command, exit status 1 (check for command name)
Test input:
  /bin/false
  exit
Actual stdout:
  > 
Actual stderr:
  ASAN:DEADLYSIGNAL
  =================================================================
  ==10518==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x55ea6239478b bp 0x7ffe541c20b0 sp 0x7ffe541c16b0 T0)
  ==10518==The signal is caused by a READ memory access.
  ==10518==Hint: address points to the zero page.
  [plus 9 more lines, not shown]
Expected stdout regular expression pattern:
  > /bin/false.*[Ee]xit status: 1.*
  > 
Expected stderr regular expression pattern:
  <empty>
Errors:
  in stdout: could not find a match for pattern [> /bin/false.*[Ee]xit status: 1.*] in line [> ]
  in stdout: could not find match for pattern [> /bin/false.*[Ee]xit status: 1.*] in ['> ']
  in stderr: unexpected extra output [['ASAN:DEADLYSIGNAL', '=================================================================', '==10518==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x55ea6239478b bp 0x7ffe541c20b0 sp 0x7ffe541c16b0 T0)', '==10518==The signal is caused by a READ memory access.', '==10518==Hint: address points to the zero page.', '    #0 0x55ea6239478a in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/student/shell-skeleton/shell/main.cc:188', '    #1 0x55ea6238f9b5 in main /home/student/shell-skeleton/shell/main.cc:322', '    #2 0x7f258343db96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)', '    #3 0x55ea6238fcc9 in _start (/home/student/shell-skeleton/shell/msh+0x2cc9)', '', 'AddressSanitizer can not provide additional info.', 'SUMMARY: AddressSanitizer: SEGV /home/student/shell-skeleton/shell/main.cc:188 in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)', '==10518==ABORTING']]
Failed test pass arguments
Test input:
  test/argument_test.sh first second_with_underscore third
  exit
Actual stdout:
  > 
Actual stderr:
  ASAN:DEADLYSIGNAL
  =================================================================
  ==10525==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x559e5f98d78b bp 0x7ffee10497b0 sp 0x7ffee1048db0 T0)
  ==10525==The signal is caused by a READ memory access.
  ==10525==Hint: address points to the zero page.
  [plus 9 more lines, not shown]
Expected stdout regular expression pattern:
  > 
  number of arguments: 3
  argument 1: first
  argument 2: second_with_underscore
  argument 3: third
  [plus 3 more lines, not shown]
Expected stderr regular expression pattern:
  <empty>
Errors:
  in stdout: could not find match for pattern [number of arguments: 3] in []
  in stderr: unexpected extra output [['ASAN:DEADLYSIGNAL', '=================================================================', '==10525==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x559e5f98d78b bp 0x7ffee10497b0 sp 0x7ffee1048db0 T0)', '==10525==The signal is caused by a READ memory access.', '==10525==Hint: address points to the zero page.', '    #0 0x559e5f98d78a in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/student/shell-skeleton/shell/main.cc:188', '    #1 0x559e5f9889b5 in main /home/student/shell-skeleton/shell/main.cc:322', '    #2 0x7f2f284c9b96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)', '    #3 0x559e5f988cc9 in _start (/home/student/shell-skeleton/shell/msh+0x2cc9)', '', 'AddressSanitizer can not provide additional info.', 'SUMMARY: AddressSanitizer: SEGV /home/student/shell-skeleton/shell/main.cc:188 in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)', '==10525==ABORTING']]
Failed test " is not quote
Test input:
  test/argument_test.sh "not quoted"
  exit
Actual stdout:
  > 
Actual stderr:
  ASAN:DEADLYSIGNAL
  =================================================================
  ==10526==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x562f2b89e78b bp 0x7ffc5620fa00 sp 0x7ffc5620f000 T0)
  ==10526==The signal is caused by a READ memory access.
  ==10526==Hint: address points to the zero page.
  [plus 9 more lines, not shown]
Expected stdout regular expression pattern:
  > 
  number of arguments: 2
  argument 1: "not
  argument 2: quoted"
  argument 3: 
  [plus 3 more lines, not shown]
Expected stderr regular expression pattern:
  <empty>
Errors:
  in stdout: could not find match for pattern [number of arguments: 2] in []
  in stderr: unexpected extra output [['ASAN:DEADLYSIGNAL', '=================================================================', '==10526==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x562f2b89e78b bp 0x7ffc5620fa00 sp 0x7ffc5620f000 T0)', '==10526==The signal is caused by a READ memory access.', '==10526==Hint: address points to the zero page.', '    #0 0x562f2b89e78a in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/student/shell-skeleton/shell/main.cc:188', '    #1 0x562f2b8999b5 in main /home/student/shell-skeleton/shell/main.cc:322', '    #2 0x7f8b91f64b96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)', '    #3 0x562f2b899cc9 in _start (/home/student/shell-skeleton/shell/msh+0x2cc9)', '', 'AddressSanitizer can not provide additional info.', 'SUMMARY: AddressSanitizer: SEGV /home/student/shell-skeleton/shell/main.cc:188 in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)', '==10526==ABORTING']]
Failed test varying argument counts and lengths
Test input:
  test/argument_test.sh aX bX cX dX eX
  test/argument_test.sh f g hZZ i
  test/argument_test.sh j k l
  exit
Actual stdout:
  > 
Actual stderr:
  ASAN:DEADLYSIGNAL
  =================================================================
  ==10527==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x5651f613e78b bp 0x7ffef14c0470 sp 0x7ffef14bfa70 T0)
  ==10527==The signal is caused by a READ memory access.
  ==10527==Hint: address points to the zero page.
  [plus 9 more lines, not shown]
Expected stdout regular expression pattern:
  > 
  number of arguments: 5
  argument 1: aX
  argument 2: bX
  argument 3: cX
  [plus 17 more lines, not shown]
Expected stderr regular expression pattern:
  <empty>
Errors:
  in stdout: could not find match for pattern [number of arguments: 5] in []
  in stderr: unexpected extra output [['ASAN:DEADLYSIGNAL', '=================================================================', '==10527==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x5651f613e78b bp 0x7ffef14c0470 sp 0x7ffef14bfa70 T0)', '==10527==The signal is caused by a READ memory access.', '==10527==Hint: address points to the zero page.', '    #0 0x5651f613e78a in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/student/shell-skeleton/shell/main.cc:188', '    #1 0x5651f61399b5 in main /home/student/shell-skeleton/shell/main.cc:322', '    #2 0x7f578e72fb96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)', '    #3 0x5651f6139cc9 in _start (/home/student/shell-skeleton/shell/msh+0x2cc9)', '', 'AddressSanitizer can not provide additional info.', 'SUMMARY: AddressSanitizer: SEGV /home/student/shell-skeleton/shell/main.cc:188 in parse_and_run_command(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)', '==10527==ABORTING']]
Makefile:13: recipe for target 'test' failed
