# Algorithms and data structures project for the course "Algoritmi e principi per l'informatica" of Politecnico di Milano, a.y. 2018/2019
The project requires the design and development of a correct and efficient C program. Only the libc is allowed. Any kind of parallelism is forbidden.
The main goal of the project is to create a mechanism able to monitor entities and directed relations between entities.
In particular, the algorithm has to interpret a list of input commands, compute them by a set of efficient data structures and produce a string as output.
The input commands are:
1. addent ⟨id_ent⟩ //create a new entity and add it to the monitored ones. If it already exists, skip.
2. delent ⟨ident⟩id_ent⟩ //delete a monitored entity and all the relations incoming and outgoing from it. If the specified entity is not monitored, skip.
3. addrel ⟨id_orig⟩ ⟨id_dest⟩ ⟨id_rel⟩ // create a new relation with the id_orig as the starting entity and id_dest as the destinatary entity with a id_rel label. If the relation already exists, skip.
4. delrel ⟨id_orig⟩ ⟨id_dest⟩ ⟨id_rel⟩ // delete an existing relation with the id_orig as the starting entity and id_dest as the destinatary entity with a id_rel label. If the relation did not exists, skip.
5. report //prints an output string following this convention: ⟨id_rel1⟩ ⟨id_ent1⟩ ⟨n_rel1⟩;⟨id_rel2⟩ ⟨id_ent2⟩ ⟨n_rel2⟩;...  ,where id_rel indicates a relation label, id_ent is the enity with the highest number of incoming relations with label id_rel and n_rel is the number of incoming arcs. If there are more than one entities with the same number of incoming relations (max) with label id_rel, list them separated by a blank space
6. end // end of computation.

## Testing
The project required to pass some private (blind) test: the code was compiled and run on a remote server and tested with some unknown tests which stressed the code in terms of memory and computation time. If the program was able to finish its execution inside some time and memory boundaries, the test was considered as passed. 

## Tools
In order to measure the performance of the code, I used *Valgrind* for the memory consumption and *kcachecallgring* for the computation time.