#pragma once

// Only on Linux.
#ifdef __linux__

#include <cstddef>
#include <string>

#include <cxxabi.h>
#include <execinfo.h>
#include <inttypes.h>
#include <stdio.h>
#include <signal.h>

static size_t __attribute__((unused))
stack_backtrace(char* output_buf, size_t output_buflen) {
    void* stack_ptr[256];
    int stack_size = 0;
    stack_size = backtrace(stack_ptr, 256);

    char** stack_msg = nullptr;
    stack_msg = backtrace_symbols(stack_ptr, stack_size);

    size_t offset = 0;
    size_t frame_num = 0;
    for (int i=0; i<stack_size; ++i) {
        int fname_len = 0;
        while ( stack_msg[i][fname_len] != '(' &&
                stack_msg[i][fname_len] != ' ' &&
                stack_msg[i][fname_len] != 0 ) {
            ++fname_len;
        }

        char cmd[1024];
        sprintf( cmd,"addr2line -f -e %.*s %" PRIxPTR "",
                 fname_len, stack_msg[i], (uintptr_t)stack_ptr[i] );
        FILE* fp = popen(cmd, "r");
        if (!fp) continue;

        char mangled_name[1024];
        char file_line[1024];
        int ret = fscanf(fp, "%s %s", mangled_name, file_line);
        (void)ret;
        pclose(fp);

        offset += snprintf( output_buf + offset, output_buflen - offset,
                            "#%-2zu 0x%016" PRIxPTR " in ",
                            frame_num++, (uintptr_t)stack_ptr[i] );
        if (offset >= output_buflen) return output_buflen;

        int status;
        char *cc = abi::__cxa_demangle(mangled_name, 0, 0, &status);
        if (cc) {
            offset += snprintf( output_buf + offset, output_buflen - offset,
                                "%s at ", cc );
        } else {
            std::string msg_str = stack_msg[i];
            size_t s_pos = msg_str.find("(");
            size_t e_pos = msg_str.rfind("+");
            std::string _func_name = msg_str.substr(s_pos+1, e_pos-s_pos-1);
            offset += snprintf( output_buf + offset, output_buflen - offset,
                                "%s() at ", _func_name.c_str() );
        }
        if (offset >= output_buflen) return output_buflen;

        offset += snprintf( output_buf + offset, output_buflen - offset,
                            "%s\n", file_line );
        if (offset >= output_buflen) return output_buflen;
    }

    return offset;
}

#else

static size_t __attribute__((unused))
stack_backtrace(char* output_buf, size_t output_buflen) {
    return 0;
}

#endif
