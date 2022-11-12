## CANDY Demo

### Prerequisites

* Python 3.10
* CMake (version 3.13 or above is recommended)

### Getting started
To run this demo on your own machine, follow these steps.

1. Clone this repository to your machine: 
```
git clone https://github.com/CANDY-NSF/candydemo.git
```

2. Navigate to candydemo directory: `cd candydemo`

3. In the `candydemo` folder, run: 
```
cmake -B build .
cmake --build build
```

4. Copy the file [`CANDY_demo.ipynb`](https://github.com/CANDY-NSF/candydemo/blob/master/CANDY_demo.ipynb) to the `candydemo/build` directory, e.g. `cp ../CANDY_demo.ipynb .`

5. From the build directory you can open jupyter-notebook by typing `jupyter-notebook`

6. Step 5 will open a jupyter notbook on your favorite browser and you can see the CANDY_demo.ipynb file  in file navigator

7. From the navigator window on jupyter notebook you can launch the  CANDY_demo.ipynb  and run the candy library, more instruction about the candy library are  provided in the `CANDY_demo.ipynb` file
