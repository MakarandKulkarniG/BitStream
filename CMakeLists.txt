project("cryptopals challenge")

add_executable(hex2base64 hex2base64.c
	BitStream.c)

add_executable(xor xor.c
	BitStream.c)

add_executable(singlebytexor singlebytexor.c
	BitStream.c)

add_executable(detectsinglexor detectsinglexor.c
	BitStream.c)

add_executable(repeatkeyxor repeatkeyxor.c
	BitStream.c)
