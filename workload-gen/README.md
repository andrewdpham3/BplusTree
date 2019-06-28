# Harvard CS265 - Big Data Systems #
---
*This repository contains the code of a workload generator for an LSM tree.
It follows the DSL specified for the systems project of CS265 (Spring 2017).*

*More information can be found [here](http://daslab.seas.harvard.edu/classes/cs265/project.html).*

## Workload and Data Generator ##
---

### Dependencies ###
You need the GNU scientific library in order to use the generator (https://www.gnu.org/software/gsl/).

* Ubuntu Linux: ```sudo apt-get install libgsl-dev```
* Fedora Linux: ```dnf install gsl-devel```
* Mac OS X: ```brew install gsl```
* Cygwin: install the *gsl, gsl-devel* packages

### Building ###
```
cd generator;
make clean; make;
```

or simply...
```
cc generator.c -o generator -lgsl -lgslcblas
```

### Running ###
You can now run the following to see all available options:
```
./generator --help
```

![Screen Shot 2017-01-24 at 1.21.17 PM.png](https://bitbucket.org/repo/9de5E4/images/2315274117-Screen%20Shot%202017-01-24%20at%201.21.17%20PM.png)

### Examples ###

**Workload 1:** Insert 100000 keys, perform 1000 gets and 10 range queries.

```
./generator --puts 100000 --gets 1000  > workload.txt
```

**Workload 2:** Same as above but store the data in external (.dat) binary files.
```
./generator --puts 100000 --gets 1000  > workload.txt
```

**Workload 3:** Perform 100000 puts and issue 100 range queries (drawn from a uniform distribution).
```
./generator --puts 100000 --ranges 100 --uniform-ranges > workload.txt
```

**Note: You can always set the random number generator seed using --seed XXXX**


## Evaluating a Workload ##
---
You can execute a workload and see some basic statistics about it, using the ```evaluate.py``` python script.

### Dependencies ###
You need to install the [blist](https://pypi.python.org/pypi/blist/?) library.

Most platforms: ```pip install blist```

*Note: In Fedora Linux, you might need to install it using: ```dnf install python-blist```.*

### Running ###

Run as follows:
```
python evaluate.py workload.txt
```

**Note: For extra options etc, please look inside the script.**