#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "devices/shutdown.h"
#include "devices/input.h"
#include "lib/kernel/console.h"

static void syscall_handler (struct intr_frame *);

void 
halt(void){
	//printf("Entered halt function\n");
	shutdown_power_off();
}


void 
exit(int status){
	//struct thread* curr=thread_current();
	printf("%s: exit(%d)\n",thread_name(), status);	
	thread_current()->exit_status=status;
	thread_exit();
}

pid_t 
exec(const char* cmd){
	return process_execute(cmd);
}

int 
wait(pid_t pid){
	return process_wait(pid);
}




void
check_address(void* addr){
  if(!is_user_vaddr(addr)){
    //printf("Invalid address!\n");
    exit(-1);    
  }
}


int 
write (int fd, const void* buffer, unsigned size){
	if(fd!=1)
		return -1;
	else{
		putbuf(buffer,size);
		return size;
	}
}

int 
read(int fd, void* buffer, unsigned size){
	if(fd!=0)
		return -1;
	else{
		int i=0;
		for(i=0;i<size;i++){
			if(input_getc()=='\0')
				break;
		}
		return i; 
	}
}

int 
fibonacci(int n){
	//printf("Entered Fibo! %d\n",n);
	if(n==0)
		return 0;
	else{
		int a=0;
		int b=1;
		int cnt=2;
		while(cnt<n){
			int c=b;
			b=a+b;
			a=c;
			++cnt;
		}
		return a+b;
	}
}

int 
max_of_four_int(int a,int b,int c,int d){
	//printf("Entered MOF! %d %d %d %d\n",a,b,c,d);
	int arr[4];
	arr[0]=a; arr[1]=b; arr[2]=c; arr[3]=d;
	int max=arr[0];
	for(int i=0;i<4;i++){
		if(arr[i]>=max){
			max=arr[i];
		}
	}
	return max;
}




void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f ) 
{
  //printf ("system call! : %d\n",*(uint32_t*)(f->esp));
  //hex_dump(f->esp, f->esp, 100, 1);
  void* ESP=f->esp;
  switch(*(uint32_t*)(ESP)){
	  case SYS_HALT:
		  //printf("SYS_HALT!\n");
		  halt();
		  break;

	  case SYS_EXIT:
		  check_address(ESP+4);
		  exit(*(uint32_t *)(ESP+4));
		  break;

	  case SYS_EXEC:
		  check_address(ESP+4); 
		  f->eax=exec((const char*)*(uint32_t*)(ESP+4));
		  break;

	  case SYS_WAIT:
		  check_address(ESP+4);
		  f->eax=wait((pid_t)*(uint32_t*)(ESP+4));
		  break;

	  case SYS_READ:
		  check_address(ESP+4);
		  check_address(ESP+8);
		  check_address(ESP+12);
		  f->eax = read ( (int)*(uint32_t *)(ESP + 4), (void *)*(uint32_t *)(ESP + 8), (unsigned)*((uint32_t *)(ESP + 12)) );
		  break;

	  case SYS_WRITE:
		  check_address(ESP+4);
		  check_address(ESP+8);
		  check_address(ESP+12);
		  f->eax = write((int)*(uint32_t*)(ESP+4),(void*)*(uint32_t*)(ESP+8),(unsigned)*((uint32_t*)(ESP+12)));
		  break;

	  case SYS_FIBONACCI:
		  check_address(ESP+4);
		  f->eax = fibonacci((int)*(uint32_t*)(ESP+4));
		  break;

	  case SYS_MAX_OF_FOUR_INT:
		  check_address(ESP+4);
		  check_address(ESP+8);
		  check_address(ESP+12);
		  check_address(ESP+16);
		  f->eax=max_of_four_int((int)*(uint32_t*)(ESP+4),(int)*(uint32_t*)(ESP+8),(int)*(uint32_t*)(ESP+12),(int)*(uint32_t*)(ESP+16));
		  break;
  }
  //thread_exit ();
}
