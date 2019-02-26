#include "defines.h"
#include "kozos.h"
#include "syscall.h"
#include "lib.h"

kz_thread_id_t kz_run(kz_func_t func, char *name, int stacksize,
						int argc, char *argv[]){
	puts("kz_run\n");
	kz_syscall_param_t param;
	param.un.run.func = func;
	param.un.run.name = name;
	param.un.run.stacksize = stacksize;
	param.un.run.argc = argc;
	param.un.run.argv = argv;
	kz_syscall(KZ_SYSCALL_TYPE_RUN, &param);
	return param.un.run.ret;
}

void kz_exit(void){
	puts("kz_exit\n");
	kz_syscall(KZ_SYSCALL_TYPE_EXIT, NULL);
}
