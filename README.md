This repo tracks my learning of MPI (Message Passing Interface).
It contains a list of resources I read, and examples I wrote while discovering MPI.

Resources
=========

- The [Wikipedia article about MPI](https://en.wikipedia.org/wiki/Message_Passing_Interface)
- The [MPI standards](https://www.mpi-forum.org/docs/)
- [OpenMPI 4.1 documentation](https://www.open-mpi.org/doc/v4.1/)
- The [Wikipedia article about Flynn's taxonomy](https://en.wikipedia.org/wiki/Flynn%27s_taxonomy)
- This [MPI tutorial](https://mpitutorial.com), quite complete, easy to read
- This [presentation about MPI patterns](https://cs.gmu.edu/~kauffman/cs499/comm-patterns.pdf) (page 7+, **fundamental reading**), which references [these notes on MPI](http://www.mathcs.emory.edu/~cheung/Courses/355/Syllabus/92-MPI/group-comm.html)

Memento
=======

Acronyms
--------

- MPI: Message Passing Interface (generic term for the specification and the many implementations)
- OMPI: [Open MPI](https://www.open-mpi.org/), an implementation of MPI
- OPAL: Open Portable Access Layer: utility and glue in OpenMPI
- ORTE: Open Run Time Environment: the runtime part of OpenMPI
- PMIx: [Process Management Interface](https://pmix.github.io/)
- OPMI: [Open PMIx](https://openpmix.github.io/), an implementation of PMIx

Concepts
--------

### Processor

A "processor" is a [process](https://en.wikipedia.org/wiki/Process_(computing)) using the MPI infrastructure.

### Communicator

A "communicator" is a set of processors that can communicate together.
A single communicator exists at the start of the program: `MPI_COMM_WORLD`, which includes all processors.
New communicators can be created using *e.g.* `MPI_Comm_create` or `MPI_Comm_split`.

To allow communication without interferences between several communicators,
each communicator contains not only the set of processors, but also a unique identifier.
The set of processors in a communicator is called a "group".
A group is nothing more that a set of processors.
The group of a communicator can be retrieved using `MPI_Comm_group`.
General set manipulation is provided by API like `MPI_Group_union` and `MPI_Group_intersection`.
A communicator can be created from a group using `MPI_Comm_create` or `MPI_Comm_create_group`.

@todo Define inter-communicator

@todo Define intra-communicator

@todo Define neighbor

### Window

@todo Define

### Type

@todo Define

Synchronization API
-------------------

`MPI_Barrier` implements the usual barrier synchronization: each processor waits for all other processors to have reached the barrier

Communication API
-----------------

These API, and a bit more, are demonstrated in the [memento](memento) example.

### Main

Legend:
- ⬛: uninitialized data
- 🟪🟦🟩🟨🟧🟥🐶🐱🐷♠️♥️♦️: initialized data

| Purpose | Processor | Before | Action | After |
| --- | --- | --- | --- | --- |
| One-to-one | 0<br>1 | 🟩<br>⬛ | `Send(1)`<br>`Recv(0)` | 🟩<br>🟩 |
| One-to-one,<br>dynamic size | 0<br><br>1<br><br>&nbsp; | 🟩<br><br>-<br>-<br>⬛ | `Send(1)`<br><br>`Probe(0)`, `Get_count`<br>allocate<br>`Recv(0)` | 🟩<br><br>-<br>⬛<br>🟩 |
| One-to-one-to-one | 0<br>1<br>2 | ⬛<br>🟩, ⬛<br>🟪 | `Recv(1)`<br>`Sendrecv(0, 1)`<br>`Send(1)` | 🟩<br>🟩, 🟪<br>🟪
| One-to-all,<br>same data | 0<br>1<br>2 | 🟩<br>⬛<br>⬛ | `Bcast(0)`<br>`Bcast(0)`<br>`Bcast(0)` | 🟩<br>🟩<br>🟩 |
| One-to-all,<br>different data | 0<br>1<br>2 | ⬛, 🟩🟪🟧<br>⬛<br>⬛ | `Scatter(0)`<br>`Scatter(0)`<br>`Scatter(0)` | 🟩, 🟩🟪🟧<br>🟪<br>🟧 |
| All-to-one | 0<br>1<br>2 | 🟩, ⬛⬛⬛<br>🟪<br>🟧 | `Gather(0)`<br>`Gather(0)`<br>`Gather(0)` | 🟩, 🟩🟪🟧<br>🟪<br>🟧 |
| All-to-all,<br>same data | 0<br>1<br>2 | 🟩, ⬛⬛⬛<br>🟪, ⬛⬛⬛<br>🟧, ⬛⬛⬛ | `Allgather(0)`<br>`Allgather(0)`<br>`Allgather(0)` | 🟩, 🟩🟪🟧<br>🟪, 🟩🟪🟧<br>🟧, 🟩🟪🟧 |
| All-to-all,<br>different data<br>(transposition) | 0<br>1<br>2 | 🟪🟦🟩, ⬛⬛⬛<br>🐶🐱🐷, ⬛⬛⬛<br>♠️♥️♦️, ⬛⬛⬛ | `Alltoall()`<br>`Alltoall()`<br>`Alltoall()` | 🟪🟦🟩, 🟪🐶♠️<br>🐶🐱🐷, 🟦🐱♥️<br>♠️♥️♦️, 🟩🐷♦️ |
| All-to-one,<br>reduction | 0<br>1<br>2 | 🟩, ⬛<br>🟪<br>🟧 | `Reduce(0)`<br>`Reduce(0)`<br>`Reduce(0)` | 🟩, 🟥 (=🟩•🟪•🟧)<br>🟪<br>🟧 |
| All-to-all,<br>reduction | 0<br>1<br>2 | 🟩, ⬛<br>🟪, ⬛<br>🟧, ⬛ | `Allreduce(•)`<br>`Allreduce(•)`<br>`Allreduce(•)` | 🟩, 🟥 (=🟩•🟪•🟧)<br>🟪, 🟥<br>🟧, 🟥 |
| All-to-all,<br>reduction,<br>scattered | 0<br>1<br>2 | 🟪🟦🟩, ⬛<br>🐶🐱🐷, ⬛<br>♠️♥️♦️, ⬛ | `Reduce_scatter(•)`<br>`Reduce_scatter(•)`<br>`Reduce_scatter(•)` | 🟪🟦🟩, 🟨 (=🟪•🐶•♠️)<br>🐶🐱🐷, 🟧 (=🟦•🐱•♥️)<br>♠️♥️♦️, 🟥 (=🟩•🐷•♦️) |
| All-to-all,<br>partial reductions,<br>inclusive | 0<br>1<br>2 | 🟩, ⬛<br>🟪, ⬛<br>🟧, ⬛ | `Scan(•)`<br>`Scan(•)`<br>`Scan(•)` | 🟩, 🟩<br>🟪, 🟦 (=🟩•🟪)<br>🟧, 🟥 (=🟩•🟪•🟧) |
| All-to-all,<br>partial reductions,<br>exclusive | 0<br>1<br>2<br>3 | 🟩, ⬛<br>🟪, ⬛<br>🟧, ⬛<br>🟨, ⬛ | `Exscan(•)`<br>`Exscan(•)`<br>`Exscan(•)`<br>`Exscan(•)` | 🟩, ⬛<br>🟪, 🟩<br>🟧, 🟦 (=🟩•🟪)<br>🟨, 🟥 (=🟩•🟪•🟧) |

### Variants

#### Non-blocking

Most API have a variant prefixed with `I` (*e.g.* `MPI_Isend`) that doesn't block.
These variants populate an `MPI_Request` object.
`MPI_Test` can be used to poll that object for completion,
and `MPI_Wait` can be used to wait (block) until it's complete.

#### Buffered

`MPI_Bsend`, along with `MPI_Buffer_attach`, uses user-managed buffers.

#### Matched

`MPI_Mprobe` and `MPI_Mrecv` can be used to ensure the message received *is* the message matched by the probe.
With `MPI_Probe` and `MPI_Recv`, another message could be received even if the source and tag are set properly.

#### Neighbor

`MPI_Alltoall` and `MPI_Allgather` have "neighbor" variants the send data only to the neighboring processors,
as defined in the topology of the communicator.

#### Varying count

Some all-to-* API have a variant suffixed by `v`, that communicate a varying amount of data from each processor.

#### Varying types

`MPI_Ialltoallw` allow additional flexibility over `MPI_Ialltoallv` by specifying different types.

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

Try and compile using plain `gcc`, to understand what the wrapper does: see example "No wrappers" below.

Try and run without the wrapper, to understand what it does: see example "No wrappers" below.

Looking at the code:

- Everything is wrapped between `MPI_Init` and `MPI_Finalize`
- Each processor gets a "rank", using `MPI_Comm_rank`, and knows how many processors there are in total, using `MPI_Comm_size`. This requires that `mpiexec` does all bookkeeping to let these processes, and only them, communicate.
- Each processor then chooses its role based on its rank, à la `fork`.
- `MPI_Send` is used to send a message to another processor, identified by its rank.
- `MPI_Recv` is used to receive a message from another processor, also identified by its rank.

Is there a higher level of abstraction than rank to choose the role of each processor? No

@todo Can binaries built from different source code communicate? (*e.g.* a coordinator and several workers)

Non-blocking "send" and "receive" functions: `MPI_Iprobe` *et al.*

What happens when a processor calls `MPI_Send` while no other processor is calling `MPI_Recv`? Everything enters an active wait loop, making no progress and using 100% CPU.

How does one run this example on several machines? See example "Coordination" below.

What happens to rank and and size when a process crashes? By default, `mpiexec` just stops everything.

Ring
----

[Source code](ring).

The goal of this example is for me to write something not completely trivial using the most basic API.

MPI is *very* flexible: it only provides a way to send individual messages from a processor to an other.
No request-response, no dataflow (like UDP, not like TCP), no RPC, nothing that gives structure to the exchange.
It's low-level, and thus both very simple and sometimes too generic and flexible.

Termination conditions for this example are a bit subtle: processor 0 must stop after it *receives* the last message from the last processor, but all other processors must stop after *sending* the last message.
This ring feels like a pattern that could be abstracted in a library, with others.

MPI patterns: see "Resources" above.

Programs that use the `MPI_Send`/`MPI_Recv` API **have** to use their rank to specialize their behavior.
True per-to-peer behavior is not possible using these API.

Coordination
------------

[Source code](coordination).

Goals:

- write another example myself
- try and encapsulate the communication API in a more functional style
- demonstrate how the same binary can be run on several machines and interact with each other (simulated using `docker-compose`)

When running this example, you can see '*e.g.* using `htop` that `coordination` processes run each in a separate Docker container.

This method requires `ssh` access between the containers.
This is unusual to say the least.

@todo Investigate other methods, especially methods using SLURM or Kubernetes.

No wrappers
-----------

[Source code](no-wrappers).

Goal: build and execute an MPI program without using MPI wrappers `mpicc` and `mpiexec`.

Building without `mpicc` or `mpic++` is quite easy, using `gcc` of `g++` with the usual `pkg-config`.
Building with `nvcc` should be easy as well.

Running without `mpiexec` on the other hand, is not going to be trivial.
The program `hello.cpp` in this example displays the command line and the environment variables that are set by `mpiexec`.
In that case, all command lines are the same.
But the environment variables are more complex: about 50 variables, most of them prefixed with `OMPI_` or `PMIX_` are added.
There is little hope in running MPI programs without `mpiexec`.

Collide 1D
----------

[Source code](collide-1d)

Goals:
- write something myself
- use dynamic point-to-point communications (`MPI_Probe`, `MPI_Get_count`)
- investigate how to test an MPI application (@todo)
- use `MPI_Allreduce`, a more advanced MPI API (not an initial goal, but the best solution for an actual requirement of this example)

This example (tries to) implement a fixed-time-increment simulation of discs moving on a rail.

It started out as a discrete-event simulation, but distributing this kind of simulation is a research domain on its own, so not well suited for a quick example focused on learning MPI!
