This repo tracks my learning of MPI (Message Passing Interface).
It contains a list of resources I read, and examples I wrote while discovering MPI.

Resources
=========

- The [Wikipedia article about MPI](https://en.wikipedia.org/wiki/Message_Passing_Interface)
- [OpenMPI 4.1 documentation](https://www.open-mpi.org/doc/v4.1/)
- The [Wikipedia article about Flynn's taxonomy](https://en.wikipedia.org/wiki/Flynn%27s_taxonomy)
- This [MPI tutorial](https://mpitutorial.com), quite complete, easy to read
- This [presentation about MPI patterns](https://cs.gmu.edu/~kauffman/cs499/comm-patterns.pdf) (page 7+, **fundamental reading**), which references [these notes on MPI](http://www.mathcs.emory.edu/~cheung/Courses/355/Syllabus/92-MPI/group-comm.html)

Memento
=======

<!--
All MPI API from https://www.open-mpi.org/doc/v4.1/, grouped by concept
@todo Finish grouping

Abort
Accumulate

Add_error_class
Add_error_code
Add_error_string

Address
Aint_add
Aint_diff
Alloc_mem

Allgather
Allgatherv
Allreduce
Alltoall
Alltoallv
Alltoallw

Attr_delete
Attr_get
Attr_put

Barrier

Bcast

Bsend
Bsend_init
Buffer_attach
Buffer_detach

Cancel

Cart_coords
Cart_create
Cart_get
Cart_map
Cart_rank
Cart_shift
Cart_sub

Cartdim_get

Close_port

Comm_accept
Comm_c2f
Comm_call_errhandler
Comm_compare
Comm_connect
Comm_create
Comm_create_errhandler
Comm_create_group
Comm_create_keyval
Comm_delete_attr
Comm_disconnect
Comm_dup
Comm_dup_with_info
Comm_f2c
Comm_free
Comm_free_keyval
Comm_get_attr
Comm_get_errhandler
Comm_get_info
Comm_get_name
Comm_get_parent
Comm_group
Comm_idup
Comm_join
Comm_rank
Comm_remote_group
Comm_remote_size
Comm_set_attr
Comm_set_errhandler
Comm_set_info
Comm_set_name
Comm_size
Comm_spawn
Comm_spawn_multiple
Comm_split
Comm_split_type
Comm_test_inter

Compare_and_swap

Dims_create

Dist_graph_create
Dist_graph_create_adjacent
Dist_graph_neighbors
Dist_graph_neighbors_count

Errhandler_create
Errhandler_free
Errhandler_get
Errhandler_set
Error_class
Error_string

Exscan

Fetch_and_op

File_c2f
File_call_errhandler
File_close
File_create_errhandler
File_delete
File_f2c
File_get_amode
File_get_atomicity
File_get_byte_offset
File_get_errhandler
File_get_group
File_get_info
File_get_position
File_get_position_shared
File_get_size
File_get_type_extent
File_get_view
File_iread
File_iread_all
File_iread_at
File_iread_at_all
File_iread_shared
File_iwrite
File_iwrite_all
File_iwrite_at
File_iwrite_at_all
File_iwrite_shared
File_open
File_preallocate
File_read
File_read_all
File_read_all_begin
File_read_all_end
File_read_at
File_read_at_all
File_read_at_all_begin
File_read_at_all_end
File_read_ordered
File_read_ordered_begin
File_read_ordered_end
File_read_shared
File_seek
File_seek_shared
File_set_atomicity
File_set_errhandler
File_set_info
File_set_size
File_set_view
File_sync
File_write
File_write_all
File_write_all_begin
File_write_all_end
File_write_at
File_write_at_all
File_write_at_all_begin
File_write_at_all_end
File_write_ordered
File_write_ordered_begin
File_write_ordered_end
File_write_shared

Finalize
Finalized

Free_mem

Gather
Gatherv

Get
Get_accumulate
Get_address
Get_count
Get_elements
Get_elements_x
Get_library_version
Get_processor_name
Get_version

Graph_create
Graph_get
Graph_map
Graph_neighbors
Graph_neighbors_count

Graphdims_get

Grequest_complete
Grequest_start

Group_c2f
Group_compare
Group_difference
Group_excl
Group_f2c
Group_free
Group_incl
Group_intersection
Group_range_excl
Group_range_incl
Group_rank
Group_size
Group_translate_ranks
Group_union

Iallgather
Iallgatherv
Iallreduce
Ialltoall
Ialltoallv
Ialltoallw
Ibarrier
Ibcast
Ibsend
Iexscan
Igather
Igatherv
Improbe
Imrecv
Ineighbor_allgather
Ineighbor_allgatherv
Ineighbor_alltoall
Ineighbor_alltoallv
Ineighbor_alltoallw
Iprobe
Irecv
Ireduce
Ireduce_scatter
Ireduce_scatter_block
Irsend
Iscan
Iscatter
Iscatterv
Isend
Issend

Info_c2f
Info_create
Info_delete
Info_dup
Info_env
Info_f2c
Info_free
Info_get
Info_get_nkeys
Info_get_nthkey
Info_get_valuelen
Info_set

Init
Init_thread
Initialized

Intercomm_create
Intercomm_merge

Is_thread_main

Keyval_create
Keyval_free

Lookup_name

Message_c2f
Message_f2c

Mprobe
Mrecv

Neighbor_allgather
Neighbor_allgatherv
Neighbor_alltoall
Neighbor_alltoallv
Neighbor_alltoallw

Op_c2f
Op_commutative
Op_create
Op_f2c
Op_free

Open_port

Pack
Pack_external
Pack_external_size
Pack_size

Pcontrol

Probe

Publish_name

Put

Query_thread

Raccumulate

Recv
Recv_init

Reduce
Reduce_local
Reduce_scatter
Reduce_scatter_block

Register_datarep

Request_c2f
Request_f2c
Request_free
Request_get_status

Rget
Rget_accumulate
Rput
Rsend
Rsend_init

Scan
Scatter
Scatterv
Send
Send_init
Sendrecv
Sendrecv_replace

Sizeof

Ssend
Ssend_init

Start
Startall

Status_c2f
Status_f2c
Status_set_cancelled
Status_set_elements
Status_set_elements_x

T_category_changed
T_category_get_categories
T_category_get_cvars
T_category_get_info
T_category_get_num
T_category_get_pvars
T_cvar_get_info
T_cvar_get_num
T_cvar_handle_alloc
T_cvar_handle_free
T_cvar_read
T_cvar_write
T_enum_get_info
T_enum_get_item
T_finalize
T_init_thread
T_pvar_get_info
T_pvar_get_num
T_pvar_handle_alloc
T_pvar_handle_free
T_pvar_read
T_pvar_readreset
T_pvar_reset
T_pvar_session_create
T_pvar_session_free
T_pvar_start
T_pvar_stop
T_pvar_write

Test
Test_cancelled
Testall
Testany
Testsome

Topo_test

Type_c2f
Type_commit
Type_contiguous
Type_create_darray
Type_create_f90_complex
Type_create_f90_integer
Type_create_f90_real
Type_create_hindexed
Type_create_hindexed_block
Type_create_hvector
Type_create_indexed_block
Type_create_keyval
Type_create_resized
Type_create_struct
Type_create_subarray
Type_delete_attr
Type_dup
Type_extent
Type_f2c
Type_free
Type_free_keyval
Type_get_attr
Type_get_contents
Type_get_envelope
Type_get_extent
Type_get_extent_x
Type_get_name
Type_get_true_extent
Type_get_true_extent_x
Type_hindexed
Type_hvector
Type_indexed
Type_lb
Type_match_size
Type_set_attr
Type_set_name
Type_size
Type_size_x
Type_struct
Type_ub
Type_vector

Unpack
Unpack_external

Unpublish_name

Wait
Waitall
Waitany
Waitsome

Win_allocate
Win_allocate_shared
Win_attach
Win_c2f
Win_call_errhandler
Win_complete
Win_create
Win_create_dynamic
Win_create_errhandler
Win_create_keyval
Win_delete_attr
Win_detach
Win_f2c
Win_fence
Win_flush
Win_flush_all
Win_flush_local
Win_flush_local_all
Win_free
Win_free_keyval
Win_get_attr
Win_get_errhandler
Win_get_group
Win_get_info
Win_get_name
Win_lock
Win_lock_all
Win_post
Win_set_attr
Win_set_errhandler
Win_set_info
Win_set_name
Win_shared_query
Win_start
Win_sync
Win_test
Win_unlock
Win_unlock_all
Win_wait

Wtick
Wtime

-->

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

## Processor

A [process](https://en.wikipedia.org/wiki/Process_(computing)) using the MPI infrastructure.

## Communicator

@todo Define

## Type

@todo Define

Synchronization API
-------------------

`MPI_Barrier` implements the usual barrier synchronization: each processor waits for all other processors to have readched the barrier

Communication API
-----------------

These API, and a bit more, are demonstrated in the [memento](memento) example.

Legend:
- ⬛: uninitialized data
- 🟪🟦🟩🟨🟧🟥: initialized data
- 3️⃣: size of data

| Purpose | Processor | Before | Action | After |
| --- | --- | --- | --- | --- |
| One-to-one | 0<br>1 | 🟩<br>⬛ | `Send(🟩, 1)`<br>`Recv(⬛, 0)` | 🟩<br>🟩 |
| One-to-one,<br>dynamic size | 0<br><br>1<br><br>&nbsp; | 🟩<br><br>-<br>-<br>⬛ | `Send(🟩, 1)`<br><br>`Probe(0)`, `Get_count`<br>allocate<br>`Recv(⬛, 0)` | 🟩<br><br>-<br>⬛<br>🟩 |
| One-to-all,<br>same data | 0<br>1<br>2 | 🟩<br>⬛<br>⬛ | `Bcast(🟩, 0)`<br>`Bcast(⬛, 0)`<br>`Bcast(⬛, 0)` | 🟩<br>🟩<br>🟩 |
| One-to-all,<br>same data,<br>dynamic size | 0<br><br><br>N<br><br>&nbsp; | 3️⃣, 🟩🟪🟧<br>3️⃣, 🟩🟪🟧<br><br>⬛<br>3️⃣<br>3️⃣, ⬛⬛⬛ | `Bcast(3️⃣, 0)`<br>`Bcast(🟩🟪🟧, 0)`<br><br>`Bcast(⬛, 0)`<br>allocate<br>`Bcast(⬛⬛⬛, 0)` | 3️⃣, 🟩🟪🟧<br>3️⃣, 🟩🟪🟧<br><br>3️⃣<br>3️⃣, ⬛⬛⬛<br>3️⃣, 🟩🟪🟧 |
| One-to-all,<br>different data | 0<br>1<br>2 | ⬛, 🟩🟪🟧<br>⬛<br>⬛ | `Scatter(0)`<br>`Scatter(0)`<br>`Scatter(0)` | 🟩, 🟩🟪🟧<br>🟪<br>🟧 |
| All-to-one | 0<br>1<br>2 | 🟩, ⬛⬛⬛<br>🟪<br>🟧 | `Gather(0)`<br>`Gather(0)`<br>`Gather(0)` | 🟩, 🟩🟪🟧<br>🟪<br>🟧 |
| All-to-all | 0<br>1<br>2 | 🟩, ⬛⬛⬛<br>🟪, ⬛⬛⬛<br>🟧, ⬛⬛⬛ | `Allgather(0)`<br>`Allgather(0)`<br>`Allgather(0)` | 🟩, 🟩🟪🟧<br>🟪, 🟩🟪🟧<br>🟧, 🟩🟪🟧 |
| All-to-one,<br>reduction | 0<br>1<br>2 | 🟩, ⬛<br>🟪<br>🟧 | `Reduce(0)`<br>`Reduce(0)`<br>`Reduce(0)` | 🟩, 🟥<br>🟪<br>🟧 |
| All-to-one,<br>reduction,<br>in place | 0<br>1<br>2 | 🟩<br>🟪<br>🟧 | `Reduce(0, IN_PLACE)`<br>`Reduce(0, IN_PLACE)`<br>`Reduce(0, IN_PLACE)` | 🟥<br>🟪<br>🟧 |
| All-to-all,<br>reduction | 0<br>1<br>2 | 🟩, ⬛<br>🟪, ⬛<br>🟧, ⬛ | `Allreduce()`<br>`Allreduce()`<br>`Allreduce()` | 🟩, 🟥<br>🟪, 🟥<br>🟧, 🟥 |

@todo Add `Sendrecv`, `Accumulate`, [`Alltoall`](https://stackoverflow.com/questions/15049190/difference-between-mpi-allgather-and-mpi-alltoall-functions), `Scan`, `Exscan`, `Reduce_scatter`, `Fetch_and_op`

@todo Add prefix variants: `I` (non-blocking), `B` (buffered), `M` (matched), `neighbor`

@todo Add suffix variants: `v`, `w`

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
- Each process gets a "rank", using `MPI_Comm_rank`, and knows how many processes there are in total, using `MPI_Comm_size`. This requires that `mpiexec` does all bookkeeping to let these processes, and only them, communicate.
- Each process then chooses its role based on its rank, à la `fork`.
- `MPI_Send` is used to send a message to another process, identified by its rank.
- `MPI_Recv` is used to receive a message from another process, also identified by its rank.

@todo Is there a higher level of abstraction than rank to choose the role of each process?

@todo Can binaries built from different source code communicate? (*e.g.* a coordinator and several workers)

Non-blocking "send" and "receive" functions: `MPI_Iprobe` *et al.*

What happens when a process calls `MPI_Send` while no other process is calling `MPI_Recv`? Everything enters an active wait loop, making no progress and using 100% CPU.

How does one run this example on several machines? See example "Coordination" below.

What happens to rank and and size when a process crashes? By default, `mpiexec` just stops everything.

Ring
----

[Source code](ring).

The goal of this example is for me to write something not completely trivial using the most basic API.

MPI is *very* flexible: it only provides a way to send individual messages from a process to an other.
No request-response, no dataflow (like UDP, not like TCP), no RPC, nothing that gives structure to the exchange.
It's low-level, and thus both very simple and sometimes too generic and flexible.

Termination conditions for this example are a bit subtle: process 0 must stop after it *receives* the last message from the last process, but all other processes must stop after *sending* the last message.
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
