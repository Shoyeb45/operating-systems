# The Process API's

- Standard libraries provide many API's to interact with the process. Few of them are: 

## 1. `fork`

- fork system call is used to create a new process from one running process. The newly created process is called `child` process and the process which have created the child, is known as `parent`.
- Codes related to the fork: [fork](./codes/01-fork-api.c) 

## 2. `wait`
- `wait` system call is used to wait for a process to finish. So, it will only start executing if the request process has finished executing. There are variants for wait, for eg., `wait`, `waitpid`, `wait2`, ..,`wait4`.
- Codes: [wait](./codes/02-wait-api.c)

## 3. `exec`
- `exec` has many variants like wait, `execl`, `execv`, `execvp`, `execp`. There is difference how the parameter will be passed to different exec system calls
- Now, `exec` will replace the current running process to the specified executable process. 
- Codes: [exec](./codes/03-exec-api.c). Here, we are calling exec with grep, so it will replace the current c program to the grep and output will be return of grep.

- UNIX shell used combination of these system calls to produce magic tricks, like piping the processes input-output, redirecting the output to another file using '>' or using &.
- One such example of redirection: [write-to-file](./codes/04-redirection.c)