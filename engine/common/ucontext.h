
#include <asm/sigcontext.h>       /* for sigcontext */
#include <asm/signal.h>           /* for stack_t */
#include <signal.h>
#include <dlfcn.h>

//ucontext.h is not present on android, so we need to define some types

typedef struct ucontext {
	unsigned long uc_flags;
	struct ucontext *uc_link;
	stack_t uc_stack;
	struct sigcontext uc_mcontext;
	unsigned long uc_sigmask;
} ucontext_t;
