This repo tracks my learning of MPI (Message Passing Interface).
It contains a list of resources I read, and examples I wrote while discovering MPI.

Resources
=========

- The [Wikipedia article about MPI](https://en.wikipedia.org/wiki/Message_Passing_Interface)
- [OpenMPI 4.1 documentation](https://www.open-mpi.org/doc/v4.1/)
- The [Wikipedia article about Flynn's taxonomy](https://en.wikipedia.org/wiki/Flynn%27s_taxonomy)
- This [MPI tutorial](https://mpitutorial.com), quite complete, easy to read

Acronyms
========

- MPI: Message Passing Interface (generic term for the specification and the many implementations)
- ORTE: Open Run Time Environment: the runtime part of OpenMPI
- OPAL: Open Portable Access Layer: utility and glue in OpenMPI

Examples
========

Each example can be built and run using its `run.sh` script.

I'm using the [ubuntu:20.04](https://hub.docker.com/_/ubuntu) Docker image as a base

Hello World (from the Wikipedia article)
----------------------------------------

[Source code](wikipedia-hello-world).

Package [libopenmpi-dev](https://www.open-mpi.org/) comes with a compiler wrapper for `gcc` named [`mpicc`](https://www.open-mpi.org/doc/v4.1/man1/mpicc.1.php) (and similar wrappers for `g++`, `gfortran`, etc.), and a wrapper to execute the program, named [`mpiexec`](https://www.open-mpi.org/doc/v4.1/man1/mpiexec.1.php). Their use is visible in `wikipedia-hello-world/run.sh`.

`mpirun` is an alias for `mpiexec`, but `mpiexec` is "recommended by the MPI standard".

Being familiar with [Flynn's taxonomy](https://en.wikipedia.org/wiki/Flynn%27s_taxonomy) seems useful to understand the man page of `mpiexec`.

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
- Each process gets a "rank", using `MPI_Comm_rank`, and knows how many processes there are in total, using `MPI_Comm_size`. This requires that `mpiexec` does all bookkeeping to let these processes, and only them, communicate.
- Each process then chooses its role based on its rank, à la `fork`.
- `MPI_Send` is used to send a message to another process, identified by its rank.
- `MPI_Recv` is used to receive a message from another process, also identified by its rank.

@todo Is there a higher level of abstraction than rank to choose the role of each process?

@todo Can binaries built from different source code communicate? (*e.g.* a coordinator and several workers)

@todo Investigate non-blocking "send" and "receive" functions.

@todo What happens when a process calls `MPI_Send` while no other process is calling `MPI_Recv`?

@todo How does one run this example on several machines?

@todo What happens to rank and and size when a process crashes?

Ring
----

[Source code](ring).

The goal of this example is for me to write something not completely trivial using the most basic API.

MPI is *very* flexible: it only provides a way to send individual messages from a process to an other.
No request-response, no dataflow (like UDP, not like TCP), no RPC, nothing that gives structure to the exchange.
It's low-level, and thus both very simple and sometimes too generic and flexible.

Termination conditions for this example are a bit subtle: process 0 must stop after it *receives* the last message from the last process, but all other processes must stop after *sending* the last message.
This ring feels like a pattern that could be abstracted in a library, with others.

@todo Search for "MPI patterns"

Programs that use the `MPI_Send`/`MPI_Recv` API **have** to use their rank to specialize their behavior.
True per-to-peer behavior is not possible using these API.
