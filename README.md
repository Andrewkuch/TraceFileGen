# TraceFileGen

The goal of this project is to design and develop a program that can automatically generate all types of basic memory management operations and write into trace files. Trace files can be used as further benchmarks for simulating Automatic Memory Management Systems (such as Garbage Collectors).

# Trace File

 A trace file is a collection of the following basic memory management operations typically found in executing an application.  
* Allocation
* Store
* Read

# Trace File Format and Specification

 The trace file generated by TraceFileGen has the following formats and meanings:
 
 MM Operations    											     | Notation	 | Usage
 --------------------------------------------------------------- | --------- | -----------
 Allocation 													 | a 		 | Ti Oj Sk Nl Cj
 Add a reference of an object to the rootset a thread  			 | +         | Ti Oj
 Store/write a primitive field into an object 					 | s         | Ti Oj Ix(/Fm) Sn Vo
 Store/write a static primitive field into a class object 		 | s         | Ti Cj Ix(/Fm) Sn Vo
 Store/write an object reference field into an object 			 | w         | Ti Pj \#k Ol Fm Sn Vo
 Store/write a static object reference field into a class object | c         | Ti Cj Ix(/Fm) Ol Sn Vo
 Read a primitive or a reference field from an object			 | r         | Ti Oj Ix(/Fm) Sn Vo
 Read a static primitive or reference filed from a class object  | r         | Ti Cj Ix(/Fm) Sn Vo
 Delete an object reference from the rootset of a thread 		 | -         | Ti Oj	
 
 * Ti: i is a thread id (i >= 0)
 * Oj: j is an object id (j >= 1)
 * Cj: j is a class id (j >= 1)
 * Sk: k is the size of a object in bytes (k > 0)
 * Nl: l is the no. of reference slots in an object (l >= 0)
 * Ix(/Fm): x(/m) is the index(/offset) of a filed in an object (x(/m) >= 0)
 * Sn: n is the size of a field in an object (n >= 8) 
 * Vo: o is either 0 or 1 represents the field type either non-volatile or volatile
 * Pj: j is an object (Parent) id (j >= 1)
 * Ol: l is an object (Child)  id (l >= 1)
 * \#k: k is the slot number of a reference field in an object
 
# Obtaining TraceFileGen
TraceFileGen can be downloaded from https://github.com/GarCoSim/TraceFileGen

# Required Dependencies
  TraceFileGen has the following requirements
  * CMake is required to compile the sources. CMake can be obtained from https://cmake.org/
  * g++ (GCC) 4.4.7 or newer is required to compile the sources, visit https://gcc.gnu.org/

# Overview of the Directories Structure
  The following table lists the names of all directories and a short description.
  
 Name    		| Description
 -------------- | -----------
 build   		| build folder which is created by the ./build.sh script
 ObjectClasses	| C++ header and sources
 Utils			| C++ header and sources
 Documentation	| documentation
   
# Installing (Initial Build)
  After downloading, extracting or cloning, do the following:
  * $ cd TraceFileGen
  * $ ./build.sh

# Compiling (After Initial Build)
  After initial build, do the following:
  * $ cmake ..
  * $ make
  
# Running (TraceFileGen - the executable in the build folder)
Usage: At least one argument must be provided to run TraceFileGen
* The name of trace file to be generated (such as traceFile)

Example: $./TraceFileGen traceFile

Options:

  Argument				| Short			 | Description
 ---------------------- | -------------- | -----------
 --operations x  	    | -o x			 | x is the number of total memory management operations to be created in a trace file (default: 100) 
 --thread x			    | -t x			 | x is the number of threads (default: 10) 
 --class x			    | -c x      	 | x is the number of classes (default: 300) 
 --pointers x			| -p x 			 | x is the maximum number of pointer fields in an object or static pointer fields in a class (default: 10) 
 --primitives x			| -pm x     	 | x is the maximum number of primitive fields in an object or static primitive fields in a class (default: 6) 
 --allocation x			| -a x      	 | x is the percentage of allocation in total MM operations (default: 1)* 
 --storeaccess x 		| -s x     		 | x is the percentage of store operations in total MM operations (default: 8)* 
 --readaccess x			| -r x      	 | x is the percentage of read operations in total MM operations (default: 80)* 
 --deleteroot x			| -d x      	 | x is the percentage of root delete operations in total MM operations (default: 11)*
 --static x				| -sf x     	 | x is the ratio of static field access (store/read) from a class(default: 30) 
 --prifaccess x			| -pfa x    	 | x is the ratio of primitive field access (store/read) from either a object or a class  (default: 70) 
 --escape x				| -e x      	 | x is the ratio of an object being escaped (default: 12) 
 --esctopartner x		| -etp x  		 | x is the ratio of an object being escaped to partner thread (default: 90)
	*100% distribution among them
 
# Output (After running)
	traceGen will generate three files
	* *.trace --- containing a sequence of memory management operations
	* *.cls   --- containing a lists of classes
	* *.log   --- containing statistical data, where * is the name of the trace file specified on the command line (e.g., "traceFile")

# Usage of Trace Files
	The obtained trace files can be used to simulate and analyse memory management operations by using TraceFileSim can be found in	https://github.com/GarCoSim/TraceFileSim
