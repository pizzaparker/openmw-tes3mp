# Once found, defines:
#  Sol2_FOUND
#  Sol2_INCLUDE_DIRS

include(LibFindMacros)

libfind_pkg_detect(Sol2 sol2
    FIND_PATH sol.hpp
)
libfind_process(Sol2)

