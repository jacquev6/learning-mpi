This repo tracks my learning of MPI (Message Passing Interface).
It contains a list of resources I read, and examples I wrote while discovering MPI.

Resources
=========

- The [Wikipedia article about MPI](https://en.wikipedia.org/wiki/Message_Passing_Interface)

Examples
========

Hello World (from the Wikipedia article)
----------------------------------------

[Source code](wikipedia-hello-world).

I'm using the [ubuntu:20.04](https://hub.docker.com/_/ubuntu) Docker image as a base

Package [libopenmpi-dev](https://www.open-mpi.org/) comes with a compiler wrapper for `gcc` named [`mpicc`](https://www.open-mpi.org/doc/current/man1/mpicc.1.php) (and similar wrappers for `g++`, `gfortran`, etc.), and a wrapper to execute the program, named [`mpirun`](https://www.open-mpi.org/doc/current/man1/mpirun.1.php). Their use is visible in `wikipedia-hello-world/run.sh`.

These wrappers are installed using `update-alternatives`, so they can't be found using [Ubuntu's packages website](https://packages.ubuntu.com/)'s "Search the contents of packages" functionality.
Finding their actual location is quite the treasure hunt:

    $ which mpicc
    /usr/bin/mpicc
    $ file /usr/bin/mpicc
    /usr/bin/mpicc: symbolic link to /etc/alternatives/mpi
    $ file /etc/alternatives/mpi
    /etc/alternatives/mpi: symbolic link to /usr/bin/mpicc.openmpi
    $ file /usr/bin/mpicc.openmpi
    /usr/bin/mpicc.openmpi: symbolic link to opal_wrapper

And `opal_wrapper` comes from package [openmpi-bin](https://packages.ubuntu.com/search?searchon=contents&keywords=opal_wrapper&mode=exactfilename&suite=impish&arch=any).

@todo Try and compile using plain `gcc`, to understand what the wrapper does

@todo Try and run without the wrapper, to understand what it does

Looking at the code:

- Everything is wrapped between `MPI_Init` and `MPI_Finalize`
- Each process gets a "rank", using `MPI_Comm_rank`, and knows how many processes there are in total, using `MPI_Comm_size`. This requires that `mpirun` does all bookkeeping to let these processes, and only them, communicate.
- Each process then chooses its role based on its rank, à la `fork`.
- `MPI_Send` is used to send a message to another process, identified by its rank.
- `MPI_Recv` is used to receive a message from another process, also identified by its rank.

@todo Is there a higher level of abstraction than rank to choose the role of each process?

@todo Can binaries built from different source code communicate? (*e.g.* a coordinator and several workers)

@todo Investigate non-blocking "send" and "receive" functions.

@todo What happens when a process calls `MPI_Send` while no other process is calling `MPI_Recv`?

@todo How does one run this example on several machines?
