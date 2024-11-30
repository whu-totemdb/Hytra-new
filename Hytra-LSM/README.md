# Inverted_LSM

This project is the hybrid storage engine of Hytra, extending the idea of LSM-tree. Data will be stored in memory  and external storage.


## Compile and Build Executable Programs

1. Install Cmake.

2. Use the `cmake -S. -Bbuild` command to compile this project and generate a `build` directory.

3. Use the `cmake --build build` command to generate executable files, and generate executable files in the `build` directory.
   

## Debug and Run

1. Create folder `index_file` in project root path.

2. Referring to the structure of the `CMakeLists.txt` file, there are a total of 2 executable programs named test_insert_operation and Inverted_LSM respectively.

   * `test_insert_operation` is a test program that reads data into LSM-tree and builds an index structure.

   * `Inverted_LSM` is used to start the socket program and expose the entry to localhost:9200 for users to execute queries. You can refer to the writing method of `client_test.py`.

3. When running in the IDE, please note that the working directory of the project (Working Directory) needs to be set to the root directory of the project, otherwise the index file will not be created under the specified `index_file` path.

4. To run on an ARM architecture machine, please uncomment the `set(CMAKE_SYSTEM_PROCESSOR arm)` line in the `CMakeLists.txt` file.


## Implementation Details

The function of test_insert_operation is to provide the data in the existing file (`operation/put.config`) to be read into LSM-tree and stored in a hierarchical index structure.
The data on the hard disk is placed in the `index_file` directory. During the process of continuously inserting data, the data will be automatically merged and stored in the next layer.


## Transfer Protocol

This project uses socket as the communication method. Supported statements use the imperative style and include operations such as insert (including delete and update), get, LSM-tree status, update new configuration, and disconnect.


## PUT Operation

The inserted data is passed in in the format of `put:[key],[value]`. When parsing, the key value is determined according to the position of the colon and comma and updated to the LSM-tree. The LSM-tree on the shard of the day it was placed.
Return OK/ERR, which needs to be actively read and obtained.


## GET Operation

The acquired data is passed in in the format of `get:[date],[key]`. When parsing, the position of the key is also judged according to the position of the colon, and the data is queried from the LSM-tree and the result array is returned. What is obtained is the data on the date shard of the current day.
Return a row of specific values, separated by commas.


## STATUS Operation

Use the `status` command to obtain the status of the LSM-tree. The bottom layer will obtain the memory layer and the hard disk layer respectively to obtain the current data quantity and return the results to the user in the format of [level0]: [count0], [level1]: [count1]... . 

Returns an array of storage states, separated by commas.


## CONFIG Operation

To update the configuration information of the LSM-tree, use `config:[date],[path]`. Since the LSM-tree reads the configuration first to generate a tree with a specific structure and then stores the data, updating the configuration will update the LSM-tree on the specified time slice Data cleared. It needs to be combined with the PUT command to put the data back in.
Return OK/ERR, which needs to be actively read and obtained.

