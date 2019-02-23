#include "defines.h"
#include "kozos.h"
#include "intr.h"
#include "interrupt.h"
#include "syscall.h"
#include "lib.h"

#include "utils.h"

#define THREAD_NUM	6
#define THREAD_NAME_SIZE	15

typedef struct _kz_context{
	uint64 sp;
}kz_context;

typedef struct _kz_thread{
	struct _kz_thread *next;
	char name[THREAD_NAME_SIZE + 1];
	char *stack;

	struct{
		kz_func_t func;
		int argc;
		char **argv;
	}init;

	struct{
		kz_syscall_type_t type;
		kz_syscall_param_t *param;
	}syscall;

	kz_context context;
}kz_thread;

static struct{
	kz_thread *head;
	kz_thread *tail;
}readyque;

static kz_thread *current;
static kz_thread threads[THREAD_NUM];
static kz_handler_t handlers[SOFTVEC_TYPE_NUM];

unsigned long dispatch(kz_context *context);

static int getcurrent(void){
	if(current == NULL){
		return -1;
	}

	readyque.head = current->next;
	if(readyque.head == NULL){
		readyque.tail = NULL;
	}
	current->next = NULL;

	return 0;
}

static int putcurrent(void){
	if(current == NULL){
		return -1;
	}

	if(readyque.tail){
		readyque.tail->next = current;
	}else{
		readyque.head = current;
	}
	readyque.tail = current;

	return 0;
}

static void thread_end(void){
	puts("thread_end begin\n");
	kz_exit();
}

static void thread_init(kz_thread *thp){
	puts("thread_init begin\n");
	thp->init.func(thp->init.argc, thp->init.argv);
	thread_end();
}

static kz_thread_id_t thread_run(kz_func_t func, char *name,
								int stacksize, int argc, char *argv[]){
	puts("thread_run begin\n");

	int i;
	kz_thread *thp;
	uint64 *sp;
	extern char _userstack;
	static char *thread_stack = &_userstack;

	for(i=0; i<THREAD_NUM; i++){
		thp = &threads[i];
		if(!thp->init.func)
			break;
	}
	if(i == THREAD_NUM)
		return -1;
	
	puts("thp   "); putxval(thp,0); puts("\n");
	memset(thp, 0, sizeof(*thp));

	strcpy(thp->name, name);
	thp->next = NULL;

	thp->init.func = func;
	thp->init.argc = argc;
	thp->init.argv = argv;

	memset(thread_stack, 0, stacksize);
	thread_stack += stacksize;

	thp->stack = thread_stack;

	sp = (uint64 *)thp->stack;
	// *(--sp) = (uint64)thread_end;
	putxval(sp,0); puts("\n");

	// *(--sp) = (uint64)thread_init;
	set_elr_el1((uint64)thread_init);

	--sp;
	*(--sp) = 0; // X30
	*(--sp) = 0; // X29
	*(--sp) = 0; // X28
	*(--sp) = 0; // X27
	*(--sp) = 0; // X26
	*(--sp) = 0; // X25
	*(--sp) = 0; // X24
	*(--sp) = 0; // X23
	*(--sp) = 0; // X22
	*(--sp) = 0; // X21
	*(--sp) = 0; // X20
	*(--sp) = 0; // X19
	*(--sp) = 0; // X18
	*(--sp) = 0; // X17
	*(--sp) = 0; // X16
	*(--sp) = 0; // X15
	*(--sp) = 0; // X14
	*(--sp) = 0; // X13
	*(--sp) = 0; // X12
	*(--sp) = 0; // X11
	*(--sp) = 0; // X10
	*(--sp) = 0; // X9
	*(--sp) = 0; // X8
	*(--sp) = 0; // X7
	*(--sp) = 0; // X6
	*(--sp) = 0; // X5
	*(--sp) = 0; // X4
	*(--sp) = 0; // X3
	*(--sp) = 0; // X2
	*(--sp) = 0; // X1

	*(--sp) = (uint64)thp;

	thp->context.sp = (uint64)sp;

	putcurrent();

	current = thp;
	putcurrent();

	return (kz_thread_id_t)current;
}

static int thread_exit(void){
	puts("thread_exit begin\n");
	puts(current->name);
	puts(" EXIT.\n");
	memset(current, 0, sizeof(*current));
	return 0;
}

static void thread_intr(softvec_type_t type, unsigned long sp);

static int setintr(softvec_type_t type, kz_handler_t handler){
	puts("setintr begin\n");
	/*prototype declaration inside a function is not allowed
	  in the compiler, so declear "thread_intr" above this function*/
	softvec_setintr(type, thread_intr);

	handlers[type] = handler;

	return 0;
}

static void call_functions(kz_syscall_type_t type, kz_syscall_param_t *p){
	puts("call_functions begin\n");
	switch(type){
		case KZ_SYSCALL_TYPE_RUN:
			p->un.run.ret = thread_run(p->un.run.func, p->un.run.name,
										p->un.run.stacksize,
										p->un.run.argc, p->un.run.argv);
			break;
		case KZ_SYSCALL_TYPE_EXIT:
			thread_exit();
			break;
		default:
			break;
	}
}

static void syscall_proc(kz_syscall_type_t type, kz_syscall_param_t *p){
	puts("syscall_proc begin\n");
	getcurrent();
	call_functions(type, p);
}

static void schedule(void){
	puts("schedule begin\n");
	if(!readyque.head)
		kz_sysdown();
	
	current = readyque.head;
}

static void syscall_intr(void){
	puts("syscall_intr begin\n");
	syscall_proc(current->syscall.type, current->syscall.param);
}

static void softerr_intr(void){
	puts(current->name);
	puts(" DOWN.\n");
	getcurrent();
	thread_exit();
}

static void thread_intr(softvec_type_t type, unsigned long sp){
	puts("thread_intr\n");
	unsigned long addr;
	addr = get_esr_el1();
	puts("esr_el1: "); putxval(addr, 0); puts("\n");


	current->context.sp = sp;

	if(handlers[type])
		handlers[type]();

	schedule();

	dispatch(&current->context);
}

void kz_start(kz_func_t func, char *name, int stacksize,
				int argc, char *argv[]){
	puts("kz_start begin\n");
	current = NULL;

	readyque.head = readyque.tail = NULL;
	memset(threads, 0, sizeof(threads));
	memset(handlers, 0, sizeof(handlers));

	setintr(SOFTVEC_TYPE_SYSCALL, syscall_intr);
	setintr(SOFTVEC_TYPE_SOFTERR, softerr_intr);

	current = (kz_thread *)thread_run(func, name, stacksize, argc, argv);

	putxval(&current->context, 0); puts("\n");
	putxval(current->context.sp, 0); puts("\n");

	unsigned long addr;
	addr = dispatch(&current->context);
	puts("in dis"); putxval(addr, 0); puts("\n");
}

void kz_sysdown(void){
	puts("system error!\n");
	while(1)
		;
}

void kz_syscall(kz_syscall_type_t type, kz_syscall_param_t *param){
	current->syscall.type = type;
	current->syscall.param = param;
	asm volatile ("svc #0");
}
