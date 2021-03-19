# Example 3 - Use XIIDM extensions

This example shows how to create a cmake project that uses XIIDM extensions. In this example, we use the `EurostagNetworkFactory` to create a network. Then, we create an `ActivePowerRange` extension, attached to the generator `GEN`. Before exporting the network to an XIIDM file, we register its serializer.

## Build
To build this example, you have to build the project passing the `-DBUILD_EXAMPLES=ON` flag to the configure command. Follow the [build instructions](../../README.md#build-from-sources)

```
$> cmake --build .
[ 49%] Built target iidm
[ 52%] Built target ext-iidm-static
[100%] Built target iidm-static
[100%] Building CXX object examples/example3/CMakeFiles/example3.dir/example3.cpp.o
[100%] Linking CXX executable example3
[100%] Built target example3
```

Note that in this example, we link with the static libraries but you can also link with the dynamic libraries. To do so, you have to change the `CMakeLists.txt` file:
```
target_link_libraries(example3 PRIVATE iidm ext-iidm)
```

## Run the program
You can run the program we just compile, running the following command:
```
$> ./example3
Usage: ./example3 <output-file>
```
