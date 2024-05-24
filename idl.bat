midl /amd64 /cstub print_c_64.cpp /sstub print_s_64.c .\print.idl
midl /win32 /cstub print_c.cpp /sstub print_s.c .\print.idl
move /Y print_c_64.cpp global-inject-lib
move /Y print_c.cpp global-inject-lib
copy /Y print.h global-inject-lib
move /Y print_s_64.c global-inject-log-server
move /Y print_s.c global-inject-log-server
copy /Y print.h global-inject-log-server
