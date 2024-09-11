# pragma once
/* #pragma once 是一个预处理器指令，它的主要作用是让头文件（.h 文件）只被包含一次。当编译器遇到这个指令时，它会检查当前文件是否已经被包含过，如果已经包含则不会再重复包含，这样可以避免因多次包含同一头文件而导致的定义重复问题。
这个特性不是标准C的一部分，但在很多现代编译器中（如 GCC, Clang 和 MSVC）都有支持。使用 #pragma once 的优点包括简化代码、提高编译速度等。不过，在某些情况下，为了保证代码的兼容性和可移植性，开发者可能会选择传统的包含保护（include guards）方法来替代 #pragma once。*/

enum class EDirection
{
	Front, Back, Left, Right, Top, Bottom
};

enum class EBlock
{
	Null, Air, Stone, Grass, Dirt, Sand, Water, Lava
};