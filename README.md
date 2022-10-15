# Efficient-and-Effecive-algorithms-for-generalized-densest-subgraph-discovery


# Densest subgraphs discovery
 
This is the code for experiments on the paper *Efficient and Effective Algorithms for Generalized Densest Subgraph Discovery*. Experiments on different kinds of graphs are performed on different cpp files. The file __experiment.cpp__ is for original density; __experiment2.cpp__ is for weighted density; __experiment3.cpp__ is for denominator weighted density. In each of the three mentioned file, functions __berkleyexp2__, __berkleyexp3__, __soda22exp2__, __soda22exp3__, __soda22exp4__ correspond to __cCoreExact__, __FlowExact__, __cCoreApp__\*, __FlowApp__* and __FlowApp__ respectively.
 
## Run algorithms
 
### Installing
Use git to clone the repo.
 
### Data preparation
You can download datasets mentioned in our paper. Their links are also provided. Or you can find a dataset by yourself. We also provide multiple functions to help you preprocess the dataset so that its format will align with our implementations.
 
### Compiling and running
We use -O2 optimization as an example. Feel free to change functions run to test different experiments. 
```
g++ -O2 experiment.cpp experiment2.cpp berkley.cpp DALKS.cpp dinic.cpp soda22.cpp greedypp.cpp -o experiment
```
Now, you can run the executables. 

```
./experiment
```

 




