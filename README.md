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
3. Optionally (but highly recommended), create a python virtual environment (you can install virtualenv with `pip3.10 install virtualenv`).

   ```
   python3.10 -m venv venv
   source venv/bin/activate
   ```
   Install jupyter if you don't already have it, `pip3.10 install jupyter`
4. In the `candydemo` folder, run:

   ```
   cmake -B build .
   cmake --build build
   ```
5. Copy the file [`CANDY_demo.ipynb`](https://github.com/CANDY-NSF/candydemo/blob/master/CANDY_demo.ipynb) to the `candydemo/build` directory, e.g. `cp ../CANDY_demo.ipynb .`
6. In `candydemo/build`, run

   ```
   jupyter-notebook
   ```
7. The previous step will automatically open a Jupyter notebook on your favorite browser and you can see the 	`CANDY_demo.ipynb` file  in file navigator
8. From the navigator window on jupyter notebook you can launch the  `CANDY_demo.ipynb`  and run the candy library, more instruction about the candy library are  provided in the `CANDY_demo.ipynb` file
