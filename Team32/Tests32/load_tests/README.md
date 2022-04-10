This directory contains test source and queries that have been generated for load/stress testing. The generation script used can be found in Tests32/scripts. 

- LoadTest_1 contains 100 select BOOLEAN queries with 10 clauses each that all returns FALSE, and serves to show that generated queries do not have syntactic errors

- LoadTest_2, LoadTest_3 and LoadTestNextAffects are meant only for checking of execution time and the presence of infinite loops. The evaluation outputs are not checked for correctness. 

- LoadTest_4 contains ~100 select BOOLEAN / tuple queries with 10 clauses each. Each query returns either TRUE or a non-empty list. Only the evaluated answers from this set are checked for correctness. 